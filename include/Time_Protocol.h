/*=================================================================*/
/*研发团队  : 深圳市新飞扬
/* 文件名   : Time_Protocol.h
/* 文件描述 : 时间事件结构体定义
/* 创建时间 : 2008-5-7
/* 创建者   : 曾庆辉
/* 备注     :
/*
/*-----------------------------------------------------------------*/
/* 修改记录1:
/* 修改时间 :
/* . . .
/*=================================================================*/
#ifndef __TIME_PROTOCOL__
#define __TIME_PROTOCOL__

enum TIME_PROTOCOL
{
	//时间事件
	EN_TIMER_ARMY_OUT = 0x1301,				//部队出征
	EN_TIMER_CONSTRUCTION,					//建筑建筑物
	EN_TIMER_PRODUCE_WEAPON,				//生产武器
	EN_TIMER_PRODUCE_DEFEND,				//生产防具
	EN_TIMER_PRODUCE_ANIMAL,				//生产驯兽技术
	EN_TIMER_PRODUCE_SPECIAL,				//生产特殊技术

	EN_TIMER_RESEARCH_WEAPON,				//研究武器
	EN_TIMER_RESEARCH_DEFEND,				//研究防具
	EN_TIMER_RESEARCH_ANIMAL,				//研究驯兽技术
	EN_TIMER_RESEARCH_SPECIAL,				//研究特殊技术

	EN_TIMER_RESEARCH,						//科研道具技术

	EN_TIMER_LEVYING,						//正处于招兵中...
	EN_TIMER_RECRUITING_HERO,				//正在招募英雄...
	EN_TIMER_ARMY_WARM,						//警报信息

	EN_TIMER_HERO_RESUME,					//英雄恢复：从重伤恢复到正常
	EN_TIMER_FIGHT_NOTFIY,					//战斗通知信息
	EN_TIMER_LOG_NOTFIY ,                   //在线人数通知
	EN_TIMER_CREATENPCARMY,					//创建NPC部队,
	EN_TIMER_CLEARDAYTASK,					//清除日常任务定时器
	EN_TIMER_END = 0x1350

};
#pragma pack(push,1)
//时间事件头协议,modify by xujincai 20090205 定时器消息作为普通消息加入消息队列处理
struct TIMER_HEADER:DISTRIBUTED_HEADER
{
	inline void serialize(int,int);
	int timer_command;				// 时间协议命令
	long TimerID;					//定时器ID
	bool isDeleted;					//是否已删除定时器
};
struct MsgFightTimer : TIMER_HEADER{
	int uEctypeID;
	inline void serialize(int ectypeRoomID){
		uEctypeID = ectypeRoomID;
		TIMER_HEADER::serialize(sizeof(MsgFightTimer),CMD_FIGHT_TIMER_MSG);
	}
};
//出征部队
struct OUT_ARMY_PATH : TIMER_HEADER
{
	inline void serialize(unsigned long lPlayerID,unsigned long lArmyID, int lFromMapID, int lToMapID, int lOutMapID, int lTargetMapID,int command, unsigned long officerid/*=0*/);
	unsigned long							lPlayerID;					//出征玩家的ID
	unsigned long							lOfficerID;					//部队的指挥官ID
	unsigned long							lArmyID;

	int							lFromMapID;					//当前起始郡ID
	int							lToMapID;					//当前达到郡ID

