// PlayerWeapon.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DataManage.h"
#include "PlayerWeapon.h"
#include "../include/DB_Proxy.h"
#include "../include/DB_Table.h"
#include ".\playerresource.h"
#include ".\playerweapon.h"
#include ".\datamanagedlg.h"

// CPlayerWeapon �Ի���

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


// CPlayerWeapon ��Ϣ�������

void CPlayerWeapon::OnBnClickedAdditem()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
		m_stHint.SetWindowText("���ʹ���");
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
		m_stHint.SetWindowText("�ֿ���������.");
		return;
	}
	if(restb.content_ < iCount)
	{
		//MessageBox("�ֿ���������.");
		m_stHint.SetWindowText("�ֿ���������.");
		return;
	}
	
	if(iType <1 || iType >4)
	{
		m_stHint.SetWindowText("������������������");
		return;
	}
	if(iCount ==0)
	{
		m_stHint.SetWindowText("����Ϊ0������������.");
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
		m_stHint.SetWindowText("��ѯDBG����.");
		return;
	}
	memset(&protb,0,sizeof(protb));
	protb.setmasterid(iMapID,iCityID,iPlayerID,iType);
	protb.setdetailid(iItemID);
	protb.productioncount_ = iCount;
	if(iRet == DB_Proxy::DB_EOF)
	{//����
		iRet = pMainDlg->m_DBProxy.db_insert(protb);
	}
	else
	{//�޸�
		iRet = pMainDlg->m_DBProxy.db_increase(protb);
	}
	if(iRet == DB_Proxy::DB_SUCCEED)
	{//�޸�����
		if(pMainDlg->m_DBProxy.db_increase(restb) != DB_Proxy::DB_SUCCEED)
		{
			m_stHint.SetWindowText("���²ֿ�����ʧ��.");
		}
		else
		{
			m_stHint.SetWindowText("���ӷ��ߡ������ɹ�.");
		}
	}
	else
	{
		m_stHint.SetWindowText("���ӷ��ߡ�����ʧ��.");
	}
}

void CPlayerWeapon::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// TODO: �ڴ˴������Ϣ����������
	CDataManageDlg *pMainDlg = (CDataManageDlg*)AfxGetMainWnd();
	pMainDlg->InitWeaponParamComboBox(&m_cbWeaponList);
	m_cbWeaponList.SetCurSel(0);
	m_edCount.SetWindowText("1");
}

void CPlayerWeapon::OnBnClickedButtonDelete()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
		m_stHint.SetWindowText("���ʹ���");
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
		m_stHint.SetWindowText("��ѯDBG����.");
		return;
	}
	iCount = protb.productioncount_;
	iRet = pMainDlg->m_DBProxy.db_delete(protb);
	if(iRet != DB_Proxy::DB_SUCCEED)
	{
		m_stHint.SetWindowText("ɾ��ʱ����.");
		return;
	}
	if(iCount >0)
	{//�޸�����
		PlayerResource restb;
		restb.setmasterid(iCityID,iMapID);
		restb.setdetailid(iPlayerID);
		restb.content_ = iCount;
		pMainDlg->m_DBProxy.db_increase(restb);
	}
	CString strTmp;
	strTmp.Format("ɾ���ɹ�,������%d,����:%d,����:%d.",iItemID,iType,iCount);
	m_stHint.SetWindowText("ɾ���ɹ�.");
}
