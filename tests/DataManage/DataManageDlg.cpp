// DataManageDlg.cpp : 实现文件
//

#include "stdafx.h"
#include <stdio.h>
#include "DataManage.h"
#include "DataManageDlg.h"
#include ".\datamanagedlg.h"

#include "PlayerSoldier.h"
#include "PlayerResource.h"
#include "HeroPack.h"
#include "PlayerWeapon.h"
#include "PlayerOfficer.h"
#include "RecourceView.h"
#include "InitPlayerRes.h"
#include "DlgConsortia.h"
#include "ProFightSystem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CDataManageDlg 对话框



CDataManageDlg::CDataManageDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDataManageDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDataManageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPADDRESS1, m_ipDBGIP);
	DDX_Control(pDX, IDC_EDIT_DBGPORT, m_edDBGPort);
	DDX_Control(pDX, IDC_EDIT2, m_edPlayerID);
	DDX_Control(pDX, IDC_EDIT_CITYID, m_edCityID);
	DDX_Control(pDX, IDC_BTN_HEROPACK, m_btnHeroPack);
	DDX_Control(pDX, IDC_BTN_RESOURCE, m_btnResource);
	DDX_Control(pDX, IDC_BTN_SOLDIER, m_btnSoldier);
	DDX_Control(pDX, IDC_BTN_WEAPON, m_btnWeapon);
	DDX_Control(pDX, ID_BTN_CONNECT, m_btnConnect);
	DDX_Control(pDX, IDC_COMBO_MAP, m_cbMapList);
	DDX_Control(pDX, IDC_BTN_REFRESH, m_btnRefresh);
	DDX_Control(pDX, IDC_BTN_ADDCONTENT, m_btnAddContent);
	DDX_Control(pDX, IDC_BTN_HEROMANAGE, m_btnHeroManage);
	DDX_Control(pDX, IDC_BUTTON_HERO, m_btnAddHero);
	DDX_Control(pDX, IDC_BUTTON_DATAVIEW, m_btnDataView);
	DDX_Control(pDX, IDC_BUTTON_INIT, m_btnInit);
	DDX_Control(pDX, IDC_STATIC_INFO, m_csInfo);
	DDX_Control(pDX, IDC_BTN_RESUME, m_btnResume);
	DDX_Control(pDX, IDC_EDIT_CURLEVEL, m_edCurLevel);
	DDX_Control(pDX, IDC_EDIT_EXP, m_edExp);
	DDX_Control(pDX, IDC_BUTTON_DELCONS, m_btnDelCons);
	DDX_Control(pDX, IDC_BUTTON1, m_btnInitRes);
	DDX_Control(pDX, IDC_COMBO_HEROJOB, m_cbHeroJob);
	DDX_Control(pDX, IDC_BUTTON2, m_btnResafeAllHero);
	DDX_Control(pDX, IDC_BUTTON3, m_btnDelEmail);
	DDX_Control(pDX, IDC_BUTTON4, m_btnSpecialHero);
	DDX_Control(pDX, IDC_COMBO2, m_cbSpecialHero);
	DDX_Control(pDX, IDC_BUTTON5, m_btnConsortia);
	DDX_Control(pDX, IDC_EDIT1, m_edUser);
	DDX_Control(pDX, IDC_EDIT4, m_edPassword);
	DDX_Control(pDX, IDC_BUTTON7, m_btnModifyHeroAttribute);
	DDX_Control(pDX, IDC_BUTTON8, m_btnTaskBF);
}

BEGIN_MESSAGE_MAP(CDataManageDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(ID_BTN_CONNECT, OnBnClickedBtnConnect)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTN_HEROPACK, OnBnClickedBtnHeropack)
	ON_BN_CLICKED(IDC_BTN_RESOURCE, OnBnClickedBtnResource)
	ON_BN_CLICKED(IDC_BTN_SOLDIER, OnBnClickedBtnSoldier)
	ON_BN_CLICKED(IDC_BTN_WEAPON, OnBnClickedBtnWeapon)
	ON_BN_CLICKED(IDC_BTN_REFRESH, OnBnClickedBtnRefresh)
	ON_CBN_SELCHANGE(IDC_COMBO_MAP, OnCbnSelchangeComboMap)
	ON_BN_CLICKED(IDC_BTN_ADDCONTENT, OnBnClickedBtnAddcontent)
	ON_BN_CLICKED(IDC_BTN_HEROMANAGE, OnBnClickedBtnHeromanage)
	ON_BN_CLICKED(IDC_BUTTON_HERO, OnBnClickedButtonHero)
	//	ON_BN_CLICKED(IDC_BUTTON_ADDCITY, OnBnClickedButtonAddcity)
	ON_BN_CLICKED(IDC_BUTTON_DATAVIEW, OnBnClickedButtonDataview)
	ON_BN_CLICKED(IDC_BUTTON_INIT, OnBnClickedButtonInit)
	ON_BN_CLICKED(IDC_BTN_RESUME, OnBnClickedBtnResume)
	ON_BN_CLICKED(IDC_BUTTON_GETLEVEL, OnBnClickedButtonGetlevel)
	ON_BN_CLICKED(IDC_BUTTON_DELCONS, OnBnClickedButtonDelcons)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnBnClickedButton3)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON4, OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON8, OnBnClickedButton8)
END_MESSAGE_MAP()


// CDataManageDlg 消息处理程序

BOOL CDataManageDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将\“关于...\”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	srand(time(NULL));
	m_ipDBGIP.SetWindowText("192.168.1.244");
	m_edDBGPort.SetWindowText("8899");
	m_btnHeroPack.EnableWindow(FALSE);
	m_btnResource.EnableWindow(FALSE);
	m_btnSoldier.EnableWindow(FALSE);
	m_btnWeapon.EnableWindow(FALSE);
	m_btnRefresh.EnableWindow(FALSE);
	m_edCityID.EnableWindow(FALSE);	
	m_btnHeroManage.EnableWindow(FALSE);
	m_btnAddContent.EnableWindow(FALSE);
	m_btnAddHero.EnableWindow(FALSE);
	m_btnDataView.EnableWindow(FALSE);
	m_btnInit.EnableWindow(FALSE);
	m_btnResume.EnableWindow(FALSE);
	m_edCurLevel.SetWindowText("1");
	m_edExp.SetWindowText("0");
	m_btnDelCons.EnableWindow(FALSE);
	m_btnInitRes.EnableWindow(FALSE);
	m_btnResafeAllHero.EnableWindow(FALSE);
	m_btnDelEmail.EnableWindow(FALSE);
	m_btnSpecialHero.EnableWindow(FALSE);
	m_btnConsortia.EnableWindow(FALSE);
	m_bConnected = false;
	m_btnConnect.EnableWindow(FALSE);
	m_btnModifyHeroAttribute.EnableWindow(FALSE);
	m_btnTaskBF.EnableWindow(FALSE);
	m_edUser.SetWindowText("Administrator");
	m_edPassword.SetWindowText("xfy_datamanager");
	
	int index = 0;
	index = m_cbHeroJob.AddString("随机");
	m_cbHeroJob.SetItemData(index,-1);
	index = m_cbHeroJob.AddString("战士");
	m_cbHeroJob.SetItemData(index,2);
	index = m_cbHeroJob.AddString("弓箭手");
	m_cbHeroJob.SetItemData(index,3);
	index = m_cbHeroJob.AddString("法师");
	m_cbHeroJob.SetItemData(index,4);
	index = m_cbHeroJob.AddString("牧师");
	m_cbHeroJob.SetItemData(index,5);
	m_cbHeroJob.SetCurSel(0);
	//读取参数
	ReadRaceJobParam();
	ReadHeroName();
	ReadLevelParam();
	ReadSpecialHeroParam();
	ReadWeaponParam();
	ReadHeroEquipParam();
	ReadTaskBFParam();
	ClearEquipAttribute();
	ReadEquipAttribute();
	ClearCardConfig();
	ReadCardConfig();
	readOfficerInitSkill();
	readSkillDetail();
	ReadPackStoreParam();
	loadConstructionConfigure();
	return TRUE;  // 除非设置了控件的焦点，否则返回 TRUE
}

void CDataManageDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CDataManageDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
HCURSOR CDataManageDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CDataManageDlg::OnBnClickedBtnConnect()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_bConnected)
	{
		if(!m_DBProxy.fini())
			return;
		m_btnHeroPack.EnableWindow(FALSE);
		m_btnResource.EnableWindow(FALSE);
		m_btnSoldier.EnableWindow(FALSE);
		m_btnWeapon.EnableWindow(FALSE);
		m_btnRefresh.EnableWindow(FALSE);		
		m_btnAddContent.EnableWindow(FALSE);
		m_btnHeroManage.EnableWindow(FALSE);
		m_btnAddHero.EnableWindow(FALSE);
		m_btnDataView.EnableWindow(FALSE);
		m_btnInit.EnableWindow(FALSE);
		m_btnResume.EnableWindow(FALSE);
		m_btnDelCons.EnableWindow(FALSE);
		m_btnInitRes.EnableWindow(FALSE);
		m_btnResafeAllHero.EnableWindow(FALSE);
		m_btnDelEmail.EnableWindow(FALSE);
		m_btnSpecialHero.EnableWindow(FALSE);
		m_btnConsortia.EnableWindow(FALSE);
		m_btnModifyHeroAttribute.EnableWindow(FALSE);
		m_btnTaskBF.EnableWindow(FALSE);
		m_btnConnect.SetWindowText("连接");
		m_bConnected = false;
		return;
	}
	CString strDBGIP,strDBGPort;
	m_ipDBGIP.GetWindowText(strDBGIP);
	m_edDBGPort.GetWindowText(strDBGPort);
	if(!m_DBProxy.init(strDBGIP,atoi(strDBGPort),100))
	{
		m_csInfo.SetWindowText("连接DBG失败。。。。。。");
		return;
	}
	m_btnHeroPack.EnableWindow(TRUE);
	m_btnResource.EnableWindow(TRUE);
	m_btnSoldier.EnableWindow(TRUE);
	m_btnWeapon.EnableWindow(TRUE);
	m_btnRefresh.EnableWindow(TRUE);
	m_btnHeroManage.EnableWindow(TRUE);
	//m_btnAddContent.EnableWindow(TRUE);
	//m_btnAddHero.EnableWindow(TRUE);
	m_btnDataView.EnableWindow(TRUE);
	//m_btnInit.EnableWindow(TRUE);
	//m_btnResume.EnableWindow(TRUE);
	//m_btnDelCons.EnableWindow(TRUE);
	m_btnConnect.SetWindowText("断开");
	m_btnInitRes.EnableWindow(TRUE);
	//m_btnResafeAllHero.EnableWindow(TRUE);
	//m_btnDelEmail.EnableWindow(TRUE);
	//m_btnSpecialHero.EnableWindow(TRUE);
	//m_btnConsortia.EnableWindow(TRUE);
	//m_btnModifyHeroAttribute.EnableWindow(TRUE);
	//m_btnTaskBF.EnableWindow(TRUE);
	m_bConnected = true;
}

void CDataManageDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_DBProxy.fini();
	CDialog::OnClose();
}

//英雄背包
void CDataManageDlg::OnBnClickedBtnHeropack()
{
	// TODO: 在此添加控件通知处理程序代码
	m_csInfo.SetWindowText("");
	CString strPlayerID;
	m_edPlayerID.GetWindowText(strPlayerID);
	if(strPlayerID.IsEmpty())
	{
		m_csInfo.SetWindowText("Please enter player id.");
		m_edPlayerID.SetFocus();
		return;
	}
	CHeroPack dlgHeroPack;
	dlgHeroPack.DoModal();
}

void CDataManageDlg::OnBnClickedBtnResource()
{
	// TODO: 在此添加控件通知处理程序代码
	m_csInfo.SetWindowText("");
	CString strPlayerID,strMapID,strCityID;
	m_edPlayerID.GetWindowText(strPlayerID);
	m_cbMapList.GetWindowText(strMapID);
	m_edCityID.GetWindowText(strCityID);
	if(strPlayerID.IsEmpty())
	{
		m_csInfo.SetWindowText("Please enter player id.");
		m_edPlayerID.SetFocus();
		return;
	}
	if(strMapID.IsEmpty())
	{
		m_csInfo.SetWindowText("Please enter MapID.");
		m_cbMapList.SetFocus();
		return;
	}
	if(strCityID.IsEmpty())
	{
		m_csInfo.SetWindowText("Please enter CityID.");
		m_edCityID.SetFocus();
		return;
	}
	CPlayerResource dlgResource;
	dlgResource.DoModal();
}

void CDataManageDlg::OnBnClickedBtnSoldier()
{
	// TODO: 在此添加控件通知处理程序代码
	m_csInfo.SetWindowText("");
	CString strPlayerID,strMapID,strCityID;
	m_edPlayerID.GetWindowText(strPlayerID);
	m_cbMapList.GetWindowText(strMapID);
	m_edCityID.GetWindowText(strCityID);
	if(strPlayerID.IsEmpty())
	{
		m_csInfo.SetWindowText("Please enter player id.");
		m_edPlayerID.SetFocus();
		return;
	}
	if(strMapID.IsEmpty())
	{
		m_csInfo.SetWindowText("Please enter MapID.");
		m_cbMapList.SetFocus();
		return;
	}
	if(strCityID.IsEmpty())
	{
		m_csInfo.SetWindowText("Please enter CityID.");
		m_edCityID.SetFocus();
		return;
	}
	CPlayerSoldier dlgSoldier;
	dlgSoldier.DoModal();
}