	int							lOutMapID;					//出征郡ID
	int							lTargetMapID;				//目标郡ID
	int							iCount;
	int							iMapList[50];
};
//建筑建筑物
struct BUILD_CONSTRUCTION_NODE : TIMER_HEADER
{
	inline void serialize(unsigned long lCityID, unsigned long lPlayerID, unsigned long lSortID, unsigned long lActionID, unsigned long lConID, 
		int iSLevel, int iLevel, int iAuto, int command);
	unsigned long							lCityID;					//城市ID
	unsigned long							lPlayerID;					//
	unsigned long							lSortID;
	unsigned long							lActionID;
	unsigned long							lConID;						//建筑物ID
	int										iSLevel;					//
	int										iLevel;						//等级
	int										iAuto;						//托管
};
//生产道具
struct PRODUCE_ITEM_NODE : TIMER_HEADER
{
	inline void serialize(unsigned long lCityID, unsigned long lPlayerID, int iItemID, int iType, unsigned long lSortID, int iCount, int command);
	unsigned long							lCityID;					//城市ID
	unsigned long							lPlayerID;
	int										iItemID;					//道具ID
	int										iCount;						//数量
	int										iType;						//
	unsigned long							lSortID;
};
//科研道具
struct RESEARCH_ITEM_NODE : TIMER_HEADER
{
	inline void serialize(unsigned long lPlayerID, int iType, int iItemID, int iLevel, int iCount, int command);
	unsigned long							lPlayerID;
	int										iType;
	int										iItemID;					//道具ID
	int										iLevel;						//道具等级
	int										iCount;						//数量

};
//招兵
struct LEVYING_SOLDIER_LIST : TIMER_HEADER
{
	inline void serialize(unsigned long lPlayerID, unsigned long lMapID, unsigned long lCityID, int iReceID, int iCount,int sortid, int command);
	unsigned long	lPlayerID;
	unsigned long	lMapID;
	unsigned long	lCityID;
	int				iReceID;
	int				iCount;
	int				iSortID;
};
//正在招募英雄...
struct RECRUITING_HERO : TIMER_HEADER
{
	inline void serialize(unsigned long lPlayerID, int iSortID, int iCountry, int command);
	unsigned long		lPlayerID;
	int					iSortID;						//序列号
	int					iCountry;						//国家
};
//警报信息
struct ARMY_WARM : TIMER_HEADER
{
	inline void serialize(unsigned long lPlayerID, unsigned long lMapID,unsigned long armyid, int command);
	unsigned long lPlayerID;
	unsigned long lMapID;
	unsigned long lArmyID;
};

//英雄重伤恢复
typedef struct tagTimer_Hero_Resume:TIMER_HEADER
{
	unsigned int PlayerID;			//玩家ID
	unsigned int OfficerID;			//英雄ID（指挥官ID）
	inline void serialize(unsigned int playerid,unsigned int officerid);
}Timer_Hero_Resume,*LPTimer_Hero_Resume;

struct FightNotfiy:TIMER_HEADER
{
	unsigned int iFightRoomID;
	inline void serialize( unsigned int roomid,int command = EN_TIMER_FIGHT_NOTFIY)
	{
		this->iFightRoomID = roomid;
		TIMER_HEADER::serialize(sizeof(*this),command);
	}
};
struct LOG_ONLINE_NUM_TIMER : TIMER_HEADER
{
	inline void serialize(int command = EN_TIMER_LOG_NOTFIY)
	{
		TIMER_HEADER::serialize(sizeof(*this),command);
	}
};
//创建NPC反击部队EN_TIMER_CREATENPCARMY
struct Timer_CreateNPCArmy : TIMER_HEADER
{
	int CountryID;				//NPC部队的阵营
	int Count;					//数量
	int Level;					//部队的级别
	int FromMap;				//上一个郡（主要为了着陆点的确定);
	int TargetMap;				//目标郡
	int Times;					//次数
	int Interval;				//时间间隔，单位：秒
	inline void serialize(int countryid,int level,int count,int frommap,int targetmap,int times,int interval);
};
inline void Timer_CreateNPCArmy::serialize(int countryid,int level,int count,int frommap,int targetmap,int times,int interval)
{
	this->CountryID = countryid;
	this->Count = count;
	this->Level = level;
	this->FromMap = frommap;
	this->TargetMap = targetmap;
	this->Times = times;
	this->Interval = interval;
	TIMER_HEADER::serialize(sizeof(*this),EN_TIMER_CREATENPCARMY);
}
struct Timer_ClearDayTask:TIMER_HEADER
{
	inline void serialize();
};
inline void Timer_ClearDayTask::serialize()
{
	TIMER_HEADER::serialize(sizeof(*this),EN_TIMER_CLEARDAYTASK);
}
#pragma pack(pop)


