#ifndef PRO_CHAT_H
#define PRO_CHAT_H

#define MAX_SIZE_CHAT_ROOM  8 
#define MAX_CHAT_ROOM_NUM   5000
#define MAX_NEWS_NUM		20
#include "Global_Macro.h"
#include "Distributed_Header.h"
#include <string.h>
#pragma pack(push ,1)
typedef enum
{
	Normal					=1,		//普通文本邮件
	Invite_add_friend		=2,		//好友添加申请
	Apply_add_consortia		=3,		//加入公会申请，
	Consortia_union			=4,		//公会结盟申请，
	Invite_add_consortia	=5,		//加入公会邀请，

}MAIL_TYPE;  //邮件类型枚举


typedef enum
{
	Map_Event1  = 1,  //郡被攻占
	Map_Event2  = 2,  //郡战斗状态
	Map_Event3  = 3,  //郡战报概括
	Hero_Event1 = 4,  //英雄被俘
	Hero_Event2 = 5,  //英雄阵亡
	Hero_Event3 = 6,  //被俘英雄被杀
	Hero_Event4 = 7,  //被俘英雄被释放
	Hero_Event5 = 8,  //英雄战斗中战死或俘获
	Hero_Event6 = 9,  //英雄被释放或绞杀
	EVENT_FIGHT_START = 10, //战斗开始
	EVENT_FIGHT_END = 11    //战斗结束
}NEWS_TYPE;				// 新闻战报类型

typedef enum
{
	tBoth	=1, //私聊
	tWorld	=2, //国家
	tMap	=3, //地图
	tUnion	=4, //军团
	tRoom	=5, //聊天室
	tSystem =6, //系统广播
	tEcype  =7,
	tAffiche=8, //公告类消息
	tComonAffiche =9,
	tALL  = 10, //所有玩家
	
}CHAT_TYPE;

typedef enum
{
	FRIEND = 1,
	BLACKLIST = 2,
}FRIEND_TYPE;



struct REQ_ENTER_CHAT_SYSTEM_STRUCT :DISTRIBUTED_HEADER
{
	unsigned long ulUserID;
	unsigned long ulMapID; // 进入的郡地图
	char Name[SIZE_16];
	inline void serialize(unsigned long, unsigned long, char*);
};

struct CHAT_INFO_STRUCT :DISTRIBUTED_HEADER
{
	unsigned long ulUserID; //
	char ulOtherUser[SIZE_16];
	CHAT_TYPE ChatType;
	char info[SIZE_256] ;
	inline void serialize(unsigned long, unsigned long ,char * ,CHAT_TYPE , char* ,int cmd = REQ_SNED_CHAT_INFO,int agent = 0);
	inline void serialize_Ex(unsigned long ,char* ,CHAT_TYPE,char* ,int serverid,int cmd  = REQ_SNED_CHAT_INFO,int agent = 0);
};

struct REQ_EXIT_CHAT_SYSTEM_STRUCT :DISTRIBUTED_HEADER
{
	unsigned long ulUserID;
	inline void serialize(unsigned long );
};

struct RSP_EXIT_CHAT_SYSTEM_STRUCT :DISTRIBUTED_HEADER // 如果进入了聊天室,通知聊天室里其他玩家,他下线了
{
	unsigned long ulUserID;
	inline void serialize(int length, int agent, int from, int to, int command);
};

inline void RSP_EXIT_CHAT_SYSTEM_STRUCT::serialize(int length, int agent, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(length,agent,from,to,command);
}

struct  CHAT_ROOM_MEMBER_STRUCT :DISTRIBUTED_HEADER
{
	unsigned long ulUserID;
	char Name[SIZE_16] ; //成员名称
	bool bstate; //是否在线
	inline void  serialize(int agent,int playerid,int cmd);
}; //

struct REQ_ENTER_CHAT_ROOM_STRUCT :DISTRIBUTED_HEADER
{
	unsigned long ulUserID ;
	inline void serialize(unsigned long );
}; //申请进入聊天室

struct RSP_ENTER_CHAT_ROOM_STRUCT : DISTRIBUTED_HEADER
{
	unsigned long ulUserID ;
	int RoomID;
	bool flag ;
	int MemberNum ;
	CHAT_ROOM_MEMBER_STRUCT Member[8];
	inline void  serialize(int agent,int playerid,int cmd);
}; //


struct RSP_OTHER_ENTER_CHAT_ROOM_STRUCT :DISTRIBUTED_HEADER
{
	unsigned ulUserID ;
	inline void  serialize(int agent,int playerid,int cmd = RSP_OTHER_ENTER_CHAT_ROOM);
};


