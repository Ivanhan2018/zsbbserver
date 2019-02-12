/*
处理战报新闻信息
*/
#pragma once
#include "ProChatSystem.h"
#include <map>
#include "DB_Table.h"
#include "DB_Proxy.h"
using namespace std;
class CNewsReport
{
public:
	CNewsReport(void);
	~CNewsReport(void);
	void		InitNews(void) ;
	NewsTable*	GetNews(int newsid);
	int			insertNews(NewsTable* news ,DB_Proxy& proxy);
	void		deleteNews(int newsid ,DB_Proxy & proxy);
	int			GetNewsCount();

	void		EmptyeNewList();
private:
	int m_newscount;
public:
	map<unsigned int ,NewsTable*> m_NewsList;

};

extern CNewsReport  g_NewsReport;
