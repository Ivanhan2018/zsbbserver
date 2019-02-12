// HeroPack.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DataManage.h"
#include "HeroPack.h"
#include "DB_Proxy.h"
#include "DB_Table.h"
#include "ZSBB_DataDefine.h"
#include ".\datamanagedlg.h"
#include ".\heropack.h"


// CHeroPack �Ի���

IMPLEMENT_DYNAMIC(CHeroPack, CDialog)
CHeroPack::CHeroPack(CWnd* pParent /*=NULL*/)
	: CDialog(CHeroPack::IDD, pParent)
{
}

CHeroPack::~CHeroPack()
{
}

void CHeroPack::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_HINT, m_stHint);
	DDX_Control(pDX, IDC_EDIT_PACKID, m_edPackID);
	DDX_Control(pDX, IDC_EDIT_MAPID, m_edMapID);
	DDX_Control(pDX, IDC_COMBO_POS, m_cbEquipList);
	DDX_Control(pDX, IDC_LIST_ITEM, m_lstItemList);
	DDX_Control(pDX, IDC_BUTTON_DELETE, m_btnDelItem);
	DDX_Control(pDX, IDC_BUTTON_ADDCITY, m_btnAddCity);
	DDX_Control(pDX, IDC_EDIT1, m_edCount);
}


BEGIN_MESSAGE_MAP(CHeroPack, CDialog)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(ID_ADDITEM, OnBnClickedAdditem)
	ON_BN_CLICKED(IDC_BUTTON_ADDCITY, OnBnClickedButtonAddcity)
	ON_BN_CLICKED(ID_BUTTON_QUERY, OnBnClickedButtonQuery)
	ON_NOTIFY(NM_CLICK, IDC_LIST_ITEM, OnNMClickListItem)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnBnClickedButtonDelete)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_ITEM, OnLvnItemchangedListItem)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnBnClickedButton3)
	ON_CBN_SELCHANGE(IDC_COMBO_POS, &CHeroPack::OnCbnSelchangeComboPos)
END_MESSAGE_MAP()


// CHeroPack ��Ϣ�������

void CHeroPack::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// TODO: �ڴ˴������Ϣ����������
	CDataManageDlg *pMainDlg = (CDataManageDlg*)AfxGetMainWnd();	
	m_edPackID.SetWindowText("1");
	int index = 0;
	m_lstItemList.InsertColumn(index++,"PackID",LVCFMT_LEFT,60);
	m_lstItemList.InsertColumn(index++,"SortID",LVCFMT_LEFT,60);
	m_lstItemList.InsertColumn(index++,"TypeID",LVCFMT_LEFT,45);
	m_lstItemList.InsertColumn(index++,"ItemID",LVCFMT_LEFT,120);
	m_lstItemList.InsertColumn(index++,"Pos",LVCFMT_LEFT,40);
	m_lstItemList.InsertColumn(index++,"ShowPos",LVCFMT_LEFT,60);
	m_lstItemList.InsertColumn(index++,"Count",LVCFMT_LEFT,60);
	
	m_lstItemList.SetExtendedStyle(LVS_EX_FULLROWSELECT |LVS_EX_GRIDLINES );
	
	m_btnDelItem.EnableWindow(FALSE);
	pMainDlg->InitHeroEquipParamComboBox(&m_cbEquipList);
	m_cbEquipList.SetCurSel(0);
	m_edCount.SetWindowText("1");
	//m_btnAddCity.EnableWindow(FALSE);
}

