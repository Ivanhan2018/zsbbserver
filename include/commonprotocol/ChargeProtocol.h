#ifndef CHARGEPROTOCOL_H__
#define CHARGEPROTOCOL_H__
#pragma warning(disable:4244)
#include <string.h>
#include "Distributed_Header.h"
#include "Global_Macro.h"
#pragma pack(push,1)
#define DEFAULT_AGENT		4000
#define MAX_PACKNUM			60

enum ExchangeType
{
	GO2Money = 0,					//Go点兑换金币
	Money2GO						//金币兑换Go点
};
enum LogType
{
	BuyCard_Log = 0,				//买卡日志格式
	UseCard_Log,					//使用卡日志格式
	UseAutoCard_Log,				//使用托管卡日志格式
	ImmedComplete_Log,				//立即完成日志格式
	Election_Log,					//参加竞选日志格式
	AddGODian_Log,					//奖励Go点日志格式
	ImmedResourceRate_Log,			//提高生产率的日志格式
	UpdateConsortia_Log,			//升级军团扣除GO点
};


//查询Go点CGC_C_QUERYMONEY,CGC_S_QUERYMONEY
typedef struct tagQueryMoney_Request:DISTRIBUTED_HEADER
{
	inline void serialize(int playerid);
}QueryMoney_Request,*LPQueryMoney_Request;
inline void tagQueryMoney_Request::serialize(int playerid)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,MDM_GP_MAP_CHARGE,CGC_C_QUERYMONEY);
}

typedef struct tagQueryMoney_Response:DISTRIBUTED_HEADER
{
	int Money;					//玩家的Go点，<0时表示失败，>=0为Go点
	inline void serialize(int agent,int playerid,int money);
}QueryMoney_Response,*LPQueryMoney_Response;
inline void tagQueryMoney_Response::serialize(int agent,int playerid,int money)
{
	Money = money;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CHARGE,playerid,CGC_S_QUERYMONEY);
}

//使用卡的基本消息结构
typedef struct tagBaseUseCard_Request:DISTRIBUTED_HEADER
{
	int OfficerID;					//道具所在背包的英雄ID，公共背包是为1
	int SortID;						//流水号
	//int ItemID;						//道具ID
	inline void serialize(int length,int playerid,int officerid,int sortid,int cmd);
}BaseUseCard_Request,*LPBaseUseCard_Request;
inline void tagBaseUseCard_Request::serialize(int length,int playerid,int officerid,int sortid,int cmd)
{
	OfficerID = officerid;
	SortID = sortid;
	//ItemID = itemid;
	DISTRIBUTED_HEADER::serialize(length,0,playerid,MDM_GP_MAP_CHARGE,cmd);
}

//购买卡CGC_C_BUYCARD/CGC_S_BUYCARD,	
typedef struct tagBuyCard_Request:DISTRIBUTED_HEADER
{
	int ItemID;						//道具ID（卡ID）
	inline void serialize(int playerid,int itemid);
}BuyCard_Request,*LPBuyCard_Request;
inline void tagBuyCard_Request::serialize(int playerid,int itemid)
{
	ItemID = itemid;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,MDM_GP_MAP_CHARGE,CGC_C_BUYCARD);
}

typedef struct tagBuyCard_Response:DISTRIBUTED_HEADER
{
	int Error_Code;
	inline void serialize(int agent,int playerid,int code);
}BuyCard_Response,*LPBuyCard_Response;
inline void tagBuyCard_Response::serialize(int agent,int playerid,int code)
{
	Error_Code = code;					//操作结果，0：成功，1：Go点不足；2：背包已满；111：其他
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CHARGE,playerid,CGC_S_BUYCARD);
}

//使用基本资源卡，购买基本资源CGC_C_RESOURCECARD，CGC_S_RESOURCECARD
typedef struct tagUseResourceCard_Request:tagBaseUseCard_Request
{
	int MapID;
	int CityID;
	inline void serialize(int playerid,int officerid,int MapID,int CityID,int sortid);
}UseResourceCard_Request,*LPUseResourceCard_Request;
inline void tagUseResourceCard_Request::serialize(int playerid,int officerid,int mapid,int cityid,int sortid)
{
	MapID = mapid;
	CityID = cityid;
	//DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,MDM_GP_MAP_CHARGE,CGC_C_RESOURCECARD);
	tagBaseUseCard_Request::serialize(sizeof(*this),playerid,officerid,sortid,CGC_C_RESOURCECARD);
}

typedef struct tagUseResourceCard_Response:DISTRIBUTED_HEADER
{
	int Error_Code;					//操作结果，0：成功，1:玩家没有该道具；2:仓库容量不够；5:不能使用；111：其他
	inline void serialize(int agent,int playerid,int code);
}UseResourceCard_Response,*LPUseResourceCard_Response;
inline void tagUseResourceCard_Response::serialize(int agent,int playerid,int code)
{
	Error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CHARGE,playerid,CGC_S_RESOURCECARD);
}

