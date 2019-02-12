#ifndef __ProTaskSystem_H__
#define __ProTaskSystem_H__
#include "Distributed_Header.h"
#include "Server_Protocol.h"
#include <string.h>

//define TaskSystemserverID	MDM_GP_MAP_TASK(1901)
#define MIN_PACKET_LEN   sizeof(DISTRIBUTED_HEADER)

/*
struct TS_NetRead
{
	int				Command;
	unsigned int	Len;
	char			Buf[SIZE_128];
};
*/
#pragma pack(push ,1)


/* 任务来源(个人任务，公会任务，国家任务，系统任务-静态任务表) */
enum TaskFrom
{
	tfPerson	=	0x01,
	tfUnion		=	0x02,
	tfCountry	=	0x03,
	tfSystem	=	0x04,
};


/* 任务数据(对应任务表) 
   任务ID，任务状态，任务状态时间，任务来源，任务类型，任务名称，任务等级，玩家等级，军衔等级，任务道具，
   前序任务，任务奖励，任务内容，任务描述	
*/
struct BASE_TASK_INFO
{
    unsigned int 	TaskID;
	char			TaskName[SIZE_32];
	int				TaskType;
	int				TaskLevel;
	int				PlayerLevel;
	int				PlayerRank;
	int				TaskItem;
	int				PreTaskID;
	char			Bonus[SIZE_128];
	char			Contest[SIZE_128];
};
struct OTHER_TASK_INFO
{
	int					taskstate;
	int					taskfrom;
	unsigned __int64	time;
};

struct PLAYER_TASK_INFO
{
	unsigned int	TaskID;
	OTHER_TASK_INFO	OtherInfo;
};

/* 获取玩家任务完成个数 */
struct GET_TASK_COUNT:DISTRIBUTED_HEADER
{
	inline void serialize(unsigned long);
};

inline 
void GET_TASK_COUNT::serialize(unsigned long playerid)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,MDM_GP_MAP_TASK,TC_GET_TASK_COUNT);
}

/* 回复玩家任务完成个数 */
struct REP_TASK_COUNT:DISTRIBUTED_HEADER
{
	int count;
	inline void serialize(int agentid,unsigned long playerid,int count);
};
inline 
void REP_TASK_COUNT::serialize(int agentid,unsigned long playerid,int count)
{
	this->count = count;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agentid,MDM_GP_MAP_TASK,playerid,TC_REP_TASK_COUNT);
}


/* 玩家获取任务列表 */
struct GET_TASK_LIST:DISTRIBUTED_HEADER
{
	inline void serialize(unsigned long);
};
inline void 
GET_TASK_LIST::serialize(unsigned long playerid)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,MDM_GP_MAP_TASK,TC_GET_TASK_LIST);
}

/* 玩家请求更改任务状态 */
struct SET_TASK_STATE:DISTRIBUTED_HEADER
{
	unsigned int    TaskID;
	int				 taskstate;  
	inline void serialize(unsigned long, unsigned int,int);
};
inline void 
SET_TASK_STATE::serialize(unsigned long playerid, unsigned int taskid, int taskstate)
{
	this->TaskID	=	taskid;
	this->taskstate	=	taskstate;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,MDM_GP_MAP_TASK,TC_SET_TASK_STATE);
}

/* 发送玩家任务列表 */
struct REP_GET_TASK_LIST:DISTRIBUTED_HEADER
{
	unsigned char	bSucced;
	unsigned int	TaskCount;			  
	unsigned int	BufLen;
    inline void    serialize(int, unsigned long,unsigned char,unsigned int);
};
inline void 
REP_GET_TASK_LIST::serialize(int agendtid,unsigned long palyerid, unsigned char bSucceed_, unsigned int taskcount)
{
	this->bSucced	=	bSucceed_;
	this->TaskCount	=	taskcount;
	this->BufLen	=	sizeof(PLAYER_TASK_INFO) * taskcount;
	DISTRIBUTED_HEADER::serialize(sizeof(*this) + this->BufLen,agendtid,MDM_GP_MAP_TASK,palyerid,TC_REP_GET_TASK_LIST);
}

