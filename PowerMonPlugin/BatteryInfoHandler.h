#pragma once
#include "pch.h"






class BatteryInfoHandler
{
private:

protected:


	SYSTEM_POWER_STATUS _sys_pwr_state = { 0 };

	DWORD _on_battery = GBS_ONBATTERY;


	HANDLE _h_Battery = INVALID_HANDLE_VALUE;
	PSP_DEVICE_INTERFACE_DETAIL_DATA _pdidd = { 0 };

	BATTERY_WAIT_STATUS  _qry_bws_inf = { 0 };
	BATTERY_STATUS _qry_result_info = { 0 };
	bool is_relative_battery_rate = false;





public:



	bool has_battery() const {

		return _on_battery;
	}

	bool is_dbg = false;

	INT64 GetBatteryStatusPowerRate() const {
		//if (_qry_result_info.PowerState & BATTERY_CHARGING) {

		//	return _qry_result_info.Rate;
		//}
		//else if (_qry_result_info.PowerState & BATTERY_DISCHARGING) {
		//	return _qry_result_info.Rate;
		//}
		//else {
		//	return -1;
		//}
		return _qry_result_info.Rate;
	}

	INT64 GetBatteryCapacity() const {
		return _qry_result_info.Capacity;
	}

	INT64 GetBatteryStatusVoltage() const {
		return _qry_result_info.Voltage;
	}

	BYTE GetACLineStatus() const {

		return _sys_pwr_state.ACLineStatus;
	}

	BYTE GetSystemBatteryFlag() const {

		return _sys_pwr_state.BatteryFlag;
	}

	BYTE GetSystemBatteryLifePercent() const {
		return _sys_pwr_state.BatteryLifePercent;
	}

	BYTE GetSystemStatusFlag() const {

		return _sys_pwr_state.SystemStatusFlag;
	}

	INT64 GetBatteryLifeTime() const {
		if (_sys_pwr_state.BatteryLifeTime == MAX_UINT32) {
			return -1;
		}
		return _sys_pwr_state.BatteryLifeTime;
	}

	INT64 GetBatteryFullLifeTime() const {
		if (_sys_pwr_state.BatteryFullLifeTime == MAX_UINT32) {
			return -1;
		}
		return _sys_pwr_state.BatteryFullLifeTime;
	}

	BatteryInfoHandler(bool auto_init_bt_driver = true) {
		if (auto_init_bt_driver) {
			InitBatteryDriver();
		}

	};

	~BatteryInfoHandler() {
		ReleaseResources();
	}

