/*=================================================================*/
/*研发团队  : 深圳市新飞扬
/* 文件名   : MapServer_Protocol.h
/* 文件描述 : 游戏服务器与客户端通信结构体定义
/* 创建时间 : 2008-3-27
/* 创建者   : 曾庆辉
/* 备注     :
/*
/*-----------------------------------------------------------------*/
/* 修改记录1:
/* 修改时间 :
/* . . .
/*=================================================================*/
#include <memory.h>
#include <string.h>
#include "Distributed_Header.h"
#include "Global_Macro.h"

#pragma warning(disable:4100)
#ifndef GAME_STRUCT_H
#define GAME_STRUCT_H

#define INVALID_USER_ID								(~0)						//无效用户ID

#define	MAX_CONSTRUCTION_NUM						12							//建筑物
#define MAX_CONSTRUCTION_LEVEL						20							//建筑物等级

#define	MAX_WEAPON_NUM								11							//武器数量
#define	MAX_DEFEND_NUM								3							//防具数量
#define	MAX_ANIMALS_NUM								5							//野兽数量
#define	MAX_SPECIAL_TOOL_NUM						2							//特殊工具数量

#define	MAX_WEAPON_LEVEL							5							//武器等级
#define	MAX_DEFEND_LEVEL							5							//防具等级
#define	MAX_ANIMALS_LEVEL							5							//野兽等级
#define MAX_SPECIAL_LEVEL							3							//特殊工具等级

#pragma pack(push,1)

/*********************************************************************/
//地图类型
enum MapType
{
	Country_Map = 0,//郡地图
	Ectype_Map = 1//副本地图
};
//错误信息码
enum ERROR_INFO_CODE
{
	EN_SUCCESSED = 0,									//成功
	EN_ERROR_START	= 0x1310,
	
	EN_FOOD_COUNT,										//粮食   数量不满足条件
	EN_WOOD_COUNT,										//木材
	EN_IRON_COUNT,										//铁
	EN_SKIN_COUNT,										//皮
	EN_MONEY_COUNT,										//金钱
	EN_SCIENCE_COUNT,									//科技点
	EN_RED_STONE_COUNT,									//红宝石
	EN_YELLOW_STONE_COUNT,								//黄宝石
	EN_GREEN_STONE_COUNT,								//绿宝石
	EN_BACK_IRON_COUNT,									//黑铁

	EN_COUNT_EEROR,										//输入的数量不合法
	EN_COMPLETE_TIME_ERROR,								//完成时间
	EN_LEVEL_UPPER,										//等级超出范围

	EN_DATABASE_CONNECT_FAILED,							//数据库连接失败
	EN_RECRUIT_OFFICER_PLAYERID,						//招募指挥官时,输入的玩家ID不存在数据库中
	EN_OFFICER_COUNT_UPPER,								//此玩家的指挥官数已达到上限,不能再招募指挥官
	
	EN_CREATE_ARMY_RACE,								//兵种  数量不够  创建部队
	EN_CREATE_ARMY_WEAPON,								//武器  数量不够 
	EN_CREATE_ARMY_DEFEND,								//防具  数量不够 
	EN_CREATE_ARMY_JOB,									//种族跟职业不匹配
	EN_CREATE_ARMY_COUNT,								//数量超过上下限

	EN_OFFICER_ID_ERROR,								//指挥官ID非法
	EN_FIND_OFFICER_ERROR,								//在表在找不到这个指挥官

	EN_TECH_ID_ERROR,									//技能ID 非法
	EN_TECH_COUNT_UPPER,								//目前不能再学习技能了

	EN_DATA_ERROR,										//数据库没有此数据

	EN_CONSTRUCTION_UPPER,								//建筑物数量超出上限不能再建造

	EN_PRODUCE_ITEM_UPPER,								//生产道具 在列队中超出上限 暂时不能再生产此类道具
	EN_ITEM_TYPE_ERROR,									//道具类型不是在有效范围内

	EN_CLIENT_DATA_ERROR,								//客户端发送过来的的数据无效

	EN_RACE_COUNT,										//目前城市中此种族数不够        招兵
	EN_LEVY_FOOD_COUNT,									//粮食不够
	EN_LEVY_MONEY,										//金钱不够
	EN_LEVY_RACE_ID,									//种族ID不正确
	EN_LEVY_COUNT,										//数量超出上限
	EN_OPERATE_DBG_FAILED,								//操作DBG失败

	EN_ITEM_ID_ERROR,									//道具ID类型不在有效范围内

	EN_ARMY_TRANSFER,									//部队转移失败

	EN_COMEIN_NEW_COUNTRY,								//进入新手村
	EN_USER_ACCOUNTS_ERROR,								//用户帐号不正确

	EN_BUILD_CONSTRUCTION_ERROR,						//建造对应的建筑物 才可生产
	EN_RESEARCH_ITEM_ERROR,								//科研对应的科研技能 才可生产
	EN_RESEARCH_ITEM_LOWER,								//首先需要科研底级的科研技能
	EN_NO_BULID_CASERN,									//首先需要建造兵营才能征兵

	EN_RECRUITING_HERO,									//正在招募英雄...
	EN_NOT_HERO_ID,										//没有此英雄ID
	EN_CANNOT_DISMISS_HEADER_HERO,						//不能开除首领英雄
	EN_HERO_LEISURE,									//空闲 
	EN_HERO_NOT_LEISURE,								//非空闲 
	EN_HERO_HAVE_ARMY,									//带队
	EN_HERO_GBH,										// 重伤 
	EN_HEADER_HERO_BY_CAPTIVE,							//首领英雄被俘 
	EN_HERO_BY_CAPTIVE,									//被俘 
	EN_HERO_CAPTIVE,									//俘虏
	EN_TECH_DOT_NOT,									//技能点不够
	EN_SCIENCE_DOT_NOT,									//科技点不够
	EN_LEVEL_NOT_ENOUGH,								//英雄等级不满足需求
	EN_STAMINA_NOT_ENOUGH,								//英雄耐力不够
	EN_POWER_NOT_ENOUGH,								//英雄力量不够
	EN_CELERITY_NOT_ENOUGH,								//英雄敏捷不够
	EN_WIT_NOT_ENOUGH,									//英雄智力不够

	EN_NOT_ARMY_ID,										//部队ID不存在
	EN_NOT_TRANSIT_ARMY,								//不是运输部队
	EN_NOT_COLONIZE_ARMY,								//不是殖民部队
	EN_ARMY_NO_CONTENT,									//部队容量不够
	EN_CITY_NO_CONTENT,									//城市容量不够
	EN_NOT_SELF_MAP,									//非本方郡
	EN_HAVE_CITY,										//已郡内已有城市
	EN_COLONIZE_CITY_FAILED,							//殖民城市失败
	EN_COLONIZE_CITY,									//殖民城市ok
	EN_CONTRIBUTE_OK,									//捐献ok
	EN_CONTRIBUTE_1000,									//捐献金钱不能少于1000
	EN_COLONIZE_UP_10,									//殖民城市时,城市数已有10个
	EN_MAX_ARMYCOUNT,									//已到郡守备队数量上限
	EN_NOT_CREATGARRISON_INSAFEMAP,						//安全村不能创建郡守备队
	EN_NOT_REACHMAP,									//无法到达（出征）
	EN_NOT_ACCTIONDOT,									//行动值不足
	EN_PACKFULL,										//背包已满
	EN_JOBNOMATCHING,									//职业不匹配
	EN_SKILL_CANTSTUDY,									//非可学习技能
	EN_MAP_INWAR,										//郡在战斗状态中，无法建造、生产
	EN_OFFICER_INWAR,									//英雄在战斗中，不能操作装备和技能
	EN_PACKPOS_HAVEITEM,								//新位置已有物品占据
	EN_NO_ExpeditionSpeedCard,							//无加速卡,加速出征失败
	EN_OFFICER_NOSKILL,									//英雄无可清除技能
	EN_NO_HAVECARD,										//卡不存在
	EN_HERONAMEEXIST,									//英雄名已存在
	EN_HAVEDRESSEQUIPOFPOSTION,							//穿装备时，英雄已穿上该类型的装备
};
//建筑,生产,科研成功信息码
enum SUCCEED_INFO_CODE
{
	EN_SUCCEED_START = 0x1411,

	EN_PRODUCE_WEAPON,									//生产武器成功
	EN_PRODUCE_DEFEND,									//生产防具
	EN_PRODUCE_ANIMAL,									//生产驯兽技术
	EN_PRODUCE_SPECIAL,									//生产特殊工具

	EN_RESEARCH_WEAPON,									//科研武器成功
	EN_RESEARCH_DEFEND,									//科研防具
	EN_RESEARCH_ANIMAL,									//科研驯兽技术
	EN_RESEARCH_SPECIAL,								//科研特殊技术

	EN_BUILD_CONSTRUCTION,								//建筑成功
	EN_CANCEL_BUILD_CONSTRUCTION,						//取消建筑物建筑
	EN_DEMOLISH_CONSTRUCTION,							//拆毁建筑物
	EN_DEMOLISH_A_CONSTRUCTION,							//拆毁A建筑物

	EN_BUILDING_CON,									//此建筑物正在建造中
	EN_PRODUCEING_ITEM,									//此道具正在生产中
	EN_RESEARCHING_ITEM,								//此道具技术正在科研

	EN_START_BUILD_CON,									//开始建筑物建造
	EN_START_PRODUCE,									//开始生产
	EN_START_RESEARCH,									//开始科研

	EN_WAIT_BUILD_CON,									//此物建造正处在等待中...
	EN_WAIT_PRODUCE_ITEM,								//此类生产道具进入等待列队中...
	EN_WAIT_RESEARCH_ITEM,								//此科研技术进入等待列队中...
	
	EN_CANCEL_PRODUCE_ITEM,								//取消生产道具

	EN_CANCEL_RESEARCH_ITEM,							//取消科研道具

	EN_SUCCEED_END = 0x1500
};
//市场信息码
enum MARKET_INFO_CODE
{
	EN_MARKET_INFO_START = 0x1501,
	EN_GOODS_SALED,										//此种物品已出售或已过期
	EN_GOODS_PRICE,										//此种物品价格已改变,请及时更新
	EN_GOODS_COUNT,										//此种物品数量不够
	EN_GOODS_BASE_PRICE,								//此种物品底价不正确
	EN_GOODS_ONCE_PRICE,								//此种物品一口价不正确
	EN_GOODS_BIDDING_FAILED,							//此种物品出价有误
	EN_GOODS_BIDDING_SUCCEED,							//此种物品出价成功
	EN_GOODS_HANG_SUCCEED,								//此种物品挂牌成功
	EN_GOODS_HANG_FAILED,								//此种物品挂牌失败
	EN_GOODS_NOT_ENOUGHT_GOLD,							//购买此种物品时,金钱不够
	EN_GOODS_BUY_SUCCEED,								//交易成功
	EN_GOODS_BUY_FAILED,								//交易失败
	EN_GOODS_SALE_SUCCEED,								//卖  交易成功
	EN_GOODS_SALE_FAILED,								//卖  交易失败
	EN_GOODS_COUNT_ERROR,								//输入的卖买数量非法
	EN_BUY_SUCCEED,										//买成功
	EN_BUY_FAILED,										//买失败
		
	EN_ALLOW_BIDDEN,									//不允许连续出价
	EN_BIDDEN_ERROR,									//出价小于当前价
	EN_BIDDEN_OWN,										//不能给自己的物品出价
	EN_STORAGE_CONTENT_ERROR,							//此城市内的仓库容量不够
	EN_NOT_CANSALE,										//不能卖
	EN_MARKET_INFO_END = 0x1520
};
//Begin 郡，城市编号

//所有的编号参照:  郡命名.xls

//郡地图编号(Laiyi)
enum COUNTY_NO
{
	COUNTY_UNION_NEW=1001,   //联盟新手村
	COUNTY_CLAN_NEW =1004,   //部落新手村
	COUNTY_WAR_SMZL =1003,   //交战区(神秘之林)
   //...


};

//所有的城市编号 
enum CITY_NO
{
	//联盟新手村(1001)中的6个城市
	CITY_UNION_NEW1 =1,
	CITY_UNION_NEW2 =2,
	CITY_UNION_NEW3 =3,
	CITY_UNION_NEW4 =4,
	CITY_UNION_NEW5 =5,
	CITY_UNION_NEW6 =6,
	
    //部落新手村(1002)城市
	CITY_CLAN_NEW1 = 7,
	CITY_CLAN_NEW2 = 8,
	CITY_CLAN_NEW3 = 9,
	CITY_CLAN_NEW4 = 10,
	CITY_CLAN_NEW5 = 11,
	CITY_CLAN_NEW6 = 12,

	//....

};
//End 郡，城市编号
//*****************************************************************/

/**********************************************************************************************
//大地图服务器结构体
**********************************************************************************************/
struct InlayInfo
{
private:
	unsigned int ItemID:24;
	unsigned int Inlay0:8;
	unsigned int Inlay1:8;
	unsigned int Inlay2:8;
	unsigned int Inlay3:8;
	unsigned int Inlay4:8;
public:
	void SetItemID(int itemid)
	{
		Inlay0 = Inlay1 = Inlay2 = Inlay3 = Inlay4 = 0;
		ItemID = itemid;
	}
	unsigned int GetItemID() const
	{
		return ItemID;
	}
	unsigned int GetInlayCount() const
	{
		int count = 0;
		if (Inlay0>0)
		{
			count++;
		}
		if (Inlay1>0)
		{
			count++;
		}
		if (Inlay2>0)
		{
			count++;
		}
		if (Inlay3>0)
		{
			count++;
		}
		if (Inlay4>0)
		{
			count++;
		}
		return count;
	}
	void GetInlayInfo(int *addinfo)
	{
		addinfo[0]=Inlay0;
		addinfo[1]=Inlay1;
		addinfo[2]=Inlay2;
		addinfo[3]=Inlay3;
		addinfo[4]=Inlay4;
	}
	int Inlay(int itemid)
	{
		if(Inlay0 <=0)
		{
			Inlay0 = itemid;
			return 1;
		}
		if(Inlay1 <=0)
		{
			Inlay1 = itemid;
			return 2;
		}
		if(Inlay2 <=0)
		{
			Inlay2 = itemid;
			return 3;
		}
		if(Inlay3 <=0)
		{
			Inlay3 = itemid;
			return 4;
		}
		if(Inlay4 <=0)
		{
			Inlay4 = itemid;
			return 5;
		}
		return 0;
	}
};
union Inlay_Union
{
	__int64 NewItemID;
	InlayInfo DetailInfo;
};
//游戏中的时间
struct GAME_TIME : DISTRIBUTED_HEADER
{
	inline void serialize(long long lGameTime, int from, int to, int command);
	long long lGameTime;//游戏中的时间(单位:秒)
};
//部队达到
struct ARMY_REACH
{
	unsigned long			lMapID;						//郡ID
	unsigned long			lArmyID;					//部队ID
};

//基本信息
struct PLAYER_BASE_INFO : DISTRIBUTED_HEADER
{
	inline void serialize(unsigned long lPlayerID, char Name[], int iFace, unsigned long lConsortiaID, long lExp, int iRaceID, 
		int iJobID, int iCredit, int iLevel, int iMilitaryRank, unsigned long lGameTime,int iCountry, long lScience, long lMoney,
		int from=0, int to=0);

	unsigned long		lPlayerID;						//玩家ID
	char				szNick[SIZE_32];				//昵称
	int					iFace;							//头像
	unsigned long		lConsortiaID;					//军团ID
	long				lExp;							//经验值
	int					iRaceID;						//种族
	int					iJobID;							//职系
	int					iCredit;						//荣誉点
	int					iLevel;							//等级
	int					iMilitaryRank;					//军衔
	unsigned long		lGameTime;						//游戏时间
	int					iCountry;						//1:联盟  2: 部落
	long				lScience;						//科技点
	long				lMoney;							//金钱
	
	void operator = (PLAYER_BASE_INFO& pInfo)   //运算符=的重载  Laiyi  
	{
		this->lPlayerID=pInfo.lPlayerID;
		strcpy(this->szNick,pInfo.szNick);
		this->iFace=pInfo.iFace;
		this->lConsortiaID=pInfo.lConsortiaID;
		this->lExp=pInfo.lExp;
		this->iRaceID=pInfo.iRaceID;
		this->iJobID=pInfo.iJobID;
		this->iCredit=pInfo.iCredit;
		this->iLevel=pInfo.iLevel;
		this->iMilitaryRank=pInfo.iMilitaryRank;
		this->lGameTime=pInfo.lGameTime;
		this->lScience=pInfo.lScience;
		this->lMoney=pInfo.lMoney;
		
		//基类成员
		strcpy(this->http_tunnel,pInfo.http_tunnel);
		this->length=pInfo.length;
		this->agent=pInfo.agent;
		this->from=pInfo.from;
		this->to=pInfo.to;
		this->command=pInfo.command;
		
	
	}

};
//城市列表信息
typedef struct
{
	short							iTypeID;				//国家ID
	unsigned long					lMapID;					//郡ID
	unsigned long					lCityID;				//城市ID
	bool							bState;					//状态
} tagCityList;
//郡状态信息
struct tagMapState
{
	unsigned long lMapID;									//郡ID
	int	iCountry;											//国家ID  //1:联盟  2: 部落
	int iState;												//状态 //1: 战斗状态
};
struct MAP_STATE_LIST : DISTRIBUTED_HEADER
{
	unsigned int iCount;
	inline void serialize(int count, int from, int to, int command=0);
	inline tagMapState & operator [](unsigned int);
};
struct CITY_LIST : DISTRIBUTED_HEADER
{
	unsigned int iCount;
	inline void serialize(int count, int from, int to);
	inline tagCityList & operator [](unsigned int);
};
//密钥验证
struct MAP_SERVER_VALIDATE : DISTRIBUTED_HEADER
{
	inline void serialize(char const szAccounts[], char const szValidate[], int from=0, int to=0);
	char				szAccounts[SIZE_16];		//账号
	char				szValidate[SIZE_16];		//验证码
};
//玩家城市改变
struct PLAYER_CITY_MODFIY : DISTRIBUTED_HEADER
{
	inline void serialize(unsigned long lMapID, unsigned long lCityID, int from=0, int to=0);
	unsigned long					lMapID;					//郡ID
	unsigned long					lCityID;				//城市ID
	int								iState;					//1:得到一个城市,0:失去一个城市
};
//玩家城市列表信息
struct tagPlayerCityList
{
	unsigned long					lMapID;					//郡ID
	unsigned long					lCityID;				//城市ID
	short							lState;					//城市状态，100：新手村
};
struct PLAYER_CITY_LIST : DISTRIBUTED_HEADER
{
	unsigned int iCount;
	inline void serialize(int count, int from, int to);
	inline tagPlayerCityList & operator [](unsigned int);
};

