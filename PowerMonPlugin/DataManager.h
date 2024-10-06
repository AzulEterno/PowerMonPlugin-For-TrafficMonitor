#pragma once
#include "pch.h"

#define g_data CDataManager::Instance()


struct SettingData
{
	//TODO: 在此添加选项设置的数据

	INT64 settings_altered_counter = 0;
	bool is_dbg_mode = false;
	bool enable_gpu_monitor = false;
	bool enable_cpu_monitor = false;
	INT64 default_value_unit_space = 1;
	INT64 default_adaptive_decimal_places = 2;
	CString pwr_unit_str{};
	CString electric_capacity_unit_str{};
	CString electric_voltage_unit_str{};
	CString hour_unit_str{};
	CString minute_unit_str{};
	// Assignment operator
	SettingData& operator=(const SettingData& other) {
		if (this == &other) {
			return *this; // Handle self-assignment
		}

		// Copy primitive types
		this->settings_altered_counter = other.settings_altered_counter;
		this->is_dbg_mode = other.is_dbg_mode;
		this->enable_gpu_monitor = other.enable_gpu_monitor;
		this->enable_cpu_monitor = other.enable_cpu_monitor;
		this->default_value_unit_space = other.default_value_unit_space;
		this->default_adaptive_decimal_places = other.default_adaptive_decimal_places;

		// Copy CString members using StrCpyNW
		StrCpyNW(this->pwr_unit_str.GetBuffer(UNIT_STR_MAXLEN),
			other.pwr_unit_str.GetString(), UNIT_STR_MAXLEN);
		StrCpyNW(this->electric_capacity_unit_str.GetBuffer(UNIT_STR_MAXLEN),
			other.electric_capacity_unit_str.GetString(), UNIT_STR_MAXLEN);
		StrCpyNW(this->electric_voltage_unit_str.GetBuffer(UNIT_STR_MAXLEN),
			other.electric_voltage_unit_str.GetString(), UNIT_STR_MAXLEN);
		StrCpyNW(this->hour_unit_str.GetBuffer(UNIT_STR_MAXLEN),
			other.hour_unit_str.GetString(), UNIT_STR_MAXLEN);
		StrCpyNW(this->minute_unit_str.GetBuffer(UNIT_STR_MAXLEN),
			other.minute_unit_str.GetString(), UNIT_STR_MAXLEN);

		// Release the buffers
		this->pwr_unit_str.ReleaseBuffer();
		this->electric_capacity_unit_str.ReleaseBuffer();
		this->electric_voltage_unit_str.ReleaseBuffer();
		this->hour_unit_str.ReleaseBuffer();
		this->minute_unit_str.ReleaseBuffer();

		return *this;
	}
	bool CopyTo(SettingData* targetObject) const {

		(*targetObject) = *this;

		return true;
	}
};

class CDataManager
{
private:
	CDataManager();
	~CDataManager();

public:
	static CDataManager& Instance();

	bool FirstInitalCheck(SettingData setting_data);

	void LoadConfig(const std::wstring& config_dir);

	void SaveConfig() const;
	const CString& StringRes(UINT id);      //根据资源id获取一个字符串资源
	void DPIFromWindow(CWnd* pWnd);
	int DPI(int pixel);
	float DPIF(float pixel);
	int RDPI(int pixel);
	HICON GetIcon(UINT id);

	SettingData m_setting_data;



private:
	static CDataManager m_instance;
	std::wstring m_config_path;
	std::map<UINT, CString> m_string_table;
	std::map<UINT, HICON> m_icons;
	int m_dpi{ 96 };
};
