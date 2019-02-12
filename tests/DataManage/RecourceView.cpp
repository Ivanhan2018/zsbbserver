// RecourceView.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DataManage.h"
#include "RecourceView.h"
#include "../include/DB_Proxy.h"
#include "../include/DB_Table.h"
#include ".\datamanagedlg.h"
#include ".\recourceview.h"


// CRecourceView �Ի���

IMPLEMENT_DYNAMIC(CRecourceView, CDialog)
CRecourceView::CRecourceView(CWnd* pParent /*=NULL*/)
	: CDialog(CRecourceView::IDD, pParent)
{
}

CRecourceView::~CRecourceView()
{
}

void CRecourceView::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_FOOD, m_edFood);
	DDX_Control(pDX, IDC_EDIT_WOOD, m_edWood);
	DDX_Control(pDX, IDC_EDIT_SKIN, m_edSkin);
	DDX_Control(pDX, IDC_EDIT_IRON, m_edIron);
	DDX_Control(pDX, IDC_EDIT_YSTONE, m_edYStone);
	DDX_Control(pDX, IDC_EDIT_RSTONE, m_edRStone);
	DDX_Control(pDX, IDC_EDIT_GSTONE, m_edGStone);
	DDX_Control(pDX, IDC_EDIT_BLACKIRON, m_edBlackIron);
	DDX_Control(pDX, IDC_LIST_AMOUNT, m_lstAmount);
	DDX_Control(pDX, IDC_STATIC_INFO, m_csInfo);
	DDX_Control(pDX, IDC_EDIT_CONTENT, m_edContent);
	DDX_Control(pDX, IDC_EDIT_TOTAL, m_edTotal);
	DDX_Control(pDX, IDC_EDIT_SUM, m_edSum);
	DDX_Control(pDX, IDC_BUTTON_RESET, m_btnReset);
}


BEGIN_MESSAGE_MAP(CRecourceView, CDialog)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(ID_BUTTON_QUERY, OnBnClickedButtonQuery)
	ON_BN_CLICKED(ID_BUTTON_ADD, OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_RESET, OnBnClickedButtonReset)
	ON_NOTIFY(NM_CLICK, IDC_LIST_AMOUNT, OnNMClickListAmount)
END_MESSAGE_MAP()


// CRecourceView ��Ϣ�������

void CRecourceView::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// TODO: �ڴ˴������Ϣ����������
	int index = 0;
	m_lstAmount.InsertColumn(index++,"PlayerID",LVCFMT_LEFT,80);
	m_lstAmount.InsertColumn(index++,"MapID",LVCFMT_LEFT,80);
	m_lstAmount.InsertColumn(index++,"CityID",LVCFMT_LEFT,80);
	m_lstAmount.InsertColumn(index++,"ItemID",LVCFMT_LEFT,60);
	m_lstAmount.InsertColumn(index++,"Type",LVCFMT_LEFT,60);
	m_lstAmount.InsertColumn(index++,"Count",LVCFMT_LEFT,80);
	m_lstAmount.SetExtendedStyle(LVS_EX_FULLROWSELECT |LVS_EX_GRIDLINES );
	m_btnReset.EnableWindow(FALSE);
}

