#ifndef __SERVER_PROTOCOL__
#define __SERVER_PROTOCOL__

#include "Global_Macro.h"
#include "DBTableHeader.h"
#include "ProChatSystem.h"
#pragma warning(disable:4100)

#pragma pack(push,1)

//GM管理
//修改势力公告事件(-发给聊天服务器)
struct MODIFY_NOTICEFORCES_EVENT:DISTRIBUTED_HEADER
{
	int NoticeID;	//请参照NoticeType
	int ForcesID;  //请照ChannelType
	int DataLen;
	char cData[GM_DATA_SIZE+1];
	inline void serialize(int nid,int fid,const char * data);
};
inline 
void MODIFY_NOTICEFORCES_EVENT::serialize(int nid,int fid,const char * data)
{
	this->ForcesID = fid;
	this->NoticeID = nid;
	if (data)
	{
		DataLen = (int)strlen(data);
		if (DataLen>GM_DATA_SIZE) DataLen=GM_DATA_SIZE;
		strncpy(cData,data,DataLen);
	}
	else
	{
		DataLen = 0;
	}
	cData[DataLen]='\0';
	DISTRIBUTED_HEADER::serialize(sizeof(*this)-(GM_DATA_SIZE+1)+DataLen,0,MDM_GP_GAME_MANAGE_SERVER,MDM_GP_MAP_CHAT,GC_NOTICEFORCES_EVENT);
}
/*
//GM私聊事件（-聊天服务器)（保留）
struct GMCHAT_EVENT : DISTRIBUTED_HEADER
{
	int fromGmID;			//邀请者
	int toGmID;				//被邀请者
	inline void serialize(int fid,int tid);
};
inline 
void GMCHAT_EVENT::serialize(int fid,int tid)   
{
	this->fromGmID = fid;
	this->toGmID	= tid;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,MDM_GP_GAME_MANAGE_SERVER,MDM_GP_MAP_CHAT,GC_CHAT_EVENT);
}
*/

/*   begin 任务系统事件  */
/* 玩家等级、军衔变更事件 */

struct USER_PLAYERLEVEL_PLAYERRANK_EVENT:DISTRIBUTED_HEADER
{
	unsigned long	PlayerID;
	int				PalyerLevel;
	int				PlayerRank;
    inline void serialize(int,unsigned long, int, int);
};
inline void
USER_PLAYERLEVEL_PLAYERRANK_EVENT::serialize(int agentid, unsigned long playerid, int playerlevel, int playerrank)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agentid,0,MDM_GP_MAP_TASK,TC_USER_PLAYERLEVEL_PLAYERRANK_EVENT);
	this->PlayerID		=	playerid;
	this->PalyerLevel	=	playerlevel;
	this->PlayerRank	=	playerrank;
}


/* 玩家任务道具触发事件 */
struct USER_TASKITEM_EVENT:DISTRIBUTED_HEADER
{
	unsigned long	PlayerID;
	int				PlayerLevel;
	int				PlayerRank;
	unsigned int	TaksItem;
	inline void serialize(int,unsigned long, int, int, unsigned int);
};
inline void
USER_TASKITEM_EVENT::serialize(int agentid, unsigned long playerid, int playerlevel, int playerrank, unsigned int taskitem)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agentid,0,MDM_GP_MAP_TASK,TC_USER_TASKITEM_EVENT);
	this->PlayerID		=	playerid;
	this->PlayerLevel	=	playerlevel;
	this->PlayerRank	=	playerrank;
	this->TaksItem		=	taskitem;
}


/* 玩家任务完成事件 */
struct USER_TASK_COMPLETE_EVENT:DISTRIBUTED_HEADER
{
	unsigned long	PlayerID;
	unsigned int	TaskID;
	inline void serialize(int,unsigned long, unsigned int); 
};
inline void
USER_TASK_COMPLETE_EVENT::serialize(int agendtid, unsigned long playerid, unsigned int taskid)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agendtid,0,MDM_GP_MAP_TASK,TC_USER_TASK_COMPLETE_EVENT);
	this->PlayerID		=	playerid;
	this->TaskID		=	taskid;
}