void CDataManageDlg::OnBnClickedBtnWeapon()
{
	// TODO: 在此添加控件通知处理程序代码
	m_csInfo.SetWindowText("");
	CString strPlayerID,strMapID,strCityID;
	m_edPlayerID.GetWindowText(strPlayerID);
	m_cbMapList.GetWindowText(strMapID);
	m_edCityID.GetWindowText(strCityID);
	if(strPlayerID.IsEmpty())
	{
		m_csInfo.SetWindowText("Please enter player id.");
		m_edPlayerID.SetFocus();
		return;
	}
	if(strMapID.IsEmpty())
	{
		m_csInfo.SetWindowText("Please enter MapID.");
		m_cbMapList.SetFocus();
		return;
	}
	if(strCityID.IsEmpty())
	{
		m_csInfo.SetWindowText("Please enter CityID.");
		m_edCityID.SetFocus();
		return;
	}
	CPlayerWeapon dlgWeapon;
	dlgWeapon.DoModal();
}
int CDataManageDlg::GetUnusedPos(PlayerItemStorageTable &packitem,DB_Proxy *pDBProxy)
{
	packitem.showpos_ = -1;
	packitem.sortid_ = 0;
	PlayerItemStorageTable itemPos;
	PlayerItemStorageTable *pRetSet = 0;
	char *retbuf = 0;
	int count = 0;
	int iRet = 0;
	int iPackNum = EN_PACK_GRID_NUM;
	if(packitem.officerid_ == 1)
	{
		PlayerChatRoomTable playertb;
		//取公共背包数量
		playertb.setid(packitem.playerid_);
		if(pDBProxy->db_select(playertb) != DB_Proxy::DB_SUCCEED)
		{
			packitem.showpos_  = -2;
			return -2;
		}
		iPackNum = playertb.maxpacknum_;
	}
	itemPos.setmasterid(packitem.officerid_,packitem.playerid_);
	iRet = pDBProxy->db_multi_select(itemPos,retbuf,count);
	if(iRet == DB_Proxy::DB_FAILED)
	{
		packitem.showpos_ = -2;
	}
	else if(iRet == DB_Proxy::DB_EOF)
	{//背包为空
		packitem.showpos_ = 0;
	}
	else
	{
		count = count /sizeof(PlayerItemStorageTable);
		pRetSet = (PlayerItemStorageTable*)retbuf;
		int tmppos[100] = {0};
		int i = 0;
		for(i = 0;i<count && i<iPackNum;i++)
		{
			if(pRetSet[i].type_ == 1)//英雄身上的除外
				continue;

			tmppos[pRetSet[i].showpos_] = 1;
			if (pRetSet[i].itemid_ == packitem.itemid_)
			{
				memcpy(&packitem,&pRetSet[i],sizeof(PlayerItemStorageTable));
				break;
			}
		}
		for(i=0;i<iPackNum && packitem.showpos_ <0;i++)
		{
			if(tmppos[i] ==0)
			{
				packitem.showpos_ = i;
				break;
			}
		}
	}
	if(retbuf)
	{
		delete[] retbuf;
		retbuf = 0;
	}
	return packitem.showpos_;
}
int CDataManageDlg::GetUnusedPos(int playerid,int officerid,DB_Proxy *pDBProxy)
{
	int pos = -1;
	PlayerItemStorageTable itemPos;
	PlayerItemStorageTable *pRetSet = 0;
	char *retbuf = 0;
	int count = 0;
	int iRet = 0;
	int iPackNum = 20;
	if(officerid == 1)
	{
		PlayerChatRoomTable playertb;
		//取公共背包数量
		playertb.setid(playerid);
		if(pDBProxy->db_select(playertb) != DB_Proxy::DB_SUCCEED)
		{
			return -2;
		}
		iPackNum = playertb.maxpacknum_;
	}
	itemPos.setmasterid(officerid,playerid);
	iRet = pDBProxy->db_multi_select(itemPos,retbuf,count);
	if(iRet == DB_Proxy::DB_FAILED)
	{
		pos = -2;
	}
	else if(iRet == DB_Proxy::DB_EOF)
	{//背包为空
		pos = 0;
	}
	else
	{
		count = count /sizeof(PlayerItemStorageTable);
		pRetSet = (PlayerItemStorageTable*)retbuf;
		int tmppos[100] = {0};
		int i = 0;
		for(i = 0;i<count && i<iPackNum;i++)
		{
			tmppos[pRetSet[i].showpos_] = 1;
		}
		for(i=0;i<iPackNum;i++)
		{
			if(tmppos[i] ==0)
			{
				pos = i;
				break;
			}
		}
	}
	if(retbuf)
	{
		delete[] retbuf;
		retbuf = 0;
	}
	return pos;
}

int CDataManageDlg::ReadRaceJobParam()
{
	FILE *fp = 0;
	char buf[1024] = {0};
	fp = fopen("RaceJob.csv","r");
	if(fp == NULL)
		return 1;

	int index = 0;
	fgets(buf,1024,fp);
	while(!feof(fp))
	{
		fgets(buf,1024,fp);
		sscanf(buf,"%d,%d,%d,%d,%d,%d",&m_RaceJobParam[index].FaceID,&m_RaceJobParam[index].IsKnight,
			&m_RaceJobParam[index].IsSoldier,&m_RaceJobParam[index].IsShooter,
			&m_RaceJobParam[index].IsMaster,&m_RaceJobParam[index].IsPastor);

		index++;
	}
	fclose(fp);
	return 0;
}

int CDataManageDlg::ReadHeroName()
{
	FILE *fp = 0;
	fp = fopen("HeroName.csv","r");
	if(fp == NULL)
		return 1;

	memset(m_HeroName,0,1024*16);
	int index = 0;
	char buf[32]= {0};
	fgets(buf,32,fp);
	int tmpLen = 0;
	while(!feof(fp))
	{
		fgets(m_HeroName[index],16,fp);
		tmpLen = strlen(m_HeroName[index])-1;

		if(m_HeroName[index][tmpLen] == 0x0A)
		{
			m_HeroName[index][tmpLen] = 0;
		}
		index++;
	}
	fclose(fp);
	return 0;
}
int CDataManageDlg::ReadLevelParam()
{
	FILE *fp = 0;
	fp = fopen("LevelParam.csv","r");
	if(fp == NULL)
		return 1;

	memset(m_LevelParam,0,sizeof(int) * MAX_LEVEL+1);
	int iLevel = 0;
	int iExp = 0;
	char buf[32]= {0};
	fgets(buf,32,fp);

	while(!feof(fp))
	{
		fgets(buf,32,fp);
		sscanf(buf,"%d,%d",&iLevel,&iExp);
		if(iLevel >MAX_LEVEL || iLevel<0)
			continue;
		m_LevelParam[iLevel] = iExp;
	}
	fclose(fp);
	return 0;
}
int CDataManageDlg::GetExp(int level)
{
	if(level <0 || level >MAX_LEVEL)
		return -1;
	return m_LevelParam[level];
}

LPRaceJobParam CDataManageDlg::GetRaceJobParam(int raceid)
{
	if (raceid <0 || raceid >=8)
	{
		return NULL;
	}
	return &m_RaceJobParam[raceid];
}
char *CDataManageDlg::GetHeroName(int id)
{
	if (id <0 || id >= 1024)
	{
		return NULL;
	}
	return m_HeroName[id];
}
int CDataManageDlg::ReadSpecialHeroParam()
{
	FILE *fp = 0;
	fp = fopen("SpecialHeroTable.csv","r");
	if(fp == NULL)
		return 1;

	char buf[512]= {0};
	fgets(buf,512,fp);
	char szJobName[6][16] = {"","","战士","射手","法师","牧师"};
	char szCountry[2][16] = {"联盟","部落"};
	int index = 0;
	LPSpecialHeroParam pNewItem = 0;
	CString strTmp;
	while(!feof(fp))
	{
		fgets(buf,512,fp);
		pNewItem = new SpecialHeroParam;
		if(sscanf(buf,"%d,%[^,],%d,%d,%d,%d,%d,%d,%d,%d,%d",&pNewItem->SortID,&pNewItem->Name,&pNewItem->JobID,&pNewItem->RaceID,
			&pNewItem->FaceID,&pNewItem->Stamina,&pNewItem->Power,&pNewItem->Celerity,&pNewItem->Wit,&pNewItem->TechID,&pNewItem->TechLevel) != 11)
		{
			delete pNewItem;
			pNewItem = 0;
			continue;
		}

		m_SpecialHeroParamList[pNewItem->SortID] = pNewItem;
		if (pNewItem->RaceID <=4)
		{
			strTmp.Format("%s[%s%s]",pNewItem->Name,szCountry[0],szJobName[pNewItem->JobID]);
		}
		else
		{
			strTmp.Format("%s[%s%s]",pNewItem->Name,szCountry[1],szJobName[pNewItem->JobID]);
		}
		index = m_cbSpecialHero.AddString(strTmp);
		m_cbSpecialHero.SetItemData(index,(DWORD)pNewItem);
	}
	m_cbSpecialHero.SetCurSel(0);
	fclose(fp);
	return 0;
}
LPSpecialHeroParam CDataManageDlg::GetSpecialHeroParam(int id)
{
	SpecialHeroParam_Iterator iter;
	iter = m_SpecialHeroParamList.find(id);
	if(iter == m_SpecialHeroParamList.end())
		return NULL;
	return iter->second;
}
int CDataManageDlg::ClearSpecialHeroParam()
{
	SpecialHeroParam_Iterator iter;
	LPSpecialHeroParam pDelParam = 0;
	for (iter = m_SpecialHeroParamList.begin();iter != m_SpecialHeroParamList.end();iter++)
	{
		if (pDelParam = iter->second)
		{
			delete pDelParam;
			pDelParam = 0;
		}
	}
	m_SpecialHeroParamList.clear();
	return 0;
}
void CDataManageDlg::OnBnClickedBtnRefresh()
{
	// TODO: 在此添加控件通知处理程序代码
	m_csInfo.SetWindowText("");
	PlayerCity citytb;
	PlayerCity *pRetSet = 0;
	char *buf = 0;
	int count;
	CString strPlayerID;
	m_cbMapList.ResetContent();
	m_edPlayerID.GetWindowText(strPlayerID);
	if(strPlayerID.IsEmpty())
	{
		m_csInfo.SetWindowText("Please enter PlayerID.");
		m_edPlayerID.SetFocus();
		return;
	}
	citytb.setmasterid(atoi(strPlayerID));
	if(m_DBProxy.db_multi_select(citytb,buf,count) != DB_Proxy::DB_SUCCEED)
	{
		//
		m_csInfo.SetWindowText("查询玩家城市列表失败。");
		return;
	}
	count = count/sizeof(PlayerCity);
	pRetSet = (PlayerCity*)buf;
	int index = 0;
	CString strTmp;
	for(int i=0;i<count;i++)
	{
		strTmp.Format("%d",pRetSet[i].mapid_);
		index = m_cbMapList.AddString(strTmp);
		m_cbMapList.SetItemData(index,pRetSet[i].cityid_);
	}
	if(buf)
	{
		delete[] buf;
		buf = 0;
	}
	m_cbMapList.SetCurSel(0);
	strTmp.Format("%d",m_cbMapList.GetItemData(m_cbMapList.GetCurSel()));
	m_edCityID.SetWindowText(strTmp);
}

void CDataManageDlg::OnCbnSelchangeComboMap()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strTmp;
	strTmp.Format("%d",m_cbMapList.GetItemData(m_cbMapList.GetCurSel()));
	m_edCityID.SetWindowText(strTmp);
}

