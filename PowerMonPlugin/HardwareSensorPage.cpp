// HardwareSensorPage.cpp: 实现文件
//

#include "pch.h"
#include "afxdialogex.h"
#include "HardwareSensorPage.h"
#include "PowerMon.h"


// HardwareSensorPage 对话框

IMPLEMENT_DYNAMIC(HardwareSensorPage, CDialogEx)

HardwareSensorPage::HardwareSensorPage(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_HARDWARE_SENSOR_PAGE, pParent)
{
	//SyncTreeViewContent();
}

HardwareSensorPage::~HardwareSensorPage()
{
}

void HardwareSensorPage::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(HardwareSensorPage, CDialogEx)

	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_SENSOR, &HardwareSensorPage::OnTvnSelchangedSensorTree)
	ON_BN_CLICKED(IDC_BUTTON_REFRESH_HW_SENSORS, &HardwareSensorPage::OnBnClickedButtonRefreshHwSensors)
END_MESSAGE_MAP()


// HardwareSensorPage 消息处理程序

void HardwareSensorPage::SyncTreeViewContent() {
#if WINRT_USE_FLAG

	auto& datasrc = PowerMon::Instance().GetHWPowerSensorDataProvider();

	auto m_TreeCtrl = (CTreeCtrl*)this->GetDlgItem(IDC_TREE_SENSOR);
	if (!m_TreeCtrl) {
		return;
	}
	m_TreeCtrl->DeleteAllItems();

	HTREEITEM cpu_info_node = m_TreeCtrl->InsertItem(L"CPU");
	m_TreeCtrl->Expand(cpu_info_node, TVGN_NEXTVISIBLE);
	HTREEITEM gpu_info_node = m_TreeCtrl->InsertItem(L"GPU");
	m_TreeCtrl->Expand(gpu_info_node, TVGN_NEXTVISIBLE);

	auto& cpu_data_map = datasrc.GetCPUSensorDataMap();
	auto& gpu_data_map = datasrc.GetGPUSensorDataMap();
	//Add content part
	{

		std::map<std::wstring, InterOpLibreHWMon::CPUHardwareRep>::iterator
			cpu_iterator;
		for (cpu_iterator = cpu_data_map.begin();
			cpu_iterator != cpu_data_map.end();
			cpu_iterator++)
		{
			HTREEITEM cpu_instance_node = m_TreeCtrl->InsertItem(
				cpu_iterator->second.GetName().c_str(), cpu_info_node);

			m_TreeCtrl->Expand(cpu_instance_node, TVGN_NEXTVISIBLE);

			std::map<std::wstring, InterOpLibreHWMon::SensorRepresentiveBase>::iterator sensor_iter;

			for (sensor_iter = cpu_iterator->second.GetSensorsMap().begin();
				sensor_iter != cpu_iterator->second.GetSensorsMap().end();
				sensor_iter++) {
				auto& sensor = sensor_iter->second;
				std::wstringstream wss;
				wss << sensor.GetName() << L": " << sensor.GetValue() << GetSensorTypeUnitString(sensor.GetSensorType());

				HTREEITEM cpu_sensor_node = m_TreeCtrl->InsertItem(
					wss.str().c_str(), cpu_instance_node);
			}
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

			m_TreeCtrl->Expand(gpu_instance_node, TVGN_NEXTVISIBLE);

			std::map<std::wstring, InterOpLibreHWMon::SensorRepresentiveBase>::iterator sensor_iter;

			for (sensor_iter = gpu_iterator->second.GetSensorsMap().begin();
				sensor_iter != gpu_iterator->second.GetSensorsMap().end();
				sensor_iter++) {
				auto& sensor = sensor_iter->second;
				std::wstringstream wss;
				wss << sensor.GetName() << L": " << sensor.GetValue() << GetSensorTypeUnitString(sensor.GetSensorType());

				HTREEITEM gpu_sensor_node = m_TreeCtrl->InsertItem(
					wss.str().c_str(), gpu_instance_node);
			}
		}

		//delete& gpu_iterator;

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