/* 副本任务完成事件(玩家ID，副本ID，杀死BOSS标识，杀敌数，捣毁建筑数) */
struct ECTYPE_EVENT :DISTRIBUTED_HEADER
{
	unsigned long	PlayerID;
	unsigned int	EctypeID;
	unsigned char	Boss;
	unsigned int	NpcNums;
	unsigned int	BuildNums;
	inline void	serialize(int, unsigned long, unsigned int, unsigned char, unsigned int, unsigned int);
};
inline void 
ECTYPE_EVENT::serialize(int agentid, unsigned long playerid, unsigned int ectypeid, unsigned char boss, unsigned int npcnums, unsigned int buildnums)
{
	this->PlayerID	=	playerid;
	this->EctypeID	=	ectypeid;
	this->Boss		=	boss;
	this->NpcNums	=	npcnums;
	this->BuildNums	=	buildnums;
	DISTRIBUTED_HEADER::serialize(sizeof(*this), agentid,0,MDM_GP_MAP_TASK,TC_ECTYPE_EVENT);
}
/*    end 任务系统事件 */


////evet struct
struct startFight
{
	int cmd;
	unsigned long MapID;
};
struct ArmyArrive: DISTRIBUTED_HEADER
{
	int cmd ;
	unsigned long uArmyID;
	unsigned long uFromMapID;
	unsigned long uToMapID;
	unsigned long uMidMapID; //到达目标郡的前一个郡的编号
	inline void serialize(int length, int agent, int from, int to, int command = CMD_ARMY_ARRIVED);
};

inline void ArmyArrive::serialize(int length, int agent, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(length,4000,from,to,cmd);
}

struct EndFight:DISTRIBUTED_HEADER
{
	int cmd;
	unsigned long uMapID;
	int winer; // 1 = 联盟 2 ,部落

	inline void serialize(int length, int agent, int from, int to, int command = CMD_END_FIGHT);
};

inline void EndFight::serialize(int length, int agent, int from, int to, int command )
{
	DISTRIBUTED_HEADER::serialize(length,4000,from,to,command);
}

struct tagArmy_Reach : DISTRIBUTED_HEADER
{
	inline void serialize(unsigned long lPlayerID,unsigned long lFromMapID,unsigned long uMidMapID, unsigned long lToMapID,unsigned long lArmyID, int sortid,int command/* = CMD_ARMY_REACH*/);
	unsigned long							lPlayerID;					//出征玩家的ID
	unsigned long							lFromMapID;
	unsigned long							lToMapID;					//到达目标郡ID
	unsigned long							uMidMapID;					//到达目标郡的前一个郡的编号
	unsigned long							lArmyID;
	int										sortID;						//是否使用加速卡：0不使用；非0使用
};

struct tagBackArmy_Reach:DISTRIBUTED_HEADER
{
	unsigned long ulPlayerID;
	unsigned long ulFromMapID;
	unsigned long lArmyID;
	int				ret ; // ret = 1 成功 0 ＝ 失败
	inline void serialize(unsigned long playerid,unsigned long frommapid,unsigned long armyid,int ret)
	{
		DISTRIBUTED_HEADER::serialize(sizeof(*this),0,0,0,CMD_BACK_ARMY_REACH);
		this->lArmyID =  armyid;
		this->ulFromMapID = frommapid;
		this->ulPlayerID = playerid;
		this->ret = ret;
	}
};
inline void
tagArmy_Reach::serialize(unsigned long lPlayerID,unsigned long lFromMapID,unsigned long uMidMapID,unsigned long lToMapID,unsigned long lArmyID, int sortid,int command)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, command);//CMD_ARMY_REACH / CMD_ARMY_ARRIVED
	//this->cmd = cmd;
	this->lPlayerID = lPlayerID;
	this->lFromMapID = lFromMapID;
	this->uMidMapID = uMidMapID;
	this->lToMapID = lToMapID;
	this->lArmyID = lArmyID;
	this->sortID = sortid;
}
struct tagHeroParam : DISTRIBUTED_HEADER
{
	inline void serialize(long long lStartFight, unsigned long lPlayerID,unsigned long lHeroID,unsigned long lMapID,char szArmyName[],int iExp,int iCredit,
		int iEnemyCount,int iGeneralCount,int iState);
	long long			lStartFight;		//开始战争时间
	unsigned long		lPlayerID;			//玩家ID
	unsigned long		lHeroID;			//英雄ID
	unsigned long		lMapID;				//郡ID
	char				szArmyName[SIZE_32];//部队名称
	int					iExp;				//经验值
	int					iCredit;			//荣誉点
	int					iEnemyCount;		//杀敌数
	int					iGeneralCount;		//俘虏将数
	int					iState;				//状态 //0,空闲 1,带队 2,重伤 3,被俘 4 俘虏 见类型 HERO_STATE
};

