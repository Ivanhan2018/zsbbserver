// DlgProgressBar.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "LogExportTool.h"
#include "DlgProgressBar.h"


// CDlgProgressBar �Ի���

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


// CDlgProgressBar ��Ϣ�������

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

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	if (m_isUserInLoadDownLog)
	{
		SetDlgItemText(IDC_STATIC_PROGRESSBAR, _T("���ڴ���־������������־��,���Ժ�..."));
	}
	else
	{
		SetDlgItemText(IDC_STATIC_PROGRESSBAR, _T("���ڰ���־���뵽���ݿ���,���Ժ�..."));
	}

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}