//郡内哨塔等属性
struct tagCityAttributeList
{
	int									iConID;								//建筑物ID
	int									iLevel;								//等级
	int									iContribute;						//捐献度
	int									ihp;								//HP
	int									iRecovery;							//防御
	int									iAttack;							//攻击
	int									iX;									//建筑物坐标
	int									iY;									//
};
struct CITY_ATTRIBUTE_LIST : DISTRIBUTED_HEADER
{
	unsigned int iCount;
	inline void serialize(int count, int from, int to);
	inline tagCityAttributeList & operator [](unsigned int);
};
//城市信息
struct CITY_INFO : DISTRIBUTED_HEADER
{
	unsigned long					lMapID;					//郡ID
	unsigned long					lCityID;				//城市ID
	inline void serialize(unsigned long lMapID, unsigned long lCityID,int from=0, int to=0, int command=0);
};
//城市信息
struct REQ_CITY_INFO : DISTRIBUTED_HEADER
{
	int								ectypeID;				//副本ID 
	unsigned long					lMapID;					//郡ID
	unsigned long					lCityID;				//城市ID
	inline void serialize(unsigned long lMapID, unsigned long lCityID,int from=0, int to=0, int command=0);
};
//进入郡
struct COMEIN_MAP : DISTRIBUTED_HEADER
{
	unsigned long					lMapID;					//郡ID
	inline void serialize(unsigned long lMapID, int from=0, int to=0);
};
//玩家的兵种数
struct PLAYER_SOLDIER_LIST : DISTRIBUTED_HEADER
{
	inline void serialize(int iHuman, int iFairy, int iMannikin, int iDandiprat, int iHalfAnimal, int iCattle, int iBlackFairy, int iDecPerson, int from, int to);
	int								iHuman;					//人类
	int								iFairy;					//精灵
	int								iMannikin;				//矮人
	int								iDandiprat;				//侏儒
	int								iHalfAnimal;			//半兽人
	int								iCattle;				//牛头人
	int								iBlackFairy;			//黑精灵
	int								iDecPerson;				//亡灵

};
//玩家的资源量
struct PALEYER_RESOURCE : DISTRIBUTED_HEADER
{
	inline void serialize(int iFoodCount,int iWoodCount,int iIronCount,int iSkinCount,int iRedStoneCount,
		int iYellowStoneCount,int iGreenStoneCount,int iBackIronCount,int iScience,int iMoney, int iContribute,
		int content,int from/*=0*/, int to/*=0*/);
	int								iFoodCount;				//粮食数量
	int								iWoodCount;				//木材数量
	int								iIronCount;				//铁数量
	int								iSkinCount;				//皮数量
	int								iRedStoneCount;			//红宝石
	int								iYellowStoneCount;		//黄宝石
	int								iGreenStoneCount;		//绿宝石
	int								iBackIronCount;			//黑铁
	int								iMoney;					//
	int								iScience;				//科技点
	int								iContribute;			//捐献度
	int								iContent;				//仓库剩余容量
};
//玩家的基本资源
struct PALEYER_RESOURCE_LIST : DISTRIBUTED_HEADER
{
	inline void serialize(int iFoodCount,int iWoodCount,int iIronCount,int iSkinCount,int iRedStoneCount,
		int iYellowStoneCount,int iGreenStoneCount,int iBackIronCount,int content,int from, int to);
	int								iFoodCount;				//粮食数量
	int								iWoodCount;				//木材数量
	int								iIronCount;				//铁数量
	int								iSkinCount;				//皮数量
	int								iRedStoneCount;			//红宝石
	int								iYellowStoneCount;		//黄宝石
	int								iGreenStoneCount;		//绿宝石
	int								iBackIronCount;			//黑铁
	int								iContent;				//仓库剩余容量
};
//征兵数量
struct LEVY_SOLDIER_COUNT : DISTRIBUTED_HEADER
{
	inline void serialize(unsigned long lMapID,unsigned long lCityID,int iRace,int iCount,int from=0, int to=0);
	unsigned long						lMapID;					//郡ID
	unsigned long						lCityID;				//城市ID
	int									iRace;					//种族ID
	int									iCount;					//数量
};
//进入征兵状态中
struct LEVY_SOLDIER_INFO : DISTRIBUTED_HEADER
{
	inline void serialize(int iRace,int iCount, int iState, int from, int to);
	int									iRace;					//种族ID
	int									iCount;					//数量
	int									iState;					//征兵状态 1:正处于招兵 2: 等待中...
};
//郡内指挥官资源总量
struct Officer_Resource_Info
{
	unsigned long 	lPlayerID;					//玩家ID
	char			Accounts[SIZE_32];			//昵称
	long int		lResource;					//资源总量

	Officer_Resource_Info()
	{
		InitData();
	}
	void InitData()
	{
		memset(this,0,sizeof(Officer_Resource_Info));
		lPlayerID = (unsigned long)INVALID_USER_ID;
	}
};

//获取郡地图资源信息
struct tagGet_Map_Info
{
	unsigned long					lMapID;					//郡ID
};

//郡地图资源信息
struct tagSend_Map_Info
{
	int					iFood;					//粮食
	int					iWood;					//木材
	int					iIron;					//铁
	int					iSkin;					//皮
};

//城市信息
struct MAP_CITY_INFO : DISTRIBUTED_HEADER
{
	inline void serialize(int iFoodCount,int iWoodCount,int iIronCount,int iSkinCount,
		int iHuman,int iMannikin,int iFairy,int iDandiprat,int iHalfAnimal,int iCattle,int iBlackFairy,int iDecPerson,
		int from=0, int to=0);
	int										iFoodCount;							//粮食
	int										iWoodCount;							//木材
	int										iIronCount;							//铁
	int										iSkinCount;							//皮

//	int										iMoney;
	int										iHuman;					//人类
	int										iFairy;					//精灵
	int										iMannikin;				//矮人
	int										iDandiprat;				//侏儒
	int										iHalfAnimal;			//半兽人
	int										iCattle;				//牛头人
	int										iBlackFairy;			//黑精灵
	int										iDecPerson;				//亡灵
};

//英雄系统信息
struct HERO_SYSTEM_INFO : DISTRIBUTED_HEADER
{
	inline void serialize(int iCode, int from, int to, int command);
	int iCode;//信息码
};
//创建指挥官
struct CREATE_OFFICER : DISTRIBUTED_HEADER
{
	inline void serialize(char szNick[], int iFace, int iCountry, int iRaceID, int iJobID, int from=0, int to=0);
	char				szNick[SIZE_16];				//昵称
	int					iFace;							//头像
	int					iCountry;						//国家
	int					iRaceID;						//种族 1:人类;2:精灵;3:矮人;4:侏儒;5:半兽人;6:牛头人;7:黑精灵;8:亡灵;
	int					iJobID;							//职业: 1:圣骑士,2:战士,3:弓箭手,4:法师,5:牧师,
};
//创建指挥官失败
struct CREATE_OFFICER_FAILD : DISTRIBUTED_HEADER
{
	inline void serialize(int iError, int from=0, int to=0, int command=0);
	int					iError;							//错误码 1:国家ID不在有效范围内 2: 种族ID不在有效范围内 3:英雄名字已被占用 5: 插入数据失败 6:职业ID不在有效范围内.7:种族与职业不对应
};

//开除英雄
struct DISMISS_HERO : DISTRIBUTED_HEADER
{
	inline void serialize(unsigned long lHeroID, int from, int to);
	unsigned long lHeroID;//英雄ID
};
//开除英雄信息
struct DISMISS_HERO_INFO : DISTRIBUTED_HEADER
{
	inline void serialize(int iCode, int from, int to, int command);
	int iCode;//信息码
};
//获取部队信息
struct GET_ARMY_INFO : DISTRIBUTED_HEADER
{
	inline void serialize(unsigned long lMapID, unsigned long lCityID, unsigned long lArmyID,int from=0, int to=0);
	unsigned long						lMapID;								//郡ID
	unsigned long						lCityID;							//城市ID
	unsigned long						lArmyID;							//部队ID
};
//玩家指挥官列表信息
struct tagPlayer_Officer_List
{
	unsigned long lOfficerID;
	char szOfficerName[SIZE_32];
	int									iLevel;
	int									iFaceID;						//头像
	int									iRaceID;						//种族
	int									iJobID;							//职系
	int									iMilitaryRank;					//军衔
	int									iState;							//状态 0:没有带部队 1:已有部队 2,重伤 3,被俘 4 俘虏 (参见 结构体HERO_STATE)
	int									iTechDon;						//技能
	int									iType;							//操作类型 0:已招募的英雄 1:正在招募中的英雄;....
	long								lTime;							//还差多长时间完成
};
struct PLAYER_OFFICER_LIST : DISTRIBUTED_HEADER
{
	unsigned int iCount;
	inline void serialize(int count, int from=0, int to=0, int command=0);
	inline tagPlayer_Officer_List & operator [](unsigned int);
};
//创建部队
struct CREATE_ARMY : DISTRIBUTED_HEADER
{
	inline void serialize(unsigned long lMapID, unsigned long lCityID, unsigned long lOfficerID, char szName[],int iRaceID, int iSoldiertype, int iFSoldierCount, 
		int iFWeaponID, int iWeaponLevel,int iDefendID, int iDefendLevel,  int iX, int iY, int from=0, int to=0);
	unsigned long							lMapID;					//郡ID
	unsigned long							lCityID;					//城市ID
	unsigned long							lOfficerID;				//指挥官

	char									szName[SIZE_32];		//部队名称
	int										iRaceID;				//种族
	int										iSoldiertype;			//职业
	int										iFSoldierCount;			//数量
	int										iFWeaponID;				//武器
	int										iWeaponLevel;			//武器级别
	int										iDefendID;				//防具
	int										iDefendLevel;			//防具级别
	int										iX;						//
	int										iY;
};
//创建部队失败
struct CREATE_ARMY_FAILD : DISTRIBUTED_HEADER
{
	inline void serialize(int iError, int from=0, int to=0, int command=0);
	int										iError;						//1:种族; 2:武器; 3:防具;数量不够 4:种族,武器不匹配;5:数量超过上下限,6:指挥官ID 非法,7 武器ID 非法 8 防具ID 非法 9:武器专精技能级别不够（或为学）；10：防具专精技能级别不够（或为学）;
};
//组编部队
struct EDIT_ARMY : DISTRIBUTED_HEADER
{
	inline void serialize(unsigned long lMapID, unsigned long lCityID, unsigned long lOfficerID, 
		int iArmyID, int iDefendID, int iCount, int from=0, int to=0);
	unsigned long							lMapID;						//郡ID
	unsigned long							lCityID;					//城市ID
	unsigned long							lOfficerID;					//现在的指挥官ID
	int										iArmyID;					//部队ID
	int										iDefendID;					//防具ID
	int										iCount;						//数量
	char									szName[SIZE_32];			//新部队名称
};

//解散部队EN_C_UNLAYARMY,EN_S_UNLAYARMY,
typedef struct tagUnlayArmy_Request:DISTRIBUTED_HEADER
{
	//int MapID;							//当前郡ID
	int ArmyID;							//部队ID
	inline void serialize(int playerid,int mapid,int armyid);
}UnlayArmy_Request,*LPUnlayArmy_Request;

typedef struct tagUnlayArmy_Response:DISTRIBUTED_HEADER
{
	int Error_Code;						//0:成功；1：没有本人据点，无法解散；2：战斗状态中，无法解散；3:部队不存在；4:仓库已满，部分武器装备被丢弃；5:运输部队装载有物质，不能解散；其他：失败
	inline void serialize(int agent,int mapid,int playerid,int code);
}UnlayArmy_Response,*LPUnlayArmy_Response;

//删除部队EN_C_DELETEARMY,EN_S_DELETEARMY,
typedef struct tagDeleteArmy_Request:DISTRIBUTED_HEADER
{
	//int MapID;						//所在郡ID
	int ArmyID;						//部队ID
	inline void serialize(int playerid,int mapid,int armyid);
}DeleteArmy_Request,*LPDeleteArmy_Request;

typedef struct tagDeleteArmy_Response:DISTRIBUTED_HEADER
{
	int Error_Code;					//0:成功；1:部队不存在；其他：失败
	inline void serialize(int agent,int mapid,int playerid,int code);
}DeleteArmy_Response,*LPDeleteArmy_Response;

