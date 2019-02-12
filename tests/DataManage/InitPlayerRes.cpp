// InitPlayerRes.cpp : 实现文件
//

#include "stdafx.h"
#include "DataManage.h"
#include "InitPlayerRes.h"
#include ".\initplayerres.h"
#include "DB_Table.h"
#include "ConsortiaProtocol.h"


// CInitPlayerRes 对话框

IMPLEMENT_DYNAMIC(CInitPlayerRes, CDialog)
CInitPlayerRes::CInitPlayerRes(CWnd* pParent /*=NULL*/)
	: CDialog(CInitPlayerRes::IDD, pParent)
{
}

CInitPlayerRes::~CInitPlayerRes()
{
}

void CInitPlayerRes::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_MAPID, m_edMapID);
	DDX_Control(pDX, IDC_STATIC_HINT, m_scInfo);
	DDX_Control(pDX, IDC_EDIT_STARTID, m_edStartID);
	DDX_Control(pDX, IDC_EDIT_ENDID, m_edEndID);
	DDX_Control(pDX, IDC_EDIT3, m_edCountryID2);
	DDX_Control(pDX, IDC_COMBO1, m_cbJobList);
	DDX_Control(pDX, IDC_EDIT1, m_edPlayerID);
	DDX_Control(pDX, IDC_COMBO3, m_cbDeptList);
	DDX_Control(pDX, IDC_COMBO4, m_cbPostion);
	DDX_Control(pDX, IDC_COMBO5, m_cbCountryList);
	DDX_Control(pDX, IDC_COMBO2, m_cbCardTypeList);
	DDX_Control(pDX, IDC_EDIT2, m_edNumber);
	DDX_Control(pDX, IDC_COMBO6, m_cbInitWeapon);
	DDX_Control(pDX, IDC_EDIT4, m_edCountryMoney);
}


BEGIN_MESSAGE_MAP(CInitPlayerRes, CDialog)
	ON_BN_CLICKED(ID_OK, OnBnClickedOk)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BUTTON_INITOFFICER, OnBnClickedButtonInitofficer)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
	ON_CBN_SELCHANGE(IDC_COMBO1, OnCbnSelchangeCombo1)
	ON_BN_CLICKED(IDC_BUTTON3, OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, OnBnClickedButton6)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON8, OnBnClickedButton8)
END_MESSAGE_MAP()


// CInitPlayerRes 消息处理程序

