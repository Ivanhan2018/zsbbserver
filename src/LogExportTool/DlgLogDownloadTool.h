// CDlgLogDownloadTool.h : 头文件
//

#pragma once


// CDlgLogDownloadTool 对话框
class CDlgLogDownloadTool : public CDialog
{
// 构造
public:
	CDlgLogDownloadTool(CWnd* pParent = NULL);	// 标准构造函数

	~CDlgLogDownloadTool();
// 对话框数据
	enum { IDD = IDD_LOGEXPORTTOOL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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

	// LogServer配置信息
	CString m_sLogServerIP;
	unsigned short m_iPort;

	HANDLE m_hThread;

private:
	// DBServer配置信息
	CString m_sDBDSN;
	CString m_sDBUser;
	CString m_sDBPassword;

	unsigned int m_iFromDateYearMonth;
	unsigned int m_iFromDateDay;

	unsigned int m_iToDateYearMonth;
	unsigned int m_iToDateDay;
};