//部队出征列表
struct ARMY_OUT_LIST : DISTRIBUTED_HEADER
{
	inline void serialize(unsigned long lMapID, unsigned long lCityID, unsigned long lTargetMapID, unsigned long  iArmyID, int from=0, int to=0);
	unsigned long							lMapID;						//出征处郡ID
	unsigned long							lCityID;					//出征处城市ID
	unsigned long							lTargetMapID;				//到达目标郡ID
	unsigned long							iArmyID;					//部队ID 
	//int										iArmy2ID;					//部队ID
	//int										iArmy3ID;					//部队ID
	//int										iArmy4ID;					//部队ID
	//int										iArmy5ID;					//部队ID
};
// 部队行走路径
struct tagArmyMovePath 
{
	int									iRMapID;				//当前起始郡ID
	int									iDMapID;				//当前达到郡ID
	int									iAllTime;				//总时间 
	int									iTime;					//还要多长时间达到目标郡
};
//正处于出征中的部队列表
struct tagOutingArmysList
{
	char						szArmyName[SIZE_32];	//部队名称
	short						iRaceID;				//种族
	short						iSoldierType;			//
	short						iSoldierCount;			//
	unsigned long				lOfficerID;				//指挥官ID
	char						szOfficerName[SIZE_32];	//指挥官名称
	unsigned long				lArmyID;				//部队ID
	short						iItem;					//经过郡数量
	short						iFMapID;				//出发郡ID
	short						iTMapID;				//目标郡ID
	tagArmyMovePath				Path[50];				//每个部队所经过郡ID
};
struct ARMY_MOVE_PATH : DISTRIBUTED_HEADER
{
	int							iCount;					//部队数量
	inline void serialize(int count, int from, int to, int command);
	inline tagOutingArmysList & operator [](unsigned int);
};
//部队不能经过
struct ARMY_CANT_MOVE : DISTRIBUTED_HEADER
{
	inline void serialize(int iCode, int from, int to);
	int iCode;			//信息码 1 不能跨正处于战斗状态郡 2 不能跨敌郡
};
//部队转移
struct ARMY_TRANSFER : DISTRIBUTED_HEADER
{
	inline void serialize(unsigned long lRMapID, unsigned long lTargetMapID, unsigned long  lArmyID, int from=0, int to=0);
	unsigned long							lRMapID;					//转移起始郡ID
	unsigned long							lTargetMapID;				//到达目标郡ID
	unsigned long 							lArmyID;					//部队ID 
};
//拆毁建筑物
struct DEMOLISH_CONSTRUCTION : DISTRIBUTED_HEADER
{
	inline void serialize(unsigned long lMapID, unsigned long lCityID, unsigned long lPlayerID,int from=0, int to=0);
	unsigned long							lMapID;						//郡ID
	unsigned long							lCityID;					//城市ID
	unsigned long							lPlayerID;					//拆毁指定玩家的ID
};
//建筑物列表
struct tagConstructionList
{
	unsigned long							lConstructionID;			//建筑物ID
	int										iLevel;						//建筑等级
	int										iModify;					//产量
	int										iFoodCount;
	int										iWoodCount;
	int										iIronCount;
	int										iSkinCount;
	int										iScience;
	int										iMoney;
	int										iCompleteTime;				//建筑完成的时间
	int										bBuild;						//满足建筑条件
	int										iHave;						// 1: 此已有建筑物或已建筑好
};
struct CONSTRUCTION_LIST : DISTRIBUTED_HEADER
{
	unsigned int iCount;
	inline void serialize(int count, int from, int to);
	inline tagConstructionList & operator [](unsigned int);
};
//玩家已建筑好的建筑物
struct tagPlayerConstrList
{
	unsigned long							lSortID;					//ID  如果是建筑 此值为:0 否则是从服务器转过来的那个值
	unsigned long							lConstrID;					//建筑物ID
	int										iLevel;						//建筑等级
	int										iUpGrade;					//1 : 满足升级条件
};
//玩家已建筑好的建筑物
struct PLAYER_CONSTR_LIST : DISTRIBUTED_HEADER
{
	unsigned int iCount;
	inline void serialize(int count, int from, int to);
	inline tagPlayerConstrList & operator [](unsigned int);
};
//玩家正在建筑中的建筑物
struct tagBuildingConstrList
{
	unsigned long							lSortID;
	unsigned long							lConstrID;					//建筑物ID
	int										iLevel;						//建筑等级
	int										iTime;						//还需要多少时间完成 秒
};
//玩家正在建筑中的建筑物
struct BUILDING_CONSTR_LIST : DISTRIBUTED_HEADER
{
	unsigned int iCount;
	inline void serialize(int count, int from, int to);
	inline tagBuildingConstrList & operator [](unsigned int);
};
//取消建筑物建筑
struct CANCEL_BUILD : DISTRIBUTED_HEADER
{
	inline void serialize(unsigned long lMapID, unsigned long lCityID, unsigned long lSortID,unsigned long lConID,int iLevel, int from=0, int to=0);
	unsigned long							lSortID;
	unsigned long							lMapID;						//郡ID
	unsigned long							lCityID;					//城市ID
	unsigned long							lConID;						//建筑物ID
	int										iLevel;						//建筑等级
};
//取消生产道具
struct CANCEL_PRODUCE : DISTRIBUTED_HEADER
{
	inline void serialize(unsigned long lMapID, unsigned long lCityID, unsigned long lSortID,int iType, int iItemID,int iLevel, int iCount, int from=0, int to=0);
	unsigned long							lMapID;						//郡ID
	unsigned long							lCityID;					//城市ID
	unsigned long							lSortID;					//流水号
	int										iType;						//道具类型 1,武器,2,防具,3,野兽驯养,4,特殊工具
	int										iItemID;					//道具ID
	int										iLevel;						//道具等级
	int										iCount;						//数量
};
//取消科研道具
struct CANCEL_RESEARCH : DISTRIBUTED_HEADER
{
	inline void serialize(int iType, int iItemID,int iLevel,int from=0, int to=0);
	int										iType;						//道具类型 1,武器,2,防具,3,野兽驯养,4,特殊工具
	int										iItemID;					//道具ID
	int										iLevel;						//道具等级
}; 
//广播捐献信息
struct BROADCAST_CONTRIBUTE : DISTRIBUTED_HEADER
{
	inline void serialize(char szPlayerName[], unsigned long lMapID,unsigned long lCityID, int iLevel, int iType, int iMoney,int iCredit, int from=0, int to=0);
	char									szPlayerName[SIZE_24];
	unsigned long							lMapID;						//郡ID
	unsigned long							lCityID;					//城市ID
	int										iLevel;						//建筑物等级
	int										iType;						//建筑物类型
	int										iMoney;						//金钱 
	int										iCredit;					//荣誉点
};
//捐献信息
struct CONTRIBUTE_INFO : DISTRIBUTED_HEADER
{
	inline void serialize(unsigned long lCityID, int iLevel, int iCurMoney, int iTolMoney, int iAttack, int iHP, int iAttackScop, int iVisualRange, long lCMoney, long lPMoney, int from, int to);
	unsigned long lCityID;
	int iLevel;
	int iCurMoney;
	int iTolMoney;
	int iAttack;
	int iHP;
	int iAttackScop;
	int iVisualRange;
	long lCMoney;
	long lPMoney;
};
//捐献
struct PLAYER_CONTRIBUTE : DISTRIBUTED_HEADER
{
	inline void serialize(unsigned long lMapID,unsigned long lCityID, int iMoney, int iType, int from=0, int to=0);
	unsigned long							lMapID;						//郡ID
	unsigned long							lCityID;					//城市ID
	int										iType;						//建筑类型 国防军:-1
	int										iMoney;						//金钱 
};
//建筑建筑物
struct BUILD_CONSTRUCTION : DISTRIBUTED_HEADER
{
	inline void serialize(unsigned long lMapID,unsigned long lCityID,unsigned long lSortID,unsigned long lConstructionID,int iLevel,int from=0, int to=0, int command=0);
	unsigned long							lMapID;						//郡ID
	unsigned long							lCityID;					//城市ID
	unsigned long							lSortID;					//ID  如果是建筑建筑物 此值为:0 否则(升级/拆毁)是从服务器转过来的那个值
	unsigned long							lConstructionID;			//建筑物ID
	int										iLevel;						//建筑等级
};
//研究
struct	ITEM_RESEARCH : DISTRIBUTED_HEADER
{
	inline void serialize(int from=0, int to=0, int command=0);
};
//道具研究
struct tagItemResearch
{
	int										iType;						//道具类型 1,武器,2,防具,3,野兽驯养,4,特殊工具
	int										iItemID;					//道具ID
	int										iLevel;						//道具等级
	int										iCount;						//数量
	int										bResearch;					//科研下一等级资源满足为 1
};
//已经研究了的道具列表
struct ITEM_RESEARCH_LIST : DISTRIBUTED_HEADER
{
	unsigned int iCount;
	inline void serialize(int count, int from=0, int to=0, int command=0);
	inline tagItemResearch & operator [](unsigned int);
};
//正在研究中的道具
struct tagResearchingItemList
{
	unsigned long							lSortID;
	int										iType;						//道具类型 1,武器,2,防具,3,野兽驯养,4,特殊工具
	int										iItemID;					//道具ID
	int										iLevel;						//道具等级
	int										iCompletetime;				//还需要多少时间完成时间  秒
};
//正在研究中的道具
struct RESEARCHING_ITEM_LIST : DISTRIBUTED_HEADER
{
	unsigned int iCount;
	inline void serialize(int count, int from=0, int to=0, int command=0);
	inline tagResearchingItemList & operator [](unsigned int);
};
//研究武器列表
struct RESEARCH_WEAPON_LIST : DISTRIBUTED_HEADER
{
	unsigned int iCount;
	inline void serialize(int count, int from=0, int to=0);
	inline tagItemResearch & operator [](unsigned int);
};
//研究道具列表
struct RESEARCH_DEFEND_LIST : DISTRIBUTED_HEADER
{
	unsigned int iCount;
	inline void serialize(int count, int from=0, int to=0);
	inline tagItemResearch & operator [](unsigned int);
};
//研究驯兽技术列表
struct RESEARCH_ANIMAL_LIST : DISTRIBUTED_HEADER
{
	unsigned int iCount;
	inline void serialize(int count, int from=0, int to=0);
	inline tagItemResearch & operator [](unsigned int);
};
//研究特殊技术列表
struct RESEARCH_SPECIAL_LIST : DISTRIBUTED_HEADER
{
	unsigned int iCount;
	inline void serialize(int count, int from=0, int to=0);
	inline tagItemResearch & operator [](unsigned int);
};
//研究道具
struct RESEARCH_ITEM : DISTRIBUTED_HEADER
{
	inline void serialize(int iType, int iItemID, int iLevel, int from=0, int to=0, int command=0);
	int										iType;						//道具类型 1,武器,2,防具,3,野兽驯养,4,特殊工具
	int										iItemID;					//道具ID
	int										iLevel;						//道具等级
};
//正在生产中的道具
struct tagProduceingItemList
{
	unsigned long							lSortID;				//流水号
	int										iItemID;				//道具ID
	int										iLevel;					//等级
	int										iType;					//生产类型 1,武器,2,防具,3,野兽驯养,4,特殊工具
	int										iCount;					//数量
	int										iCompletetime;			//还需要多少时间完成时间  秒
};
//正在生产中的道具
struct PRODUCEING_ITEM_LIST : DISTRIBUTED_HEADER
{
	unsigned int iCount;
	inline void serialize(int count, int from, int to,int command);
	inline tagProduceingItemList & operator [](unsigned int);
};
//生产表
struct tagProduction
{
	int										iItemID;			//道具ID
	int										iLevel;					//等级
	int										iType;					//道具类型 1,武器,2,防具,3,野兽驯养,4,特殊工具
	int										iFoodCount;
	int										iWoodCount;
	int										iIronCount;
	int										iSkinCount;
	int										iRedStoneCount;			//红宝石
	int										iYellowStoneCount;		//黄宝石
	int										iGreenStoneCount;		//绿宝石
	int										iBackIronCount;			//黑铁
	int										iMoney;
	int										iScience;				//科技点
	bool									bPro;					//满足生产条件
	int										iCompletetime;			//完成时间
};
//生产武器列表
struct PRODUCTION_WEAPON_LIST : DISTRIBUTED_HEADER
{
	unsigned int iCount;
	inline void serialize(int count, int from, int to);
	inline tagProduction & operator [](unsigned int);
};
//生产道具列表
struct PRODUCTION_DEFEND_LIST : DISTRIBUTED_HEADER
{
	unsigned int iCount;
	inline void serialize(int count, int from, int to);
	inline tagProduction & operator [](unsigned int);
};
//驯兽技术生产
struct PRODUCTION_ANIMAL_LIST : DISTRIBUTED_HEADER
{
	unsigned int iCount;
	inline void serialize(int count, int from, int to);
	inline tagProduction & operator [](unsigned int);
};
//特殊工具生产
struct PRODUCTION_SPECIAL_LIST : DISTRIBUTED_HEADER
{
	unsigned int iCount;
	inline void serialize(int count, int from, int to);
	inline tagProduction & operator [](unsigned int);
};
//生产道具
struct PRODUCT_ITEM : DISTRIBUTED_HEADER
{
	inline void serialize(unsigned long lMapID,unsigned long lCityID, int iType, int iItem, int iLevel, int iCount,	int from=0, int to=0, int command=0);
	unsigned long							lMapID;						//郡ID
	unsigned long							lCityID;					//城市ID
	int										iType;						//道具类型 1,武器,2,防具,3,野兽驯养,4,特殊工具
	int										iItem;						//道具ID
	int										iLevel;						//等级
	int										iCount;
};
//征兵参数
struct tagLevy_Soldier_Param
{
	int										iRaceType;
	int										iCurrentCount;
};
//征兵参数
struct LEVY_SOLDIER_PARAM : DISTRIBUTED_HEADER
{
	unsigned int iCount;
	inline void serialize(int count, int from, int to);
	inline tagLevy_Soldier_Param & operator [](unsigned int);
};
//正处于招兵中...
struct tagLevying_Soldier
{
	int									iRaceID;							//种族ID
	int									iCount;								//招兵数
	int									iState;								//状态 1: 正处于招募中... 2: 等待中...
	int									iTime;								//还有多长时间完成  秒
};
struct LEVYING_SOLDIER : DISTRIBUTED_HEADER
{
	unsigned int iCount;
	inline void serialize(int count, int from, int to);
	inline tagLevying_Soldier & operator [](unsigned int);
};
//道具列表
struct tagItem_List
{
	unsigned long							lMapID;						//郡ID
	unsigned long							lCityID;					//城市ID
	int										iItemID;
	int										iType;
	int										iCount;
};
//武器列表
struct WEAPON_LIST : DISTRIBUTED_HEADER
{
	unsigned int iCount;
	inline void serialize(int count, int from, int to, int command);
	inline tagItem_List & operator [](unsigned int);
};
//防具列表
struct DEFEND_LIST : DISTRIBUTED_HEADER
{
	unsigned int iCount;
	inline void serialize(int count, int from, int to, int command);
	inline tagItem_List & operator [](unsigned int);
};
//驯兽技术列表
struct ANIMAL_LIST : DISTRIBUTED_HEADER
{
	unsigned int iCount;
	inline void serialize(int count, int from, int to, int command);
	inline tagItem_List & operator [](unsigned int);
};
//特殊工具列表
struct SPECIAL_LIST : DISTRIBUTED_HEADER
{
	unsigned int iCount;
	inline void serialize(int count, int from, int to, int command);
	inline tagItem_List & operator [](unsigned int);
};
//市场物品买卖  
struct COUNTRY_MARKET_BUSINESS : DISTRIBUTED_HEADER
{
	inline void serialize(unsigned long lMapID, unsigned long lCityID, int iType, int iSaleID, int iLevel, int iCount, int iMode, int from=0, int to=0, int command=0);
	unsigned long							lMapID;						//郡ID
	unsigned long							lCityID;					//城市ID
	int										iType;						//买卖物品类型:1,武器; 2,防具;3,驯兽技术,4,特殊工具,5,英雄装备.6,基本物资 GOODS_TYPE
	int										iSaleID;					//类型为武器,防具时:iSaleID为武器,防具的ID; 基本物资时:1,粮食;2,木材;3,铁;4,皮 5,黄宝石,6,红宝石,7,绿宝石,8,黑铁
	int										iLevel;						//武器等级
	int										iCount;						//数量
	int										iMode;						//方式 1:卖; 2:买
};
//国家市场信息
struct COUNTRY_MARKET_INFO : DISTRIBUTED_HEADER
{
	inline void serialize(int from=0, int to=0, int command=0);
};
//市场物品信息
struct PEOPLE_MARKET : DISTRIBUTED_HEADER
{
	inline void serialize(unsigned long lSMapID, unsigned long lSCityID, int iType, int iMarketType, int from=0, int to=0, int command=0);	
	unsigned long							lSMapID;					//郡ID
	unsigned long							lSCityID;					//城市ID
	int										iType;						//物品类型:1,武器; 2,防具;3,驯兽技术,4,特殊工具,5,英雄装备.6,基本物资 GOODS_TYPE
	int										iMarketType;				//市场类型 1,国家市场,2,民间市场,3,拍卖
};
//物品挂牌
struct PUT_UP : DISTRIBUTED_HEADER
{
	inline void serialize(unsigned long lMapID, unsigned long lCityID, int iType, int iSaleID, unsigned long lSortID, 
		int iCount, int iMinPrice, int iOncePrice, int from=0, int to=0, int command=0);	
	unsigned long							lMapID;						//郡ID
	unsigned long							lCityID;					//城市ID
	int										iType;						//出售物品类型:1,武器; 2,防具;3,驯兽技术,4,特殊工具,5,英雄装备.6,基本物资 GOODS_TYPE
	int										iSaleID;					//类型为道具时:iSaleID为道具的ID; 材料时:1,粮食;2,木材;3,铁;4,皮;6,红宝石;5,黄宝石;7,绿宝石;8,黑铁 BASE_RESOURCE_TYPE;
	unsigned long							lSortID;					//英雄装备 流水号
	int										iCount;						//数量
	int										iMinPrice;					//最低价
	int										iOncePrice;					//一口价
};
//物品列表
struct tagGoodsList
{
	unsigned long							lSortID;					//流水号
	char									szPlayerName[SIZE_32];		//出售玩家
	__int64									iSaleID;					//类型为道具时:iSaleID为道具的ID; 材料时:1,粮食;2,木材;3,铁;4,皮;6,红宝石;5,黄宝石;7,绿宝石;8,黑铁 BASE_RESOURCE_TYPE
	int										iCount;						//数量
	int										iMinPrice;					//最低价
	int										iCurrentPrice;				//当前价
	int										iOncePrice;					//一口价
	int										iLeavingTime;				//剩余时间
	int										iTax;						//税收
};
//物品列表
struct GOODS_LIST : DISTRIBUTED_HEADER
{
	int iPage;				//页数(30条记录为1页)
	unsigned int iCount;	//记录条数
	int	iGoodsType;			//物品类型
	inline void serialize(int count, int iGoodsType, int iPage, int from, int to);
	inline tagGoodsList & operator [](unsigned int);
};
//出价
struct GOODS_BIDDEN : DISTRIBUTED_HEADER
{
	inline void serialize(unsigned long lMapID, unsigned long lCityID, int iType, unsigned long lSortID, int iCurrentPrice, int from=0, int to=0, int command=0);	
	unsigned long							lMapID;						//郡ID
	unsigned long							lCityID;					//城市ID
	int										iType;						//物品类型:1,武器; 2,防具;3,驯兽技术,4,特殊工具,5,英雄装备.6,基本物资 GOODS_TYPE
	unsigned long							lSortID;					//流水号
	int										iCurrentPrice;				//当前出价
};
//购买物品
struct BUY_GOODS : DISTRIBUTED_HEADER
{
	inline void serialize(unsigned long lMapID, unsigned long lCityID, int iType, unsigned long lSortID, int from=0, int to=0, int command=0);
	unsigned long							lMapID;						//郡ID
	unsigned long							lCityID;					//城市ID
	int										iType;						//物品类型:1,武器; 2,防具;3,驯兽技术,4,特殊工具,5,英雄装备.6,基本物资 GOODS_TYPE
	unsigned long							lSortID;					//流水号
};
//部队列表信息
struct tagArmysList
{
	unsigned long						lMapID;						    	//目标郡ID
	unsigned long						lPlayerID;							//玩家ID
	unsigned long						lArmyID;							//部队ID
	unsigned long						lOfficerID;							//指挥官ID
	char								szOfficerName[SIZE_32];				//指挥官名称
	char								szArmyName[SIZE_32];				//部队名称
	int									iFace;								//指挥官头像
	int									iRaceID;							//种族
	int									iJobID;								//职系
	int									iX;									//当前部队坐标
	int									iY;									//
	int									iCount;
	int									iDirection;							//当前部队方向
	int									iState;								//0:城内 1:城外

