#pragma once

class CDataTss
{
public:
	CDataTss(void);
	~CDataTss(void);
	void PutData(int info);
	int GetData();
private:
	int m_data;
};

extern CDataTss g_DataTss;
