/*=================================================================*/
/*研发团队  : 深圳市新飞扬
/* 文件名   : CommInterface.h
/* 文件描述 : 通用函数调用接口
/* 创建时间 : 2008-10-27
/* 创建者   : 曾庆辉
/* 备注     :
/*
/*-----------------------------------------------------------------*/
/* 修改记录1:
/* 修改时间 :
/* . . .
/*=================================================================*/
#ifndef __COMM_INFERFACE__
#define __COMM_INFERFACE__
#include <stdio.h>
#include "Header.h"
#include "ISystemLog.h"
#define MAX_LEVEL		200			//最高级别

//int HeroExp(const int lExp, int iLevel, int &iSExp);
int HeroCredit(const int lCredit);
int GetLevel(int curlevel,int in_exp,int &out_exp);//add by xujincai 20090211
//拾到装备
int OnInsertHeroEquip(DB_Proxy *pProxy, unsigned long lPlayerID, unsigned long lHeroID, __int64 iItemID, int iLevel, int iType,bool bOverlap,int skill,int skilllevel,PlayerItemStorageTable &Pack);

//英雄成长
void heroUpgrade(int level,PlayerOfficerTable *officer,PlayerOfficerTable *upofficer);
int OnHeroGrowUp(DB_Proxy *pProxy, tagHeroParam & HeroParam,const int method);
//俘虏英雄
//int OnCaptiveHero(DB_Proxy *pProxy, tagCaptiveHero & HeroParam);
//更新玩家资源 输出:0:失败;1:成功
int OnUpDataPlayerRes(DB_Proxy *pProxy, unsigned long lPlayerID, unsigned long lMapID, unsigned long lCityID, tagPlayerResource &PlayerRes);

//用于拍卖的战利品
//营阵ID:iCountry(联盟;部落),战利品ID:iItemID 战利品类型(见GOODS_TYPE枚举):iType 战利品数量:iCount
//输出:0:失败;1:成功
int OnVendueGoods(DB_Proxy *pProxy, int iCountry, unsigned long lMapID, int iItemID, int iType, int iCount);
//删除玩家城市
//输出:0:失败;1:成功
int OnDeleteCity(DB_Proxy *pProxy, unsigned long lPlayerID, unsigned long lMapID, unsigned long lCityID);
//拾到装备
int OnInsertHeroEquip(DB_Proxy &Proxy, unsigned long lPlayerID, unsigned long lHeroID, __int64 iItemID, int iLevel, int iType,bool bOverlap,int skill,int skilllevel,PlayerItemStorageTable &Pack);
//英雄成长
int OnHeroGrowUp(DB_Proxy &Proxy, tagHeroParam & HeroParam,const int method);
//俘虏英雄
//int OnCaptiveHero(DB_Proxy &Proxy, tagCaptiveHero & HeroParam);
//更新玩家资源 输出:0:失败;1:成功
int OnUpDataPlayerRes(DB_Proxy &Proxy, unsigned long lPlayerID, unsigned long lMapID, unsigned long lCityID, tagPlayerResource &PlayerRes);

//用于拍卖的战利品
//营阵ID:iCountry(联盟;部落),战利品ID:iItemID 战利品类型(见GOODS_TYPE枚举):iType 战利品数量:iCount
//输出:0:失败;1:成功
int OnVendueGoods(DB_Proxy &Proxy, int iCountry, unsigned long lMapID, int iItemID, int iType, int iCount);
//删除玩家城市
//输出:0:失败;1:成功
int OnDeleteCity(DB_Proxy &Proxy, unsigned long lPlayerID, unsigned long lMapID, unsigned long lCityID);
//指挥官学习新的技能
//ArmyID 指挥官代的队伍编号
//skillId 学习技能编号
void OfficerStudySkill(unsigned long ArmyID,int skillID);

//更新指挥官装备后更新攻击
//ArmyID 指挥官代的队伍编号
//attackpower 攻击力 
// attacktype 攻击类型
// defendpower 防御力
// defendtype 防御类型
void UpdateOfficerAttr(unsigned long ArmyID, int Attackpower,int DefendPypower,int defendMgPower);

//add by xujincai 20081218
int GetUnusedPos(PlayerItemStorageTable &packitem,DB_Proxy *pDBProxy);		//查找一个未使用的背包单元格
//查询玩家的新手村
int GetHomeCityInfo(DB_Proxy *pDBProxy,int playerid,int &mapid,int &cityid);
//add by xujincai 20090205
int UpdateProductionAmount(DB_Proxy *pDBProxy, unsigned long lPlayerID, int iItemID, int iType, int iCount);//更新武器、防具等
int UpdateProductionAmountEx(DB_Proxy *pDBProxy,unsigned long lPlayerID, int mapid,int ciytid,
							 int iItemID, int iType, int iCount,int &oldcount,int &curcount);
void ProcHeroAttribute(PlayerOfficerTable &hero);
int deleteItemOfPack(DB_Proxy *dbProxy,PlayerItemStorageTable &item);//删除背包中的一个项目
GOODS_TYPE getGoodsTypeEx(int goodsId);

//从一段范围值内随机一个数
int getRandomValue(int minvalue,int maxvalue);
#endif