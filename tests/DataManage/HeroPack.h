#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CHeroPack 对话框

class CHeroPack : public CDialog
{
	DECLARE_DYNAMIC(CHeroPack)

public:
	CHeroPack(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CHeroPack();

// 对话框数据
	enum { IDD = IDD_DIALOG_HEROPACK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_iCurItem;
public:
	CStatic m_stHint;
	CEdit m_edPackID;
	//CEdit m_edItemID;
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBnClickedAdditem();
	afx_msg void OnBnClickedButtonAddcity();
	CEdit m_edMapID;
	CComboBox m_cbEquipList;
	CListCtrl m_lstItemList;
	afx_msg void OnBnClickedButtonQuery();
	CButton m_btnDelItem;
	afx_msg void OnNMClickListItem(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonDelete();
	CButton m_btnAddCity;
	afx_msg void OnLvnItemchangedListItem(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
public:
	CEdit m_edCount;
public:
	afx_msg void OnCbnSelchangeComboPos();
};
