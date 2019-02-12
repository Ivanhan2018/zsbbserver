#ifndef PRO_NPC_LOGIC_
#define PRO_NPC_LOGIC_
#include "Distributed_Header.h"
#include "ProFightSystem.h"
#define   MAX_WAIT_ROOM_NUM  1000
#define   SEND_MAX_ROOM_INFO 200
#define   MAX_ECTYPE_ROOM_MEM 5
#pragma pack(push ,1)
/////////////////////////////Npc////////////////////////////////////////
struct ReqCreateEctypeRoom :DISTRIBUTED_HEADER
{
	unsigned long ulPlayerID; //玩家编号
	unsigned int  DoorID; //副本编号	
	char		  szEctypeName[SIZE_16];
	char		  szPassWord[SIZE_16];
	char		  szNotify[SIZE_24*2];
	//inline void serialize(int mapid,int player,int command);
	inline void serialize(int mapid,int player,int command = REQ_CREATE_ROOM_INFO );//20091221
};

struct RspCreateEctypeRoom :DISTRIBUTED_HEADER
{
	unsigned long playerID;
	unsigned int  iEctypeRoomID; //返回副本大厅的房间编号
	unsigned char cPasswordflag;// 密码标志位  1＝ 有密码 0 ＝ 没有密码
	char		  cEctypeRoomName[SIZE_16]; //房间名称，创建者的名字
	char          czCreateName[SIZE_16];
	inline void serialize(int length, int agent, int from, int to, int command);
};

struct ReqAddToEctypeRoom : DISTRIBUTED_HEADER //加入房间
{
	//unsigned long ulPlayerID;
	//unsigned int  DoorID;//uEctypeID;
	//unsigned int  ulEctypeRoomID; //加入的副本大厅的房间编号
	//char		  szPassWord[SIZE_16];
	//inline void  serialize(int mapid ,int player,int command);
	unsigned long ulPlayerID;
	unsigned int  uEctypeID;
	unsigned int  ulEctypeRoomID; //加入的副本大厅的队伍编号
	char		  szPassWord[SIZE_16];
	inline void  serialize(int mapid ,int player,int command = REQ_ADD_ECTYPE_ROOM_INFO);//20091221
};

struct RspAddToEctypeRoom :DISTRIBUTED_HEADER
{
	unsigned long ulPlayerID;
	unsigned char flag ; //1=  成功，通知该房间的所有人，失败只返回该请求的人 0 =失败 2 表示 密码错误 3：此房间已进入战场等待状态 4.已在副本中 5: //成员已达上限
	unsigned int uEctypeRoomID;
	char Name[SIZE_24];
	unsigned int faceID;
	int			 iLevel;   //等级
	int			 iMilitaryRank; //军衔
	inline void serialize(int length, int agent, int from, int to, int command);
};
struct ReqKickOutEctypeRoom:DISTRIBUTED_HEADER // 踢出副本房间
{
	unsigned long	ulPlayerID;
	unsigned long	ulOtherID;
	unsigned int	uEctypeID;
	unsigned int	uEcytypeRoomID;
	//inline void serialize( int from, int to, int command);
	inline void serialize( int from, int to, int command = REQ_KICK_OUT_OTHER_INFO);//20091221
};

struct RspKickOutEctypeRoom:DISTRIBUTED_HEADER
{
	bool			flag ;
	unsigned long	ulOtherID;
	unsigned long	uEctypeID;
	unsigned int	uEctypeRoomID;
	inline void serialize(int length, int agent, int from, int to, int command);
};


struct ReqHallInfo :DISTRIBUTED_HEADER //获取大厅信息
{
	//unsigned long plyaerID;  //玩家编号
	//unsigned long DoorID;    //副本编号
	//inline void serialize(int from, int to, int command);
	unsigned long plyaerID;  //玩家编号
	unsigned long EctypeID;  //副本传送门ID
	inline void serialize(int from, int to, int command= REQ_ENTER_HALL_INFO);//20091221
};

struct ReqHallInfoFromArmyList :DISTRIBUTED_HEADER // 双击部队列表时，发送的获取大厅信息
{
	unsigned long armyId;    // 部队ID
	unsigned long plyaerID;  // 玩家编号