void CDataManageDlg::OnBnClickedBtnAddcontent()
{
	// TODO: 在此添加控件通知处理程序代码
	m_csInfo.SetWindowText("");
	CString strPlayerID,strMapID,strCityID;
	int iPlayerID,iMapID,iCityID;
	m_edPlayerID.GetWindowText(strPlayerID);
	m_cbMapList.GetWindowText(strMapID);
	m_edCityID.GetWindowText(strCityID);
	if(strPlayerID.IsEmpty())
	{
		m_csInfo.SetWindowText("Please enter player id.");
		m_edPlayerID.SetFocus();
		return;
	}
	if(strMapID.IsEmpty())
	{
		m_csInfo.SetWindowText("Please enter MapID.");
		m_cbMapList.SetFocus();
		return;
	}
	if(strCityID.IsEmpty())
	{
		m_csInfo.SetWindowText("Please enter CityID.");
		m_edCityID.SetFocus();
		return;
	}
	iPlayerID = atoi(strPlayerID);
	iMapID = atoi(strMapID);
	iCityID = atoi(strCityID);
	
	int content = 50000;
	int iWeaponCount = 0;
	int i=0;
	char *buf = 0;
	int count;

	__int64 lCompleteTime = 0;
	PlayerResource restb;
	restb.setmasterid(iCityID,iMapID);
	restb.setdetailid(iPlayerID);
	int iRet = m_DBProxy.db_select(restb);
	if( iRet != DB_Proxy::DB_SUCCEED)
	{
		m_csInfo.SetWindowText("查询PlayerResource表失败.");
		return;
	}
	lCompleteTime = restb.completetimes_;
	PlayerConstruction constb;

	constb.setmasterid(iMapID,iCityID,iPlayerID);
	PlayerConstruction *pConsSet = 0;
	iRet = m_DBProxy.db_multi_select(constb,buf,count);
	if( iRet == DB_Proxy::DB_FAILED)
	{
		m_csInfo.SetWindowText("查询PlayerConstruction表失败.");
		return;
	}
	else if(iRet == DB_Proxy::DB_EOF)
	{
	}
	else
	{
		count = count/sizeof(PlayerConstruction);
		pConsSet = (PlayerConstruction*)buf;
		for(i=0;i<count;i++)
		{
			if(pConsSet[i].constructionid_ == 5)
			{
				content = pConsSet[i].output_;
				break;
			}
		}
		if(buf)
		{
			delete[] buf;
			buf = 0;
		}
	}
	ProductionAmount protb;
	ProductionAmount *pRetSet = 0;
	for(i=1;i<=4;i++)
	{
		protb.setmasterid(iMapID,iCityID,iPlayerID,i);
		if(m_DBProxy.db_multi_select(protb,buf,count) != DB_Proxy::DB_SUCCEED)
			continue;

		count = count/sizeof(ProductionAmount);
		pRetSet = (ProductionAmount*)buf;
		for(int j=0;j<count;j++)
		{
			iWeaponCount = iWeaponCount + pRetSet[j].productioncount_;
		}
		if(buf)
		{
			delete[] buf;
			buf = 0;
		}
	}

	content = content -restb.foodcount_ - restb.woodcount_ -restb.skincount_ - restb.ironcount_;
	content = content - restb.redstonecount_ - restb.yollowstonecount_ - restb.greenstonecount_ - restb.backironcount_;
	content = content - iWeaponCount;
	content = content - restb.content_ ;
	memset(&restb,0,sizeof(restb));
	restb.content_ = content;
	restb.setmasterid(iCityID,iMapID);
	restb.setdetailid(iPlayerID);
	time_t cur_tm;
	time(&cur_tm);
	restb.completetimes_ = cur_tm - lCompleteTime;
	if(m_DBProxy.db_increase(restb) != DB_Proxy::DB_SUCCEED)
	{
		m_csInfo.SetWindowText("修改仓库容量失败.");
	}
	else
	{
		m_csInfo.SetWindowText("修改仓库容量成功.");
	}
}

void CDataManageDlg::OnBnClickedBtnHeromanage()
{
	// TODO: 在此添加控件通知处理程序代码
	m_csInfo.SetWindowText("");
	CString strPlayerID;
	m_edPlayerID.GetWindowText(strPlayerID);
	if(strPlayerID.IsEmpty())
	{
		m_csInfo.SetWindowText("Please enter PlayerID.");
		m_edPlayerID.SetFocus();
		return;
	}

	CPlayerOfficer dlgPlayerOfficer;
	dlgPlayerOfficer.DoModal();
}

void CDataManageDlg::OnBnClickedButtonHero()
{
	// TODO: 在此添加控件通知处理程序代码
	m_csInfo.SetWindowText("");
	CString strPlayerID;
	int iPlayerID;
	int iJobID = m_cbHeroJob.GetItemData(m_cbHeroJob.GetCurSel());
	m_edPlayerID.GetWindowText(strPlayerID);

	if(strPlayerID.IsEmpty())
	{
		m_csInfo.SetWindowText("Please enter player id.");
		m_edPlayerID.SetFocus();
		return;
	}
	iPlayerID = atoi(strPlayerID);
	//判断玩家阵营
	PlayerChatRoomTable playertb;
	playertb.setid(iPlayerID);
	if(m_DBProxy.db_select(playertb) != DB_Proxy::DB_SUCCEED)
	{
		m_csInfo.SetWindowText("招募英雄失败，查询PlayerChatRoomTable失败.");
		return;
	}
	//要判断玩家英雄数量，不能超过20个。
	PlayerOfficerTable *pRetSet = 0;
	char *buf = 0;
	int count = 0;
	PlayerOfficerTable offtb,mainofficer;
	offtb.setmasterid(iPlayerID);
	if(m_DBProxy.db_multi_select(offtb,buf,count) != DB_Proxy::DB_SUCCEED)
	{
		m_csInfo.SetWindowText("查询表PlayerOfficerTable失败");
		return;
	}
	count = count/sizeof(PlayerOfficerTable);
	if(count >=10)
	{
		m_csInfo.SetWindowText("玩家的英雄数量已到上限，无法再招募。");
		delete[] buf;
		buf = 0;
		return;
	}
	pRetSet = (PlayerOfficerTable*)buf;
	for(int i=0;i<count;i++)
	{
		if(pRetSet[i].officerid_ >offtb.officerid_ && pRetSet[i].officerid_ != pRetSet[i].playerid_)
		{
			offtb.officerid_ = pRetSet[i].officerid_;
		}
		if(pRetSet[i].officerid_ == pRetSet[i].playerid_)
		{
			memcpy(&mainofficer,&pRetSet[i],sizeof(PlayerOfficerTable));
		}
	}
	offtb.officerid_ +=11;
	if(buf)
	{
		delete[] buf;
		buf = 0;
	}
	int iMaxHeroNum = 9;//1 + mainofficer.militaryrank_;
	if(iMaxHeroNum >9)
	{
		iMaxHeroNum = 9;
	}
	if(count >= iMaxHeroNum)
	{
		m_csInfo.SetWindowText("玩家的英雄数量已到上限，无法再招募。");
		return;
	}
	offtb.mapid_ = 0;
	offtb.cityid_ = 0;
	offtb.level_ = 1;
	offtb.exp_ = 0;
	if(iJobID <=-1)
	{
		offtb.jobid_ = 2+rand()%4;
	}
	else
	{
		offtb.jobid_ = iJobID;
	}

	offtb.militaryrank_ = 0;
	offtb.credit_ = 0;
	offtb.enemycount_ = 0;
	offtb.generalcount_ = 0;
	offtb.state_ = 0;

	int iracebase = 1;
	if(playertb.country_ == 1)
	{
		iracebase = 1;
	}
	else
	{
		iracebase = 5;
	}
	offtb.officerrace_ = iracebase+rand()%4;
	//头像与职位匹配
	int iRaceID = offtb.officerrace_ -1;
	while(1)
	{
		if(m_RaceJobParam[iRaceID].IsKnight && offtb.jobid_ == 1)
			break;
		else if(m_RaceJobParam[iRaceID].IsSoldier && offtb.jobid_ ==2)
			break;
		else if(m_RaceJobParam[iRaceID].IsShooter && offtb.jobid_ == 3)
			break;
		else if(m_RaceJobParam[iRaceID].IsMaster && offtb.jobid_ == 4)
			break;
		else if(m_RaceJobParam[iRaceID].IsPastor && offtb.jobid_ == 5)
			break;
		else
		{
			offtb.officerrace_ = iracebase+rand()%4;
			iRaceID = offtb.officerrace_ -1;
		}
	}

	
	switch(offtb.officerrace_)
	{
	case 1:
		offtb.faceid_ = 1101+rand()%5;
		break;
	case 2:
		offtb.faceid_ = 2201+rand()%5;
		break;
	case 3:
		offtb.faceid_ = 3101+rand()%5;
		break;
	case 4:
		offtb.faceid_ = 4101+rand()%5;
		break;
	case 5:
		offtb.faceid_ = 5101+rand()%5;
		break;
	case 6:
		offtb.faceid_ = 6101+rand()%5;
		break;
	case 7:
		offtb.faceid_ = 7201+rand()%5;
		break;
	case 8:
		offtb.faceid_ = 8101+rand()%5;
		break;
	};

	if (offtb.faceid_ < 1101) 
	{
		offtb.faceid_ = 1101;
	}
	//性别
	int iSex = 20;
	if (offtb.officerrace_ == 2 || offtb.officerrace_ == 7)
		iSex = 259;

	offtb.stamina_ =50;
	offtb.power_ = 50;
	offtb.celerity_ = 50;
	offtb.wit_ = 50;
	offtb.techdot_ = 1;
	ProcHeroAttribute(offtb);
	
	char szName[24];
	memset(szName, 0, sizeof(szName));
	sprintf(szName, "%s · %s", m_HeroName[rand()%20], m_HeroName[iSex+rand()%230]);
	strcpy(offtb.officername_, szName);
	offtb.armyid_ = 0;
	offtb.state_ = 0;

	if(m_DBProxy.db_insert(offtb) != DB_Proxy::DB_SUCCEED)
	{
		m_csInfo.SetWindowText("招募英雄失败。");
	}
	else
	{
		//添加技能
		//增加英雄初始精专技能
		OfficerInitSkill_MultiMap_iterator lowerIter,upperIter; 
		OfficerInitSkill *officerInitSkill = NULL;
		lowerIter = getOfficerInitSkill_Lowerbound(offtb.jobid_);
		upperIter = getOfficerInitSkill_Upperbound(offtb.jobid_);
		for (;lowerIter != upperIter;lowerIter++)
		{
			officerInitSkill = lowerIter->second;
			if (officerInitSkill == NULL)
			{
				continue;
			}
			SkillDetail  *skillDetail = getSkillDetail((SKILL_TYPE)officerInitSkill->skill,officerInitSkill->skillLevel);

			if(skillDetail)
			{
				PlayerSkillTable officerSkill;
				officerSkill.setmasterid(offtb.playerid_,offtb.officerid_);
				officerSkill.setdetailid(skillDetail->skillID);
				officerSkill.skilltype_ = skillDetail->skillType;
				officerSkill.skilllevel_ = skillDetail->level;
				if (m_DBProxy.db_insert(officerSkill) != DB_Proxy::DB_SUCCEED)
				{		
				}
			}
		}
		m_csInfo.SetWindowText("招募英雄成功。");
	}
}
//读取英雄初始技能参数表
void CDataManageDlg::readOfficerInitSkill()
{
	FILE *fp = 0;
	fp = fopen("OfficerInitSkill.csv","r");
	if(fp == NULL)
	{
		printf("OfficerInitSkill.csv失败。\n");
		return ;
	}

	char buf[1024]= {0};
	fgets(buf,1024,fp);

	OfficerInitSkill *pItem = 0;	//

	while(!feof(fp))
	{
		if(fgets(buf,1024,fp) == NULL)
			break;
		pItem = new OfficerInitSkill;
		memset(pItem,0,sizeof(OfficerInitSkill));
		if(sscanf(buf,"%d,%d,%d",
			&pItem->jobID,&pItem->skill,&pItem->skillLevel)<3)
		{
			delete pItem;
			pItem = 0;
			continue;
		}
		officerInitSkillMultiMap.insert(OfficerInitSkill_MultiMap_Pair(pItem->jobID,pItem));
	}
	fclose(fp);
}
void CDataManageDlg::clearOfficerInitSkill()
{
	OfficerInitSkill_MultiMap_iterator iter;
	OfficerInitSkill * delOfficerInitSkill = 0;
	for (iter = officerInitSkillMultiMap.begin();iter != officerInitSkillMultiMap.end();iter++)
	{
		if((delOfficerInitSkill = iter->second))
		{
			delete delOfficerInitSkill;
			delOfficerInitSkill = 0;
		}
	}
	officerInitSkillMultiMap.clear();
}
//根据职业ID 获取该职业默认的技能参数
OfficerInitSkill_MultiMap_iterator CDataManageDlg::getOfficerInitSkill_Lowerbound(int jobid)
{
	return officerInitSkillMultiMap.lower_bound(jobid);
}
OfficerInitSkill_MultiMap_iterator CDataManageDlg::getOfficerInitSkill_Upperbound(int jobid)
{
	return officerInitSkillMultiMap.upper_bound(jobid);
}
//void CDataManageDlg::OnBnClickedButtonAddcity()
//{
//	// TODO: 在此添加控件通知处理程序代码
//}

void CDataManageDlg::OnBnClickedButtonDataview()
{
	// TODO: 在此添加控件通知处理程序代码
	
	m_csInfo.SetWindowText("");
	CString strPlayerID,strMapID,strCityID;
	m_edPlayerID.GetWindowText(strPlayerID);
	m_cbMapList.GetWindowText(strMapID);
	m_edCityID.GetWindowText(strCityID);
	if(strPlayerID.IsEmpty())
	{
		m_csInfo.SetWindowText("Please enter player id.");
		m_edPlayerID.SetFocus();
		return;
	}
	if(strMapID.IsEmpty())
	{
		m_csInfo.SetWindowText("Please enter MapID.");
		m_cbMapList.SetFocus();
		return;
	}
	if(strCityID.IsEmpty())
	{
		m_csInfo.SetWindowText("Please enter CityID.");
		m_edCityID.SetFocus();
		return;
	}
	CRecourceView viewDlg;
	viewDlg.DoModal();
	
}

