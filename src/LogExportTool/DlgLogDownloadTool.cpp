// CDlgLogDownloadTool.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include <cassert>
#include <process.h> 

#include "LogExportTool.h"
#include "DlgLogDownloadTool.h"
#include "DlgExportLogTool.h"


#include "export_log_tool_dll.hpp"
#include "DlgProgressBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

enum
{
	TEN = 10,
	ONE_HUNDRED_EIGTH = 128
};


CString g_sSaveFolder;
bool g_isCancel = false;

CDlgProgressBar g_dlgProgressBar(g_isCancel, true);

bool EXPORT_LOG_TOOL_DLL_API createAndWriteFile(const char * filename, const char * buffer, int length, float rate)
{
	if (NULL == filename || NULL == buffer || 0 == length)
	{
		assert(0);
		return false;
	}

	CString sFilePath(g_sSaveFolder);
	sFilePath += "\\";
	sFilePath += filename;

	//CFile file(sFilePath.GetBuffer(), CFile::modeCreate | CFile::modeWrite);
	//file.Write(buffer, length);
	//file.Flush();

	//file.Close();

	FILE * fp = ::fopen(sFilePath.GetBuffer(), "w+");
	::fwrite(buffer, length, 1, fp);
	fclose(fp);

	if (NULL != g_dlgProgressBar.m_hWnd)
	{
		g_dlgProgressBar.setProgressBarPos(static_cast<int>(rate));
	}

	return !g_isCancel;
}

unsigned int __stdcall Entry(void * ptr)
{
	CDlgLogDownloadTool * dlgLogDownTool = (CDlgLogDownloadTool *)ptr;

	if (NULL == dlgLogDownTool)
	{
		return 1;
	}

#ifdef _DEBUG
	unsigned int iTime = GetTickCount();
#endif

	int iRet = log_download(dlgLogDownTool->m_sLogServerIP.GetBuffer(), dlgLogDownTool->m_iPort, atoi(dlgLogDownTool->m_sFromDate.GetBuffer()),
				atoi(dlgLogDownTool->m_sToDate.GetBuffer()), createAndWriteFile);

	if (socket_error == iRet || connect_error == iRet)
	{
		if (NULL != g_dlgProgressBar.m_hWnd)
		{
			::PostMessage(g_dlgProgressBar.m_hWnd, WM_CLOSE, 0 , 0);
		}

		AfxMessageBox("Ŀ¼�����ڻ�����������,����", MB_OK | MB_ICONERROR);
	}
	else if (dir_nonexist == iRet)
	{
		if (NULL != g_dlgProgressBar.m_hWnd)
		{
			::PostMessage(g_dlgProgressBar.m_hWnd, WM_CLOSE, 0 , 0);
		}

		AfxMessageBox("�������־����־���������Ҳ���", MB_OK | MB_ICONWARNING);
	}
	else if (uncompress_error == iRet)
	{
		if (NULL != g_dlgProgressBar.m_hWnd)
		{
			::PostMessage(g_dlgProgressBar.m_hWnd, WM_CLOSE, 0 , 0);
		}

		AfxMessageBox("���ݰ��ڽ�ѹ��ʱ���ִ���", MB_OK | MB_ICONWARNING);
	}
	else
	{		
		AfxMessageBox("������ɣ�����");

		if (NULL != g_dlgProgressBar.m_hWnd)
		{
			::PostMessage(g_dlgProgressBar.m_hWnd, WM_CLOSE, 0 , 0);
		}
#ifdef _DEBUG
		iTime = GetTickCount() - iTime;
		CString sTime;
		sTime.Format("%d", iTime);
		AfxMessageBox(sTime);
#endif
	}

	::CloseHandle(dlgLogDownTool->m_hThread);
	dlgLogDownTool->m_hThread = NULL;
	return 0;
}


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CLogExportToolDlg �Ի���

CDlgLogDownloadTool::CDlgLogDownloadTool(CWnd* pParent /*=NULL*/)
: CDialog(CDlgLogDownloadTool::IDD, pParent), m_sFromDate(_T("")), m_sToDate(_T("")), m_sLogServerIP(_T("")), m_iPort(0), 
m_sDBDSN(_T("")), m_sDBUser(_T("")), m_sDBPassword(_T("")), m_iFromDateYearMonth(0), m_iFromDateDay(0), m_iToDateYearMonth(0), m_iToDateDay(0), m_hThread(NULL)

{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CDlgLogDownloadTool::~CDlgLogDownloadTool()
{
	if (NULL != m_hThread)
	{
		::CloseHandle(m_hThread);
		m_hThread = NULL;
	}
}

void CDlgLogDownloadTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_FORM_DATE, m_sFromDate);
	DDX_Text(pDX, IDC_EDIT_TO_DATE, m_sToDate);
	//DDV_MaxChars(pDX, m_sFromDate, 10);
	//DDV_MaxChars(pDX, m_sToDate, 10);
}

