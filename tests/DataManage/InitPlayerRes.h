#pragma once
#include "afxwin.h"
#include ".\datamanagedlg.h"
#include "../include/DB_Proxy.h"
#include <map>

// CInitPlayerRes 对话框
typedef struct tagInitWeaponParam
{
	int Type;
	int ItemID;
	int Count;
}InitWeaponParam,*LPInitWeaponParam;

class CInitPlayerRes : public CDialog
{
	DECLARE_DYNAMIC(CInitPlayerRes)

public:
	CInitPlayerRes(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CInitPlayerRes();

// 对话框数据
	enum { IDD = IDD_DIALOG1 };
	typedef std::map<int,LPInitWeaponParam> InitWeaponParam_Map;
	typedef std::map<int,LPInitWeaponParam>::iterator InitWeaponParam_Map_Iterator;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
private:
	void RecruitHero(CDataManageDlg *pMainDlg,int playerid,int country,int jobid,int raceid);
	void ReadInitWeaponList();
	void ClearInitWeaponList();
	LPInitWeaponParam GetInitWeaponParam(int type,int itemid);
private:
	InitWeaponParam_Map m_InitWeaponList;
	DECLARE_MESSAGE_MAP()
public:
	CEdit m_edMapID;
	CStatic m_scInfo;
	afx_msg void OnBnClickedOk();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	CEdit m_edStartID;
	CEdit m_edEndID;
	afx_msg void OnBnClickedButtonInitofficer();
	CEdit m_edCountryID2;
	CComboBox m_cbJobList;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnBnClickedButton3();
	CEdit m_edPlayerID;
	CComboBox m_cbDeptList;
	CComboBox m_cbPostion;
	CComboBox m_cbCountryList;
	CComboBox m_cbCardTypeList;
	CEdit m_edNumber;
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnDestroy();
	CComboBox m_cbInitWeapon;
	CEdit m_edCountryMoney;
	afx_msg void OnBnClickedButton8();
};