	inline void serialize(int from, int to, int command = REQ_ENTER_HALL_INFO_FROM_ARMY_LIST)
	{
		DISTRIBUTED_HEADER::serialize(sizeof(*this),0,from ,to ,command);
	}
};

struct HallInfo
{
	unsigned int uEctypeRoomID; // 副本房间	
	int			 count; //房间人数
	unsigned char cPasswordflag;// 密码标志位  1＝ 有密码 0 ＝ 没有密码
	char		 cName[SIZE_24];//房间名称
	char		 czCreateName[SIZE_16];
};
struct RspHallInfo :DISTRIBUTED_HEADER
{
	bool flag;    //是否成功
	int iRoomCount;  //发送的房间数信息
	HallInfo Info[SEND_MAX_ROOM_INFO]; //房间信息
	inline void serialize(int length, int agent, int from, int to, int command);
};

struct ReqEctypeRoomInfo:DISTRIBUTED_HEADER //进入副本获取副本房间信息
{
	//unsigned int DoorID;//EctypeID;     
	//unsigned int uEctypeRoomID;
	//inline void serialize(int from, int to, int command);
	unsigned int uEctypeID;     
	unsigned int uEctypeRoomID;
	inline void serialize(int from, int to, int command = REQ_GET_ONE_ROOM_INFO);//20091221
};

struct RspEctypeRoomInfo:DISTRIBUTED_HEADER
{
	char name[5][SIZE_24];
	char RoomNote[SIZE_32];
	int count ; //玩家数量
	inline void serialize(int agent, int from, int to, int command);
};

struct ReqEctypeRoomNote :DISTRIBUTED_HEADER // 发送房间公告信息
{
	unsigned long ulplayerID;
	unsigned int  uEctypID;
	unsigned int  uEctypeRoomID;
	char		  RoomNote[SIZE_32];
	//inline void serialize( int from, int to, int command);
	inline void serialize( int from, int to, int command =REQ_SEND_NOTICE_INFO);//20091221
};

struct RspEctypeRoomNote :DISTRIBUTED_HEADER
{
	unsigned int  uEctypeID;
	unsigned int  uEctypeRoomID;
	char		  RoomNote[SIZE_32];
	inline void serialize(int length ,int agent, int from, int to, int command);
};

struct ReqAddArmyToEctype :DISTRIBUTED_HEADER // 添加队伍
{
	bool isSigleEctype; // 是否是单人副本
	unsigned long ulPlayerID;
	unsigned int fromMapId;  // 来源郡ID
	unsigned int  DoorID;//uEctypeID;
	unsigned int  uEctypeRoomID;
	unsigned char ucArmyNum ; //添加部队的数量
	inline void serialize(int from, int to, int command);
	inline unsigned long& operator[](int iIndex);
};

struct ReqLeaveEctypeArmyInfo : DISTRIBUTED_HEADER
{
	unsigned char ucArmyNum ; // 部队的数量
	
	inline void serialize(int from, int to, int command = EVENT_ARMY_LEAVE_ECTYPE)
	{
		DISTRIBUTED_HEADER::serialize(sizeof(*this) + sizeof(unsigned long) * ucArmyNum, 0, from, to, command);
	}

	inline unsigned long& operator[](int iIndex)
	{
		return ((unsigned long*)((unsigned char *)this + sizeof(ReqLeaveEctypeArmyInfo)))[iIndex];
	}
};

// 部队离开或死亡，或副本结束时发的消息
struct RspArmyLeaveEctype : DISTRIBUTED_HEADER
{
	unsigned int playerId; // 部队归属玩家ID
	unsigned int roomId;   // 副本服务器中实际的房间ID
	unsigned int armyId;   // armyId填0表示这个副本结束或实际的部队ID
	inline void serialize(int length, int agent, int from, int to, int command = EVENT_ARMY_LEAVE_OR_DIE_IN_ECTYPE);
};

void RspArmyLeaveEctype::serialize(int length, int agent, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(length, 4000, from, to, command);
}

// 副本exe起来时，注册到游戏大厅服务器所发的消息
struct ESvcRegiInGameHall : DISTRIBUTED_HEADER
{
	unsigned int eServerId;
    inline void serialize(int length, int agent, int from, int to, int command = ECTYPE_SERVER_REGISTER_IN_GAMEHALL);
};