void CInitPlayerRes::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDataManageDlg *pMainDlg = (CDataManageDlg*)AfxGetMainWnd();
	CString strMapID,strCountryID;
	int iMapID,iCityID,iCountryID;
	m_edMapID.GetWindowText(strMapID);

	m_scInfo.SetWindowText("");
	if(strMapID.IsEmpty())
	{
		m_scInfo.SetWindowText("郡地图编号或阵营不能为空。");
		return;
	}
	iMapID = atoi(strMapID);
	iCityID = iMapID * 10000 + 101;
	iCountryID = m_cbCountryList.GetItemData(m_cbCountryList.GetCurSel());
	//把NPC郡该外该阵营的郡	
	char *buf = 0;
	int count = 0;
	int i =0;
	MapInfoTable maptb;
	/*
	MapInfoTable *pMapList = 0;
	if(pMainDlg->m_DBProxy.db_select_all(maptb,buf,count) != DB_Proxy::DB_SUCCEED)
		return;
	count = count/sizeof(MapInfoTable);
	pMapList = (MapInfoTable *)buf;
	for(i =0;i<count;i++)
	{
		if(pMapList[i].countryid_ !=0)
			continue;
		pMapList[i].countryid_ = iCountryID;
		pMainDlg->m_DBProxy.db_update(pMapList[i],pMapList[i]);
	}
	if(buf)
	{
		delete[] buf;
		buf = 0;
	}
	*/
	maptb.setid(iMapID);
	if(pMainDlg->m_DBProxy.db_select(maptb) != DB_Proxy::DB_SUCCEED)
	{
		m_scInfo.SetWindowText("郡地图编号错误。");
		return;
	}
	if(maptb.countryid_ != iCountryID)
	{
		m_scInfo.SetWindowText("郡地图所属阵营与填写阵营不一致。");
		return;
	}
	PlayerChatRoomTable playertb;
	PlayerChatRoomTable *pPlayerList = 0;
	count = 0;
	if(pMainDlg->m_DBProxy.db_select_all(playertb,buf,count) != DB_Proxy::DB_SUCCEED)
	{
		m_scInfo.SetWindowText("查询表PlayerChatRoomTable失败");
		return;
	}
	count = count/sizeof(PlayerChatRoomTable);
	pPlayerList = (PlayerChatRoomTable*)buf;

	PlayerCity playercity;
	PlayerResource playerres;
	PlayerConstruction playerCon;
	PlayerSoldierRace racetb;
	ProductionAmount protb;
	time_t curtm;
	time(&curtm);
	int iCount = 0;
	int iPlayerCount = 0;
	
	InitWeaponParam_Map_Iterator iter;
	LPInitWeaponParam pInitWeaponParam = 0;
	for(i =0;i<count;i++)
	{
		if(pPlayerList[i].country_ != iCountryID)
			continue;
		playercity.setmasterid(pPlayerList[i].playerid_);
		playercity.setdetailid(iMapID);
		if(pMainDlg->m_DBProxy.db_select(playercity) == DB_Proxy::DB_EOF)
		{
			//不存在，殖民城市
			iCityID = iMapID* 10000+ 101 + rand()%3;
			playercity.cityid_ = iCityID;
			if(iMapID == 1003 || iMapID == 1007)
			{
				playercity.state_ = 100;
			}
			else
			{
				playercity.state_ = 0;
			}
			pMainDlg->m_DBProxy.db_insert(playercity);
			//添加资源
			playerres.setmasterid(iCityID,iMapID);
			playerres.setdetailid(pPlayerList[i].playerid_);
			playerres.content_ = 1922168;
			playerres.completetimes_ = curtm;
			pMainDlg->m_DBProxy.db_insert(playerres);
			//添加仓库
			playerCon.setmasterid(iMapID,iCityID,pPlayerList[i].playerid_);
			playerCon.setdetailid(1);
			playerCon.constructionid_ = 5;
			playerCon.level_ = 10;
			playerCon.output_ = 1922168;
			playerCon.completetimes_ = curtm;
			pMainDlg->m_DBProxy.db_insert(playerCon);
		}
		else
		{
			iCityID = playercity.cityid_;
			playerres.setmasterid(iCityID,iMapID);
			playerres.setdetailid(pPlayerList[i].playerid_);
			if(pMainDlg->m_DBProxy.db_select(playerres) != DB_Proxy::DB_SUCCEED)
				continue;
			if(playerres.content_ <800000)
				continue;
		}
		iPlayerCount++;
		//添加兵种	
		racetb.setmasterid(iMapID,iCityID);
		racetb.setdetailid(pPlayerList[i].playerid_);
		if(pMainDlg->m_DBProxy.db_select(racetb) == DB_Proxy::DB_SUCCEED)
		{
			racetb.human_ = 50000;
			racetb.mannikin_ = 50000;
			racetb.fairy_ = 50000;
			racetb.dandiprat_ = 50000;
			racetb.halfanimal_ = 50000;
			racetb.cattle_ = 50000;
			racetb.blackfairy_ = 50000 ;
			racetb.decpesrson_ = 50000;
			racetb.completetimes_ = curtm;
			pMainDlg->m_DBProxy.db_update(racetb,racetb);
		}
		else
		{
			racetb.human_ = 50000;
			racetb.mannikin_ = 50000;
			racetb.fairy_ = 50000;
			racetb.dandiprat_ = 50000;
			racetb.halfanimal_ = 50000;
			racetb.cattle_ = 50000;
			racetb.blackfairy_ = 50000 ;
			racetb.decpesrson_ = 50000;
			racetb.completetimes_ = curtm;
			pMainDlg->m_DBProxy.db_insert(racetb);
		}

		//添加武器1、防具2
		iCount = 0;
		for(iter=m_InitWeaponList.begin();iter != m_InitWeaponList.end();iter++)
		{
			pInitWeaponParam = iter->second;
			if(pInitWeaponParam == NULL)
				continue;

			protb.setmasterid(iMapID,iCityID,pPlayerList[i].playerid_,pInitWeaponParam->Type);
			protb.setdetailid(pInitWeaponParam->ItemID);
			if(pMainDlg->m_DBProxy.db_select(protb) == DB_Proxy::DB_SUCCEED)
			{
				if(protb.productioncount_ <pInitWeaponParam->Count)
				{
					protb.productioncount_ = pInitWeaponParam->Count - protb.productioncount_;
					pMainDlg->m_DBProxy.db_increase(protb);
					iCount +=protb.productioncount_;
				}
			}
			else
			{
				protb.productioncount_ = pInitWeaponParam->Count;
				pMainDlg->m_DBProxy.db_insert(protb);
				iCount += pInitWeaponParam->Count;
			}
		}
		
		memset(&playerres,0,sizeof(PlayerResource));
		playerres.setmasterid(iCityID,iMapID);
		playerres.setdetailid(pPlayerList[i].playerid_);
		playerres.content_ = - iCount;
		pMainDlg->m_DBProxy.db_increase(playerres);

	}
	if(buf)
	{
		delete[] buf;
		buf = 0;
	}

	CString strTmp;
	strTmp.Format("刷装备成功，共有%d玩家",iPlayerCount);
	m_scInfo.SetWindowText(strTmp);
}

