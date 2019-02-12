// DlgConsortia.cpp : 实现文件
//

#include "stdafx.h"
#include "DataManage.h"
#include "DlgConsortia.h"
#include ".\dlgconsortia.h"
#include ".\datamanagedlg.h"
#include <string.h>


// CDlgConsortia 对话框

IMPLEMENT_DYNAMIC(CDlgConsortia, CDialog)
CDlgConsortia::CDlgConsortia(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgConsortia::IDD, pParent)
{
}

CDlgConsortia::~CDlgConsortia()
{
}

void CDlgConsortia::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON1, m_btnModify);
	DDX_Control(pDX, IDC_LIST1, m_lstConsortia);
	DDX_Control(pDX, IDC_EDIT1, m_edID);
	DDX_Control(pDX, IDC_EDIT2, m_edName);
	DDX_Control(pDX, IDC_BUTTON7, m_btnModifyPlacard);
	DDX_Control(pDX, IDC_EDIT3, m_edPlacard);
	DDX_Control(pDX, IDC_EDIT4, m_edTenet);
	DDX_Control(pDX, IDC_STATIC_HINT, m_csInfo);
}


BEGIN_MESSAGE_MAP(CDlgConsortia, CDialog)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, OnNMClickList1)
	ON_BN_CLICKED(ID_QUERYCONSORTIA, OnBnClickedQueryconsortia)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BUTTON7, OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
END_MESSAGE_MAP()


// CDlgConsortia 消息处理程序

void CDlgConsortia::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	NMLISTVIEW*   pList=   (NMLISTVIEW*)pNMHDR; 
	/* 
	m_iCurItem = pList->iItem;
	
	if(pList->iItem >=0)
	{
		m_btnModify.EnableWindow(TRUE);
	}
	else
	{
		m_btnModify.EnableWindow(FALSE);
	}
	*/
	ResetConsortiaInfo(pList->iItem);
	*pResult = 0;
}
void CDlgConsortia::ResetConsortiaInfo(int item)
{
	if (item == m_iCurItem)
	{
		return;
	}
	m_iCurItem = item;
	if (m_iCurItem <0)
	{
		m_btnModify.EnableWindow(FALSE);
		m_btnModifyPlacard.EnableWindow(FALSE);
		m_edPlacard.EnableWindow(FALSE);
		m_edTenet.EnableWindow(FALSE);
		m_edID.SetWindowText("");
		m_edName.SetWindowText("");
		m_edPlacard.SetWindowText("");
		m_edTenet.SetWindowText("");
	}
	else
	{
		CString strTmp;
		strTmp = m_lstConsortia.GetItemText(m_iCurItem,0);
		m_edID.SetWindowText(strTmp);
		strTmp = m_lstConsortia.GetItemText(m_iCurItem,1);
		m_edName.SetWindowText(strTmp);
		strTmp = m_lstConsortia.GetItemText(m_iCurItem,3);
		m_edPlacard.SetWindowText(strTmp);
		strTmp = m_lstConsortia.GetItemText(m_iCurItem,4);
		m_edTenet.SetWindowText(strTmp);
		m_btnModify.EnableWindow(TRUE);
		m_btnModifyPlacard.EnableWindow(TRUE);
		m_edPlacard.EnableWindow(TRUE);
		m_edTenet.EnableWindow(TRUE);
	}
}

