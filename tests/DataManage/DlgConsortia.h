#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CDlgConsortia �Ի���

class CDlgConsortia : public CDialog
{
	DECLARE_DYNAMIC(CDlgConsortia)

public:
	CDlgConsortia(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgConsortia();

// �Ի�������
	enum { IDD = IDD_DLG_CONSORTIA };
private:
	int m_iCurItem;
private:
	void ResetConsortiaInfo(int item);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	CButton m_btnModify;
	afx_msg void OnBnClickedQueryconsortia();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	CListCtrl m_lstConsortia;
	CEdit m_edID;
	CEdit m_edName;
	CButton m_btnModifyPlacard;
	CEdit m_edPlacard;
	CEdit m_edTenet;
	CStatic m_csInfo;
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton1();
};
