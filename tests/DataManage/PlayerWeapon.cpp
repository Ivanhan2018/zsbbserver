// PlayerWeapon.cpp : 实现文件
//

#include "stdafx.h"
#include "DataManage.h"
#include "PlayerWeapon.h"
#include "../include/DB_Proxy.h"
#include "../include/DB_Table.h"
#include ".\playerresource.h"
#include ".\playerweapon.h"
#include ".\datamanagedlg.h"

// CPlayerWeapon 对话框

IMPLEMENT_DYNAMIC(CPlayerWeapon, CDialog)
CPlayerWeapon::CPlayerWeapon(CWnd* pParent /*=NULL*/)
	: CDialog(CPlayerWeapon::IDD, pParent)
{
}

CPlayerWeapon::~CPlayerWeapon()
{
}

void CPlayerWeapon::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_HINT, m_stHint);
	DDX_Control(pDX, IDC_EDIT_COUNT, m_edCount);
	DDX_Control(pDX, IDC_COMBO1, m_cbWeaponList);
}


BEGIN_MESSAGE_MAP(CPlayerWeapon, CDialog)
	ON_BN_CLICKED(ID_ADDITEM, OnBnClickedAdditem)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnBnClickedButtonDelete)
END_MESSAGE_MAP()


// CPlayerWeapon 消息处理程序

void CPlayerWeapon::OnBnClickedAdditem()
{
	// TODO: 在此添加控件通知处理程序代码
	CDataManageDlg *pMainDlg = (CDataManageDlg*)AfxGetMainWnd();	
	LPWeaponParam pWeaponParam = 0;
	CString strPlayerID,strMapID,strCityID;
	CString strItemID,strType,strCount;
	int iPlayerID,iMapID,iCityID;
	int iItemID,iType,iCount;
	pMainDlg->m_edPlayerID.GetWindowText(strPlayerID);
	pMainDlg->m_cbMapList.GetWindowText(strMapID);
	pMainDlg->m_edCityID.GetWindowText(strCityID);
	m_edCount.GetWindowText(strCount);
	iPlayerID = atoi(strPlayerID);
	iMapID = atoi(strMapID);
	iCityID = atoi(strCityID);
	iItemID = m_cbWeaponList.GetItemData(m_cbWeaponList.GetCurSel());
	pWeaponParam = pMainDlg->GetWeaponParam(iItemID);
	if(pWeaponParam == NULL)
	{
		m_stHint.SetWindowText("类型错误");
		return;
	}
	iType = pWeaponParam->Type;
	iCount = atoi(strCount);

	PlayerResource restb;
	restb.setmasterid(iCityID,iMapID);
	restb.setdetailid(iPlayerID);
	m_stHint.SetWindowText("");
	if(pMainDlg->m_DBProxy.db_select(restb) != DB_Proxy::DB_SUCCEED)
	{
		//MessageBox("select PlayerResource table fault.");
		m_stHint.SetWindowText("仓库容量不够.");
		return;
	}
	if(restb.content_ < iCount)
	{
		//MessageBox("仓库容量不够.");
		m_stHint.SetWindowText("仓库容量不够.");
		return;
	}
	
	if(iType <1 || iType >4)
	{
		m_stHint.SetWindowText("类型有误，请重新输入");
		return;
	}
	if(iCount ==0)
	{
		m_stHint.SetWindowText("数量为0，请重新输入.");
		m_edCount.SetFocus();
		return;
	}
	memset(&restb,0,sizeof(restb));
	restb.setmasterid(iCityID,iMapID);
	restb.setdetailid(iPlayerID);
	restb.content_ = -iCount;
	
	ProductionAmount protb;
	protb.setmasterid(iMapID,iCityID,iPlayerID,iType);
	protb.setdetailid(iItemID);
	int iRet = pMainDlg->m_DBProxy.db_select(protb);
	if(iRet == DB_Proxy::DB_FAILED)
	{
		m_stHint.SetWindowText("查询DBG错误.");
		return;
	}
	memset(&protb,0,sizeof(protb));
	protb.setmasterid(iMapID,iCityID,iPlayerID,iType);
	protb.setdetailid(iItemID);
	protb.productioncount_ = iCount;
	if(iRet == DB_Proxy::DB_EOF)
	{//插入
		iRet = pMainDlg->m_DBProxy.db_insert(protb);
	}
	else
	{//修改
		iRet = pMainDlg->m_DBProxy.db_increase(protb);
	}
	if(iRet == DB_Proxy::DB_SUCCEED)
	{//修改容量
		if(pMainDlg->m_DBProxy.db_increase(restb) != DB_Proxy::DB_SUCCEED)
		{
			m_stHint.SetWindowText("更新仓库容量失败.");
		}
		else
		{
			m_stHint.SetWindowText("增加防具、武器成功.");
		}
	}
	else
	{
		m_stHint.SetWindowText("增加防具、武器失败.");
	}
}

void CPlayerWeapon::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// TODO: 在此处添加消息处理程序代码
	CDataManageDlg *pMainDlg = (CDataManageDlg*)AfxGetMainWnd();
	pMainDlg->InitWeaponParamComboBox(&m_cbWeaponList);
	m_cbWeaponList.SetCurSel(0);
	m_edCount.SetWindowText("1");
}

void CPlayerWeapon::OnBnClickedButtonDelete()
{
	// TODO: 在此添加控件通知处理程序代码
	CDataManageDlg *pMainDlg = (CDataManageDlg*)AfxGetMainWnd();
	LPWeaponParam pWeaponParam = 0;
	CString strPlayerID,strMapID,strCityID;
	CString strItemID,strType,strCount;
	int iPlayerID,iMapID,iCityID;
	int iItemID,iType,iCount;
	pMainDlg->m_edPlayerID.GetWindowText(strPlayerID);
	pMainDlg->m_cbMapList.GetWindowText(strMapID);
	pMainDlg->m_edCityID.GetWindowText(strCityID);
	iPlayerID = atoi(strPlayerID);
	iMapID = atoi(strMapID);
	iCityID = atoi(strCityID);
	iItemID = m_cbWeaponList.GetItemData(m_cbWeaponList.GetCurSel());
	pWeaponParam = pMainDlg->GetWeaponParam(iItemID);
	if(pWeaponParam == NULL)
	{
		m_stHint.SetWindowText("类型错误");
		return;
	}
	iType = pWeaponParam->Type;
	m_stHint.SetWindowText("");
	
	ProductionAmount protb;
	protb.setmasterid(iMapID,iCityID,iPlayerID,iType);
	protb.setdetailid(iItemID);
	int iRet = pMainDlg->m_DBProxy.db_select(protb);
	if(iRet == DB_Proxy::DB_FAILED)
	{
		m_stHint.SetWindowText("查询DBG错误.");
		return;
	}
	iCount = protb.productioncount_;
	iRet = pMainDlg->m_DBProxy.db_delete(protb);
	if(iRet != DB_Proxy::DB_SUCCEED)
	{
		m_stHint.SetWindowText("删除时错误.");
		return;
	}
	if(iCount >0)
	{//修改容量
		PlayerResource restb;
		restb.setmasterid(iCityID,iMapID);
		restb.setdetailid(iPlayerID);
		restb.content_ = iCount;
		pMainDlg->m_DBProxy.db_increase(restb);
	}
	CString strTmp;
	strTmp.Format("删除成功,武器：%d,类型:%d,数量:%d.",iItemID,iType,iCount);
	m_stHint.SetWindowText("删除成功.");
}
