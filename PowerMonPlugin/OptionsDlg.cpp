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
		m_tab.InsertItem(0, L"单位设置");
		m_tab.InsertItem(1, L"硬件监控");
		m_tab.InsertItem(2, L"信息");

		//创建两个对话框
		unit_string_page.Create(IDD_UNIT_DISPLAY_PAGE, &m_tab);
		info_page.Create(IDD_INFO_PAGE, &m_tab);
		hw_sensor_page.Create(IDD_HARDWARE_SENSOR_PAGE, &m_tab);

		//设定在Tab内显示的范围
		CRect rc;
		m_tab.GetClientRect(rc);
		m_tab.AdjustRect(FALSE, &rc);

		//rc.top += 40;
		//rc.bottom -= 0;
		//rc.left += 0;
		//rc.right -= 0;
		unit_string_page.MoveWindow(&rc);
		hw_sensor_page.MoveWindow(&rc);
		info_page.MoveWindow(&rc);

		//把对话框对象指针保存起来
		pDialogs[0] = &unit_string_page;
		pDialogs[1] = &hw_sensor_page;
		pDialogs[2] = &info_page;
		//显示初始页面
		pDialogs[0]->ShowWindow(SW_SHOW);
		pDialogs[1]->ShowWindow(SW_HIDE);
		pDialogs[2]->ShowWindow(SW_HIDE);
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

	//CheckDlgButton(IDC_CHECKBOX_DBGMODE, m_data.is_dbg_mode);
	//SetDlgItemText(IDC_INPUT_PWR_UNIT_STR, m_data.pwr_unit_str.GetString());
	unit_string_page.SetDlgItemText(IDC_INPUT_PWR_UNIT_STR, m_data.pwr_unit_str.GetString());
	unit_string_page.SetDlgItemText(IDC_INPUT_WH_UNIT_STR, m_data.electric_capacity_unit_str.GetString());
	unit_string_page.SetDlgItemText(IDC_INPUT_VOLT_UNIT_STR, m_data.electric_voltage_unit_str.GetString());

	unit_string_page.SetDlgItemText(IDC_INPUT_HOUR_UNIT_STR, m_data.hour_unit_str.GetString());
	unit_string_page.SetDlgItemText(IDC_INPUT_MINUTE_UNIT_STR, m_data.minute_unit_str.GetString());

	unit_string_page.SetDlgItemText(IDC_INPUT_NAN_STR, m_data.nan_str.GetString());





	info_page.CheckDlgButton(IDC_CHECK_DEBUG_MODE, m_data.is_dbg_mode);




#if WINRT_USE_FLAG
	hw_sensor_page.CheckDlgButton(IDC_CHECK_ENABLE_CPU_MON, m_data.enable_cpu_monitor);
	hw_sensor_page.CheckDlgButton(IDC_CHECK_ENABLE_GPU_MON, m_data.enable_gpu_monitor);

#else
	hw_sensor_page.CheckDlgButton(IDC_CHECK_ENABLE_CPU_MON, false);
	hw_sensor_page.CheckDlgButton(IDC_CHECK_ENABLE_GPU_MON, false);

#endif



	unit_string_page.SetDlgItemInt(IDC_INPUT_SPACING_SIZE, m_data.default_value_unit_space, FALSE);
	unit_string_page.SetDlgItemInt(IDC_INPUT_DISPLAY_MAX_DECI_PLACES, m_data.default_max_adaptive_decimal_places, FALSE);

	return 0;
}


int COptionsDlg::SyncSettingDataWithWidget() {

	m_data.is_dbg_mode = (bool)info_page.IsDlgButtonChecked(IDC_CHECK_DEBUG_MODE);

	BOOL bTranslated = FALSE;
	m_data.default_value_unit_space = unit_string_page.GetDlgItemInt(IDC_INPUT_SPACING_SIZE, &bTranslated, FALSE);

	if (!bTranslated || m_data.default_value_unit_space < 0) {
		m_data.default_value_unit_space = 1;
	}
	bTranslated = FALSE;
	m_data.default_max_adaptive_decimal_places = unit_string_page.GetDlgItemInt(IDC_INPUT_DISPLAY_MAX_DECI_PLACES, &bTranslated, FALSE);

	if (!bTranslated || m_data.default_max_adaptive_decimal_places < 0) {
		m_data.default_max_adaptive_decimal_places = 2;
	}


#if WINRT_USE_FLAG
	m_data.enable_cpu_monitor = (bool)hw_sensor_page.IsDlgButtonChecked(IDC_CHECK_ENABLE_CPU_MON);
	m_data.enable_gpu_monitor = (bool)hw_sensor_page.IsDlgButtonChecked(IDC_CHECK_ENABLE_GPU_MON);
#else
	//Disable related options.
	m_data.enable_cpu_monitor = false;
	m_data.enable_gpu_monitor = false;
#endif
	wchar_t unit_str_temp_store[UNIT_STR_MAXLEN + 1] = L"";

	//PWR Unit
	unit_string_page.GetDlgItemText(IDC_INPUT_PWR_UNIT_STR, unit_str_temp_store, UNIT_STR_MAXLEN);
	StrCpyNW(m_data.pwr_unit_str.GetBuffer(UNIT_STR_MAXLEN + 1),
		unit_str_temp_store, UNIT_STR_MAXLEN);

	unit_string_page.GetDlgItemText(IDC_INPUT_WH_UNIT_STR, unit_str_temp_store, UNIT_STR_MAXLEN);
	StrCpyNW(m_data.electric_capacity_unit_str.GetBuffer(UNIT_STR_MAXLEN + 1),
		unit_str_temp_store, UNIT_STR_MAXLEN);

	unit_string_page.GetDlgItemText(IDC_INPUT_VOLT_UNIT_STR, unit_str_temp_store, UNIT_STR_MAXLEN);
	StrCpyNW(m_data.electric_voltage_unit_str.GetBuffer(UNIT_STR_MAXLEN + 1),
		unit_str_temp_store, UNIT_STR_MAXLEN);

	unit_string_page.GetDlgItemText(IDC_INPUT_HOUR_UNIT_STR, unit_str_temp_store, UNIT_STR_MAXLEN);
	StrCpyNW(m_data.hour_unit_str.GetBuffer(UNIT_STR_MAXLEN + 1),
		unit_str_temp_store, UNIT_STR_MAXLEN);

	unit_string_page.GetDlgItemText(IDC_INPUT_MINUTE_UNIT_STR, unit_str_temp_store, UNIT_STR_MAXLEN);
	StrCpyNW(m_data.minute_unit_str.GetBuffer(UNIT_STR_MAXLEN + 1),
		unit_str_temp_store, UNIT_STR_MAXLEN);

	unit_string_page.GetDlgItemText(IDC_INPUT_NAN_STR, unit_str_temp_store, UNIT_STR_MAXLEN);
	StrCpyNW(m_data.nan_str.GetBuffer(UNIT_STR_MAXLEN + 1),
		unit_str_temp_store, UNIT_STR_MAXLEN);

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
	//Save immediately 
	//g_data.SaveConfig();
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
