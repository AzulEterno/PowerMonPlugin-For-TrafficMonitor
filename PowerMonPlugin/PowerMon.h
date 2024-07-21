#pragma once
#define ENABLE_DEBUG_POP_WINDOWS false

#include <sstream>
#include "poclass.h"
#include "ioapiset.h"
#include "windows.h"
#include "WinBase.h"
#include "SetupAPI.h"
#pragma comment (lib, "SetupApi.lib")
#include "BatClass.h"
#include "devguid.h"
#include "winioctl.h"

#include "PluginInterface.h"
#include "PowerMonItem.h"
#include <string>
#include "DataManager.h"




class PowerMon : public ITMPlugin
{
private:
	PowerMon();

public:
	static PowerMon& Instance();

	virtual IPluginItem* GetItem(int index) override;
	virtual const wchar_t* GetTooltipInfo() override;
	virtual void DataRequired() override;
	virtual OptionReturn ShowOptionsDialog(void* hParent) override;
	virtual const wchar_t* GetInfo(PluginInfoIndex index) override;
	virtual void OnExtenedInfo(ExtendedInfoIndex index, const wchar_t* data) override;

	bool update_battery_base_info(bool debug_mode);

	bool update_battery_base_info();
	bool do_update_info();
	void free_res_mem(bool debug_mode);
	void free_res_mem();
	void sync_g_settings();

	~PowerMon();

	BATTERY_STATUS qry_res_inf = { 0 };

	SYSTEM_POWER_STATUS sys_pwr_state = { 0 };

	DWORD on_battery;


	HANDLE hBattery = INVALID_HANDLE_VALUE;
	PSP_DEVICE_INTERFACE_DETAIL_DATA pdidd;

	BATTERY_WAIT_STATUS  qry_bws_inf = { 0 };

	bool is_dbg = ENABLE_DEBUG_POP_WINDOWS;
	wchar_t pwr_unit_str[PWR_UNIT_STR_MAXLEN] = L"W";
	//BATTERY_QUERY_INFORMATION* lp_bty_qry_inf = nullptr;
private:

	bool is_relative_battery_rate = false;



private:
	static PowerMon m_instance;
	PowerMonItem m_item;
	std::wstring m_tooltip_info;
};

#ifdef __cplusplus
extern "C" {
#endif
	__declspec(dllexport) ITMPlugin* TMPluginGetInstance();

#ifdef __cplusplus
}
#endif
