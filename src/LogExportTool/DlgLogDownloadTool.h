// CDlgLogDownloadTool.h : ͷ�ļ�
//

#pragma once


// CDlgLogDownloadTool �Ի���
class CDlgLogDownloadTool : public CDialog
{
// ����
public:
	CDlgLogDownloadTool(CWnd* pParent = NULL);	// ��׼���캯��

	~CDlgLogDownloadTool();
// �Ի�������
	enum { IDD = IDD_LOGEXPORTTOOL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnDownload();
	afx_msg void OnBnClickedBtnExport();
	afx_msg void OnEnKillfocusEditFromDate();
	afx_msg void OnEnKillfocusEditToDate();
	afx_msg void OnNcDestroy();

private:
	bool checkoutInputString(CString &sText, bool isFromDate);
	void readIniFile();

public:
	CString m_sFromDate;
	CString m_sToDate;

	// LogServer������Ϣ
	CString m_sLogServerIP;
	unsigned short m_iPort;

	HANDLE m_hThread;

private:
	// DBServer������Ϣ
	CString m_sDBDSN;
	CString m_sDBUser;
	CString m_sDBPassword;

	unsigned int m_iFromDateYearMonth;
	unsigned int m_iFromDateDay;

	unsigned int m_iToDateYearMonth;
	unsigned int m_iToDateDay;
};