void ESvcRegiInGameHall::serialize(int length, int agent, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(length, 4000, from, to, command);
}

struct RspAddArmyToEctype :DISTRIBUTED_HEADER
{
	unsigned long ulPlayerID;
	unsigned long uEctypeID;
	int count;
	//char ArmyID[MAX_ARMY_NUM][SIZE_24];
	char ArmyName[MAX_ARMY_NUM][SIZE_24];//20091221
	inline void serialize(int length, int agent, int from, int to, int command);
};
//向正在运行的副本、战场、野外等房间中增加部队
struct ReqAddArmyToRoom :DISTRIBUTED_HEADER
{
	int fromMapID; //来源地图
	unsigned long serverID; //实际服务器ID
	int roomID; //实际房间ID
	int count; //部队数量
	inline unsigned long& operator[](int index)//需要加入的部队
	{
		return ((unsigned long *)((char *)this + sizeof(ReqAddArmyToRoom)))[index]  ;
	}
	inline void serialize(int from , int to){
		DISTRIBUTED_HEADER::serialize(sizeof(*this) + sizeof(unsigned long) * count ,0,from,to,ADD_ARMY_TO_ROOM);
	}
};
struct RspInfo{
	unsigned long armyId; //部队ＩＤ
	unsigned char failReason; //失败原因
};
//回应副本、战场、野外等房间中增加部队
struct RspAddArmyToRoom :DISTRIBUTED_HEADER
{
	bool state;//返回状态 0 加入失败 ， 1加入成功
	int count; //加入失败的部队数量
	inline RspInfo& operator[](int index)//加入失败的部队
	{
		return ((RspInfo *)((char *)this + sizeof(RspAddArmyToRoom)))[index]  ;
	}
	inline void serialize(int from , int to){
		DISTRIBUTED_HEADER::serialize(sizeof(*this) + sizeof(RspInfo) * count ,0,from,to,RSP_ADD_ARMY_TO_ROOM);
	}
};
struct ReqEnterEctypeRoom :DISTRIBUTED_HEADER //进入副本大厅房间
{
	unsigned long	ulPlayerID;
	int				DoorID;//iEctypeID;
	unsigned int	uEctypeRoomID;
	inline void		serialize(int from, int to, int command);
};


struct EctypeRoomPlayerInfo  //副本房间信息
{
	unsigned long	playerid;  //玩家信息
	int				faceid;  //头像
	int				count ;    //队伍数量
	unsigned char	cflag; //是否贮备完成标志	
	int             iLevel;
	int				iMilitrayRank;
	char            cHeroName[SIZE_24];
	char			ArmyName[MAX_ARMY_NUM][SIZE_16]; //所带队伍名字
};
struct RspEnterEctypeRoom :DISTRIBUTED_HEADER
{
	int							count;
	EctypeRoomPlayerInfo		RoomPlayerInfo[MAX_ECTYPE_ROOM_MEM];
	inline void serialize(int length, int agent, int from, int to, int command);
};

struct ReqEnterEctypeSystem : DISTRIBUTED_HEADER // 进入副本系统
{
	unsigned long ulPlayer;
	//unsigned int  uEctypeID;
	unsigned int  uEctypeRoomID;
	unsigned int  doorId;
	inline void serialize(int from, int to, int command = REQ_ENTER_ECTYPE_SYSTEM_INFO);
};

struct RspEnterEctypeSystem :DISTRIBUTED_HEADER
{
	bool isSigleEctype; // 是否是单人副本
	unsigned char flag; //1 ＝ 准备好 ，0 = 没有玩家未准备好， 2 ＝ 不是房主  3. 进入副本服务器失败
	int  EctypeID;//副本编号
	int  EcServerID;           //副本服务器ID
	unsigned int Ectypemapid;
	int  EctypeRoomID; //返回副本大厅的房间编号
	int  NpcRoomID;  // 副本系统房间编号
	inline void serialize(int length, int agent, int from, int to, int command);
};

struct ReqExitEctypeHall :DISTRIBUTED_HEADER
{
	unsigned long playerID;
	unsigned int  iEctypeID;
	unsigned int  iRoomId;
	//inline void serialize(int from, int to, int command);
	inline void serialize(int from, int to, int command = REQ_LEAVE_ECTYPE_HALL_INFO);
};

inline void ReqExitEctypeHall::serialize(int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,from,to,command);
}