struct tagCaptiveHero : DISTRIBUTED_HEADER
{
	inline void serialize(unsigned long lPlayerID,unsigned long lHeroID,unsigned long lByPlayerID,unsigned long lByHeroID);
	unsigned long		lPlayerID;			//玩家ID
	unsigned long		lHeroID;			//英雄ID
	unsigned long		lByPlayerID;		//被俘英雄 玩家ID
	unsigned long		lByHeroID;			//被俘英雄ID
};
inline void
tagCaptiveHero::serialize(unsigned long lPlayerID,unsigned long lHeroID,unsigned long lByPlayerID,unsigned long lByHeroID)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, 0);
	this->lPlayerID = lPlayerID;
	this->lHeroID = lHeroID;
	this->lByPlayerID = lByPlayerID;
	this->lByHeroID = lByHeroID;
}
//玩家的资源量
struct tagPlayerResource : DISTRIBUTED_HEADER
{
	inline void serialize(int iFoodCount,int iWoodCount,int iIronCount,int iSkinCount,int iRedStoneCount,int iYellowStoneCount,
		int iGreenStoneCount,int iBackIronCount,int iMoney,int iScience,int iContribute);
	int								iFoodCount;				//粮食数量
	int								iWoodCount;				//木材数量
	int								iIronCount;				//铁数量
	int								iSkinCount;				//皮数量
	int								iRedStoneCount;			//红宝石
	int								iYellowStoneCount;		//黄宝石
	int								iGreenStoneCount;		//绿宝石
	int								iBackIronCount;			//黑铁
	int								iMoney;					//钱
	int								iScience;				//科技点
	int								iContribute;			//捐献度
};
inline void
tagHeroParam::serialize(long long lStartFight, unsigned long lPlayerID,unsigned long lHeroID,unsigned long lMapID,char szArmyName[],int iExp,int iCredit,
							int iEnemyCount,int iGeneralCount,int iState)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, CMD_HERO_PARAM);
	this->lStartFight = lStartFight;
	this->lPlayerID = lPlayerID;
	this->lHeroID = lHeroID;
	this->lMapID = lMapID;
	this->iExp = iExp;
	this->iCredit = iCredit;
	this->iEnemyCount = iEnemyCount;
	this->iGeneralCount = iGeneralCount;
	this->iState = iState;
	strcpy(this->szArmyName, szArmyName);
}
inline void
tagPlayerResource::serialize(int iFoodCount,int iWoodCount,int iIronCount,int iSkinCount,int iRedStoneCount,int iYellowStoneCount,
							int iGreenStoneCount,int iBackIronCount,int iMoney,int iScience,int iContribute)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, CMD_PLAYER_UPDATA_RESOURCE);
	this->iFoodCount = iFoodCount;
	this->iWoodCount = iWoodCount;
	this->iIronCount = iIronCount;
	this->iSkinCount = iSkinCount;
	this->iRedStoneCount = iRedStoneCount;
	this->iYellowStoneCount = iYellowStoneCount;
	this->iGreenStoneCount = iGreenStoneCount;
	this->iBackIronCount = iBackIronCount;
	this->iMoney = iMoney;
	this->iScience = iScience;
	this->iContribute =iContribute ;
}

struct ArmyId2MapId
{
	unsigned long armyId;
	unsigned long mapId;
};

// 野外副本结束事件
struct EventEctypeOver :DISTRIBUTED_HEADER{
	unsigned int entraceID; 
	inline void serialize(int from, int to)
	{
		DISTRIBUTED_HEADER::serialize(sizeof(*this),4000,from,to, EVENT_ECTYPE_OVER);
	}
};
struct Notify_Enter_Ectype :DISTRIBUTED_HEADER
{
	//int				cmd;
	unsigned int	uEctypeID;
	unsigned int    entranceID;
	unsigned int    uEctypeRoom[2];
	unsigned int    playerCount;
	unsigned int	iArmyCount;

	inline unsigned long& getPlayerId(int iIndex)
	{
		return ((unsigned long*)((unsigned char *)this + sizeof(Notify_Enter_Ectype)))[iIndex];
	}

