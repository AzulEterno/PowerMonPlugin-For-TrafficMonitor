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

void UnitStringSettingPage::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int nDelta;
	switch (nSBCode)
	{
	case SB_LINEUP:
		nDelta = -1;
		break;
	case SB_LINEDOWN:
		nDelta = 1;
		break;
	case SB_PAGEUP:
		nDelta = -10;
		break;
	case SB_PAGEDOWN:
		nDelta = 10;
		break;
	case SB_THUMBTRACK:
		nDelta = (int)nPos - m_nScrollPos;
		break;
	default:
		return;
	}

	int nNewScrollPos = m_nScrollPos + nDelta;
	nNewScrollPos = max(0, min(nNewScrollPos, m_nScrollMax));
	if (nNewScrollPos != m_nScrollPos)
	{
		SetScrollPos(SB_VERT, nNewScrollPos, TRUE);
		ScrollWindow(0, m_nScrollPos - nNewScrollPos);
		m_nScrollPos = nNewScrollPos;
	}
}

void UnitStringSettingPage::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// Adjust the scroll bar as needed when the window is resized
	SCROLLINFO scrollInfo;
	scrollInfo.cbSize = sizeof(SCROLLINFO);
	scrollInfo.fMask = SIF_ALL;
	scrollInfo.nMin = 0;
	scrollInfo.nMax = m_nScrollMax;
	scrollInfo.nPage = cy;
	scrollInfo.nPos = m_nScrollPos;
	SetScrollInfo(SB_VERT, &scrollInfo, TRUE);
}

void UnitStringSettingPage::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BOOL UnitStringSettingPage::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	{

		// Initialize scroll bar
		m_nScrollPos = 0;
		m_nScrollMax = 100; // Set the maximum scroll range

		SCROLLINFO scrollInfo;
		scrollInfo.cbSize = sizeof(SCROLLINFO);
		scrollInfo.fMask = SIF_ALL;
		scrollInfo.nMin = 0;
		scrollInfo.nMax = m_nScrollMax;
		scrollInfo.nPage = 10; // Set the page size
		scrollInfo.nPos = m_nScrollPos;
		SetScrollInfo(SB_VERT, &scrollInfo, TRUE);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
}


BEGIN_MESSAGE_MAP(UnitStringSettingPage, CDialogEx)
END_MESSAGE_MAP()


// UnitStringSettingPage 消息处理程序