void CInitPlayerRes::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// TODO: 在此处添加消息处理程序代码
	CDataManageDlg *pMainDlg = (CDataManageDlg*)AfxGetMainWnd();
	m_edMapID.SetWindowText("1018");
	
	int index = 0;
	index = m_cbCountryList.AddString("联盟");
	m_cbCountryList.SetItemData(index,1);
	index = m_cbCountryList.AddString("部落");
	m_cbCountryList.SetItemData(index,2);
	m_cbCountryList.SetCurSel(0);

	index = m_cbJobList.AddString("战士");
	m_cbJobList.SetItemData(index,2);
	index = m_cbJobList.AddString("弓箭手");
	m_cbJobList.SetItemData(index,3);
	index = m_cbJobList.AddString("法师");
	m_cbJobList.SetItemData(index,4);
	index = m_cbJobList.AddString("牧师");
	m_cbJobList.SetItemData(index,5);
	m_cbJobList.SetCurSel(0);

	index = m_cbDeptList.AddString("国防部");
	m_cbDeptList.SetItemData(index,DOD_DEPT);
	index = m_cbDeptList.AddString("内政部");
	m_cbDeptList.SetItemData(index,INTERIOR_DEPT);
	m_cbDeptList.SetCurSel(0);

	index = m_cbPostion.AddString("部长");
	m_cbPostion.SetItemData(index,MINISTER_POSTION);
	index = m_cbPostion.AddString("副部长");
	m_cbPostion.SetItemData(index,UNDERSECRETARY_POSTION);
	m_cbPostion.SetCurSel(0);

	//index = m_cbCardTypeList.AddString("复原卡");
	//m_cbCardTypeList.SetItemData(index,1033);

	//index = m_cbCardTypeList.AddString("逃跑卡");
	//m_cbCardTypeList.SetItemData(index,1032);
	pMainDlg->InitCardConfigList(&m_cbCardTypeList);
	m_cbCardTypeList.SetCurSel(0);

	m_edNumber.SetWindowText("15");
	ReadInitWeaponList();
}