//生产加速卡CGC_C_RESOURCESPEEDCARD,CGC_S_RESOURCESPEEDCARD,
typedef struct tagUseResourceSpeedCard_Request:tagBaseUseCard_Request
{
	inline void serialize(int playerid,int officerid,int sortid);
}UseResourceSpeedCard_Request,*LPUseResourceSpeedCard_Request;
inline void tagUseResourceSpeedCard_Request::serialize(int playerid,int officerid,int sortid)
{
	tagBaseUseCard_Request::serialize(sizeof(*this),playerid,officerid,sortid,CGC_C_RESOURCESPEEDCARD);
}

typedef struct tagUseResourceSpeedCard_Response:DISTRIBUTED_HEADER
{
	int Error_Code;					//操作结果，0：成功，1:玩家没有该道具；5:不能使用；111：其他
	inline void serialize(int agent,int playerid,int code);
}UseResourceSpeedCard_Response,*LPUseResourceSpeedCard_Response;
inline void tagUseResourceSpeedCard_Response::serialize(int agent,int playerid,int code)
{
	Error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CHARGE,playerid,CGC_S_RESOURCESPEEDCARD);
}

//招募英雄卡CGC_C_RESRUITHEROCARD,CGC_S_RESRUITHEROCARD,
typedef struct tagUseResruitHeroCard_Request:tagBaseUseCard_Request
{
	inline void serialize(int playerid,int officerid,int sortid);
}UseResruitHeroCard_Request,*LPUseResruitHeroCard_Request;
inline void tagUseResruitHeroCard_Request::serialize(int playerid,int officerid,int sortid)
{
	tagBaseUseCard_Request::serialize(sizeof(*this),playerid,officerid,sortid,CGC_C_RESRUITHEROCARD);
}

typedef struct tagUseResruitHeroCard_Response:DISTRIBUTED_HEADER
{
	int Error_Code;					//操作结果，0：成功，1:玩家没有该道具；5:不能使用；111：其他
	inline void serialize(int agent,int playerid,int code);
}UseResruitHeroCard_Response,*LPUseResruitHeroCard_Response;
inline void tagUseResruitHeroCard_Response::serialize(int agent,int playerid,int code)
{
	Error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CHARGE,playerid,CGC_S_RESRUITHEROCARD);
}

//特殊技能卡CGC_C_SPECIALSKILLCARD,	CGC_S_SPECIALSKILLCARD,
typedef struct tagUseSpecialSkillCard_Request:tagBaseUseCard_Request
{
	int HeroID;					//学习特殊技能的英雄ID
	inline void serialize(int playerid,int officerid,int sortid,int heroid);
}UseSpecialSkillCard_Request,*LPUseSpecialSkillCard_Request;
inline void tagUseSpecialSkillCard_Request::serialize(int playerid,int officerid,int sortid,int heroid)
{
	HeroID = heroid;
	tagBaseUseCard_Request::serialize(sizeof(*this),playerid,officerid,sortid,CGC_C_SPECIALSKILLCARD);
}

typedef struct tagUseSpecialSkillCard_Response:DISTRIBUTED_HEADER
{
	int Error_Code;					//操作结果，0：成功，1:玩家没有该道具；2:英雄技能已满；5:不能使用；111：其他
	inline void serialize(int agent,int playerid,int code);
}UseSpecialSkillCard_Response,*LPUseSpecialSkillCard_Response;
inline void tagUseSpecialSkillCard_Response::serialize(int agent,int playerid,int code)
{
	Error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CHARGE,playerid,CGC_S_SPECIALSKILLCARD);
}

//建造、科研、征兵加速卡CGC_C_OTHERSPEEDCARD,CGC_S_OTHERSPEEDCARD,
typedef struct tagUseOtherSpeedCard_Request:tagBaseUseCard_Request
{
	int MapID;
	int CityID;
	inline void serialize(int playerid,int officerid,int sortid,int mapid,int cityid);
}UseOtherSpeedCard_Request,*LPUseOtherSpeedCard_Request;
inline void tagUseOtherSpeedCard_Request::serialize(int playerid,int officerid,int sortid,int mapid,int cityid)
{
	MapID = mapid;
	CityID = cityid;
	tagBaseUseCard_Request::serialize(sizeof(*this),playerid,officerid,sortid,CGC_C_OTHERSPEEDCARD);
}

typedef struct tagUseOtherSpeedCard_Response:DISTRIBUTED_HEADER
{
	int Error_Code;					//操作结果，0：成功，1:玩家没有该道具；5:不能使用；111：其他
	inline void serialize(int agent,int playerid,int code);
}UseOtherSpeedCard_Response,*LPUseOtherSpeedCard_Response;
inline void tagUseOtherSpeedCard_Response::serialize(int agent,int playerid,int code)
{
	Error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CHARGE,playerid,CGC_S_OTHERSPEEDCARD);
}

//使用托管卡CGC_C_AUTOCARD,	CGC_S_AUTOCARD,
typedef struct tagAutoConstruction
{
	int MapID;							//郡地图ID
	int CityID;							//城市ID
	int ConstructionID;					//建筑物ID
	int SortID;							//当为0时，表示新造建筑，否则为待升级建造的SortID
	int Level;							//级别
	inline __int64 GetKey();
}AutoConstruction,*LPAutoConstruction;	
inline __int64 tagAutoConstruction::GetKey()
{
	__int64 key;
	key = MapID + CityID + SortID;
	return key;
}
typedef struct tagUseAutoCard_Request:tagBaseUseCard_Request
{
	int Count;						//托管建造的数量，该结构后紧跟Count个AutoConstruction
	inline void serialize(int length,int playerid,int officerid,int sortid,int count);//适用于托管卡
}UseAutoCard_Request,*LPUseAutoCard_Request;
inline void tagUseAutoCard_Request::serialize(int length,int playerid,int officerid,int sortid,int count)
{
	OfficerID = officerid;
	SortID = sortid;
	Count = count;
	DISTRIBUTED_HEADER::serialize(length,0,playerid,MDM_GP_MAP_CHARGE,CGC_C_AUTOCARD);
}

