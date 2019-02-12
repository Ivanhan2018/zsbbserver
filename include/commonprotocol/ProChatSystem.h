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
	Normal					=1,		//��ͨ�ı��ʼ�
	Invite_add_friend		=2,		//�����������
	Apply_add_consortia		=3,		//���빫�����룬
	Consortia_union			=4,		//����������룬
	Invite_add_consortia	=5,		//���빫�����룬

}MAIL_TYPE;  //�ʼ�����ö��


typedef enum
{
	Map_Event1  = 1,  //������ռ
	Map_Event2  = 2,  //��ս��״̬
	Map_Event3  = 3,  //��ս������
	Hero_Event1 = 4,  //Ӣ�۱���
	Hero_Event2 = 5,  //Ӣ������
	Hero_Event3 = 6,  //����Ӣ�۱�ɱ
	Hero_Event4 = 7,  //����Ӣ�۱��ͷ�
	Hero_Event5 = 8,  //Ӣ��ս����ս�������
	Hero_Event6 = 9,  //Ӣ�۱��ͷŻ��ɱ
	EVENT_FIGHT_START = 10, //ս����ʼ
	EVENT_FIGHT_END = 11    //ս������
}NEWS_TYPE;				// ����ս������

typedef enum
{
	tBoth	=1, //˽��
	tWorld	=2, //����
	tMap	=3, //��ͼ
	tUnion	=4, //����
	tRoom	=5, //������
	tSystem =6, //ϵͳ�㲥
	tEcype  =7,
	tAffiche=8, //��������Ϣ
	tComonAffiche =9,
	tALL  = 10, //�������
	
}CHAT_TYPE;

typedef enum
{
	FRIEND = 1,
	BLACKLIST = 2,
}FRIEND_TYPE;



struct REQ_ENTER_CHAT_SYSTEM_STRUCT :DISTRIBUTED_HEADER
{
	unsigned long ulUserID;
	unsigned long ulMapID; // ����Ŀ���ͼ
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

struct RSP_EXIT_CHAT_SYSTEM_STRUCT :DISTRIBUTED_HEADER // ���������������,֪ͨ���������������,��������
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
	char Name[SIZE_16] ; //��Ա����
	bool bstate; //�Ƿ�����
	inline void  serialize(int agent,int playerid,int cmd);
}; //

struct REQ_ENTER_CHAT_ROOM_STRUCT :DISTRIBUTED_HEADER
{
	unsigned long ulUserID ;
	inline void serialize(unsigned long );
}; //�������������

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

}; //������ҽ���������

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
	unsigned long	ulMailID;	// �ʼ����
	MAIL_TYPE		type;		//�ʼ�����
	char			sztitle[SIZE_16];   // �ʼ�����
	char			szSendPlayerName[SIZE_24];//����������
	int64			ulTime;    // ����ʱ��
	bool			flag;		//�Ƿ������ʼ��ı�־
	//char			szMailBody[SIZE_256];
};
struct RspEntermailSystem:DISTRIBUTED_HEADER
{
	int icount ;//�ʼ�����
	tagMailBaseInfo mailInfo[50];
	inline void serialize(int length, int agent, int from, int to, int command = RSP_ENTER_MAIL_SYSTEM_INFO);
};

inline void RspEntermailSystem ::serialize(int length, int agent, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(length ,agent ,from,to,command);
}
struct ReqGetMailBodyInfo :DISTRIBUTED_HEADER  // ��ȡ�ʼ�����
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
	unsigned long ulMailID; // �ʼ����
	char szMailBody[SIZE_256]; //������
	char szPertain[SIZE_64];  // ����
	int  flag;				//��ȡ�Ƿ�ɹ�
	inline void serialize(int length, int agent, int from, int to, int command = RSP_GET_MAIL_BODY_INFO);
};

inline void RspGetMaiBodyInfo::serialize(int length, int agent, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(length,agent,from,to,command);
}

struct ReqSendMailInfo :DISTRIBUTED_HEADER
{
	unsigned long ulPlayerID; //
	char szReciverPlayer[SIZE_24];// �ռ�������
	char szTitle[SIZE_16]; //�ʼ�����
	char szMailBody[SIZE_256]; //�ʼ�����
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
	bool flag; // �����Ƿ�ɹ�
	inline void serialize(int length, int agent, int from, int to, int command);
};

