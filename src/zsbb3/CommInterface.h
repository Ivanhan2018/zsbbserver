/*=================================================================*/
/*�з��Ŷ�  : �������·���
/* �ļ���   : CommInterface.h
/* �ļ����� : ͨ�ú������ýӿ�
/* ����ʱ�� : 2008-10-27
/* ������   : �����
/* ��ע     :
/*
/*-----------------------------------------------------------------*/
/* �޸ļ�¼1:
/* �޸�ʱ�� :
/* . . .
/*=================================================================*/
#ifndef __COMM_INFERFACE__
#define __COMM_INFERFACE__
#include <stdio.h>
#include "Header.h"
#include "ISystemLog.h"
#define MAX_LEVEL		200			//��߼���

//int HeroExp(const int lExp, int iLevel, int &iSExp);
int HeroCredit(const int lCredit);
int GetLevel(int curlevel,int in_exp,int &out_exp);//add by xujincai 20090211
//ʰ��װ��
int OnInsertHeroEquip(DB_Proxy *pProxy, unsigned long lPlayerID, unsigned long lHeroID, __int64 iItemID, int iLevel, int iType,bool bOverlap,int skill,int skilllevel,PlayerItemStorageTable &Pack);

//Ӣ�۳ɳ�
void heroUpgrade(int level,PlayerOfficerTable *officer,PlayerOfficerTable *upofficer);
int OnHeroGrowUp(DB_Proxy *pProxy, tagHeroParam & HeroParam,const int method);
//��²Ӣ��
//int OnCaptiveHero(DB_Proxy *pProxy, tagCaptiveHero & HeroParam);
//���������Դ ���:0:ʧ��;1:�ɹ�
int OnUpDataPlayerRes(DB_Proxy *pProxy, unsigned long lPlayerID, unsigned long lMapID, unsigned long lCityID, tagPlayerResource &PlayerRes);

//����������ս��Ʒ
//Ӫ��ID:iCountry(����;����),ս��ƷID:iItemID ս��Ʒ����(��GOODS_TYPEö��):iType ս��Ʒ����:iCount
//���:0:ʧ��;1:�ɹ�
int OnVendueGoods(DB_Proxy *pProxy, int iCountry, unsigned long lMapID, int iItemID, int iType, int iCount);
//ɾ����ҳ���
//���:0:ʧ��;1:�ɹ�
int OnDeleteCity(DB_Proxy *pProxy, unsigned long lPlayerID, unsigned long lMapID, unsigned long lCityID);
//ʰ��װ��
int OnInsertHeroEquip(DB_Proxy &Proxy, unsigned long lPlayerID, unsigned long lHeroID, __int64 iItemID, int iLevel, int iType,bool bOverlap,int skill,int skilllevel,PlayerItemStorageTable &Pack);
//Ӣ�۳ɳ�
int OnHeroGrowUp(DB_Proxy &Proxy, tagHeroParam & HeroParam,const int method);
//��²Ӣ��
//int OnCaptiveHero(DB_Proxy &Proxy, tagCaptiveHero & HeroParam);
//���������Դ ���:0:ʧ��;1:�ɹ�
int OnUpDataPlayerRes(DB_Proxy &Proxy, unsigned long lPlayerID, unsigned long lMapID, unsigned long lCityID, tagPlayerResource &PlayerRes);

//����������ս��Ʒ
//Ӫ��ID:iCountry(����;����),ս��ƷID:iItemID ս��Ʒ����(��GOODS_TYPEö��):iType ս��Ʒ����:iCount
//���:0:ʧ��;1:�ɹ�
int OnVendueGoods(DB_Proxy &Proxy, int iCountry, unsigned long lMapID, int iItemID, int iType, int iCount);
//ɾ����ҳ���
//���:0:ʧ��;1:�ɹ�
int OnDeleteCity(DB_Proxy &Proxy, unsigned long lPlayerID, unsigned long lMapID, unsigned long lCityID);
//ָ�ӹ�ѧϰ�µļ���
//ArmyID ָ�ӹٴ��Ķ�����
//skillId ѧϰ���ܱ��
void OfficerStudySkill(unsigned long ArmyID,int skillID);

//����ָ�ӹ�װ������¹���
//ArmyID ָ�ӹٴ��Ķ�����
//attackpower ������ 
// attacktype ��������
// defendpower ������
// defendtype ��������
void UpdateOfficerAttr(unsigned long ArmyID, int Attackpower,int DefendPypower,int defendMgPower);

//add by xujincai 20081218
int GetUnusedPos(PlayerItemStorageTable &packitem,DB_Proxy *pDBProxy);		//����һ��δʹ�õı�����Ԫ��
//��ѯ��ҵ����ִ�
int GetHomeCityInfo(DB_Proxy *pDBProxy,int playerid,int &mapid,int &cityid);
//add by xujincai 20090205
int UpdateProductionAmount(DB_Proxy *pDBProxy, unsigned long lPlayerID, int iItemID, int iType, int iCount);//�������������ߵ�
int UpdateProductionAmountEx(DB_Proxy *pDBProxy,unsigned long lPlayerID, int mapid,int ciytid,
							 int iItemID, int iType, int iCount,int &oldcount,int &curcount);
void ProcHeroAttribute(PlayerOfficerTable &hero);
int deleteItemOfPack(DB_Proxy *dbProxy,PlayerItemStorageTable &item);//ɾ�������е�һ����Ŀ
GOODS_TYPE getGoodsTypeEx(int goodsId);

//��һ�η�Χֵ�����һ����
int getRandomValue(int minvalue,int maxvalue);
#endif