struct REQ_INVITE_OTHER_ENTER_CHAT_ROOM_STRUCT :DISTRIBUTED_HEADER
{
	unsigned long ulUserID;
	char szOtherName[SIZE_24];
	int ulRoomID;
	inline void serialize(unsigned long ,char* ,unsigned long );

}; //邀请玩家进入聊天室

struct  RSP_INVITE_OTHER_ENTER_CHAT_ROOM_STRUCT : DISTRIBUTED_HEADER
{
	bool flag ;
	unsigned long  ulOtherID;
	int  ChatRoomID;
	char Name[SIZE_16];
	inline void  serialize(int agent,int playerid,int cmd);
};

struct  CHAT_MEMBER_ROOM_INFO
{
	unsigned long ulRoomID;
	unsigned long ulPlayerID[8];
};

struct  REQ_EXIT_CHAT_ROOM_STRUCT : DISTRIBUTED_HEADER
{
	unsigned long ulUserID;
	unsigned long ulRoomID;
	inline void serialize(unsigned long userid,unsigned long roomid);
};

struct RspExitChatRoom :DISTRIBUTED_HEADER
{
	unsigned long ulUserID;
	inline void serialize(int length, int agent, int from, int to, int command = RSP_EXIT_CHAT_ROOM);
};

inline void RspExitChatRoom::serialize(int length, int agent, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(length,agent,from,to,command);
}
struct REQ_CHANGE_MAP_STRUCT :DISTRIBUTED_HEADER
{
	unsigned long ulUserID;
	unsigned long ulMapID;
	inline void serialize(unsigned long UserID,unsigned long mapId);
};
////////////////////////////Friend system pro//////////////////////////////////////////////
struct ReqEnterMailSystem :DISTRIBUTED_HEADER // enter mail system ,
{
	unsigned long playerID;
	inline void serialize(unsigned long playerID,int serverID  = MDM_GP_MAP_CHAT );
};

inline void ReqEnterMailSystem::serialize(unsigned long playerID,int serverID )
{
	this->playerID = playerID;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerID,serverID,REQ_ENTER_MAIL_SYSTEM_INFO);
}
struct tagMailBaseInfo
{
	unsigned long	ulMailID;	// 邮件编号
	MAIL_TYPE		type;		//邮件类型
	char			sztitle[SIZE_16];   // 邮件标题
	char			szSendPlayerName[SIZE_24];//发件人名字
	int64			ulTime;    // 发送时间
	bool			flag;		//是否是新邮件的标志
	//char			szMailBody[SIZE_256];
};
struct RspEntermailSystem:DISTRIBUTED_HEADER
{
	int icount ;//邮件数量
	tagMailBaseInfo mailInfo[50];
	inline void serialize(int length, int agent, int from, int to, int command = RSP_ENTER_MAIL_SYSTEM_INFO);
};

inline void RspEntermailSystem ::serialize(int length, int agent, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(length ,agent ,from,to,command);
}
struct ReqGetMailBodyInfo :DISTRIBUTED_HEADER  // 获取邮件内容
{
	unsigned long ulUserID;
	unsigned long ulMailID;
	inline void serialize(unsigned long UserID,unsigned long MailID,int serverID = MDM_GP_MAP_CHAT);
};

inline void ReqGetMailBodyInfo::serialize(unsigned long UserID,unsigned long MailID,int serverID)
{
	this->ulUserID = UserID;
	this->ulMailID = MailID;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,UserID,serverID,REQ_GET_MAIL_BODY_INFO);
}
struct RspGetMaiBodyInfo:DISTRIBUTED_HEADER
{
	unsigned long ulMailID; // 邮件编号
	char szMailBody[SIZE_256]; //邮主体
	char szPertain[SIZE_64];  // 附件
	int  flag;				//获取是否成功
	inline void serialize(int length, int agent, int from, int to, int command = RSP_GET_MAIL_BODY_INFO);
};

inline void RspGetMaiBodyInfo::serialize(int length, int agent, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(length,agent,from,to,command);
}

struct ReqSendMailInfo :DISTRIBUTED_HEADER
{
	unsigned long ulPlayerID; //
	char szReciverPlayer[SIZE_24];// 收件人名字
	char szTitle[SIZE_16]; //邮件标题
	char szMailBody[SIZE_256]; //邮件主体
	inline void serialize(unsigned long playerid,char * szReciverPlyaername,char* title,char* mailbody,int serverid =MDM_GP_MAP_CHAT);
};