void CDataManageDlg::OnBnClickedButtonInit()
{
	// TODO: 在此添加控件通知处理程序代码
	
	CString strTmp;
	if(MessageBox("确定初始化吗？请慎用","提示",MB_ICONINFORMATION|MB_OKCANCEL) != IDOK)
		return;
	//读取所有玩家
	PlayerChatRoomTable playertb;
	PlayerChatRoomTable *pPlayerList = 0;
	char *pPlayerBuf = 0;
	int iPlayerCount;
	m_csInfo.SetWindowText("");
	if(m_DBProxy.db_select_all(playertb,pPlayerBuf,iPlayerCount) != DB_Proxy::DB_SUCCEED)
	{
		m_csInfo.SetWindowText("查询表PlayerChatRoomTable失败");
		return;
	}
	iPlayerCount = iPlayerCount/sizeof(PlayerChatRoomTable);
	pPlayerList = (PlayerChatRoomTable*)pPlayerBuf;
	
	PlayerOfficerTable officertb;
	PlayerOfficerTable *pOfficerList = 0;
	char *pOfficerBuf = 0;
	int iOfficerCount = 0;


	PlayerCity citytb;
	PlayerCity *pCityList = 0;
	char *pCityBuf = 0;
	int iCityCount = 0;

	PlayerConstruction playerCons;
	PlayerResource playerRes;
	time_t curtm;
	time(&curtm);
	for(int i=0;i<iPlayerCount;i++)
	{
		//更新玩家所有英雄的状态为空闲
		officertb.setmasterid(pPlayerList[i].playerid_);
		if(m_DBProxy.db_multi_select(officertb,pOfficerBuf,iOfficerCount) == DB_Proxy::DB_SUCCEED)
		{
			iOfficerCount = iOfficerCount/sizeof(PlayerOfficerTable);
			pOfficerList = (PlayerOfficerTable*)pOfficerBuf;
			for(int j=0;j<iOfficerCount;j++)
			{
				pOfficerList[j].state_ = 0;
				m_DBProxy.db_update(pOfficerList[j],pOfficerList[j]);
			}
			if(pOfficerBuf)
			{
				delete[] pOfficerBuf;
				pOfficerBuf = 0;
			}
		}
		//根据玩家城市表增加仓库
		citytb.setmasterid(pPlayerList[i].playerid_);
		if(m_DBProxy.db_multi_select(citytb,pCityBuf,iCityCount) == DB_Proxy::DB_SUCCEED)
		{
			iCityCount = iCityCount/sizeof(PlayerCity);
			pCityList = (PlayerCity*)pCityBuf;
			for(int k = 0;k<iCityCount;k++)
			{
				//为每个城市增加仓库，根据仓库基本，填写仓库容量
				playerCons.setmasterid(pCityList[k].mapid_,pCityList[k].cityid_,pPlayerList[i].playerid_);
				playerCons.setdetailid(1);
				if(m_DBProxy.db_select(playerCons) != DB_Proxy::DB_SUCCEED)
				{
					strTmp.Format("玩家%d的城市%d-%d没有仓库",pPlayerList[i].playerid_,pCityList[k].mapid_,pCityList[k].cityid_);
					MessageBox(strTmp);
					continue;
				}
				playerRes.setmasterid(pCityList[k].cityid_,pCityList[k].mapid_);
				playerRes.setdetailid(pPlayerList[i].playerid_);
				playerRes.content_ = playerCons.output_;
				playerRes.completetimes_ = curtm;
				m_DBProxy.db_insert(playerRes);
			}
		}
	}

	if(pPlayerBuf)
	{
		delete[] pPlayerBuf;
		pPlayerBuf = 0;
	}
	//根据地图状态，删除NPC部队
	MapInfoTable maptb;
	MapInfoTable *pMapList = 0;
	char *pMapBuf = 0;
	int iMapCount = 0;
	if(m_DBProxy.db_select_all(maptb,pMapBuf,iMapCount) != DB_Proxy::DB_SUCCEED)
	{
		m_csInfo.SetWindowText("查询MapInfoTable失败");
		return ;
	}
	iMapCount = iMapCount/sizeof(MapInfoTable);
	pMapList = (MapInfoTable*)pMapBuf;

	ArmyTable armytb;
	ArmyTable *pArmyList = 0;
	char *pArmyBuf = 0;
	int iArmyCount = 0;
	for(int m=0;m<iMapCount;m++)
	{
		if(pMapList[m].countryid_ != 0)
			continue;

		armytb.setmasterid(pMapList[m].mapid_);
		if(m_DBProxy.db_multi_select(armytb,pArmyBuf,iArmyCount) != DB_Proxy::DB_SUCCEED)
			continue;

		iArmyCount = iArmyCount/sizeof(ArmyTable);
		pArmyList = (ArmyTable*)pArmyBuf;
		for(int a = 0;a<iArmyCount;a++)
		{
			m_DBProxy.db_delete(pArmyList[a]);
		}
		if(pArmyBuf)
		{
			delete[] pArmyBuf;
			pArmyBuf = 0;
		}
	}
	if(pMapBuf)
	{
		delete[] pMapBuf;
		pMapBuf = 0;
	}
	m_csInfo.SetWindowText("初始化成功");
}

void CDataManageDlg::OnBnClickedBtnResume()
{
	// TODO: 在此添加控件通知处理程序代码
	m_csInfo.SetWindowText("");
	PlayerOfficerTable offtb,updateOfftb;
	PlayerOfficerTable *pRetSet = 0;
	char *buf = 0;
	int count = 0;
	int iTotal  =0;
	if(m_DBProxy.db_select_all(offtb,buf,count) != DB_Proxy::DB_SUCCEED)
	{
		m_csInfo.SetWindowText("查询指挥官表失败");
		return;
	}
	count = count/sizeof(PlayerOfficerTable);
	pRetSet = (PlayerOfficerTable*)buf;
	for(int i=0;i<count;i++)
	{
		if(pRetSet[i].playerid_<100000)//非玩家英雄
			continue;
		if(pRetSet[i].state_ == EN_GBH || pRetSet[i].state_ <0)//重伤
		{
			memset(&updateOfftb,0,sizeof(PlayerOfficerTable));
			updateOfftb.setmasterid(pRetSet[i].playerid_);
			updateOfftb.setdetailid(pRetSet[i].officerid_);
			updateOfftb.state_ = -pRetSet[i].state_;
			m_DBProxy.db_increase(updateOfftb);
			iTotal++;
		}
	}
	if(buf)
	{
		delete[] buf;
		buf = 0;
	}
	
	CString strTmp;
	strTmp.Format("恢复成功,共恢复：%d",iTotal);
	m_csInfo.SetWindowText(strTmp);
}

void CDataManageDlg::OnBnClickedButtonGetlevel()
{
	// TODO: 在此添加控件通知处理程序代码
	m_csInfo.SetWindowText("");
	CString strCurLevel,strExp;
	int iCurLevel,iExp;
	m_edCurLevel.GetWindowText(strCurLevel);
	m_edExp.GetWindowText(strExp);
	iCurLevel = atoi(strCurLevel);
	iExp = atoi(strExp);
	int iNextLevel = iCurLevel;
	int iNextExp = 0;
	while(1)
	{
		iNextLevel = iCurLevel + 1;//下一级别
		iNextExp = GetExp(iNextLevel);
		if(iNextExp <0)
			break;

		if(iExp < iNextExp || iNextExp ==0)
			break;
		iExp -= iNextExp;//剩余Exp
		iCurLevel = iNextLevel;
	}
	CString strTmp;
	strTmp.Format("新等级为：%d,剩余经验值：%d",iCurLevel,iExp);
	m_csInfo.SetWindowText(strTmp);
}
void CDataManageDlg::OnBnClickedButtonDelcons()
{
	// TODO: 在此添加控件通知处理程序代码
	/*
	PlayerConstruction constb;
	PlayerConstruction *pConsList = 0;
	char *buf = 0;
	int count = 0;
	m_csInfo.SetWindowText("");
	if(m_DBProxy.db_select_all(constb,buf,count) != DB_Proxy::DB_SUCCEED)
	{
		m_csInfo.SetWindowText("查询失败。");
		return;
	}
	count = count/sizeof(PlayerConstruction);
	pConsList = (PlayerConstruction*)buf;
	for(int i=0;i<count;i++)
	{
		if(pConsList[i].constructionid_ == 8 || pConsList[i].cityid_ <1000)
		{
			m_DBProxy.db_delete(pConsList[i]);
		}
	}

	if(buf)
	{
		delete[] buf;
		buf = 0;
	}
	m_csInfo.SetWindowText("删除成功。");
	m_csInfo.SetWindowText("");
	PlayerOfficerTable offtb;
	PlayerOfficerTable *pOffList = 0;
	char *buf = 0;
	int count = 0;
	if(m_DBProxy.db_select_all(offtb,buf,count) != DB_Proxy::DB_SUCCEED)
	{
		return;
	}
	count = count/sizeof(PlayerOfficerTable);
	pOffList = (PlayerOfficerTable*)buf;
	for(int i=0;i<count;i++)
	{
		if(pOffList[i].level_ >60)
		{
			pOffList[i].level_ = 30;
			m_DBProxy.db_update(pOffList[i],pOffList[i]);
		}
	}
	if(buf)
	{
		delete[] buf;
		buf = 0;
	}
	m_csInfo.SetWindowText("修改级别成功。");
	
	
	*/
	/*
	m_csInfo.SetWindowText("");
	PlayerCompare playercmp;
	PlayerCompare *pRetSet = 0;
	char *buf = 0;
	int count = 0;
	int iRet = 0;
	iRet =m_DBProxy.db_select_all(playercmp,buf,count);
	if( iRet == DB_Proxy::DB_EOF)
	{
		m_csInfo.SetWindowText("记录为空");
		return;
	}
	else if(iRet == DB_Proxy::DB_FAILED)
	{
		return;
	}

	count = count/sizeof(PlayerCompare);
	pRetSet = (PlayerCompare*)buf;
	for(int i=0;i<count;i++)
	{
	}
	if(buf)
	{
		delete[] buf;
		buf = 0;
	}
	CString strTmp;
	strTmp.Format("记录数:%d",count);
	m_csInfo.SetWindowText(strTmp);
	m_csInfo.SetWindowText("");
	PullulateNote pulnottb;
	PullulateNote *pPulNotList = 0;
	char *buf = 0;
	int count = 0;
	if(m_DBProxy.db_select_all(pulnottb,buf,count) != DB_Proxy::DB_SUCCEED)
	{
		m_csInfo.SetWindowText("记录为空");
		return;
	}

	count = count/sizeof(PullulateNote);
	pPulNotList =(PullulateNote*)buf;
	time_t cur_tm;
	time(&cur_tm);
	for(int i=0;i<count;i++)
	{
		if(pPulNotList[i].upgradedate_ >100000000000)
			pPulNotList[i].upgradedate_ = cur_tm;

		m_DBProxy.db_update(pPulNotList[i],pPulNotList[i]);
	}
	if(buf)
	{
		delete[] buf;
		buf = 0;
	}
	m_csInfo.SetWindowText("修改成功。");

	PlayerCaptiveHero captivertb;
	char *buf = 0;
	int count = 0;
	if(m_DBProxy.db_select_all(captivertb,buf,count) != DB_Proxy::DB_SUCCEED)
	{
		m_csInfo.SetWindowText("记录为空");
		return;
	}
	count = count/sizeof(PlayerCaptiveHero);
	CString strTmp;
	strTmp.Format("被俘记录表的记录数为：%d",count);
	m_csInfo.SetWindowText(strTmp);


	ArmyTable armytb;
	ArmyOutTable armyouttb;
	armytb.setmasterid(1028);
	armytb.setdetailid(13261);
	m_DBProxy.db_select(armytb);
	armyouttb.setmasterid(armytb.playerid_);
	armyouttb.setdetailid(13261);
	m_DBProxy.db_delete(armyouttb);
	m_DBProxy.db_delete(armytb);
	int iPlayerID;
	CString strPlayerID;
	m_edPlayerID.GetWindowText(strPlayerID);
	iPlayerID = atoi(strPlayerID);
	PlayerChatRoomTable playertb,updateplayertb;
	playertb.setid(iPlayerID);
	updateplayertb.setid(iPlayerID);
	if(m_DBProxy.db_select(playertb) != DB_Proxy::DB_SUCCEED)
	{
		m_csInfo.SetWindowText("查询玩家信息失败.");
		return;
	}
	updateplayertb.contribute_ = -playertb.contribute_;
	if(m_DBProxy.db_increase(updateplayertb) != DB_Proxy::DB_SUCCEED)
	{
		m_csInfo.SetWindowText("修改玩家捐献信息失败.");
	}
	else
	{
		m_csInfo.SetWindowText("修改玩家捐献信息成功。.");
	}
	*/
/*
	Country country;
	country.setid(2);
	if(m_DBProxy.db_select(country) != DB_Proxy::DB_SUCCEED)
	{
		m_csInfo.SetWindowText("查询阵营信息失败.");
		return;
	}
	country.nexttime2_ = 0;
	country.mapid_ = 0;
	if(m_DBProxy.db_update(country,country) != DB_Proxy::DB_SUCCEED)
	{
	}
	else
	{
	}*/
/*
	CString strPlayerID;
	int iPlayerID;
	m_edPlayerID.GetWindowText(strPlayerID);
	iPlayerID = atoi(strPlayerID);

	PlayerCity playecity;
	PlayerCity *pCityList;
	char *buf = 0;
	int count = 0;
	int mapid = 1001;
	int cityid = 10010101;
	PlayerChatRoomTable playertb;
	playertb.setid(iPlayerID);
	if(m_DBProxy.db_select(playertb) != DB_Proxy::DB_SUCCEED)
		return;

	if(playertb.country_ == 1)
	{
		mapid = 1003;
		cityid = 10030101;
	}
	else if(playertb.country_ == 2)
	{
		mapid = 1007;
		cityid = 10070101;
	}
	else
	{
		return;
	}

	PlayerResource playerres;
	playecity.setmasterid(iPlayerID);
	if(m_DBProxy.db_multi_select(playecity,buf,count) != DB_Proxy::DB_SUCCEED)
		return;
	pCityList = (PlayerCity*)buf;
	count = count/sizeof(PlayerCity);
	for(int i=0;i<count;i++)
	{
		if(pCityList[i].state_ == 100)
		{
			m_DBProxy.db_delete(pCityList[i]);
			playerres.setmasterid(pCityList[i].cityid_,pCityList[i].mapid_);
			playerres.setdetailid(iPlayerID);
			if(m_DBProxy.db_select(playerres) == DB_Proxy::DB_SUCCEED)
			{
				m_DBProxy.db_delete(playerres);
				playerres.mapid_ = mapid;
				playerres.cityid_ = cityid;
				m_DBProxy.db_insert(playerres);
			}
			pCityList[i].mapid_ = mapid;
			pCityList[i].cityid_ = cityid;
			m_DBProxy.db_insert(pCityList[i]);
			break;
		}
	}
	if(buf)
	{
		delete[] buf;
		buf= 0;
	}
*/
/*
	ArmyTable armytb;
	ArmyTable *pArmyList = 0;
	char *buf = 0;
	int count = 0;
	ArmyOutTable armyout;
	if(m_DBProxy.db_select_all(armytb,buf,count) != DB_Proxy::DB_SUCCEED)
		return;
	count = count/sizeof(ArmyTable);
	pArmyList = (ArmyTable*)buf;
	for(int i =0;i<count;i++)
	{
		if(pArmyList[i].playerid_ != 100 && pArmyList[i].playerid_ != 200)
			continue;

		if(pArmyList[i].state_ != 2)
			continue;

		armyout.setmasterid(pArmyList[i].playerid_);
		armyout.setdetailid(pArmyList[i].armyid_);
		m_DBProxy.db_delete(armyout);
		pArmyList[i].state_ = 1;
		pArmyList[i].officerid_ = pArmyList[i].playerid_;
		m_DBProxy.db_update(pArmyList[i],pArmyList[i]);
	}
	*/
	MapInfoTable maptb;
	MapInfoTable *pMapList = 0;
	char *buf = 0;
	int count = 0;
	if(m_DBProxy.db_select_all(maptb,buf,count) != DB_Proxy::DB_SUCCEED)
		return;
	count = count/sizeof(MapInfoTable);
	pMapList = (MapInfoTable *)buf;
	for(int i =0;i<count;i++)
	{
		if(pMapList[i].countryid_ !=0)
		{
		}
		else
		{
			pMapList[i].countryid_ = 2;
			pMapList[i].state_ = 0;
			m_DBProxy.db_update(pMapList[i],pMapList[i]);
		}
		/*
		if(pMapList[i].mapid_ <1010 &&  pMapList[i].state_ != 100)
		{
			pMapList[i].state_ =100;
			m_DBProxy.db_update(pMapList[i],pMapList[i]);
		}
		else if(pMapList[i].countryid_ !=0 && pMapList[i].state_ !=0)
		{
			pMapList[i].state_ = 0;
			m_DBProxy.db_update(pMapList[i],pMapList[i]);
		}
		else
		{
			pMapList[i].countryid_ = 2;
			pMapList[i].state_ = 0;
			m_DBProxy.db_update(pMapList[i],pMapList[i]);
		}
		*/
	}
	if(buf)
	{
		delete[] buf;
		buf = 0;
	}
}

void CDataManageDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CInitPlayerRes dlg;
	dlg.DoModal();
}

void CDataManageDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	PlayerOfficerTable officer;
	PlayerOfficerTable *pOfficerList = 0;
	char *buf = 0;
	int count = 0;
	//officer.setmasterid(111441);
	if(m_DBProxy.db_select_all(officer,buf,count) != DB_Proxy::DB_SUCCEED)
		return;
	count = count/sizeof(PlayerOfficerTable);
	pOfficerList = (PlayerOfficerTable*)buf;
	int i=0;
	int count1 = 0;
	for(i=0;i<count;i++)
	{
		if(pOfficerList[i].state_ != EN_GBH && pOfficerList[i].state_ != EN_BY_CAPTIVE)
			continue;
		pOfficerList[i].state_ = 0;
		m_DBProxy.db_update(pOfficerList[i],pOfficerList[i]);
		count1++;
	}
	if(buf)
	{
		delete[] buf;
		buf = 0;
	}

	PlayerCaptiveHero caphero;
	PlayerCaptiveHero *pHeroList = 0;
	count = 0;
	if(m_DBProxy.db_select_all(caphero,buf,count) == DB_Proxy::DB_SUCCEED)
	{

		count = count/sizeof(PlayerCaptiveHero);
		pHeroList = (PlayerCaptiveHero*)buf;
		for(i = 0;i<count;i++)
		{
			m_DBProxy.db_delete(pHeroList[i]);
		}
		if(buf)
		{
			delete[] buf;
			buf = 0;
		}
	}
	CString strTmp;
	strTmp.Format("共修改英雄:%d，被俘列表%d",count1,count);
	m_csInfo.SetWindowText(strTmp);
}

void CDataManageDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	MailTable mailtb;
	MailTable *pMailList = 0;
	char *buf = 0;
	int count = 0;
	if(m_DBProxy.db_select_all(mailtb,buf,count) != DB_Proxy::DB_SUCCEED)
	{
		m_csInfo.SetWindowText("邮件表为空。");
		return;
	}
	count = count/sizeof(MailTable);
	pMailList = (MailTable*)buf;
	for(int i=0;i<count;i++)
	{
		m_DBProxy.db_delete(pMailList[i]);
	}
	if(buf)
	{
		delete[] buf;
		buf = 0;
	}
	CString strTmp;
	strTmp.Format("清空邮件表成功，共删除%d封邮件",count);
	m_csInfo.SetWindowText(strTmp);
}

int CDataManageDlg::ReadEquipAttribute()
{
	/*
	EquipAttribute equiptb;
	EquipAttribute *pEquipList = 0;
	char *buf = 0;
	int count = 0;
	if (m_DBProxy.db_select_all(equiptb,buf,count) != DB_Proxy::DB_SUCCEED)
	{
		return 1;
	}
	count = count/sizeof(EquipAttribute);
	pEquipList = (EquipAttribute*)buf;
	EquipAttribute *pNewItem = 0;
	for (int i=0;i<count;i++)
	{
		pNewItem = new EquipAttribute;
		memcpy(pNewItem,&pEquipList[i],sizeof(EquipAttribute));
		m_EquipAttributeList[pNewItem->equipid_] = pNewItem;
	}
	if (buf)
	{
		delete[] buf;
		buf = 0;
	}
	*/
	FILE *fp = 0;
	fp = fopen("OldEquipAttribute.csv","r");
	if(fp == NULL)
	{
		printf("OldEquipAttribute.csv失败。\n");
		return -1;
	}

	char buf[1024]= {0};
	fgets(buf,1024,fp);

	EquipAttribute *pNewItem = 0;
	/*
	int equipid_; // equipid_ = 一条
	int level_;
	int pos_;
	int endurance_;
	int str_;
	int agi_;
	int int_;
	int attack_;
	int attackrat_;
	int defend_;
	int basicprice_;*/
	while(!feof(fp))
	{
		if(fgets(buf,1024,fp) == NULL)
			break;
		pNewItem = new EquipAttribute;
		memset(pNewItem,0,sizeof(EquipAttribute));
		if(sscanf(buf,"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
			&pNewItem->equipid_,&pNewItem->level_,&pNewItem->pos_,&pNewItem->endurance_,&pNewItem->str_,&pNewItem->agi_,
			&pNewItem->int_,&pNewItem->attack_,&pNewItem->attackrat_,&pNewItem->defend_,&pNewItem->basicprice_)<11)
		{
			delete pNewItem;
			pNewItem = 0;
			continue;
		}
		m_EquipAttributeList[pNewItem->equipid_] = pNewItem;
	}
	fclose(fp);
	return 0;
}
EquipAttribute* CDataManageDlg::GetEquipAttribute(int id)
{
	EquipAttribute_Iterator iter;
	iter = m_EquipAttributeList.find(id);
	if(iter == m_EquipAttributeList.end())
		return NULL;
	return iter->second;
}
int CDataManageDlg::ClearEquipAttribute()
{
	EquipAttribute_Iterator iter;
	EquipAttribute *pDelItem = 0;
	for(iter =m_EquipAttributeList.begin();iter != m_EquipAttributeList.end();iter++)
	{
		if(pDelItem = iter->second)
		{
			delete pDelItem;
			pDelItem = 0;
		}
	}
	m_EquipAttributeList.clear();
	return 0;
}
int CDataManageDlg::ReadCardConfig()
{
	FILE *fp = 0;
	fp = fopen("CardConfig.csv","r");
	if(fp == NULL)
	{
		printf("CardConfig.csv失败。\n");
		return -1;
	}

	char buf[1024]= {0};
	fgets(buf,1024,fp);

	LPCardParam pCardParamItem = 0;
	//
	int iPicture = 0;
	char efcetBuf[128]={0};
	int iOpenEnable = 0;
	int iKind = 0;
	while(!feof(fp))
	{
		if(fgets(buf,1024,fp) == NULL)
			break;
		pCardParamItem = new CardParam;
		memset(pCardParamItem,0,sizeof(CardParam));
		if(sscanf(buf,"%d,%[^,],%[^,],%d,%d,%d,%d,%d,%d,%d,%d",
			&pCardParamItem->CardID,&pCardParamItem->CardName,&efcetBuf,&iPicture,&pCardParamItem->Type,&iKind,
			&pCardParamItem->Price,&pCardParamItem->Value,&pCardParamItem->Time,&pCardParamItem->Postion,&iOpenEnable)<11)
		{
			delete pCardParamItem;
			pCardParamItem = 0;
			continue;
		}
		pCardParamItem->OpenEnabled = (iOpenEnable >0)?1:0;
		m_CardConfigList[pCardParamItem->CardID] = pCardParamItem;
	}
	fclose(fp);
	return 0;
}
LPCardParam CDataManageDlg::GetCardConfig(int id)
{
	CardParam_Map_Iterator iter;
	iter = m_CardConfigList.find(id);
	if(iter == m_CardConfigList.end())
		return NULL;
	return iter->second;
}
int CDataManageDlg::ClearCardConfig()
{
	CardParam_Map_Iterator iter;
	LPCardParam pDelItem = 0;
	for (iter = m_CardConfigList.begin();iter != m_CardConfigList.end();iter++)
	{
		if(pDelItem = iter->second)
		{
			delete pDelItem;
			pDelItem = 0;
		}
	}
	m_CardConfigList.clear();
	return 0;
}
int CDataManageDlg::InitCardConfigList(CComboBox *pComboBox)
{
	pComboBox->Clear();
	CardParam_Map_Iterator iter;
	int index = 0;
	for (iter = m_CardConfigList.begin();iter != m_CardConfigList.end();iter++)
	{
		index = pComboBox->AddString(iter->second->CardName);
		pComboBox->SetItemData(index,iter->second->CardID);
	}
	return 0;
}

void CDataManageDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: 在此处添加消息处理程序代码

	ClearEquipAttribute();
	ClearCardConfig();
	ClearSpecialHeroParam();
	ClearWeaponParam();
	ClearHeroEquipParam();
	ClearTaskBFParam();
	clearOfficerInitSkill();
	clearSkillDetail();
	ClearPackStoreParam();
}

