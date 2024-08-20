#pragma once
#define ENABLE_DEBUG_POP_WINDOWS false


#include "pch.h"

#include "BatteryPowerMonItem.h"

#include "DataManager.h"
#include "BatteryInfoHandler.h"
#include "ValueUnitStringFormatter.h"




class PowerMon : public ITMPlugin
{
private:
	PowerMon();

public:
	static PowerMon& Instance();

	virtual IPluginItem* GetItem(int index) override;
	virtual const wchar_t* GetTooltipInfo() override;
	virtual void DataRequired() override;
	std::wstring GetBatteryPowerToolTipString(int updatePwrStateResult, int updateBatteryInfoResult);
	std::wstring UpdateStringLabel(int updatePwrStateResult, int updateBatteryInfoResult);
	virtual OptionReturn ShowOptionsDialog(void* hParent) override;
	virtual const wchar_t* GetInfo(PluginInfoIndex index) override;
	virtual void OnExtenedInfo(ExtendedInfoIndex index, const wchar_t* data) override;


	void sync_g_settings();

	~PowerMon();

	BatteryInfoHandler& GetBatteryInfoProvider() {
		return _bih;
	}

	//bool update_battery_base_info(bool debug_mode);

	//bool update_battery_base_info();
	//bool do_update_info();
	//void free_res_mem(bool debug_mode);
	//void free_res_mem();

	//BATTERY_STATUS qry_res_inf = { 0 };

	//SYSTEM_POWER_STATUS sys_pwr_state = { 0 };

	//DWORD on_battery;


	//HANDLE hBattery = INVALID_HANDLE_VALUE;
	//PSP_DEVICE_INTERFACE_DETAIL_DATA pdidd;

	//BATTERY_WAIT_STATUS  qry_bws_inf = { 0 };

	bool is_dbg = ENABLE_DEBUG_POP_WINDOWS;

	//BATTERY_QUERY_INFORMATION* lp_bty_qry_inf = nullptr;
protected:
	ValueUnitStringFormatter _vusf;
	BatteryInfoHandler _bih;
	//bool is_relative_battery_rate = false;

	BatteryPowerMonItem bp_m_item;
	BatteryPercentageMonItem b_percentage_m_item;
	BatteryCapacityMonItem b_cap_m_item;
	BatteryVoltageMonItem b_volt_m_item;
	BatteryTimeMonItem b_time_m_item;

	std::wstring m_tooltip_info;

private:
	static PowerMon m_instance;
};

#ifdef __cplusplus
extern "C" {
#endif
	__declspec(dllexport) ITMPlugin* TMPluginGetInstance();

#ifdef __cplusplus
}
#endif