inline void RspSendMailInfo::serialize(int length, int agent, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(length,agent,from,to,RSP_SEND_MAIL_INFO);
}

struct NotifyNewMailArrive :DISTRIBUTED_HEADER  // ֪ͨ�����ʼ�����
{
	unsigned long	ulMailID;	// �ʼ����
	MAIL_TYPE		type;		//�ʼ�����
	char			sztitle[SIZE_16];   // �ʼ�����
	char			szSendPlayerName[SIZE_24];//����������
	int64			ulTime;    // ����ʱ��
	bool			flag;		//�Ƿ������ʼ��ı�־
	inline void serialize(int length, int agent, int from, int to, int command = RSP_NEW_MAIL_ARRIVE_INFO)
	{
		DISTRIBUTED_HEADER::serialize(length,agent,from,to,command);
	}
};
struct ReqDeleteMailInfo:DISTRIBUTED_HEADER // ɾ���ʼ���Ϣ
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

struct ReqEnterFriendSystem:DISTRIBUTED_HEADER  //�������/������ϵͳ,��ȡ����/��������Ϣ
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

struct FriendInfo  //����/��������Ϣ
{
	unsigned long	ulUserID;       //���ѱ��
	char			szName[SIZE_24];//��������
	int				ilevel;			//���ѵȼ�
	int				military_rank;	//���Ѿ���
	int             coutryid;
	bool			bIsOnline;		//�Ƿ�����
};

struct RspEnterFriendSystem :DISTRIBUTED_HEADER // ���غ��ѻ��ߺ�������Ϣ
{
	FRIEND_TYPE type;
	int			iCount; //��������
	FriendInfo  firend[50];
	inline void serialize(int length, int agent, int from, int to, int command = RSP_ENTER_FRIEND_SYSTEM_INFO);
};

inline void RspEnterFriendSystem::serialize(int length, int agent, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(length,agent,from,to,command);
}
struct ReqAddFriend :DISTRIBUTED_HEADER // ��Ӻ���/������
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
	FRIEND_TYPE		type;					//��������  ����/������
	FriendInfo      Info;
	int				flag;					//����Ƿ�ɹ��ı�־ 
	//unsigned long	ulFriendID;				//���ѱ��
	//char			szFriendName[SIZE_24];	//��������
	//int				iFriendLevel;			//���ѵȼ�
	//int				iFriendMilitaryRank;	//���Ѿ���
	inline void serialize(int length, int agent, int from, int to, int command = RSP_ADD_FRIEND_INFO);
};

inline void RspAddFriend::serialize(int length, int agent, int from, int to, int command /* = RSP_ADD */)
{
	DISTRIBUTED_HEADER::serialize(length,agent,from,to,command);
}
struct RspInviteFriend:DISTRIBUTED_HEADER
{
	unsigned long	ulFriendID;		//������Ϊ���ѵı��
	char			szName[SIZE_24];//��������
	int				iLevel;			//���ѵȼ�
	int				iMilitary_rank;	//����
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
	bool			flag; // �Ƿ���ܵı�־λ
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
	int flag; //1 = �ɹ�,0 = ʧ��
	unsigned long ulFriendID; // ���ѱ��
	inline void serialize(int length, int agent, int from, int to, int command);
};

inline void RspAcceptAddFriend::serialize(int length, int agent, int from, int to, int command)
{
	DISTRIBUTED_HEADER::serialize(length,agent,from,to,command);
}


struct ReqDeleteFriend:DISTRIBUTED_HEADER  // ɾ������/������
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
	int				flag; // ɾ���Ƿ�ɹ���־
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

	//int newsid_; // newsid = id һ��
	//int newstype_;
	//char content_[SIZE_128];
	unsigned int		iNewsReportID;
	NEWS_TYPE			type;
	char				szConent[SIZE_128];
};

struct RspNewsReport : DISTRIBUTED_HEADER
{
	int					count;				//���͵���������
	tagNewsReport		News[MAX_NEWS_NUM];	//����
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