#pragma once
#include "afxwin.h"


// CPlayerSoldier �Ի���

class CPlayerSoldier : public CDialog
{
	DECLARE_DYNCREATE(CPlayerSoldier)

public:
	CPlayerSoldier(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPlayerSoldier();

// �Ի�������
	enum { IDD = IDD_DIALOG_SOLDIER};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_edHuman;
	CEdit m_edManikin;
	CEdit m_edFairy;
	CEdit m_edDandiprat;
	CEdit m_edHalfMan;
	CEdit m_edCattle;
	CEdit m_edBlackFairy;
	CEdit m_edDecPerson;
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBnClickedAddsoldier();
	CStatic m_stHint;
};