void CInitPlayerRes::OnBnClickedButtonInitofficer()
{
	// TODO: 在此添加控件通知处理程序代码
	CDataManageDlg *pMainDlg = (CDataManageDlg*)AfxGetMainWnd();
	CString strStartID,strEndID,strCountryID;
	int iStartID,iEndID,iCountryID,iMapID,iCityID,iRaceID,iFaceID,iJobID;
	m_edStartID.GetWindowText(strStartID);
	m_edEndID.GetWindowText(strEndID);
	m_edCountryID2.GetWindowText(strCountryID);
	m_scInfo.SetWindowText("");
	if(strStartID.IsEmpty() || strEndID.IsEmpty() || strCountryID.IsEmpty())
	{
		m_scInfo.SetWindowText("编号输入有误");
		return;
	}
	iJobID = m_cbJobList.GetItemData(m_cbJobList.GetCurSel());
	iStartID = atoi(strStartID);
	iEndID = atoi(strEndID);
	iCountryID = atoi(strCountryID);
	if(iStartID > iEndID)
	{
		m_scInfo.SetWindowText("编号输入有误");
		return;
	}
	//srand(GetTickCount());
	if(iCountryID ==1)
	{
		iMapID = 1003;
		iRaceID = 1;
		iFaceID = 1109;
	}
	else if(iCountryID ==2)
	{
		iMapID = 1007;
		iRaceID = 5;
		iFaceID = 5201;
	}
	else
	{
		m_scInfo.SetWindowText("阵营输入有误");
		return;
	}
	iCityID = iMapID * 10000 +101+rand()%4;
	PlayerOfficerTable officertb,mainOfficer;
	PlayerChatRoomTable playertb;
	char name[3][24] = {"aa","联盟","部落"};
	int iIndex = 0;
	for(int i = iStartID;i<=iEndID;i++)
	{
		officertb.setmasterid(i);
		officertb.setdetailid(i);
		if(pMainDlg->m_DBProxy.db_select(officertb) == DB_Proxy::DB_SUCCEED)
			continue;
		mainOfficer.setmasterid(i);
		mainOfficer.setdetailid(i);
		mainOfficer.mapid_ = iMapID;
		mainOfficer.cityid_ = iCityID;
		mainOfficer.faceid_ = iFaceID;
		mainOfficer.officerrace_ = iRaceID;
		mainOfficer.jobid_ = iJobID;
		mainOfficer.level_ = 1;
		mainOfficer.stamina_ = 30;
		mainOfficer.power_ = 30;
		mainOfficer.celerity_ = 30;
		mainOfficer.wit_ = 30;
		mainOfficer.attack_ = 150;
		mainOfficer.physicrecovery_ = 150;
		mainOfficer.magicrecovery_ = 150;
		sprintf(mainOfficer.officername_,"%s%d",name[iCountryID],i);
		if(pMainDlg->m_DBProxy.db_insert(mainOfficer) != DB_Proxy::DB_SUCCEED)
			continue;
		//添加
		playertb.setid(i);

		playertb.country_ = iCountryID;
		playertb.money_ = 2000000;
		playertb.science_ = 5000;
		playertb.maxpacknum_ = EN_PACK_GRID_NUM;
		pMainDlg->m_DBProxy.db_insert(playertb) ;
		//刷英雄
		RecruitHero(pMainDlg,i,iCountryID,iJobID,iRaceID);
		iIndex++;
	}
	CString strTmp;
	strTmp.Format("刷英雄成功,共%d",iIndex);
	m_scInfo.SetWindowText(strTmp);
}
void CInitPlayerRes::RecruitHero(CDataManageDlg *pMainDlg,int playerid,int country,int jobid,int raceid)
{
	int iHeroID = 0;
	PlayerOfficerTable officertb;
	int iracebase = 1;
	int iRaceID = 0;
	char szName[24] = {0};
	char *pFrtName = 0;
	char *pSecName = 0;
	CString strTmp;
	//性别
	int iSex = 20;
	if(country == 1)
	{
		iracebase = 1;
	}
	else
	{
		iracebase = 5;
	}
	for (int i= 0;i<8;i++)
	{
		iHeroID += 11;
		memset(&officertb,0,sizeof(PlayerOfficerTable));
		officertb.setmasterid(playerid);
		officertb.setdetailid(iHeroID);
		officertb.mapid_ = 0;
		officertb.cityid_ = 0;
		officertb.level_ = 1;
		officertb.exp_ = 0;
		officertb.jobid_ = 2+rand()%4;
		officertb.officerrace_ = iracebase+rand()%4;
		iRaceID = officertb.officerrace_ -1;
		//strTmp.Format("%d -- %d:职业：%d",playerid,iHeroID,officertb.jobid_);
		while(1)
		{
			if(pMainDlg->GetRaceJobParam(iRaceID)->IsKnight && officertb.jobid_ == 1)
				break;
			else if(pMainDlg->GetRaceJobParam(iRaceID)->IsSoldier && officertb.jobid_ ==2)
				break;
			else if(pMainDlg->GetRaceJobParam(iRaceID)->IsShooter && officertb.jobid_ == 3)
				break;
			else if(pMainDlg->GetRaceJobParam(iRaceID)->IsMaster && officertb.jobid_ == 4)
				break;
			else if(pMainDlg->GetRaceJobParam(iRaceID)->IsPastor && officertb.jobid_ == 5)
				break;
			else
			{
				officertb.officerrace_ = iracebase+rand()%4;
				iRaceID = officertb.officerrace_ -1;
			}
		}
		//strTmp.Format("%d -- %d:Race：%d",playerid,iHeroID,officertb.officerrace_);
		switch(officertb.officerrace_)
		{
		case 1:
			officertb.faceid_ = 1101+rand()%5;
			break;
		case 2:
			officertb.faceid_ = 2201+rand()%5;
			break;
		case 3:
			officertb.faceid_ = 3101+rand()%5;
			break;
		case 4:
			officertb.faceid_ = 4101+rand()%5;
			break;
		case 5:
			officertb.faceid_ = 5101+rand()%5;
			break;
		case 6:
			officertb.faceid_ = 6101+rand()%5;
			break;
		case 7:
			officertb.faceid_ = 7201+rand()%5;
			break;
		case 8:
			officertb.faceid_ = 8101+rand()%5;
			break;
		};

		if (officertb.faceid_ < 1101) 
		{
			officertb.faceid_ = 1101;
		}
		if (officertb.officerrace_ == 2 || officertb.officerrace_ == 7)
			iSex = 259;

		officertb.stamina_ =50;
		officertb.power_ = 50;
		officertb.celerity_ = 50;
		officertb.wit_ = 50;
		officertb.techdot_ = 1;
		
		pMainDlg->ProcHeroAttribute(officertb);
		pFrtName = pMainDlg->GetHeroName(rand()%20);
		pSecName = pMainDlg->GetHeroName(iSex+rand()%230);
		if (pFrtName && pSecName)
		{
			sprintf(officertb.officername_, "%s · %s",pFrtName, pSecName);
		}
		else
		{
			sprintf(officertb.officername_,"%d · %d",playerid,iHeroID );
		}
		officertb.armyid_ = 0;
		officertb.state_ = 0;

		if(pMainDlg->m_DBProxy.db_insert(officertb) != DB_Proxy::DB_SUCCEED)
		{
			//m_csInfo.SetWindowText("招募英雄失败。");
		}
		else
		{
			//m_csInfo.SetWindowText("招募英雄成功。");
		}
		strTmp.Format("%d -- %d:%s",playerid,iHeroID,officertb.officername_);
		m_scInfo.SetWindowText(strTmp);
	}
}
void CInitPlayerRes::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CDataManageDlg *pMainDlg = (CDataManageDlg*)AfxGetMainWnd();
	CString strCardNum;
	int iCardNum = 0;
	int iItemID = m_cbCardTypeList.GetItemData(m_cbCardTypeList.GetCurSel());
	m_edNumber.GetWindowText(strCardNum);
	if (strCardNum.IsEmpty())
	{
		m_scInfo.SetWindowText("数量为空，请输入");
		m_edNumber.SetFocus();
		return;
	}
	iCardNum = atoi(strCardNum);
	LPCardParam pCardConfig = pMainDlg->GetCardConfig(iItemID);
	if (pCardConfig == NULL)
	{
		m_scInfo.SetWindowText("卡不存在。。。。。");
		return;
	}
	PlayerChatRoomTable playertb;
	PlayerChatRoomTable *pPlayerList = 0;
	char *buf = 0;
	int count = 0;
	if(pMainDlg->m_DBProxy.db_select_all(playertb,buf,count) != DB_Proxy::DB_SUCCEED)
	{
		m_scInfo.SetWindowText("玩家表PlayerChatRoomTable为空");
		return;
	}
	count = count/sizeof(PlayerChatRoomTable);
	pPlayerList = (PlayerChatRoomTable*)buf;
	int j = 0;
	PlayerItemStorageTable itemtb;
	itemtb.type_ = EN_COMM_PACK;
	itemtb.count_ = 1;
	itemtb.level_ = 1;
	itemtb.officerid_ = 1;
	int iPos = 0;
	for(int i=0;i<count;i++)
	{
		itemtb.playerid_ = pPlayerList[i].playerid_;
		for(j = 0;j<iCardNum;j++)
		{
			iPos = pMainDlg->GetUnusedPos(pPlayerList[i].playerid_,1,&pMainDlg->m_DBProxy);
			if(iPos <0)
				break;
			itemtb.itemid_ = iItemID;
			itemtb.showpos_ = iPos;
			itemtb.pos_ = pCardConfig->Postion;
			pMainDlg->m_DBProxy.db_insert(itemtb);
		}
	}
	if(buf)
	{
		delete[] buf;
		buf = 0;
	}
	CString strTmp;
	strTmp.Format("刷卡成功，共有%d玩家",count);
	m_scInfo.SetWindowText(strTmp);
}

