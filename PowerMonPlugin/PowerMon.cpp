#include "pch.h"
#include "PowerMon.h"
#include "DataManager.h"
#include "OptionsDlg.h"
#include "WinCPP_Utility.h"



PowerMon PowerMon::m_instance;

PowerMon::PowerMon()
{
	//Initalizing code

	//g_data.LoadConfig(L"");

	//update_battery_base_info();


	//_bih = BatteryInfoHandler();


		//std::shared_ptr<ValueUnitStringFormatter> sptr_vusf = std::shared_ptr<ValueUnitStringFormatter>(&_vusf);
		//std::shared_ptr<BatteryInfoHandler> sptr_bih = std::shared_ptr<BatteryInfoHandler>(&_bih);

	bp_m_item.SetDataSource(&_vusf, &_bih);

	b_percentage_m_item.SetDataSource(&_vusf, &_bih);

	b_cap_m_item.SetDataSource(&_vusf, &_bih);
	b_volt_m_item.SetDataSource(&_vusf, &_bih);
	b_time_m_item.SetDataSource(&_vusf, &_bih);

#if WINRT_USE_FLAG
	sm_m_item.SetDataSource(&_vusf, &_bih, &_hwpdp);

	cpu_m_item.SetDataSource(&_vusf, &_hwpdp);
	gpu_m_item.SetDataSource(&_vusf, &_hwpdp);
#endif

}

PowerMon& PowerMon::Instance()
{
	return m_instance;
}

IPluginItem* PowerMon::GetItem(int index)
{
	switch (index)
	{
		//Battery Status section
	case 0:
		return &bp_m_item;
	case 1:
		return &b_cap_m_item;
	case 2:
		return &b_volt_m_item;
	case 3:
		return &b_time_m_item;
	case 4:
		return &b_percentage_m_item;
#if WINRT_USE_FLAG
		//Hardware aware section.
	case 5:
		return &sm_m_item;
	case 6:
		return &cpu_m_item;
	case 7:
		return &gpu_m_item;
#endif
	default:
		break;
	}
	return nullptr;
}

const wchar_t* PowerMon::GetTooltipInfo()
{
	return m_tooltip_info.c_str();
}

void PowerMon::DataRequired()
{
	//TODO: 在此添加获取监控数据的代码 

	//Battery part started
	int updatePwrStateResult = _bih.UpdateSystemPowerStatus();


	if (updatePwrStateResult && this->is_dbg) {
		std::wstringstream wss;
		wss << "Something went wrong when calling UpdateSystemPowerStatus:" << updatePwrStateResult << std::endl;
		//		wss << g_data.StringRes(IDS_BATTERY_DRIVER_REBUILDING).GetString() << std::endl;
		//m_tooltip_info = wss.str();

		MessageBox(NULL, (const wchar_t*)wss.str().c_str(), L"Tip", MB_OK);
	}

	int updateBatteryInfoResult = _bih.UpdateBatteryInfo();




	if (updateBatteryInfoResult && this->is_dbg) {
		std::wstringstream wss;
		wss << "Something went wrong when calling UpdateBatteryInfo:" << updateBatteryInfoResult << std::endl;
		//		wss << g_data.StringRes(IDS_BATTERY_DRIVER_REBUILDING).GetString() << std::endl;
		//m_tooltip_info = wss.str();

		MessageBox(NULL, (const wchar_t*)wss.str().c_str(), L"Tip", MB_OK);
	}

	std::wstring hw_mon_power_str = L"";

	if (g_data.m_setting_data.enable_cpu_monitor || g_data.m_setting_data.enable_gpu_monitor) {
#if WINRT_USE_FLAG
		updateBatteryInfoResult = _hwpdp.CallUpdateInfo();
		hw_mon_power_str = _hwpdp.GetPowerSummaryStr(_vusf);
#endif
	}

	//Battery part ended.

	auto result_str_btr_module = UpdateStringLabel(updatePwrStateResult, updateBatteryInfoResult);

	m_tooltip_info = hw_mon_power_str + result_str_btr_module;
}


