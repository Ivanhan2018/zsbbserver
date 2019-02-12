/*=================================================================*/
/* 文件名   : SpeedCard.h
/* 文件描述 : 提速数值
/* 创建时间 : 2008-11-25
/* 创建者   : 曾庆辉
/* 备注     :
/*
/*-----------------------------------------------------------------*/
/* 修改记录1:
/* 修改时间 :
/* . . .
/*=================================================================*/
#ifndef SPEED_CARD_HEADER
#define SPEED_CARD_HEADER

#pragma once
#include "Header.h"

struct tagSpeedCard
{
	int iProduceValue;		//生产提速卡，基本资源生产提升卡
	int iFoodValue;				//粮食提速卡
	int iWoodValue;				//木材提速卡
	int iSkinValue;				//皮卡提速卡
	int iIronValue;				//铁卡提速卡
	int iConstrValue;			//建筑提速卡
	int iConscrValue;			//征兵提速卡
	int iResearchValue;			//科研提速卡
	int iRecruitValue;			//招募英雄卡
	tagSpeedCard()
	{
		memset(this,0,sizeof(*this));
	}
};
class CSpeedCard
{
	//变量定义
public:
	map<unsigned long, tagSpeedCard*>		m_mapPlayerCard;
	//函数定义
public:
	CSpeedCard();
	virtual ~CSpeedCard();
public:
	//提速通知
	void IncreaseSpeed(void * pDataBuffer, int iDataSize);
	//恢复正常速度
	void NormalSpeed(void * pDataBuffer, int iDataSize);
public:
	int AddPlayer(unsigned long lPlayerID);
	int DeletePlayer(unsigned long lPlayerID);
	int FindPlayer(unsigned long lPlayerID);
	void ClearPlayer();
public:
	void SetProduceValue(unsigned long lPlayerID, int iValue);
	void SetFoodValue(unsigned long lPlayerID, int iValue);
	void SetWoodValue(unsigned long lPlayerID, int iValue);
	void SetSkinValue(unsigned long lPlayerID, int iValue);
	void SetIronValue(unsigned long lPlayerID, int iValue);
	void SetConstrValue(unsigned long lPlayerID, int iValue);
	void SetConscrValue(unsigned long lPlayerID, int iValue);
	void SetResearchValue(unsigned long lPlayerID, int iValue);
	void SetRecruitValue(unsigned long lPlayerID, int iValue);

	int GetProduceValue(unsigned long lPlayerID);
	int GetFoodValue(unsigned long lPlayerID);
	int GetWoodValue(unsigned long lPlayerID);
	int GetSkinValue(unsigned long lPlayerID);
	int GetIronValue(unsigned long lPlayerID);
	int GetConstrValue(unsigned long lPlayerID);
	int GetConscrValue(unsigned long lPlayerID);
	int GetResearchValue(unsigned long lPlayerID);
	int GetRecruitValue(unsigned long lPlayerID);

	tagSpeedCard * GetSpeedCardInfo(unsigned long lPlayerID);
};

#endif