void CDlgConsortia::OnBnClickedQueryconsortia()
{
	// TODO: 在此添加控件通知处理程序代码
	CDataManageDlg *pMainDlg = (CDataManageDlg*)AfxGetMainWnd();
	ConsortiaInfo contb;
	char *buf = 0;
	int count = 0;
	m_csInfo.SetWindowText("");
	if (pMainDlg->m_DBProxy.db_select_all(contb,buf,count) != DB_Proxy::DB_SUCCEED)
	{
		m_csInfo.SetWindowText("ConsortiaInfo表为空");
		return;
	}
	m_lstConsortia.DeleteAllItems();
	ConsortiaInfo *pConsList = (ConsortiaInfo*)buf;
	count = count/sizeof(ConsortiaInfo);
	int index = 0;
	CString strTmp;
	for (int i=0;i<count;i++)
	{
		strTmp.Format("%d",pConsList[i].consortiaid_);
		index = m_lstConsortia.InsertItem(i,strTmp);
		strTmp.Format("%s",pConsList[i].name_);
		m_lstConsortia.SetItemText(index,1,strTmp);
		strTmp.Format("%d",pConsList[i].chairmanid_);
		m_lstConsortia.SetItemText(index,2,strTmp);
		strTmp.Format("%s",pConsList[i].placard_);
		m_lstConsortia.SetItemText(index,3,strTmp);

		strTmp.Format("%s",pConsList[i].tenet_);
		m_lstConsortia.SetItemText(index,4,strTmp);
	}
	if (buf)
	{
		delete[] buf;
		buf = 0;
	}
	strTmp.Format("公会数量：%d",count);
	m_csInfo.SetWindowText(strTmp);
	ResetConsortiaInfo(-1);
}

void CDlgConsortia::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// TODO: 在此处添加消息处理程序代码
	int index = 0;
	m_lstConsortia.InsertColumn(index++,"编号",LVCFMT_LEFT,60);
	m_lstConsortia.InsertColumn(index++,"名称",LVCFMT_LEFT,60);
	m_lstConsortia.InsertColumn(index++,"会长",LVCFMT_LEFT,60);
	m_lstConsortia.InsertColumn(index++,"公告",LVCFMT_LEFT,120);
	m_lstConsortia.InsertColumn(index++,"宣言",LVCFMT_LEFT,120);

	m_lstConsortia.SetExtendedStyle(LVS_EX_FULLROWSELECT |LVS_EX_GRIDLINES );
	m_btnModify.EnableWindow(FALSE);
	m_btnModifyPlacard.EnableWindow(FALSE);
	m_edPlacard.EnableWindow(FALSE);
	m_edTenet.EnableWindow(FALSE);
}

void CDlgConsortia::OnBnClickedButton7()
{
	// TODO: 在此添加控件通知处理程序代码
	CDataManageDlg *pMainDlg = (CDataManageDlg*)AfxGetMainWnd();
	CString strTmp;
	int iConsID;
	m_edID.GetWindowText(strTmp);
	iConsID = atoi(strTmp);
	ConsortiaInfo consinfo;
	consinfo.setid(iConsID);
	if (pMainDlg->m_DBProxy.db_select(consinfo) != DB_Proxy::DB_SUCCEED)
	{
		m_csInfo.SetWindowText("公会不存在");
		return;
	}
	m_edPlacard.GetWindowText(strTmp);
	strset(consinfo.placard_,0);
	strncpy(consinfo.placard_,strTmp,126);
	if (pMainDlg->m_DBProxy.db_update(consinfo,consinfo) != DB_Proxy::DB_SUCCEED)
	{
		m_csInfo.SetWindowText("更新公会公告信息失败");
	}
	else
	{
		m_csInfo.SetWindowText("更新公会公告信息成功");
	}
}

void CDlgConsortia::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CDataManageDlg *pMainDlg = (CDataManageDlg*)AfxGetMainWnd();
	CString strTmp;
	int iConsID;
	m_edID.GetWindowText(strTmp);
	iConsID = atoi(strTmp);
	ConsortiaInfo consinfo;
	consinfo.setid(iConsID);
	if (pMainDlg->m_DBProxy.db_select(consinfo) != DB_Proxy::DB_SUCCEED)
	{
		m_csInfo.SetWindowText("公会不存在");
		return;
	}
	m_edTenet.GetWindowText(strTmp);
	strset(consinfo.tenet_,0);
	strncpy(consinfo.tenet_,strTmp,254);
	if (pMainDlg->m_DBProxy.db_update(consinfo,consinfo) != DB_Proxy::DB_SUCCEED)
	{
		m_csInfo.SetWindowText("更新公会宣言信息失败");
	}
	else
	{
		m_csInfo.SetWindowText("更新公会宣言信息成功");
	}
}
