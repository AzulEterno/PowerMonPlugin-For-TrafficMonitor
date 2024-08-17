#pragma once
#include <string>
#include <map>
#include "resource.h"

#define g_data CDataManager::Instance()


struct SettingData
{
	//TODO: 在此添加选项设置的数据

	INT64 settings_altered_counter = 0;
	bool is_dbg_mode = false;
	CString pwr_unit_str{};
	CString electric_capacity_unit_str{};
	CString electric_voltage_unit_str{};

	int CopyTo(SettingData* targetObject) const {
		if (targetObject == nullptr) {
			return -1;
		}
		if (this == targetObject) {
			return -2;
		}

		int altered = 0;

		if (this->settings_altered_counter != targetObject->settings_altered_counter) {
			targetObject->settings_altered_counter = this->settings_altered_counter;
			altered += 1;
		}
		if (this->is_dbg_mode != targetObject->is_dbg_mode) {
			targetObject->is_dbg_mode = this->is_dbg_mode;
			altered += 1;
		}
		if (!StrCmp(this->pwr_unit_str.GetString(),
			targetObject->pwr_unit_str.GetString())) {

			StrCpyNW(targetObject->pwr_unit_str.GetBuffer(PWR_UNIT_STR_MAXLEN),
				this->pwr_unit_str.GetString(),
				PWR_UNIT_STR_MAXLEN);
			targetObject->pwr_unit_str = this->pwr_unit_str;
			altered += 1;
		}
		if (!StrCmp(this->electric_capacity_unit_str.GetString(),
			targetObject->electric_capacity_unit_str.GetString())) {
			StrCpyNW(targetObject->electric_capacity_unit_str.GetBuffer(PWR_UNIT_STR_MAXLEN),
				this->electric_capacity_unit_str.GetString(),
				PWR_UNIT_STR_MAXLEN);
			altered += 1;
		}
		if (!StrCmp(this->electric_voltage_unit_str.GetString(),
			targetObject->electric_voltage_unit_str.GetString())) {
			StrCpyNW(targetObject->electric_voltage_unit_str.GetBuffer(PWR_UNIT_STR_MAXLEN),
				this->electric_voltage_unit_str.GetString(),
				PWR_UNIT_STR_MAXLEN);
			altered += 1;
		}

		return altered;
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
