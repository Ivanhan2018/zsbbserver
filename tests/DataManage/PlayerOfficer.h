#pragma once
#include "afxwin.h"

#include "../include/DB_Proxy.h"
#include "../include/DB_Table.h"

// CPlayerOfficer 对话框

class CPlayerOfficer : public CDialog
{
	DECLARE_DYNAMIC(CPlayerOfficer)

public:
	CPlayerOfficer(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPlayerOfficer();

// 对话框数据
	enum { IDD = IDD_DIALOG_HEROATTRI };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
private:
	int HeroCredit(const int lCredit);
	DECLARE_MESSAGE_MAP()
public:
	CStatic m_stHint;
	CEdit m_edLevel;
	CEdit m_edStamina;
	CEdit m_edPower;
	CEdit m_edCelerity;
	CEdit m_edWit;
	CEdit m_edTechDot;
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBnClickedModifyhero();
	CEdit m_edHeroID;
	CComboBox m_cbStateList;
	afx_msg void OnEnChangeEditHeroid();
	CEdit m_edHeroState;
	afx_msg void OnBnClickedButtonState();
	CEdit m_edMoney;
	CEdit m_edScience;
	afx_msg void OnBnClickedAddplayer();
	afx_msg void OnEnChangeEditLevel();
	CButton m_btnModify;
	afx_msg void OnBnClickedButtonReset();
	CEdit m_edName;
	
	PlayerOfficerTable offtb;
	CEdit m_edExp;
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButton1();
	CEdit m_edCredit;
	CEdit m_edActionDot;
};
