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
END_MESSAGE_MAP()


// COptionsDlg 消息处理程序


BOOL COptionsDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO:  在此添加额外的初始化
    {
        CheckDlgButton(IDC_CHECKBOX_DBGMODE, m_data.is_dbg_mode);
    }



    return TRUE;  // return TRUE unless you set the focus to a control
                  // 异常: OCX 属性页应返回 FALSE
}


void COptionsDlg::OnBnClickedCheckboxDbgmode()
{
    // TODO: 在此添加控件通知处理程序代码
    m_data.is_dbg_mode = (bool)IsDlgButtonChecked(IDC_CHECKBOX_DBGMODE);
    /*

    std::wstringstream info_str;
    info_str << "Setting Debug Mode:" << g_data.m_setting_data.is_dbg_mode;
    info_str << "Debug Mode:" << PowerMon::Instance().is_dbg;


    MessageBox(NULL, info_str.str().c_str(),MB_OK | MB_DEFBUTTON2);*/
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
            MB_ICONWARNING | MB_OKCANCEL | MB_DEFBUTTON2);

        if (msgboxID == IDOK) {
            inf_obj.free_res_mem();
            inf_obj.update_battery_base_info();
        }
        //return 1;

    }
}