void CRecourceView::OnBnClickedButtonQuery()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDataManageDlg *pMainDlg = (CDataManageDlg*)AfxGetMainWnd();
	CString strPlayerID,strMapID,strCityID;
	int iPlayerID,iMapID,iCityID;
	int iTotal = 0;
	pMainDlg->m_edPlayerID.GetWindowText(strPlayerID);
	pMainDlg->m_cbMapList.GetWindowText(strMapID);
	pMainDlg->m_edCityID.GetWindowText(strCityID);
	iPlayerID = atoi(strPlayerID);
	iMapID = atoi(strMapID);
	iCityID = atoi(strCityID);

	m_csInfo.SetWindowText("");
	m_lstAmount.DeleteAllItems();
	PlayerResource playerRes;
	playerRes.setmasterid(iCityID,iMapID);
	playerRes.setdetailid(iPlayerID);
	if(pMainDlg->m_DBProxy.db_select(playerRes) != DB_Proxy::DB_SUCCEED)
	{
		m_csInfo.SetWindowText("��ѯPlayerResourceʧ��");
		return;
	}
	CString strTmp;
	strTmp.Format("%d",playerRes.content_);
	m_edContent.SetWindowText(strTmp);

	strTmp.Format("%d",playerRes.foodcount_);
	m_edFood.SetWindowText(strTmp);
	strTmp.Format("%d",playerRes.woodcount_);
	m_edWood.SetWindowText(strTmp);
	strTmp.Format("%d",playerRes.skincount_);
	m_edSkin.SetWindowText(strTmp);
	strTmp.Format("%d",playerRes.ironcount_);
	m_edIron.SetWindowText(strTmp);

	strTmp.Format("%d",playerRes.yollowstonecount_);
	m_edYStone.SetWindowText(strTmp);
	strTmp.Format("%d",playerRes.redstonecount_);
	m_edRStone.SetWindowText(strTmp);
	strTmp.Format("%d",playerRes.greenstonecount_);
	m_edGStone.SetWindowText(strTmp);
	strTmp.Format("%d",playerRes.backironcount_);
	m_edBlackIron.SetWindowText(strTmp);

	iTotal = playerRes.foodcount_ + playerRes.woodcount_ + playerRes.skincount_ + playerRes.ironcount_;
	iTotal += playerRes.yollowstonecount_ + playerRes.redstonecount_ + playerRes.greenstonecount_ + playerRes.backironcount_;
	ProductionAmount proAmount;
	ProductionAmount *pAmountList = 0;
	char *buf = 0;
	int count = 0;
	int index = 0;
	for(int i=1;i<=4;i++)
	{
		proAmount.setmasterid(iMapID,iCityID,iPlayerID,i);
		int iRet = pMainDlg->m_DBProxy.db_multi_select(proAmount,buf,count);
		if(iRet != DB_Proxy::DB_SUCCEED)
		{
			continue;
		}
		count = count/sizeof(ProductionAmount);
		pAmountList = (ProductionAmount*)buf;
		for(int j=0;j<count;j++)
		{
			strTmp.Format("%d",pAmountList[j].playerid_);
			m_lstAmount.InsertItem(index,strTmp);
			strTmp.Format("%d",pAmountList[j].mapid_);
			m_lstAmount.SetItemText(index,1,strTmp);
			strTmp.Format("%d",pAmountList[j].cityid_);
			m_lstAmount.SetItemText(index,2,strTmp);
			strTmp.Format("%d",pAmountList[j].productionid_);
			m_lstAmount.SetItemText(index,3,strTmp);
			strTmp.Format("%d",pAmountList[j].type_);
			m_lstAmount.SetItemText(index,4,strTmp);
			strTmp.Format("%d",pAmountList[j].productioncount_);
			m_lstAmount.SetItemText(index,5,strTmp);
			iTotal += pAmountList[j].productioncount_;
			index++;
		}
		if(buf)
		{
			delete[] buf;
			buf = 0;
		}
	}
	strTmp.Format("%d",iTotal);
	m_edTotal.SetWindowText(strTmp);
	iTotal += playerRes.content_;
	strTmp.Format("%d",iTotal);
	m_edSum.SetWindowText(strTmp);
	m_csInfo.SetWindowText("��ѯ�ɹ�");
}

