#include "pch.h"
#include "DataManager.h"
#include "Common.h"
#include <vector>
#include <sstream>
#include <cstdlib>

CDataManager CDataManager::m_instance;

CDataManager::CDataManager()
{
	//初始化DPI
	HDC hDC = ::GetDC(HWND_DESKTOP);
	m_dpi = GetDeviceCaps(hDC, LOGPIXELSY);
	::ReleaseDC(HWND_DESKTOP, hDC);
}

CDataManager::~CDataManager()
{
	SaveConfig();
}

CDataManager& CDataManager::Instance()
{
	return m_instance;
}

static void WritePrivateProfileInt(const wchar_t* app_name, const wchar_t* key_name, int value, const wchar_t* file_path)
{
	wchar_t buff[16];
	swprintf_s(buff, L"%d", value);
	WritePrivateProfileString(app_name, key_name, buff, file_path);
}

void CDataManager::LoadConfig(const std::wstring& config_dir)
{
	//获取模块的路径
	HMODULE hModule = reinterpret_cast<HMODULE>(&__ImageBase);
	wchar_t path[MAX_PATH];
	GetModuleFileNameW(hModule, path, MAX_PATH);
	std::wstring module_path = path;
	m_config_path = module_path;
	if (!config_dir.empty())
	{
		size_t index = module_path.find_last_of(L"\\/");
		//模块的文件名
		std::wstring module_file_name = module_path.substr(index + 1);
		m_config_path = config_dir + module_file_name;
	}
	m_config_path += L".ini";


	//TODO: 在此添加载入配置的代码
	{
		m_setting_data.settings_altered_counter = GetPrivateProfileInt(L"config", L"settings_altered_counter", 0, m_config_path.c_str());
		m_setting_data.is_dbg_mode = (GetPrivateProfileInt(L"config", L"is_dbg_mode", 0, m_config_path.c_str()) != 0);
		m_setting_data.enable_cpu_monitor = (GetPrivateProfileInt(L"config", L"enable_cpu_monitor", 0, m_config_path.c_str()) != 0);
		m_setting_data.enable_gpu_monitor = (GetPrivateProfileInt(L"config", L"enable_gpu_monitor", 0, m_config_path.c_str()) != 0);
		m_setting_data.default_value_unit_space = GetPrivateProfileInt(L"config", L"default_value_unit_space",
			1, m_config_path.c_str());
		m_setting_data.default_max_adaptive_decimal_places = GetPrivateProfileInt(L"config",
			L"default_max_adaptive_decimal_places", 2, m_config_path.c_str());
		// Read both keys as text: GetPrivateProfileInt returns UINT, and the
		// "key missing" case has to be told apart from a legitimate value so
		// configs from the single-key (-1 = off) layout migrate correctly.
		wchar_t max_power_buf[32] = L"";
		::GetPrivateProfileString(L"config", L"max_plausible_power_w", L"",
			max_power_buf, 32, m_config_path.c_str());
		const bool has_max_power_key = max_power_buf[0] != L'\0';
		const int raw_max_power_w = _wtoi(max_power_buf);
		const bool max_power_in_range = raw_max_power_w >= 10 && raw_max_power_w <= 100000;
		m_setting_data.max_plausible_power_w = max_power_in_range ? raw_max_power_w : 1000;

		wchar_t filter_buf[16] = L"";
		::GetPrivateProfileString(L"config", L"enable_power_filter", L"",
			filter_buf, 16, m_config_path.c_str());
		if (filter_buf[0] != L'\0')
			m_setting_data.enable_power_filter = (_wtoi(filter_buf) != 0);
		else
			// Older configs only had the threshold: a valid threshold meant the
			// filter was on, while -1 (or no key at all) meant it was off.
			m_setting_data.enable_power_filter = has_max_power_key && max_power_in_range;

		m_setting_data.keep_last_valid_value =
			(GetPrivateProfileInt(L"config", L"keep_last_valid_value",
				0, m_config_path.c_str()) != 0);

		::GetPrivateProfileString(L"config", L"pwr_unit_str", L"",
			m_setting_data.pwr_unit_str.GetBuffer(UNIT_STR_MAXLEN + 1),
			UNIT_STR_MAXLEN, m_config_path.c_str());
		::GetPrivateProfileString(L"config", L"electric_capacity_unit_str", L"",
			m_setting_data.electric_capacity_unit_str.GetBuffer(UNIT_STR_MAXLEN + 1),
			UNIT_STR_MAXLEN, m_config_path.c_str());

		::GetPrivateProfileString(L"config", L"electric_voltage_unit_str", L"",
			m_setting_data.electric_voltage_unit_str.GetBuffer(UNIT_STR_MAXLEN + 1),
			UNIT_STR_MAXLEN, m_config_path.c_str());
		::GetPrivateProfileString(L"config", L"hour_unit_str", L"",
			m_setting_data.hour_unit_str.GetBuffer(UNIT_STR_MAXLEN + 1),
			UNIT_STR_MAXLEN, m_config_path.c_str());
		::GetPrivateProfileString(L"config", L"minute_unit_str", L"",
			m_setting_data.minute_unit_str.GetBuffer(UNIT_STR_MAXLEN + 1),
			UNIT_STR_MAXLEN, m_config_path.c_str());

		::GetPrivateProfileString(L"config", L"nan_str", L"",
			m_setting_data.nan_str.GetBuffer(UNIT_STR_MAXLEN + 1),
			UNIT_STR_MAXLEN, m_config_path.c_str());

		bool isFirstInital = FirstInitalCheck(m_setting_data), altered_flag = false;
		if (isFirstInital) {

			altered_flag = true;

#if WINRT_USE_FLAG
			m_setting_data.enable_cpu_monitor = 1;
			m_setting_data.enable_gpu_monitor = 1;

#endif
		}

		if (isFirstInital ||
			m_setting_data.pwr_unit_str.GetLength() >= UNIT_STR_MAXLEN) {
			m_setting_data.pwr_unit_str = "W";
			altered_flag = true;
		}
		if (isFirstInital ||
			m_setting_data.electric_capacity_unit_str.GetLength() >= UNIT_STR_MAXLEN) {
			m_setting_data.electric_capacity_unit_str = "Wh";
			altered_flag = true;
		}
		if (isFirstInital ||
			m_setting_data.electric_voltage_unit_str.GetLength() >= UNIT_STR_MAXLEN) {
			m_setting_data.electric_voltage_unit_str = "V";
			altered_flag = true;
		}
		if (isFirstInital ||
			m_setting_data.hour_unit_str.GetLength() >= UNIT_STR_MAXLEN) {
			m_setting_data.hour_unit_str = "H";
			altered_flag = true;
		}
		if (isFirstInital ||
			m_setting_data.minute_unit_str.GetLength() >= UNIT_STR_MAXLEN) {
			m_setting_data.minute_unit_str = "M";
			altered_flag = true;
		}

		if (isFirstInital ||
			m_setting_data.nan_str.GetLength() >= UNIT_STR_MAXLEN) {
			m_setting_data.nan_str = "NaN";
			altered_flag = true;
		}

		if (altered_flag) {
			m_setting_data.settings_altered_counter += 1;
			SaveConfig();
		}
	}
}