	int									iWeaponID;							//武器ID
	int									iDefendID;							//防具ID
	int									iWeaponLevel;						//武器等级
	int									iDefendLevel;						//防具等级
	int									iExp;
};
struct MAP_ARMY_LIST : DISTRIBUTED_HEADER
{
	unsigned int iCount;
	inline void serialize(int count, int from=0, int to=0, int command=0);
	inline tagArmysList & operator [](unsigned int);
};
//部队信息
struct ARMY_INFO : DISTRIBUTED_HEADER
{
	inline void serialize(int iCount, int iDefendID, int iWeaponLevel,int iDefendLevel,int iExp, int from=0, int to=0);
	
	
	int									iCount;
	int									iDefendID;						//防具ID
	int									iWeaponLevel;					//武器等级
	int									iDefendLevel;					//防具等级
	int									iExp;
};
//部队列表
struct tagArmyList
{
	unsigned long							lArmyID;							//部队ID
	char									szName[SIZE_32];					//部队名称
	
};
//部队列表
struct ARMY_LIST : DISTRIBUTED_HEADER
{
	unsigned int iCount;
	inline void serialize(int count, int from=0, int to=0);
	inline tagArmyList & operator [](unsigned int);
};
//部队在城市地图上移动
struct ARMY_MOVE_MAP : DISTRIBUTED_HEADER
{
	inline void serialize(unsigned long lMapID, unsigned long lArmyID, int iStartX, int iStartY,  int iEndX, int iEndY, int iflag, int from=0, int to=0, int command=0);
	unsigned long						lMapID;								//郡ID
	unsigned long						lArmyID;							//部队ID
	int									iStartX;							//移动开始坐标
	int									iStartY;							//
	int									iEndX;								//移动结束坐标
	int									iEndY;								//
	int									iflag;								//当前部队方向
};
//部队城市地图上可移动的坐标
struct tagArmyMoveList
{
	unsigned long						lArmyID;							//部队ID
	int									iX;									//可移动的坐标
	int									iY;
	int									iWeight ;							//障碍物
};
//部队城市地图上可移动的坐标列表
struct ARMY_CAN_MOVE_LIST : DISTRIBUTED_HEADER
{
	unsigned int iCount;
	inline void serialize(int count, int from=0, int to=0);
	inline tagArmyMoveList & operator [](unsigned int);
	
};
//修改指挥官名字
struct MODIFY_OFFICER_NAME : DISTRIBUTED_HEADER
{
	inline void serialize(unsigned long lOfficerID, char szOfficerName[],int from=0, int to=0);
	unsigned long					lOfficerID;
	char							szOfficerName[SIZE_24];
};
//成功信息码
struct SUCCEED_INFO : DISTRIBUTED_HEADER
{
	inline void serialize(unsigned long lCityID,unsigned long lSortID,int iEventID, int iLevel, int iCount, int iCode, int from=0, int to=0);
	unsigned long lCityID;			//城市
	unsigned long lSortID;			//流水号
	int iEventID;					//生产,科研道具的ID 或 建筑物的ID
	int iLevel;						//等级
	int iCount;						//数量
	int iCode;//成功码:
};
//市场信息码
struct MARKET_INFO : DISTRIBUTED_HEADER
{
	inline void serialize(int iTyeID, int iGoodsID,int iCount, int iCode, int from=0, int to=0);
	int	iTypeID;					//类型ID
	int iGoodsID;					//物品ID
	int iCount;						//数量
	int iCode;						//信息码
};
//错误信息
struct ERROR_INFO : DISTRIBUTED_HEADER
{
	inline void serialize(int iCode, int from, int to);
	int iCode;//错误码:
};
//退出游戏
struct EXIT_MAP_SERVER : DISTRIBUTED_HEADER 
{
	inline void serialize(int from, int to);
};
//退出游戏
struct EXIT_GAME : DISTRIBUTED_HEADER 
{
	inline void serialize(unsigned long lPlayerID, unsigned long lMapID, int from, int to);
	unsigned long lPlayerID;
	unsigned long lMapID;
};
//指挥官技能
struct tagOfficerTechList 
{
	int					iTechID;	//技能ID
	int					Level;		//技能等级	
};
struct OFFICER_TECH_LIST : DISTRIBUTED_HEADER 
{
	unsigned long		lOfficerID;		//指挥官ID
	unsigned int iCount;
	inline void serialize(int officerid,int count, int from/*=0*/, int to/*=0*/, int command/*=0*/);
	inline tagOfficerTechList & operator [](unsigned int);
};
//指挥官学习技能
struct STUDY_OFFICER_TECH : DISTRIBUTED_HEADER 
{
	inline void serialize(unsigned long lOfficerID, int iTechID, int iType, int from=0, int to=0, int command=0);
	unsigned long		lOfficerID;
	int					iTechID;	//技能ID
	int					iType;		//技能类型
};
//指挥官信息
struct GET_OFFICER_INFO : DISTRIBUTED_HEADER 
{
	inline void serialize(unsigned long lOfficerID,int from=0, int to=0, int command=0);
	unsigned long		lOfficerID;
};
//指挥官属性
struct OFFICER_ATTRIBUTE : DISTRIBUTED_HEADER 
{
	inline void serialize(int officerid,int iCredit, int iStamina, int iPower, int iCelerity, int iWit, int iAttack,
		int iPhysicRecovery, int iMagicRecovery, int iLevel, int iMilitaryRank_,int iExp, int techdot,
		int iActionDot,int iEquipStamina,int iEquipPower,int iEquipCelerity,int iEquipWit,int iHeroHP,int iMagicAttack,
		int from/*=0*/, int to/*=0*/);
	int iCredit;				//荣誉点
	int iStamina;				//耐力
	int iPower;					//力量
	int iCelerity;				//敏捷
	int iWit;					//智力
	int iAttack;				//攻击
	int iPhysicRecovery;		//防御
	int iMagicRecovery;			//魔防
	int iLevel;					//等级
	int iMilitaryRank_;			//军衔
	int OfficerID;				//英雄ID
	int Exp;					//经验值
	int TechDot;				//技能点
	int ActionDot;				//行动值
	int EquipStamina;			//装备附加耐力
	int EquipPower;				//装备附加力量
	int EquipCelerity;			//装备附加敏捷
	int EquipWit;				//装备附加智力
	int HeroHP;					//生命值
	int MagicAttack;			//魔法攻击
};
//英雄装备列表
struct tagHeroEquipList
{
	int iSortID;				//流水号
	Inlay_Union InlayItem;		//装备ID，可能是复合的ID，包括升级信息
	int iLevel;					//等级
	int iPos;					//使用部位
	int iShowPos;				//背包中显示位置 在英雄身
	int	iType;					//类型 1:英雄身上 2:英雄背包 3: 公共背包 (引用 PACK_TYPE)
	int iCount;					//数量
	int Skill;					//技能
	int SkillLevel;				//技能等级
};
struct HERO_EQUIP_LIST : DISTRIBUTED_HEADER 
{
	int iCount;
	inline void serialize(int count, int from=0, int to=0, int command=0);
	inline tagHeroEquipList & operator [](unsigned int);
};
//操作背包中的装备
struct tagExchangeEquipList
{
	int iSortID;				//流水号
	int iShowPos;				//背包中显示位置
	int	iType;					//类型 1:英雄身上 2:英雄背包 3: 公共背包
	int iMode;					//操作方式 1:穿上装备,2:脱下装备 3:换入背包中,4:从背包中取出,5:同个背包中装备交换位置(引用 EXCHANGE_EQUIP_MODE)
};
struct EXCHANGE_EQUIP_LIST : DISTRIBUTED_HEADER 
{
	int iCount;
	unsigned long lHeroID;		//英雄ID
	inline void serialize(int count, int from, int to);
	inline tagExchangeEquipList & operator [](unsigned int);
};
//操作装备返回信息
struct EXCHANGE_EQUIP_INFO : DISTRIBUTED_HEADER 
{
	inline void serialize(int iCode, int iSortID, int from, int to);
	int iCode;					//操作码
	int iSortID;				//流水号
//	int iItemID;				//装备ID
};
//销毁装备
struct DESTROY_EQUIP : DISTRIBUTED_HEADER 
{
	inline void serialize(unsigned long lHeroID,int iSortID,int count, int from, int to);
	unsigned long lHeroID;		//英雄ID
	int iSortID;				//流水号
	int Count;					//销毁数量（回收数量）
};
//赎回英雄
struct REDEEM_HERO : DISTRIBUTED_HEADER 
{
	inline void serialize(unsigned long lHeroID, int iMoney, int from, int to);
	unsigned long lHeroID;		//英雄ID
	int iMoney;
};
//英雄战争记录
struct tagHeroWarRecord
{
	int iSortID;						//
	__int64 lEnterDate;					//进入战斗日期
	unsigned long lMap;					//战场(郡ID)
	unsigned long lArmyID;				//带领部队ID
	int iEnemyCount;					//杀敌数量
	int iGeneralCount;					//杀将数量
	__int64 lExitDate;					//退出战斗日期
	int iState;							//英雄状态
	__int64 lFreeDate;					//释放日期
	char Armyname[SIZE_32];			//部队名称
};
struct HERO_WAR_RECORD_LIST : DISTRIBUTED_HEADER 
{
	int iCount;
	inline void serialize(int count, int from, int to);
	inline tagHeroWarRecord & operator [](unsigned int);
};
//英雄简成长记录
struct tagHeroGrowUpRecord
{
	int iSortID;						//
	int iType;							//记录类型 1:英雄升级 2:英雄军衔
	__int64 lGrowUpDate;				//升级日期
	int	iLevel;							//上升等级
};
struct HERO_GROWUP_RECORD_LIST : DISTRIBUTED_HEADER 
{
	int iCount;
	inline void serialize(int count, int from, int to);
	inline tagHeroGrowUpRecord & operator [](unsigned int);
};
//装载/卸载物资
struct tagMaterials
{
	int iType;	//买卖物品类型:1,武器; 2,防具;3,驯兽技术,4,特殊工具,5,英雄装备.6,基本物资 (见GOODS_TYPE枚举)
	int iMatID;	//类型为武器,防具时:iMatID为武器,防具的ID; 基本物资时:1,粮食;2,木材;3,铁;4,皮 5,黄宝石,6,红宝石,7,绿宝石,8,黑铁 (见BASE_RESOURCE_TYPE枚举)
	int iCount;	//物资数量
};
//装载/卸载物资
struct MATERIAL_LIST : DISTRIBUTED_HEADER 
{
	int iCount;	//记录条数
	unsigned long lArmyID;	//部队ID
	unsigned long lMapID;	//郡ID
	unsigned long lCityID;	//城市ID
	inline void serialize(int count, int from, int to, int command);
	inline tagMaterials & operator [](unsigned int);
};
//殖民城市
struct COLONIZE_CITY : DISTRIBUTED_HEADER 
{
	inline void serialize(unsigned long lArmyID, unsigned long lMapID, unsigned long lCityID, int from, int to, int command);
	unsigned long lArmyID;	//部队ID
	unsigned long lMapID;	//郡ID
	unsigned long lCityID;	//城市ID
};
//物资查看
struct LOOK_MATERIALS : DISTRIBUTED_HEADER 
{
	inline void serialize(unsigned long lArmyID, unsigned long lMapID, int from, int to, int command);
	unsigned long lArmyID;	//部队ID
	unsigned long lMapID;	//郡ID
};
//军衔晋升
struct MILITARY_RANK : DISTRIBUTED_HEADER 
{
	inline void serialize(char szName[], int iRank, int from, int to, int command);
	char szName[SIZE_24];			//英雄名称
	int iRank;						//英雄等级
};
//英雄被俘
struct CAPTIVE_HERO : DISTRIBUTED_HEADER 
{
	inline void serialize(char szByPlayerName[], char szByHeroName[], char szPlayerName[], int from, int to, int command);
	char szByPlayerName[SIZE_24];			//被俘英雄的玩家名称
	char szByHeroName[SIZE_24];			//被俘英雄的名称
	char szPlayerName[SIZE_24];			//玩家名称
};
//进入/离开地图
struct COMEIN_EXIT_MAP : DISTRIBUTED_HEADER 
{
	inline void serialize(char szName[], unsigned long lMapID, int iType, int from, int to, int command);
	char szName[SIZE_24];			//玩家名称
	unsigned long lMapID;			//郡ID
	int iType;						//1;进入. 2:离开地图
};
//部队达到预警
struct ARMY_REACH_WARM : DISTRIBUTED_HEADER 
{
	inline void serialize( unsigned long lMapID,int from, int to,int unioncnt,int clancnt,int aboriginalcnt);
	unsigned long lMapID;			//郡ID
	unsigned int  UnionCount;		//联盟即将到达的部队数量
	unsigned int ClanCount;			//部落即将到达的部队数量
	unsigned int AboriginalCount;	//土著即将到达的部队数量
};
//资源采集率cmd = EN_S_RESOURCE_RATE  or EN_S_RESOURCE_PERCENTRATE
struct RESOURCE_RATE : DISTRIBUTED_HEADER
{
	inline void serialize(int iFoodRate, int iWoodRate, int iSkinRate, int iIronRate, int iScience, int from, int to,int cmd = EN_S_RESOURCE_RATE);
	int iFoodRate;
	int iWoodRate;
	int iSkinRate;
	int iIronRate;
	int iScience;
};


//增加一个新的宝箱到英雄背包EN_C_NEWTREASUREPACK,EN_S_NEWTREASUREPACK,
typedef struct tagNewTreasurePack_Request:DISTRIBUTED_HEADER
{
	int OfficerID;					//英雄ID
	int ItemID;						//宝箱或军牌编号
	int ItemType;					//类型（参看枚举），宝箱：TREASUREPACK；军牌：ARMYBRAND
	inline void serialize(int playerid,int officerid,int itemid,int type);
}NewTreasurePack_Request,*LPNewTreasurePack_Request;

typedef struct tagNewTreasurePack_Response:DISTRIBUTED_HEADER
{
	int Error_Code;					//0:成功；EN_PACKFULL：背包已满;EN_OPERATE_DBG_FAILED：添加失败
	tagHeroEquipList itemInfo;		//新增的宝箱、军牌信息
	inline void serialize(int agent,int playerid,int code);
}NewTreasurePack_Response,*LPNewTreasurePack_Response;

//打开宝箱EN_C_OPENTREASUREPACK,EN_S_OPENTREASUREPACK,
typedef struct tagOpenTreasurePack_Request:DISTRIBUTED_HEADER
{
	int MapID;					//当前郡地图ID	
	int CityID;
	int OfficerID;				//道具所在背包的英雄ID，公共背包是为1
	int SortID;					//流水号
	inline void serialize(int playerid,int mapid,int cityid,int officerid,int sortid);
}OpenTreasurePack_Request,*LPOpenTreasurePack_Request;

typedef struct tagOpenTreasurePack_Response:DISTRIBUTED_HEADER
{
	int Error_Code;					//0:成功；1：无宝箱；2：仓库容量不够；
	int Money;						//成功时，打开的金钱数
	int ItemID;						//EN_YELLOW_STONE = 5,黄宝石
									//EN_RED_STONE = 6,		红宝石
									//EN_GREEN_STONE = 7,		绿宝石
									//EN_BLACK_STONE = 8		黑铁
	int Count;						//数量
	tagHeroEquipList EquipItem;		//装备信息，ItemID<=0时，未开出装备
	inline void serialize(int agent,int playerid,int code,int money = 0,int itemid = 0,int randcount = 0,
		int equipid = 0,int sortid = 0,int level = 0,int pos = 0,int showpos = 0,int type = 0,int equipcount=0);
}OpenTreasurePack_Response,*LPOpenTreasurePack_Response;

//英雄重伤通知EN_C_HEROGBH_NOTIFY,EN_S_HEROGBH_NOTIFY
typedef struct tagHeroGBH_Notify:DISTRIBUTED_HEADER
{
	int OfficerID;
	inline void serialize(int playerid,int officerid);
}HeroGBH_Notify,*LPHeroGBH_Notify;

typedef struct tagHeroGBH_Broadcast:DISTRIBUTED_HEADER
{
	char	PlayerName[SIZE_24];	//玩家名称
	char	OfficerName[SIZE_24];	//英雄名称
	inline void serialize(const char *playername,const char *officername);
}HeroGBH_Broadcast,*LPHeroGBH_Broadcast;

//立即完成CGC_C_IMMEDCOMPLETE,CGC_S_IMMEDCOMPLETE,
typedef struct tagImmedComplete_Request:DISTRIBUTED_HEADER
{
	/*
	Construction_Speed_Card,		//建筑提速卡
	Research_Speed_Card,			//科研提速卡
	Conscription_Speed_Card,		//征兵提速卡
	Recruit_Card,					//招募英雄卡
	Produce_Speed_Card,				//生产提速卡
	*/
	int Type;						//与卡保持一致，包括：建筑立即完成、科研立即完成、征兵立即完成、招募英雄、生产
	int MapID;
	int CityID;
	int ProductionType;				//主要针对生产立即完成，表示生产的物品类型，如武器、防具、特殊工具等，具体参看GOODS_TYPE枚举
	int ItemID;						//建筑时为建筑ID、生产时为类型ID（如武器、防具等）
	int SortID;						//流水号
	int Level;						//级别
	inline void serialize(int playerid,int type,int mapid,int cityid,int itemid,int sortid,int level,int protype);
}ImmedComplete_Request,*LPImmedComplete_Request;
inline void tagImmedComplete_Request::serialize(int playerid,int type,int mapid,int cityid,int itemid,int sortid,int level ,int protype)
{
	Type = type;
	MapID = mapid;
	CityID = cityid;
	ItemID = itemid;
	SortID = sortid;
	Level = level;
	ProductionType = protype;
	int svrid = 0;
	switch(type)
	{
	case Construction_Speed_Card:
	case Conscription_Speed_Card:
	case Produce_Speed_Card:
		svrid = mapid;
		break;
	case Research_Speed_Card:
		svrid = MDM_GP_MAP_RESEARCH;
		break;
	case Recruit_Card:
		svrid = SERVER_BOUND_MAP_OTHER;
		break;
	default:
		svrid = SERVER_BOUND_MAP_OTHER;
	}
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,svrid,EN_C_IMMEDCOMPLETE);
}

typedef struct tagImmedComplete_Response:DISTRIBUTED_HEADER
{
	int Error_Code;					//0:成功；1：Go点不足；2：未建造或升级中（或已完成）;3:不需要使用立即招募；111：其他
	int Type;						//类型
	int SortID;						//流水号
	int Count;						//耗费的Go点数量
	inline void serialize(int agent,int from,int playerid,int code,int type,int sortid,int count);
}ImmedComplete_Response,*LPImmedComplete_Response;
inline void tagImmedComplete_Response::serialize(int agent,int from,int playerid,int code,int type,int sortid,int count)
{
	Error_Code = code;
	Type = type;
	SortID = sortid;
	Count = count;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,from,playerid,EN_S_IMMEDCOMPLETE);
}

//重置郡地图信息EN_C_RESETMAPINFO,大地图通知郡服务器
typedef struct tagResetMapInfo_Request:DISTRIBUTED_HEADER
{
	inline void serialize(int mapid);
}ResetMapInfo_Request,*LPResetMapInfo_Request;
inline void tagResetMapInfo_Request::serialize(int mapid)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,SERVER_BOUND_MAP_OTHER,mapid,EN_C_RESETMAPINFO);
}


//获取玩家部队列表EN_C_GETARMYLIST,	EN_S_GETARMYLIST,
typedef struct tagGetArmyList_Request:DISTRIBUTED_HEADER
{
	inline void serialize(int playerid);
}GetArmyList_Request,*LPGetArmyList_Request;
inline void tagGetArmyList_Request::serialize(int playerid)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,SERVER_BOUND_MAP_OTHER,EN_C_GETARMYLIST);
}

typedef struct tagPlayerArmyInfo
{
	int				OfficerID;				//指挥官ID
	unsigned int	ArmyID;					//部队ID
	char			ArmyName[SIZE_32];		//部队名称
	char			OfficerName[SIZE_24];	//指挥官名称
	int				Mapid;					//部队所在地图
	int				Soldiertype;			//士兵类型
	int				Raceid;					//种族ID
	int				Firstcount;				//数量
	int				Fweapon;				//武器类型
	int				Fdefendtype;			//防具类型
	int				State;					//状态，参考枚举ARMY_TRUST_STATE
}PlayerArmyInfo,*LPPlayerArmyInfo;

