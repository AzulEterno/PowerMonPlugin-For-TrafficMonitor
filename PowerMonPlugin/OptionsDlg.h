#pragma once
#include "DataManager.h"

#include "windows.h" 
#include "UnitStringSettingPage.h"
#include "InformationPage.h"
// COptionsDlg 对话框

class COptionsDlg : public CDialog
{
	DECLARE_DYNAMIC(COptionsDlg)

public:
	COptionsDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~COptionsDlg();

	SettingData m_data;

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OPTIONS_DIALOG };
#endif

private:

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()


	CTabCtrl m_tab;
	int m_CurSelTab;
	UnitStringSettingPage unit_string_page;
	InformationPage info_page;
	CDialog* pDialogs[2];
public:
	virtual BOOL OnInitDialog();
	int SyncWidgetWithSettingData();
	int SyncSettingDataWithWidget();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedRevertConfiguration();
	afx_msg void OnTcnSelchangeTabMain(NMHDR* pNMHDR, LRESULT* pResult);
};