	int InitBatteryDriver(bool auto_release_previous_data = true) {

		if (auto_release_previous_data) {

			ReleaseResources();
		}

		// Returned value includes GBS_HASBATTERY if the system has a 
	// non-UPS battery, and GBS_ONBATTERY if the system is running on 
	// a battery.
	//
	// dwResult & GBS_ONBATTERY means we have not yet found AC power.
	// dwResult & GBS_HASBATTERY means we have found a non-UPS battery.

		_on_battery = GBS_ONBATTERY;

		// IOCTL_BATTERY_QUERY_INFORMATION,
		// enumerate the batteries and ask each one for information.

		if (is_dbg)
			MessageBox(NULL, L"Start updating battery base info.", L"Important Tip", MB_OK);

		HDEVINFO hdev =
			SetupDiGetClassDevs(&GUID_DEVCLASS_BATTERY,
				0,
				0,
				DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);
		if (INVALID_HANDLE_VALUE != hdev)
		{
			// Limit search to 100 batteries max
			for (int idev = 0; idev < 100; idev++)
			{
				SP_DEVICE_INTERFACE_DATA did = { 0 };
				did.cbSize = sizeof(did);

				if (SetupDiEnumDeviceInterfaces(hdev,
					0,
					&GUID_DEVCLASS_BATTERY,
					idev,
					&did))
				{
					DWORD cbRequired = 0;

					SetupDiGetDeviceInterfaceDetail(hdev,
						&did,
						0,
						0,
						&cbRequired,
						0);
					if (ERROR_INSUFFICIENT_BUFFER == GetLastError())
					{
						SetLastError(NOERROR);
						//PSP_DEVICE_INTERFACE_DETAIL_DATA _pdidd =
						//    (PSP_DEVICE_INTERFACE_DETAIL_DATA)LocalAlloc(LPTR,
						//        cbRequired);
						_pdidd =
							(PSP_DEVICE_INTERFACE_DETAIL_DATA)LocalAlloc(LPTR,
								cbRequired);
						if (_pdidd)
						{
							_pdidd->cbSize = sizeof(*_pdidd);
							if (SetupDiGetDeviceInterfaceDetail(hdev,
								&did,
								_pdidd,
								cbRequired,
								&cbRequired,
								0))
							{
								// Enumerated a battery.  Ask it for information.
								_h_Battery =
									CreateFile(_pdidd->DevicePath,
										GENERIC_READ | GENERIC_WRITE,
										FILE_SHARE_READ | FILE_SHARE_WRITE,
										NULL,
										OPEN_EXISTING,
										FILE_ATTRIBUTE_NORMAL,
										NULL);
								if (INVALID_HANDLE_VALUE != _h_Battery)
								{
									// Ask the battery for its tag.
									BATTERY_QUERY_INFORMATION bqi = { 0 };

									DWORD dwWait = 0;
									DWORD dwOut;

									if (DeviceIoControl(_h_Battery,
										IOCTL_BATTERY_QUERY_TAG,
										&dwWait,
										sizeof(dwWait),
										&bqi.BatteryTag,
										sizeof(bqi.BatteryTag),
										&dwOut,
										NULL)
										&& bqi.BatteryTag)
									{
										//cout << "BatteryTag:" << bqi.BatteryTag << endl;
										// With the tag, you can query the battery info.
										BATTERY_INFORMATION bi = { 0 };
										bqi.InformationLevel = BatteryInformation;

										if (DeviceIoControl(_h_Battery,
											IOCTL_BATTERY_QUERY_INFORMATION,
											&bqi,
											sizeof(bqi),
											&bi,
											sizeof(bi),
											&dwOut,
											NULL))
										{
											// Only non-UPS system batteries count
											if (bi.Capabilities & BATTERY_SYSTEM_BATTERY)
											{
												is_relative_battery_rate = bi.Capabilities & BATTERY_CAPACITY_RELATIVE;
												//cout << "Capabilities:" << bi.Capabilities << endl;
												//cout << "Technology:" << bi.Technology << endl;
												//cout << "Reserved:" << bi.Reserved << endl;
												//cout << "Chemistry:" << bi.Chemistry << endl;
												//cout << "DesignedCapacity:" << bi.DesignedCapacity << endl;
												//cout << "FullChargedCapacity:" << bi.FullChargedCapacity << endl;
												//cout << "DefaultAlert1:" << bi.DefaultAlert1 << endl;
												//cout << "DefaultAlert2:" << bi.DefaultAlert2 << endl;
												//cout << "CriticalBias:" << bi.CriticalBias << endl;
												//cout << "CycleCount:" << bi.CycleCount << endl;

												if (!(bi.Capabilities & BATTERY_IS_SHORT_TERM))
												{
													_on_battery |= GBS_HASBATTERY;
												}

												// Query the battery status.
												BATTERY_WAIT_STATUS bws = { 0 };
												bws.BatteryTag = bqi.BatteryTag;

												BATTERY_STATUS bs{};
												if (DeviceIoControl(_h_Battery,
													IOCTL_BATTERY_QUERY_STATUS,
													&bws,
													sizeof(bws),
													&bs,
													sizeof(bs),
													&dwOut,
													NULL))
												{
													if (bs.PowerState & BATTERY_POWER_ON_LINE)
													{
														_on_battery &= ~GBS_ONBATTERY;
													}
												}
											}
										}

										//bqi2.BatteryTag = bqi.BatteryTag;
										//bqi2.InformationLevel = BatteryEstimatedTime;
										//BATTERY_STATUS* nbtst = new BATTERY_STATUS;
										//BATTERY_WAIT_STATUS nbwst = { bqi.BatteryTag,0,0,0,0 };
										//if (_qry_bws_inf != nullptr) {
										//    delete _qry_bws_inf;
										//}
										//_qry_bws_inf = new BATTERY_WAIT_STATUS;
										_qry_bws_inf.BatteryTag = bqi.BatteryTag;


										//ULONG estimated_time = 0;
										break;

									}



								}
							}

						}
						else {
							if (is_dbg)
								MessageBox(NULL, L"Failed to evaluate _pdidd.", L"Important Tip", MB_OK);
						}
					}
					else {
						if (is_dbg)
							MessageBox(NULL, L"Failed to recreate _pdidd.", L"Important Tip", MB_OK);
					}
				}
				else  if (ERROR_NO_MORE_ITEMS == GetLastError())
				{
					SetLastError(NOERROR);
					if (is_dbg)
						MessageBox(NULL, L"Enumeration failed - perhaps we're out of items.", L"Important Tip", MB_OK);
					break;  // Enumeration failed - perhaps we're out of items
				}
			}
			SetupDiDestroyDeviceInfoList(hdev);
		}
		else {
			//cout << "NO Available Batteries.";
			if (is_dbg)
				MessageBox(NULL, L"No Available Batteries.\nhdev == INVALID_HANDLE_VALUE.", L"Important Tip", MB_OK);
		}
		//  Final cleanup:  If we didn't find a battery, then presume that we
		//  are on AC power.

		if (!(_on_battery & GBS_HASBATTERY))
			_on_battery &= ~GBS_ONBATTERY;


		if (is_dbg) {
			wchar_t info_str[50] = { 0 };
			swprintf_s(info_str, L" 已经完成初始化监控数据。BatteryTag:%d, _on_battery:%x", _qry_bws_inf.BatteryTag, _on_battery);
			MessageBox(NULL, info_str, L"Tip", MB_OK);
		}


		return false;
	}


