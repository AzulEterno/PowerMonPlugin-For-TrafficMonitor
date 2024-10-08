#pragma once
#include "afxdialogex.h"


// InformationPage 对话框

class InformationPage : public CDialogEx
{
	DECLARE_DYNAMIC(InformationPage)

public:
	InformationPage(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~InformationPage();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_INFO_PAGE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonRepoUrl();
	afx_msg void OnStnClickedTextDescription();
	afx_msg void OnBnClickedButtonBtrdriver();
	BOOL OnInitDialog() override;

	CString GenerateInfoText();
};