inline void ReqSendMailInfo::serialize(unsigned long playerid,char * szReciverPlyaername,char* title,char* mailbody,int serverid /* =MDM_GP_MAP_CHAT */)
{
	this->ulPlayerID = playerid;
	strcpy(this->szReciverPlayer,szReciverPlyaername);
	strcpy(this->szTitle,title);
	strcpy(this->szMailBody,mailbody);
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,serverid,REQ_SEND_MAIL_INFO);
}

struct RspSendMailInfo :DISTRIBUTED_HEADER
{
	bool flag; // 发送是否成功
	inline void serialize(int length, int agent, int from, int to, int command);
};

inline void RspSendMailInfo::serialize(int length, int agent, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(length,agent,from,to,RSP_SEND_MAIL_INFO);
}

struct NotifyNewMailArrive :DISTRIBUTED_HEADER  // 通知有新邮件到达
{
	unsigned long	ulMailID;	// 邮件编号
	MAIL_TYPE		type;		//邮件类型
	char			sztitle[SIZE_16];   // 邮件标题
	char			szSendPlayerName[SIZE_24];//发件人名字
	int64			ulTime;    // 发送时间
	bool			flag;		//是否是新邮件的标志
	inline void serialize(int length, int agent, int from, int to, int command = RSP_NEW_MAIL_ARRIVE_INFO)
	{
		DISTRIBUTED_HEADER::serialize(length,agent,from,to,command);
	}
};
struct ReqDeleteMailInfo:DISTRIBUTED_HEADER // 删除邮件信息
{
	unsigned long ulUserID;
	unsigned long ulMailID;
	inline void serialize(unsigned long ulUserID, unsigned long ulMailID ,int serverID = MDM_GP_MAP_CHAT) ;
};

inline void ReqDeleteMailInfo ::serialize(unsigned long ulUserID, unsigned long ulMailID ,int serverID /* = MDM_GP_MAP_CHAT */)
{
	this->ulUserID = ulUserID;
	this->ulMailID = ulMailID;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,ulUserID,serverID,REQ_DELETE_MAIL_INFO);
}
struct RspDeleteMailInfo:DISTRIBUTED_HEADER
{
	unsigned long mailID;
	bool flag;
	inline void serialize(int length, int agent, int from, int to, int command)
	{
		DISTRIBUTED_HEADER::serialize(length ,agent ,from ,to,command);
	}
};


//////////////////////////////////friend system ////////////////////////////////////////

struct ReqEnterFriendSystem:DISTRIBUTED_HEADER  //进入好友/黑名单系统,获取好友/黑名单信息
{
	unsigned long	ulUserID;
	FRIEND_TYPE		type;
	inline void serialize(unsigned long UserID,FRIEND_TYPE type ,int serverID = MDM_GP_MAP_CHAT);
};

inline void ReqEnterFriendSystem::serialize(unsigned long UserID,FRIEND_TYPE type, int serverID /* = MDM_GP_MAP_CHAT */)
{
	this->ulUserID = UserID;
	this->type = type;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,UserID,serverID,REQ_ENTER_FRIEND_SYSTEM_INFO);
}

struct FriendInfo  //好友/黑名单信息
{
	unsigned long	ulUserID;       //好友编号
	char			szName[SIZE_24];//好友名字
	int				ilevel;			//好友等级
	int				military_rank;	//好友军衔
	int             coutryid;
	bool			bIsOnline;		//是否再线
};

struct RspEnterFriendSystem :DISTRIBUTED_HEADER // 返回好友或者黑名单信息
{
	FRIEND_TYPE type;
	int			iCount; //好友数量
	FriendInfo  firend[50];
	inline void serialize(int length, int agent, int from, int to, int command = RSP_ENTER_FRIEND_SYSTEM_INFO);
};

inline void RspEnterFriendSystem::serialize(int length, int agent, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(length,agent,from,to,command);
}
struct ReqAddFriend :DISTRIBUTED_HEADER // 添加好友/黑名单
{
	unsigned long ulUserID;
	char		  szFirendName[SIZE_24];
	FRIEND_TYPE   type;
	inline void  serialize(unsigned long userid,char * friendname,FRIEND_TYPE type,int serverid = MDM_GP_MAP_CHAT);
};