typedef struct tagGetArmyList_Response:DISTRIBUTED_HEADER
{
	int Count;								//部队数量，
	inline void serialize(int agent,int playerid,int count);
	inline PlayerArmyInfo & operator [](unsigned int);
}GetArmyList_Response,*LPGetArmyList_Response;
inline void tagGetArmyList_Response::serialize(int agent,int playerid,int count)
{
	Count = count;
	if(count <0)
		count = 0;
	DISTRIBUTED_HEADER::serialize(sizeof(*this) + count * sizeof(PlayerArmyInfo),agent,SERVER_BOUND_MAP_OTHER,playerid,EN_S_GETARMYLIST);
}
inline PlayerArmyInfo & tagGetArmyList_Response::operator [](unsigned int index)
{
	return ((LPPlayerArmyInfo)(this + 1))[index];
}
//战斗服务、副本发给大地图的通知消息，部队状态改变EN_C_MODIFYARMYSTATE,	
typedef struct tagNotify_ModifyArmyState:DISTRIBUTED_HEADER
{
	PlayerArmyInfo armyInfo;
	inline void serialize(int playerid,int armyid,int mapid,int soldiercount,int state,int weapontype);
}Notify_ModifyArmyState,*LPNotify_ModifyArmyState;
inline void tagNotify_ModifyArmyState::serialize(int playerid,int armyid,int mapid,int soldiercount,int state,int weapontype)
{
	armyInfo.ArmyID = armyid;				//部队ID
	armyInfo.Mapid = mapid;					//部队所在郡ID，大于0是有效
	armyInfo.Firstcount = soldiercount;		//部队人数，大于0时有效
	armyInfo.State = state;					//状态，参考枚举ARMY_TRUST_STATE
	armyInfo.Fweapon = weapontype;			//武器类型
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,SERVER_BOUND_MAP_OTHER,EN_C_MODIFYARMYSTATE);
}
//大地图服务器下发到玩家，部队状态改变EN_S_MODIFYARMYSTATE,	
typedef struct tagModifyArmyState_Response:DISTRIBUTED_HEADER
{
	PlayerArmyInfo armyInfo;
	inline void serialize(int agent,int playerid,int armyid,int mapid,int soldiercount,int state,int from = SERVER_BOUND_MAP_OTHER);
}ModifyArmyState_Response,*LPModifyArmyState_Response;
inline void tagModifyArmyState_Response::serialize(int agent,int playerid,int armyid,int mapid,int soldiercount,int state,int from)
{
	armyInfo.ArmyID = armyid;//部队ID
	armyInfo.Mapid = mapid;//部队所在郡ID，大于0是有效
	armyInfo.Firstcount = soldiercount;//部队人数，大于0时有效
	armyInfo.State = state;//状态，参考枚举ARMY_TRUST_STATE
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,from,playerid,EN_S_MODIFYARMYSTATE);
}
//玩家拆除一个据点EN_C_DESTROYCITY,	EN_S_DESTROYCITY,
typedef struct tagDestroyCity_Request:DISTRIBUTED_HEADER
{
	inline void serialize(int playerid,int mapid);
}DestroyCity_Request,*LPDestroyCity_Request;
inline void tagDestroyCity_Request::serialize(int playerid,int mapid)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,mapid,EN_C_DESTROYCITY);
}

typedef struct tagDestroyCity_Response:DISTRIBUTED_HEADER
{
	int Error_Code;			//0:成功;1:玩家在该地图没有据点；2:新手村无法拆除；111:失败
	inline void serialize(int agent,int playerid,int mapid,int code);
}DestroyCity_Response,*LPDestroyCity_Response;
inline void tagDestroyCity_Response::serialize(int agent,int playerid,int mapid,int code)
{
	Error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,mapid,playerid,EN_S_DESTROYCITY);
}
//查询俘虏信息EN_C_QUERYCAPTIVELIST,EN_S_QUERYCAPTIVELIST,
typedef struct tagQueryCaptiveList_Request:DISTRIBUTED_HEADER
{
	inline void serialize(int playerid);
}QueryCaptiveList_Request,*LPQueryCaptiveList_Request;
inline void tagQueryCaptiveList_Request::serialize(int playerid)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,SERVER_BOUND_MAP_OTHER,EN_C_QUERYCAPTIVELIST);
}

typedef struct tagCaptiveInfo
{
	int	PlayerID;						//玩家ID
	int OfficerID;						//英雄ID
	char PlayerName[SIZE_24];			//玩家名称
	char OfficerName[SIZE_24];			//英雄名称
	
	int Officerrace;					//种族
	int Level;							//级别
	int Militaryrank;					//军衔
	int Jobid;							//职业
	int Faceid;							//头像
	int Stamina;						//耐力
	int Power;							//力量
	int Celerity;						//敏捷
	int Wit;							//智力
	int EquipStamina;					//装备耐力
	int EquipPower;						//装备力量
	int EquipCelerity;					//装备敏捷
	int EquipWit;						//装备智力
	int Techdot;						//技能点
	int Attack;							//攻击-物理
	int Physicrecovery;					//物理防御
	int Magicrecovery;					//魔法防御
	int MagicAttack;					//魔法攻击
	unsigned int Times;					//已抓获时间，单位毫秒
}CaptiveInfo,*LPCaptiveInfo;
typedef struct tagQueryCaptiveList_Response:DISTRIBUTED_HEADER
{
	int Count;//俘虏数量>0为有俘虏,-1：失败
	inline void serialize(int agent,int playerid,int count);
	inline CaptiveInfo & operator [](unsigned int index);
}QueryCaptiveList_Response,*LPQueryCaptiveList_Response;
inline void tagQueryCaptiveList_Response::serialize(int agent,int playerid,int count)
{
	Count = count;
	DISTRIBUTED_HEADER::serialize(sizeof(*this) + Count*sizeof(CaptiveInfo),agent,SERVER_BOUND_MAP_OTHER,playerid,EN_S_QUERYCAPTIVELIST);
}
inline CaptiveInfo & tagQueryCaptiveList_Response::operator [](unsigned int index)
{
	return ((LPCaptiveInfo)(this + 1))[index];
}
//创建NPC部队EN_C_CREATENPCARMY,EN_S_CREATENPCARMY,
typedef struct tagNotify_CreateNPCArmy_Request:DISTRIBUTED_HEADER
{
	int GMCmd;					//GM命令
	int PlayerID;				//GM的ID
	int CountryID;				//NPC部队的阵营
	int Count;					//数量
	int Level;					//部队的级别
	int FromMap;				//出发郡（主要为了着陆点的确定);
	int TargetMap;				//目标郡
	int Times;					//次数
	int Interval;				//间隔时间，单位：秒
	inline void serialize(int agent,int playerid,int cmd,int countryid,int level,int count,int frommap,int targetmap,int times,int interval,int from);
}Notify_CreateNPCArmy_Request,*LPNotify_CreateNPCArmy_Request;
inline void tagNotify_CreateNPCArmy_Request::serialize(int agent,int playerid,int cmd,int countryid,int level,int count,int frommap,int targetmap,int times,int interval,int from)
{
	this->GMCmd = cmd;
	this->PlayerID = playerid;
	this->CountryID = countryid;
	this->Count = count;
	this->Level = level;
	this->FromMap = frommap;
	this->TargetMap = targetmap;
	this->Times = times;
	this->Interval = interval;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,from,SERVER_BOUND_MAP_OTHER,EN_C_CREATENPCARMY);
}
//大地图返回
typedef struct tagNotify_CreateNPCArmy_Response:DISTRIBUTED_HEADER
{
	short Error_Code;				//0：成功，1：无法到达;2:NPC参数不存在；3:参数有误；
	int GMCmd;						//GM命令
	int CountryID;					//NPC部队的阵营
	int Count;						//数量
	int Level;						//部队的级别
	int FromMap;					//出发郡（主要为了着陆点的确定);
	int TargetMap;					//目标郡
	int Times;					//次数
	int Interval;				//间隔时间，单位：秒
	inline void serialize(int agent,int playerid,short code,int cmd,int countryid,int level,int count,int frommap,int targetmap,int times,int interval);
}Notify_CreateNPCArmy_Response,*LPNotify_CreateNPCArmy_Response;
inline void tagNotify_CreateNPCArmy_Response::serialize(int agent,int playerid,short code,int cmd,int countryid,int level,int count,int frommap,int targetmap,int times,int interval)
{
	this->Error_Code = code;
	this->GMCmd = cmd;
	this->CountryID = countryid;
	this->Count = count;
	this->Level = level;
	this->FromMap = frommap;
	this->TargetMap = targetmap;
	this->Times = times;
	this->Interval = interval;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,SERVER_BOUND_MAP_OTHER,playerid,EN_S_CREATENPCARMY);
}

//删除NPC部队EN_C_DELNPCARMY,EN_S_DELNPCARMY,直接通知对应的郡服务器
typedef struct tagNotify_DelNPCArmy_Request:DISTRIBUTED_HEADER
{
	int GMCmd;				//GM命令
	int PlayerID;			//GM的ID
	int CountryID;
	inline void serialize(int agent,int playerid,int cmd,int mapid,int countryid,int from);
}Notify_DelNPCArmy_Request,*LPNotify_DelNPCArmy_Request;
inline void tagNotify_DelNPCArmy_Request::serialize(int agent,int playerid,int cmd,int mapid,int countryid,int from)
{
	this->PlayerID = playerid;
	this->GMCmd = cmd;
	this->CountryID = countryid;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,from,mapid,EN_C_DELNPCARMY);
}
typedef struct tagNotify_DelNPCArmy_Response:DISTRIBUTED_HEADER
{
	int DelCount;			//删除的NPC部队数
	int GMCmd;				//GM命令
	int CountryID;
	inline void serialize(int agent,int playerid,int count,int cmd,int mapid,int countryid);
}Notify_DelNPCArmy_Response,*LPNotify_DelNPCArmy_Response;
inline void tagNotify_DelNPCArmy_Response::serialize(int agent,int playerid,int count,int cmd,int mapid,int countryid)
{
	this->DelCount = count;
	this->GMCmd = cmd;
	this->CountryID = countryid;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,mapid,playerid,EN_S_DELNPCARMY);
}

//通知战斗线程新增部队EN_C_FIGHT_CREATEARMY,EN_S_FIGHT_CREATEARMY,
typedef struct tagFIGHT_CreateArmy_Request:DISTRIBUTED_HEADER
{
	unsigned int ArmyID;			//部队ID
	inline void serialize(int agent,int playerid,int mapid,int armyid);
}FIGHT_CreateArmy_Request,*LPFIGHT_CreateArmy_Request;
inline void tagFIGHT_CreateArmy_Request::serialize(int agent,int playerid,int mapid,int armyid)
{
	ArmyID = armyid;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,playerid,mapid,EN_C_FIGHT_CREATEARMY);
}

//郡服务器通知大地图服务器郡状态改变EN_C_NOTIFY_MAPSTATE,EN_S_NOTIFY_MAPSTATE,
typedef struct tagNotify_MapState_Request:DISTRIBUTED_HEADER
{
	int CountryID;				//归属
	int State;					//状态
	inline void serialize(int mapid,int countryid,int state);
}Notify_MapState_Request,*LPNotify_MapState_Request;
inline void tagNotify_MapState_Request::serialize(int mapid,int countryid,int state)
{
	CountryID = countryid;
	State = state;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,mapid,SERVER_BOUND_MAP_OTHER,EN_C_NOTIFY_MAPSTATE);
}

typedef struct tagNotify_MapState_Response:DISTRIBUTED_HEADER//暂无用
{
	short Error_Code;//0:成功
	inline void serialize(int from,int to,short code);
}Notify_MapState_Response,*LPNotify_MapState_Response;
inline void tagNotify_MapState_Response::serialize(int from,int to,short code)
{
	Error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,from,to,EN_S_NOTIFY_MAPSTATE);
}
//公共建筑升级EN_C_MODIFY_PUBLICBUILD,		
typedef struct tagModify_PublicBuild_Request:DISTRIBUTED_HEADER
{
	int BuildType;
	unsigned long BuildID;
	int CurrentLevel;
	inline void serialize(int agent,int playerid,int buildtype,unsigned long buildid,int level);
}Modify_PublicBuild_Request,*LPModify_PublicBuild_Request;
inline void tagModify_PublicBuild_Request::serialize(int agent,int playerid,int buildtype,unsigned long buildid,int level)
{
	BuildType = buildtype;
	BuildID = buildid;
	CurrentLevel = level;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,playerid,0,EN_C_MODIFY_PUBLICBUILD);
}

//精练（镶嵌）装备EN_C_INLAYEQUIP,EN_S_INLAYEQUIP,
typedef struct tagInlayEquip_Request:DISTRIBUTED_HEADER
{
	int SortID_Equip;			//装备sortid
	int SortID_InlayResource;	//镶嵌原料的sortid
	int SortID_Cards[2];		//幸运符和保底卡的sortid
	inline void serialize(int playerid,int sortid_eqiup,int sortid_inlayres,int sortid_card1,int sortid_card2);
}InlayEquip_Request,*LPInlayEquip_Request;
inline void tagInlayEquip_Request::serialize(int playerid,int sortid_eqiup,int sortid_inlayres,int sortid_card1,int sortid_card2)
{
	SortID_Equip = sortid_eqiup;
	SortID_InlayResource = sortid_inlayres;
	SortID_Cards[0] = sortid_card1;
	SortID_Cards[1] = sortid_card2;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,SERVER_BOUND_MAP_OTHER,EN_C_INLAYEQUIP);
}

typedef struct tagInlayEquip_Response:DISTRIBUTED_HEADER
{
	short Error_Code;				//0:成功;-1:不能精练(装备不能本身不能精练或者已精练到最大次数);-2:装备与宝石不匹配;-3:非宝石;-4:非装备;1:失败；111:失败
	inline void serialize(int agent,int playerid,short code);
}InlayEquip_Response,*LPInlayEquip_Response;
inline void tagInlayEquip_Response::serialize(int agent,int playerid,short code)
{
	Error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,SERVER_BOUND_MAP_OTHER,playerid,EN_S_INLAYEQUIP);
}
//背包中新增一物品，服务器主动下发EN_S_NEWITEMINPACK,

//使用援兵卡EN_C_USEADDSOLDIERCARD,EN_S_USEADDSOLDIERCARD,
struct ReqUsePropCard:DISTRIBUTED_HEADER
{
	int armyID;						//部队ID
	int cardType;					//卡类型，
	int roomID;						//房间号，郡地图忽略；副本时，填写房间号
	inline void serialize(int playerid,int armyid,int cardType,int svrId,int roomid);
};
inline void ReqUsePropCard::serialize(int playerid,int armyid,int cardType,int svrId,int roomid)
{
	armyID = armyid;
	cardType = cardType;
	roomID = roomid;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,svrId,REQ_USE_PROP_CARD);
}
struct RspUsePropCard:DISTRIBUTED_HEADER
{
	int cardType;					//卡类型，
	short error_Code;				//1:目标部队不存在，2：该类型的卡不存在，3：此种类型卡已达使用上限
	inline void serialize(int agent,int playerid,int mapid, int cardType ,short code);
};
inline void RspUsePropCard::serialize(int agent,int playerid,int mapid, int cardType ,short code)
{
	error_Code = code; //1:目标部队不存在，2：该类型的卡不存在，3：此种类型卡已达使用上限
	cardType = cardType;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,mapid,playerid,RSP_USE_PROP_CARD);
}
//参看HERO_EQUIP_LIST


//兑换EN_C_CONVERT,EN_S_CONVERT,如果兑换方式一致（兑换的物品是固定的），可以使用统一的接口，目前只用于军牌
typedef struct tagConvert_Request:DISTRIBUTED_HEADER
{
	int officerID;				//道具所在背包的英雄ID，公共背包是为1
	int sortID;					//流水号
	inline void serialize(int playerid,int officerid,int sortid);
}Convert_Request,*LPConvert_Request;
inline void tagConvert_Request::serialize(int playerid,int officerid,int sortid)
{
	officerID = officerid;
	sortID = sortid;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,SERVER_BOUND_MAP_OTHER,EN_C_CONVERT);
}
typedef struct tagConvert_Response:DISTRIBUTED_HEADER
{
	short error_Code;					//0:成功；1：不存在；111：失败
	inline void serialize(int agent,int playerid,short code);
}Convert_Response,*LPConvert_Response;
inline void tagConvert_Response::serialize(int agent,int playerid,short code)
{
	error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,SERVER_BOUND_MAP_OTHER,playerid,EN_S_CONVERT);
}

//招募到特殊英雄进行广播EN_S_BROAD_SPECIALHERO,
typedef struct tagBroad_SpecialHero:DISTRIBUTED_HEADER
{
	char playerName[SIZE_24];				//玩家名称
	int countryID;							//阵营
	int specialHeroID;						//英雄ID,>0:特殊英雄ID;<=0则为普通英雄
	int  totalAttribute;					//总属性
	inline void serialize(const char *playername,int country,int heroid,int attribute);
}Broad_SpecialHero,*LPBroad_SpecialHero;
inline void tagBroad_SpecialHero::serialize(const char *playername,int country,int heroid,int attribute)
{
	if(playername)
	{
		memcpy(playerName,playername,SIZE_24);
	}
	else
	{
		playerName[0] = 0;
	}
	countryID = country;
	specialHeroID = heroid;
	totalAttribute = attribute;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,SERVER_BOUND_MAP_OTHER,0,EN_S_BROAD_SPECIALHERO);
}

//打开卡，统一右键点击就能打开的卡（原有右键打开的采用原有 方式，以后右键打开的卡可采用统一的协议）EN_C_COMMONOPENCARD,EN_S_COMMONOPENCARD
typedef struct tagCommonOpenCard_Request:DISTRIBUTED_HEADER
{
	int heroID;					//英雄ID，使用卡的英雄ID
	int sortID;					//卡的流水号
	inline void serialize(int playerid,int heroid,int sortid);
}CommonOpenCard_Request,*LPCommonOpenCard_Request;
inline void tagCommonOpenCard_Request::serialize(int playerid,int heroid,int sortid)
{
	heroID = heroid;
	sortID = sortid;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,SERVER_BOUND_MAP_OTHER,EN_C_COMMONOPENCARD);
}
typedef struct tagCommonOpenCard_Response:DISTRIBUTED_HEADER
{
	short error_Code;				//0:成功；EN_OFFICER_INWAR：战斗中不能使用；EN_OPERATE_DBG_FAILED：操作DBG失败；EN_OFFICER_NOSKILL：没有可清除的技能
	inline void serialize(int agent,int playerid,short code);
}CommonOpenCard_Response,*LPCommonOpenCard_Response;
inline void tagCommonOpenCard_Response::serialize(int agent,int playerid,short code)
{
	error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,SERVER_BOUND_MAP_OTHER,playerid,EN_S_COMMONOPENCARD);
}