struct RspExitEctypeHall:DISTRIBUTED_HEADER
{
	bool flag;
	bool IsHost;
	unsigned long ExitPlayerID;
	inline void serialize(int length, int agent, int from, int to, int command);
};
inline void RspExitEctypeHall::serialize(int length, int agent, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(length,agent,from,to,command);
}

// 对于单人副本，野外副本，服务器发给客户端的消息结构体
struct RspSingleEctypeInfo :DISTRIBUTED_HEADER
{
	bool flag;    //是否成功
	unsigned int uEctypeRoomID; // 副本房间ID
	inline void serialize(int length, int agent, int from, int to, int command = RSP_BACK_SINGLE_ECTYPE_ROOM_INFO);
};

// 战场开始等待
struct RspBattleFieldWaitingInfo : DISTRIBUTED_HEADER
{
	inline void serialize(int length, int agent, int from, int to, int command = RSP_BATTLE_FIELD_WAITING_INFO)
	{
		DISTRIBUTED_HEADER::serialize(length, agent, from, to, command);
	}
};

// 取消战场等待
struct ReqCancelBattleFieldWaitingInfo : DISTRIBUTED_HEADER
{
	unsigned int roomId;
	unsigned int playerId;  // 点击取消等待的玩家ID
	unsigned int campId;    // 阵营ID
	unsigned int entranceId; 

	inline void serialize(int length, int agent, int from, int to, int command = REQ_CANCEL_BATTLE_FIELD_WAITING_INFO)
	{
		DISTRIBUTED_HEADER::serialize(length, agent, from, to, command);
	}
};

// 取消战场等待的反馈。
struct RspCancelBattleFieldWaitingInfo : DISTRIBUTED_HEADER
{
	bool flag; // 成功与否

	inline void serialize(int length, int agent, int from, int to, int command = RSP_CANCEL_BATTLE_FIELD_WAITING_INFO)
	{
		DISTRIBUTED_HEADER::serialize(length, agent, from, to, command);
	}
};

inline void RspSingleEctypeInfo::serialize(int length, int agent, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(length, agent, from, to, command);
}

struct ReqExitSingleEctypeInfo : DISTRIBUTED_HEADER
{
	unsigned long playerID;
	//unsigned int  iEctypeID;
	unsigned int  iRoomId;
	unsigned int  doorId;
	inline void serialize(int from, int to, int command);
};

inline void ReqExitSingleEctypeInfo::serialize(int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to, command);
}

//////////////////////////////////////Ectype///////////////////////////////////////////////

struct REQ_ENTER_NPC_SYSTEM_STRUCT : DISTRIBUTED_HEADER//请求进入副本系统
{
	unsigned int uplayerID;
	unsigned int EctypeRoomID;
	int          EctypeID; // 进入的副本编号	
	int			 msgFromMapID; //消息来源服务器  add by ljw 2009 12 03
	//inline void  serialize(int agant,int mapid ,int player , int command);
	inline void  serialize(int playerID ,int EcServerID , int command = REQ_ENTER_NPC_SYSTEM_INFO);
};


struct RSP_ENTER_NPC_SYSTEM_STRUCT : DISTRIBUTED_HEADER //返回请求进入副本信息
{
	bool			flag ;     //进入是否成功标志位
	unsigned int	uPlayerID; //
	unsigned int    uEctypeID; //副本编号
	unsigned int	uNpcMapID; //副本地图
	int				count;     //队伍数量
	ARMY_POS		ArmyInfo[MAX_ARMY_NUM*10];
	inline void  serialize(int lenth,int agent ,int from ,int to , int command);
};
// 返回队伍信息

struct ReqMoveEctypeArmyInfo :DISTRIBUTED_HEADER //请求移动
{
	unsigned int	ulUserID;
	unsigned int    uEctypeID;
	unsigned int    uRoomID;
	unsigned int	ulArmyID;
	unsigned		uMapID ;
	POS_STRUCT		pos;
	inline void serialize(int from,int to  ,int command = REQ_MOVE_ECTYPE_ARMY_INFO );
};