void CHeroPack::OnBnClickedAdditem()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDataManageDlg *pMainDlg = (CDataManageDlg*)AfxGetMainWnd();
	HeroEquipAttribute *pHeroEquipParam = NULL;
	CString strPlayerID;
	CString strPackID,strItemID,strCount;
	int iPlayerID,iCount;
	int iPackID,iItemID,iPos;
	pMainDlg->m_edPlayerID.GetWindowText(strPlayerID);
	m_edPackID.GetWindowText(strPackID);
	m_edCount.GetWindowText(strCount);
	iPlayerID = atoi(strPlayerID);
	iPackID = atoi(strPackID);
	iCount = atoi(strCount);

	iItemID = m_cbEquipList.GetItemData(m_cbEquipList.GetCurSel());

	m_stHint.SetWindowText("");
	pHeroEquipParam = pMainDlg->GetHeroEquipParam(iItemID);
	if (pHeroEquipParam == NULL)
	{
		m_stHint.SetWindowText("װ�������ڣ�����������.");
		return;
	}
	iPos = pHeroEquipParam->postion;

	if(iPackID <=0 || iItemID <=0 || iCount <=0)
	{
		m_stHint.SetWindowText("������������������.");
		return;
	}
	PackStoreParam *storeParam = pMainDlg->GetPackStoreParam(iPos);

	PlayerItemStorageTable itemtb;
	itemtb.setmasterid(iPackID,iPlayerID);
	if (storeParam && storeParam->IsOverlap)
	{
		itemtb.itemid_ = iItemID;
	}
	else
	{
		iCount = 1;
	}
	pMainDlg->GetUnusedPos(itemtb,&pMainDlg->m_DBProxy);
	if(itemtb.showpos_ <0)
	{
		m_stHint.SetWindowText("�����������޷����.");
		return;
	}
	int iRet = 0;
	if (itemtb.sortid_ >0)
	{
		int sortID = itemtb.sortid_;
		memset(&itemtb,0,sizeof(itemtb));
		itemtb.setmasterid(iPackID,iPlayerID);
		itemtb.setdetailid(sortID);
		itemtb.count_ = iCount;//����1
		iRet = pMainDlg->m_DBProxy.db_increase(itemtb);
	}
	else
	{
		if(iPackID ==1)
		{
			itemtb.type_ =EN_COMM_PACK;
		}
		else
		{
			itemtb.type_ = EN_HERO_PACK;
		}
		itemtb.itemid_ = iItemID;
		itemtb.pos_ = iPos;
		itemtb.count_ = iCount;
		itemtb.level_ = pHeroEquipParam->level;
		itemtb.tech_ = pHeroEquipParam->skill;
		itemtb.techlevel_ = pHeroEquipParam->skillLevel;
		iRet = pMainDlg->m_DBProxy.db_insert(itemtb);
	}
	if (iRet == DB_Proxy::DB_SUCCEED)
	{
		m_stHint.SetWindowText("���Ӣ�۱����ɹ�.");
	}
	else
	{
		m_stHint.SetWindowText("���Ӣ�۱���ʧ��.");
	}
	
}

void CHeroPack::OnBnClickedButtonAddcity()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDataManageDlg *pMainDlg = (CDataManageDlg*)AfxGetMainWnd();	
	
	CString strPlayerID,strMapID;
	pMainDlg->m_edPlayerID.GetWindowText(strPlayerID);
	m_edMapID.GetWindowText(strMapID);
	int iPlayerID,iMapID,iCityID;

	if(strMapID.IsEmpty())
	{
		m_stHint.SetWindowText("Please enter MapID.");
		m_edMapID.SetFocus();
		return;
	}
	iPlayerID = atoi(strPlayerID);
	iMapID = atoi(strMapID);
	iCityID = iMapID * 10000 + 101 + rand()%3;
	MapInfoTable mapinfo;
	mapinfo.setid(iMapID);
	if(pMainDlg->m_DBProxy.db_select(mapinfo) != DB_Proxy::DB_SUCCEED)
	{
		m_stHint.SetWindowText("��Ӫ��ͬ���޷�ֳ��.");
		return;
	}

	PlayerCity playercity;
	playercity.setmasterid(iPlayerID);
	playercity.setdetailid(iMapID);
	if(pMainDlg->m_DBProxy.db_select(playercity) != DB_Proxy::DB_EOF)
	{
		m_stHint.SetWindowText("�õ�ͼ���г���.");
		if(playercity.cityid_<1000)
		{
			playercity.cityid_ = playercity.mapid_ *10000 + playercity.cityid_;
			pMainDlg->m_DBProxy.db_update(playercity,playercity);
		}
		return;
	}

	int iConID = 5;
	/*
	ConstructionConfigure *pConscfg = pMainDlg->getConstructionConfigure(iConID,1);
	if(pConscfg == NULL)
	{
		m_stHint.SetWindowText("ֳ�����ʧ�ܣ��ֿ����������.");
		return;
	}
	*/

	playercity.cityid_ = iCityID;
	playercity.state_ = 0;
	if(pMainDlg->m_DBProxy.db_insert(playercity) != DB_Proxy::DB_SUCCEED)
	{
		m_stHint.SetWindowText("ֳ�����ʧ�ܣ�д��ҳ��б�ʧ��.");
		return;
	}
	//���вֿ�
	PlayerResource playerres;
	playerres.setmasterid(iCityID,iMapID);
	playerres.setdetailid(iPlayerID);

	playerres.foodcount_ = 0;
	playerres.woodcount_ = 0;
	playerres.ironcount_ = 0;
	playerres.skincount_ = 0;
	playerres.yollowstonecount_ = 0;
	playerres.redstonecount_ = 0;
	playerres.greenstonecount_ = 0;
	playerres.backironcount_ = 0;
	playerres.content_ = 50000;
	time_t curtm;
	time(&curtm);
	playerres.completetimes_ = curtm;
	//playerres.content_ = pConscfg->modify_;
	pMainDlg->m_DBProxy.db_insert(playerres);

	PlayerConstruction playerCon;
	playerCon.constructionid_ = iConID;
	playerCon.level_ = 1;
	playerCon.output_ = 5000;//pConscfg->modify_;
	playerCon.completetimes_ = curtm;

	playerCon.setmasterid(iMapID, iCityID, iPlayerID);
	playerCon.sortid_ = 1;
	pMainDlg->m_DBProxy.db_insert(playerCon);
	m_stHint.SetWindowText("ֳ����гɹ�.");
}

