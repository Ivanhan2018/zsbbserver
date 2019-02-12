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
			return "����ִ�гɹ�";
		}
	case 2:
		{
			return "��GM�˻�";
		}
	case 3:
		{
			return "��Ч������";
		}
	case 4:
		{
			return "�޴�����Ȩ��";
		}
	case 5:
		{
			return "������ʽ����ȷ";
		}
	case 6:
		{
			return "����ֵ��Ч";
		}
	case 7:
		{
			return "�ù��ܱ���";
		}
	case 8:
		{
			return "�޴�����Ӣ����";
		}
	case 0:
		{
			return "ִ��ʱ��������";
		}
	default:
		{
			return "δ֪����";
		}
	}
}

enum
{
	MIN_PACKET_LEN		=	sizeof(DISTRIBUTED_HEADER),		//����С����
	MAX_PACKET_LEN		=	300								//����󳤶�
};
struct TS_NetRead
{
	int				Command;
	int				BufLen;
	char			Buf[MAX_PACKET_LEN];
};
*/
#pragma pack(push ,1)

//Login GM (UM,����¼������)
struct tagGmLogin:DISTRIBUTED_HEADER
{
	inline void serialize(unsigned long playerid);
};

//�ظ�Login GM
struct tagRepGmLogin:DISTRIBUTED_HEADER
{
	char cLogin;   //1�� 0����
	char cSpeak;   //1�� 0����
	inline void serialize(int agent,unsigned long playerid,char clogin,char cspeak);
};

//GM Logout GM 
struct tagGmLogout:DISTRIBUTED_HEADER
{
	inline void serialize(unsigned long playerid);
};

//����Э�� 
struct tagGmSpeakDisabled:DISTRIBUTED_HEADER
{
	inline void serialize(unsigned long playerid);
};

//����¼Э��
struct tagGmLoginDisabled:DISTRIBUTED_HEADER
{
	inline void serialize(unsigned long playerid);
};

//���Իظ�
struct tagRepSpeak:DISTRIBUTED_HEADER
{
	char bDisabled;   //1�� 0����
	inline void serialize(int agent,unsigned long playerid,char disabled);
};

//����¼�ظ�
struct tagRepLogin:DISTRIBUTED_HEADER
{
	char bDisabled;   //1�� 0����
	inline void serialize(int agent,unsigned long playerid,char disabled);
};
inline
void tagRepLogin::serialize(int agent,unsigned long playerid,char disabled)
{
	this->bDisabled = disabled;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_GAME_MANAGE_SERVER,playerid,GC_LOGINDISABLED);
}

//��ȡ��������
struct tagGetNoticeForces:DISTRIBUTED_HEADER
{
	int frocesID;			//����ChannelType
	inline void serialize(int fid,unsigned long playerid);
};

//�ظ���������
struct tagRepNoticeForces:DISTRIBUTED_HEADER
{
	int  DataLen;
	char cData[GM_DATA_SIZE+1];
	inline void serialize(int agentid,unsigned long playerid,const char * data);
};

//GM��ͳһ�ṹ
struct tagRepGM:DISTRIBUTED_HEADER
{
	int bSucceed;
	int DataLen;
	char cData[GM_DATA_SIZE+1];
	inline void serialize(int agentid,unsigned long playerid,int bSucceed,const char * data);
};

//GM����
struct tagGMCommand:DISTRIBUTED_HEADER
{
	int DataLen;
	char cData[GM_DATA_SIZE+1];
	inline void serialize(unsigned long playerid,const char * data);
};

//GM��ͨ���棨�㲥��
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