typedef struct tagUseAutoCard_Response:DISTRIBUTED_HEADER
{
	int Error_Code;					//操作结果，0：成功；1:玩家没有该道具；2:托管数量不能操作该卡的最大数量；3：托管中；4:修改托管状态失败；5：不能使用；其他：失败
	inline void serialize(int agent,int playerid,int code);
}UseAutoCard_Response,*LPUseAutoCard_Response;
inline void tagUseAutoCard_Response::serialize(int agent,int playerid,int code)
{
	Error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CHARGE,playerid,CGC_S_AUTOCARD);
}
//取消托管CGC_C_CANCELAUTO,CGC_S_CANCELAUTO
typedef struct tagCancelAuto_Request:DISTRIBUTED_HEADER
{
	inline void serialize(int playerid);
}CancelAuto_Request,*LPCancelAuto_Request;
inline void tagCancelAuto_Request::serialize(int playerid)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,MDM_GP_MAP_CHARGE,CGC_C_CANCELAUTO);
}
	
typedef struct tagCancelAuto_Response:DISTRIBUTED_HEADER
{
	int Error_Code;					//操作结果，0：成功，1:未托管；111：其他
	inline void serialize(int agent,int playerid,int code);
}CancelAuto_Response,*LPCancelAuto_Response;
inline void tagCancelAuto_Response::serialize(int agent,int playerid,int code)
{
	Error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CHARGE,playerid,CGC_S_CANCELAUTO);
}

//查询托管列表CGC_C_QUERYAUTOLIST,	CGC_S_QUERYAUTOLIST,
typedef struct tagQueryAutoList_Request:DISTRIBUTED_HEADER
{
	inline void serialize(int playerid);
}QueryAutoList_Request,*LPQueryAutoList_Request;
inline void tagQueryAutoList_Request::serialize(int playerid)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,MDM_GP_MAP_CHARGE,CGC_C_QUERYAUTOLIST);
}

//返回的结构体AutoConstruction
typedef struct tagQueryAutoList_Response:DISTRIBUTED_HEADER
{
	int Count;							//返回数量，-1：未托管
	inline void serialize(int length,int agent,int playerid,int count);
}QueryAutoList_Response,*LPQueryAutoList_Response;
inline void tagQueryAutoList_Response::serialize(int length,int agent,int playerid,int count)
{
	Count = count;
	DISTRIBUTED_HEADER::serialize(length,agent,MDM_GP_MAP_CHARGE,playerid,CGC_S_QUERYAUTOLIST);
}

//装备卡CGC_C_HEROEQUIPCARD,CGC_S_HEROEQUIPCARD,
typedef struct tagUseHeroEquipCard_Request:tagBaseUseCard_Request
{
	inline void serialize(int playerid,int officerid,int sortid);
}UseHeroEquipCard_Request,*LPUseHeroEquipCard_Request;
inline void tagUseHeroEquipCard_Request::serialize(int playerid,int officerid,int sortid)
{
	tagBaseUseCard_Request::serialize(sizeof(*this),playerid,officerid,sortid,CGC_C_HEROEQUIPCARD);
}

typedef struct tagUseHeroEquipCard_Response:DISTRIBUTED_HEADER
{
	int Error_Code;					//操作结果，0：成功，1:玩家没有该道具；2:不存在对应的装备；5:不能使用；111：其他
	inline void serialize(int agent,int playerid,int code);
}UseHeroEquipCard_Response,*LPUseHeroEquipCard_Response;
inline void tagUseHeroEquipCard_Response::serialize(int agent,int playerid,int code)
{
	Error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CHARGE,playerid,CGC_S_HEROEQUIPCARD);
}

//查询当前某生产速率CGC_C_QUERYRESOURCERATE,CGC_S_QUERYRESOURCERATE	
typedef struct tagQueryResourceRate_Request:DISTRIBUTED_HEADER
{
	CardType Type;
	inline void serialize(int playerid,CardType type);
}QueryResourceRate_Request,*LPQueryResourceRate_Request;
inline void tagQueryResourceRate_Request::serialize(int playerid,CardType type)
{
	Type = type;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,MDM_GP_MAP_CHARGE,CGC_C_QUERYRESOURCERATE);
}

typedef struct tagQueryResourceRate_Response:DISTRIBUTED_HEADER
{
	int Error_Code;					//操作结果，>=0:当前值；<0失败
	inline void serialize(int agent,int playerid,int code);
}QueryResourceRate_Response,*LPQueryResourceRate_Response;
inline void tagQueryResourceRate_Response::serialize(int agent,int playerid,int code)
{
	Error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CHARGE,playerid,CGC_S_QUERYRESOURCERATE);
}

