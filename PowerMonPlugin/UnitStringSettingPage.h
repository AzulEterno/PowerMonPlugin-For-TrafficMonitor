


#pragma once
#include "TabDlg.h"
#include "afxdialogex.h"


// UnitStringSettingPage 对话框

class UnitStringSettingPage : public CTabDlg
{
	DECLARE_DYNAMIC(UnitStringSettingPage)

public:
	UnitStringSettingPage(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~UnitStringSettingPage();

	BOOL OnInitDialog() override;

	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_UNIT_DISPLAY_PAGE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持



	CScrollBar m_VerticalScrollBar;
	int m_nScrollPos;
	int m_nScrollMax;

	DECLARE_MESSAGE_MAP()
};
