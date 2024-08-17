// InformationPage.cpp: 实现文件
//

#include "pch.h"
#include "afxdialogex.h"
#include "InformationPage.h"
#include "PowerMon.h"


// InformationPage 对话框

IMPLEMENT_DYNAMIC(InformationPage, CDialogEx)

InformationPage::InformationPage(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_INFO_PAGE, pParent)
{

}

InformationPage::~InformationPage()
{
}

void InformationPage::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(InformationPage, CDialogEx)
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