//使用改名卡修改英雄的名字EN_C_MODIFYHERONAME,EN_S_MODIFYHERONAME,
typedef struct tagModifHeroName_Request:DISTRIBUTED_HEADER
{
	int heroID;					//所修改的英雄ID
	char newName[SIZE_24];		//新名称
	int sortID;					//改名卡的流水号
	inline void serialize(int playerid,int heorid,const char *name,int sortid);
}ModifHeroName_Request,*LPModifHeroName_Request;
inline void tagModifHeroName_Request::serialize(int playerid,int heroid,const char *name,int sortid)
{
	heroID = heroid;
	memset(newName,0,SIZE_24);
	if(name)
	{
		strncpy(newName,name,SIZE_24-1);
	}
	sortID = sortid;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,SERVER_BOUND_MAP_OTHER,EN_C_MODIFYHERONAME);
}
typedef struct tagModifHeroName_Response:DISTRIBUTED_HEADER
{
	short error_Code;				//0：成功；EN_OFFICER_ID_ERROR：英雄ID非法；EN_NO_HAVECARD：卡不存在;EN_HERONAMEEXIST：英雄名已存在；111:其他失败
	inline void serialize(int agent,int playerid,short code);
}ModifHeroName_Response,*LPModifHeroName_Response;
inline void tagModifHeroName_Response::serialize(int agent,int playerid,short code)
{
	error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,SERVER_BOUND_MAP_OTHER,playerid,EN_S_MODIFYHERONAME);
}

/*
mapID：只对建筑、生产有效
itemID:可以为建筑物ID、科研ID、装备ID、技能ID、生产的武器装备ID;且当为装备ID时，可以根据ID获取精练信息
itemType：对于生产包括武器、防具、特殊工具；
*/
typedef struct tagBaseNotifyPlayerMsg:DISTRIBUTED_HEADER
{
	int mapID;							//郡ID
	__int64 itemID;						//ID
	short itemLevel;					//级别
	short itemType;						//类型	
	inline void serialize(int agent,int playerid,int mapid,__int64 itemid,short level,short type,int cmd,int svrid);
}BaseNotifyPlayerMsg,*LPBaseNotifyPlayerMsg;
inline void tagBaseNotifyPlayerMsg::serialize(int agent,int playerid,int mapid,__int64 itemid,short level,short type,int cmd,int svrid)
{
	this->mapID = mapid;
	this->itemID = itemid;
	this->itemLevel = level;
	this->itemType = type;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,svrid,playerid,cmd);
}
typedef struct tagBaseBroadMsg:DISTRIBUTED_HEADER
{
	short countryID;					//阵营
	int playerID;						//玩家ID
	char playerName[SIZE_24];			//玩家名称
	int mapID;							//郡ID
	__int64 itemID;						//ID
	short itemLevel;					//级别
	short itemType;						//类型	
	inline void serialize(int agent,short country,int playerid,const char *name,int mapid,__int64 itemid,short level,short type,int cmd,int svrid);
}BaseBroadMsg,*LPBaseBroadMsg;
inline void tagBaseBroadMsg::serialize(int agent,short country,int playerid,const char *name,int mapid,__int64 itemid,short level,short type,int cmd,int svrid)
{
	this->countryID = country;
	this->playerID = playerid;
	if(name)
	{
		strcpy(playerName,name);
	}
	else
	{
		playerName[0] = 0;
	}
	this->mapID = mapid;
	this->itemID = itemid;
	this->itemLevel = level;
	this->itemType = type;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,svrid,playerid,cmd);
}
//大地图通知各服务器玩家是否在线EN_S_NOTIFYPLAYERONLINE
typedef struct tagNotifyPlayerOnline:DISTRIBUTED_HEADER
{
	int playerID;					//玩家ID，消息头里的agent>0是在线，否则下线
	inline void serialize(int playerid,int agent,int toSvrID);
}NotifyPlayerOnline,*LPNotifyPlayerOnline;
inline void tagNotifyPlayerOnline::serialize(int playerid,int agent,int toSvrID)
{
	playerID = playerid;//SERVER_BOUND_MAP_OTHER
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,SERVER_BOUND_MAP_OTHER,toSvrID,EN_S_NOTIFYPLAYERONLINE);
}
//玩家待完成的日常任务通知，任务通知战斗EN_C_NOTIFY_DAYTASKOFPLAYER,EN_S_NOTIFY_DAYTASKOFPLAYER,
typedef struct tagNotify_DayTaskOfPlayer_Request:DISTRIBUTED_HEADER
{
	int playerID;
	int taskID;
	short kindType;//种类  //TASKCOMPLETETARGETTYPE
	short itemType;//目标类型
	int itemID;//目标ID
	short number;//数量
	short itemLevel;//级别
	__int64 endTime;//结束时间
	inline void serialize(int playerid,int taskid,short kindtype,short itemtype,int itemid,short number,short level,__int64 endtime,int from,int to);
}Notify_DayTaskOfPlayer_Request,*LPNotify_DayTaskOfPlayer_Request;
inline void tagNotify_DayTaskOfPlayer_Request::serialize(int playerid,int taskid,short kindtype,short itemtype,int itemid,short num,short level,__int64 endtime,int from,int to)
{
	this->playerID = playerid;
	this->taskID = taskid;
	this->kindType = kindtype;
	this->itemType = itemtype;
	this->itemID = itemid;
	this->number = num;
	this->itemLevel = level;
	this->endTime = endtime;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,from,to,EN_C_NOTIFY_DAYTASKOFPLAYER);
}

typedef struct tagNotify_DayTaskOfPlayer_Response:DISTRIBUTED_HEADER
{
	int playerID;
	int taskID;
	inline void serialize(int playerid,int taskid,int from,int to);
}Notify_DayTaskOfPlayer_Response,*LPNotify_DayTaskOfPlayer_Response;
inline void tagNotify_DayTaskOfPlayer_Response::serialize(int playerid,int taskid,int from,int to)
{
	this->playerID = playerid;
	this->taskID = taskid;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,from,to,EN_S_NOTIFY_DAYTASKOFPLAYER);
}

//释放英雄EN_C_RELEASECAPTIVE,EN_S_RELEASECAPTIVE,
typedef struct tagReleaseCaptive_Request:DISTRIBUTED_HEADER
{
	int captPlayerID;			//被俘玩家ID
	int captOfficerID;			//被俘英雄ID
	inline void serialize(int playerid,int captplayerid,int captofficerid);
}ReleaseCaptive_Request,*LPReleaseCaptive_Request;
inline void tagReleaseCaptive_Request::serialize(int playerid,int captplayerid,int captofficerid)
{
	captPlayerID = captplayerid;
	captOfficerID = captofficerid;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,SERVER_BOUND_MAP_OTHER,EN_C_RELEASECAPTIVE);
}

typedef struct tagReleaseCaptive_Response:DISTRIBUTED_HEADER
{
	short error_Code;				//0:成功；1：被俘英雄不存在；111：其他失败
	inline void serialize(int agent,int playerid,short code);
}ReleaseCaptive_Response,*LPReleaseCaptive_Response;
inline void tagReleaseCaptive_Response::serialize(int agent,int playerid,short code)
{
	error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,SERVER_BOUND_MAP_OTHER,playerid,EN_S_RELEASECAPTIVE);
}
//英雄获取经验或者荣誉点通知EN_C_NOTIFY_EXPORCREDIT,
typedef struct tagNotify_ExpOrCredit_Request:DISTRIBUTED_HEADER
{
	int playerID;
	int exp;				//经验
	int credit;				//荣誉点
	inline void serialize(int playerid,int expval,int creditval);
}Notify_ExpOrCredit_Request,*LPNotify_ExpOrCredit_Request;
inline void tagNotify_ExpOrCredit_Request::serialize(int playerid,int expval,int creditval)
{
	playerID = playerid;
	exp = expval;
	credit = creditval;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,SERVER_BOUND_MAP_OTHER,MDM_GP_MAP_CONSORTIA,EN_C_NOTIFY_EXPORCREDIT);
}

// 双击修炼门获取玩家所有英雄的挂机情况或者当某一个英雄部队进入修炼门的感应区域发次请求
struct ReqHerosHangSys : DISTRIBUTED_HEADER
{
	unsigned long doorId;        // 修炼门ID. 
	inline void serialize(unsigned long doorId, int from, int to);
};

// 挂机系统中在各挂机英雄信息（含某一个英雄部队刚进入修炼门的感应区域的英雄信息）
struct tagHangingItemList
{
	unsigned long heroId;                 // 英雄ID
	char szName[SIZE_24];				  // 英雄名称
	unsigned int  hangSysType;            // 挂机类型 1:普通  2:极限
	int iCompletetime;		              // 还需要多少时间完成时间  秒
	unsigned long expVal;                 // 能获取的经验值
	unsigned long moneyVal;               // 需要花费的金钱
//	unsigned int heroState;               // 英雄状态 	0:空闲  1:带队  2:重伤  3:被俘  4:俘虏  5.挂机
};

// 返回玩家所有在挂机状态下的英雄或者英雄部队刚进入感应区的影响相关信息
struct RspHerosHangSys : DISTRIBUTED_HEADER
{
	unsigned int count; 
	inline void serialize(unsigned int count, int agent, int from, int to);
	inline tagHangingItemList & operator [](unsigned int index);
};

// 请求挂机一个英雄
struct ReqStartHangSystemOneHero : DISTRIBUTED_HEADER
{
	unsigned long heroID;        // 英雄ID
	unsigned int hangSysType;    // 挂机类型 1:普通  2:极限
	unsigned int trainingTime;   // 训练时间 单位：小时
	
	inline void serialize(unsigned long heroID, unsigned int hangSysType, unsigned int trainingTime, int from, int to);
};

// 挂机英雄的反馈
struct RspStartHangSystemOneHero : DISTRIBUTED_HEADER
{
	unsigned int ret;					  // 返回结果 0:成功 1:英雄状态不符合挂机条件，无法挂机 2: 金钱不足   3:没有对应级别的道具：强化剂  4：挂机失败，出现异常
	unsigned long heroId;                 // 英雄ID
	unsigned int completeTime;		      // 还需要多少时间完成时间  秒
	unsigned long expVal;                 // 能获取的经验值
	unsigned long moneyVal;               // 需要花费的金钱

	inline void serialize(unsigned int ret, unsigned long heroId, unsigned int completeTime, unsigned long expVal, unsigned long moneyVal, int agent, int from, int to);
};

// 取消某个英雄的挂机
struct ReqCancelHangOneHero : DISTRIBUTED_HEADER
{
	unsigned long heroID;        // 英雄ID

	inline void serialize(unsigned long heroID, int from, int to);
};

// 取消某个英雄的挂机 反馈
struct RspCancelHangOneHero : DISTRIBUTED_HEADER
{
	unsigned int ret;	 // 返回结果 0:成功 1:取消挂机失败
	inline void serialize(unsigned int ret, int agent, int from, int to);
};

// 某个英雄挂机完成通知
struct RspHangOneHeroFinish : DISTRIBUTED_HEADER
{
	unsigned long heroID;        // 英雄ID
	inline void serialize(unsigned long heroID, int agent, int from, int to);
};


#pragma pack(pop)

/**********************************************************************************************
//大地图服务器结构体
**********************************************************************************************/

//密钥验证
inline void
MAP_SERVER_VALIDATE::serialize(char const szAccounts[], char const szValidate[], int from, int to)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, EN_C_VALIDATE);
	strcpy(this->szAccounts, szAccounts);
	strcpy(this->szValidate, szValidate);
}
//基本信息
inline void 
PLAYER_BASE_INFO::serialize(unsigned long lPlayerID, char Name[], int iFace, unsigned long lConsortiaID, long lExp, int iRaceID, 
		int iJobID, int iCredit, int iLevel, int iMilitaryRank, unsigned long lGameTime,int iCountry, long lScience, long lMoney,
		int from, int to)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, EN_S_PLAYER_BASE_INFO);
	this->lPlayerID = lPlayerID;
	this->iFace = iFace;
	this->lConsortiaID = lConsortiaID;
	this->lExp = lExp;
	this->iRaceID = iRaceID;
	this->iJobID = iJobID;
	this->iCredit = iCredit;
	this->iLevel = iLevel;
	this->iMilitaryRank = iMilitaryRank;
	this->lGameTime = lGameTime;
	this->iCountry = iCountry;
	this->lScience = lScience;
	this->lMoney = lMoney;
	strcpy(this->szNick, Name);
}
//玩家的资源量
inline void 
PALEYER_RESOURCE::serialize(int iFoodCount,int iWoodCount,int iIronCount,int iSkinCount,int iRedStoneCount,
		int iYellowStoneCount,int iGreenStoneCount,int iBackIronCount,int iScience,int iMoney,int iContribute,
		int content,int from, int to)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, EN_S_PLAYER_RESOURCE_COUNT);
	this->iFoodCount = iFoodCount;
	this->iWoodCount = iWoodCount;
	this->iIronCount = iIronCount;
	this->iSkinCount = iSkinCount;
	this->iRedStoneCount = iRedStoneCount;
	this->iYellowStoneCount = iYellowStoneCount;
	this->iGreenStoneCount = iGreenStoneCount;
	this->iBackIronCount = iBackIronCount;
	this->iScience = iScience;
	this->iMoney = iMoney;
	this->iContribute = iContribute;
	this->iContent = content;
}
//玩家的基本资源
inline void 
PALEYER_RESOURCE_LIST::serialize(int iFoodCount,int iWoodCount,int iIronCount,int iSkinCount,int iRedStoneCount,
		int iYellowStoneCount,int iGreenStoneCount,int iBackIronCount,int content,int from, int to)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, EN_S_BASE_RESOURCE_LIST);
	this->iFoodCount = iFoodCount;
	this->iWoodCount = iWoodCount;
	this->iIronCount = iIronCount;
	this->iSkinCount = iSkinCount;
	this->iRedStoneCount = iRedStoneCount;
	this->iYellowStoneCount = iYellowStoneCount;
	this->iGreenStoneCount = iGreenStoneCount;
	this->iBackIronCount = iBackIronCount;
	this->iContent = content;
}
//城市信息
inline void 
CITY_INFO::serialize(unsigned long lMapID, unsigned long lCityID,int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, command);
	this->lMapID = lMapID;
	this->lCityID = lCityID;
}
//进入郡
inline void 
COMEIN_MAP::serialize(unsigned long lMapID, int from, int to)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, EN_C_COMEIN_MAP);
	this->lMapID = lMapID;
}

//城市列表信息
inline void 
CITY_LIST::serialize(int count, int from, int to)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this) + count * sizeof(tagCityList), 0, from, to, EN_S_CITY_LIST_INFO);
	this->iCount = count;
}

inline tagCityList & 
CITY_LIST::operator [](unsigned int index)
{
	return ((tagCityList *)((unsigned char *)this + sizeof(*this)))[index];
}
//郡状态信息
inline void 
MAP_STATE_LIST::serialize(int count, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this) + count * sizeof(tagMapState), 0, from, to, command);
	this->iCount = count;
}

inline tagMapState & 
MAP_STATE_LIST::operator [](unsigned int index)
{
	return ((tagMapState *)((unsigned char *)this + sizeof(*this)))[index];
}
//玩家城市列表信息
inline void 
PLAYER_CITY_LIST::serialize(int count, int from, int to)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this) + count * sizeof(tagPlayerCityList), 0, from, to, EN_S_PLAYER_CITY_LIST);
	this->iCount = count;
}

inline tagPlayerCityList & 
PLAYER_CITY_LIST::operator [](unsigned int index)
{
	return ((tagPlayerCityList *)((unsigned char *)this + sizeof(*this)))[index];
}
//郡内部队列表信息
inline void 
MAP_ARMY_LIST::serialize(int count, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this) + count * sizeof(tagArmysList), 0, from, to, command);
	this->iCount = count;
}

inline tagArmysList & 
MAP_ARMY_LIST::operator [](unsigned int index)
{
	return ((tagArmysList *)((unsigned char *)this + sizeof(*this)))[index];
}
//郡内哨塔等属性 
inline void 
CITY_ATTRIBUTE_LIST::serialize(int count, int from, int to)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this) + count * sizeof(tagCityAttributeList), 0, from, to, EN_S_CITY_ATTRIBUTE_LIST);
	this->iCount = count;
}

inline tagCityAttributeList & 
CITY_ATTRIBUTE_LIST::operator [](unsigned int index)
{
	return ((tagCityAttributeList *)((unsigned char *)this + sizeof(*this)))[index];
}
//建筑物表
inline void 
CONSTRUCTION_LIST::serialize(int count, int from, int to)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this) + count * sizeof(tagConstructionList), 0, from, to, EN_S_CONSTRUCTION_COUNT);
	this->iCount = count;
}

inline tagConstructionList & 
CONSTRUCTION_LIST::operator [](unsigned int index)
{
	return ((tagConstructionList *)((unsigned char *)this + sizeof(*this)))[index];
}
//玩家已建筑好的建筑物
inline void 
PLAYER_CONSTR_LIST::serialize(int count, int from, int to)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this) + count * sizeof(tagPlayerConstrList), 0, from, to, EN_S_CONSTRUCTION_COUNT);
	this->iCount = count;
}
inline tagPlayerConstrList & 
PLAYER_CONSTR_LIST::operator [](unsigned int index)
{
	return ((tagPlayerConstrList *)((unsigned char *)this + sizeof(*this)))[index];
}
//玩家正在建筑中的建筑物
inline void 
BUILDING_CONSTR_LIST::serialize(int count, int from, int to)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this) + count * sizeof(tagBuildingConstrList), 0, from, to, EN_S_BUILDING_CONSTR);
	this->iCount = count;
}
inline tagBuildingConstrList & 
BUILDING_CONSTR_LIST::operator [](unsigned int index)
{
	return ((tagBuildingConstrList *)((unsigned char *)this + sizeof(*this)))[index];
}
//正在生产中的道具
inline void 
PRODUCEING_ITEM_LIST::serialize(int count, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this) + count * sizeof(tagProduceingItemList), 0, from, to, command);
	this->iCount = count;
}

inline tagProduceingItemList & 
PRODUCEING_ITEM_LIST::operator [](unsigned int index)
{
	return ((tagProduceingItemList *)((unsigned char *)this + sizeof(*this)))[index];
}
//生产武器生列表
inline void 
PRODUCTION_WEAPON_LIST::serialize(int count, int from, int to)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this) + count * sizeof(tagProduction), 0, from, to, EN_S_PRODUCE_WEAPON_COUNT);
	this->iCount = count;
}