inline void ReqMoveEctypeArmyInfo::serialize(int from ,int to ,int command )
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,from ,to,command);
}
struct RspMoveEctypeArmyInfo :DISTRIBUTED_HEADER
{
	unsigned int ulUserID;
	unsigned int ulArmyID;
	//mapstate state ;
	POS_STRUCT POS;
	bool flag;
	inline void serialize(int length, int agent, int from, int to, int command = RSP_MOVE_ECTYPE_ARMY_INFO);
};

inline void RspMoveEctypeArmyInfo::serialize(int length, int agent, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(length,agent,from,to,command);
}

struct ReqAttackTargetInfo :DISTRIBUTED_HEADER // 设置进攻目标
{
	unsigned int uUserID;
	unsigned int uEctypeID;
	unsigned int uEctypeRoomID;
	unsigned int myArmyID;
	unsigned int TargetArmyID;
	int			 x;
	int			 y;
	inline void serialize(int from ,int to , int command = REQ_ATTACK_ECTYPE_TARGET_INFO);

};//选择攻击目标
inline void ReqAttackTargetInfo::serialize(int from ,int to , int command )
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,from,to,command);
}
struct RspAttackTargetInfo : DISTRIBUTED_HEADER
{
	int flag ;
	inline void serialize(int ,int,int,int,int command = RSP_ATTACK_ECTYPE_TAGGET_INFO);
};
inline void RspAttackTargetInfo::serialize(int length ,int agent,int from,int to ,int command)
{
	DISTRIBUTED_HEADER::serialize(length,agent,from,to,command);
}
struct REQ_EXIT_NPC_SYSTEM_STRUCT :DISTRIBUTED_HEADER
{
	unsigned int uUserID;
	inline void serialize(int agent ,int mapid,int userid,int cmd = REQ_EXIT_NPC_SYSTEM_INFO);
};
inline void REQ_EXIT_NPC_SYSTEM_STRUCT :: serialize(int agent ,int mapid,int userid,int cmd)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent ,mapid,userid,cmd);
}
struct RSP_EXIT_NPC_SYSTEM_STRUCT :DISTRIBUTED_HEADER
{
	unsigned long uUserID;
	//FIGHT_ROLE_TYPE type ;
	inline void serialize(int agent ,int mapid ,int userid,int cmd);
};

struct ReqLeaveEctypeSystem:DISTRIBUTED_HEADER  //离开副本
{
	unsigned long uUserID;
	unsigned int  uEctypeID;
	unsigned int  uEctypeRoomID;
	inline void serialize(int from ,int to ,int command = REQ_LEAVE_ECTYPE_SYSTEM_INFO);
};

inline void ReqLeaveEctypeSystem::serialize(int from ,int to ,int command)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,from,to,command);
}


struct ReqEctypeGetArmyInfo : DISTRIBUTED_HEADER
{
	unsigned long ArmyID;
	unsigned long EctypeID;
	unsigned long EctypeRoomID;
	inline void serialize(int PlayerID, int EctypeServerID, int command = REQ_ECTYPE_GET_ARMY_INFO);
};

inline void ReqEctypeGetArmyInfo::serialize(int PlayerID, int EctypeServerID, int command)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,PlayerID,EctypeServerID,command);
}
struct ReqEctypeRetreatArmyInfo : DISTRIBUTED_HEADER
{
	unsigned long ArmyID;
	unsigned long EctypeID;
	unsigned long EctypeRoomID;
	int  entranceID; //入口ID
	inline void serialize(int PlayerID, int EctypeServerID, int command = REQ_ECTYPE_SYSTEM_ARMY_RETREAT);
};
inline void ReqEctypeRetreatArmyInfo::serialize(int PlayerID, int EctypeServerID, int command)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,PlayerID,EctypeServerID,command);
}
struct RspEctypeGetArmyInfo : DISTRIBUTED_HEADER
{
	unsigned long	ArmyHostID; //队伍玩家编号
	unsigned int	FaceID; //英雄头像编号
	ATTACK_TYPE		Attacktype; //攻击类型(物理/魔法)
	int				AttackPower;//攻击力
	int				DefendPyPower;//防御力
	int				defendMgPower;//魔法防御
	int				soldiercount;//总的兵的数量
	int				totalHp;// HP总数
	int				weaponID; //武器ID
	int				defendID; //防具ID
	bool			TrustFlag; //托管标志
	RACE_TYPE	    raceID;	//种族ID
	char			ArmyName[SIZE_16]; 
	char			szMainHeroName[SIZE_16];
	char            szHeroName[SIZE_16];
	unsigned char	iSodierOccupation;//部队职业
	unsigned short  iLevel; //等级
	unsigned short  iPower;//力量
	unsigned short  iCelerity;//敏捷
	unsigned short  iIntellect;//智力
	unsigned short  iStamina;//耐力