void CHeroPack::OnBnClickedButtonQuery()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
	CDataManageDlg *pMainDlg = (CDataManageDlg*)AfxGetMainWnd();	
	
	CString strPlayerID,strOfficerID;
	int iPlayerID,iOfficerID;
	pMainDlg->m_edPlayerID.GetWindowText(strPlayerID);
	m_edPackID.GetWindowText(strOfficerID);
	iPlayerID = atoi(strPlayerID);
	iOfficerID = atoi(strOfficerID);
	m_stHint.SetWindowText("");
	
	m_lstItemList.DeleteAllItems();
	PlayerItemStorageTable itemtb;
	PlayerItemStorageTable *pItemList = 0;
	char *buf = 0;
	int count = 0;

	itemtb.setmasterid(iOfficerID,iPlayerID);
	int iRet = pMainDlg->m_DBProxy.db_multi_select(itemtb,buf,count);
	if( iRet == DB_Proxy::DB_EOF)
	{
		m_stHint.SetWindowText("�ñ���Ϊ�ա�");
		return;
	}
	else if(iRet == DB_Proxy::DB_FAILED)
	{
		m_stHint.SetWindowText("��ѯ����ʧ�ܡ�");
		return;
	}
	count = count/sizeof(PlayerItemStorageTable);
	pItemList = (PlayerItemStorageTable*)buf;

	CString strTmp;
	int colIndex = 0;
	for(int i=0;i<count;i++)
	{
		colIndex = 0;
		strTmp.Format("%d",pItemList[i].officerid_);
		m_lstItemList.InsertItem(i,strTmp);

		strTmp.Format("%d",pItemList[i].sortid_);
		m_lstItemList.SetItemText(i,++colIndex,strTmp);

		strTmp.Format("%d",pItemList[i].type_);
		m_lstItemList.SetItemText(i,++colIndex,strTmp);

		strTmp.Format("%I64d",pItemList[i].itemid_);
		m_lstItemList.SetItemText(i,++colIndex,strTmp);

		strTmp.Format("%d",pItemList[i].pos_);
		m_lstItemList.SetItemText(i,++colIndex,strTmp);

		strTmp.Format("%d",pItemList[i].showpos_);
		m_lstItemList.SetItemText(i,++colIndex,strTmp);

		strTmp.Format("%d",pItemList[i].count_);
		m_lstItemList.SetItemText(i,++colIndex,strTmp);
	}

	if(buf)
	{
		delete[] buf;
		buf = 0;
	}
}

void CHeroPack::OnNMClickListItem(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	NMLISTVIEW*   pList=   (NMLISTVIEW*)pNMHDR;  
	m_iCurItem = pList->iItem;
	if(pList->iItem >=0)
	{
		m_btnDelItem.EnableWindow(TRUE);
	}
	else
	{
		m_btnDelItem.EnableWindow(FALSE);
	}
	*pResult = 0;
}

void CHeroPack::OnBnClickedButtonDelete()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(m_iCurItem <0)
		return;

	PlayerItemStorageTable itemtb;
	CDataManageDlg *pMainDlg = (CDataManageDlg*)AfxGetMainWnd();	
	
	CString strPlayerID,strOfficerID,strSortID;
	int iPlayerID,iOfficerID,iSortID;
	pMainDlg->m_edPlayerID.GetWindowText(strPlayerID);
	m_edPackID.GetWindowText(strOfficerID);
	strSortID = m_lstItemList.GetItemText(m_iCurItem,1);
	iPlayerID = atoi(strPlayerID);
	iOfficerID = atoi(strOfficerID);
	iSortID = atoi(strSortID);

	m_stHint.SetWindowText("");
	itemtb.setmasterid(iOfficerID,iPlayerID);
	itemtb.setdetailid(iSortID);
	if(pMainDlg->m_DBProxy.db_delete(itemtb) == DB_Proxy::DB_SUCCEED)
	{
		m_stHint.SetWindowText("ɾ���ɹ�");
		m_lstItemList.DeleteItem(m_iCurItem);
		//LVIS_SELECTED/LVIF_STATE
		if (m_iCurItem >= m_lstItemList.GetItemCount())
		{
			m_iCurItem = m_lstItemList.GetItemCount() - 1;
		}
		if (m_iCurItem >=0)
		{
			LVITEM lvitem;
			lvitem.mask = LVIF_STATE;
			lvitem.iItem = m_iCurItem;
			lvitem.state = LVIS_SELECTED ;
			lvitem.stateMask = LVIS_OVERLAYMASK ;//LVIS_STATEIMAGEMASK  ;
			//m_lstItemList.SetItemState(m_iCurItem,LVIS_SELECTED|LVIS_FOCUSED,LVIF_STATE);
			//m_lstItemList.SetHotItem(m_iCurItem);
			m_lstItemList.SetItemState(0,&lvitem);
		}
		else
		{
			m_btnDelItem.EnableWindow(FALSE);
		}
	}
	else
	{
		m_stHint.SetWindowText("ɾ��ʧ��");
	}
}