inline tagProduction & 
PRODUCTION_WEAPON_LIST::operator [](unsigned int index)
{
	return ((tagProduction *)((unsigned char *)this + sizeof(*this)))[index];
}
//生产道具列表
inline void 
PRODUCTION_DEFEND_LIST::serialize(int count, int from, int to)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this) + count * sizeof(tagProduction), 0, from, to, EN_S_PRODUCE_DEFEND_COUNT);
	this->iCount = count;
}

inline tagProduction & 
PRODUCTION_DEFEND_LIST::operator [](unsigned int index)
{
	return ((tagProduction *)((unsigned char *)this + sizeof(*this)))[index];
}
//驯兽技术生产
inline void 
PRODUCTION_ANIMAL_LIST::serialize(int count, int from, int to)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this) + count * sizeof(tagProduction), 0, from, to, EN_S_PRODUCE_ANIMAL_COUNT);
	this->iCount = count;
}

inline tagProduction & 
PRODUCTION_ANIMAL_LIST::operator [](unsigned int index)
{
	return ((tagProduction *)((unsigned char *)this + sizeof(*this)))[index];
}
//特殊技术生产
inline void 
PRODUCTION_SPECIAL_LIST::serialize(int count, int from, int to)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this) + count * sizeof(tagProduction), 0, from, to, EN_S_PRODUCE_SPECIAL_COUNT);
	this->iCount = count;
}

inline tagProduction & 
PRODUCTION_SPECIAL_LIST::operator [](unsigned int index)
{
	return ((tagProduction *)((unsigned char *)this + sizeof(*this)))[index];
}
//已经研究了的道具列表
inline void 
ITEM_RESEARCH_LIST::serialize(int count, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this) + count * sizeof(tagItemResearch), 0, from, to, command);
	this->iCount = count;
}

inline tagItemResearch & 
ITEM_RESEARCH_LIST::operator [](unsigned int index)
{
	return ((tagItemResearch *)((unsigned char *)this + sizeof(*this)))[index];
}
//正在研究中的道具
inline void 
RESEARCHING_ITEM_LIST::serialize(int count, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this) + count * sizeof(tagResearchingItemList), 0, from, to, command);
	this->iCount = count;
}

inline tagResearchingItemList & 
RESEARCHING_ITEM_LIST::operator [](unsigned int index)
{
	return ((tagResearchingItemList *)((unsigned char *)this + sizeof(*this)))[index];
}
//研究武器生列表
inline void 
RESEARCH_WEAPON_LIST::serialize(int count, int from, int to)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this) + count * sizeof(tagItemResearch), 0, from, to, EN_S_WEAPON_RESEARCH);
	this->iCount = count;
}

inline tagItemResearch & 
RESEARCH_WEAPON_LIST::operator [](unsigned int index)
{
	return ((tagItemResearch *)((unsigned char *)this + sizeof(*this)))[index];
}
//研究道具列表
inline void 
RESEARCH_DEFEND_LIST::serialize(int count, int from, int to)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this) + count * sizeof(tagItemResearch), 0, from, to, EN_S_DEFEND_RESEARCH);
	this->iCount = count;
}

inline tagItemResearch & 
RESEARCH_DEFEND_LIST::operator [](unsigned int index)
{
	return ((tagItemResearch *)((unsigned char *)this + sizeof(*this)))[index];
}
//研究驯兽技术列表
inline void 
RESEARCH_ANIMAL_LIST::serialize(int count, int from, int to)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this) + count * sizeof(tagItemResearch), 0, from, to, EN_S_ANIMAL_RESEARCH);
	this->iCount = count;
}

inline tagItemResearch & 
RESEARCH_ANIMAL_LIST::operator [](unsigned int index)
{
	return ((tagItemResearch *)((unsigned char *)this + sizeof(*this)))[index];
}
//研究特殊技术列表
inline void 
RESEARCH_SPECIAL_LIST::serialize(int count, int from, int to)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this) + count * sizeof(tagItemResearch), 0, from, to, EN_C_SPECIAL_RESEARCH);
	this->iCount = count;
}

inline tagItemResearch & 
RESEARCH_SPECIAL_LIST::operator [](unsigned int index)
{
	return ((tagItemResearch *)((unsigned char *)this + sizeof(*this)))[index];
}
//征兵参数
inline void 
LEVY_SOLDIER_PARAM::serialize(int count, int from, int to)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this) + count * sizeof(tagLevy_Soldier_Param), 0, from, to, EN_S_LEVY_SOLDIER_PARAM);
	this->iCount = count;
}

inline tagLevy_Soldier_Param & 
LEVY_SOLDIER_PARAM::operator [](unsigned int index)
{
	return ((tagLevy_Soldier_Param *)((unsigned char *)this + sizeof(*this)))[index];
}
//正处于招兵中...
inline void 
LEVYING_SOLDIER::serialize(int count, int from, int to)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this) + count * sizeof(tagLevying_Soldier), 0, from, to, EN_S_LEVYING_SOLDIER);
	this->iCount = count;
}

inline tagLevying_Soldier & 
LEVYING_SOLDIER::operator [](unsigned int index)
{
	return ((tagLevying_Soldier *)((unsigned char *)this + sizeof(*this)))[index];
}
//部队信息
inline void
ARMY_INFO::serialize(int iCount,int iDefendID, int iWeaponLevel,int iDefendLevel, int iExp,int from, int to)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, EN_S_SEND_ARMY_INFO);
	
	this->iCount = iCount;
	this->iDefendID = iDefendID;
	this->iWeaponLevel = iWeaponLevel;
	this->iDefendLevel = iDefendLevel;
	this->iExp = iExp;

}
//玩家指挥官列表信息
inline void 
PLAYER_OFFICER_LIST::serialize(int count, int from, int to,int command)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this) + count * sizeof(tagPlayer_Officer_List), 0, from, to, command);
	this->iCount = count;
}
inline tagPlayer_Officer_List & 
PLAYER_OFFICER_LIST::operator [](unsigned int index)
{
	return ((tagPlayer_Officer_List *)((unsigned char *)this + sizeof(*this)))[index];
}

//武器列表
inline void 
WEAPON_LIST::serialize(int count, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this) + count * sizeof(tagItem_List), 0, from, to, command);
	this->iCount = count;
}

inline tagItem_List & 
WEAPON_LIST::operator [](unsigned int index)
{
	return ((tagItem_List *)((unsigned char *)this + sizeof(*this)))[index];
}

//防具列表
inline void 
DEFEND_LIST::serialize(int count, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this) + count * sizeof(tagItem_List), 0, from, to, command);
	this->iCount = count;
}

inline tagItem_List & 
DEFEND_LIST::operator [](unsigned int index)
{
	return ((tagItem_List *)((unsigned char *)this + sizeof(*this)))[index];
}
//驯兽技术列表
inline void 
ANIMAL_LIST::serialize(int count, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this) + count * sizeof(tagItem_List), 0, from, to, command);
	this->iCount = count;
}

inline tagItem_List & 
ANIMAL_LIST::operator [](unsigned int index)
{
	return ((tagItem_List *)((unsigned char *)this + sizeof(*this)))[index];
}
//特殊工具列表
inline void 
SPECIAL_LIST::serialize(int count, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this) + count * sizeof(tagItem_List), 0, from, to, command);
	this->iCount = count;
}

inline tagItem_List & 
SPECIAL_LIST::operator [](unsigned int index)
{
	return ((tagItem_List *)((unsigned char *)this + sizeof(*this)))[index];
}
//部队列表
inline void 
ARMY_LIST::serialize(int count, int from, int to)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this) + count * sizeof(tagArmyList), 0, from, to, EN_S_ARMY_LIST);
	this->iCount = count;
}

inline tagArmyList & 
ARMY_LIST::operator [](unsigned int index)
{
	return ((tagArmyList *)((unsigned char *)this + sizeof(*this)))[index];
}
//物品列表
inline void 
GOODS_LIST::serialize(int count, int iGoodsType, int iPage, int from, int to)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this) + count * sizeof(tagGoodsList), 0, from, to, EN_S_MARKET_GOODS_LIST);
	this->iCount = count;
	this->iGoodsType = iGoodsType;
	this->iPage = iPage;
}

inline tagGoodsList & 
GOODS_LIST::operator [](unsigned int index)
{
	return ((tagGoodsList *)((unsigned char *)this + sizeof(*this)))[index];
}



//部队在城市地图上移动
inline void
ARMY_MOVE_MAP::serialize(unsigned long lMapID, unsigned long lArmyID, int iStartX, int iStartY,  int iEndX, int iEndY, int iflag, int from, int to, int command)
{
	 DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, command);
	 this->lMapID = lMapID;
	 this->lArmyID = lArmyID;
	 this->iStartX = iStartX;
	 this->iStartY = iStartY;
	  this->iEndX = iEndX;
	 this->iEndY = iEndY;
	 this->iflag = iflag;
}
//征兵数量
inline void
LEVY_SOLDIER_COUNT::serialize(unsigned long lMapID,unsigned long lCityID,int iRace,int iCount,int from, int to)
{
	 DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, EN_C_LEVY_SOLDIER_COUNT);
	 this->lMapID = lMapID;
	 this->lCityID = lCityID;
	 this->iRace = iRace;
	 this->iCount = iCount;
}
//进入征兵状态中
inline void
LEVY_SOLDIER_INFO::serialize(int iRace,int iCount, int iState, int from, int to)
{
	 DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, EN_S_LEVY_SOLDIER_INFO);
	 this->iRace = iRace;
	 this->iCount = iCount;
	 this->iState = iState;
}
//创建部队
inline void
CREATE_ARMY::serialize(unsigned long lMapID, unsigned long lCityID, unsigned long lOfficerID, char szName[],int iRaceID, int iSoldiertype, int iFSoldierCount, 
						int iFWeaponID,int iWeaponLevel, int iDefendID, int iDefendLevel, int iX, int iY, int from, int to)
{
	 DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, EN_C_CREATE_ARMY);
	 this->lMapID = lMapID;
	 this->lCityID = lCityID;
	 this->lOfficerID = lOfficerID;
	 this->iRaceID = iRaceID;
	 this->iSoldiertype = iSoldiertype;
	 this->iFSoldierCount = iFSoldierCount;
	 this->iFWeaponID = iFWeaponID;
	  this->iWeaponLevel = iWeaponLevel;
	 this->iDefendID = iDefendID;
	 this->iDefendLevel = iDefendLevel;
	 this->iX = iX;
	 this->iY = iY;
	 strcpy(this->szName, szName);
}
//组编部队
inline void
EDIT_ARMY::serialize(unsigned long lMapID, unsigned long lCityID, unsigned long lOfficerID, 
		int iArmyID, int iDefendID, int iCount, int from, int to)
{
	 DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, EN_C_EDIT_ARMY);
	 this->lMapID = lMapID;
	 this->lCityID = lCityID;
	 this->lOfficerID = lOfficerID;
	 this->iArmyID = iArmyID;
	 this->iDefendID = iDefendID;
	 this->iCount = iCount;
}
//创建部队失败
inline void 
CREATE_ARMY_FAILD::serialize(int iError, int from, int to, int command)
{
	 DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, EN_S_CREATE_ARMY_FAILD);
	 this->iError = iError;
}

//解散部队EN_C_UNLAYARMY,EN_S_UNLAYARMY,
inline void tagUnlayArmy_Request::serialize(int playerid,int mapid,int armyid)
{
	//MapID = mapid;
	ArmyID = armyid;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,mapid,EN_C_UNLAYARMY);
}

inline void tagUnlayArmy_Response::serialize(int agent,int mapid,int playerid,int code)
{
	Error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,mapid,playerid,EN_S_UNLAYARMY);
}

//删除部队EN_C_DELETEARMY,EN_S_DELETEARMY,
inline void tagDeleteArmy_Request::serialize(int playerid,int mapid,int armyid)
{
	//MapID = mapid;
	ArmyID = armyid;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,mapid,EN_C_DELETEARMY);
}

inline void tagDeleteArmy_Response::serialize(int agent,int mapid,int playerid,int code)
{
	Error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,mapid,playerid,EN_S_DELETEARMY);
}
//英雄系统信息
inline void
HERO_SYSTEM_INFO::serialize(int iCode, int from, int to,  int command)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, command);
	this->iCode = iCode;
}
//创建指挥官
inline void
CREATE_OFFICER::serialize(char szNick[], int iFace, int iCountry, int iRaceID, int iJobID, int from, int to )
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, EN_C_CREATE_OFFICER);
	strcpy(this->szNick, szNick);
	this->iFace = iFace;
	this->iCountry = iCountry;
	this->iRaceID = iRaceID;
	this->iJobID = iJobID;
}
//创建指挥官失败
inline void 
CREATE_OFFICER_FAILD::serialize(int iError, int from, int to, int command)
{
	 DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, EN_S_CREATE_OFFICER_FAILD);
	 this->iError = iError;
}
//开除英雄
inline void
DISMISS_HERO::serialize(unsigned long lHeroID, int from, int to)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, EN_C_DISMISS_HERO);
	this->lHeroID = lHeroID;
}
//开除英雄信息
inline void
DISMISS_HERO_INFO::serialize(int iCode, int from, int to,  int command)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, command);
	this->iCode = iCode;
}
//获取部队信息
inline void
GET_ARMY_INFO::serialize(unsigned long lMapID, unsigned long lCityID, unsigned long lArmyID,int from, int to)
{
	 DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, EN_C_GET_ARMY_INFO);
	 this->lMapID = lMapID;
	 this->lCityID = lCityID;
	 this->lArmyID = lArmyID;
}
//建筑建筑物
inline void
BUILD_CONSTRUCTION::serialize(unsigned long lMapID,unsigned long lCityID,unsigned long lSortID,unsigned long lConstructionID,int iLevel,int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, command);
	this->lSortID = lSortID;
	this->lMapID = lMapID;
	this->lCityID = lCityID;
	this->lConstructionID = lConstructionID;
	this->iLevel = iLevel;
	
}
//拆毁建筑物
inline void
DEMOLISH_CONSTRUCTION::serialize(unsigned long lMapID, unsigned long lCityID, unsigned long lPlayerID,int from, int to)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, EN_C_DEMOLISH_CONSTRUCTION);
	this->lMapID = lMapID;
	this->lCityID = lCityID;
	this->lPlayerID = lPlayerID;
}
//取消建筑物建筑
inline void
CANCEL_BUILD::serialize(unsigned long lMapID, unsigned long lCityID, unsigned long lSortID, unsigned long lConID,int iLevel, int from, int to)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, EN_C_CANCEL_BUILD);
	this->lMapID = lMapID;
	this->lCityID = lCityID;
	this->lSortID = lSortID;
	this->lConID = lConID;
	this->iLevel = iLevel;
}
//取消生产道具
inline void
CANCEL_PRODUCE::serialize(unsigned long lMapID, unsigned long lCityID, unsigned long lSortID,int iType, int iItemID,int iLevel, int iCount, int from, int to)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, EN_C_CANCEL_PRODUCE);
	this->lMapID = lMapID;
	this->lCityID = lCityID;
	this->lSortID = lSortID;
	this->iType = iType;
	this->iItemID = iItemID;
	this->iLevel = iLevel;
	this->iCount = iCount;
}
//取消科研道具
inline void 
CANCEL_RESEARCH::serialize(int iType, int iItemID,int iLevel,int from, int to)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, EN_C_CANCEL_RESEARCH);
	this->iType = iType;
	this->iItemID = iItemID;
	this->iLevel = iLevel;
}
//生产道具
inline void
PRODUCT_ITEM::serialize(unsigned long lMapID,unsigned long lCityID, int iType, int iItem, int iLevel, int iCount,int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, command);
	this->lMapID = lMapID;
	this->lCityID = lCityID;
	this->iType = iType;
	this->iItem = iItem;
	this->iLevel = iLevel;
	this->iCount = iCount;
	
}
//研究
inline void
ITEM_RESEARCH::serialize(int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, command);
}
//研究道具
inline void
RESEARCH_ITEM::serialize(int iType, int iItemID, int iLevel, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, command);
	this->iType = iType;
	this->iItemID = iItemID;
	this->iLevel = iLevel;
}
//国家市场信息
inline void
COUNTRY_MARKET_INFO::serialize(int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, command);
}
//国家市场物品买卖
inline void
COUNTRY_MARKET_BUSINESS::serialize(unsigned long lMapID, unsigned long lCityID, int iType, int iSaleID, int iLevel, int iCount,  int iMode, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, command);
	this->lMapID = lMapID;
	this->lCityID = lCityID;
	this->iType = iType;
	this->iSaleID = iSaleID;
	this->iLevel = iLevel;
	this->iCount = iCount;
	this->iMode = iMode;
}
//民间市场信息
inline void
PEOPLE_MARKET::serialize(unsigned long lSMapID, unsigned long lSCityID, int iType, int iMarketType, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, command);
	this->lSMapID = lSMapID;
	this->lSCityID = lSCityID;
	this->iType = iType;
	this->iMarketType = iMarketType;
}
//物品挂牌
inline void PUT_UP::serialize(unsigned long lMapID, unsigned long lCityID, int iType, int iSaleID, unsigned long lSortID, 
		int iCount, int iMinPrice, int iOncePrice, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, command);
	this->lMapID = lMapID;
	this->lCityID = lCityID;
	this->iType = iType;
	this->iSaleID = iSaleID;
	this->lSortID = lSortID;
	this->iCount = iCount;
	this->iMinPrice = iMinPrice;
	this->iOncePrice = iOncePrice;
}
//出价
inline void GOODS_BIDDEN::serialize(unsigned long lMapID, unsigned long lCityID, int iType, unsigned long lSortID, int iCurrentPrice, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, command);
	this->lMapID = lMapID;
	this->lCityID = lCityID;
	this->iType = iType;
	this->lSortID = lSortID;
	this->iCurrentPrice = iCurrentPrice;
}
//购买物品
inline void BUY_GOODS::serialize(unsigned long lMapID, unsigned long lCityID, int iType, unsigned long lSortID, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, command);
	this->lMapID = lMapID;
	this->lCityID = lCityID;
	this->iType = iType;
	this->lSortID = lSortID;
