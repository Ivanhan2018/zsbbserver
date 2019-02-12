// PlayerResource.cpp : 实现文件
//

#include "stdafx.h"
#include "DataManage.h"
#include "PlayerResource.h"
#include ".\datamanagedlg.h"
#include "../include/DB_Proxy.h"
#include "../include/DB_Table.h"
#include ".\playerresource.h"
#include <time.h>

// CPlayerResource 对话框

IMPLEMENT_DYNAMIC(CPlayerResource, CDialog)
CPlayerResource::CPlayerResource(CWnd* pParent /*=NULL*/)
	: CDialog(CPlayerResource::IDD, pParent)
{
}

CPlayerResource::~CPlayerResource()
{
}

void CPlayerResource::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_FOOD, m_edFood);
	DDX_Control(pDX, IDC_EDIT_WOOD, m_edWood);
	DDX_Control(pDX, IDC_EDIT_SKIN, m_edSkin);
	DDX_Control(pDX, IDC_EDIT_IRON, m_edIron);
	DDX_Control(pDX, IDC_EDIT_REDSTONE, m_edRedStone);
	DDX_Control(pDX, IDC_EDIT_YELLOWSTONE, m_edYellowStone);
	DDX_Control(pDX, IDC_EDIT_GREENSTONE, m_edGreenStone);
	DDX_Control(pDX, IDC_EDIT_BLACKIRON, m_edBlackIron);
	DDX_Control(pDX, IDC_STATIC_HINT, m_stHint);
}


BEGIN_MESSAGE_MAP(CPlayerResource, CDialog)
	ON_BN_CLICKED(ID_ADDRESOURCE, OnBnClickedAddresource)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CPlayerResource 消息处理程序

void CPlayerResource::OnBnClickedAddresource()
{
	// TODO: 在此添加控件通知处理程序代码
	CDataManageDlg *pMainDlg = (CDataManageDlg*)AfxGetMainWnd();
	CString strPlayerID,strMapID,strCityID;
	CString strFoodCount,strWoodCount,strSkinCount,strIronCount;
	CString strRedStoneCount,strYellowStoneCount,strGreenStoneCount,strBlackIronCount;
	int iFoodCount,iWoodCount,iSkinCount,iIronCount;
	int iRedStoneCount,iYellowStoneCount,iGreenStoneCount,iBlackIronCount;
	bool bTime = false;
	pMainDlg->m_edPlayerID.GetWindowText(strPlayerID);
	pMainDlg->m_cbMapList.GetWindowText(strMapID);
	pMainDlg->m_edCityID.GetWindowText(strCityID);

	m_edFood.GetWindowText(strFoodCount);
	m_edWood.GetWindowText(strWoodCount);
	m_edSkin.GetWindowText(strSkinCount);
	m_edIron.GetWindowText(strIronCount);
	m_edRedStone.GetWindowText(strRedStoneCount);
	m_edYellowStone.GetWindowText(strYellowStoneCount);
	m_edGreenStone.GetWindowText(strGreenStoneCount);
	m_edBlackIron.GetWindowText(strBlackIronCount);
	iFoodCount = atoi(strFoodCount);
	iWoodCount = atoi(strWoodCount);
	iSkinCount = atoi(strSkinCount);
	iIronCount = atoi(strIronCount);
	iRedStoneCount = atoi(strRedStoneCount);
	iYellowStoneCount = atoi(strYellowStoneCount);
	iGreenStoneCount = atoi(strGreenStoneCount);
	iBlackIronCount = atoi(strBlackIronCount);
	PlayerResource restb;
	restb.setmasterid(atoi(strCityID),atoi(strMapID));
	restb.setdetailid(atoi(strPlayerID));
	m_stHint.SetWindowText("");
	if(pMainDlg->m_DBProxy.db_select(restb) != DB_Proxy::DB_SUCCEED)
	{
		//MessageBox("select PlayerResource table fault.");
		m_stHint.SetWindowText("仓库容量不够.");
		return;
	}
	if(restb.completetimes_ ==0)
	{
		bTime = true;
	}
	int iTotalCount = iFoodCount + iWoodCount + iSkinCount + iIronCount;
	iTotalCount += iRedStoneCount + iYellowStoneCount + iGreenStoneCount + iBlackIronCount;
	if(restb.content_ < iTotalCount)
	{
		//MessageBox("仓库容量不够.");
		m_stHint.SetWindowText("仓库容量不够.");
		return;
	}
	if(iFoodCount ==0 && iWoodCount ==0 && iSkinCount ==0 && iIronCount ==0
		&&iRedStoneCount == 0 && iYellowStoneCount ==0 && iGreenStoneCount ==0 && iBlackIronCount ==0)
	{
		m_stHint.SetWindowText("所有资源数量为0，请重新输入.");
		return;
	}
	memset(&restb,0,sizeof(restb));
	restb.setmasterid(atoi(strCityID),atoi(strMapID));
	restb.setdetailid(atoi(strPlayerID));
	restb.foodcount_ = iFoodCount;
	restb.woodcount_ = iWoodCount;
	restb.skincount_ = iSkinCount;
	restb.ironcount_ = iIronCount;
	restb.redstonecount_ = iRedStoneCount;
	restb.yollowstonecount_ = iYellowStoneCount;
	restb.greenstonecount_ = iGreenStoneCount;
	restb.backironcount_ = iBlackIronCount;
	restb.content_ = - iTotalCount;
	if(bTime)
	{
		time_t curtm;
		time(&curtm);
		restb.completetimes_ = curtm;
	}
	if(pMainDlg->m_DBProxy.db_increase(restb) == DB_Proxy::DB_SUCCEED)
	{
		m_stHint.SetWindowText("添加资源成功.");
	}
	else
	{
		m_stHint.SetWindowText("添加资源失败.");
	}
}

void CPlayerResource::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// TODO: 在此处添加消息处理程序代码
	m_edFood.SetWindowText("0");
	m_edWood.SetWindowText("0");
	m_edSkin.SetWindowText("0");
	m_edIron.SetWindowText("0");
	m_edRedStone.SetWindowText("0");
	m_edYellowStone.SetWindowText("0");
	m_edGreenStone.SetWindowText("0");
	m_edBlackIron.SetWindowText("0");
}
