#pragma once
#include "afxwin.h"


// CPlayerResource 对话框

class CPlayerResource : public CDialog
{
	DECLARE_DYNAMIC(CPlayerResource)

public:
	CPlayerResource(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPlayerResource();

// 对话框数据
	enum { IDD = IDD_DIALOG_RESOURCE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedAddresource();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	CEdit m_edFood;
	CEdit m_edWood;
	CEdit m_edSkin;
	CEdit m_edIron;
	CEdit m_edRedStone;
	CEdit m_edYellowStone;
	CEdit m_edGreenStone;
	CEdit m_edBlackIron;
	CStatic m_stHint;
};
