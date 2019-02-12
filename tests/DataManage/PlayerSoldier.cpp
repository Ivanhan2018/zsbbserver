// PlayerSoldier.cpp : 实现文件
//

#include "stdafx.h"
#include "DataManage.h"
#include "PlayerSoldier.h"
#include ".\playersoldier.h"
#include "../include/DB_Proxy.h"
#include "../include/DB_Table.h"
#include ".\datamanagedlg.h"


// CPlayerSoldier 对话框

IMPLEMENT_DYNCREATE(CPlayerSoldier, CDialog)

CPlayerSoldier::CPlayerSoldier(CWnd* pParent /*=NULL*/)
	: CDialog(CPlayerSoldier::IDD, pParent)
{
}

CPlayerSoldier::~CPlayerSoldier()
{
}

void CPlayerSoldier::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_HUMAN, m_edHuman);
	DDX_Control(pDX, IDC_EDIT_MANNIKIN, m_edManikin);
	DDX_Control(pDX, IDC_EDIT_FAIRY, m_edFairy);
	DDX_Control(pDX, IDC_EDIT_DANDIPRAT, m_edDandiprat);
	DDX_Control(pDX, IDC_EDIT_HALFMAN, m_edHalfMan);
	DDX_Control(pDX, IDC_EDIT_CATTLE, m_edCattle);
	DDX_Control(pDX, IDC_EDIT_BLACKFAIRY, m_edBlackFairy);
	DDX_Control(pDX, IDC_EDIT_DECPERSION, m_edDecPerson);
	DDX_Control(pDX, IDC_STATIC_HINT, m_stHint);
}


BEGIN_MESSAGE_MAP(CPlayerSoldier, CDialog)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(ID_ADDSOLDIER, OnBnClickedAddsoldier)
END_MESSAGE_MAP()


void CPlayerSoldier::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// TODO: 在此处添加消息处理程序代码
	m_edHuman.SetWindowText("0");
	m_edManikin.SetWindowText("0");
	m_edFairy.SetWindowText("0");
	m_edDandiprat.SetWindowText("0");
	m_edHalfMan.SetWindowText("0");
	m_edCattle.SetWindowText("0");
	m_edBlackFairy.SetWindowText("0");
	m_edDecPerson.SetWindowText("0");
}

void CPlayerSoldier::OnBnClickedAddsoldier()
{
	// TODO: 在此添加控件通知处理程序代码
	
	CDataManageDlg *pMainDlg = (CDataManageDlg*)AfxGetMainWnd();
	CString strPlayerID,strMapID,strCityID;
	CString strHuman,strMannikin,strFairy,strDandiprat,strHalfAnimal,strCattle,strBlackFairy,strDecPesrson;
	int iPlayerID,iMapID,iCityID;
	int iHuman,iMannikin,iFairy,iDandiprat,iHalfAnimal,iCattle,iBlackFairy,iDecPesrson;
	__int64 oldtm;
	pMainDlg->m_edPlayerID.GetWindowText(strPlayerID);
	pMainDlg->m_cbMapList.GetWindowText(strMapID);
	pMainDlg->m_edCityID.GetWindowText(strCityID);
	m_edHuman.GetWindowText(strHuman);
	m_edManikin.GetWindowText(strMannikin);
	m_edFairy.GetWindowText(strFairy);
	m_edDandiprat.GetWindowText(strDandiprat);
	m_edHalfMan.GetWindowText(strHalfAnimal);
	m_edCattle.GetWindowText(strCattle);
	m_edBlackFairy.GetWindowText(strBlackFairy);
	m_edDecPerson.GetWindowText(strDecPesrson);

	iPlayerID = atoi(strPlayerID);
	iMapID = atoi(strMapID);
	iCityID = atoi(strCityID);

	iHuman = atoi(strHuman);
	iMannikin = atoi(strMannikin);
	iFairy = atoi(strFairy);
	iDandiprat = atoi(strDandiprat);
	iHalfAnimal = atoi(strHalfAnimal);
	iCattle = atoi(strCattle);
	iBlackFairy = atoi(strBlackFairy);
	iDecPesrson = atoi(strDecPesrson);
	m_stHint.SetWindowText("");
	if(iHuman ==0 && iMannikin ==0 && iFairy ==0 && iDandiprat ==0
		&&iHalfAnimal == 0 && iCattle ==0 && iBlackFairy ==0 && iDecPesrson ==0)
	{
		m_stHint.SetWindowText("所有兵种数量为0，请重新输入.");
		return;
	}
	
	if(iHuman >1000 || iMannikin >1000 || iFairy >1000 || iDandiprat >1000 ||
		iHalfAnimal >1000 || iCattle >1000 || iBlackFairy>1000 || iDecPesrson >1000)
	{
		m_stHint.SetWindowText("所有兵种数量为最多为1000，请重新输入.");
		return;
	}

	PlayerSoldierRace racetb;
	racetb.setmasterid(iMapID,iCityID);
	racetb.setdetailid(iPlayerID);
	int iRet = pMainDlg->m_DBProxy.db_select(racetb);
	if(iRet == DB_Proxy::DB_FAILED)
	{
		m_stHint.SetWindowText("查询DBG失败，请重试.");
		return;
	}
	oldtm = racetb.completetimes_;
	memset(&racetb,0,sizeof(racetb));
	racetb.setmasterid(iMapID,iCityID);
	racetb.setdetailid(iPlayerID);
	racetb.human_ = iHuman;
	racetb.mannikin_ = iMannikin;
	racetb.fairy_ = iFairy;
	racetb.dandiprat_ = iDandiprat;
	racetb.halfanimal_ = iHalfAnimal;
	racetb.cattle_ = iCattle;
	racetb.blackfairy_ = iBlackFairy ;
	racetb.decpesrson_ = iDecPesrson;
	time_t curtm;
	time(&curtm);
	if(iRet == DB_Proxy::DB_EOF)
	{
		racetb.completetimes_ = curtm;
		iRet = pMainDlg->m_DBProxy.db_insert(racetb);
	}
	else
	{
		racetb.completetimes_ = curtm - oldtm;
		iRet = pMainDlg->m_DBProxy.db_increase(racetb);
	}
	if(iRet == DB_Proxy::DB_SUCCEED)
	{
		m_stHint.SetWindowText("添加兵种数量成功.");
	}
	else
	{
		m_stHint.SetWindowText("添加兵种数量失败.");
	}
}