/* 发送玩家请求更改任务状态结果 */
struct REP_SET_TASK_STATE:DISTRIBUTED_HEADER
{
	unsigned int	 TaskID;
	int		        tasksate;
	unsigned char	 bSucceed;
	inline void serialize(int,unsigned long, unsigned int, int, unsigned char);
};
inline void 
REP_SET_TASK_STATE::serialize(int agentid, unsigned long playerid, unsigned int taskid, int taskstate, unsigned char bsucceed)
{
	this->TaskID	=	taskid;
	this->tasksate	=	taskstate;
	this->bSucceed	=	bsucceed;
	DISTRIBUTED_HEADER::serialize(sizeof(*this), agentid, MDM_GP_MAP_TASK, playerid, TC_REP_SET_TASK_STATE);
}


/* 主动发布任务 */
struct  ISSUE_USER_TASK :DISTRIBUTED_HEADER
{
	PLAYER_TASK_INFO TaskInfo;
	inline void serialize(int agentid,unsigned long playerid);
	inline void serialize(const PLAYER_TASK_INFO & taskinfo);
};
inline void
ISSUE_USER_TASK::serialize(int agentid,unsigned long playerid)
{
    DISTRIBUTED_HEADER::serialize(sizeof(*this), agentid, MDM_GP_MAP_TASK, playerid,TC_ISSUE_USER_TASK);
}
inline void
ISSUE_USER_TASK::serialize(const PLAYER_TASK_INFO & taskinfo)
{
	this->TaskInfo	=	taskinfo;
}

//inline void 
//ISSUE_USER_TASK::serialize(int agentid,unsigned long playerid, unsigned int taskid, TaskFrom taskfrom, TaskType tasktype,
//						   char * bonus, char * contest,char * describe)
//{
//	this->TaskID	=	taskid;
//	this->taskfrom	=	taskfrom;
//	this->tasktype	=	tasktype;
//	strcpy(this->Bonus,bonus);
//	strcpy(this->Contest,contest);
//	strcpy(this->Describe,describe);
//	DISTRIBUTED_HEADER::serialize(sizeof(*this), agentid, MDM_GP_MAP_TASK, playerid,TC_ISSUE_USER_TASK);
//}

/* 任务完成状态发送*/
struct REP_TASKSTATE_COMPLETE:DISTRIBUTED_HEADER
{
	unsigned int TaskID;
    inline void serialize(int,unsigned long, unsigned int);
};
inline void
REP_TASKSTATE_COMPLETE::serialize(int agentid,unsigned long playerid, unsigned int taskid)
{
	this->TaskID	=	taskid;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agentid,MDM_GP_MAP_TASK,playerid,TC_REP_TASKSTATE_COMPLETE);
}

/* 请求检测是否完成 */
struct REQ_CHECK_TASK_OK:DISTRIBUTED_HEADER
{
	inline void serialize(unsigned long);
};
inline void
REQ_CHECK_TASK_OK::serialize(unsigned long playerid)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,MDM_GP_MAP_TASK,TC_REQ_CHECK_TASK_OK);
}
//以下移至Server_Protocol.h文件中...

