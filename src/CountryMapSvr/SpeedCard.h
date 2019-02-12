/*=================================================================*/
/* �ļ���   : SpeedCard.h
/* �ļ����� : ������ֵ
/* ����ʱ�� : 2008-11-25
/* ������   : �����
/* ��ע     :
/*
/*-----------------------------------------------------------------*/
/* �޸ļ�¼1:
/* �޸�ʱ�� :
/* . . .
/*=================================================================*/
#ifndef SPEED_CARD_HEADER
#define SPEED_CARD_HEADER

#pragma once
#include "Header.h"

struct tagSpeedCard
{
	int iProduceValue;		//�������ٿ���������Դ����������
	int iFoodValue;				//��ʳ���ٿ�
	int iWoodValue;				//ľ�����ٿ�
	int iSkinValue;				//Ƥ�����ٿ�
	int iIronValue;				//�������ٿ�
	int iConstrValue;			//�������ٿ�
	int iConscrValue;			//�������ٿ�
	int iResearchValue;			//�������ٿ�
	int iRecruitValue;			//��ļӢ�ۿ�
	tagSpeedCard()
	{
		memset(this,0,sizeof(*this));
	}
};
class CSpeedCard
{
	//��������
public:
	map<unsigned long, tagSpeedCard*>		m_mapPlayerCard;
	//��������
public:
	CSpeedCard();
	virtual ~CSpeedCard();
public:
	//����֪ͨ
	void IncreaseSpeed(void * pDataBuffer, int iDataSize);
	//�ָ������ٶ�
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