BEGIN_MESSAGE_MAP(CDlgLogDownloadTool, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_DOWNLOAD, &CDlgLogDownloadTool::OnBnClickedBtnDownload)
	ON_BN_CLICKED(IDC_BTN_EXPORT, &CDlgLogDownloadTool::OnBnClickedBtnExport)
	ON_EN_KILLFOCUS(IDC_EDIT_FORM_DATE, &CDlgLogDownloadTool::OnEnKillfocusEditFromDate)
	ON_EN_KILLFOCUS(IDC_EDIT_TO_DATE, &CDlgLogDownloadTool::OnEnKillfocusEditToDate)
END_MESSAGE_MAP()


// CDlgLogDownloadTool ��Ϣ�������

BOOL CDlgLogDownloadTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	readIniFile();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CDlgLogDownloadTool::OnSysCommand(UINT nID, LPARAM lParam)
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CDlgLogDownloadTool::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ��������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
//
HCURSOR CDlgLogDownloadTool::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CDlgLogDownloadTool::OnBnClickedBtnDownload()
{
	g_sSaveFolder.Empty();

	if (m_sFromDate.IsEmpty() || m_sToDate.IsEmpty())
	{
		MessageBox(_T("���������ڣ�"), _T("����"), MB_OK | MB_ICONERROR);
		return;
	}

	if (0 != m_iToDateYearMonth && m_iToDateYearMonth < m_iFromDateYearMonth)
	{
		MessageBox(_T("�����to���ڱ�from���ڻ�����, ����������"), _T("����"), MB_OK | MB_ICONERROR);
		return;
	}

	if (m_iToDateYearMonth == m_iFromDateYearMonth && m_iToDateDay < m_iFromDateDay)
	{
		MessageBox(_T("�����to���ڱ�from���ڻ�����, ����������"), _T("����"), MB_OK | MB_ICONERROR);
		return;
	}

	TCHAR szDir[MAX_PATH] = {0};
	BROWSEINFO bi;
	memset(&bi, 0, sizeof(bi));

	bi.hwndOwner = this->m_hWnd;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = szDir;
	bi.lpszTitle = _T("ѡ�����ص���־���ֵ��ļ���");
	bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_DONTGOBELOWDOMAIN;
	bi.lpfn = NULL;
	bi.lParam = 0;
	bi.iImage = 0;

	ITEMIDLIST *pidl  = SHBrowseForFolder(&bi);

	if(NULL == pidl) 
	{
		return;
	}

	SHGetPathFromIDList(pidl, szDir);

	g_sSaveFolder = CString(szDir);

	if (g_sSaveFolder.IsEmpty())
	{
		MessageBox(_T("û��ѡ�����ر�����־���ļ���, ����ѡ��"), _T("����"), MB_OK | MB_ICONWARNING);
		return;
	}

	if (NULL != m_hThread)
	{
		::CloseHandle(m_hThread);
		m_hThread = NULL;
	}

	unsigned int tid = 0;
	m_hThread = (HANDLE)::_beginthreadex(0, 0, 
		Entry, (void *)this, 0, &tid);

	if (NULL == m_hThread)
	{
		MessageBox(_T("���������߳�ʧ�ܣ�������"), _T("����"), MB_OK | MB_ICONERROR);	

		return;
	}

	g_isCancel = false;
	g_dlgProgressBar.DoModal();
}		

void CDlgLogDownloadTool::OnBnClickedBtnExport()
{
	CDlgExportLogTool dlgExportLogTool(m_sDBDSN, m_sDBUser, m_sDBPassword);
	dlgExportLogTool.DoModal();
}

void CDlgLogDownloadTool::OnEnKillfocusEditFromDate()
{
	GetDlgItemText(IDC_EDIT_FORM_DATE, m_sFromDate);
	m_sFromDate.Trim();

	if (m_sFromDate.IsEmpty())
	{
		return;
	}

	int ilen = m_sFromDate.GetLength();

	if (ilen > TEN)
	{
		MessageBox(_T("����������ַ����ȹ���, ����������"), _T("����"), MB_OK | MB_ICONERROR);
		m_sFromDate.Empty();
		m_iFromDateYearMonth = 0;
		m_iFromDateDay = 0;
		SetDlgItemText(IDC_EDIT_FORM_DATE, _T(""));
		GetDlgItem(IDC_EDIT_FORM_DATE)->SetFocus();
	}

	bool isVaild = checkoutInputString(m_sFromDate, true);

	if (!isVaild)
	{
		m_sFromDate.Empty();
		m_iFromDateYearMonth = 0;
		m_iFromDateDay = 0;
		SetDlgItemText(IDC_EDIT_FORM_DATE, _T(""));
		GetDlgItem(IDC_EDIT_FORM_DATE)->SetFocus();
	}
}

