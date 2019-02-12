#ifndef __FixTaskTable_H__
#define __FixTaskTable_H__

#include <map>
#include "ProTaskSystem.h"
#include "DB_ProxyHeader.h"
#include "DBTableHeader.h"
#include "MapServer_Protocol.h"
#include "CommonFunction.h"
#include "ZSBB_DataDefine.h"
#pragma warning(disable:4267)
#pragma warning(disable:4706)
class CNetTask;
//20090206
/*
新增的任务类型，看看能不能做
1、捐献任务。玩家捐献指定的金钱
	PlayerChatRoomTable
	contribute_
	8(待定)

2、招募英雄。判断玩家的英雄个数及等级
	PlayerOfficerTable
	9

3、加入军团。判断玩家是否是军团成员。（加入、创建都行）
	PlayerChatRoomTable 
	公会ID>0
	10

4、添加好友。判断玩家的好友数是否达到3个
	firendtable
	11


5、市场挂牌交易。判断玩家是否有商品在市场上挂牌
	Market
	12

6、收集套装。判断玩家英雄背包是否拥有指定套装装备。
	playeritemstoragetable
	13
7、基本资源
	14
	种类:类型ID:项编号:数量:等级
	项编号:1粮食2木材 3皮4铁 若有其的再编号
16:GO点查询任务
	16:0:0:GO点数:0
17:卡查询任务
    17:卡ID:Pos:数量:0
*/


//副本内容格式
//副本ID:boss:npc:build:副本ID:boss:npc:build


//任务奖励格式为
//0.道具(最多3项)		0:道具ID:道具类型:数量:道具ID:道具类型:数量:道具ID:道具类型:数量
//1.装备(最多3项)		1:装备ID:数量:装备ID:数量:装备ID:数量
//2.荣誉点，经验值		2:荣誉点:经验值
//3.金钱				3:金钱
//4.粮食,木材，铁，皮料 4:粮食:木材:皮料:铁
//5.奖励卡（最多5项)	5：卡ID：数量：卡ID：数量：卡ID：数量：卡ID：数量：卡ID：数量
//6.奖励GO点			6：数量
//7.奖励士兵			7：数量
//8.奖励技能点			8：数量
//以上用分号间隔，例奖励只有2和4则表示为 2:荣誉点:经验值;4:粮食:木材:皮料:铁


//道具
struct tagH_zero
{
	int hid;
	int htype;
	int num;
};
struct tagH_zero_list
{
	int count;
	tagH_zero hlist[3];
};

//装备；装备ID，数量
struct tagH_one
{
	int hid;
	int num;
};
struct tagH_one_list
{
	int count;
	tagH_one hlist[3];
};

//荣誉点，经验值
struct tagH_two
{
	int i1;   
	int i2;
};

//粮食，木材，铁，皮料
struct tagH_four
{
	int nums[4];
};

//副本内容
struct tagEctypeTask
{
	unsigned int taskid;
	int taskType;
	unsigned char boos;
	unsigned int npc;
	unsigned int build;
};

struct tagCard_Info
{
	int CardItemID;
	int Count;
};
struct  HORTATION_Plus
{
	bool b0;
	tagH_zero_list h0;

	bool b1;
	tagH_one_list h1;

	bool b2;
	tagH_two h2;

	bool b3;
	int money;

	bool b4;
	tagH_four h4;

	//卡,最多5张
	int CardCount;
	tagCard_Info CardItem[5];

	//GO点
	int GoDian;
	//奖励技能点
	int TechDot;
	//奖励兵
	int SoldierCount;
};

class CFixTaskTable
{
private:
	CFixTaskTable(){}
public:
	bool LoadData(CNetTask *pNetTask,const char * addr, short port);
	void UnLoadData();//读取任务参数
	bool loadTaskBaseTable(CNetTask *pNetTask);

	static CFixTaskTable & Instance()
	{
		static CFixTaskTable ftt;
		return ftt;
	}
	LPMailInfoParam_Item getMailInfoParam(int countryid,int type);
	//读取邮件参数表
	void loadMailInfoParam();
	void clearMailInfoParam();
	LPCardParam getCardConfig(int itemid);
	int loadCardConfigTable();
	int clearCardConfig();
	PackStoreParam *getPackStoreParam(int postion);
	int loadPackStoreParam();
	void clearPackStoreParam();
	HeroEquipAttribute * getHeroEquipAttribute(int itemid);
	//读取装备属性表
	void loadEquipAttribute();
	void clearEquipAttribute();


	//key(TaskID) 
	TaskBaseTable_Map m_BaseTaskMap;
	//key(taskID)
	std::map<unsigned int,HORTATION_Plus> m_HortationMap;
	
	HeroEquipAttribute_Map m_HeroEquipAttributeList; 
	//副本
	std::map<int,tagEctypeTask> m_EctypeTaslContainerMap;

	//日常任务
	std::map<int,DayTask_DetailInfo> m_DayTaskMap;

private:
	DB_Proxy mProxy;
	//邮件参数表
	MailInfoParam_Map									m_MailInfoParamMap;
	CardParam_Map										m_CardConfigMap;
	PackStoreParam_Map									m_PackStoreParamMap;
};

#endif