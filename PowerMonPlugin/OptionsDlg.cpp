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
	DDX_Control(pDX, IDC_TAB_MAIN, m_tab);
	CDialog::DoDataExchange(pDX);

}


BEGIN_MESSAGE_MAP(COptionsDlg, CDialog)
	ON_BN_CLICKED(IDOK, &COptionsDlg::OnBnClickedOk)
	ON_BN_CLICKED(ID_REVERT_CONFIGURATION, &COptionsDlg::OnBnClickedRevertConfiguration)


	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_MAIN, &COptionsDlg::OnTcnSelchangeTabMain)
END_MESSAGE_MAP()


// COptionsDlg 消息处理程序


BOOL COptionsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	{
		//为Tab Control增加两个页面
		m_tab.InsertItem(0, _T("单位设置"));
		m_tab.InsertItem(1, _T("信息"));

		//创建两个对话框
		unit_string_page.Create(IDD_UNIT_DISPLAY_PAGE, &m_tab);
		info_page.Create(IDD_INFO_PAGE, &m_tab);
		//设定在Tab内显示的范围
		CRect rc;
		m_tab.GetClientRect(rc);
		m_tab.AdjustRect(FALSE, &rc);

		//rc.top += 40;
		//rc.bottom -= 0;
		//rc.left += 0;
		//rc.right -= 0;
		unit_string_page.MoveWindow(&rc);
		info_page.MoveWindow(&rc);

		//把对话框对象指针保存起来
		pDialogs[0] = &unit_string_page;
		pDialogs[1] = &info_page;
		//显示初始页面
		pDialogs[0]->ShowWindow(SW_SHOW);
		pDialogs[1]->ShowWindow(SW_HIDE);
		//保存当前选择

	}

	// TODO:  在此添加额外的初始化
	{




		//unit_str_temp_store = m_data.pwr_unit_str;
		SyncWidgetWithSettingData();





	}



	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

int COptionsDlg::SyncWidgetWithSettingData() {

	CheckDlgButton(IDC_CHECKBOX_DBGMODE, m_data.is_dbg_mode);
	//SetDlgItemText(IDC_PWR_UNIT_STR_INPUT, m_data.pwr_unit_str.GetString());
	unit_string_page.SetDlgItemText(IDC_PWR_UNIT_STR_INPUT, m_data.pwr_unit_str.GetString());
	unit_string_page.SetDlgItemText(IDC_WH_UNIT_STR_INPUT, m_data.electric_capacity_unit_str.GetString());
	unit_string_page.SetDlgItemText(IDC_VOLT_UNIT_STR_INPUT, m_data.electric_voltage_unit_str.GetString());

	info_page.CheckDlgButton(IDC_DEBUG_MODE_SWITCH, m_data.is_dbg_mode);

	return 0;
}


int COptionsDlg::SyncSettingDataWithWidget() {

	m_data.is_dbg_mode = (bool)info_page.IsDlgButtonChecked(IDC_CHECKBOX_DBGMODE);

	wchar_t unit_str_temp_store[PWR_UNIT_STR_MAXLEN + 1] = L"W";

	//PWR Unit
	unit_string_page.GetDlgItemText(IDC_PWR_UNIT_STR_INPUT, unit_str_temp_store, PWR_UNIT_STR_MAXLEN);
	StrCpyNW(m_data.pwr_unit_str.GetBuffer(PWR_UNIT_STR_MAXLEN + 1),
		unit_str_temp_store, PWR_UNIT_STR_MAXLEN);

	unit_string_page.GetDlgItemText(IDC_WH_UNIT_STR_INPUT, unit_str_temp_store, PWR_UNIT_STR_MAXLEN);
	StrCpyNW(m_data.electric_capacity_unit_str.GetBuffer(PWR_UNIT_STR_MAXLEN + 1),
		unit_str_temp_store, PWR_UNIT_STR_MAXLEN);

	unit_string_page.GetDlgItemText(IDC_VOLT_UNIT_STR_INPUT, unit_str_temp_store, PWR_UNIT_STR_MAXLEN);
	StrCpyNW(m_data.electric_voltage_unit_str.GetBuffer(PWR_UNIT_STR_MAXLEN + 1),
		unit_str_temp_store, PWR_UNIT_STR_MAXLEN);

	SyncWidgetWithSettingData();

	return 0;
}



void COptionsDlg::OnBnClickedOk()
{

	SyncSettingDataWithWidget();

	if (m_data.is_dbg_mode) {
		std::wstringstream info_str;
		info_str << "Setting Debug Mode:" << g_data.m_setting_data.is_dbg_mode << std::endl;
		info_str << "Debug Mode:" << PowerMon::Instance().is_dbg << std::endl;

		info_str << "PWR Unit Str:" << m_data.pwr_unit_str.GetString() << std::endl;

		::MessageBox(GetSafeHwnd(), info_str.str().c_str(), L"Debug Config Info", MB_OK | MB_ICONINFORMATION);


	}

	//ExportConfig
	m_data.settings_altered_counter += 1;

	CDialog::OnOK();
}



void COptionsDlg::OnBnClickedRevertConfiguration()
{
	// TODO: 在此添加控件通知处理程序代码



	int msgboxID = ::MessageBox(GetSafeHwnd(),
		g_data.StringRes(IDS_REVERT_SETTING_CONFIG),
		g_data.StringRes(IDS_WARNING),
		MB_OKCANCEL | MB_ICONINFORMATION);

	if (msgboxID == IDOK) {
		//inf_obj.GetBatteryInfoProvider().ReleaseResources();
		g_data.m_setting_data.CopyTo(&m_data);
		SyncWidgetWithSettingData();
	}


}



void COptionsDlg::OnTcnSelchangeTabMain(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: 在此添加控件通知处理程序代码
		//把当前的页面隐藏起来
	pDialogs[m_CurSelTab]->ShowWindow(SW_HIDE);
	//得到新的页面索引
	m_CurSelTab = m_tab.GetCurSel();
	//把新的页面显示出来
	pDialogs[m_CurSelTab]->ShowWindow(SW_SHOW);

	*pResult = 0;
}