//使用背包扩展卡CGC_C_EXPANDPACKCARD,CGC_S_EXPANDPACKCARD,	
typedef struct tagUseExpandPackCard_Request:tagBaseUseCard_Request
{
	int PackID;						//待扩展的背包类型，1：公共背包；英雄ID为英雄背包
	inline void serialize(int playerid,int officerid,int sortid,int packid = 1);
}UseExpandPackCard_Request,*LPUseExpandPackCard_Request;
inline void tagUseExpandPackCard_Request::serialize(int playerid,int officerid,int sortid,int packid)
{
	PackID = packid;
	tagBaseUseCard_Request::serialize(sizeof(*this),playerid,officerid,sortid,CGC_C_EXPANDPACKCARD);
}

typedef struct tagUseExpandPackCard_Response:DISTRIBUTED_HEADER
{
	int Error_Code;					//0：成功，1:玩家没有该道具；2:背包已到最大数量；5:不能使用；111：其他
	int PackNum;					//背包容量，Error_Code为0时有效
	inline void serialize(int agent,int playerid,int code,int num);
}UseExpandPackCard_Response,*LPUseExpandPackCard_Response;
inline void tagUseExpandPackCard_Response::serialize(int agent,int playerid,int code,int num)
{
	Error_Code = code;
	PackNum = num;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CHARGE,playerid,CGC_S_EXPANDPACKCARD);
}

//立即完成通知消息CGC_C_NOTIFY_IMMEDCOMPLETE,CGC_S_NOTIFY_IMMEDCOMPLETE，该消息由各业务服务器发送过来
typedef struct tagImmedComplete_Param
{
	int Type;						//业务类型，如：建筑立即完成、生产立即完成、征兵立即完成、科研立即完成、招募英雄立即完成
	int MapID;
	int CityID;
	int ProductionType;				//主要针对生产类，表示生产的物品类型，如武器、防具、特殊工具等，具体参看GOODS_TYPE枚举
	int ItemID;
	int SortID;
	int Level;
	int GOCount;					//需要消耗的GO点数
}ImmedComplete_Param,*LPImmedComplete_Param;

typedef struct tagNotify_ImmedComplete_Request:DISTRIBUTED_HEADER
{//from 为服务器ID
	int PlayerID;
	ImmedComplete_Param Param;
	inline void serialize(int agent,int playerid,int type,int mapid,int cityid,int itemid,int sortid,int level,int count,int protype);
}Notify_ImmedComplete_Request,*LPNotify_ImmedComplete_Request;
inline void tagNotify_ImmedComplete_Request::serialize(int agent,int playerid,int type,int mapid,int cityid,int itemid,int sortid,int level,int count,int protype)
{
	PlayerID = playerid;
	Param.Type = type;
	Param.MapID = mapid;
	Param.CityID = cityid;
	Param.ProductionType = protype;
	Param.ItemID = itemid;
	Param.SortID = sortid;
	Param.Level = level;
	Param.GOCount = count;
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
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,svrid,MDM_GP_MAP_CHARGE,CGC_C_NOTIFY_IMMEDCOMPLETE);
}
//返回业务服务器
typedef struct tagNotify_ImmedComplete_Response:DISTRIBUTED_HEADER
{
	int Error_Code;				//0：成功，1：GO点不足；111：其他失败
	ImmedComplete_Param Param;
	inline void serialize(int agent,int playerid,int type,int mapid,int cityid,int itemid,int sortid,int level,int code,int count,int protype);
}Notify_ImmedComplete_Response,*LPNotify_ImmedComplete_Response;
inline void tagNotify_ImmedComplete_Response::serialize(int agent,int playerid,int type,int mapid,int cityid,int itemid,int sortid,int level,int code,int count,int protype)
{
	Error_Code = code;
	Param.Type = type;
	Param.MapID = mapid;
	Param.CityID = cityid;
	Param.ProductionType = protype;
	Param.ItemID = itemid;
	Param.SortID = sortid;
	Param.Level = level;
	Param.GOCount = count;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,playerid,MDM_GP_MAP_CHARGE,CGC_S_NOTIFY_IMMEDCOMPLETE);
}
//参加竞选扣费CGC_C_NOTIFY_ELECTION,CGC_S_NOTIFY_ELECTION
typedef struct tagNotify_Election_Request:DISTRIBUTED_HEADER
{
	int CountryID;				//阵营
	short Dept;					//参选职位，如国防部、内政部，参看Dept_Type
	char Creed[100];
	int Money;
	int GOMoney;
	inline void serialize(int agent,int playerid,int countryid,short type,const char *pCreed,int money,int gomoney);
}Notify_Election_Request,*LPNotify_Election_Request;
inline void tagNotify_Election_Request::serialize(int agent,int playerid,int countryid,short type,const char *pCreed,int money,int gomoney)
{
	CountryID = countryid;
	Dept = type;
	if(pCreed)
	{
		strcpy(Creed,pCreed);
	}
	Money = money;
	GOMoney = gomoney;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,playerid,MDM_GP_MAP_CHARGE,CGC_C_NOTIFY_ELECTION);
}

