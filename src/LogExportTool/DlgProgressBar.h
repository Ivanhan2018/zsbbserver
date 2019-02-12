#pragma once
#include "afxcmn.h"


// CDlgProgressBar �Ի���

class CDlgProgressBar : public CDialog
{
	DECLARE_DYNAMIC(CDlgProgressBar)

public:
	CDlgProgressBar(bool &isCancel, bool isUserInLoadDownLog, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgProgressBar();

// �Ի�������
	enum { IDD = IDD_DLG_PROGRESS_BAR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnCancel();

	void setProgressBarPos(int iPos);
public:
	CProgressCtrl m_progressCtl;
	bool &m_isCancel;
	bool m_isUserInLoadDownLog;
};