//	this->iLevel = iLevel;
}
//广播捐献信息
inline void BROADCAST_CONTRIBUTE::serialize(char szPlayerName[], unsigned long lMapID,unsigned long lCityID, int iLevel, int iType, int iMoney,int iCredit, int from, int to)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, EN_S_BROADCAST_CONTRIBUTE);
	this->lMapID = lMapID;
	this->lCityID = lCityID;
	this->iLevel = iLevel;
	this->iType = iType;
	this->iMoney = iMoney;
	this->iCredit = iCredit;
	strcpy(this->szPlayerName, szPlayerName);
}
//捐献
inline void PLAYER_CONTRIBUTE::serialize(unsigned long lMapID,unsigned long lCityID, int iMoney, int iType, int from, int to)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, EN_C_CONTRIBUTE);
	this->lMapID = lMapID;
	this->lCityID = lCityID;
	//this->lConID = lConID;
	this->iType = iType;
	this->iMoney = iMoney;
}
//城市信息
inline void MAP_CITY_INFO::serialize(int iFoodCount,int iWoodCount,int iIronCount,int iSkinCount,
		int iHuman,int iMannikin,int iFairy,int iDandiprat,int iHalfAnimal,int iCattle,int iBlackFairy,int iDecPerson,
		int from, int to)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, EN_S_SEND_CITY_INFO);
	this->iFoodCount = iFoodCount;
	this->iWoodCount = iWoodCount;
	this->iSkinCount = iSkinCount;
	this->iIronCount = iIronCount;
	this->iHuman = iHuman;
	this->iMannikin = iMannikin;
	this->iFairy = iFairy;
	this->iDandiprat = iDandiprat;
	this->iHalfAnimal = iHalfAnimal;
	this->iCattle = iCattle;
	this->iBlackFairy = iBlackFairy;
	this->iDecPerson = iDecPerson;
}
//部队转移
inline void
ARMY_TRANSFER::serialize(unsigned long lRMapID, unsigned long lTargetMapID, unsigned long  lArmyID, int from, int to)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, EN_C_ARMYS_TRANSFER);
	this->lRMapID = lRMapID;
	this->lTargetMapID = lTargetMapID;
	this->lArmyID = lArmyID;
}
//玩家的兵种数
inline void
PLAYER_SOLDIER_LIST::serialize(int iHuman, int iFairy, int iMannikin, int iDandiprat, int iHalfAnimal, int iCattle, int iBlackFairy, int iDecPerson, int from, int to)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, EN_S_ALL_SOLDIERS);
	this->iHuman = iHuman;
	this->iFairy = iFairy;
	this->iMannikin = iMannikin;
	this->iDandiprat = iDandiprat;
	this->iHalfAnimal = iHalfAnimal;
	this->iCattle = iCattle;
	this->iBlackFairy = iBlackFairy;
	this->iDecPerson = iDecPerson;
}

//部队城市地图上可移动的坐标列表
inline void 
ARMY_CAN_MOVE_LIST::serialize(int count, int from, int to)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this) + count * sizeof(tagArmyMoveList), 0, from, to, EN_S_ARMY_MOVE);
	this->iCount = count;
}

inline tagArmyMoveList & 
ARMY_CAN_MOVE_LIST::operator [](unsigned int index)
{
	return ((tagArmyMoveList *)((unsigned char *)this + sizeof(*this)))[index];
}
//修改指挥官名字
inline void
MODIFY_OFFICER_NAME::serialize(unsigned long lOfficerID, char szOfficerName[],int from, int to)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, EN_C_MODIFY_OFFICER_NAME);
	this->lOfficerID = lOfficerID;
	strcpy(this->szOfficerName, szOfficerName);
}
//成功信息码
inline void 
SUCCEED_INFO::serialize(unsigned long lCityID, unsigned long lSortID, int iEventID, int iLevel, int iCount, int iCode, int from, int to)
{
	 DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, EN_S_SUCCEED_INFO);
	 this->lCityID = lCityID;
	 this->lSortID = lSortID;
	 this->iEventID = iEventID;
	 this->iLevel = iLevel;
	 this->iCount = iCount;
	 this->iCode = iCode;
}
//市场信息码
inline void 
MARKET_INFO::serialize(int iTypeID, int iGoodsID, int iCount, int iCode, int from, int to)
{
	 DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, EN_S_MARKET_INFO);
	 this->iTypeID = iTypeID;
	 this->iGoodsID = iGoodsID;
	 this->iCount = iCount;
	 this->iCode = iCode;
}
//错误信息
inline void 
ERROR_INFO::serialize(int iCode, int from, int to)
{
	 DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, EN_S_ERROR_INFO);
	 this->iCode = iCode;
}
//退出游戏
inline void
EXIT_MAP_SERVER::serialize(int from, int to)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, EN_C_EXIT_MAP_SERVER);
}
//退出游戏
inline void
EXIT_GAME::serialize(unsigned long lPlayerID, unsigned long lMapID, int from, int to)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, EN_C_EXIT_MAP_SERVER);
	this->lPlayerID = lPlayerID;
	this->lMapID = lMapID;
}
//指挥官技能
inline void 
OFFICER_TECH_LIST::serialize(int officerid,int count, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this) + count * sizeof(tagOfficerTechList), 0, from, to, command);
	this->lOfficerID = officerid;
	this->iCount = count;
}

inline tagOfficerTechList & 
OFFICER_TECH_LIST::operator [](unsigned int index)
{
	return ((tagOfficerTechList *)((unsigned char *)this + sizeof(*this)))[index];
}

//学习指挥官技能
inline void 
STUDY_OFFICER_TECH::serialize(unsigned long lOfficerID, int iTechID, int iType, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, command);
	this->lOfficerID = lOfficerID;
	this->iTechID = iTechID;
	this->iType = iType;
}
//指挥官信息
inline void
GET_OFFICER_INFO:: serialize(unsigned long lOfficerID,int from, int to, int command)	
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, command);
	this->lOfficerID = lOfficerID;
}
//指挥官属性
inline void
OFFICER_ATTRIBUTE::serialize(int officerid,int iCredit, int iStamina, int iPower, int iCelerity, int iWit, int iAttack,
		int iPhysicRecovery, int iMagicRecovery,  int iLevel, int iMilitaryRank_, int iExp,int techdot,int iActionDot, 
		int iEquipStamina,int iEquipPower,int iEquipCelerity,int iEquipWit,int iHeroHP,int iMagicAttack,int from, int to)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, EN_S_OFFICERO_ATTRIBUTE);
	this->OfficerID = officerid;
	this->iCredit = iCredit;
	this->iStamina = iStamina;
	this->iPower = iPower;
	this->iCelerity = iCelerity;
	this->iWit = iWit;
	this->iAttack = iAttack;
	this->iPhysicRecovery = iPhysicRecovery;
	this->iMagicRecovery  = iMagicRecovery;
	this->iLevel = iLevel;
	this->iMilitaryRank_ = iMilitaryRank_;
	this->Exp = iExp;
	this->TechDot = techdot;
	this->ActionDot = iActionDot;
	this->EquipStamina = iEquipStamina;
	this->EquipPower = iEquipPower;
	this->EquipCelerity = iEquipCelerity;
	this->EquipWit = iEquipWit;
	this->HeroHP = iHeroHP;
	this->MagicAttack = iMagicAttack;
}
//英雄装备列表
inline void 
HERO_EQUIP_LIST::serialize(int count, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this) + count * sizeof(tagHeroEquipList), 0, from, to, command);
	this->iCount = count;
}

inline tagHeroEquipList & 
HERO_EQUIP_LIST::operator [](unsigned int index)
{
	return ((tagHeroEquipList *)((unsigned char *)this + sizeof(*this)))[index];
}
//操作背包中的装备
inline void 
EXCHANGE_EQUIP_LIST::serialize(int count, int from, int to)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this) + count * sizeof(tagExchangeEquipList), 0, from, to, EN_C_PACK_EXCHANGE);
	this->iCount = count;
}

inline tagExchangeEquipList & 
EXCHANGE_EQUIP_LIST::operator [](unsigned int index)
{
	return ((tagExchangeEquipList *)((unsigned char *)this + sizeof(*this)))[index];
}
//赎回英雄
inline void
REDEEM_HERO:: serialize(unsigned long lHeroID, int iMoney, int from, int to)	
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, EN_C_REDEEM_HERO);
	this->lHeroID = lHeroID;
	this->iMoney = iMoney;
}
//操作装备
inline void
EXCHANGE_EQUIP_INFO::serialize(int iCode, int iSortID, int from, int to)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, EN_S_EXCHANGE_EQUIP);
	this->iCode = iCode;
	this->iSortID = iSortID;
//	this->iItemID = iItemID;
}
//销毁装备
inline void
DESTROY_EQUIP::serialize(unsigned long lHeroID, int iSortID, int count,int from, int to)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, EN_C_DESTROY_EQUIP);
	this->lHeroID = lHeroID;
	this->iSortID = iSortID;
	this->Count = count;
}

//英雄战争记录
inline void 
HERO_WAR_RECORD_LIST::serialize(int count, int from, int to)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this) + count * sizeof(tagHeroWarRecord), 0, from, to, EN_S_HERO_WAR_RECORD);
	this->iCount = count;
}

inline tagHeroWarRecord & 
HERO_WAR_RECORD_LIST::operator [](unsigned int index)
{
	return ((tagHeroWarRecord *)((unsigned char *)this + sizeof(*this)))[index];
}
//英雄简成长记录
inline void 
HERO_GROWUP_RECORD_LIST::serialize(int count, int from, int to)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this) + count * sizeof(tagHeroGrowUpRecord), 0, from, to, EN_S_HERO_GROWUP_RECORD);
	this->iCount = count;
}

inline tagHeroGrowUpRecord & 
HERO_GROWUP_RECORD_LIST::operator [](unsigned int index)
{
	return ((tagHeroGrowUpRecord *)((unsigned char *)this + sizeof(*this)))[index];
}
//游戏中的时间
inline void
GAME_TIME::serialize(long long lGameTime, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, command);
	this->lGameTime = lGameTime;
}
//装载/卸载物资
inline void 
MATERIAL_LIST::serialize(int count, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this) + count * sizeof(tagMaterials), 0, from, to, command);
	this->iCount = count;
	this->lArmyID = lArmyID;
	this->lMapID = lMapID;
	this->lCityID = lCityID;
}
inline tagMaterials & 
MATERIAL_LIST::operator [](unsigned int index)
{
	return ((tagMaterials *)((unsigned char *)this + sizeof(*this)))[index];
}
//殖民城市
inline void
COLONIZE_CITY::serialize(unsigned long lArmyID, unsigned long lMapID, unsigned long lCityID, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, command);
	this->lArmyID = lArmyID;
	this->lMapID = lMapID;
	this->lCityID = lCityID;
}
//物资查看
inline void
LOOK_MATERIALS::serialize(unsigned long lArmyID, unsigned long lMapID, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, command);
	this->lArmyID = lArmyID;
	this->lMapID = lMapID;
}
//军衔晋升
inline void
MILITARY_RANK::serialize(char szName[], int iRank, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, command);
	strcpy(this->szName, szName);
	this->iRank = iRank;
}
//英雄被俘
inline void
CAPTIVE_HERO::serialize(char szByPlayerName[], char szByHeroName[], char szPlayerName[], int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, command);
	strcpy(this->szByPlayerName, szByPlayerName);
	strcpy(this->szByHeroName, szByHeroName);
	strcpy(this->szPlayerName, szPlayerName);
}
//进入地图
inline void
COMEIN_EXIT_MAP::serialize(char szName[], unsigned long lMapID, int iType, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, command);
	strcpy(this->szName, szName);
	this->lMapID = lMapID;
	this->iType = iType;
}
//部队达到预警
inline void
ARMY_REACH_WARM ::serialize( unsigned long lMapID,int from, int to,int unioncnt,int clancnt,int aboriginalcnt)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, EN_S_BROADCAST_ARMY_WARM);
	this->lMapID = lMapID;
	this->UnionCount = unioncnt;
	this->ClanCount = clancnt;
	this->AboriginalCount = aboriginalcnt;
}
inline void
CONTRIBUTE_INFO::serialize(unsigned long lCityID, int iLevel, int iCurMoney, int iTolMoney, int iAttack, int iHP, int iAttackScop, int iVisualRange, long lCMoney, long lPMoney, int from, int to)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, EN_S_CONTRIBUTE_INFO);
	this->lCityID = lCityID;
	this->iLevel = iLevel;
	this->iCurMoney = iCurMoney;
	this->iTolMoney = iTolMoney;
	this->iAttack = iAttack;
	this->iHP = iHP;
	this->iAttackScop = iAttackScop;
	this->iVisualRange = iVisualRange;
	this->lCMoney = lCMoney;
	this->lPMoney = lPMoney;
}
//部队行走路径
inline void 
ARMY_MOVE_PATH::serialize(int count, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this) + count * sizeof(tagOutingArmysList), 0, from, to, command);
	this->iCount = count;
}
inline tagOutingArmysList & 
ARMY_MOVE_PATH::operator [](unsigned int index)
{
	return ((tagOutingArmysList *)((unsigned char *)this + sizeof(*this)))[index];
}
//部队不能经过
inline void 
ARMY_CANT_MOVE ::serialize(int iCode, int from, int to)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, EN_S_ARMY_CANT_MOVE);
	this->iCode = iCode;			//信息码
}
//资源采集率
inline void
RESOURCE_RATE::serialize(int iFoodRate, int iWoodRate, int iSkinRate, int iIronRate, int iScience, int from, int to,int cmd)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, cmd);
	this->iFoodRate = iFoodRate;
	this->iWoodRate = iWoodRate;
	this->iSkinRate = iSkinRate;
	this->iIronRate = iIronRate;
	this->iScience = iScience;
}


//增加一个新的宝箱到英雄背包EN_C_NEWTREASUREPACK,EN_S_NEWTREASUREPACK,
inline void tagNewTreasurePack_Request::serialize(int playerid,int officerid,int itemid,int type)
{
	OfficerID = officerid;
	ItemID = itemid;
	ItemType = type;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,SERVER_BOUND_MAP_OTHER,EN_C_NEWTREASUREPACK);
}

inline void tagNewTreasurePack_Response::serialize(int agent,int playerid,int code)
{
	Error_Code = code;
	if (code == 0)
	{
		DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,SERVER_BOUND_MAP_OTHER,playerid,EN_S_NEWTREASUREPACK);
	}
	else
	{
		DISTRIBUTED_HEADER::serialize(sizeof(*this) - sizeof(tagHeroEquipList),agent,SERVER_BOUND_MAP_OTHER,playerid,EN_S_NEWTREASUREPACK);
	}
}

//打开宝箱EN_C_OPENTREASUREPACK,EN_S_OPENTREASUREPACK,
inline void tagOpenTreasurePack_Request::serialize(int playerid,int mapid,int cityid,int officerid,int sortid)
{
	MapID = mapid;
	CityID = cityid;
	OfficerID = officerid;
	SortID = sortid;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,SERVER_BOUND_MAP_OTHER,EN_C_OPENTREASUREPACK);
}

inline void tagOpenTreasurePack_Response::serialize(int agent,int playerid,int code,int money,int itemid,int randcount,
													int equipid,int sortid,int level,int pos,int showpos,int type,int equipcount)
{
	Error_Code = code;
	Money = money;
	ItemID = itemid;
	Count = randcount;
	//EquipItem.iItemID = equipid;
	EquipItem.InlayItem.NewItemID = equipid;
	EquipItem.iSortID = sortid;
	EquipItem.iLevel = level;
	EquipItem.iPos = pos;
	EquipItem.iShowPos = showpos;
	EquipItem.iType = type;
	EquipItem.iCount = equipcount;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,SERVER_BOUND_MAP_OTHER,playerid,EN_S_OPENTREASUREPACK);
}

//英雄重伤通知EN_C_HEROGBH_NOTIFY
inline void tagHeroGBH_Notify::serialize(int playerid,int officerid)
{
	OfficerID = officerid;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,SERVER_BOUND_MAP_OTHER,EN_C_HEROGBH_NOTIFY);
}

inline void tagHeroGBH_Broadcast::serialize(const char *playername,const char *officername)
{
	if(playername)
	{
		strcpy(PlayerName,playername);
	}
	if(officername)
	{
		strcpy(OfficerName,officername);
	}
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,0,0,EN_S_HEROGBH_NOTIFY);
}

inline void ReqHerosHangSys::serialize(unsigned long doorId, int from, int to)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, EN_REQ_HEROS_HANGINGSYS);
	this->doorId = doorId;
}

inline void RspHerosHangSys::serialize(unsigned int count, int agent, int from, int to)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this) + count * sizeof(tagHangingItemList), agent, from, to, EN_RSP_HEROS_HANGINGSYS);
	this->count = count;
}

inline tagHangingItemList &RspHerosHangSys::operator [](unsigned int index)
{
	return ((tagHangingItemList *)((unsigned char *)this + sizeof(*this)))[index];
}

inline void ReqStartHangSystemOneHero::serialize(unsigned long heroID, unsigned int hangSysType, unsigned int trainingTime, int from, int to)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, EN_REQ_HANG_ONEHERO);
	this->heroID = heroID;
	this->hangSysType = hangSysType;
	this->trainingTime = trainingTime;
}

inline void RspStartHangSystemOneHero::serialize(unsigned int ret, unsigned long heroId, unsigned int completeTime, unsigned long expVal, unsigned long moneyVal, int agent, int from, int to)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), agent, from, to, EN_RSP_HANG_ONEHERO);
	this->ret = ret;
	this->heroId = heroId;
	this->completeTime = completeTime;
	this->expVal = expVal;
	this->moneyVal = moneyVal;
}

inline void ReqCancelHangOneHero::serialize(unsigned long heroID, int from, int to)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, EN_REQ_CANCEL_HANG_ONEHERO);
	this->heroID = heroID;
}

inline void RspCancelHangOneHero::serialize(unsigned int ret, int agent, int from, int to)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), agent, from, to, EN_RSP_CANCEL_HANG_ONEHERO);
	this->ret = ret;
}

inline void RspHangOneHeroFinish::serialize(unsigned long heroID, int agent, int from, int to)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), agent, from, to, EN_RSP_TIMEOUT_HANG_ONEHERO);
	this->heroID = heroID;
}


#endif