typedef struct tagNotify_Election_Response:DISTRIBUTED_HEADER
{
	short Error_Code;			//0成功；1：GO点不足；111：失败
	int CountryID;				//阵营
	short Dept;					//参选职位，如国防部、内政部，参看Dept_Type
	char Creed[100];
	int Money;
	inline void serialize(int agent,int playerid,int code,int countryid,short type,const char *pCreed,int money);
}Notify_Election_Response,*LPNotify_Election_Response;
inline void tagNotify_Election_Response::serialize(int agent,int playerid,int code,int countryid,short type,const char *pCreed,int money)
{
	Error_Code = static_cast<short> (code);
	CountryID = countryid;
	Dept =type;
	if(pCreed)
	{
		strcpy(Creed,pCreed);
	}
	Money = money;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,playerid,MDM_GP_MAP_CHARGE,CGC_S_NOTIFY_ELECTION);
}
//兑换CGC_C_EXCHANGE,CGC_S_EXCHANGE,
typedef struct tagExchange_Request:DISTRIBUTED_HEADER
{
	int Type;					//类型ExchangeType，目前只支持Go点兑换金钱GO2Money
	int Count;					//待兑换的数量，不能小于等于0
	inline void serialize(int playerid,int type,int count);
}Exchange_Request,*LPExchange_Request;
inline void tagExchange_Request::serialize(int playerid,int type,int count)
{
	Type = type;
	Count = count;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,MDM_GP_MAP_CHARGE,CGC_C_EXCHANGE);
}

typedef struct tagExchange_Response:DISTRIBUTED_HEADER
{
	int Type;
	int Count;					//返回兑换的金钱数量，客户端增加该值即可,小于等于0时有错误：-1：不足；-2：其他；
	inline void serialize(int agent,int playerid,int type,int count);
}Exchange_Response,*LPExchange_Response;
inline void tagExchange_Response::serialize(int agent,int playerid,int type,int count)
{
	Type = type;
	Count = count;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CHARGE,playerid,CGC_S_EXCHANGE);
}
//提高生产率，不使用卡CGC_C_INCREASERECOURCERATE,CGC_S_INCREASERECOURCERATE,
typedef struct tagIncreaeRecourceRate_Request:DISTRIBUTED_HEADER
{
	int Type;					//类型，参看卡类型
	int Rate;					//提速百分比
	inline void serialize(int playerid,int type,int rate);
}IncreaeRecourceRate_Request,*LPIncreaeRecourceRate_Request;
inline void tagIncreaeRecourceRate_Request::serialize(int playerid,int type,int rate)
{
	Type = type;
	Rate = rate;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,MDM_GP_MAP_CHARGE,CGC_C_INCREASERECOURCERATE);
}

typedef struct tagIncreaeRecourceRate_Response:DISTRIBUTED_HEADER
{
	int Count;					//大于0时为加速成功消耗的GO点，-1，GO点不足；-111：失败
	int Type;					//类型，参看卡类型
	int Rate;					//提速百分比
	inline void serialize(int agent,int playerid,int count,int type,int rate);
}IncreaeRecourceRate_Response,*LPIncreaeRecourceRate_Response;
inline void tagIncreaeRecourceRate_Response::serialize(int agent,int playerid,int count,int type,int rate)
{
	Count = count;
	Type = type;
	Rate = rate;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CHARGE,playerid,CGC_S_INCREASERECOURCERATE);
}

//使用恢复卡CGC_C_UEEHERORESUME,CGC_S_USEHERORESUME,
typedef struct tagUseHeroResume_Request:tagBaseUseCard_Request
{
	int HeroID;			//重伤英雄ID
	inline void serialize(int playerid,int officerid,int sortid,int heroid);
}UseHeroResume_Request,*LPUseHeroResume_Request;
inline void tagUseHeroResume_Request::serialize(int playerid,int officerid,int sortid,int heroid)
{
	HeroID = heroid;
	tagBaseUseCard_Request::serialize(sizeof(*this),playerid,officerid,sortid,CGC_C_UEEHERORESUME);
}

typedef struct tagUseHeroResume_Response:DISTRIBUTED_HEADER
{
	int Error_Code;			//操作结果，0：成功，1:玩家没有该道具；2：非恢复卡；3:英雄未重伤；111：其他
	int HeroID;				//英雄ID
	inline void serialize(int agent,int playerid,int code,int heroid);
}UseHeroResume_Response,*LPUseHeroResume_Response;
inline void tagUseHeroResume_Response::serialize(int agent,int playerid,int code,int heroid)
{
	Error_Code = code;
	HeroID = heroid;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CHARGE,playerid,CGC_S_USEHERORESUME);
}

//通知建造建筑，用于托管类CGC_C_NOTITY_BUILDCONSTRUCTION,CGC_S_NOTITY_BUILDCONSTRUCTION
typedef struct tagNotify_BuildConstruction_Request:DISTRIBUTED_HEADER
{
	int PlayerID;
	AutoConstruction ConstructionInfo;
	inline void serialize(int playerid,LPAutoConstruction info);
}Nodify_BuildConstruction_Request,*LPNodify_BuildConstruction_Request;
inline void tagNotify_BuildConstruction_Request::serialize(int playerid,LPAutoConstruction info =0)
{
	PlayerID = playerid;
	if(info)
	{
		ConstructionInfo = *info;
	}
	DISTRIBUTED_HEADER::serialize(sizeof(*this),DEFAULT_AGENT,0,0,CGC_C_NOTITY_BUILDCONSTRUCTION);
}

