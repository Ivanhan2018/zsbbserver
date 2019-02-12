#pragma once

#define OTL_STL
#define OTL_ODBC

#if defined (WIN32)
#define OTL_BIGINT __int64
#else
#define OTL_BIGINT long long
#endif

#include <afxdb.h>  // MFC ODBC database classes
#include <list>

#include "otlv4.h"

// CDlgExportLogTool �Ի���

class CDlgExportLogTool : public CDialog
{
	DECLARE_DYNAMIC(CDlgExportLogTool)

public:
	CDlgExportLogTool(const CString &sDBDSN, const CString &sDBUser, const CString &sDBPassword, CWnd* pParent = NULL);   // ��׼���캯��
	~CDlgExportLogTool();

// �Ի�������
	enum { IDD = IDD_DLG_EXPORT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnExportFiles();
	afx_msg void OnBnClickedBtnExportFolder();	
	
	HANDLE m_hThread;
	std::list<CString> m_lstFilePath;
	otl_connect db_;

private:
	void Recurse(LPCTSTR szDir);
	void createWorkThreadTToExportLogToDB();

	CString m_sDBDSN;
	CString m_sDBUser;
	CString m_sDBPassword;
	

};