void CInitPlayerRes::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	CDataManageDlg *pMainDlg = (CDataManageDlg*)AfxGetMainWnd();
	PlayerChatRoomTable playertb;
	PlayerChatRoomTable *pPlayerList = 0;
	char *buf = 0;
	int count = 0;
	if(pMainDlg->m_DBProxy.db_select_all(playertb,buf,count) != DB_Proxy::DB_SUCCEED)
	{
		m_scInfo.SetWindowText("玩家表PlayerChatRoomTable为空");
		return;
	}
	count = count/sizeof(PlayerChatRoomTable);
	pPlayerList = (PlayerChatRoomTable*)buf;

	for(int i=0;i<count;i++)
	{
		if(pPlayerList[i].money_ >=1000000)
			continue;
		pPlayerList[i].money_ = 1000000;
		pMainDlg->m_DBProxy.db_update(pPlayerList[i],pPlayerList[i]);
	}
	if(buf)
	{
		delete[] buf;
		buf = 0;
	}
}

void CInitPlayerRes::OnCbnSelchangeCombo1()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CInitPlayerRes::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	CDataManageDlg *pMainDlg = (CDataManageDlg*)AfxGetMainWnd();
	CString strCountryID,strPlayerID;
	int iPlayerID,iCountryID,iDeptID,iPostion;

	m_edPlayerID.GetWindowText(strPlayerID);
	m_scInfo.SetWindowText("");
	if(strPlayerID.IsEmpty())
	{
		m_scInfo.SetWindowText("玩家ID不能为空");
		return;
	}
	iPlayerID = atoi(strPlayerID);

	iDeptID = m_cbDeptList.GetItemData(m_cbDeptList.GetCurSel());
	iPostion = m_cbPostion.GetItemData(m_cbPostion.GetCurSel());
	PlayerOfficerTable playeroff;
	playeroff.setmasterid(iPlayerID);
	playeroff.setdetailid(iPlayerID);
	if(pMainDlg->m_DBProxy.db_select(playeroff) != DB_Proxy::DB_SUCCEED)
	{
		m_scInfo.SetWindowText("玩家不存在");
		return;
	}
	PlayerChatRoomTable playertb;
	playertb.setid(iPlayerID);
	if(pMainDlg->m_DBProxy.db_select(playertb) != DB_Proxy::DB_SUCCEED)
	{
		m_scInfo.SetWindowText("查询玩家阵营失败。");
		return;
	}
	iCountryID = playertb.country_;
	Council council;
	if(iPostion == MINISTER_POSTION)
	{
		Council *pCouncilList = 0;
		char *buf = 0;
		int count = 0;
		council.setmasterid(iCountryID,iDeptID);
		if(pMainDlg->m_DBProxy.db_multi_select(council,buf,count) == DB_Proxy::DB_SUCCEED)
		{
			count = count/sizeof(Council);
			pCouncilList = (Council*)buf;
			for(int i=0;i<count;i++)
			{
				if(pCouncilList[i].postion_ == iPostion)
				{
					count = -1;
					break;
				}
			}
		}
		if(buf)
		{
			delete[] buf;
			buf = 0;
		}
		if(count == -1)
		{
			m_scInfo.SetWindowText("已有国防部长，无法添加");
			return;
		}
		memset(&council,0,sizeof(Council));
	}
	council.setmasterid(iCountryID,iDeptID);
	council.setdetailid(iPlayerID);
	strcpy(council.name_,playeroff.officername_);
	council.postion_ = iPostion;
	if(pMainDlg->m_DBProxy.db_insert(council) == DB_Proxy::DB_SUCCEED)
	{
		m_scInfo.SetWindowText("添加国防部长成功");
	}
	else
	{
		m_scInfo.SetWindowText("添加国防部长失败");
	}
}