typedef struct tagNodify_BuildConstruction_Response:DISTRIBUTED_HEADER
{
	int PlayerID;
	int Error_Code;
	inline void serialize(int playerid,int code);
}Nodify_BuildConstruction_Response,*LPNodify_BuildConstruction_Response;
inline void tagNodify_BuildConstruction_Response::serialize(int playerid,int code)
{
	PlayerID = playerid;					//玩家ID
	Error_Code = code;						//操作结果，0：成功，其他：失败
	DISTRIBUTED_HEADER::serialize(sizeof(*this),DEFAULT_AGENT,0,0,CGC_S_NOTITY_BUILDCONSTRUCTION);
}

//提速通知CGC_C_NOTIFY_INCREASESPEED,CGC_S_NOTIFY_INCREASESPEED，
typedef struct tagNotify_IncreaseSpeed_Request:DISTRIBUTED_HEADER
{
	int			PlayerID;				//玩家ID
	CardType	Type;					//加速类型
	int			Value;					//加速百分比，如50代表加速50%
	inline void serialize(int playerid,CardType type,int value);
}Notify_IncreaseSpeed_Request,*LPNotify_IncreaseSpeed_Request;
inline void tagNotify_IncreaseSpeed_Request::serialize(int playerid,CardType type,int value)
{
	PlayerID = playerid;
	Type = type;
	Value = value;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),DEFAULT_AGENT,0,0,CGC_C_NOTIFY_INCREASESPEED);
}

typedef struct tagNotify_IncreaseSpeed_Response:DISTRIBUTED_HEADER
{
	int			PlayerID;
	CardType	Type;
	int			Error_Code;
	inline void serialize(int playerid,CardType type,int code);
}Notify_IncreaseSpeed_Response,*LPNotify_IncreaseSpeed_Response;
inline void tagNotify_IncreaseSpeed_Response::serialize(int playerid,CardType type,int code)
{
	PlayerID = playerid;
	Type = type;
	Error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),DEFAULT_AGENT,0,0,CGC_S_NOTIFY_INCREASESPEED);
}

typedef struct tagChargeSvr_TimerParam
{
	int TimerID;
	int PlayerID;
	CardType TimerType;
	int MapID;
	int CityID;
}ChargeSvr_TimerParam,*LPChargeSvr_TimerParam;
//时间通知（定时器）CGC_C_TIMERNOTIFY	
typedef struct tagNotify_Timer:DISTRIBUTED_HEADER
{
	ChargeSvr_TimerParam Param;
	inline void serialize(int playerid,CardType type,int mapid,int cityid,int timerid = -1);
}Notify_Timer,*LPNotify_Timer;	
inline void tagNotify_Timer::serialize(int playerid,CardType type,int mapid,int cityid,int timerid)
{
	Param.PlayerID = playerid;
	Param.TimerType = type;
	Param.TimerID = timerid;
	Param.MapID = mapid;
	Param.CityID = cityid;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),DEFAULT_AGENT,0,0,CGC_C_TIMERNOTIFY);
}

//恢复正常速度CGC_C_NOTIFY_NORMALSPEED,	CGC_S_NOTIFY_NORMALSPEED,
typedef struct tagNotify_NormalSpeed_Request:DISTRIBUTED_HEADER
{
	int PlayerID;
	CardType	Type;					//加速类型
	inline void serialize(int playerid,CardType type);
}Notify_NormalSpeed,*LPNotify_NormalSpeed;
inline void tagNotify_NormalSpeed_Request::serialize(int playerid,CardType type)
{
	PlayerID = playerid;
	Type = type;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),DEFAULT_AGENT,0,0,CGC_C_NOTIFY_NORMALSPEED);
}

typedef struct tagNotify_NormalSpeed_Response:DISTRIBUTED_HEADER
{
	int PlayerID;
	int Error_Code;
	inline void serialize(int playerid,int code);
}Notify_NormalSpeed_Response,*LPNotify_NormalSpeed_Response;
inline void tagNotify_NormalSpeed_Response::serialize(int playerid,int code)
{
	PlayerID = playerid;
	Error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),DEFAULT_AGENT,0,0,CGC_S_NOTIFY_NORMALSPEED);
}

//一次性加速，包括建造、科研、征兵加速卡和招募英雄CGC_C_NOTIFY_INCREASEOTHERSPEED,		CGC_S_NOTIFY_INCREASEOTHERSPEED,
typedef struct tagNotify_IncreaseOtherSpeed_Request:DISTRIBUTED_HEADER
{
	int			PlayerID;				//玩家ID
	int			MapID;					//
	int			CityID;					//
	CardType	Type;					//加速类型
	int			Value;					//加速百分比，如50代表加速50%
	int			SortID;					//当为建造、科研、征兵加速时，为待加速的任务ID,生产加速则为0（无效）
	inline void serialize(int playerid,int mapid,int cityid,CardType type,int value,int sortid);
}Notify_IncreaseOtherSpeed_Request,*LPNotify_IncreaseOtherSpeed_Request;
inline void tagNotify_IncreaseOtherSpeed_Request::serialize(int playerid,int mapid,int cityid,CardType type,int value,int sortid)
{
	PlayerID = playerid;
	MapID = mapid;
	CityID = cityid;
	Type = type;
	Value = value;
	SortID = sortid;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),DEFAULT_AGENT,0,0,CGC_C_NOTIFY_INCREASEOTHERSPEED);
}

