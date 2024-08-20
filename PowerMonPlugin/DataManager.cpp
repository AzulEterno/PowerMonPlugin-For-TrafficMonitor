#include "pch.h"
#include "DataManager.h"
#include "Common.h"
#include <vector>
#include <sstream>

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
		m_setting_data.settings_altered_counter = (GetPrivateProfileInt(L"config", L"settings_altered_counter", 0, m_config_path.c_str()) != 0);
		m_setting_data.is_dbg_mode = (GetPrivateProfileInt(L"config", L"is_dbg_mode", 0, m_config_path.c_str()) != 0);
		m_setting_data.enable_cpu_monitor = (GetPrivateProfileInt(L"config", L"enable_cpu_monitor", 0, m_config_path.c_str()) != 0);
		m_setting_data.enable_gpu_monitor = (GetPrivateProfileInt(L"config", L"enable_gpu_monitor", 0, m_config_path.c_str()) != 0);

		::GetPrivateProfileString(L"config", L"pwr_unit_str", L"",
			m_setting_data.pwr_unit_str.GetBuffer(PWR_UNIT_STR_MAXLEN + 1),
			PWR_UNIT_STR_MAXLEN, m_config_path.c_str());
		::GetPrivateProfileString(L"config", L"electric_capacity_unit_str", L"",
			m_setting_data.electric_capacity_unit_str.GetBuffer(PWR_UNIT_STR_MAXLEN + 1),
			PWR_UNIT_STR_MAXLEN, m_config_path.c_str());

		::GetPrivateProfileString(L"config", L"electric_voltage_unit_str", L"",
			m_setting_data.electric_voltage_unit_str.GetBuffer(PWR_UNIT_STR_MAXLEN + 1),
			PWR_UNIT_STR_MAXLEN, m_config_path.c_str());
		::GetPrivateProfileString(L"config", L"hour_unit_str", L"",
			m_setting_data.hour_unit_str.GetBuffer(PWR_UNIT_STR_MAXLEN + 1),
			PWR_UNIT_STR_MAXLEN, m_config_path.c_str());
		::GetPrivateProfileString(L"config", L"minute_unit_str", L"",
			m_setting_data.minute_unit_str.GetBuffer(PWR_UNIT_STR_MAXLEN + 1),
			PWR_UNIT_STR_MAXLEN, m_config_path.c_str());

		bool isFirstInital = FirstInitalCheck(m_setting_data), altered_flag = false;
		if (isFirstInital) {

			altered_flag = true;
		}

		if (isFirstInital ||
			m_setting_data.pwr_unit_str.GetLength() >= PWR_UNIT_STR_MAXLEN) {
			m_setting_data.pwr_unit_str = "W";
			altered_flag = true;
		}
		if (isFirstInital ||
			m_setting_data.electric_capacity_unit_str.GetLength() >= PWR_UNIT_STR_MAXLEN) {
			m_setting_data.electric_capacity_unit_str = "Wh";
			altered_flag = true;
		}
		if (isFirstInital ||
			m_setting_data.electric_voltage_unit_str.GetLength() >= PWR_UNIT_STR_MAXLEN) {
			m_setting_data.electric_voltage_unit_str = "V";
			altered_flag = true;
		}
		if (isFirstInital ||
			m_setting_data.hour_unit_str.GetLength() >= PWR_UNIT_STR_MAXLEN) {
			m_setting_data.hour_unit_str = "H";
			altered_flag = true;
		}
		if (isFirstInital ||
			m_setting_data.minute_unit_str.GetLength() >= PWR_UNIT_STR_MAXLEN) {
			m_setting_data.minute_unit_str = "M";
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