	inline void serialize(int length , int agent,int from ,int to ,int cmd);

};

inline void RspEctypeGetArmyInfo::serialize(int length , int agent,int from ,int to ,int cmd)
{
	DISTRIBUTED_HEADER::serialize(length,agent,from,to,cmd);
}

////////////////////////////////////////////////////////////////////////////////
//test 
struct tagEnterEctype: DISTRIBUTED_HEADER
{
	unsigned long   uPlayerID;
	unsigned int    EcypeID;  
	unsigned int    DoorID;   //传送门ID （3位整数）
	inline void     serialize( int from, int to, int command =REQ_TEST_ENTER_ECTYPE_SYSTEM_INFO );
};
inline void tagEnterEctype::serialize(int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,from,to,command);
}

struct tagRspEnterEctype:DISTRIBUTED_HEADER
{
	bool flag ;
	inline void serialize(int length, int agent, int from, int to, int command =RSP_TEST_ENTER_ECTYPE_SYSTEM_INFO);
};

inline void tagRspEnterEctype::serialize(int length, int agent, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(length,agent,from,to,command);
}
///////////////

struct ReqSetEctypeArmyTrust : DISTRIBUTED_HEADER
{
	unsigned long ulPlayerID;
	unsigned long ulArmyID;
	unsigned int  uEctypeID;
	unsigned int  uEctypeRoomID;
	inline void  serialize(int playerID, int ectServerID, int command = REQ_SET_ECTYPE_ARMY_TRUST_INFO);
};

inline void ReqSetEctypeArmyTrust::serialize(int playerID, int ectServerID, int command)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerID,ectServerID,command);
}
struct RspSetEctypeArmyTrust :DISTRIBUTED_HEADER
{
	bool flag;
	bool IsTrust;
	inline void serialize(int length, int agent, int from, int to, int command);
};

inline void RspSetEctypeArmyTrust::serialize(int length, int agent, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(length,agent,from,to,command);
}

/*
struct RspNoticeEndEctypeInfo :DISTRIBUTED_HEADER
{
	unsigned int EctypeID;
	unsigned int EctypeRoomID;
	int			 flag;
	int			 iKillNpcNum;  //杀死NPC数量
	int			 iDealNum;     //伤亡数量
	int			 iGetBoxNum;	//获取宝箱数量
	inline void serialize(int length, int agent, int from, int to, int command);
};
*/

struct RspNoticeEndEctypeInfo :DISTRIBUTED_HEADER
{
	unsigned int EctypeID;
	unsigned int EctypeRoomID;
	unsigned int ulFromMapID;  //从哪个郡进入副本
	int			 flag;
	int			 iKillNpcNum;  //杀死NPC数量
	int			 iDealNum;     //伤亡数量
	int			 iGetBoxNum;   //获取宝箱数量
	inline void serialize(int length, int agent, int from, int to, int command);
};
inline void RspNoticeEndEctypeInfo::serialize(int length, int agent, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(length,agent,from,to,command);
}

struct ReqBackToHall:DISTRIBUTED_HEADER //返回大厅
{
	unsigned int ulPlayerID;
	unsigned int DoorID;
	int			 iEctypeRoomID;
	inline void serialize(int playerid, int mapid, int command = REQ_BACK_ECTYPE_HALL_INFO);

};

struct ReqEnterFieldEctype : DISTRIBUTED_HEADER // 进入野外副本
{
	unsigned int armyID;
	unsigned int doorId;

	inline void serialize(int from, int to, int command = REQ_ENTER_FIELD_ECTYPE)
	{
		DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, from, to,command);
	}
};

struct RspEnterFieldEctype : DISTRIBUTED_HEADER
{
	unsigned int roomId;
	unsigned int ectypeId;
	unsigned int armyId;

	inline void serialize(int length, int agent, int from, int to, int command = RSP_ENTER_FIELD_ECTYPE)
	{
		DISTRIBUTED_HEADER::serialize(length, agent, from, to, command);
	}
};