typedef struct tagNotify_IncreaseOtherSpeed_Response:DISTRIBUTED_HEADER
{
	int			PlayerID;
	CardType	Type;
	int			Error_Code;
	inline void serialize(int playerid,CardType type,int code);
}Notify_IncreaseOtherSpeed_Response,*LPNotify_IncreaseOtherSpeed_Response;
inline void tagNotify_IncreaseOtherSpeed_Response::serialize(int playerid,CardType type,int code)
{
	PlayerID = playerid;
	Type = type;
	Error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),DEFAULT_AGENT,0,0,CGC_S_NOTIFY_INCREASEOTHERSPEED);
}

//采集资源的通知CGC_C_NOTIFY_COLLECTRESOURCE,	CGC_S_NOTIFY_COLLECTRESOURCE,	
typedef struct tagNotify_CollectResource_Request:DISTRIBUTED_HEADER
{
	int PlayerID;
	int MapID;
	int CityID;
	inline void serialize(int playerid,int mapid,int cityid);
}Notify_CollectResource_Request,*LPNotify_CollectResource_Request;
inline void tagNotify_CollectResource_Request::serialize(int playerid,int mapid,int cityid)
{
	PlayerID = playerid;
	MapID = mapid;
	CityID = cityid;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),DEFAULT_AGENT,0,0,CGC_C_NOTIFY_COLLECTRESOURCE);
}

typedef struct tagNotify_CollectResource_Response:DISTRIBUTED_HEADER
{
	int Error_Code;						//0:成功；其他：失败
	int PlayerID;						//玩家ID；
	int MapID;							//郡地图ID
	int CityID;							//城市ID
	inline void serialize(int playerid,int mapid,int cityid,int code);
}Notify_CollectResource_Response,*LPNotify_CollectResource_Response;
inline void tagNotify_CollectResource_Response::serialize(int playerid,int mapid,int cityid,int code)
{
	PlayerID = playerid;
	MapID = mapid;
	CityID = cityid;
	Error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),DEFAULT_AGENT,0,0,CGC_S_NOTIFY_COLLECTRESOURCE);
}

//建造完成通知，有郡地图服务器自动下发通知CGC_S_NOTIFY_BUILDCOMPLETE	
typedef struct tagNotify_BuildComplete_Response:DISTRIBUTED_HEADER
{
	int PlayerID;
	int MapID;						//完成建造的郡服务器ID
	int CityID;						//城市ID
	int Error_Code;					//操作结果，0：正常结束；1：资源不够；其他：失败
	inline void serialize(int playerid,int mapid,int cityid,int code);
}Notify_BuildComplete_Response,*LPNotify_BuildComplete_Response;
inline void tagNotify_BuildComplete_Response::serialize(int playerid,int mapid,int cityid,int code)
{
	PlayerID = playerid;
	MapID = mapid;
	CityID = cityid;
	Error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),DEFAULT_AGENT,0,0,CGC_S_NOTIFY_BUILDCOMPLETE);
}
//任务系统查询GO点CGC_C_NOTIFY_QUERYMONEY,CGC_S_NOTIFY_QUERYMONEY
typedef struct tagNotify_QueryMoney_Request:DISTRIBUTED_HEADER
{
	int PlayerID;
	int TaskID;//任务ID
	inline void serialize(int agent,int playerid,int taskid,int from);
}Notify_QueryMoney_Request,*LPNotify_QueryMoney_Request;
inline void tagNotify_QueryMoney_Request::serialize(int agent,int playerid,int taskid,int from)
{
	PlayerID = playerid;
	TaskID = taskid;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,from,MDM_GP_MAP_CHARGE,CGC_C_NOTIFY_QUERYMONEY);
}

typedef struct tagNotify_QueryMoney_Response:DISTRIBUTED_HEADER
{
	int PlayerID;
	int TaskID;//任务ID
	int Money;//GO点数
	inline void serialize(int agent,int playerid,int taskid,int money,int to);
}Notify_QueryMoney_Response,*LPNotify_QueryMoney_Response;
inline void tagNotify_QueryMoney_Response::serialize(int agent,int playerid,int taskid,int money,int to)
{
	PlayerID = playerid;
	TaskID = taskid;
	Money = money;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CHARGE,to,CGC_S_NOTIFY_QUERYMONEY);
}

//扣除/添加GO点CGC_C_NOTIFY_ADDMONEY,CGC_S_NOTIFY_ADDMONEY
typedef struct tagNotify_AddMoney_Request:DISTRIBUTED_HEADER
{
	int PlayerID;		//玩家ID
	int Money;			//为正时添加GO点，为负数时减少GO点
	inline void serialize(int agent,int playerid,int money,int from);
}Notify_AddMoney_Request,*LPNotify_AddMoney_Request;
inline void tagNotify_AddMoney_Request::serialize(int agent,int playerid,int money,int from)
{
	PlayerID = playerid;
	Money = money;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,from,MDM_GP_MAP_CHARGE,CGC_C_NOTIFY_ADDMONEY);
}