///* 玩家等级、军衔变更事件 */
//struct USER_PLAYERLEVEL_PLAYERRANK_EVENT:DISTRIBUTED_HEADER
//{
//	unsigned long	PlayerID;
//	int				PalyerLevel;
//	int				PlayerRank;
//    inline void serialize(int,unsigned long, int, int);
//};
//inline void
//USER_PLAYERLEVEL_PLAYERRANK_EVENT::serialize(int agentid, unsigned long playerid, int playerlevel, int playerrank)
//{
//	DISTRIBUTED_HEADER::serialize(sizeof(*this),agentid,0,MDM_GP_MAP_TASK,TC_USER_PLAYERLEVEL_PLAYERRANK_EVENT);
//	this->PlayerID		=	playerid;
//	this->PalyerLevel	=	playerlevel;
//	this->PlayerRank	=	playerrank;
//}
//
//
///* 玩家任务道具触发事件 */
//struct USER_TASKITEM_EVENT:DISTRIBUTED_HEADER
//{
//	unsigned long	PlayerID;
//	int				PlayerLevel;
//	int				PlayerRank;
//	unsigned int	TaksItem;
//	inline void serialize(int,unsigned long, int, int, unsigned int);
//};
//inline void
//USER_TASKITEM_EVENT::serialize(int agentid, unsigned long playerid, int playerlevel, int playerrank, unsigned int taskitem)
//{
//	DISTRIBUTED_HEADER::serialize(sizeof(*this),agentid,0,MDM_GP_MAP_TASK,TC_USER_TASKITEM_EVENT);
//	this->PlayerID		=	playerid;
//	this->PlayerLevel	=	playerlevel;
//	this->PlayerRank	=	playerrank;
//	this->TaksItem		=	taskitem;
//}
//
//
///* 玩家任务完成事件 */
//struct USER_TASK_COMPLETE_EVENT:DISTRIBUTED_HEADER
//{
//	unsigned long	PlayerID;
//	unsigned int	TaskID;
//	inline void serialize(int,unsigned long, unsigned int); 
//};
//inline void
//USER_TASK_COMPLETE_EVENT::serialize(int agendtid, unsigned long playerid, unsigned int taskid)
//{
//	DISTRIBUTED_HEADER::serialize(sizeof(*this),agendtid,0,MDM_GP_MAP_TASK,TC_USER_TASK_COMPLETE_EVENT);
//	this->PlayerID		=	playerid;
//	this->TaskID		=	taskid;
//}
//
///* 副本任务完成事件(玩家ID，副本ID，杀死BOSS标识，杀敌数，捣毁建筑数) */
//struct ECTYPE_EVENT :DISTRIBUTED_HEADER
//{
//	unsigned long	PlayerID;
//	unsigned int	EctypeID;
//	unsigned char	Boss;
//	unsigned int	NpcNums;
//	unsigned int	BuildNums;
//	inline void	serialize(int, unsigned long, unsigned int, unsigned char, unsigned int, unsigned int);
//};
//inline void 
//ECTYPE_EVENT::serialize(int agentid, unsigned long playerid, unsigned int ectypeid, unsigned char boss, unsigned int npcnums, unsigned int buildnums)
//{
//	this->PlayerID	=	playerid;
//	this->EctypeID	=	ectypeid;
//	this->Boss		=	boss;
//	this->NpcNums	=	npcnums;
//	this->BuildNums	=	buildnums;
//	DISTRIBUTED_HEADER::serialize(sizeof(*this), agentid,0,MDM_GP_MAP_TASK,TC_ECTYPE_EVENT);
//}

///* 玩家进入任务系统 */
//struct ENTER_TASK:DISTRIBUTED_HEADER
//{
//   inline void serialize(int, int);
//};
//inline void 
//ENTER_TASK::serialize(int from, int to)
//{
//	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,from,to,TS_ENTER_TASK);
//}P

///* 玩家离开任务系统 */
//struct EXIT_TASK:DISTRIBUTED_HEADER
//{
//   inline void serialize(int, int);
//};
//inline void 
//EXIT_TASK::serialize(int from, int to)
//{
//	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,from,to,TS_EXIT_TASK);
//}
//
///* 玩家离开任务系统 */
//struct MULTI_EXIT_TASK:DISTRIBUTED_HEADER
//{
//   inline void serialize(int,int, int);
//};
//inline void 
//MULTI_EXIT_TASK::serialize(int agentid,int from, int to)
//{
//	DISTRIBUTED_HEADER::serialize(sizeof(*this),agentid,from,to,TS_MULTI_EXIT_TASK);
//}


#pragma pack(pop)

#endif