inline void ReqAddFriend::serialize(unsigned long userid,char * friendname,FRIEND_TYPE type,int serverid /* = MDM_GP_MAP_CHAT */)
{
	this->ulUserID  = userid;
	this->type = type;
	strcpy(this->szFirendName , friendname);
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,userid,serverid,REQ_ADD_FRIEND_INFO);
}

struct RspAddFriend:DISTRIBUTED_HEADER // 
{
	FRIEND_TYPE		type;					//好友类型  好友/黑名单
	FriendInfo      Info;
	int				flag;					//添加是否成功的标志 
	//unsigned long	ulFriendID;				//好友编号
	//char			szFriendName[SIZE_24];	//好友名字
	//int				iFriendLevel;			//好友等级
	//int				iFriendMilitaryRank;	//好友军衔
	inline void serialize(int length, int agent, int from, int to, int command = RSP_ADD_FRIEND_INFO);
};

inline void RspAddFriend::serialize(int length, int agent, int from, int to, int command /* = RSP_ADD */)
{
	DISTRIBUTED_HEADER::serialize(length,agent,from,to,command);
}
struct RspInviteFriend:DISTRIBUTED_HEADER
{
	unsigned long	ulFriendID;		//邀请你为好友的编号
	char			szName[SIZE_24];//好友名字
	int				iLevel;			//好友等级
	int				iMilitary_rank;	//军衔
	inline void serialize(int length,int agent,int from ,int to ,int command);
};

inline void RspInviteFriend::serialize(int length,int agent,int from ,int to ,int command)
{
	DISTRIBUTED_HEADER::serialize(length,agent,from,to,command);
}
struct ReqAcceptAddFriend:DISTRIBUTED_HEADER
{
	unsigned long	ulUserID;
	unsigned long	ulFriendID;
	bool			flag; // 是否接受的标志位
	inline void serialize(unsigned long userid,unsigned long friendid,int serverid = MDM_GP_MAP_CHAT);
};

inline void ReqAcceptAddFriend::serialize(unsigned long userid,unsigned long friendid,int serverid /* = MDM_GP_MAP_CHAT */)
{
	this->ulUserID = userid;
	this->ulFriendID = friendid;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,userid,serverid,REQ_ACCEPT_ADD_FRIEND_INFO);
}

struct RspAcceptAddFriend:DISTRIBUTED_HEADER
{
	int flag; //1 = 成功,0 = 失败
	unsigned long ulFriendID; // 好友编号
	inline void serialize(int length, int agent, int from, int to, int command);
};

inline void RspAcceptAddFriend::serialize(int length, int agent, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(length,agent,from,to,command);
}


struct ReqDeleteFriend:DISTRIBUTED_HEADER  // 删除好友/黑名单
{
	unsigned long	ulUserID;
	unsigned long	ulFirendID;
	FRIEND_TYPE		type;
	inline void serialize(unsigned long ulUserID,unsigned long FirendID,FRIEND_TYPE type ,int serverID =MDM_GP_MAP_CHAT );
};
inline void ReqDeleteFriend::serialize(unsigned long ulUserID,unsigned long FirendID,FRIEND_TYPE type ,int serverID /* =MDM_GP_MAP_CHAT */ )
{
	this->ulUserID = ulUserID;
	this->ulFirendID = FirendID;
	this->type = type;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,ulUserID,serverID,REQ_DELETE_FRIEND_INFO);
}

struct RspDeleteFriend:DISTRIBUTED_HEADER
{
	unsigned long	ulFriendID;
	int				flag; // 删除是否成功标志
	FRIEND_TYPE		type;
	inline void serialize(int length, int agent, int from, int to, int command = RSP_DELETE_FRIEND_INFO);
};

inline void RspDeleteFriend::serialize(int length, int agent, int from, int to, int command )
{
	DISTRIBUTED_HEADER::serialize(length,agent,from,to,command);
}


struct RspNotifyFriendOnline:DISTRIBUTED_HEADER
{
	unsigned long ulFriendID;
	char		  szName[SIZE_24];
	inline void serialize(int length, int agent, int from, int to, int command);
};
struct RspNotifyFriendDownLine:DISTRIBUTED_HEADER
{
	unsigned long ulFriendID;
	char		  szName[SIZE_24];
	inline void serialize(int length,int agent,int from ,int to, int command);
};

inline void RspNotifyFriendOnline::serialize(int length, int agent, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(length,agent,from,to,command);

}
inline void RspNotifyFriendDownLine::serialize(int length, int agent, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(length,agent,from,to,command);

}

//////////////////////////////////fight new system////////////////////////////////////////


