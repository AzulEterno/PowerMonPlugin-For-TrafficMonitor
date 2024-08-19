#include "pch.h"
#include "PowerMon.h"
#include "DataManager.h"
#include "OptionsDlg.h"



PowerMon PowerMon::m_instance;

PowerMon::PowerMon()
{
	//Initalizing code

	g_data.LoadConfig(L"");
	sync_g_settings();
	//update_battery_base_info();


	//_bih = BatteryInfoHandler();

	_bih.is_dbg = this->is_dbg;

	std::shared_ptr<ValueUnitStringFormatter> sptr_vusf = std::shared_ptr<ValueUnitStringFormatter>(&_vusf);
	std::shared_ptr<BatteryInfoHandler> sptr_bih = std::shared_ptr<BatteryInfoHandler>(&_bih);

	bp_m_item.SetDataSource(sptr_bih, sptr_vusf);

	b_percentage_m_item.SetDataSource(sptr_bih, sptr_vusf);

	b_cap_m_item.SetDataSource(sptr_bih, sptr_vusf);
	b_volt_m_item.SetDataSource(sptr_bih, sptr_vusf);
	b_time_m_item.SetDataSource(sptr_bih, sptr_vusf);

}

PowerMon& PowerMon::Instance()
{
	return m_instance;
}

