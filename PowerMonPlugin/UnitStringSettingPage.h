#pragma once
#include "afxdialogex.h"


// UnitStringSettingPage 对话框

class UnitStringSettingPage : public CDialogEx
{
	DECLARE_DYNAMIC(UnitStringSettingPage)

public:
	UnitStringSettingPage(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~UnitStringSettingPage();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_UNIT_DISPLAY_PAGE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
