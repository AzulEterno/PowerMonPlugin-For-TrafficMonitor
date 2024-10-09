// HardwareSensorPage.cpp: 实现文件
//

#include "pch.h"
#include "afxdialogex.h"
#include "HardwareSensorPage.h"
#include "PowerMon.h"


// HardwareSensorPage 对话框

IMPLEMENT_DYNAMIC(HardwareSensorPage, CTabDlg)

HardwareSensorPage::HardwareSensorPage(CWnd* pParent /*=nullptr*/)
	: CTabDlg(IDD_HARDWARE_SENSOR_PAGE, pParent)
{
	//SyncTreeViewContent();
}

HardwareSensorPage::~HardwareSensorPage()
{
}

void HardwareSensorPage::DoDataExchange(CDataExchange* pDX)
{
	CTabDlg::DoDataExchange(pDX);
}

BOOL HardwareSensorPage::OnInitDialog() {
	CTabDlg::OnInitDialog();
#if !(WINRT_USE_FLAG)
	std::set<INT64> disabled_widgets = { IDC_CHECK_ENABLE_CPU_MON ,IDC_CHECK_ENABLE_GPU_MON };
	CheckDlgButton(IDC_CHECK_ENABLE_CPU_MON, false);
	CheckDlgButton(IDC_CHECK_ENABLE_GPU_MON, false);
	for (auto idc : disabled_widgets) {
		CButton* pCheckBox = (CButton*)GetDlgItem(idc);
		if (pCheckBox != nullptr)
		{
			// Disable the checkbox
			pCheckBox->EnableWindow(FALSE);
		}

	}
#endif
	// Generate and set the text for IDC_TEXT_DESCRIPTION
	SyncTreeViewContent();

	return TRUE;  // return TRUE unless you set the focus to a control
}


BEGIN_MESSAGE_MAP(HardwareSensorPage, CTabDlg)

	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_SENSOR, &HardwareSensorPage::OnTvnSelchangedSensorTree)
	ON_BN_CLICKED(IDC_BUTTON_REFRESH_HW_SENSORS, &HardwareSensorPage::OnBnClickedButtonRefreshHwSensors)
END_MESSAGE_MAP()


// HardwareSensorPage 消息处理程序

