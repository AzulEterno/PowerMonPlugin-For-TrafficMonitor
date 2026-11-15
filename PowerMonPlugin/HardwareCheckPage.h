#pragma once
#include "afxdialogex.h"
#include "TabDlg.h"


// HardwareCheckPage 对话框
// Displays a read-only device tree showing all detected sensors and their current values.

class HardwareCheckPage : public CTabDlg
{
	DECLARE_DYNAMIC(HardwareCheckPage)

public:
	HardwareCheckPage(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~HardwareCheckPage();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HARDWARE_CHECK_PAGE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	void SyncTreeViewContent();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTvnSelchangedSensorTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedButtonRefreshHwSensors();

	BOOL OnInitDialog() override;
};