void CInitPlayerRes::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码

	CDataManageDlg *pMainDlg = (CDataManageDlg*)AfxGetMainWnd();
	int iCountryID = m_cbCountryList.GetItemData(m_cbCountryList.GetCurSel());
	CString strMoney;
	m_edCountryMoney.GetWindowText(strMoney);
	int iMoney =atoi(strMoney);
	if(iMoney >100000000)
	{
		m_scInfo.SetWindowText("国防部刷钱金额不能超过100000000。");
		return;
	}
	Country countrytb,updatecountry;
	countrytb.setid(iCountryID);
	if(pMainDlg->m_DBProxy.db_select(countrytb) != DB_Proxy::DB_SUCCEED)
		return;
	if (countrytb.dodmoney_ >1000000000)
	{
		countrytb.dodmoney_ = -1;
	}
	if (countrytb.financemoney_ >1000000000)
	{
		countrytb.financemoney_ = -1;
	}
	if (countrytb.financemoney_ < 0 &&countrytb.dodmoney_ <0)
	{
		m_scInfo.SetWindowText("国防部、内政部金额已过亿，不需添加。");
		return;
	}
	updatecountry.setid(iCountryID);

	updatecountry.dodmoney_ = iMoney;
	updatecountry.financemoney_ = iMoney;
	if(pMainDlg->m_DBProxy.db_increase(updatecountry) == DB_Proxy::DB_SUCCEED)
		m_scInfo.SetWindowText("国防部刷钱成功..");
	else
		m_scInfo.SetWindowText("国防部刷钱失败..");
}

