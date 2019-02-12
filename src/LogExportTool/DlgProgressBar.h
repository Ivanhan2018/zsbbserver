#pragma once
#include "afxcmn.h"


// CDlgProgressBar 对话框

class CDlgProgressBar : public CDialog
{
	DECLARE_DYNAMIC(CDlgProgressBar)

public:
	CDlgProgressBar(bool &isCancel, bool isUserInLoadDownLog, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgProgressBar();

// 对话框数据
	enum { IDD = IDD_DLG_PROGRESS_BAR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnCancel();

	void setProgressBarPos(int iPos);
public:
	CProgressCtrl m_progressCtl;
	bool &m_isCancel;
	bool m_isUserInLoadDownLog;
};