void CHeroPack::OnLvnItemchangedListItem(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}

void CHeroPack::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDataManageDlg *pMainDlg = (CDataManageDlg*)AfxGetMainWnd();
	char *buf = 0;
	int count = 0;

	
	PlayerItemStorageTable itemtb;
	PlayerItemStorageTable *pItemList = 0;
	if(pMainDlg->m_DBProxy.db_select_all(itemtb,buf,count) != DB_Proxy::DB_SUCCEED)
		return;
	count = count/sizeof(PlayerItemStorageTable);
	pItemList = (PlayerItemStorageTable*)buf;
	for(int i=0;i<count;i++)
	{
		if (pItemList[i].itemid_ != 1032 && pItemList[i].officerid_ != 1)
		{
			continue;
		}
		//if(pItemList[i].playerid_ >0 && pItemList[i].officerid_ >0)
			//continue;
		pMainDlg->m_DBProxy.db_delete(pItemList[i]);
	}
	if(buf)
	{
		delete[] buf;
		buf = 0;
	}
}

void CHeroPack::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDataManageDlg *pMainDlg = (CDataManageDlg*)AfxGetMainWnd();
	CString strPlayerID,strMapID;
	pMainDlg->m_edPlayerID.GetWindowText(strPlayerID);
	m_edMapID.GetWindowText(strMapID);
	int iPlayerID,iMapID;

	if(strMapID.IsEmpty())
	{
		m_stHint.SetWindowText("Please enter MapID.");
		m_edMapID.SetFocus();
		return;
	}
	iPlayerID = atoi(strPlayerID);
	iMapID = atoi(strMapID);
	
	PlayerCity playercity;
	playercity.setmasterid(iPlayerID);
	playercity.setdetailid(iMapID);
	if(pMainDlg->m_DBProxy.db_select(playercity) != DB_Proxy::DB_SUCCEED)
	{
		m_stHint.SetWindowText("�õ�ͼû�г���.");
		return;
	}

	if(pMainDlg->m_DBProxy.db_delete(playercity) != DB_Proxy::DB_SUCCEED)
	{
		m_stHint.SetWindowText("ɾ������ʧ�ܣ�д��ҳ��б�ʧ��.");
		return;
	}
	//���вֿ�
	PlayerResource playerres;
	playerres.setmasterid(playercity.cityid_,playercity.mapid_);
	playerres.setdetailid(iPlayerID);
	pMainDlg->m_DBProxy.db_delete(playerres) ;

	PlayerConstruction playerCon;

	playerCon.setmasterid(playercity.mapid_, playercity.cityid_, iPlayerID);
	playerCon.sortid_ = 1;
	pMainDlg->m_DBProxy.db_delete(playerCon);
	m_stHint.SetWindowText("ɾ�����гɹ�.");
}

void CHeroPack::OnBnClickedButton3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	PlayerItemStorageTable itemtb;
	CDataManageDlg *pMainDlg = (CDataManageDlg*)AfxGetMainWnd();	

	CString strPlayerID,strOfficerID,strSortID;
	int iPlayerID,iOfficerID,iSortID;
	pMainDlg->m_edPlayerID.GetWindowText(strPlayerID);
	m_edPackID.GetWindowText(strOfficerID);
	int count = m_lstItemList.GetItemCount();
	m_stHint.SetWindowText("");
	for (int i =0;i<count;i++)
	{
		strSortID = m_lstItemList.GetItemText(i,1);
		iPlayerID = atoi(strPlayerID);
		iOfficerID = atoi(strOfficerID);
		iSortID = atoi(strSortID);

		itemtb.setmasterid(iOfficerID,iPlayerID);
		itemtb.setdetailid(iSortID);
		pMainDlg->m_DBProxy.db_delete(itemtb);
	}
	m_lstItemList.DeleteAllItems();
}

void CHeroPack::OnCbnSelchangeComboPos()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}
