// DlgProgressBar.cpp : 实现文件
//

#include "stdafx.h"
#include "LogExportTool.h"
#include "DlgProgressBar.h"


// CDlgProgressBar 对话框

IMPLEMENT_DYNAMIC(CDlgProgressBar, CDialog)

CDlgProgressBar::CDlgProgressBar(bool &isCancel, bool isUserInLoadDownLog,CWnd* pParent /*=NULL*/)
: CDialog(CDlgProgressBar::IDD, pParent), m_isCancel(isCancel), m_isUserInLoadDownLog(isUserInLoadDownLog)
{

}

CDlgProgressBar::~CDlgProgressBar()
{
}

void CDlgProgressBar::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS1, m_progressCtl);
}


BEGIN_MESSAGE_MAP(CDlgProgressBar, CDialog)
	ON_BN_CLICKED(IDC_BTN_CANCEL, &CDlgProgressBar::OnBnClickedBtnCancel)
	ON_WM_NCDESTROY()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CDlgProgressBar 消息处理程序

void CDlgProgressBar::OnBnClickedBtnCancel()
{
	m_isCancel = true;
	CDialog::OnCancel();
}


void CDlgProgressBar::setProgressBarPos(int iPos)
{
	m_progressCtl.SetPos(iPos);
}

BOOL CDlgProgressBar::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	if (m_isUserInLoadDownLog)
	{
		SetDlgItemText(IDC_STATIC_PROGRESSBAR, _T("正在从日志服务器下载日志中,请稍候..."));
	}
	else
	{
		SetDlgItemText(IDC_STATIC_PROGRESSBAR, _T("正在把日志导入到数据库中,请稍候..."));
	}

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}