void CDlgLogDownloadTool::OnEnKillfocusEditToDate()
{
	GetDlgItemText(IDC_EDIT_TO_DATE, m_sToDate);
	m_sToDate.Trim();

	if (m_sToDate.IsEmpty())
	{
		return;
	}

	int ilen = m_sToDate.GetLength();

	if (ilen > TEN)
	{
		MessageBox(_T("����������ַ����ȹ���, ����������"), _T("����"), MB_OK | MB_ICONERROR);
		m_sToDate.Empty();
		m_iToDateYearMonth = 0;
		m_iToDateDay = 0;
		SetDlgItemText(IDC_EDIT_TO_DATE, _T(""));
		GetDlgItem(IDC_EDIT_TO_DATE)->SetFocus();
	}

	bool isVaild = checkoutInputString(m_sToDate, false);

	if (!isVaild)
	{
		m_sToDate.Empty();
		m_iToDateYearMonth = 0;
		m_iToDateDay = 0;
		SetDlgItemText(IDC_EDIT_TO_DATE, _T(""));
		GetDlgItem(IDC_EDIT_TO_DATE)->SetFocus();
	}
}

void CDlgLogDownloadTool::readIniFile() 
{
	::GetPrivateProfileString(_T("LogServer"), _T("logServerIP"), NULL, m_sLogServerIP.GetBuffer(ONE_HUNDRED_EIGTH), ONE_HUNDRED_EIGTH, _T("..\\bin\\LogExportToolConfig.ini"));
	m_iPort = static_cast<unsigned short>(::GetPrivateProfileInt(_T("LogServer"), _T("logServerPort"), 0, _T("..\\bin\\LogExportToolConfig.ini")));
	
	::GetPrivateProfileString(_T("DBServer"), _T("db_dsn"), NULL, m_sDBDSN.GetBuffer(ONE_HUNDRED_EIGTH), ONE_HUNDRED_EIGTH, _T("..\\bin\\LogExportToolConfig.ini"));
	::GetPrivateProfileString(_T("DBServer"), _T("DBUserName"), NULL, m_sDBUser.GetBuffer(ONE_HUNDRED_EIGTH), ONE_HUNDRED_EIGTH, _T("..\\bin\\LogExportToolConfig.ini"));
	::GetPrivateProfileString(_T("DBServer"), _T("DBPassword"), NULL, m_sDBPassword.GetBuffer(ONE_HUNDRED_EIGTH), ONE_HUNDRED_EIGTH, _T("..\\bin\\LogExportToolConfig.ini"));
}

bool CDlgLogDownloadTool::checkoutInputString(CString &sText, bool isFromDate)
{
	int findIndex = sText.Find('.', 0);
	
	if (-1 == findIndex || 0 == findIndex)
	{
		MessageBox(_T("����������ַ�����ʽ���Ϸ�, ����������"), _T("����"), MB_OK | MB_ICONERROR);
		return false;
	}

	CString sTmp = sText.Left(findIndex);
	int iYear = atoi(sTmp.GetBuffer());

	if (iYear <= 0)
	{
		MessageBox(_T("����������ַ��������ֵ���Ϸ�, ����������"), _T("����"), MB_OK | MB_ICONERROR);
		return false;
	}

	sText = sText.Right(sText.GetLength() - findIndex - 1);

	findIndex = sText.Find('.', 0);

	if (-1 == findIndex)
	{
		MessageBox(_T("����������ַ�����ʽ���Ϸ�, ����������"), _T("����"), MB_OK | MB_ICONERROR);
		return false;
	}

	sTmp = sText.Left(findIndex);
	int iMonth = atoi(sTmp.GetBuffer());

	if (iMonth <= 0 || iMonth > 12)
	{
		MessageBox(_T("����������ַ������·�ֵ���Ϸ�, ����������"), _T("����"), MB_OK | MB_ICONERROR);
		return false;
	}

	sText = sText.Right(sText.GetLength() - findIndex - 1);

	findIndex = sText.Find('.', 0);

	if (-1 != findIndex)
	{
		MessageBox(_T("����������ַ�����ʽ���Ϸ�, ����������"), _T("����"), MB_OK | MB_ICONERROR);
		return false;
	}

	int iDay = atoi(sText.GetBuffer());

	if (iDay <= 0 || iDay > 31)
	{
		MessageBox(_T("����������ַ�������ֵ���Ϸ�, ����������"), _T("����"), MB_OK | MB_ICONERROR);
		return false;
	}
	
	sTmp.Empty();
	sTmp.Format("%d%d", iYear, iMonth);

	if (isFromDate)
	{
		m_iFromDateYearMonth = atoi(sTmp.GetBuffer());
		m_iFromDateDay = iDay;
	}
	else
	{
		m_iToDateYearMonth = atoi(sTmp.GetBuffer());
		m_iToDateDay = iDay;
	}

	sTmp.Empty();
	sText.Empty();

	sText.Format("%d", iYear);

	if (iMonth < 10)
	{
		sTmp.Format("0%d", iMonth);
	}
	else
	{
		sTmp.Format("%d", iMonth);
	}

	sText += sTmp;
	sTmp.Empty();

	if (iDay < 10)
	{
		sTmp.Format("0%d", iDay);
	}
	else
	{
		sTmp.Format("%d", iDay);
	}

	sText += sTmp;
	
	return true;
}


void CDlgLogDownloadTool::OnNcDestroy()
{
	CDialog::OnNcDestroy();
	// TODO: �ڴ˴������Ϣ����������
}