void CInitPlayerRes::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码

	CDataManageDlg *pMainDlg = (CDataManageDlg*)AfxGetMainWnd();
	ArmyTable armytb;
	ArmyTable *pArmyList = 0;
	char* buf = 0;
	int count = 0;
	armytb.setmasterid(1007);
	if (pMainDlg->m_DBProxy.db_multi_select(armytb,buf,count) != DB_Proxy::DB_SUCCEED)
	{
		return ;
	}
	count = count/sizeof(ArmyTable);
	pArmyList = (ArmyTable*)buf;
	for (int i =0;i<count;i++)
	{
		if (pArmyList[i].playerid_ != 200)
		{
			continue;
		}
		pArmyList[i].state_ = 1;
		pMainDlg->m_DBProxy.db_update(pArmyList[i],pArmyList[i]);
	}
	if (buf)
	{
		delete[] buf;
		buf = 0;
	}
}

void CInitPlayerRes::OnBnClickedButton6()
{
	// TODO: 在此添加控件通知处理程序代码
	CDataManageDlg *pMainDlg = (CDataManageDlg*)AfxGetMainWnd();
	PlayerCity playercity;
	PlayerCity *pCityList = 0;
	char *buf = 0;
	int count = 0;
	m_scInfo.SetWindowText("......");
	if (pMainDlg->m_DBProxy.db_select_all(playercity,buf,count) != DB_Proxy::DB_SUCCEED)
	{
		return;
	}
	count /= sizeof(PlayerCity);
	pCityList = (PlayerCity*)buf;
	int index = 0;
	for (int i=0;i<count;i++)
	{
		/*
		if (pCityList[i].state_ == 100)
		{
			continue;
		}
		*/
		if (pCityList[i].cityid_%100 != 0)
		{
			continue;
		}
		index++;
		pCityList[i].cityid_ = pCityList[i].mapid_ * 10000 +101+rand()%3;
		pMainDlg->m_DBProxy.db_update(pCityList[i],pCityList[i]);
	}
	if (buf)
	{
		delete[] buf;
		buf = 0;
	}
	CString strtmp;
	strtmp.Format("修改成功,共修改%d",index);
	m_scInfo.SetWindowText(strtmp);
}

