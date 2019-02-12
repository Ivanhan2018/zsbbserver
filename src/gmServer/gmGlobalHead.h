#ifndef __gmGlobalHead_H__
#define __gmGlobalHead_H__

#include "Distributed_Header.h"
#include "Server_Protocol.h"
#include <string.h>


/*
const char *GetGmCmdRetInfo(int retcode)
{
	switch (retcode)
	{
	case 1:
		{
			return "命令执行成功";
		}
	case 2:
		{
			return "非GM账户";
		}
	case 3:
		{
			return "无效命令字";
		}
	case 4:
		{
			return "无此命令权限";
		}
	case 5:
		{
			return "参数格式不正确";
		}
	case 6:
		{
			return "参数值无效";
		}
	case 7:
		{
			return "该功能保留";
		}
	case 8:
		{
			return "无此首领英雄名";
		}
	case 0:
		{
			return "执行时发生错误";
		}
	default:
		{
			return "未知代号";
		}
	}
}

enum
{
	MIN_PACKET_LEN		=	sizeof(DISTRIBUTED_HEADER),		//包最小长度
	MAX_PACKET_LEN		=	300								//包最大长度
};
struct TS_NetRead
{
	int				Command;
	int				BufLen;
	char			Buf[MAX_PACKET_LEN];
};
*/
#pragma pack(push ,1)

//Login GM (UM,禁登录，禁言)
struct tagGmLogin:DISTRIBUTED_HEADER
{
	inline void serialize(unsigned long playerid);
};

//回复Login GM
struct tagRepGmLogin:DISTRIBUTED_HEADER
{
	char cLogin;   //1禁 0不禁
	char cSpeak;   //1禁 0不禁
	inline void serialize(int agent,unsigned long playerid,char clogin,char cspeak);
};

//GM Logout GM 
struct tagGmLogout:DISTRIBUTED_HEADER
{
	inline void serialize(unsigned long playerid);
};

//禁言协议 
struct tagGmSpeakDisabled:DISTRIBUTED_HEADER
{
	inline void serialize(unsigned long playerid);
};

//禁登录协议
struct tagGmLoginDisabled:DISTRIBUTED_HEADER
{
	inline void serialize(unsigned long playerid);
};

//禁言回复
struct tagRepSpeak:DISTRIBUTED_HEADER
{
	char bDisabled;   //1禁 0不禁
	inline void serialize(int agent,unsigned long playerid,char disabled);
};

//禁登录回复
struct tagRepLogin:DISTRIBUTED_HEADER
{
	char bDisabled;   //1禁 0不禁
	inline void serialize(int agent,unsigned long playerid,char disabled);
};
inline
void tagRepLogin::serialize(int agent,unsigned long playerid,char disabled)
{
	this->bDisabled = disabled;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_GAME_MANAGE_SERVER,playerid,GC_LOGINDISABLED);
}

//读取势力公告
struct tagGetNoticeForces:DISTRIBUTED_HEADER
{
	int frocesID;			//请照ChannelType
	inline void serialize(int fid,unsigned long playerid);
};

//回复势力公告
struct tagRepNoticeForces:DISTRIBUTED_HEADER
{
	int  DataLen;
	char cData[GM_DATA_SIZE+1];
	inline void serialize(int agentid,unsigned long playerid,const char * data);
};

//GM回统一结构
struct tagRepGM:DISTRIBUTED_HEADER
{
	int bSucceed;
	int DataLen;
	char cData[GM_DATA_SIZE+1];
	inline void serialize(int agentid,unsigned long playerid,int bSucceed,const char * data);
};

//GM命令
struct tagGMCommand:DISTRIBUTED_HEADER
{
	int DataLen;
	char cData[GM_DATA_SIZE+1];
	inline void serialize(unsigned long playerid,const char * data);
};

//GM普通公告（广播）
struct tagNoticeBroadCast:DISTRIBUTED_HEADER
{
	int DataLen;
	char cData[GM_DATA_SIZE+1];
	inline void serialize(const char * data);
};

#pragma pack(pop)

inline
void tagGmLogin::serialize(unsigned long playerid)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,MDM_GP_GAME_MANAGE_SERVER,GC_LOGIN);
}


inline
void tagRepGmLogin::serialize(int agent,unsigned long playerid,char clogin,char cspeak)
{
	this->cLogin=clogin;
	this->cSpeak=cspeak;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_GAME_MANAGE_SERVER,playerid,GC_LOGIN);
}

inline 
void tagGmLogout::serialize(unsigned long playerid)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,MDM_GP_GAME_MANAGE_SERVER,GC_LOGOUT);	
}

inline 
void tagGmSpeakDisabled::serialize(unsigned long playerid)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,MDM_GP_GAME_MANAGE_SERVER,GC_SPEAKDISABLED);
}

inline 
void tagGmLoginDisabled::serialize(unsigned long playerid)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,MDM_GP_GAME_MANAGE_SERVER,GC_LOGINDISABLED);
}

inline 
void tagRepSpeak::serialize(int agent,unsigned long playerid,char disabled)
{
	this->bDisabled =disabled;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_GAME_MANAGE_SERVER,playerid,GC_SPEAKDISABLED);
}

inline 
void tagGetNoticeForces::serialize(int fid,unsigned long playerid)
{
	this->frocesID = fid;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,MDM_GP_GAME_MANAGE_SERVER,GC_NOTICEFORCES);
}

inline 
void tagRepNoticeForces::serialize(int agentid,unsigned long playerid,const char * data)
{
	if (data)
	{
		DataLen = (int)strlen(data);
		if ( DataLen>GM_DATA_SIZE) DataLen=GM_DATA_SIZE;
		strncpy(cData,data,DataLen);
	}
	else
	{
		DataLen = 0;
	}
	cData[DataLen]='\0';

	DISTRIBUTED_HEADER::serialize(sizeof(*this)-(GM_DATA_SIZE)+DataLen,agentid,MDM_GP_GAME_MANAGE_SERVER,playerid,GC_NOTICEFORCES);
}

inline 
void tagRepGM::serialize(int agentid,unsigned long playerid,int bSucceed,const char * data)
{
	this->bSucceed = bSucceed;
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
	DISTRIBUTED_HEADER::serialize(sizeof(*this)-(GM_DATA_SIZE)+DataLen,agentid,MDM_GP_GAME_MANAGE_SERVER,playerid,GC_GMCOMMAND);
}

inline 
void tagGMCommand::serialize(unsigned long playerid,const char * data)
{
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
	DISTRIBUTED_HEADER::serialize(sizeof(*this)-(GM_DATA_SIZE)+DataLen,0,playerid,MDM_GP_GAME_MANAGE_SERVER,GC_GMCOMMAND);
}

inline 
void tagNoticeBroadCast::serialize(const char * data)
{
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
	DISTRIBUTED_HEADER::serialize(sizeof(*this)-(GM_DATA_SIZE)+DataLen,0,MDM_GP_GAME_MANAGE_SERVER,0,GC_NOTICEBREADCAST);
}


#endif
