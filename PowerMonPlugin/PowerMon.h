#pragma once



#include "pch.h"

#include "BatteryPowerMonItem.h"
#include "BatteryInfoHandler.h"
#include "ValueUnitStringFormatter.h"
#include <string>
#include <PluginInterface.h>
#include "LibreHWMonInterOp.h"
#include "HardwareSensorMon.h"




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

	InterOpLibreHWMon::HardwareSensorDataProvider& GetHWPowerSensorDataProvider() {
		return _hwpdp;
	}


	bool is_dbg = ENABLE_DEBUG_POP_WINDOWS;

protected:
	ValueUnitStringFormatter _vusf;
	BatteryInfoHandler _bih;
	InterOpLibreHWMon::HardwareSensorDataProvider _hwpdp;
	//bool is_relative_battery_rate = false;

	BatteryPowerMonItem bp_m_item;
	BatteryPercentageMonItem b_percentage_m_item;
	BatteryCapacityMonItem b_cap_m_item;
	BatteryVoltageMonItem b_volt_m_item;
	BatteryTimeMonItem b_time_m_item;
	SmartPowerMeterMonItem sm_m_item;
	CPUPowerMonItem cpu_m_item;
	GPUPowerMonItem gpu_m_item;

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
