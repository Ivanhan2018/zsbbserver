#include "NewsReport.h"
#include "NetTask.h"
#define  MAX_NEWS_NUM  20
CNewsReport  g_NewsReport;

CNewsReport::CNewsReport(void)
{
	m_newscount = 0;
}

CNewsReport::~CNewsReport(void)
{
	EmptyeNewList();
}

void CNewsReport::InitNews()
{
	NewsTable News;
	NewsTable * pNews = NULL;
	char* p;
	int len;
	int lentmp =0;
	int ret = g_NetTask.proxy.db_select_all(News,p,len);
	if (len>0)
	{
		while (len>lentmp && len>=lentmp+sizeof(NewsTable))
		{
			pNews = new NewsTable;
			memset(pNews,0,sizeof(NewsTable));
			memcpy(pNews,p+lentmp,sizeof(NewsTable));
			pNews->content_[SIZE_128-1]= '\0';

			m_NewsList[pNews->newsid_] = pNews;
			lentmp += sizeof(NewsTable);
			m_newscount++;


		}
		delete [] p;


	}
}

NewsTable* CNewsReport::GetNews(int newsid)
{
	NewsTable* pNews = NULL;
	map<unsigned int ,NewsTable*>::iterator iter;
	iter = m_NewsList.find(newsid);
	if (iter != m_NewsList.end())
	{
		pNews = iter->second;
	}
	return pNews;
}

int CNewsReport::GetNewsCount()
{
	return m_newscount;
}

int CNewsReport::insertNews(NewsTable* news ,DB_Proxy& proxy)
{
	NewsTable table;
	memcpy(table.content_,news->content_,SIZE_128);
	table.newstype_ = news->newstype_;
	int ret = proxy.db_insert(table);
	if (ret != DB_Proxy::DB_SUCCEED)
	{
		return 0;
	}
	news->newsid_ = table.newsid_;
	NewsTable* pNews = 0;
	
	// 09-2-25
	//限定战报新闻条数
	while (m_NewsList.size()>=MAX_NEWS_NUM)
	{		
		unsigned int ulNewsID = 0xFFFFFFFF;
		for (map<unsigned int,NewsTable*>::iterator iter = m_NewsList.begin();iter!=m_NewsList.end();iter ++)
		{
			pNews = iter->second;
			if (pNews->newsid_<ulNewsID)
			{
				ulNewsID = pNews->newsid_;
				break;
			}
		}
		deleteNews(ulNewsID,proxy);
	}
	
	m_NewsList.insert(map<unsigned long ,NewsTable*>::value_type(news->newsid_,news));
	return table.newsid_;
}

void CNewsReport::deleteNews(int newsid,DB_Proxy& proxy)
{
	NewsTable* pNews = NULL;
	map<unsigned int , NewsTable*>::iterator iter;
	iter = m_NewsList.find(newsid);
	if (iter != m_NewsList.end())
	{
		pNews = iter->second;
		m_NewsList.erase(iter);
		delete pNews;
		pNews = NULL;
	}
	NewsTable table ;
	table.setid(newsid);
	int ret = proxy.db_delete(table);
	if (ret!=DB_Proxy::DB_SUCCEED)
	{
		std::cout<<"db delete error"<<std::endl;
	}
}

void CNewsReport::EmptyeNewList()
{
	NewsTable* pNews = NULL;
	map<unsigned int , NewsTable*>::iterator  iter ;
	for (iter = m_NewsList.begin() ;iter != m_NewsList.end();iter ++)
	{
		pNews = iter->second;
		delete pNews;
		pNews = NULL;
	}
	m_NewsList.clear();

}