	inline ArmyId2MapId& getArmyInfo(int iIndex)
	{
		return ((ArmyId2MapId*)((unsigned char *)this + sizeof(Notify_Enter_Ectype) + sizeof(unsigned long) * playerCount))[iIndex];
	}

	inline void serialize(int length, int agent, int from, int to, int command = CMD_ENTER_ECTYPE)
	{
		DISTRIBUTED_HEADER::serialize(length,4000,from,to,command);
	}
};

struct Notify_Back_Enter_Ectype :DISTRIBUTED_HEADER // 返回的通知
{
	//int cmd ;
	int ret ;//1 表示成功,0表示房间已满 等待
	int RoomID;
	int uEctypeRoomID[2];
	int uEctypID;
	int mapid;
	unsigned int entranceID;


	inline void serialize(int length, int agent, int from, int to, int command = CMD_BACK_ENTER_ECTYPE);
};

inline void Notify_Back_Enter_Ectype::serialize(int length, int agent, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(length,4000,from,to,command);
}

struct Notify_Back_Army_Ectype:DISTRIBUTED_HEADER
{
	int cmd ;
	int uEctypeID;
	int iArmyCount;
	int entranceID; //入口ID
	unsigned long uArmyID[20*5];
	inline void serialize(int length, int agent, int from, int to, int command = CMD_BACK_ARMY_ECTYPE);
};
inline void Notify_Back_Army_Ectype::serialize(int length, int agent, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(length,4000,from,to,command);
}

struct Notify_Add_Main_Hero:DISTRIBUTED_HEADER  // t通知创建一个主英雄事件
{
	int		iOfficerID;
	char	szOfficerName[SIZE_24];
	inline void serialize(int cmd = CMD_ADD_MAIN_HERO);
};
inline void Notify_Add_Main_Hero::serialize(int cmd /* = CMD_ADD_MAIN_HERO */)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this),4000,0,0,cmd);
}

struct Notify_Send_Mail :DISTRIBUTED_HEADER // 通知邮件系统发送的邮件
{
	MAIL_TYPE		type;					//邮件类型
	unsigned long	ulRecieverPlayerID;		//收件人编号
	char			szSendPlayerName[SIZE_16];//发件人名称
	char			sztitle[SIZE_16];		//邮件标题
	char			szbody[SIZE_256];		//邮件主体
	char			szPertain[SIZE_64];    //附件

	inline void serialize(int cmd = CMD_ADD_MAIl_NOTIFY)
	{
		DISTRIBUTED_HEADER::serialize(sizeof(*this),4000,0,0,cmd);
	}
};

struct Notify_BroadMail:DISTRIBUTED_HEADER//通知按阵营群发邮件
{
	int				CountryID;			//阵营，0:全服;1：联盟；2：部落;
	char			szSendPlayerName[SIZE_16];//发件人名称
	char			sztitle[SIZE_16];		//邮件标题
	char			szbody[SIZE_256];		//邮件主体
	inline void serialize()
	{
		DISTRIBUTED_HEADER::serialize(sizeof(*this),4000,0,0,CMD_BROADMAIL_NOTIFY);
	}
};
struct Notify_Add_Consortia :DISTRIBUTED_HEADER //通知有人加如公会
{
	unsigned long		ulPlayerID;
	unsigned long		ulConsortiaID;
	inline void serialize(int cmd = CMD_ADD_CONSORTIA_NOTIFY)
	{
		DISTRIBUTED_HEADER::serialize(sizeof(*this),4000,0,0,cmd);
	}
};

struct NewsFightReprotEvent_1
{
	int64 time;			// 时间
	unsigned int	  MapID;		// 郡地图
	unsigned int	  type;			//攻占方的阵营
};


struct NewsFightReprotEvent_2
{
	int64 time;			//时间
	unsigned int MapID;			//郡编号
};

struct HeroNewsReportEvent_1
{
	int64 time;
	char  MailHero1[SIZE_16];		//被捕获英雄的首领名字
	char  Hero[SIZE_16];			//被捕获英雄的名字
	char  MailHero2[SIZE_16];		//俘获方的首领英雄名字
};

struct HeroNewsReportEvnet_2
{
	int64 time;
	char  MailHero[SIZE_16]; //首领英雄的名字
	char  Hero[SIZE_16];		//阵亡英雄名字
};

typedef HeroNewsReportEvent_1 HeroNewsReportEvent_3;
typedef HeroNewsReportEvent_1 HeroNewsReportEvent_4;

