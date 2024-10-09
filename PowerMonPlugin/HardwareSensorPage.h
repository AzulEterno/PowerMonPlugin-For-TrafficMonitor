#pragma once
#include "afxdialogex.h"
#include "TabDlg.h"


// HardwareSensorPage 对话框

class HardwareSensorPage : public CTabDlg
{
	DECLARE_DYNAMIC(HardwareSensorPage)

public:
	HardwareSensorPage(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~HardwareSensorPage();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HARDWARE_SENSOR_PAGE };
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