inline void ReqBackToHall::serialize(int playerid, int mapid, int command)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,mapid,command);
}
struct RspBackToHall:DISTRIBUTED_HEADER  // 
{
	bool IsRoomHost;   // 是否是房主标志
	unsigned long playerID;
	int  iEctypeRoomID; //退出的房间完家编号
	char Name[SIZE_24];//退出玩家名字
	inline void serialize(int length, int agent, int from, int to, int command = RSP_BACK_ECTYPE_HALL_INFO);
};
inline void RspBackToHall::serialize(int length, int agent, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(length,agent,from,to,command);
}

struct RspDeleteArmyInfo :DISTRIBUTED_HEADER
{
	int count ;
	unsigned long ArmyID[25];
	inline void serialize( int lenth, int agent, int from, int to, int command = RSP_DELETE_ARMY_INFO)
	{
		DISTRIBUTED_HEADER::serialize(lenth,agent,from,to,command);
	}
};

struct Notify_Is_In_Ectype :DISTRIBUTED_HEADER
{
	unsigned long playerID;
	unsigned int  uEctype;
	unsigned int  entranceID;
	unsigned int  uFromMapID;
	inline void serialize(int length, int command = CMD_IS_IN_ECTYPE)
	{
		DISTRIBUTED_HEADER::serialize(length,0,0,0,command);
	}
};
struct Notify_Back_Is_In_Ectype :DISTRIBUTED_HEADER
{
	unsigned long	playerID;
	bool			ret;
	int				roomid;
	int				uEctypID;
    unsigned int  entranceID;

	inline void serialize(int length, int command = CMD_BACK_IS_IN_ECTYPE)
	{
		DISTRIBUTED_HEADER::serialize(length,0,0,0,command);
	}
};
struct ReqEctypeReady:DISTRIBUTED_HEADER
{
	unsigned long ulPlayerID;
	int			  iDoorID;
	int			  iEctypeRoomID;
	unsigned char flag ;// =1 准备号，0 =没有准备号
	inline void serialize( int playerID, int serverID, unsigned char f,int doorid,int roomid,int command =REQ_ECTYPE_READY_INFO)
	{
		this->ulPlayerID = playerID;
		this->flag = f;
		this->iDoorID = doorid;
		this->iEctypeRoomID = roomid;
		DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerID,serverID,command);
	}
};
struct RspEctypeReady:DISTRIBUTED_HEADER
{
	unsigned long ulPlayerID;
	unsigned long flag;
	inline void serialize(int agent, int from, int to, int command =RSP_ECTYPE_READY_INFO)
	{
		DISTRIBUTED_HEADER::serialize(sizeof(*this), agent, from,to,command);
	}
};

struct ReqGetArmyEntranceID : DISTRIBUTED_HEADER
{
	unsigned long armyId;

	inline void serialize(int agent, int from, int to, int command = REQ_GET_ARMY_IN_ENTRANCEID)
	{
		DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,from,to,command);
	}
};

struct RspGetArmyEntranceID : DISTRIBUTED_HEADER
{
	unsigned long entranceId;

	inline void serialize(int agent, int from, int to, int command = RSP_GET_ARMY_IN_ENTRANCEID)
	{
		DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,from,to,command);
	}
};

struct ReqEctypeDeleteArmy:DISTRIBUTED_HEADER
{
	unsigned long ulPlayerID;
	unsigned long ulArmyID;
	int           RoomID;
	inline void serialize(int playerid, int serverid, int roomid,unsigned long Armyid,int command = REQ_ECTYPE_DELETE_ARMY_INFO)
	{
		this->ulPlayerID = playerid;
		this->ulArmyID = Armyid;
		this->RoomID = roomid;
		DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,serverid,command);
	}
};

struct RspEctypeDeleteArmy:DISTRIBUTED_HEADER
{
	unsigned long	ulPlayerID;
	unsigned long	ulArmyID;
	int				iRoomID;
	unsigned char   cFlag;

	inline void serialize(int length, int agent, int from, int to, int command =RSP_ECTYPE_DELETE_ARMY_INFO)
	{
		DISTRIBUTED_HEADER::serialize(length,agent,from,to,command);
	}
};
struct EctypeTurnInfo:DISTRIBUTED_HEADER
{
	unsigned long	RooID;
	int				iTurnNum;
	int				CurToTalArmyNum;
	int             CurProArmyNum;
	inline void serialize(int agent,int from ,int to , int command )
	{
		DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,from,to,command);
	}

};