struct tagNewsReport
{

	//int newsid_; // newsid = id 一条
	//int newstype_;
	//char content_[SIZE_128];
	unsigned int		iNewsReportID;
	NEWS_TYPE			type;
	char				szConent[SIZE_128];
};

struct RspNewsReport : DISTRIBUTED_HEADER
{
	int					count;				//发送的新闻条数
	tagNewsReport		News[MAX_NEWS_NUM];	//新闻
	inline void		serialize(int length, int agent, int from, int to, int command);
};

struct RspNotifyNewsReport:DISTRIBUTED_HEADER
{
	tagNewsReport Reprot;
	inline void serialize( int agent, int from, int to, int command = NOTIFY_ADD_NEWS_REPROT_INFO)
	{
		DISTRIBUTED_HEADER::serialize(sizeof(tagNewsReport),agent,from,to,command);
	}
};

inline void RspNewsReport::serialize(int length, int agent, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(length,agent,from,to,command);
}

struct ReqGetNewsReportInfo :DISTRIBUTED_HEADER
{
	unsigned long playerID;
	inline void serialize(unsigned long playerID)
	{
		DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerID,MDM_GP_MAP_CHAT,REQ_NEWS_REPORT_INFO);
	}
};
#pragma pack(pop)
inline void REQ_ENTER_CHAT_SYSTEM_STRUCT ::serialize(unsigned long userid, unsigned long mapid, char* name)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, userid, MDM_GP_MAP_CHAT, this->command);

	this->ulUserID = userid;
	this->ulMapID = mapid ;
	strcpy(this->Name,name);
}
inline void CHAT_INFO_STRUCT ::serialize(unsigned long from, unsigned long to ,char* Other,CHAT_TYPE type,char* info,int cmd , int agent)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), agent, from ,to, cmd);
	this->ChatType = type;
	this->ulUserID=from;
	//this->ulUserID = ulUserID;
	memcpy(this->ulOtherUser,Other,SIZE_16-1);
	this->ulOtherUser[SIZE_16-1]='\0';
	//memset(this->info,0,SIZE_256);
	memcpy(this->info ,info,strlen(info));
	this->info[strlen(info)]='\0';
}
inline void CHAT_INFO_STRUCT::serialize_Ex(unsigned long ulUserID,char* Other ,CHAT_TYPE type,char* info ,int serverid,int cmd ,int agent /* = 0 */)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,ulUserID,serverid,cmd);
	this->ChatType = type;
	strcpy(this->ulOtherUser,Other);
	strcpy(this->info,info);
}
inline void REQ_ENTER_CHAT_ROOM_STRUCT ::serialize(unsigned long userid)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, userid, MDM_GP_MAP_CHAT, this->command);
	this->ulUserID =userid;
}
inline void REQ_INVITE_OTHER_ENTER_CHAT_ROOM_STRUCT ::serialize(unsigned long userid,char* name ,unsigned long roomid  )
{
	this->ulUserID = userid;
	strcpy(this->szOtherName ,name);
	this->ulRoomID = roomid;
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, userid, MDM_GP_MAP_CHAT, this->command);
}
inline void REQ_EXIT_CHAT_ROOM_STRUCT::serialize(unsigned long userid,unsigned long roomid)
{
	this->ulUserID = userid;
	this->ulRoomID = roomid;
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, userid, MDM_GP_MAP_CHAT, this->command);
}
inline void REQ_EXIT_CHAT_SYSTEM_STRUCT::serialize(unsigned long userid)
{
	this->ulUserID = userid;
}
inline void REQ_CHANGE_MAP_STRUCT::serialize(unsigned long UserID,unsigned long mapId)
{
	this->ulUserID = UserID;
	this->ulMapID = mapId;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,UserID,MDM_GP_MAP_CHAT,REQ_CHANGE_MAP_INFO);
}

inline void RSP_INVITE_OTHER_ENTER_CHAT_ROOM_STRUCT ::serialize(int agent,int playerid,int cmd)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CHAT,playerid,cmd);
}

inline void RSP_ENTER_CHAT_ROOM_STRUCT ::serialize(int agent,int playerid,int cmd)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CHAT,playerid,cmd);
}

inline void RSP_OTHER_ENTER_CHAT_ROOM_STRUCT::serialize(int agent,int playerid,int cmd)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CHAT,playerid,cmd);
}

inline void CHAT_ROOM_MEMBER_STRUCT ::serialize(int agent,int playerid,int cmd)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CHAT,playerid,cmd);
}

#endif