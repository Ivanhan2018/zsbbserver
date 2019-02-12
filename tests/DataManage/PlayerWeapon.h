#pragma once
#include "afxwin.h"


// CPlayerWeapon 对话框

class CPlayerWeapon : public CDialog
{
	DECLARE_DYNAMIC(CPlayerWeapon)

public:
	CPlayerWeapon(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPlayerWeapon();

// 对话框数据
	enum { IDD = IDD_DIALOG_WEAPON };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	//CEdit m_edItemID;
	CStatic m_stHint;
	afx_msg void OnBnClickedAdditem();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	CEdit m_edCount;
	//CComboBox m_cbTypeList;
	afx_msg void OnBnClickedButtonDelete();
	CComboBox m_cbWeaponList;
};