void CInitPlayerRes::ReadInitWeaponList()
{
	FILE *fp = 0;
	fp = fopen("InitWeaponParam.csv","r");
	if(fp == NULL)
		return;

	char buf[128]= {0};
	fgets(buf,128,fp);

	LPInitWeaponParam pNewItem = 0;
	char Name[32] = {0};
	CString strTmp;
	while(!feof(fp))
	{
		fgets(buf,128,fp);
		pNewItem = new InitWeaponParam;
		if(sscanf(buf,"%d,%d,%[^,],%d",&pNewItem->Type,&pNewItem->ItemID,&Name,&pNewItem->Count) <4)
		{
			delete pNewItem;
			pNewItem = 0;
			continue;
		}
		m_InitWeaponList[pNewItem->ItemID * 10 + pNewItem->Type] = pNewItem;
		strTmp.Format("%s(%d)",Name,pNewItem->Count);
		m_cbInitWeapon.AddString(strTmp);
	}
	m_cbInitWeapon.SetCurSel(0);
	fclose(fp);
	return;
}
void CInitPlayerRes::ClearInitWeaponList()
{
	InitWeaponParam_Map_Iterator iter;
	LPInitWeaponParam pDelItem = 0;
	for(iter = m_InitWeaponList.begin();iter != m_InitWeaponList.end();iter++)
	{
		if(pDelItem = iter->second)
		{
			delete pDelItem;
			pDelItem = 0;
		}
	}
	m_InitWeaponList.clear();
}
LPInitWeaponParam CInitPlayerRes::GetInitWeaponParam(int type,int itemid)
{
	InitWeaponParam_Map_Iterator iter = m_InitWeaponList.find(itemid * 10 + type);
	if(iter == m_InitWeaponList.end())
		return NULL;
	return iter->second;
}

void CInitPlayerRes::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	ClearInitWeaponList();
}

void CInitPlayerRes::OnBnClickedButton8()
{
	// TODO: 在此添加控件通知处理程序代码
	CDataManageDlg *pMainDlg = (CDataManageDlg*)AfxGetMainWnd();
	//EN_BY_CAPTIVE 3
	PlayerOfficerTable playertb;
	char *buf =0;
	int count = 0;
	PlayerCaptiveHero capHero;
	char *capBuf = 0;
	int cap_count = 0;
	PlayerCaptiveHero *pCapList = 0;
	if (pMainDlg->m_DBProxy.db_select_all(playertb,buf,count) != DB_Proxy::DB_SUCCEED)
	{
		m_scInfo.SetWindowText("英雄列表为空");
		return;
	}
	count = count/sizeof(PlayerOfficerTable);
	PlayerOfficerTable *pOfficerList = (PlayerOfficerTable*)buf;
	if (pMainDlg->m_DBProxy.db_select_all(capHero,capBuf,cap_count) != DB_Proxy::DB_SUCCEED)
	{
	}
	pCapList = (PlayerCaptiveHero*)capBuf;
	cap_count = cap_count/sizeof(PlayerCaptiveHero);
	int j = 0;
	int count1=0,count2=0;
	for(int i=0;i<count;i++)
	{
		if (pOfficerList[i].state_ != EN_BY_CAPTIVE)
		{
			continue;
		}
		for (j=0;j<cap_count;j++)
		{
			if (pCapList[j].captivetimes_ >0 
				&& pCapList[j].byplayerid_ == pOfficerList[i].playerid_ 
				&& pCapList[j].byofficerid_ == pOfficerList[i].officerid_)
			{
				pCapList[j].captivetimes_ = 0;
				break;
			}
		}
		if (j<cap_count)
		{
			continue;
		}
		pOfficerList[i].state_ = 0;
		pMainDlg->m_DBProxy.db_update(pOfficerList[i],pOfficerList[i]);
		count1++;
	}
	for (j=0;j<cap_count;j++)
	{
		if (pCapList[j].captivetimes_ >0)
		{
			pMainDlg->m_DBProxy.db_delete(pCapList[j]);
			count2++;
		}
	}
	if (buf)
	{
		delete[] buf;
		buf = 0;
	}
	if (capBuf)
	{
		delete[] capBuf;
		capBuf = 0;
	}
	CString strTmp;
	strTmp.Format("修正英雄数：%d,无效被俘记录:%d",count1,count2);
	m_scInfo.SetWindowText(strTmp);
}
