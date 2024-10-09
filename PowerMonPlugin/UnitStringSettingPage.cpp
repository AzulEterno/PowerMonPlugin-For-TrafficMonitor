// UnitStringSettingPage.cpp: 实现文件
//

#include "pch.h"
#include "afxdialogex.h"
#include "UnitStringSettingPage.h"


// UnitStringSettingPage 对话框

IMPLEMENT_DYNAMIC(UnitStringSettingPage, CDialogEx)

UnitStringSettingPage::UnitStringSettingPage(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_UNIT_DISPLAY_PAGE, pParent)
{

}

UnitStringSettingPage::~UnitStringSettingPage()
{
}

void UnitStringSettingPage::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BOOL UnitStringSettingPage::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
}


BEGIN_MESSAGE_MAP(UnitStringSettingPage, CDialogEx)
END_MESSAGE_MAP()


// UnitStringSettingPage 消息处理程序