typedef struct tagNotify_AddMoney_Response:DISTRIBUTED_HEADER
{
	int PlayerID;
	int Error_Code;			//0成功；1:GO点不足；111：失败
	inline void serialize(int agent,int playerid,int code,int to);
}Notify_AddMoney_Response,*LPNotify_AddMoney_Response;
inline void tagNotify_AddMoney_Response::serialize(int agent,int playerid,int code,int to)
{
	PlayerID = playerid;
	Error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CHARGE,to,CGC_S_NOTIFY_ADDMONEY);
}

//使用技能卡通知大地图CGC_C_NOTIFY_ADDOFFICERSKILL,CGC_S_NOTIFY_ADDOFFICERSKILL
typedef struct tagNotify_AddOfficerSkill_Request:DISTRIBUTED_HEADER
{
	int PlayerID;				//玩家ID
	int PackID;					//背包ID
	int OfficerID;				//学习技能的英雄ID
	int SortID;
	int SkillID;				//技能ID
	inline void serialize(int agent,int playerid,int packid,int officerid,int sortid,int skillid);
}Notify_AddOfficerSkill_Request,*LPNotify_AddOfficerSkill_Request;
inline void tagNotify_AddOfficerSkill_Request::serialize(int agent,int playerid,int packid,int officerid,int sortid,int skillid)
{
	PlayerID = playerid;
	PackID = packid;
	OfficerID = officerid;
	SortID = sortid;
	SkillID = skillid;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CHARGE,SERVER_BOUND_MAP_OTHER,CGC_C_NOTIFY_ADDOFFICERSKILL);
}
typedef struct tagNotify_AddOfficerSkill_Response:DISTRIBUTED_HEADER
{
	short Error_Code;			//操作结果，0：成功；1：错误
	int PlayerID;				//玩家ID
	int PackID;					//背包ID
	int OfficerID;				//学习技能的英雄ID
	int SortID;
	inline void serialize(int agent,int playerid,int packid,int officerid,int sortid,int code);
}Notify_AddOfficerSkill_Response,*LPNotify_AddOfficerSkill_Response;
inline void tagNotify_AddOfficerSkill_Response::serialize(int agent,int playerid,int packid,int officerid,int sortid,int code)
{
	Error_Code = code;
	PlayerID = playerid;
	PackID = packid;
	OfficerID = officerid;
	SortID = sortid;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,SERVER_BOUND_MAP_OTHER,MDM_GP_MAP_CHARGE,CGC_S_NOTIFY_ADDOFFICERSKILL);
}
//使用一种卡，各服务器发送过来的通知CGC_C_NOTIFY_USEONECARD,CGC_S_NOTIFY_USEONECARD,
typedef struct tagNotify_UseOneCard_Request:DISTRIBUTED_HEADER
{
	int playerID;						//玩家ID
	int cardID;							//卡的ID
	inline void serialize(int agent,int playerid,int cardid,int svrid);
}Notify_UseOneCard_Request,*LPNotify_UseOneCard_Request;
inline void tagNotify_UseOneCard_Request::serialize(int agent,int playerid,int cardid,int svrid)
{
	playerID = playerid;
	cardID = cardid;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,svrid,MDM_GP_MAP_CHARGE,CGC_C_NOTIFY_USEONECARD);
}
typedef struct tagNotify_UseOneCard_Response:DISTRIBUTED_HEADER
{
	int playerID;
	int cardID;
	int error_Code;					//0:成功
	inline void serialize(int agent,int playerid,int cardid,int code,int svrid);
}Notify_UseOneCard_Response,*LPNotify_UseOneCard_Response;
inline void tagNotify_UseOneCard_Response::serialize(int agent,int playerid,int cardid,int code,int svrid)
{
	playerID = playerid;
	cardID = cardid;
	error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CHARGE,svrid,CGC_S_NOTIFY_USEONECARD);
}

//买卡通知，收费服务器通知大地图CGC_C_NOTIFY_BUYCARD,CGC_S_NOTIFY_BUYCARD
typedef struct tagNotify_BuyCard_Request:DISTRIBUTED_HEADER
{
	int playerID;			//玩家ID
	int cardID;				//卡ID
	int postion;			//
	inline void serialize(int agent,int playerid,int cardid,int pos);
}Notify_BuyCard_Request,*LPNotify_BuyCard_Request;
inline void tagNotify_BuyCard_Request::serialize(int agent,int playerid,int cardid,int pos)
{
	playerID = playerid;
	cardID = cardid;
	postion = pos;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CHARGE,SERVER_BOUND_MAP_OTHER,CGC_C_NOTIFY_BUYCARD);
}

typedef struct tagNotify_BuyCard_Response:DISTRIBUTED_HEADER
{
	int playerID;
	int cardID;
	short error_Code;
	inline void serialize(int agent,int playerid,int cardid,short code);
}Notify_BuyCard_Response,*LPNotify_BuyCard_Response;
inline void tagNotify_BuyCard_Response::serialize(int agent,int playerid,int cardid,short code)
{
	playerID = playerid;
	cardID = cardid;
	error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,SERVER_BOUND_MAP_OTHER,MDM_GP_MAP_CHARGE,CGC_S_NOTIFY_BUYCARD);
}

#pragma pack(pop)

#endif