void HardwareSensorPage::SyncTreeViewContent() {




	auto const& vusf = PowerMon::Instance().GetValueUnitStringFormatter();

	auto m_TreeCtrl = (CTreeCtrl*)this->GetDlgItem(IDC_TREE_SENSOR);
	if (!m_TreeCtrl) {
		return;
	}
	m_TreeCtrl->DeleteAllItems();

	{
		HTREEITEM battery_info_node = m_TreeCtrl->InsertItem(L"Battery");



		auto const& btry_datasrc = PowerMon::Instance().GetBatteryInfoProvider();
		if (btry_datasrc.has_battery())

		{
			HTREEITEM battery_instance_node = m_TreeCtrl->InsertItem(
				L"Default", battery_info_node);
			// Adding information to the tree
			std::wstring powerRateStr = L"Power Rate: " +
				vusf.FormatRealValue(
					btry_datasrc.GetBatteryStatusPowerRate() / 1000.0f,
					vusf.GetPowerUnitString(), nullptr,
					false, 0,
					3);
			m_TreeCtrl->InsertItem(powerRateStr.c_str(), battery_instance_node);

			std::wstring capacityStr = L"Capacity: " + vusf.FormatRealValue(
				btry_datasrc.GetBatteryCapacity() / 1000.0f,
				vusf.GetPowerCapacityString(), nullptr,
				false, 0,
				3);
			m_TreeCtrl->InsertItem(capacityStr.c_str(), battery_instance_node);

			std::wstring voltageStr = L"Voltage: " + vusf.FormatRealValue(
				btry_datasrc.GetBatteryStatusVoltage() / 1000.0f,
				vusf.GetVoltageString(), nullptr,
				false, 0,
				3);
			m_TreeCtrl->InsertItem(voltageStr.c_str(), battery_instance_node);

			std::wstring acLineStatusStr = L"AC Line Status: " + std::to_wstring(btry_datasrc.GetACLineStatus());
			m_TreeCtrl->InsertItem(acLineStatusStr.c_str(), battery_instance_node);

			std::wstring batteryFlagStr = L"Battery Flag: " + std::to_wstring(btry_datasrc.GetSystemBatteryFlag());
			m_TreeCtrl->InsertItem(batteryFlagStr.c_str(), battery_instance_node);

			std::wstring batteryLifePercentStr = L"Battery Life Percent: " + vusf.FormatRealValue(
				btry_datasrc.GetSystemBatteryLifePercent(),
				L"%", nullptr,
				false, 0,
				0);
			m_TreeCtrl->InsertItem(batteryLifePercentStr.c_str(), battery_instance_node);

			std::wstring systemStatusFlagStr = L"System Status Flag: " + std::to_wstring(btry_datasrc.GetSystemStatusFlag());
			m_TreeCtrl->InsertItem(systemStatusFlagStr.c_str(), battery_instance_node);

			std::wstring batteryLifeTimeStr = L"Battery Life Time: " + std::to_wstring(btry_datasrc.GetBatteryLifeTime());
			m_TreeCtrl->InsertItem(batteryLifeTimeStr.c_str(), battery_instance_node);

			std::wstring batteryFullLifeTimeStr = L"Battery Full Life Time: " + std::to_wstring(btry_datasrc.GetBatteryFullLifeTime());
			m_TreeCtrl->InsertItem(batteryFullLifeTimeStr.c_str(), battery_instance_node);


			m_TreeCtrl->Expand(battery_instance_node, TVGN_NEXTVISIBLE);
		}
		m_TreeCtrl->Expand(battery_info_node, TVGN_NEXTVISIBLE);
	}


#if WINRT_USE_FLAG
	auto& datasrc = PowerMon::Instance().GetHWPowerSensorDataProvider();
	HTREEITEM cpu_info_node = m_TreeCtrl->InsertItem(L"CPU");

	HTREEITEM gpu_info_node = m_TreeCtrl->InsertItem(L"GPU");



	auto& cpu_data_map = datasrc.GetCPUSensorDataMap();
	auto& gpu_data_map = datasrc.GetGPUSensorDataMap();
	//Add HW content part
	{

		std::map<std::wstring, InterOpLibreHWMon::CPUHardwareRep>::iterator
			cpu_iterator;
		for (cpu_iterator = cpu_data_map.begin();
			cpu_iterator != cpu_data_map.end();
			cpu_iterator++)
		{
			HTREEITEM cpu_instance_node = m_TreeCtrl->InsertItem(
				cpu_iterator->second.GetName().c_str(), cpu_info_node);



			std::map<std::wstring, InterOpLibreHWMon::SensorRepresentiveBase>::iterator sensor_iter;

			for (sensor_iter = cpu_iterator->second.GetSensorsMap().begin();
				sensor_iter != cpu_iterator->second.GetSensorsMap().end();
				sensor_iter++) {
				auto& sensor = sensor_iter->second;
				std::wstringstream wss;
				wss << sensor.GetName() << L": " <<
					vusf.FormatRealValue(sensor.GetValue(),
						GetSensorTypeUnitString(sensor.GetSensorType()).c_str(), nullptr, false,
						0, 6, 1);

				HTREEITEM cpu_sensor_node = m_TreeCtrl->InsertItem(
					wss.str().c_str(), cpu_instance_node);
			}


			m_TreeCtrl->Expand(cpu_instance_node, TVGN_NEXTVISIBLE);
		}

		//delete& cpu_iterator;

		std::map<std::wstring, InterOpLibreHWMon::GPUHardwareRep>::iterator
			gpu_iterator;
		for (gpu_iterator = gpu_data_map.begin();
			gpu_iterator != gpu_data_map.end();
			gpu_iterator++)
		{
			HTREEITEM gpu_instance_node = m_TreeCtrl->InsertItem(
				gpu_iterator->second.GetName().c_str(), gpu_info_node);



			std::map<std::wstring, InterOpLibreHWMon::SensorRepresentiveBase>::iterator sensor_iter;

			for (sensor_iter = gpu_iterator->second.GetSensorsMap().begin();
				sensor_iter != gpu_iterator->second.GetSensorsMap().end();
				sensor_iter++) {
				auto& sensor = sensor_iter->second;
				std::wstringstream wss;
				wss << sensor.GetName() << L": " <<
					vusf.FormatRealValue(sensor.GetValue(),
						GetSensorTypeUnitString(sensor.GetSensorType()).c_str(), nullptr, false,
						0, 6, 1);

				HTREEITEM gpu_sensor_node = m_TreeCtrl->InsertItem(
					wss.str().c_str(), gpu_instance_node);
			}
			m_TreeCtrl->Expand(gpu_instance_node, TVGN_NEXTVISIBLE);
		}

		//delete& gpu_iterator;
		m_TreeCtrl->Expand(cpu_info_node, TVGN_NEXTVISIBLE);
		m_TreeCtrl->Expand(gpu_info_node, TVGN_NEXTVISIBLE);
	}
#endif
	return;

}




void HardwareSensorPage::OnTvnSelchangedSensorTree(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}



void HardwareSensorPage::OnBnClickedButtonRefreshHwSensors()
{
	// TODO: 在此添加控件通知处理程序代码

	SyncTreeViewContent();
}