void CRecourceView::OnBnClickedButtonAdd()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	if(MessageBox("ȷ�����Ӳֿ⽨����?","ϵͳ��ʾ",MB_ICONINFORMATION |MB_OKCANCEL) != IDOK)
		return ;

	m_csInfo.SetWindowText("");
	CDataManageDlg *pMainDlg = (CDataManageDlg*)AfxGetMainWnd();
	CString strPlayerID,strMapID,strCityID;
	int iPlayerID,iMapID,iCityID;
	int iTotal = 0;
	pMainDlg->m_edPlayerID.GetWindowText(strPlayerID);
	pMainDlg->m_cbMapList.GetWindowText(strMapID);
	pMainDlg->m_edCityID.GetWindowText(strCityID);
	iPlayerID = atoi(strPlayerID);
	iMapID = atoi(strMapID);
	iCityID = atoi(strCityID);

	PlayerConstruction playerCons;
	playerCons.setmasterid(iMapID,iCityID,iPlayerID);
	playerCons.setdetailid(1);
	if(pMainDlg->m_DBProxy.db_select(playerCons) == DB_Proxy::DB_SUCCEED)
	{
		m_csInfo.SetWindowText("������ڸó����вֿ⽨��������Ҫ����.");
		if(playerCons.constructionid_ != 5)
		{
			playerCons.constructionid_ = 5;
			playerCons.level_ = 1;
			playerCons.output_ = 50000;
			pMainDlg->m_DBProxy.db_update(playerCons,playerCons);
			
			m_csInfo.SetWindowText("�ý������ǲֿ⣬�޸�1���ֿ�.");
		}
		return;
	}
	playerCons.constructionid_ = 5;
	playerCons.level_ = 1;
	time_t cur_tm;
	time(&cur_tm);
	playerCons.completetimes_ = cur_tm;
	playerCons.output_ = 50000;
	if(pMainDlg->m_DBProxy.db_insert(playerCons) == DB_Proxy::DB_SUCCEED)
	{
		m_csInfo.SetWindowText("���Ӳֿ⽨���ɹ�.");
	}
	else
	{
		m_csInfo.SetWindowText("���Ӳֿ⽨��ʧ��.");
	}
}

void CRecourceView::OnBnClickedButtonReset()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	// mapid_ + cityid_ + playerid_ + type_  + productionid_ = ID һ��
	/*
	strTmp.Format("%d",pAmountList[j].playerid_);
			m_lstAmount.InsertItem(index,strTmp);
			strTmp.Format("%d",pAmountList[j].mapid_);
			m_lstAmount.SetItemText(index,1,strTmp);
			strTmp.Format("%d",pAmountList[j].cityid_);
			m_lstAmount.SetItemText(index,2,strTmp);
			strTmp.Format("%d",pAmountList[j].productionid_);
			m_lstAmount.SetItemText(index,3,strTmp);
			strTmp.Format("%d",pAmountList[j].type_);
			m_lstAmount.SetItemText(index,4,strTmp);
			strTmp.Format("%d",pAmountList[j].productioncount_);
			m_lstAmount.SetItemText(index,5,strTmp);
			iTotal += pAmountList[j].productioncount_;
	*/
	ProductionAmount proAmount;
	int iPlayerID,iMapID,iCityID,iType,iProID;
	CString strTmp;
	strTmp = m_lstAmount.GetItemText(m_iCurItem,0);
	iPlayerID = atoi(strTmp);
	strTmp = m_lstAmount.GetItemText(m_iCurItem,1);
	iMapID = atoi(strTmp);
	strTmp = m_lstAmount.GetItemText(m_iCurItem,2);
	iCityID = atoi(strTmp);
	strTmp = m_lstAmount.GetItemText(m_iCurItem,3);
	iProID = atoi(strTmp);
	strTmp = m_lstAmount.GetItemText(m_iCurItem,4);
	iType = atoi(strTmp);
	proAmount.setdetailid(iProID);
	proAmount.setmasterid(iMapID,iCityID,iPlayerID,iType);
	m_csInfo.SetWindowText("");
	CDataManageDlg *pMainDlg = (CDataManageDlg*)AfxGetMainWnd();
	if(pMainDlg->m_DBProxy.db_select(proAmount) != DB_Proxy::DB_SUCCEED)
	{
		m_csInfo.SetWindowText("��ѯʧ�ܡ�");
		return;
	}
	proAmount.productioncount_ = 0;
	if(pMainDlg->m_DBProxy.db_update(proAmount,proAmount) != DB_Proxy::DB_SUCCEED)
	{
		m_csInfo.SetWindowText("�޸�ʧ�ܡ�");
	}
	else
	{
		m_csInfo.SetWindowText("�޸ĳɹ���");
	}
}

void CRecourceView::OnNMClickListAmount(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
	NMLISTVIEW*   pList=   (NMLISTVIEW*)pNMHDR;  
	m_iCurItem = pList->iItem;
	if(pList->iItem >=0)
	{
		m_btnReset.EnableWindow(TRUE);
	}
	else
	{
		m_btnReset.EnableWindow(FALSE);
	}
	*pResult = 0;
}