	int UpdateSystemPowerStatus() {
		if (GetSystemPowerStatus(&_sys_pwr_state)) {
			return 0;
		}

		return 1;
	}

	int UpdateBatteryInfo() {
		static BYTE btflag = GetSystemBatteryFlag();
		if (btflag == BATTERY_FLAG_NO_BATTERY || btflag == BATTERY_FLAG_UNKNOWN) {
			return -1;
		}

		ULONG dwOut = 0;
		if (_h_Battery != INVALID_HANDLE_VALUE) {
			if (DeviceIoControl(_h_Battery,
				IOCTL_BATTERY_QUERY_STATUS,
				&_qry_bws_inf,
				sizeof(_qry_bws_inf),
				&_qry_result_info,
				sizeof(_qry_result_info),
				&dwOut,
				NULL)) {

				return 0;
			}
			else {
				DWORD err_code = GetLastError();
				SetLastError(NO_ERROR);
				ReleaseResources();
				return err_code;
			}
		}
		else {
			InitBatteryDriver(true);
			return 1;
		}

		return -1;
	};
	void ReleaseResources() {
		//std::wstringstream wss;

		if (_h_Battery != INVALID_HANDLE_VALUE) {
			CloseHandle(_h_Battery);
			_h_Battery = INVALID_HANDLE_VALUE;

			//wss << "hBattery closed." << std::hex << (void*)_h_Battery << std::endl;
		}
		if (_pdidd != nullptr) {
			LocalFree(_pdidd);
			_pdidd = nullptr;
			//wss << "pdidd freed." << std::hex << (void*)_pdidd << std::endl;
		}
		//if (is_dbg)
		//	MessageBox(NULL, wss.str().c_str(), L"Battery Driver Block Reset Info", MB_OK);

	};

	std::wstring GenerateDebugString() const {
		std::wstringstream info_str;


		info_str << "Current Battery Handler:" << std::hex << (void*)_h_Battery << std::endl;
		info_str << "Current PSP_DEVICE_INTERFACE_DETAIL_DATA:" << std::hex << (void*)_pdidd << std::endl;
		info_str << "Current BATTERY_WAIT_STATUS:" << std::hex << (void*)&_qry_bws_inf << std::endl;
		info_str << "Current BATTERY_TAG:" << _qry_bws_inf.BatteryTag << std::endl;
		info_str << "Debug Mode:" << is_dbg << std::endl;

		return info_str.str();
	}
};