struct RspGetRse:DISTRIBUTED_HEADER
{
	unsigned long	ItemId;
	unsigned short  ResNum;
	inline void serialize(int agent,int from,int to,int command = RSP_ECTYPE_GET_RES)
	{
		DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,from,to,command);
	}
};

//强制转向，玩家在副本中撤退时，如果没有自己的部队则强制跳转到来源郡
struct ForceVeer :DISTRIBUTED_HEADER
{
	int fromMap; //来源郡ID
	inline void serialize(int agent, int from, int to){
		DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,from,to, EVENT_FORCE_VEER);
	}
};
#pragma pack(pop)


inline void ReqCreateEctypeRoom ::serialize(int mapid,int player,int command)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0 ,player,mapid,command);
}

inline void RspCreateEctypeRoom::serialize(int length, int agent, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(length ,agent,from,to,command);
}

inline void ReqAddToEctypeRoom ::serialize( int mapid ,int player,int command)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0 ,player,mapid,command);
}

inline void RspAddToEctypeRoom ::serialize(int length, int agent, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(length ,agent,from,to,command);
}

inline void ReqKickOutEctypeRoom ::serialize(int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,from,to,command);
}

inline void RspKickOutEctypeRoom::serialize(int length, int agent, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(length,agent,from,to,command);
}

inline void ReqHallInfo::serialize(int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,from ,to ,command);
}

inline void RspHallInfo::serialize(int length, int agent, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(length,agent,from,to,command);
}

inline void ReqEctypeRoomInfo::serialize(int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,from,to ,command);
}

inline void RspEctypeRoomInfo::serialize(int agent, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,from,to,command);
}

inline void ReqEctypeRoomNote::serialize( int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,from,to,command);
}

inline void RspEctypeRoomNote::serialize(int length ,int agent, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(length,agent,from,to,command);
}

inline void ReqAddArmyToEctype::serialize( int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this) + sizeof(unsigned long) * ucArmyNum,0,from,to,command);
}
inline unsigned long& ReqAddArmyToEctype::operator [](int iIndex)
{
	return ((unsigned long*)((unsigned char *)this + sizeof(ReqAddArmyToEctype)))[iIndex];
}
inline void RspAddArmyToEctype::serialize(int length, int agent, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(length,agent,from,to,command);
}

inline void RSP_EXIT_NPC_SYSTEM_STRUCT::serialize(int agent ,int mapid ,int userid,int cmd)
{
	this->uUserID = userid;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,userid,mapid,cmd);
}

//inline void REQ_ENTER_NPC_SYSTEM_STRUCT::serialize(int agant,int mapid ,int player , int command)
//{
//	DISTRIBUTED_HEADER::serialize(sizeof(*this),agant,mapid,player,command);
//}
//20091221
inline void REQ_ENTER_NPC_SYSTEM_STRUCT::serialize(int playerID ,int EcServerID , int command)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerID,EcServerID,command);
}

inline void RSP_ENTER_NPC_SYSTEM_STRUCT::serialize(int lenth,int agent,int playerID ,int mapid , int command)
{
	DISTRIBUTED_HEADER::serialize(lenth,agent,mapid,playerID ,command);
}
//inline void REQ_ENTER_SEE_OTHER_FIGHT_STRUCT ::serialize(int agant , int Mapid,int PlayerID,int command)
//{
//	DISTRIBUTED_HEADER::serialize(sizeof(*this),agant,PlayerID,Mapid,command);
//}
//
//inline void RSP_ENTER_SEE_OTHER_FIGHT_STRUCT::serialize(int mapid,int playerid,int command)
//{
//	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,mapid,playerid,command);
//}

inline void ReqEnterEctypeRoom ::serialize(int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,from ,to,command);
}

inline void RspEnterEctypeRoom::serialize(int length, int agent, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(length,agent,from,to,command);
}

inline void RspEnterEctypeSystem ::serialize(int length, int agent, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(length,agent,from,to,command);
}


inline void ReqEnterEctypeSystem::serialize(int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,from,to,command);
}
#endif