std::wstring PowerMon::GetBatteryPowerToolTipString(int updatePwrStateResult, int updateBatteryInfoResult) {
	std::wstringstream wss;

	{

		switch (updatePwrStateResult) {
		case 0:
			break;
		default:

			wss << "UpdatePwrStateResultCode: " << updatePwrStateResult << std::endl;
		}
		if (!updatePwrStateResult) {

			switch (_bih.GetSystemBatteryFlag()) {
			case 128:
			{
				wss << g_data.StringRes(IDS_BATTERY_NONEXISTS).GetString() << std::endl;
				return wss.str();
			}break;
			case 256:
			{
				wss << g_data.StringRes(IDS_BATTERY_INFO_UNKNOWN).GetString() << std::endl;
				return wss.str();
			}break;

			default:
			{}

			}
		}
		else {
			wss << g_data.StringRes(IDS_BATTERY_INFO_UNKNOWN).GetString() << std::endl;

			return wss.str();
		}



		switch (updateBatteryInfoResult) {
		case 0:
		{
			//wss << g_data.StringRes(IDS_DISCRIPTIVE_BATERY_PERCENTAGE_HEADER).GetString() << _bih.GetSystemBatteryLifePercent() << " %" << std::endl;
			//wss << g_data.StringRes(IDS_DISCRIPTIVE_BATERY_POWER_HEADER).GetString() << ((_bih.GetBatteryStatusPowerRate() > 0) ? "+" : "") << _bih.GetBatteryStatusPowerRate() << " mW" << std::endl;
			//wss << g_data.StringRes(IDS_DISCRIPTIVE_BATERY_CAPACITY_HEADER).GetString() << _bih.GetBatteryCapacity() << " mWh" << std::endl;
			//wss << g_data.StringRes(IDS_DISCRIPTIVE_BATERY_VOLTAGE_HEADER).GetString() << _bih.GetBatteryStatusVoltage() << " mV" << std::endl;
			//m_tooltip_info = wss.str();

			wss << g_data.StringRes(IDS_DISCRIPTIVE_BATERY_PERCENTAGE_HEADER).GetString() << _vusf.WSTR_FormatFloatValue(_bih.GetSystemBatteryLifePercent(),
				L"%", nullptr, false, 0, 0) << std::endl;
			wss << g_data.StringRes(IDS_DISCRIPTIVE_BATERY_POWER_HEADER).GetString() << _vusf.WSTR_FormatFloatValue(_bih.GetBatteryStatusPowerRate() / 1000.0,
				_vusf.GetPowerUnitString(), nullptr, false, 0, 3) << std::endl;
			wss << g_data.StringRes(IDS_DISCRIPTIVE_BATERY_CAPACITY_HEADER).GetString() << _vusf.WSTR_FormatFloatValue(_bih.GetBatteryCapacity() / 1000.0,
				_vusf.GetPowerCapacityString(), nullptr, false, 0, 3) << std::endl;
			wss << g_data.StringRes(IDS_DISCRIPTIVE_BATERY_VOLTAGE_HEADER).GetString() << _vusf.WSTR_FormatFloatValue(_bih.GetBatteryStatusVoltage() / 1000.0,
				_vusf.GetVoltageString(), nullptr, false, 0, 3) << std::endl;



		}break;
		default:
		{
			wss << g_data.StringRes(IDS_RELOADING_BATTERY_DRIVER_TIP).GetString() << std::endl;
			wss << g_data.StringRes(IDS_BATTERY_DRIVER_REBUILDING).GetString() << std::endl;
			//m_tooltip_info = wss.str();
		}

		}


	}
	//return false;
	return wss.str();
}

std::wstring PowerMon::UpdateStringLabel(int updatePwrStateResult, int updateBatteryInfoResult) {

	std::wstringstream wss;

	wss << GetBatteryPowerToolTipString(updatePwrStateResult, updateBatteryInfoResult);

	return wss.str();
}


ITMPlugin::OptionReturn PowerMon::ShowOptionsDialog(void* hParent)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CWnd* pParent = CWnd::FromHandle((HWND)hParent);
	COptionsDlg dlg(pParent);
	dlg.m_data = g_data.m_setting_data;
	if (dlg.DoModal() == IDOK)
	{
		g_data.m_setting_data = dlg.m_data;
		//更新自身设置
		sync_g_settings();


		return ITMPlugin::OR_OPTION_CHANGED;
	}
	return ITMPlugin::OR_OPTION_UNCHANGED;
}

const wchar_t* PowerMon::GetInfo(PluginInfoIndex index)
{
	static FILEVERSIONINFOSTRUCT* infoStorePtr = nullptr;

	if (infoStorePtr == nullptr) {

		infoStorePtr = new FILEVERSIONINFOSTRUCT;
		*infoStorePtr = GetSelfFileVersionInfoStructData();
	}

	//static CString str;
	switch (index)
	{
	case TMI_NAME:
		return g_data.StringRes(IDS_PLUGIN_NAME).GetString();
	case TMI_DESCRIPTION:
		return g_data.StringRes(IDS_PLUGIN_DESCRIPTION).GetString();
	case TMI_AUTHOR:
		//TODO: 在此返回作者的名字
		return infoStorePtr->companyName.c_str();
	case TMI_COPYRIGHT:
		//TODO: 在此返回版权信息
		return infoStorePtr->legalCopyright.c_str();
	case ITMPlugin::TMI_URL:
		//TODO: 在此返回URL
		return L"https://github.com/AzulEterno/PowerMonPlugin-For-TrafficMonitor";
		break;
	case TMI_VERSION:
		//TODO: 在此修改插件的版本
		return infoStorePtr->productVersion.c_str();
	default:
		break;
	}
	return L"";
}

void PowerMon::OnExtenedInfo(ExtendedInfoIndex index, const wchar_t* data)
{
	switch (index)
	{
	case ITMPlugin::EI_CONFIG_DIR:
		//从配置文件读取配置
		g_data.LoadConfig(std::wstring(data));

		sync_g_settings();
		break;
	default:
		break;
	}
}


void PowerMon::sync_g_settings()
{
	this->is_dbg = g_data.m_setting_data.is_dbg_mode;
	_bih.is_dbg = this->is_dbg;

	_vusf.LoadFromSettings(g_data.m_setting_data);
#if WINRT_USE_FLAG
	_hwpdp.SyncFromSettingData(g_data.m_setting_data);
#endif
	// Assuming g_data.m_setting_data.pwr_unit_str is an ATL::CSimpleStringT<wchar_t, true>
	//const wchar_t* constPwrUnitStr = g_data.m_setting_data.pwr_unit_str.GetString();
	//size_t length = wcslen(constPwrUnitStr) + 1;
	//wchar_t* mutablePwrUnitStr = new wchar_t[length];
	//wcscpy_s(mutablePwrUnitStr, length, constPwrUnitStr);



	//this->bp_m_item.SetPowerUnitStr(mutablePwrUnitStr);



	// Don't forget to delete the dynamically allocated memory to avoid memory leaks

	//StrCpy(this->pwr_unit_str, g_data.m_setting_data.pwr_unit_str.GetString());
}

PowerMon::~PowerMon()
{
	//delete& _bih;
}

ITMPlugin* TMPluginGetInstance()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return &PowerMon::Instance();
}