void CDataManageDlg::OnBnClickedButton4()
{

	// TODO: 在此添加控件通知处理程序代码
	m_csInfo.SetWindowText("");
	CString strPlayerID;
	int iPlayerID;
	m_edPlayerID.GetWindowText(strPlayerID);
	LPSpecialHeroParam pHeroParam = (LPSpecialHeroParam)m_cbSpecialHero.GetItemData(m_cbSpecialHero.GetCurSel());
	if (pHeroParam == NULL)
	{
		m_csInfo.SetWindowText("特殊英雄参数为空。");
		return;
	}

	if(strPlayerID.IsEmpty())
	{
		m_csInfo.SetWindowText("Please enter player id.");
		m_edPlayerID.SetFocus();
		return;
	}
	iPlayerID = atoi(strPlayerID);
	//判断玩家阵营
	PlayerChatRoomTable playertb;
	playertb.setid(iPlayerID);
	if(m_DBProxy.db_select(playertb) != DB_Proxy::DB_SUCCEED)
	{
		m_csInfo.SetWindowText("招募英雄失败，查询PlayerChatRoomTable失败.");
		return;
	}
	if ((playertb.country_ ==1 && pHeroParam->RaceID >4) ||(playertb.country_ ==2 && pHeroParam->RaceID <5))
	{
		m_csInfo.SetWindowText("该特殊英雄与玩家阵营不匹配，无法招募");
		return;
	}
	int iFlag = 0;
	//要判断玩家英雄数量，不能超过20个。
	PlayerOfficerTable *pRetSet = 0;
	char *buf = 0;
	int count = 0;
	PlayerOfficerTable offtb,mainofficer;
	offtb.setmasterid(iPlayerID);
	if(m_DBProxy.db_multi_select(offtb,buf,count) != DB_Proxy::DB_SUCCEED)
	{
		m_csInfo.SetWindowText("查询表PlayerOfficerTable失败");
		return;
	}
	count = count/sizeof(PlayerOfficerTable);
	if(count >=10)
	{
		m_csInfo.SetWindowText("玩家的英雄数量已到上限，无法再招募。");
		delete[] buf;
		buf = 0;
		return;
	}
	pRetSet = (PlayerOfficerTable*)buf;
	for(int i=0;i<count;i++)
	{
		if(pRetSet[i].officerid_ >offtb.officerid_ && pRetSet[i].officerid_ != pRetSet[i].playerid_)
		{
			offtb.officerid_ = pRetSet[i].officerid_;
		}
		if(pRetSet[i].officerid_ == pRetSet[i].playerid_)
		{
			memcpy(&mainofficer,&pRetSet[i],sizeof(PlayerOfficerTable));
		}
		if(strcmp(pRetSet[i].officername_,pHeroParam->Name) == 0)
		{
			iFlag = 1;
			break;
		}
	}
	offtb.officerid_ +=11;
	if(buf)
	{
		delete[] buf;
		buf = 0;
	}
	if (iFlag == 1)
	{
		m_csInfo.SetWindowText("玩家已有该名称的英雄。。。");
		return;
	}
	int iMaxHeroNum = 9;//1 + mainofficer.militaryrank_;
	if(iMaxHeroNum >9)
	{
		iMaxHeroNum = 9;
	}
	if(count >= iMaxHeroNum)
	{
		m_csInfo.SetWindowText("玩家的英雄数量已到上限，无法再招募。");
		return;
	}
	offtb.mapid_ = 0;
	offtb.cityid_ = 0;
	offtb.level_ = 1;
	offtb.exp_ = 0;
	offtb.jobid_ = pHeroParam->JobID;

	offtb.militaryrank_ = 0;
	offtb.credit_ = 0;
	offtb.enemycount_ = 0;
	offtb.generalcount_ = 0;
	offtb.state_ = 0;
	
	offtb.officerrace_ = pHeroParam->RaceID;
	offtb.faceid_ = pHeroParam->FaceID;

	offtb.stamina_ =pHeroParam->Stamina;
	offtb.power_ = pHeroParam->Power;
	offtb.celerity_ = pHeroParam->Celerity;
	offtb.wit_ = pHeroParam->Wit;
	offtb.techdot_ = 1;
	ProcHeroAttribute(offtb);
	strcpy(offtb.officername_, pHeroParam->Name);
	offtb.armyid_ = 0;
	offtb.state_ = 0;

	if(m_DBProxy.db_insert(offtb) != DB_Proxy::DB_SUCCEED)
	{
		m_csInfo.SetWindowText("招募特殊英雄失败。");
	}
	else
	{

		//增加英雄初始精专技能
		OfficerInitSkill_MultiMap_iterator lowerIter,upperIter; 
		OfficerInitSkill *officerInitSkill = NULL;
		lowerIter = getOfficerInitSkill_Lowerbound(offtb.jobid_);
		upperIter = getOfficerInitSkill_Upperbound(offtb.jobid_);
		for (;lowerIter != upperIter;lowerIter++)
		{
			officerInitSkill = lowerIter->second;
			if (officerInitSkill == NULL)
			{
				continue;
			}
			SkillDetail  *skillDetail = getSkillDetail((SKILL_TYPE)officerInitSkill->skill,officerInitSkill->skillLevel);

			if(skillDetail)
			{
				PlayerSkillTable officerSkill;
				officerSkill.setmasterid(offtb.playerid_,offtb.officerid_);
				officerSkill.setdetailid(skillDetail->skillID);
				officerSkill.skilltype_ = skillDetail->skillType;
				officerSkill.skilllevel_ = skillDetail->level;
				if (m_DBProxy.db_insert(officerSkill) != DB_Proxy::DB_SUCCEED)
				{		
				}
			}
		}
		m_csInfo.SetWindowText("招募特殊英雄成功。");
	}
}
int CDataManageDlg::ReadWeaponParam()
{
	FILE *fp = 0;
	fp = fopen("WeaponParam.csv","r");
	if(fp == NULL)
		return 1;

	char buf[128]= {0};
	fgets(buf,128,fp);

	LPWeaponParam pNewItem = 0;
	while(!feof(fp))
	{
		fgets(buf,128,fp);
		pNewItem = new WeaponParam;
		if(sscanf(buf,"%d,%[^,],%d",&pNewItem->ItemID,&pNewItem->Name,&pNewItem->Type) <3)
		{
			delete pNewItem;
			pNewItem = 0;
			continue;
		}
		m_WeaponParamList[pNewItem->ItemID] = pNewItem;
	}
	fclose(fp);
	return 0;
}
LPWeaponParam CDataManageDlg::GetWeaponParam(int id)
{
	WeaponParam_Map_Iterator iter = m_WeaponParamList.find(id);
	if(iter == m_WeaponParamList.end())
		return NULL;
	return iter->second;
}
int CDataManageDlg::ClearWeaponParam()
{
	WeaponParam_Map_Iterator iter;
	LPWeaponParam pDelItem = 0;
	for(iter = m_WeaponParamList.begin();iter !=m_WeaponParamList.end();iter++)
	{
		if(pDelItem = iter->second)
		{
			delete pDelItem;
			pDelItem = 0;
		}
	}
	m_WeaponParamList.clear();
	return 0;
}
int CDataManageDlg::InitWeaponParamComboBox(CComboBox *pComboBox)
{
	pComboBox->Clear();
	WeaponParam_Map_Iterator iter;
	int index = 0;
	for (iter = m_WeaponParamList.begin();iter != m_WeaponParamList.end();iter++)
	{
		index = pComboBox->AddString(iter->second->Name);
		pComboBox->SetItemData(index,iter->first);
	}
	return 0;
}

int CDataManageDlg::ReadHeroEquipParam()
{
	FILE *fp = 0;
	fp = fopen("EquipAttribute.csv","r");
	if(fp == NULL)
	{
		printf("打开EquipAttribute.csv失败。\n");
		return 0;
	}

	char buf[1024]= {0};
	fgets(buf,1024,fp);

	HeroEquipAttribute *pItem = 0;
	//m_HeroEquipParamList
	int iSale = 0;
	int iPicture = 0;
	char jobs[32]={0};
	int jobIDs[6] = {0};
	int jobCount = 0;
	while(!feof(fp))
	{
		if(fgets(buf,1024,fp) == NULL)
			break;
		pItem = new HeroEquipAttribute;
		memset(pItem,0,sizeof(HeroEquipAttribute));
		if(sscanf(buf,"%d,%[^,],%d,%d,%[^,],%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
			&pItem->itemID,&pItem->name,&pItem->level,&iPicture,&jobs,&pItem->postion,
			&pItem->needStamina,&pItem->needPower,&pItem->needCelerity,&pItem->needWit,&pItem->stamina,&pItem->power,
			&pItem->celerity,&pItem->wit,&pItem->physicAttack,&pItem->physicRecovery,&pItem->magicAttack,&pItem->magicRecovery,
			&pItem->attackSpeed,&pItem->heroHP,&pItem->injureRate,&pItem->captureRate,&pItem->beInjureRate,&pItem->beCaptureRate,
			&pItem->forceRate,&pItem->setID,&pItem->skill,&pItem->skillLevel,&pItem->quality,&pItem->inlayNums,&iSale,&pItem->price)<31)
		{
			delete pItem;
			pItem = 0;
			continue;
		}
		pItem->isSale = (iSale >0)?1:0;
		if (jobs[0] == '0')
		{
			memset(pItem->jobIDs,1,6);
		}
		else
		{
			jobCount = sscanf(jobs,"%d|%d|%d|%d|%d|%d",&jobIDs[0],&jobIDs[1],&jobIDs[2],&jobIDs[3],&jobIDs[4],&jobIDs[5]);
			for (int i=0;i<jobCount;i++)
			{
				pItem->jobIDs[jobIDs[i]] = 1;
			}
		}
		m_HeroEquipParamList[pItem->itemID] = pItem;
	}
	fclose(fp);
	return 0;
}
HeroEquipAttribute* CDataManageDlg::GetHeroEquipParam(int id)
{	
	HeroEquipAttribute_Map_Iterator iter = m_HeroEquipParamList.find(id);
	if(iter == m_HeroEquipParamList.end())
		return NULL;
	return iter->second;
}
int CDataManageDlg::ClearHeroEquipParam()
{
	HeroEquipAttribute_Map_Iterator iter;
	HeroEquipAttribute* pDelItem = 0;
	for(iter = m_HeroEquipParamList.begin();iter !=m_HeroEquipParamList.end();iter++)
	{
		if(pDelItem = iter->second)
		{
			delete pDelItem;
			pDelItem = 0;
		}
	}
	m_HeroEquipParamList.clear();
	return 0;
}
int CDataManageDlg::InitHeroEquipParamComboBox(CComboBox *pComboBox)
{
	pComboBox->Clear();
	HeroEquipAttribute_Map_Iterator iter;
	int index = 0;
	CString strTmp;
	for (iter = m_HeroEquipParamList.begin();iter != m_HeroEquipParamList.end();iter++)
	{
		strTmp.Format("%d_%s(%d)",iter->second->itemID,iter->second->name,iter->second->level);
		index = pComboBox->AddString(strTmp);
		pComboBox->SetItemData(index,iter->first);
	}
	return 0;
}
void CDataManageDlg::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	CDlgConsortia dlgConsortia;
	dlgConsortia.DoModal();
}

