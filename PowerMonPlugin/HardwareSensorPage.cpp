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

	return TRUE;  // return TRUE unless you set the focus to a control
}


BEGIN_MESSAGE_MAP(HardwareSensorPage, CTabDlg)
	ON_BN_CLICKED(IDC_CHECK_ENABLE_POWER_FILTER, &HardwareSensorPage::OnBnClickedCheckEnablePowerFilter)
END_MESSAGE_MAP()


// HardwareSensorPage 消息处理程序

void HardwareSensorPage::SyncPowerFilterControls() {
	CWnd* p_input = GetDlgItem(IDC_INPUT_MAX_POWER);
	if (p_input != nullptr) {
		p_input->EnableWindow(IsDlgButtonChecked(IDC_CHECK_ENABLE_POWER_FILTER) == BST_CHECKED);
	}
}

void HardwareSensorPage::OnBnClickedCheckEnablePowerFilter() {
	SyncPowerFilterControls();
}