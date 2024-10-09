// InformationPage.cpp: 实现文件
//

#include "pch.h"
#include "afxdialogex.h"
#include "InformationPage.h"
#include "PowerMon.h"
#include <sysinfoapi.h>


// InformationPage 对话框

IMPLEMENT_DYNAMIC(InformationPage, CTabDlg)

InformationPage::InformationPage(CWnd* pParent /*=nullptr*/)
	: CTabDlg(IDD_INFO_PAGE, pParent)
{

}

InformationPage::~InformationPage()
{
}

void InformationPage::DoDataExchange(CDataExchange* pDX)
{
	CTabDlg::DoDataExchange(pDX);
}
BOOL InformationPage::OnInitDialog() {
	CTabDlg::OnInitDialog();

	// Generate and set the text for IDC_TEXT_DESCRIPTION
	CString descriptionText = GenerateInfoText();
	GetDlgItem(IDC_TEXT_DESCRIPTION)->SetWindowText(descriptionText);

	return TRUE;  // return TRUE unless you set the focus to a control
}

BEGIN_MESSAGE_MAP(InformationPage, CTabDlg)
	ON_BN_CLICKED(IDC_BUTTON_REPO_URL, &InformationPage::OnBnClickedButtonRepoUrl)
	ON_STN_CLICKED(IDC_TEXT_DESCRIPTION, &InformationPage::OnStnClickedTextDescription)
	ON_BN_CLICKED(IDC_BUTTON_BTRDRIVER, &InformationPage::OnBnClickedButtonBtrdriver)
END_MESSAGE_MAP()


// InformationPage 消息处理程序


void InformationPage::OnBnClickedButtonRepoUrl()
{
	// TODO: 在此添加控件通知处理程序代码
	CString url = _T("https://github.com/AzulEterno/PowerMonPlugin-For-TrafficMonitor");
	ShellExecute(NULL, _T("open"), url, NULL, NULL, SW_SHOWNORMAL);

}


void InformationPage::OnStnClickedTextDescription()
{
	// TODO: 在此添加控件通知处理程序代码
}


void InformationPage::OnBnClickedButtonBtrdriver()
{
	// TODO: 在此添加控件通知处理程序代码
	{
		PowerMon& inf_obj = PowerMon::Instance();
		std::wstringstream info_str;


		info_str << inf_obj.GetBatteryInfoProvider().GenerateDebugString();
		info_str << "Press OK to reset battery driver." << std::endl;

		int msgboxID = ::MessageBox(GetSafeHwnd(), info_str.str().c_str(), L"Debug Information",
			MB_OKCANCEL | MB_ICONINFORMATION);

		if (msgboxID == IDOK) {
			//inf_obj.GetBatteryInfoProvider().ReleaseResources();
			inf_obj.GetBatteryInfoProvider().InitBatteryDriver(true);
		}
		//return 1;

	}
}


CString InformationPage::GenerateInfoText() {
	// Retrieve the Windows version
	OSVERSIONINFO osvi;
	ZeroMemory(&osvi, sizeof(osvi));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

	GetVersionEx(&osvi);

	// Retrieve PowerMon version
	auto version_str = PowerMon::Instance().GetInfo(ITMPlugin::PluginInfoIndex::TMI_VERSION);

	// Format the text
	CString infoText;
	infoText.Format(
		L"Windows Version: %d.%d.%d\n"
		L"Build: %d\n"
		L"Service Pack: %s\n"
		L"PowerMon version: %s, %s\n"
#if WINRT_USE_FLAG
		L"WinRT enabled version of PowerMon.\n"
#else
		L"WinRT disabled version of PowerMon.\n"
#endif
		,
		osvi.dwMajorVersion,
		osvi.dwMinorVersion,
		osvi.dwBuildNumber,
		osvi.dwBuildNumber,
		osvi.szCSDVersion,
		version_str,
		PLATFORM_STR
	);


	return infoText;
}