IPluginItem* PowerMon::GetItem(int index)
{
	switch (index)
	{
	case 0:
		return &bp_m_item;
	case 1:
		return &b_percentage_m_item;
	case 2:
		return &b_cap_m_item;
	case 3:
		return &b_volt_m_item;
	case 4:
		return &b_time_m_item;
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
	//Battery part ended.

	auto result_str = UpdateStringLabel(updatePwrStateResult, updateBatteryInfoResult);

	m_tooltip_info = result_str;
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
			wss << g_data.StringRes(IDS_DISCRIPTIVE_BATERY_PERCENTAGE_HEADER).GetString() << _bih.GetSystemBatteryLifePercent() << "%" << std::endl;
			wss << g_data.StringRes(IDS_DISCRIPTIVE_BATERY_POWER_HEADER).GetString() << ((_bih.GetBatteryStatusPowerRate() > 0) ? "+" : "") << _bih.GetBatteryStatusPowerRate() << " mW" << std::endl;
			wss << g_data.StringRes(IDS_DISCRIPTIVE_BATERY_CAPACITY_HEADER).GetString() << _bih.GetBatteryCapacity() << " mWh" << std::endl;
			wss << g_data.StringRes(IDS_DISCRIPTIVE_BATERY_VOLTAGE_HEADER).GetString() << _bih.GetBatteryStatusVoltage() << " mV" << std::endl;
			//m_tooltip_info = wss.str();



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
	static CString str;
	switch (index)
	{
	case TMI_NAME:
		return g_data.StringRes(IDS_PLUGIN_NAME).GetString();
	case TMI_DESCRIPTION:
		return g_data.StringRes(IDS_PLUGIN_DESCRIPTION).GetString();
	case TMI_AUTHOR:
		//TODO: 在此返回作者的名字
		return L"Ázuroso";
	case TMI_COPYRIGHT:
		//TODO: 在此返回版权信息
		return L"Copyright (C) by Ázuroso 2022";
	case ITMPlugin::TMI_URL:
		//TODO: 在此返回URL
		return L"https://github.com/AzulEterno/PowerMonPlugin-For-TrafficMonitor";
		break;
	case TMI_VERSION:
		//TODO: 在此修改插件的版本
		return L"1.2";
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
		break;
	default:
		break;
	}
}


//
//bool PowerMon::update_battery_base_info(bool debug_mode)
//{
//
//	// Returned value includes GBS_HASBATTERY if the system has a 
//	// non-UPS battery, and GBS_ONBATTERY if the system is running on 
//	// a battery.
//	//
//	// dwResult & GBS_ONBATTERY means we have not yet found AC power.
//	// dwResult & GBS_HASBATTERY means we have found a non-UPS battery.
//
//	on_battery = GBS_ONBATTERY;
//
//	// IOCTL_BATTERY_QUERY_INFORMATION,
//	// enumerate the batteries and ask each one for information.
//
//	if (debug_mode)
//		MessageBox(NULL, L"Start updating battery base info.", L"Important Tip", MB_OK);
//
//	HDEVINFO hdev =
//		SetupDiGetClassDevs(&GUID_DEVCLASS_BATTERY,
//			0,
//			0,
//			DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);
//	if (INVALID_HANDLE_VALUE != hdev)
//	{
//		// Limit search to 100 batteries max
//		for (int idev = 0; idev < 100; idev++)
//		{
//			SP_DEVICE_INTERFACE_DATA did = { 0 };
//			did.cbSize = sizeof(did);
//
//			if (SetupDiEnumDeviceInterfaces(hdev,
//				0,
//				&GUID_DEVCLASS_BATTERY,
//				idev,
//				&did))
//			{
//				DWORD cbRequired = 0;
//
//				SetupDiGetDeviceInterfaceDetail(hdev,
//					&did,
//					0,
//					0,
//					&cbRequired,
//					0);
//				if (ERROR_INSUFFICIENT_BUFFER == GetLastError())
//				{
//					//PSP_DEVICE_INTERFACE_DETAIL_DATA pdidd =
//					//    (PSP_DEVICE_INTERFACE_DETAIL_DATA)LocalAlloc(LPTR,
//					//        cbRequired);
//					pdidd =
//						(PSP_DEVICE_INTERFACE_DETAIL_DATA)LocalAlloc(LPTR,
//							cbRequired);
//					if (pdidd)
//					{
//						pdidd->cbSize = sizeof(*pdidd);
//						if (SetupDiGetDeviceInterfaceDetail(hdev,
//							&did,
//							pdidd,
//							cbRequired,
//							&cbRequired,
//							0))
//						{
//							// Enumerated a battery.  Ask it for information.
//							hBattery =
//								CreateFile(pdidd->DevicePath,
//									GENERIC_READ | GENERIC_WRITE,
//									FILE_SHARE_READ | FILE_SHARE_WRITE,
//									NULL,
//									OPEN_EXISTING,
//									FILE_ATTRIBUTE_NORMAL,
//									NULL);
//							if (INVALID_HANDLE_VALUE != hBattery)
//							{
//								// Ask the battery for its tag.
//								BATTERY_QUERY_INFORMATION bqi = { 0 };
//
//								DWORD dwWait = 0;
//								DWORD dwOut;
//
//								if (DeviceIoControl(hBattery,
//									IOCTL_BATTERY_QUERY_TAG,
//									&dwWait,
//									sizeof(dwWait),
//									&bqi.BatteryTag,
//									sizeof(bqi.BatteryTag),
//									&dwOut,
//									NULL)
//									&& bqi.BatteryTag)
//								{
//									//cout << "BatteryTag:" << bqi.BatteryTag << endl;
//									// With the tag, you can query the battery info.
//									BATTERY_INFORMATION bi = { 0 };
//									bqi.InformationLevel = BatteryInformation;
//
//									if (DeviceIoControl(hBattery,
//										IOCTL_BATTERY_QUERY_INFORMATION,
//										&bqi,
//										sizeof(bqi),
//										&bi,
//										sizeof(bi),
//										&dwOut,
//										NULL))
//									{
//										// Only non-UPS system batteries count
//										if (bi.Capabilities & BATTERY_SYSTEM_BATTERY)
//										{
//											is_relative_battery_rate = bi.Capabilities & BATTERY_CAPACITY_RELATIVE;
//											//cout << "Capabilities:" << bi.Capabilities << endl;
//											//cout << "Technology:" << bi.Technology << endl;
//											//cout << "Reserved:" << bi.Reserved << endl;
//											//cout << "Chemistry:" << bi.Chemistry << endl;
//											//cout << "DesignedCapacity:" << bi.DesignedCapacity << endl;
//											//cout << "FullChargedCapacity:" << bi.FullChargedCapacity << endl;
//											//cout << "DefaultAlert1:" << bi.DefaultAlert1 << endl;
//											//cout << "DefaultAlert2:" << bi.DefaultAlert2 << endl;
//											//cout << "CriticalBias:" << bi.CriticalBias << endl;
//											//cout << "CycleCount:" << bi.CycleCount << endl;
//
//											if (!(bi.Capabilities & BATTERY_IS_SHORT_TERM))
//											{
//												on_battery |= GBS_HASBATTERY;
//											}
//
//											// Query the battery status.
//											BATTERY_WAIT_STATUS bws = { 0 };
//											bws.BatteryTag = bqi.BatteryTag;
//
//											BATTERY_STATUS bs{};
//											if (DeviceIoControl(hBattery,
//												IOCTL_BATTERY_QUERY_STATUS,
//												&bws,
//												sizeof(bws),
//												&bs,
//												sizeof(bs),
//												&dwOut,
//												NULL))
//											{
//												if (bs.PowerState & BATTERY_POWER_ON_LINE)
//												{
//													on_battery &= ~GBS_ONBATTERY;
//												}
//											}
//										}
//									}
//
//									//bqi2.BatteryTag = bqi.BatteryTag;
//									//bqi2.InformationLevel = BatteryEstimatedTime;
//									//BATTERY_STATUS* nbtst = new BATTERY_STATUS;
//									//BATTERY_WAIT_STATUS nbwst = { bqi.BatteryTag,0,0,0,0 };
//									//if (qry_bws_inf != nullptr) {
//									//    delete qry_bws_inf;
//									//}
//									//qry_bws_inf = new BATTERY_WAIT_STATUS;
//									qry_bws_inf.BatteryTag = bqi.BatteryTag;
//
//
//									//ULONG estimated_time = 0;
//									break;
//
//								}
//
//
//
//							}
//						}
//
//					}
//					else {
//						if (debug_mode)
//							MessageBox(NULL, L"Failed to evaluate pdidd.", L"Important Tip", MB_OK);
//					}
//				}
//				else {
//					if (debug_mode)
//						MessageBox(NULL, L"Failed to recreate pdidd.", L"Important Tip", MB_OK);
//				}
//			}
//			else  if (ERROR_NO_MORE_ITEMS == GetLastError())
//			{
//				if (debug_mode)
//					MessageBox(NULL, L"Enumeration failed - perhaps we're out of items.", L"Important Tip", MB_OK);
//				break;  // Enumeration failed - perhaps we're out of items
//			}
//		}
//		SetupDiDestroyDeviceInfoList(hdev);
//	}
//	else {
//		//cout << "NO Available Batteries.";
//		if (debug_mode)
//			MessageBox(NULL, L"No Available Batteries.\nhdev == INVALID_HANDLE_VALUE.", L"Important Tip", MB_OK);
//	}
//	//  Final cleanup:  If we didn't find a battery, then presume that we
//	//  are on AC power.
//
//	if (!(on_battery & GBS_HASBATTERY))
//		on_battery &= ~GBS_ONBATTERY;
//
//
//	if (is_dbg) {
//		wchar_t info_str[50] = { 0 };
//		swprintf_s(info_str, L" 已经完成初始化监控数据。BatteryTag:%d,on_battery:%x", qry_bws_inf.BatteryTag, on_battery);
//		MessageBox(NULL, info_str, L"Tip", MB_OK);
//	}
//
//
//	return false;
//}
//
//bool PowerMon::update_battery_base_info()
//{
//	return update_battery_base_info(is_dbg);
//}
//
//bool PowerMon::do_update_info()
//{
//	if (GetSystemPowerStatus(&sys_pwr_state)) {
//		if (sys_pwr_state.BatteryFlag == 128) {
//			std::wstringstream wss;
//			wss << g_data.StringRes(IDS_BATTERY_NONEXISTS).GetString() << std::endl;
//			m_tooltip_info = wss.str();
//			return false;
//		}
//		else if (sys_pwr_state.BatteryFlag == 256) {
//			std::wstringstream wss;
//			wss << g_data.StringRes(IDS_BATTERY_INFO_UNKNOWN).GetString() << std::endl;
//			m_tooltip_info = wss.str();
//			return false;
//		}
//
//	}
//	else {
//		std::wstringstream wss;
//		wss << g_data.StringRes(IDS_BATTERY_INFO_UNKNOWN).GetString() << std::endl;
//		m_tooltip_info = wss.str();
//		return false;
//	}
//
//
//	ULONG dwOut = 0;
//	if (hBattery != INVALID_HANDLE_VALUE) {
//		if (DeviceIoControl(hBattery,
//			IOCTL_BATTERY_QUERY_STATUS,
//			&qry_bws_inf,
//			sizeof(qry_bws_inf),
//			&qry_res_inf,
//			sizeof(qry_res_inf),
//			&dwOut,
//			NULL)) {
//			//cout << "!!!estimated_time:" << estimated_time << endl;
//			//cout << "PowerState:" << nbwst.PowerState << endl;
//			//cout << "LowCapacity:" << nbwst.LowCapacity << endl;
//			//cout << "HighCapacity:" << nbwst.HighCapacity << endl;
//			//cout << "!!!Charge_Rate:" << nbtst->Rate << endl;
//			// 
//			// 
//			// 
//			//wchar_t info_str[100] = { 0 };
//
//			//swprintf_s(info_str, L"已经获取电池功耗数据代码.\nPower:%d mW, Voltage:%d mV.", qry_res_inf.Rate,qry_res_inf.Voltage);
//			//MessageBox(NULL, info_str, L"提示", MB_OK);
//
//
//			//生成鼠标提示信息
//			std::wstringstream wss;
//			wss << g_data.StringRes(IDS_DISCRIPTIVE_BATERY_POWER_HEADER).GetString() << qry_res_inf.Rate << " mW" << std::endl;
//			wss << g_data.StringRes(IDS_DISCRIPTIVE_BATERY_CAPACITY_HEADER).GetString() << qry_res_inf.Capacity << " mWh" << std::endl;
//			wss << g_data.StringRes(IDS_DISCRIPTIVE_BATERY_VOLTAGE_HEADER).GetString() << qry_res_inf.Voltage << " mV" << std::endl;
//			m_tooltip_info = wss.str();
//
//
//			return true;
//		}
//		else {
//			//MessageBox(NULL, L"获取条件满足但是获取失败.", L"提示", MB_OK);
//			free_res_mem();
//			//update_battery_base_info();
//
//			//生成鼠标提示信息
//			std::wstringstream wss;
//			wss << g_data.StringRes(IDS_RELOADING_BATTERY_DRIVER_TIP).GetString() << std::endl;
//			wss << g_data.StringRes(IDS_BATTERY_DRIVER_HANDLER_CLEANING).GetString() << std::endl;
//			m_tooltip_info = wss.str();
//			return false;
//		}
//	}
//	else {
//
//		free_res_mem();
//		update_battery_base_info();
//
//		//生成鼠标提示信息
//		std::wstringstream wss;
//		wss << g_data.StringRes(IDS_RELOADING_BATTERY_DRIVER_TIP).GetString() << std::endl;
//		wss << g_data.StringRes(IDS_BATTERY_DRIVER_REBUILDING).GetString() << std::endl;
//		m_tooltip_info = wss.str();
//
//
//
//
//		return false;
//	}
//	return false;
//}
//
//void PowerMon::free_res_mem(bool debug_mode) {
//	std::wstringstream wss;
//	//if (qry_bws_inf.BatteryTag ) {
//	//    delete qry_bws_inf;
//	//    wss << "qry_bws_inf resetted." << std::endl;
//	//}
//	//if (qry_res_inf != nullptr) {
//	//    delete qry_res_inf;
//	//}
//	//if (lp_bty_qry_inf != nullptr) {
//	//    delete lp_bty_qry_inf;
//	//    wss << "lp_bty_qry_inf resetted." << std::endl;
//	//}
//	if (hBattery != INVALID_HANDLE_VALUE) {
//		CloseHandle(hBattery);
//		hBattery = INVALID_HANDLE_VALUE;
//
//		wss << "hBattery closed." << std::hex << (void*)hBattery << std::endl;
//	}
//	if (pdidd != nullptr) {
//		LocalFree(pdidd);
//		pdidd = nullptr;
//		wss << "pdidd freed." << std::hex << (void*)hBattery << std::endl;
//	}
//	if (debug_mode)
//		MessageBox(NULL, wss.str().c_str(), L"Battery Driver Block Reset Info", MB_OK);
//
//}sync_g_settings
//
//void PowerMon::free_res_mem()
//{
//	free_res_mem(is_dbg);
//}

void PowerMon::sync_g_settings()
{
	this->is_dbg = g_data.m_setting_data.is_dbg_mode;


	_vusf.LoadFromSettings(g_data.m_setting_data);
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