void CDataManager::SaveConfig() const
{
	if (!m_config_path.empty())
	{
		//TODO: 在此添加保存配置的代码 
		WritePrivateProfileInt(L"config", L"settings_altered_counter",
			(m_setting_data.settings_altered_counter), m_config_path.c_str());
		WritePrivateProfileInt(L"config", L"is_dbg_mode",
			(m_setting_data.is_dbg_mode), m_config_path.c_str());
		WritePrivateProfileInt(L"config", L"enable_cpu_monitor",
			(m_setting_data.enable_cpu_monitor), m_config_path.c_str());
		WritePrivateProfileInt(L"config", L"enable_gpu_monitor",
			(m_setting_data.enable_gpu_monitor), m_config_path.c_str());
		WritePrivateProfileInt(L"config", L"default_value_unit_space",
			(m_setting_data.default_value_unit_space), m_config_path.c_str());
		WritePrivateProfileInt(L"config", L"default_max_adaptive_decimal_places",
			(m_setting_data.default_max_adaptive_decimal_places), m_config_path.c_str());
		WritePrivateProfileInt(L"config", L"enable_power_filter",
			(int)(m_setting_data.enable_power_filter), m_config_path.c_str());
		WritePrivateProfileInt(L"config", L"max_plausible_power_w",
			(int)(m_setting_data.max_plausible_power_w), m_config_path.c_str());
		WritePrivateProfileInt(L"config", L"keep_last_valid_value",
			(int)(m_setting_data.keep_last_valid_value), m_config_path.c_str());

		WritePrivateProfileString(L"config", L"pwr_unit_str",
			m_setting_data.pwr_unit_str, m_config_path.c_str());
		WritePrivateProfileString(L"config", L"electric_capacity_unit_str",
			m_setting_data.electric_capacity_unit_str, m_config_path.c_str());
		WritePrivateProfileString(L"config", L"electric_voltage_unit_str",
			m_setting_data.electric_voltage_unit_str, m_config_path.c_str());
		WritePrivateProfileString(L"config", L"hour_unit_str",
			m_setting_data.hour_unit_str, m_config_path.c_str());
		WritePrivateProfileString(L"config", L"minute_unit_str",
			m_setting_data.minute_unit_str, m_config_path.c_str());
		WritePrivateProfileString(L"config", L"nan_str",
			m_setting_data.nan_str, m_config_path.c_str());
	}
}

bool CDataManager::FirstInitalCheck(SettingData setting_data) {

	return setting_data.settings_altered_counter <= 0;
}

const CString& CDataManager::StringRes(UINT id)
{
	auto iter = m_string_table.find(id);
	if (iter != m_string_table.end())
	{
		return iter->second;
	}
	else
	{
		AFX_MANAGE_STATE(AfxGetStaticModuleState());
		m_string_table[id].LoadString(id);
		return m_string_table[id];
	}
}

void CDataManager::DPIFromWindow(CWnd* pWnd)
{
	CWindowDC dc(pWnd);
	HDC hDC = dc.GetSafeHdc();
	m_dpi = GetDeviceCaps(hDC, LOGPIXELSY);
}

int CDataManager::DPI(int pixel)
{
	return m_dpi * pixel / 96;
}

float CDataManager::DPIF(float pixel)
{
	return m_dpi * pixel / 96;
}

int CDataManager::RDPI(int pixel)
{
	return pixel * 96 / m_dpi;
}

HICON CDataManager::GetIcon(UINT id)
{
	auto iter = m_icons.find(id);
	if (iter != m_icons.end())
	{
		return iter->second;
	}
	else
	{
		AFX_MANAGE_STATE(AfxGetStaticModuleState());
		HICON hIcon = (HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(id), IMAGE_ICON, DPI(16), DPI(16), 0);
		m_icons[id] = hIcon;
		return hIcon;
	}
}
