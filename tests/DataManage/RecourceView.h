#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CRecourceView 对话框

class CRecourceView : public CDialog
{
	DECLARE_DYNAMIC(CRecourceView)

public:
	CRecourceView(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRecourceView();

// 对话框数据
	enum { IDD = IDD_DIALOG_RESOURCEVIEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_edFood;
	CEdit m_edWood;
	CEdit m_edSkin;
	CEdit m_edIron;
	CEdit m_edYStone;
	CEdit m_edRStone;
	CEdit m_edGStone;
	CEdit m_edBlackIron;
	CListCtrl m_lstAmount;
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	CStatic m_csInfo;
	afx_msg void OnBnClickedButtonQuery();
	CEdit m_edContent;
	CEdit m_edTotal;
	afx_msg void OnBnClickedButtonAdd();
	CEdit m_edSum;
	afx_msg void OnBnClickedButtonReset();
	int	m_iCurItem;
	afx_msg void OnNMClickListAmount(NMHDR *pNMHDR, LRESULT *pResult);
	CButton m_btnReset;
};