void CDataManageDlg::OnBnClickedButton6()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strUser,strPwd;
	m_edUser.GetWindowText(strUser);
	m_edPassword.GetWindowText(strPwd);
	if (strcmp(strUser,"Administrator")==0 && strcmp(strPwd,"xfy_datamanager")==0)
	{
		m_btnConnect.EnableWindow(TRUE);
		m_edPassword.SetWindowText("");
	}
	else
	{
		m_csInfo.SetWindowText("用户名或密码错误，请重新输入！");
		m_edUser.SetFocus();
	}
}
void CDataManageDlg::ProcHeroAttribute(PlayerOfficerTable &hero)
{//新表把基本属性分为三部分：初始、升级、装备
	/*
	Saber_OFFICER				=1, //圣骑士
	WARRIOR_OFFICER				=2 ,//战士
	SHOOTER_OFFICER				=3 ,//弓箭手
	MAGE_OFFICER				=4 ,//法师
	PRIEST_OFFICER				=5 ,//牧师
	HUNTER_OFFICER				=6 ,//猎人
	WARLOCK_OFFICER				=7 ,//术士
	Rogue_OFFICER				=8 ,//潜行者
	*/
	int totalPower = hero.power_ + hero.levelpower_ + hero.equippower_;//力量
	int totalStamina = hero.stamina_ + hero.levelstamina_ + hero.equipstamina_;//耐力
	int totalCelerity = hero.celerity_ + hero.levelcelerity_ + hero.equipcelerity_;//敏捷
	int totalWit = hero.wit_ + hero.levelwit_ + hero.equipwit_;//智力
	if(hero.jobid_ == Saber_OFFICER || hero.jobid_ == WARRIOR_OFFICER)
	{//战士系
		hero.attack_ += totalPower* 5;//物理攻击力
		hero.forcerate_ += totalPower/3000;//暴击率
		hero.physicrecovery_ += totalPower * 5;//物理防御
		hero.magicrecovery_ += totalWit * 5;//魔法防御
		hero.herohp_ += totalStamina;//HP
	}
	else if(hero.jobid_ == SHOOTER_OFFICER)
	{//弓箭手系
		hero.attack_ += totalCelerity * 5;//攻击力
		hero.forcerate_ += totalCelerity/3000;//暴击率
		hero.physicrecovery_ += totalPower * 5;//物理防御
		hero.magicrecovery_ += totalWit * 5;//魔法防御
		hero.herohp_ += totalStamina;//HP
	}
	else if(hero.jobid_ == MAGE_OFFICER)
	{//法师系
		hero.magicattack_ += totalWit * 5;//攻击力
		hero.forcerate_ += totalWit/3000;//暴击率
		hero.physicrecovery_ += totalPower * 5;//物理防御
		hero.magicrecovery_ += totalWit * 5;//魔法防御
		hero.herohp_ += totalStamina;//HP
	}
	else if(hero.jobid_ == PRIEST_OFFICER)
	{//牧师系
		hero.magicattack_ += totalWit * 5;//攻击力
		hero.forcerate_ += totalWit/3000;//暴击率
		hero.physicrecovery_ += totalPower * 5;//物理防御
		hero.magicrecovery_ += totalWit * 5;//魔法防御
		hero.herohp_ += totalStamina;//HP
	}
	else
	{
		hero.attack_ = 0;//攻击力
		hero.physicrecovery_ = 0;//物理防御
		hero.magicrecovery_ = 0;//魔法防御
	}
}
void CDataManageDlg::OnBnClickedButton7()
{
	// TODO: 在此添加控件通知处理程序代码
	FILE *fp = NULL;
	if ((fp = fopen("heroattr.csv","w")) == NULL)
	{
		m_csInfo.SetWindowText("打开文件heroattr.log失败！");
		return;
	}
	//特殊英雄的属性与其他英雄属性处理有点不同：初始属性没有加1级的职业属性值
	fprintf(fp,"玩家ID,英雄编号,英雄,级别,职业,原耐力,原力量,原敏捷,原智力,初耐力,初力量,初敏捷,初智力,升耐力,升力量,升敏捷,升智力,装耐力,装力量,装敏捷,装智力\n");
	PlayerOfficerTable offtb;
	PlayerOfficerTable *pOfficerList = 0;
	char *buf_officer = 0;
	int count_officer = 0;
	if (m_DBProxy.db_select_all(offtb,buf_officer,count_officer) != DB_Proxy::DB_SUCCEED)
	{
		m_csInfo.SetWindowText("查询英雄表失败！");
		fclose(fp);
		return;
	}
	count_officer = count_officer/sizeof(PlayerOfficerTable);
	pOfficerList = (PlayerOfficerTable*)buf_officer;
	struct JobParamEx
	{
		int heroraceid_; // heroraceid_ = id 一条
		int endurance_;
		int strength_;
		int arility_;
		int intelligence_;
	};
	JobParamEx *pJobParam = 0;
	int iErrorCount = 0;
	int iProCount = 0;
	int iLevel = 0;
	struct tagAttribute
	{
		int stamina_;
		int power_;
		int celerity_;
		int wit_;
	};
	tagAttribute tmpAttribute[3] = {0};
	PlayerItemStorageTable itemtb;
	PlayerItemStorageTable *pItemList = 0;
	char *buf_item = 0;
	int count_item = 0;
	EquipAttribute *pEquipAttribute = 0;
	CString strTmp;
	char szPlayerInfo[128] = {0};
	for (int i=0;i<count_officer;i++)
	{
		/*
		UpDataOfficer.stamina_ = JParam.endurance_/2;
		UpDataOfficer.power_ = JParam.strength_/2;
		UpDataOfficer.celerity_ = JParam.arility_/2;
		UpDataOfficer.wit_ = JParam.intelligence_/2;
		*/
		memset(&tmpAttribute,0,sizeof(tagAttribute) * 3);
		if (pOfficerList[i].playerid_ <10000)
		{
			continue;
		}
		iProCount++;
		//pJobParam = GetJobParam(pOfficerList[i].jobid_);
		if (pJobParam == NULL)
		{
			iErrorCount++;
			continue;
		}
		iLevel = pOfficerList[i].level_ - 1;
		tmpAttribute[1].stamina_ = (iLevel/2)*pJobParam->endurance_;
		if (iLevel%2 ==1)
		{
			tmpAttribute[1].stamina_ +=pJobParam->endurance_/2;
		}
		tmpAttribute[1].power_ = (iLevel/2)*pJobParam->strength_;
		if (iLevel%2 ==1)
		{
			tmpAttribute[1].power_ +=pJobParam->strength_/2;
		}
		tmpAttribute[1].celerity_ = (iLevel/2)*pJobParam->arility_;
		if (iLevel%2 ==1)
		{
			tmpAttribute[1].celerity_ +=pJobParam->arility_/2;
		}
		tmpAttribute[1].wit_ = (iLevel/2)*pJobParam->intelligence_;
		if (iLevel%2 ==1)
		{
			tmpAttribute[1].wit_ +=pJobParam->intelligence_/2;
		}
		itemtb.setmasterid(pOfficerList[i].officerid_,pOfficerList[i].playerid_);
		if (m_DBProxy.db_multi_select(itemtb,buf_item,count_item) == DB_Proxy::DB_SUCCEED)
		{
			count_item = count_item/sizeof(PlayerItemStorageTable);
			pItemList = (PlayerItemStorageTable*)buf_item;
			for (int j=0;j<count_item;j++)
			{
				if (pItemList[j].type_ != EN_HERO_BODY)
				{
					continue;
				}
				pEquipAttribute = GetEquipAttribute(pItemList[j].itemid_);
				if (pEquipAttribute == NULL)
				{
					continue;
				}
				tmpAttribute[2].stamina_ += pEquipAttribute->endurance_;
				tmpAttribute[2].power_ += pEquipAttribute->str_;
				tmpAttribute[2].celerity_ += pEquipAttribute->agi_;
				tmpAttribute[2].wit_ += pEquipAttribute->int_;
			}
			if (buf_item)
			{
				delete[] buf_item;
				buf_item = 0;
			}
			count_item = 0;
		}

		tmpAttribute[0].stamina_ = pOfficerList[i].stamina_ - tmpAttribute[1].stamina_ - tmpAttribute[2].stamina_;
		tmpAttribute[0].power_ = pOfficerList[i].power_ - tmpAttribute[1].power_ - tmpAttribute[2].power_;
		tmpAttribute[0].celerity_ = pOfficerList[i].celerity_ - tmpAttribute[1].celerity_ - tmpAttribute[2].celerity_;
		tmpAttribute[0].wit_ = pOfficerList[i].wit_ - tmpAttribute[1].wit_ - tmpAttribute[2].wit_;
		sprintf(szPlayerInfo,"%s(%d-%d:%d)",pOfficerList[i].officername_,pOfficerList[i].playerid_,pOfficerList[i].officerid_,pOfficerList[i].level_);
		
		fprintf(fp,"%d,%d,%s,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n",
			pOfficerList[i].playerid_,pOfficerList[i].officerid_,pOfficerList[i].officername_,pOfficerList[i].level_,pOfficerList[i].jobid_,
			pOfficerList[i].stamina_,pOfficerList[i].power_,pOfficerList[i].celerity_,pOfficerList[i].wit_,
			tmpAttribute[0].stamina_,tmpAttribute[0].power_,tmpAttribute[0].celerity_,tmpAttribute[0].wit_,
			tmpAttribute[1].stamina_,tmpAttribute[1].power_,tmpAttribute[1].celerity_,tmpAttribute[1].wit_,
			tmpAttribute[2].stamina_,tmpAttribute[2].power_,tmpAttribute[2].celerity_,tmpAttribute[2].wit_);

		pOfficerList[i].stamina_ = tmpAttribute[0].stamina_;
		pOfficerList[i].power_ = tmpAttribute[0].power_;
		pOfficerList[i].celerity_ = tmpAttribute[0].celerity_;
		pOfficerList[i].wit_ = tmpAttribute[0].wit_;
		pOfficerList[i].levelstamina_ = tmpAttribute[1].stamina_;
		pOfficerList[i].levelpower_ = tmpAttribute[1].power_;
		pOfficerList[i].levelcelerity_ = tmpAttribute[1].celerity_;
		pOfficerList[i].levelwit_ = tmpAttribute[1].wit_;
		pOfficerList[i].equipstamina_ = tmpAttribute[2].stamina_;
		pOfficerList[i].equippower_ = tmpAttribute[2].power_;
		pOfficerList[i].equipcelerity_ = tmpAttribute[2].celerity_;
		pOfficerList[i].equipwit_ = tmpAttribute[2].wit_;
		ProcHeroAttribute(pOfficerList[i]);
		m_DBProxy.db_update(pOfficerList[i],pOfficerList[i]);
	}
	if (buf_officer)
	{
		delete[] buf_officer;
		buf_officer = 0;
	}
	if (fp)
	{
		fclose(fp);
	}
	strTmp.Format("共处理%d条记录,其中无效职业%d",iProCount,iErrorCount);
	m_csInfo.SetWindowText(strTmp);
}
int CDataManageDlg::ReadTaskBFParam()
{
	FILE *fp = 0;
	fp = fopen("Task_BF.csv","r");
	if(fp == NULL)
		return 1;

	char buf[512]= {0};
	fgets(buf,512,fp);

	LPTaskBF_Item pNewItem = 0;
	while(!feof(fp))
	{
		fgets(buf,512,fp);
		pNewItem = new TaskBF_Item;
		if(sscanf(buf,"%d,%d,%d,%d,%d",&pNewItem->TaskID,&pNewItem->Food,&pNewItem->Wood,&pNewItem->Skin,&pNewItem->Iron) <5)
		{
			delete pNewItem;
			pNewItem = 0;
			continue;
		}
		m_TaskBFList[pNewItem->TaskID] = pNewItem;
	}
	fclose(fp);
	return 0;
}
LPTaskBF_Item CDataManageDlg::GetTaskBFParam(int taskid)
{
	TaskBF_Map_Iterator iter = m_TaskBFList.find(taskid);
	if (iter == m_TaskBFList.end())
	{
		return NULL;
	}
	return iter->second;
}
int CDataManageDlg::ClearTaskBFParam()
{
	TaskBF_Map_Iterator iter;
	LPTaskBF_Item pDelItem = 0;
	for (iter = m_TaskBFList.begin();iter != m_TaskBFList.end();iter++)
	{
		if (pDelItem = iter->second)
		{
			delete pDelItem;
			pDelItem = 0;
		}
	}
	m_TaskBFList.clear();
	return 0;
}
void CDataManageDlg::OnBnClickedButton8()
{
	// TODO: 在此添加控件通知处理程序代码
	PlayerChatRoomTable playertb;
	PlayerChatRoomTable *pPlayerList = 0;
	char *buf_player = 0;
	int count_player = 0;

	PlayerTask playertask;
	PlayerTask *pTaskList = 0;
	char *buf_task = 0;
	int count_task = 0;
	CString strTmp;
	if (m_DBProxy.db_select_all(playertb,buf_player,count_player) != DB_Proxy::DB_SUCCEED)
	{
		m_csInfo.SetWindowText("查询用户表失败");
		return ;
	}

	FILE *fp = NULL;
	if ((fp = fopen("taskbf_log.csv","w")) == NULL)
	{
		m_csInfo.SetWindowText("打开文件taskbf_log.log失败！");
		return;
	}
	PlayerResource oldres,updateres;
	PlayerCity playercity;
	count_player = count_player/sizeof(PlayerChatRoomTable);
	pPlayerList = (PlayerChatRoomTable*)buf_player;
	int iCount[3] = {0};
	LPTaskBF_Item pTaskBF = 0;
	int iTmpCount[4] = {0,0,0,0};
	for (int i=0;i<count_player;i++)
	{
		//查询玩家的新手村
		playercity.setmasterid(pPlayerList[i].playerid_);
		if (pPlayerList[i].country_ ==1)
		{
			playercity.setdetailid(1003);
		}
		else if (pPlayerList[i].country_ == 2)
		{
			playercity.setdetailid(1007);
		}
		if (m_DBProxy.db_select(playercity) != DB_Proxy::DB_SUCCEED)
		{
			continue;
		}
		oldres.setmasterid(playercity.cityid_,playercity.mapid_);
		oldres.setdetailid(playercity.playerid_);
		if (m_DBProxy.db_select(oldres) != DB_Proxy::DB_SUCCEED)
		{
			continue;
		}
		fprintf(fp,"%d,%d,%d,%d,%d,%d\n",oldres.playerid_,oldres.content_,oldres.foodcount_,oldres.woodcount_,oldres.skincount_,oldres.ironcount_);//记录原始数据
		if (oldres.content_ <0)
		{
			oldres.content_  = 0;
		}
		playertask.setmasterid(pPlayerList[i].playerid_);

		memset(&updateres,0,sizeof(PlayerResource));
		updateres.setmasterid(playercity.cityid_,playercity.mapid_);
		updateres.setdetailid(playercity.playerid_);
		if (m_DBProxy.db_multi_select(playertask,buf_task,count_task) == DB_Proxy::DB_SUCCEED)
		{
			count_task = count_task/sizeof(PlayerTask);
			pTaskList = (PlayerTask*)buf_task;

			for(int j=0;j<count_task;j++)
			{
				if (pTaskList[j].state_ != 4)
				{//未提交不处理
					continue;
				}
				pTaskBF = GetTaskBFParam(pTaskList[j].taskid_);
				if (pTaskBF == NULL)
				{
					continue;
				}
				iTmpCount[0] = (pTaskBF->Food > oldres.content_)?oldres.content_:pTaskBF->Food;
				updateres.foodcount_ += iTmpCount[0];
				oldres.content_ -= iTmpCount[0];
				updateres.content_ -= iTmpCount[0];

				iTmpCount[1] = (pTaskBF->Wood > oldres.content_)?oldres.content_:pTaskBF->Wood;
				updateres.woodcount_ += iTmpCount[1];
				oldres.content_ -= iTmpCount[1];
				updateres.content_ -= iTmpCount[1];

				iTmpCount[2] = (pTaskBF->Skin > oldres.content_)?oldres.content_:pTaskBF->Skin;
				updateres.skincount_ += iTmpCount[2];
				oldres.content_ -= iTmpCount[2];
				updateres.content_ -= iTmpCount[2];

				iTmpCount[3] = (pTaskBF->Iron > oldres.content_)?oldres.content_:pTaskBF->Iron;
				updateres.ironcount_ += iTmpCount[3] ;
				oldres.content_ -= iTmpCount[3] ;
				updateres.content_ -= iTmpCount[3];
				fprintf(fp,",%d,%d,%d,%d,%d\n",pTaskList[j].taskid_,iTmpCount[0],iTmpCount[1],iTmpCount[2],iTmpCount[3]);

			}

			if (buf_task)
			{
				delete[] buf_task;
				buf_task =0;
			}
			count_task = 0;

		}
		if (updateres.foodcount_ ==0 && updateres.woodcount_ ==0 && updateres.skincount_ ==0 && updateres.ironcount_ ==0)
		{
			iCount[1]++;
			continue;
		}
		if (m_DBProxy.db_increase(updateres) ==DB_Proxy::DB_SUCCEED)
		{
			iCount[0]++;
			fprintf(fp,"补发后,%d,%d,%d,%d,%d\n",updateres.content_,updateres.foodcount_,updateres.woodcount_,updateres.skincount_,updateres.ironcount_);
		}
		else
		{
			iCount[2]++;
			fprintf(fp,"补发失败,%d,%d,%d,%d,%d\n",updateres.content_,updateres.foodcount_,updateres.woodcount_,updateres.skincount_,updateres.ironcount_);
		}
	}
	if (buf_player)
	{
		delete[] buf_player;
		buf_player = 0;
	}
	strTmp.Format("共对%d玩家进行了任务奖励补发,不需要补发的玩家:%d,补发失败%d",iCount[0],iCount[1],iCount[2]);
	fprintf(fp,strTmp);
	m_csInfo.SetWindowText(strTmp);
	if (fp)
	{
		fclose(fp);
	}
}
//读取技能描述表
void CDataManageDlg::readSkillDetail(void){
	FILE *fp = 0;
	fp = fopen("skilldetail.csv","r");
	if(fp == NULL)
	{
		printf("skilldetail.csv失败。\n");
		return ;
	}

	char readBuffer[1024]= {0};
	fgets(readBuffer,1024,fp);

	SkillDetail *skillDetailData = 0;

	char skillName[50];
	char occupationLimit[50];
	char selfOccupationLimit[50];
	char targetOccupationLimit[50];
	int occupations[6] = {0};
	int readNum;
	int iscanLearn;
	while(!feof(fp))
	{
		if(fgets(readBuffer,1024,fp) == NULL)
			break;
		skillDetailData = new SkillDetail;
		if(sscanf(readBuffer,"%d,%[^,],%d,%d,%[^,],%d,%d,%d,%d,%d,%f,%[^,],%[^,],%d",
			&skillDetailData->skillID,skillName,&skillDetailData->skillType,&iscanLearn,occupationLimit,
			&skillDetailData->levelLimit,&skillDetailData->level,&skillDetailData->consumeSkillDot,
			&skillDetailData->odds,&skillDetailData->effectType,&skillDetailData->effectValue,selfOccupationLimit,
			targetOccupationLimit,&skillDetailData->landLimit)<14)
		{
			delete skillDetailData;
			skillDetailData = NULL;
			continue;
		}
		if (iscanLearn > 0){
			skillDetailData->iscanLearn = true;
		}
		readNum = sscanf(occupationLimit,"%d|%d|%d|%d|%d|%d",&occupations[0],&occupations[1],
			&occupations[2],&occupations[3],&occupations[4],&occupations[5]);

		for (int i=0;i<readNum;i++){
			skillDetailData->occupationLimit.insert(std::set<OFFICER_OCCUPATION_TYPE>::value_type(occupations[i]));
		}
		readNum = sscanf(selfOccupationLimit,"%d|%d|%d|%d|%d|%d",&occupations[0],&occupations[1],
			&occupations[2],&occupations[3],&occupations[4],&occupations[5]);
		for (int i=0;i<readNum;i++){
			skillDetailData->selfOccupationLimit.insert(std::set<SOLDIER_TYPE>::value_type(occupations[i]));
		}
		readNum = sscanf(targetOccupationLimit,"%d|%d|%d|%d|%d|%d",&occupations[0],&occupations[1],
			&occupations[2],&occupations[3],&occupations[4],&occupations[5]);

		for (int i=0;i<readNum;i++){
			skillDetailData->targetOccupationLimit.insert(std::set<SOLDIER_TYPE>::value_type(occupations[i]));
		}
		skillDetailMap_[skillDetailData->skillID][skillDetailData->level] = skillDetailData;
	}
	fclose(fp);
}
//释放技能描述表
void CDataManageDlg::clearSkillDetail(void){
	SkillDetail_Map_Iterator iter;
	std::map<unsigned int ,  SkillDetail*>::iterator subIter;
	for (iter = skillDetailMap_.begin(); iter != skillDetailMap_.end() ; ++iter){
		for(subIter = iter->second.begin(); subIter!= iter->second.end() ; ++subIter){
			delete subIter->second;
		}
	}
	skillDetailMap_.clear();
}
SkillDetail * CDataManageDlg::getSkillDetail(SKILL_TYPE skillID , int skillLevel) {
	SkillDetail_Map_Iterator iter;
	std::map<unsigned int ,  SkillDetail*>::iterator subIter;
	iter = skillDetailMap_.find(skillID);
	if (iter == skillDetailMap_.end()){
		return NULL;
	}
	subIter = iter->second.find(skillLevel);
	if (subIter == iter->second.end()){
		return NULL;
	}
	return subIter->second;
}
PackStoreParam *CDataManageDlg::GetPackStoreParam(int postion)
{
	PackStoreParam_Map_Iterator iter = m_PackStoreParamMap.find(postion);
	if (iter != m_PackStoreParamMap.end())
	{
		return iter->second;
	}
	return NULL;
}
int CDataManageDlg::ReadPackStoreParam()
{
	FILE *fp = 0;
	char buf[1024] = {0};
	fp = fopen("PackStoreParam.csv","r");
	if(fp == NULL)
	{
		printf("打开PackStoreParam.csv失败。\n");
		return 1;
	}

	fgets(buf,1024,fp);
	PackStoreParam* pNewNode = 0;
	while(!feof(fp))
	{
		if(fgets(buf,1024,fp) == NULL)
			break;
		pNewNode = new PackStoreParam;
		if(sscanf(buf,"%d,%d",&pNewNode->Postion,&pNewNode->IsOverlap) <2)
		{
			delete pNewNode;
			pNewNode = 0;
			continue;
		}
		m_PackStoreParamMap[pNewNode->Postion] = pNewNode;
	}
	fclose(fp);
	return 0;
}
void CDataManageDlg::ClearPackStoreParam()
{
	//背包是否重叠参数
	PackStoreParam* pPackStoreParam = 0;
	PackStoreParam_Map_Iterator packiter;
	for (packiter = m_PackStoreParamMap.begin();packiter != m_PackStoreParamMap.end();packiter++)
	{
		pPackStoreParam = packiter->second;
		if (pPackStoreParam)
		{
			delete pPackStoreParam;
			pPackStoreParam =0;
		}
	}
	m_PackStoreParamMap.clear();
}
//读取建筑配置表
void CDataManageDlg::loadConstructionConfigure()
{
	FILE *fp = 0;
	fp = fopen("ConstructionConfigure.csv","r");
	if(fp == NULL)
	{
		printf("打开ConstructionConfigure.csv失败。\n");
		return ;
	}

	char buf[1024]= {0};
	fgets(buf,1024,fp);

	ConstructionConfigure* pItem = 0;
	//m_ConstructionConfigureMultimap
	while(!feof(fp))
	{
		if(fgets(buf,1024,fp) == NULL)
			break;
		pItem = new ConstructionConfigure;
		if(sscanf(buf,"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
			&pItem->constructionid_,&pItem->level_,&pItem->completetimes_,&pItem->money_,&pItem->foodcount_,
			&pItem->woodcount_,&pItem->ironcount_,&pItem->skincount_,&pItem->science_,&pItem->modify_)<10)
		{
			delete pItem;
			pItem = 0;
			continue;
		}
		m_ConstructionConfigureMultimap.insert(ConstructionConfigure_MultiMap_Pair(pItem->constructionid_,pItem));
	}
	fclose(fp);
}
void CDataManageDlg::clearConstructionConfigure()
{
	ConstructionConfigure_Multimap_Iterator iter;
	ConstructionConfigure *pDelItem = 0;
	for (iter = m_ConstructionConfigureMultimap.begin();iter != m_ConstructionConfigureMultimap.end();iter++)
	{
		if ((pDelItem = iter->second))
		{
			delete pDelItem;
			pDelItem = 0;
		}
	}
	m_ConstructionConfigureMultimap.clear();
}
ConstructionConfigure* CDataManageDlg::getConstructionConfigure(int iConID, int iLevel)
{
	ConstructionConfigure* pConstrParam = NULL;
	ConstructionConfigure_Multimap_Iterator lowIter,upperIter;
	lowIter = m_ConstructionConfigureMultimap.lower_bound(iConID);
	upperIter = m_ConstructionConfigureMultimap.upper_bound(iConID);

	for(; lowIter != upperIter; lowIter++)
	{
		if (lowIter->second->level_ == iLevel)
		{
			pConstrParam = lowIter->second;
			break;
		}
	}

	return pConstrParam;
}
void CDataManageDlg::heroUpgrade(int level,PlayerOfficerTable *officer,PlayerOfficerTable *upofficer)
{//成长基本值为10，一般属性+1/每级
	float fTmpAttri = (level * 10 * officer->grouprate_ + officer->lsttotalattri_) * 0.01f;//剩余主属性+耐力之和

	int iTotalAttri = (int)fTmpAttri;//本次升级，可分配的点数
	fTmpAttri = fTmpAttri - iTotalAttri + 0.000001f;
	iTotalAttri -= 2 * level;
	if (iTotalAttri <=0)
	{
		return;
	}
	float fStamina = 0.0;
	float fMainAttri = 0.0;
	int iStamina = 0;
	int iMainAttri = 0;
	fStamina = iTotalAttri * 0.4f;//耐力
	fMainAttri = (float)iTotalAttri - fStamina;

	fStamina += officer->lststamina_ * 0.01f + 0.000001f ;
	fMainAttri += officer->lstmainattri_ * 0.01f + 0.000001f;
	iStamina = (int)fStamina;
	iMainAttri = (int)fMainAttri;
	fStamina = fStamina - iStamina + 0.000001f;
	fMainAttri = fMainAttri - iMainAttri + 0.000001f;
	if(officer->jobid_ == Saber_OFFICER || officer->jobid_ == WARRIOR_OFFICER)
	{//战士系
		upofficer->levelcelerity_ += level;
		upofficer->levelwit_ += level;
		upofficer->levelpower_ += iMainAttri;
	}
	else if(officer->jobid_ == SHOOTER_OFFICER)
	{//弓箭手系
		upofficer->levelcelerity_ += iMainAttri;
		upofficer->levelwit_ += level;
		upofficer->levelpower_ += level;
	}
	else if(officer->jobid_ == MAGE_OFFICER)
	{//法师系
		upofficer->levelcelerity_ += level;
		upofficer->levelwit_ += iMainAttri;
		upofficer->levelpower_ += level;
	}
	else if(officer->jobid_ == PRIEST_OFFICER)
	{//牧师系
		upofficer->levelcelerity_ += level;
		upofficer->levelwit_ += iMainAttri;
		upofficer->levelpower_ += level;
	}
	else
	{
	}
	upofficer->levelstamina_ += iStamina;
	fTmpAttri = fTmpAttri * 100;
	fStamina = fStamina * 100;
	fMainAttri = fMainAttri * 100;
	upofficer->lsttotalattri_ += (int)fTmpAttri - officer->lsttotalattri_;
	upofficer->lststamina_ += (int)fStamina - officer->lststamina_;
	upofficer->lstmainattri_ += (int)fMainAttri - officer->lstmainattri_;
}
