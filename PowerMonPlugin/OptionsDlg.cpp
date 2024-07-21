// OptionsDlg.cpp: 实现文件
//

#include "pch.h"
#include "PowerMon.h"
#include "OptionsDlg.h"
#include "afxdialogex.h"
#include "DataManager.h"

// COptionsDlg 对话框

IMPLEMENT_DYNAMIC(COptionsDlg, CDialog)

COptionsDlg::COptionsDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_OPTIONS_DIALOG, pParent)
{
}

COptionsDlg::~COptionsDlg()
{
}

void COptionsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(COptionsDlg, CDialog)
	ON_BN_CLICKED(IDC_CHECKBOX_DBGMODE, &COptionsDlg::OnBnClickedCheckboxDbgmode)
	ON_BN_CLICKED(IDC_BUTTON_BTRDRIVER, &COptionsDlg::OnBnClickedButtonBtrdriver)
	ON_EN_CHANGE(IDC_PWR_UNIT_STR_INPUT, &COptionsDlg::OnEnChangePwrUnitStrInput)
	ON_BN_CLICKED(IDOK, &COptionsDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// COptionsDlg 消息处理程序


BOOL COptionsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	{
		CheckDlgButton(IDC_CHECKBOX_DBGMODE, m_data.is_dbg_mode);

		//unit_str_temp_store = m_data.pwr_unit_str;


		SetDlgItemText(IDC_PWR_UNIT_STR_INPUT, m_data.pwr_unit_str.GetString());
	}



	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void COptionsDlg::OnBnClickedCheckboxDbgmode()
{
	// TODO: 在此添加控件通知处理程序代码
	//m_data.is_dbg_mode = (bool)IsDlgButtonChecked(IDC_CHECKBOX_DBGMODE);



	//MessageBox(NULL, PowerMon::Instance().is_dbg, L"调试信息窗口",
	//    MB_ICONWARNING | MB_OKCANCEL | MB_DEFBUTTON2);
}




void COptionsDlg::OnBnClickedButtonBtrdriver()
{
	// TODO: 在此添加控件通知处理程序代码

	{
		PowerMon& inf_obj = PowerMon::Instance();
		std::wstringstream info_str;


		info_str << "Current Battery Handler:" << std::hex << (void*)inf_obj.hBattery << std::endl;
		info_str << "Current PSP_DEVICE_INTERFACE_DETAIL_DATA:" << std::hex << (void*)inf_obj.pdidd << std::endl;
		info_str << "Current BATTERY_WAIT_STATUS:" << std::hex << (void*)&inf_obj.qry_bws_inf << std::endl;
		info_str << "Current BATTERY_TAG:" << inf_obj.qry_bws_inf.BatteryTag << std::endl;
		info_str << "Debug Mode:" << inf_obj.is_dbg << std::endl;
		info_str << "Press OK to reset battery driver." << std::endl;


		int msgboxID = ::MessageBox(NULL, info_str.str().c_str(), L"Debug Information",
			MB_OK | MB_ICONINFORMATION);

		if (msgboxID == IDOK) {
			inf_obj.free_res_mem();
			inf_obj.update_battery_base_info();
		}
		//return 1;

	}
}


void COptionsDlg::OnEnChangePwrUnitStrInput()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码


}


void COptionsDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	m_data.is_dbg_mode = (bool)IsDlgButtonChecked(IDC_CHECKBOX_DBGMODE);

	wchar_t unit_str_temp_store[PWR_UNIT_STR_MAXLEN + 1] = L"W";

	GetDlgItemText(IDC_PWR_UNIT_STR_INPUT, unit_str_temp_store, PWR_UNIT_STR_MAXLEN);


	StrCpy(m_data.pwr_unit_str.GetBuffer(PWR_UNIT_STR_MAXLEN + 1), unit_str_temp_store);

	if (m_data.is_dbg_mode) {
		std::wstringstream info_str;
		info_str << "Setting Debug Mode:" << g_data.m_setting_data.is_dbg_mode << std::endl;
		info_str << "Debug Mode:" << PowerMon::Instance().is_dbg << std::endl;

		info_str << "PWR Unit Str:" << m_data.pwr_unit_str.GetString() << std::endl;

		::MessageBox(NULL, info_str.str().c_str(), L"Debug Config Info", MB_OK | MB_ICONINFORMATION);


	}


	CDialog::OnOK();
}
