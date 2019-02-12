/*=================================================================*/
/*研发团队  : 深圳市新飞扬
/* 文件名   : ResearchManager.h
/* 文件描述 : 科研
/* 创建时间 : 2008-3-27
/* 创建者   : 曾庆辉
/* 备注     :
/*
/*-----------------------------------------------------------------*/
/* 修改记录1:
/* 修改时间 :
/* . . .
/*=================================================================*/
#ifndef RESEARCH_HEADER
#define RESEARCH_HEADER

#include "UserInfo.h"
#include "SpeedCard.h"
//科研道具
struct tagResearchNode
{
	long									TimerID;
	int										iTypeID;
	int										iItemID;					//道具ID
	int										iLevel;						//道具等级
	int64									lCompleteTime;

};
#define										MAX_PRODUCE_NUM				30

class CResearchManager
{
	//变量定义
	typedef map<unsigned long,tagResearchNode> ResearchActivity_Map;
	typedef map<unsigned long,tagResearchNode>::iterator ResearchActivity_Iterator;
private:
	CUserInfo								m_UserInfo;	
	//map<unsigned long, tagResearchNode>	m_mapResearchItem;
	ResearchActivity_Map					m_mapResearchItem;

	//函数定义
public:
	CResearchManager();
	virtual ~CResearchManager();
	//功能函数
	void SetMapID(int mapid);
public:
	//得到所有已经研究了的道具列表
	bool OnGetAllResearchList(int handle, unsigned long lPlayerID);
	//科研道具技术,涉及到金钱、科技点
	int OnResearchItem(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID);
	//得到正在科研中的道具技术
	bool OnGetResearchingItem(int handle, unsigned long lPlayerID);
	//取消科研道具技术,涉及到金钱、科技点
	int OnCancelResearchItem(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID); 
	//科研时间事件
	void OnResearchTimeEvent(void * pDataBuffer, int iDataSize);
	
	//科研时间事件
	//void OnResearchTimeEvent(void * pDataBuffer, int iDataSize,int iType);
	void OnResearchSpeed(unsigned long lPlayerID, int iSortID, int iValue);

	void OnImmedComplete(LPImmedComplete_Request msg);//立即完成
	void OnImmedCompleteNotifyMsg(LPNotify_ImmedComplete_Response msg);//处理收费系统返回的通知消息
public:
	//读取武器科研配置表
	void ReadWeaponConfig(int iType=0);
	void InitResearchActivity(DB_Proxy *pDBProxy);//恢复科研
	
private:
	int64 GetCurrentTime();
		//道具升级
	void ItemUpGrade(unsigned long lPlayerID, int iType, int iLevel);
};
extern CSpeedCard								g_SpeedCard;
extern DB_Proxy									g_Proxy;
#endif