struct Map_NewsReportEvent3
{
	unsigned int   AttackRole;  // 1＝进攻方是联盟 2 ＝ 进攻方是部落
	int64 StartTime;  // 战斗开始时间
	unsigned int mapid;//地点
	int64 EndTime;//结束时间，没有结束为0
	short warstate;// 1 = 依然浴血奋战 2 =成功占领该地区 3= 暂时战略撤退
	bool  bJoin;   //1= 积极  0 ＝ 没有
	unsigned int   ADealSolderNum;//进攻方伤亡
	unsigned int   RDealSolderNum;//防守方伤亡
	unsigned int   DestorySNum;//摧毁梢塔数量
	unsigned int   DestoryCNum;//摧毁城市数量
	unsigned int   ACatchOfficerNum; //进攻方被俘获将领数量
	unsigned int   RCatchOfficerNum; //防守方被俘获将领数量
	unsigned int   ADealOfficerNum;  //进攻方战死将领数量
	unsigned int   RDealOfficerNum;  //防守战死将领数量
};

struct HeroNewsReportEvent_5  //在战斗中俘获或格杀
{
	int64 time;       //时间
	int   MapID;      //地点
	char  szAPlayerName[SIZE_16]; // 玩家a
	char  szOfficerName[SIZE_16]; // 将领名字
	char  szBPlayerName[SIZE_16]; // 玩家B
	int   ret ;						// 1= 俘获 2 ＝ 格杀
};

struct HeroNewsReportEvent_6 //释放或绞杀将领
{
	char   szAPlayerName[SIZE_16]; // 玩家A
	bool   bRedeem;				   //是否赎回
	char   szOfficerName[SIZE_16]; //将领名字
	char   szBPlayerName[SIZE_16]; // 玩家B
	int    ret ;					// 1＝  释放  0 ＝ 格杀
};
struct Notify_Add_News_Report:DISTRIBUTED_HEADER
{
	NEWS_TYPE		type; // 战报类型
	char			szConent[SIZE_128];
	inline void serialize( int agent, int command = CMD_ADD_NEWS_REPORT_NOTIFY)
	{
		DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,0,0,command);
	}
};

struct HeroStudySkillEvent:DISTRIBUTED_HEADER
{
	unsigned long ulArmyID; // 部队编号
	unsigned long ulofficerID;//指挥官编号
	unsigned int ulskillID; //技能编号
	int				iLevel;
	inline void serialize(unsigned long ulArmyID,unsigned long officerid,unsigned int skill,int level,int cmd= CMD_HERO_STUDY_SKILL)
	{
		this->ulArmyID = ulArmyID;
		this->ulofficerID = officerid;
		this->ulskillID = skill;
		this->iLevel = level;
		DISTRIBUTED_HEADER::serialize(sizeof(*this),4000,0,0,cmd);
	}
};
struct heroModifyParamEvent:DISTRIBUTED_HEADER
{
	unsigned long PlayerID;
	unsigned long OfficerID;
	inline void serialize(int playerid,int officerid)
	{
		PlayerID = playerid;
		OfficerID = officerid;
		DISTRIBUTED_HEADER::serialize(sizeof(*this),4000,SERVER_BOUND_MAP_OTHER,0,CMD_UPDATE_HERO_PARAM);
	}
};


struct MsgEctypeDelPlayer :DISTRIBUTED_HEADER{
	int ectypeID; // 副本ID
	int playerNum; //玩家数量
	inline void serialize(int playerNum){
		this->playerNum = playerNum;
		DISTRIBUTED_HEADER::serialize(sizeof(MsgEctypeDelPlayer) + sizeof(unsigned int) * playerNum ,4000,0,0,CMD_DEL_ECTYPE_PLAYER);
	}
	inline unsigned int & operator[] (int index){
		return ((unsigned int*)((unsigned char *)this + sizeof(MsgEctypeDelPlayer)))[index];
	}
};
struct MsgEctypeFightOver :DISTRIBUTED_HEADER{
	int roomID; //房间ID
	inline void serialize(int roomID){
		this->roomID = roomID;
		DISTRIBUTED_HEADER::serialize(sizeof(MsgEctypeFightOver) ,4000,0,0,CMD_ECTYPE_FIGHT_OVER);
	}
};
#pragma pack(pop)

#endif