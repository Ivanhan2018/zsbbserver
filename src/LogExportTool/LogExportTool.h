// LogExportTool.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CLogExportToolApp:
// �йش����ʵ�֣������ LogExportTool.cpp
//

class CLogExportToolApp : public CWinApp
{
public:
	CLogExportToolApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CLogExportToolApp theApp;