//时间事件头协议
inline void TIMER_HEADER::serialize(int length, int command)
{
	//this->length = length;
	this->isDeleted = false;
	this->timer_command = command;
	DISTRIBUTED_HEADER::serialize(length,0,0,0,EN_C_TIMER_NOTIFY);
}

//出征部队
inline void
OUT_ARMY_PATH::serialize(unsigned long lPlayerID, unsigned long lArmyID, int lFromMapID, int lToMapID, int lOutMapID, int lTargetMapID,int command, unsigned long officerid)
{
	TIMER_HEADER::serialize(sizeof(*this), command);
	this->lPlayerID = lPlayerID;
	this->lArmyID = lArmyID;
	this->lFromMapID = lFromMapID;
	this->lToMapID = lToMapID;
	this->lOutMapID = lOutMapID;
	this->lTargetMapID = lTargetMapID;
	this->lOfficerID = officerid;
}
//建筑建筑物
inline void
BUILD_CONSTRUCTION_NODE::serialize(unsigned long lCityID, unsigned long lPlayerID, unsigned long lSortID,  unsigned long lActionID, unsigned long lConID,
								   int iSLevel, int iLevel, int iAuto,int command)
{
	TIMER_HEADER::serialize(sizeof(*this), command);
	this->lSortID = lSortID;
	this->lActionID = lActionID;
	this->lCityID = lCityID;
	this->lPlayerID = lPlayerID;
	this->lConID = lConID;
	this->iSLevel = iSLevel;
	this->iLevel = iLevel;
	this->iAuto = iAuto;
}//生产道具
inline void
PRODUCE_ITEM_NODE::serialize(unsigned long lCityID, unsigned long lPlayerID, int iItemID, int iType, unsigned long lSortID, int iCount, int command)
{
	TIMER_HEADER::serialize(sizeof(*this), command);
	this->lCityID = lCityID;
	this->lPlayerID = lPlayerID;
	this->iItemID = iItemID;
	this->iType = iType;
	this->iCount = iCount;
	this->lSortID = lSortID;
}
//科研道具
inline void
RESEARCH_ITEM_NODE::serialize(unsigned long lPlayerID, int iType, int iItemID, int iLevel, int iCount, int command)
{
	TIMER_HEADER::serialize(sizeof(*this), command);
	this->lPlayerID = lPlayerID;
	this->iType = iType;
	this->iItemID = iItemID;
	this->iLevel = iLevel;
	this->iCount = iCount;
}
//招兵
inline void
LEVYING_SOLDIER_LIST::serialize(unsigned long lPlayerID, unsigned long lMapID, unsigned long lCityID, int iReceID, int iCount,int sortid, int command)
{
	TIMER_HEADER::serialize(sizeof(*this), command);
	this->lPlayerID = lPlayerID;
	this->lMapID = lMapID;
	this->lCityID = lCityID;
	this->iReceID = iReceID;
	this->iCount = iCount;
	this->iSortID = sortid;
}
inline void 
RECRUITING_HERO::serialize(unsigned long lPlayerID, int iSortID, int iCountry, int command)
{
	TIMER_HEADER::serialize(sizeof(*this), command);
	this->lPlayerID = lPlayerID;
	this->iSortID = iSortID;
	this->iCountry = iCountry;
}
//警报信息
inline void 
ARMY_WARM::serialize(unsigned long lPlayerID, unsigned long lMapID, unsigned long armyid,int command)
{
	TIMER_HEADER::serialize(sizeof(*this), command);
	this->lPlayerID = lPlayerID;
	this->lMapID = lMapID;
	this->lArmyID = armyid;
}

inline void tagTimer_Hero_Resume::serialize(unsigned int playerid,unsigned int officerid)
{
	PlayerID = playerid;
	OfficerID = officerid;
	TIMER_HEADER::serialize(sizeof(*this),EN_TIMER_HERO_RESUME);
}
#endif