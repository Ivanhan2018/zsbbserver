#ifndef CONSORTIAPROTOCOL_H__
#define CONSORTIAPROTOCOL_H__

#include <string.h>
#include "Distributed_Header.h"
#include "Global_Macro.h"
#ifndef	MAX_LOGO_SIZE
#	define MAX_LOGO_SIZE			4096	//�Զ���Logo���������
#endif

#ifdef PRESS_TEST
#define MAX_COUNT_PAGE				1		//ÿҳ���ļ�¼��
#else
#define MAX_COUNT_PAGE				30		//ÿҳ���ļ�¼��
#endif
/****************************����Ȩ�޶���********************************/
#define PURVIEW_AGRESS_JOIN			0x000001	//��׼���빫���Ȩ��
#define PURVIEW_INVITE_JOIN			0x000002	//�����Ȩ��
#define PURVIEW_KICK_USER			0x000004	//���˵�Ȩ��
#define PURVIEW_OFFICIAL			0x000008	//��ְ��
#define PURVIEW_MODIFY_PLACARD		0x000010	//�޸Ĺ���
#define PURVIEW_MODIFY_LOGO			0x000020	//�޸�LOGO
#define PURVIEW_DISBAND_CON			0x000040	//��ɢ����
#define PURVIEW_MANAGE_BANK			0x000080	//֧�乫������
#define PURVIEW_MODIFY_TENT			0x000100	//�޸Ĺ�����ּ
#define PURVIEW_FINANCE				0x000200	//����Ȩ��
#define PURVIEW_WAR					0x000400	//��սȨ��
#define PURVIEW_REMOVAL				0x000800	//��ְ��	
#define PURVIEW_AUTHORIZE			0x001000	//��ȨȨ��
#define PURVIEW_UPDATECONSORTIA		0x002000	//��������Ȩ��
#define PURVIEW_MODIFYSIGNID		0x004000	//�޸ľ��ű�ʶ
/****************************ͨѶЭ�鶨��********************************/
//����ְλ��
enum Official_Type
{
	NORMAL_JOB,				//��ͨ
	CHARIRMAN_JOB,			//���ų�
	OFFICIAL_JOB,			//��Ա
	LEAGUER_JOB				//��Ա
};
enum Vote_Type
{
	NONUSER_VOTE,			//��Ȩ
	UPHOLD_VOTE,			//�޳�Ʊ
	OPPOSE_VOTE,			//����Ʊ
	DELATE_VOTE				//����(ǩ��)
};
enum Postion_Type
{
	NORMAL_POSTION,			//��
	MILITARY_POSTION,		//����ίԱ��
	ECONOMY_POSTION,		//����ίԱ��
	CONTRIBUTE_POSTION,		//����ίԱ��
	MINISTER_POSTION,		//����
	UNDERSECRETARY_POSTION	//������
};

enum Dept_Type
{
	BOULE_DEPT,				//����Ժ
	DOD_DEPT,				//������
	INTERIOR_DEPT			//������
};

enum State_Type
{
	NORMAL_STATE = 0x0,			//
	VOTE_STATE = 0x01,
	UNDERWRITE_STATE = 0x02,
	DELATE_STATE = 0x04
};
enum DeptState_Type
{
	NORMAL_DEPTSTATE = 0x0,//û�й�����������������
	DODMINISTER_DEPTSTATE = 0x01,//�й�������
	INTERIOR_DEPTSTATE = 0x02   //����������
};
//���ųƺ�����
enum LegionTitle_Type
{
	LOVE_LEGIONTITLE,			//����
	EXP_LEGIONTITLE,			//����
};
typedef struct tagCareerConfig_Item
{
	int	DeptID;			//���ţ�Dept_Type
	int JobID;
	char JobName[SIZE_32];
}CareerConfig_Item,*LPCareerConfig_Item;

//���ᴴ��CC_C_CREATE_CONSORTIA/CC_S_CREATE_CONSORTIA
typedef struct tagCreate_Consortia_Request:DISTRIBUTED_HEADER
{
	char	ConName[SIZE_32];					//��������
	char	Placard[SIZE_256];					//���ṫ��
	char	Tenet[SIZE_128];					//������ּ
	int		FaceID;								//LogoID,��
	char	LogoBuf[MAX_LOGO_SIZE];
	inline void serialize(int playerid);
	inline void serialize(int playerid,const char *name,const char* placard,const char *tenet,int faceid,const char *buf =0);
}Create_Consortia_Request,*LPCreate_Consortia_Request;
inline void tagCreate_Consortia_Request::serialize(int playerid)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,MDM_GP_MAP_CONSORTIA,CC_C_CREATE_CONSORTIA);
}
inline void tagCreate_Consortia_Request::serialize(int playerid,const char *name,const char* placard,const char *tenet,int faceid,const char *buf)
{
	if(name)
	{
		strncpy(ConName,name,SIZE_32);
	}
	if(placard)
	{
		strncpy(Placard,placard,SIZE_256);
	}
	if(tenet)
	{
		strncpy(Tenet,tenet,SIZE_128);
	}
	FaceID = faceid;
	if(FaceID ==0)
	{//�Զ���
		if(buf)
		{
			memcpy(LogoBuf,buf,MAX_LOGO_SIZE);
		}
		DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,MDM_GP_MAP_CONSORTIA,CC_C_CREATE_CONSORTIA);
	}
	else
	{
		DISTRIBUTED_HEADER::serialize(sizeof(*this) - MAX_LOGO_SIZE,0,playerid,MDM_GP_MAP_CONSORTIA,CC_C_CREATE_CONSORTIA);
	}
}

typedef struct tagCreate_Consortia_Response:DISTRIBUTED_HEADER
{
    int		Error_Code;						//���������0���ɹ���1:������2���ȼ�������3����Դ���㣻������δ֪����
    int		ConsortiaID;					//�����Ĺ���ID��
	inline void serialize(int agent,int playerid,int code,int consortiaid);
}Create_Consortia_Response,*LPCreate_Consortia_Response;
inline void tagCreate_Consortia_Response::serialize(int agent,int playerid,int code,int consortiaid)
{
	Error_Code = code;						
	ConsortiaID = consortiaid;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CONSORTIA,playerid,CC_S_CREATE_CONSORTIA);
}

//�����ѯCC_C_QUERY_CONSORTIA/CC_S_QUERY_CONSORTIA
typedef struct tagQuery_Consortia_Request:DISTRIBUTED_HEADER
{
	int		Page;					//ҳ����0����һҳ
	inline void serialize(int playerid,int page);
}Query_Consortial_Request,*LPQuery_Consortial_Request;
inline void tagQuery_Consortia_Request::serialize(int playerid,int page)
{
	Page = page;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,MDM_GP_MAP_CONSORTIA,CC_C_QUERY_CONSORTIA);
}

typedef struct tagConsortia_Info
{
	int		ConsortiaID;					//������
	char	ConName[SIZE_32];				//��������
	int		Level;							//����ȼ�
	int		Count;							//��Ա������
	int		FaceID;							//����LOGO
	int		ChairmanID;						//�᳤ID
	char	ChairmanName[SIZE_32];			//�᳤����
	char	Tenet[SIZE_128];				//��ּ
	char	IsUnion;						//�Ƿ�ͬ�˹���
	char	IsFight;						//�Ƿ�Լս����

	//20100301
	char	IsRoyal;						//�Ƿ�Ϊ�ʼҾ��ţ�0����1����
	int		MaxUserNum;						//��������
	int		Money;							//��Ǯ
	int		Exp;							//����
	int		Credit;							//��������
	int		SignID;							//���ű�־-�ʼҾ��Ų���

	int     countryID;                      // ��ӪID
}Consortia_Info,*LPConsortia_Info;
typedef struct tagQuery_Consortia_Response:DISTRIBUTED_HEADER
{
	int		Count;							//��������,0���޹��᣻<0���д����������й�����Ϣʱ���ýṹ�����Count��Consortia_Info
	int		Page;							//��ǰҳ
	int		PageCount;						//��ҳ��
	inline void serialize(int length,int agent,int playerid,int count,int page,int pagecount);
}Query_Consortia_Response,*LPQuery_Consortia_Response;
inline void tagQuery_Consortia_Response::serialize(int length,int agent,int playerid,int count,int page,int pagecount)
{
	Count = count;
	Page = page;
	PageCount = pagecount;
	DISTRIBUTED_HEADER::serialize(length,agent,MDM_GP_MAP_CONSORTIA,playerid,CC_S_QUERY_CONSORTIA);
}

//�����ɢCC_C_DELETE_CONSORTIA/CC_S_DELETE_CONSORTIA
typedef struct tagDelete_Consortia_Request:DISTRIBUTED_HEADER
{
	inline void serialize(int playerid);
}Delete_Consortia_Request,*LPDelete_Consortia_Request;
inline void tagDelete_Consortia_Request::serialize(int playerid)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,MDM_GP_MAP_CONSORTIA,CC_C_DELETE_CONSORTIA);
}

typedef struct tagDelete_Consortia_Response:DISTRIBUTED_HEADER
{
	int Error_Code;					//���������0���ɹ���1��û��Ȩ�ޣ�2�������ڣ�������ʧ��
	inline void serialize(int agent,int playerid,int code);
}Delete_Consortia_Response,*LPDelete_Consortia_Response;
inline void tagDelete_Consortia_Response::serialize(int agent,int playerid,int code)
{
	Error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CONSORTIA,playerid,CC_S_DELETE_CONSORTIA);
}

//��ѯ�����������CC_C_QUERY_USER_CON/CC_S_QUERY_USER_CON
typedef struct tagQuery_User_Con_Request:DISTRIBUTED_HEADER
{
	inline void serialize(int playerid);
}Query_User_Con_Request,*LPQuery_User_Con_Request;
inline void tagQuery_User_Con_Request::serialize(int playerid)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,MDM_GP_MAP_CONSORTIA,CC_C_QUERY_USER_CON);
}

typedef struct tagUserConInfo
{
	tagConsortia_Info conInfo;				//���������Ϣ
	char	Placard[SIZE_256];				//���ṫ��
	int		ConJob;							//ְλ���
	int		Purview;						//Ȩ��
	//add 20100301
	int		LegionTitle;					//���ųƺ�
	int		Contribute;						//���׶�
	int		Credit;							//������
	int		Exp;							//����ֵ
	int		Money;							//���׵Ľ�Ǯ��
	char	isCanSalaried;					//�Ƿ������ȡ����,0:�����ԣ�1������
}UserConInfo,*LPUserConInfo;

typedef struct tagQuery_User_Con_Response:DISTRIBUTED_HEADER
{
	int			Error_Code;					//���������0���ɹ���1�����δ���빫�᣻<0:�д�����
	UserConInfo userConInfo;
	inline void serialize(int agent,int playerid,int code);
}Query_User_Con_Response,*LPQuery_User_Con_Response;
inline void tagQuery_User_Con_Response::serialize(int agent,int playerid,int code)
{
	Error_Code = code;
	if (Error_Code == 0)
	{
		DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CONSORTIA,playerid,CC_S_QUERY_USER_CON);
	}
	else
	{
		DISTRIBUTED_HEADER::serialize(sizeof(*this) - sizeof(UserConInfo),agent,MDM_GP_MAP_CONSORTIA,playerid,CC_S_QUERY_USER_CON);
	}
}

//�޸Ĺ���CC_C_MODIFY_PLACARD/CC_S_MODIFY_PLACARD
typedef struct tagModify_Con_Placard_Request:DISTRIBUTED_HEADER
{
	char		Placard[SIZE_256];			//��������
	inline void serialize(int playerid,const char *placard = 0);
}Modify_Con_Placard_Request,*LPModify_Con_Placard_Request;
inline void tagModify_Con_Placard_Request::serialize(int playerid,const char *placard)
{
	if(placard)
	{
		strncpy(Placard,placard,SIZE_256);
	}
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,MDM_GP_MAP_CONSORTIA,CC_C_MODIFY_PLACARD);
}

typedef struct tagModify_Con_Placard_Response:DISTRIBUTED_HEADER
{
	int		Error_Code;							//���������0���ɹ���1:δ���빫�ᣬ������ʧ��;
	inline void serialize(int agent,int playerid,int code);
}Modify_Con_Placard_Response,*LPModify_Con_Placard_Response;
inline void tagModify_Con_Placard_Response::serialize(int agent,int playerid,int code)
{
	Error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CONSORTIA,playerid,CC_S_MODIFY_PLACARD);
}

//��ѯLOGO:CC_C_QUERY_LOGO/CC_S_QUERY_LOGO
typedef struct tagQuery_Con_LOGO_Request:DISTRIBUTED_HEADER
{
	int		ConsortiaID;					//����ID
	inline void serialize(int playerid,int consortiaid);
}Query_Con_LOGO_Request,*LPQuery_Con_LOGO_Request;
inline void tagQuery_Con_LOGO_Request::serialize(int playerid,int consortiaid)
{
	ConsortiaID = consortiaid;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,MDM_GP_MAP_CONSORTIA,CC_C_QUERY_LOGO);
}

typedef struct	tagQuery_Con_LOGO_Response:DISTRIBUTED_HEADER
{
	int		Error_Code;						//��ѯ�����0���ɹ���
	int		FaceID;							//LOGO���
	char	FaceBuf[MAX_LOGO_SIZE];			//LOGO����
	inline void serialize(int agent,int playerid,int code);
}Query_Con_LOGO_Response,*LPQuery_Con_LOGO_Response;
inline void tagQuery_Con_LOGO_Response::serialize(int agent,int playerid,int code)
{
	Error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CONSORTIA,playerid,CC_S_QUERY_LOGO);
}

//�޸�LOGO��CC_C_MODIFY_LOGO/CC_S_MODIFY_LOGO
typedef struct tagModify_Con_LOGO_Request:DISTRIBUTED_HEADER
{
	int		FaceID;							//LOGO���
	char	FaceBuf[MAX_LOGO_SIZE];			//LOGO����
	inline void serialize(int playerid);
	inline void serialize(int playerid,int faceid,const char *buffer = 0);
}Modify_Con_LOGO_Request,*LPModify_Con_LOGO_Request;
inline void tagModify_Con_LOGO_Request::serialize(int playerid)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,MDM_GP_MAP_CONSORTIA,CC_C_MODIFY_LOGO);
}
inline void tagModify_Con_LOGO_Request::serialize(int playerid,int faceid,const char *buffer)
{
	FaceID = faceid;
	if(FaceID ==0)
	{//�Զ���
		if(buffer)
		{
			memcpy(FaceBuf,buffer,MAX_LOGO_SIZE);
		}
		DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,MDM_GP_MAP_CONSORTIA,CC_C_MODIFY_LOGO);
	}
	else
	{
		DISTRIBUTED_HEADER::serialize(sizeof(*this) - MAX_LOGO_SIZE,0,playerid,MDM_GP_MAP_CONSORTIA,CC_C_MODIFY_LOGO);
	}
}

typedef struct tagModify_Con_LOGO_Response:DISTRIBUTED_HEADER
{
	int		Error_Code;						//���������0���ɹ���������ʧ�ܣ�
	inline void serialize(int agent,int playerid,int code);
}Modify_Con_LOGO_Response,*LPModify_Con_LOGO_Response;
inline void tagModify_Con_LOGO_Response::serialize(int agent,int playerid,int code)
{
	Error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CONSORTIA,playerid,CC_S_MODIFY_LOGO);
}

//������Դ����������CC_C_CONTRIBUTE_BANK/CC_S_CONTRIBUTE_BANK
typedef struct tagContribute_Con_Bank_Request:DISTRIBUTED_HEADER
{
	int		ItemID;						//������ԴID;
	int		ItemCount;					//����
	inline void serialize(int playerid,int itemid,int count);
}Contribute_Con_Bank_Request,*LPContribute_Con_Bank_Request;
inline void tagContribute_Con_Bank_Request::serialize(int playerid,int itemid,int count)
{
	ItemID = itemid;
	ItemCount = count;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,MDM_GP_MAP_CONSORTIA,CC_C_CONTRIBUTE_BANK);
}

typedef struct tagContribute_Con_Bank_Response:DISTRIBUTED_HEADER
{
	int		Error_Code;					//���������0���ɹ���������ʧ�ܣ�
	inline void serialize(int agent,int playerid,int code);
}Contribute_Con_Bank_Response,*LPContribute_Con_Bank_Response;
inline void tagContribute_Con_Bank_Response::serialize(int agent,int playerid,int code)
{
	Error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CONSORTIA,playerid,CC_S_CONTRIBUTE_BANK);
}

//֧��������ԴCC_C_PAYOUT_BANK/CC_S_PAYOUT_BANK(�ݲ�ʵ��)
typedef struct tagPayout_Con_Bank_Request:DISTRIBUTED_HEADER
{
	int		ItemID;						//������ԴID;
	int		ItemCount;					//����
	inline void serialize(int playerid,int itemid,int count);
}Payout_Con_Bank_Request,*LPPayout_Con_Bank_Request;
inline void tagPayout_Con_Bank_Request::serialize(int playerid,int itemid,int count)
{
	ItemID = itemid;
	ItemCount = count;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,MDM_GP_MAP_CONSORTIA,CC_C_PAYOUT_BANK);
}

typedef struct tagPayout_Con_Bank_Response:DISTRIBUTED_HEADER
{
	int		Error_Code;					//���������0���ɹ���������ʧ��
	inline void serialize(int agent,int playerid,int code);
}Payout_Con_Bank_Response,*LPPayout_Con_Bank_Response;
inline void tagPayout_Con_Bank_Response::serialize(int agent,int playerid,int code)
{
	Error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CONSORTIA,playerid,CC_S_PAYOUT_BANK);
}

//�������в�ѯCC_C_QUERY_BANK/CC_S_QUERY_BANK
typedef struct tagQuery_Con_Bank_Request:DISTRIBUTED_HEADER
{
	int		Page;						//ҳ����0����һҳ
	inline void serialize(int playerid,int page);
}Query_Con_Bank_Request,*LPQuery_Con_Bank_Request;
inline void tagQuery_Con_Bank_Request::serialize(int playerid,int page)
{
	Page = page;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,MDM_GP_MAP_CONSORTIA,CC_C_QUERY_BANK);
}

typedef struct tagBank_ItemInfo
{
	int		ItemID;
	int		ItemCount;
}Bank_ItemInfo,*LPBank_ItemInfo;

typedef struct tagQuery_Con_Bank_Response:DISTRIBUTED_HEADER
{
	int		Count;						//���ؼ�¼��,�ýṹ���������Count��Bank_ItemInfo��0������Դ��Ϣ��<0:�д�����
	int		Page;						//��ǰҳ
	int		PageCount;					//��ҳ��
	inline void serialize(int length,int agent,int playerid,int count,int page,int pagecount);
}Query_Con_Bank_Response,*LPQuery_Con_Bank_Response;
inline void tagQuery_Con_Bank_Response::serialize(int length,int agent,int playerid,int count,int page,int pagecount)
{
	Count = count;
	Page = page;
	PageCount = pagecount;
	DISTRIBUTED_HEADER::serialize(length,agent,MDM_GP_MAP_CONSORTIA,playerid,CC_S_QUERY_BANK);
}

//��������CC_C_ALLY_APPLY_CON/CC_S_ALLY_APPLY_CON
typedef struct tagAlly_Apply_Con_Request:DISTRIBUTED_HEADER
{
	int		AllyConsortiaID;			//Ԥ���˵Ĺ���ID
	inline void serialize(int playerid,int allyconsortiaid);
}Ally_Apply_Con_Request,*LPAlly_Apply_Con_Request;
inline void tagAlly_Apply_Con_Request::serialize(int playerid,int allyconsortiaid)
{
	AllyConsortiaID = allyconsortiaid;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,MDM_GP_MAP_CONSORTIA,CC_C_ALLY_APPLY_CON);
}

typedef struct tagAlly_Apply_Con_Response:DISTRIBUTED_HEADER
{
	int		Error_Code;					//���������0���ɹ���1����Ҳ��ǻ᳤��2:�Է����᲻���ڣ�3:�������޷����ˣ�������ʧ��
	inline void serialize(int agent,int playerid,int code);
}Ally_Apply_Con_Response,*LPAlly_Apply_Con_Response;
inline void tagAlly_Apply_Con_Response::serialize(int agent,int playerid,int code)
{
	Error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CONSORTIA,playerid,CC_S_ALLY_APPLY_CON);
}

//ͬ�����CC_C_ALLY_CONSORTIA_AGREE/CC_S_ALLY_CONSORTIA_AGREE
typedef struct tagAlly_Consortia_Agree_Request:DISTRIBUTED_HEADER
{
	int		ConsortiaID;				//����ID
	int		ApplyPlayerID;				//������ID
	inline void serialize(int playerid,int consortiaid,int applyid);
}Ally_Consortia_Agree_Request,*LPAlly_Consortia_Agree_Request;
inline void tagAlly_Consortia_Agree_Request::serialize(int playerid,int consortiaid,int applyid)
{
	ConsortiaID = consortiaid;
	ApplyPlayerID = applyid;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,MDM_GP_MAP_CONSORTIA,CC_C_ALLY_CONSORTIA_AGREE);
}

typedef struct tagAlly_Consortia_Agree_Response:DISTRIBUTED_HEADER
{
	int Error_Code;						//���������0���ɹ���1:���ǻ᳤;2:����ͬ�ˣ�3��ͬһ���᣻������ʧ��
	inline void serialize(int agent,int playerid,int code);
}Ally_Consortia_Agree_Response,*LPAlly_Consortia_Agree_Response;
inline void tagAlly_Consortia_Agree_Response::serialize(int agent,int playerid,int code)
{
	Error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CONSORTIA,playerid,CC_S_ALLY_CONSORTIA_AGREE);
}

//�ܾ�����CC_C_ALLY_CONSORTIA_REFUSE/CC_S_ALLY_CONSORTIA_REFUSE
typedef struct tagAlly_Consortia_Refuse_Request:DISTRIBUTED_HEADER
{
	int		ConsortiaID;				//����ID
	int		ApplyPlayerID;				//������ID
	inline void serialize(int playerid,int consortiaid,int applyid);
}Ally_Consortia_Refuse_Request,*LPAlly_Consortia_Refuse_Request;
inline void tagAlly_Consortia_Refuse_Request::serialize(int playerid,int consortiaid,int applyid)
{
	ConsortiaID = consortiaid;
	ApplyPlayerID = applyid;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,MDM_GP_MAP_CONSORTIA,CC_C_ALLY_CONSORTIA_REFUSE);
}

typedef struct tagAlly_Consortia_Refuse_Response:DISTRIBUTED_HEADER
{
	int Error_Code;						//���������0���ɹ���������ʧ��
	inline void serialize(int agent,int playerid,int code);
}Ally_Consortia_Refuse_Response,*LPAlly_Consortia_Refuse_Response;
inline void tagAlly_Consortia_Refuse_Response::serialize(int agent,int playerid,int code)
{
	Error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CONSORTIA,playerid,CC_S_ALLY_CONSORTIA_REFUSE);
}

//����CC_C_UNALLY_CONSORTIA/CC_S_UNALLY_CONSORTIA
typedef struct tagUnally_Consortia_Request:DISTRIBUTED_HEADER
{
	int		ConsortiaID;				//����ID
	inline void serialize(int playerid,int consortiaid);
}Unally_Consortia_Request,*LPUnally_Consortia_Request;
inline void tagUnally_Consortia_Request::serialize(int playerid,int consortiaid)
{
	ConsortiaID = consortiaid;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,MDM_GP_MAP_CONSORTIA,CC_C_UNALLY_CONSORTIA);
}

typedef struct tagUnally_Consortia_Response:DISTRIBUTED_HEADER
{
	int Error_Code;						//���������0���ɹ���1:û��Ȩ�ޣ�2������ͬ�˹��᣻������ʧ��
	inline void serialize(int agent,int playerid,int code);
}Unally_Consortia_Response,*LPUnally_Consortia_Response;
inline void tagUnally_Consortia_Response::serialize(int agent,int playerid,int code)
{
	Error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CONSORTIA,playerid,CC_S_UNALLY_CONSORTIA);
}

//Լս����CC_C_FIGHT_APPLY_CON/CC_S_FIGHT_APPLY_CON	
typedef struct tagFight_Apply_Con_Request:DISTRIBUTED_HEADER
{
	int		AllyConsortiaID;			//ԤԼս�Ĺ���ID
	int		MapID;						//Լս��
	int		CityID;						//Լս����
	__int64 Time;						//Լսʱ��
	inline void serialize(int playerid,int allyconsortiaid,int mapid,int cityid,__int64 time);
}Fight_Apply_Con_Request,*LPFight_Apply_Con_Request;
inline void tagFight_Apply_Con_Request::serialize(int playerid,int allyconsortiaid,int mapid,int cityid,__int64 time)
{
	AllyConsortiaID = allyconsortiaid;
	MapID = mapid;
	CityID = cityid;
	Time = time;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,MDM_GP_MAP_CONSORTIA,CC_C_FIGHT_APPLY_CON);
}

typedef struct tagFight_Apply_Con_Response:DISTRIBUTED_HEADER
{
	int		Error_Code;					//���������0���ɹ���������ʧ��
	inline void serialize(int agent,int playerid,int code);
}Fight_Apply_Con_Response,*LPFight_Apply_Con_Response;
inline void tagFight_Apply_Con_Response::serialize(int agent,int playerid,int code)
{
	Error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CONSORTIA,playerid,CC_S_FIGHT_APPLY_CON);
}

//ͬ��ԼսCC_C_FIGHT_CONSORTIA_AGREE/CC_S_FIGHT_CONSORTIA_AGREE
typedef struct tagFight_Consortia_Agree_Request:DISTRIBUTED_HEADER
{
	int		ConsortiaID;				//����ID
	int		ApplyPlayerID;				//������
	int		MapID;						//Լս��
	int		CityID;						//Լս����
	__int64 Time;						//Լսʱ��
	inline void serialize(int playerid,int consortiaid,int applyid,int mapid,int cityid,__int64 time);
}Fight_Consortia_Agree_Request,*LPFight_Consortia_Agree_Request;
inline void tagFight_Consortia_Agree_Request::serialize(int playerid,int consortiaid,int applyid,int mapid,int cityid,__int64 time)
{
	ConsortiaID = consortiaid;
	ApplyPlayerID = applyid;
	MapID = mapid;
	CityID = cityid;
	Time = time;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,MDM_GP_MAP_CONSORTIA,CC_C_FIGHT_CONSORTIA_AGREE);
}

typedef struct tagFight_Consortia_Agree_Response:DISTRIBUTED_HEADER
{
	int Error_Code;						//���������0���ɹ���������ʧ��
	inline void serialize(int agent,int playerid,int code);
}Fight_Consortia_Agree_Response,*LPFight_Consortia_Agree_Response;
inline void tagFight_Consortia_Agree_Response::serialize(int agent,int playerid,int code)
{
	Error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CONSORTIA,playerid,CC_S_FIGHT_CONSORTIA_AGREE);
}

//�ܾ�ԼսCC_C_FIGHT_CONSORTIA_REFUSE/CC_S_FIGHT_CONSORTIA_REFUSE
typedef struct tagFight_Consortia_Refuse_Request:DISTRIBUTED_HEADER
{
	int		ConsortiaID;				//����ID
	int		ApplyPlayerID;				//������
	int		MapID;						//Լս��
	int		CityID;						//Լս����
	__int64 Time;						//Լսʱ��
	inline void serialize(int playerid,int consortiaid,int applyid,int mapid,int cityid,__int64 time);
}Fight_Consortia_Refuse_Request,*LPFight_Consortia_Refuse_Request;
inline void tagFight_Consortia_Refuse_Request::serialize(int playerid,int consortiaid,int applyid,int mapid,int cityid,__int64 time)
{
	ConsortiaID = consortiaid;
	ApplyPlayerID = applyid;
	MapID = mapid;
	CityID = cityid;
	Time = time;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,MDM_GP_MAP_CONSORTIA,CC_C_FIGHT_CONSORTIA_REFUSE);
}

typedef struct tagFight_Consortia_Refuse_Response:DISTRIBUTED_HEADER
{
	int Error_Code;						//���������0���ɹ���������ʧ��
	inline void serialize(int agent,int playerid,int code);
}Fight_Consortia_Refuse_Response,*LPFight_Consortia_Refuse_Response;
inline void tagFight_Consortia_Refuse_Response::serialize(int agent,int playerid,int code)
{
	Error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CONSORTIA,playerid,CC_S_FIGHT_CONSORTIA_REFUSE);
}
//Լս��ѯCC_C_QUERY_FIGHT/CC_S_QUERY_FIGHT
typedef struct tagQuery_Con_Fight_Request:DISTRIBUTED_HEADER
{
	int		Page;						//��ѯҳ��
	inline void serialize(int playerid,int page);
}Query_Con_Fight_Request,*LPQuery_Con_Fight_Request;
inline void tagQuery_Con_Fight_Request::serialize(int playerid,int page)
{
	Page = page;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,MDM_GP_MAP_CONSORTIA,CC_C_QUERY_FIGHT);
}

typedef struct tagCon_FightInfo
{
	int		FightID;				//ԼսID
	int		Consortia1ID;			//
	char	Consortia1Name[SIZE_32];//		
	int		Consortia2ID;			//
	char	Consortia2Name[SIZE_32];//		
	int		MapID;					//��ս�Ŀ�ID  ��ս�ص�
	int		CityID;					//��ս�ĳ���ID
	int		Time;					//��սʱ��
}Con_FightInfo,*LPCon_FightInfo;

typedef struct tagQuery_Con_Fight_Response:DISTRIBUTED_HEADER
{
	int		Count;					//Լս��¼�������ýṹ�����Count��Con_FightInfo�ṹ�壬0����Լս��Ϣ��<0���д�����
	int		Page;
	int		PageCount;				//��ҳ��	
	inline void serialize(int length,int agent,int playerid,int count,int page,int pagecount);
}Query_Con_Fight_Response,*LPQuery_Con_Fight_Response;
inline void tagQuery_Con_Fight_Response::serialize(int length,int agent,int playerid,int count,int page,int pagecount)
{
	Count = count;
	Page = page;
	PageCount = pagecount;
	DISTRIBUTED_HEADER::serialize(length,agent,MDM_GP_MAP_CONSORTIA,playerid,CC_S_QUERY_FIGHT);
}

//������빫��CC_C_JOIN_APPLY_CON/CC_S_JOIN_APPLY_CON
typedef struct tagJoin_Apply_Con_Request:DISTRIBUTED_HEADER
{
	int		ConsortiaID;			//Ԥ����Ĺ���ID
	inline void serialize(int playerid,int consortiaid);
}Join_Apply_Con_Request,*LPJoin_Apply_Con_Request;
inline void tagJoin_Apply_Con_Request::serialize(int playerid,int consortiaid)
{
	ConsortiaID = consortiaid;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,MDM_GP_MAP_CONSORTIA,CC_C_JOIN_APPLY_CON);
}

typedef struct tagJoin_Apply_Con_Response:DISTRIBUTED_HEADER
{
	int		Error_Code;				//���������0���ɹ���1�ȼ�������2��������ͬһ��Ӫ���޷����룻8:����������������ʧ��
	inline void serialize(int agent,int playerid,int code);
}Join_Apply_Con_Response,*LPJoin_Apply_Con_Response;
inline void tagJoin_Apply_Con_Response::serialize(int agent,int playerid,int code)
{
	Error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CONSORTIA,playerid,CC_S_JOIN_APPLY_CON);
}

//�᳤ͬ����빤��CC_C_JOIN_CONSORTIA_AGREE/CC_S_JOIN_CONSORTIA_AGREE
typedef struct tagJoin_Consortia_Agree_Request:DISTRIBUTED_HEADER
{
	int		PlayerID;					//�����빫������ID
	inline void serialize(int playerid,int joinplayer);
}Join_Consortia_Agree_Request,*LPJoin_Consortia_Agree_Request;
inline void tagJoin_Consortia_Agree_Request::serialize(int playerid,int joinplayer)
{
	PlayerID = joinplayer;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,MDM_GP_MAP_CONSORTIA,CC_C_JOIN_CONSORTIA_AGREE);
}

typedef struct tagJoin_Consortia_Agree_Response:DISTRIBUTED_HEADER
{
	int		Error_Code;					//���������0���ɹ���1,�Ѽ��빫�᣻2:û��Ȩ�ޣ�8:����������������ʧ��
	inline void serialize(int agent,int playerid,int code);
}Join_Consortia_Agree_Response,*LPJoin_Consortia_Agree_Response;
inline void tagJoin_Consortia_Agree_Response::serialize(int agent,int playerid,int code)
{
	Error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CONSORTIA,playerid,CC_S_JOIN_CONSORTIA_AGREE);
}

//�᳤�ܾ����빤��CC_C_JOIN_CONSORTIA_REFUSE/CC_S_JOIN_CONSORTIA_REFUSE
typedef struct tagJoin_Consortia_Refuse_Request:DISTRIBUTED_HEADER
{
	int		PlayerID;					//�����빫������ID
	inline void serialize(int playerid,int joinplayer);
}Join_Consortia_Refuse_Request,*LPJoin_Consortia_Refuse_Request;
inline void tagJoin_Consortia_Refuse_Request::serialize(int playerid,int joinplayer)
{
	PlayerID = joinplayer;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,MDM_GP_MAP_CONSORTIA,CC_C_JOIN_CONSORTIA_REFUSE);
}

typedef struct tagJoin_Consortia_Refuse_Response:DISTRIBUTED_HEADER
{
	int		Error_Code;					//���������0���ɹ���������ʧ��
	inline void serialize(int agent,int playerid,int code);
}Join_Consortia_Refuse_Response,*LPJoin_Consortia_Refuse_Response;
inline void tagJoin_Consortia_Refuse_Response::serialize(int agent,int playerid,int code)
{
	Error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CONSORTIA,playerid,CC_S_JOIN_CONSORTIA_REFUSE);
}
//�����Ա��ѯCC_C_QUERY_CONUSER/CC_S_QUERY_CONUSER
typedef struct tagQuery_Con_User_Request:DISTRIBUTED_HEADER
{
	int		Page;						//ҳ��
	inline void serialize(int playerid,int page);
}Query_Con_User_Request,*LPQuery_Con_User_Request;
inline void tagQuery_Con_User_Request::serialize(int playerid,int page)
{
	Page = page;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,MDM_GP_MAP_CONSORTIA,CC_C_QUERY_CONUSER);
}
//ID�����ơ��ȼ������塢ְϵ������ֵ�����Ρ������㡢���׶ȡ�����ְλ����Ա����
typedef struct tagCon_UserInfo
{
	int		PlayerID;
	char	Name[SIZE_24];
	int		Level;						//����
	int		OfficerRace;				//����
	int		JobID;						//ְϵ
	int		Exp;						//����ֵ
	int		MilitaryRank;				//����
	int		Credit;						//������
	int		Contribute;					//���׶�
	int		ConJob;						//����ְλ����
	char	IsOnline;					//�Ƿ�����
	int		LegionTitle;				//���ųƺ�
	int		Purview;					//Ȩ��
}Con_UserInfo,*LPCon_UserInfo;

typedef struct tagQuery_Con_User_Response:DISTRIBUTED_HEADER
{
	int		Count;						//��Ա�������ýṹ������Count��Con_UserInfo��0���޳�Ա��<0���д�����(-1:û�м��빫�ᣩ
	int		Page;						//��ǰҳ
	int		PageCount;					//��ҳ��
	inline void serialize(int length,int agent,int playerid,int count,int page,int pagecount);
}Query_Con_User_Response,*LPQuery_Con_User_Response;
inline void tagQuery_Con_User_Response::serialize(int length,int agent,int playerid,int count,int page,int pagecount)
{
	Count = count;
	Page = page;
	PageCount = pagecount;
	DISTRIBUTED_HEADER::serialize(length,agent,MDM_GP_MAP_CONSORTIA,playerid,CC_S_QUERY_CONUSER);
}

//Kick��ԱCC_C_KICK_CONUSER/CC_S_KICK_CONUSER
typedef struct Kick_ConUser_Request:DISTRIBUTED_HEADER
{
	int		KickPlayerID;				//��Kick���ID
	inline void serialize(int playerid,int kickid);
}Kick_ConUser_Request,*LPKick_ConUser_Request;
inline void Kick_ConUser_Request::serialize(int playerid,int kickid)
{
	KickPlayerID = kickid;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,MDM_GP_MAP_CONSORTIA,CC_C_KICK_CONUSER);
}

typedef struct Kick_ConUser_Response:DISTRIBUTED_HEADER
{
	int		Error_Code;					//���������0���ɹ���1:û��Ȩ��;2:�����ڸó�Ա��3���޷�Kick�᳤��������ʧ��
	inline void serialize(int agent,int playerid,int code);
}Kick_ConUser_Response,*LPKick_ConUser_Response;
inline void Kick_ConUser_Response::serialize(int agent,int playerid,int code)
{
	Error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CONSORTIA,playerid,CC_S_KICK_CONUSER);
}

//�˳�����CC_C_QUIT_CONSORTIA/CC_S_QUIT_CONSORTIA
typedef struct tagQuit_Consortia_Request:DISTRIBUTED_HEADER
{
	inline void serialize(int playerid);
}Quit_Consortia_Request,*LPQuit_Consortia_Request;
inline void tagQuit_Consortia_Request::serialize(int playerid)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,MDM_GP_MAP_CONSORTIA,CC_C_QUIT_CONSORTIA);
}

typedef struct tagQuit_Consortia_Response:DISTRIBUTED_HEADER
{
	int		Error_Code;					//���������0���ɹ���1:�᳤�޷��˳���������ʧ��
	inline void serialize(int agent,int playerid,int code);
}Quit_Consortia_Response,*LPQuit_Consortia_Response;
inline void tagQuit_Consortia_Response::serialize(int agent,int playerid,int code)
{
	Error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CONSORTIA,playerid,CC_S_QUIT_CONSORTIA);
}

//������빫��CC_C_INVITE_JOIN_CON/CC_S_INVITE_JOIN_CON
typedef struct tagInvite_JoinCon_Request:DISTRIBUTED_HEADER
{
	char	InvitedName[SIZE_32];			//��������
	inline void serialize(int playerid,const char *name);
}Invite_JoinCon_Request,*LPInvite_JoinCon_Request;
inline void tagInvite_JoinCon_Request::serialize(int playerid,const char *name)
{
	if(name)
	{
		strncpy(InvitedName,name,SIZE_32);
	}
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,MDM_GP_MAP_CONSORTIA,CC_C_INVITE_JOIN_CON);
}

typedef struct tagInvite_JoinCon_Response:DISTRIBUTED_HEADER
{
	int		Error_Code;						//���������0���ɹ���1:���������Ѽ��빫�ᣬ2��������ͬһ��Ӫ��3:û�������˵�Ȩ�ޣ�4:�������˼��𲻹���8:����������������ʧ��
	inline void serialize(int agent,int playerid,int code);
}Invite_JoinCon_Response,*LPInvite_JoinCon_Response;
inline void tagInvite_JoinCon_Response::serialize(int agent,int playerid,int code)
{
	Error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CONSORTIA,playerid,CC_S_INVITE_JOIN_CON);
}
//ͬ��������빤��CC_C_INVITE_JOIN_AGREE/CC_S_INVITE_JOIN_AGREE	
typedef struct tagInvite_JoinCon_Agree_Request:DISTRIBUTED_HEADER
{
	int		ConsortiaID;				//����ID
	int		InviteID;					//������
	inline void serialize(int playerid,int consortiaid,int inviteid);
}Invite_JoinCon_Agree_Request,*LPInvite_JoinCon_Agree_Request;
inline void tagInvite_JoinCon_Agree_Request::serialize(int playerid,int consortiaid,int inviteid)
{
	ConsortiaID = consortiaid;
	InviteID = inviteid;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,MDM_GP_MAP_CONSORTIA,CC_C_INVITE_JOIN_AGREE);
}

typedef struct tagInvite_JoinCon_Agree_Response:DISTRIBUTED_HEADER
{
	int		Error_Code;					//���������0���ɹ���1:�Ѽ��빫�᣻2:�ȼ�������8:����������������ʧ��
	inline void serialize(int agent,int playerid,int code);
}Invite_JoinCon_Agree_Response,*LPInvite_JoinCon_Agree_Response;
inline void tagInvite_JoinCon_Agree_Response::serialize(int agent,int playerid,int code)
{
	Error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CONSORTIA,playerid,CC_S_INVITE_JOIN_AGREE);
}

//�ܾ�������빤��CC_C_INVITE_JOIN_REFUSE/CC_S_INVITE_JOIN_REFUSE
typedef struct tagInvite_JoinCon_Refuse_Request:DISTRIBUTED_HEADER
{
	int		InviteID;					//������
	inline void serialize(int playerid,int inviteid);
}Invite_JoinCon_Refuse_Request,*LPInvite_JoinCon_Refuse_Request;
inline void tagInvite_JoinCon_Refuse_Request::serialize(int playerid,int inviteid)
{
	InviteID = inviteid;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,MDM_GP_MAP_CONSORTIA,CC_C_INVITE_JOIN_REFUSE);
}

typedef struct tagInvite_JoinCon_Refuse_Response:DISTRIBUTED_HEADER
{
	int		Error_Code;					//���������0���ɹ���������ʧ��
	inline void serialize(int agent,int playerid,int code);
}Invite_JoinCon_Refuse_Response,*LPInvite_JoinCon_Refuse_Response;
inline void tagInvite_JoinCon_Refuse_Response::serialize(int agent,int playerid,int code)
{
	Error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CONSORTIA,playerid,CC_S_INVITE_JOIN_REFUSE);
}
//��ְCC_C_POSE_OFFICIAL/CC_S_POSE_OFFICIAL
typedef struct tagPose_ConOfficial_Request:DISTRIBUTED_HEADER
{
	int		PlayerID;					//���ID
	int		ConJob;						//ְλ���
	inline void serialize(int playerid,int pose_playerid,int conjob);
}Pose_ConOfficial_Request,*LPPose_ConOfficial_Request;
inline void tagPose_ConOfficial_Request::serialize(int playerid,int pose_playerid,int conjob)
{
	PlayerID = pose_playerid;
	ConJob = conjob;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,MDM_GP_MAP_CONSORTIA,CC_C_POSE_OFFICIAL);
}

typedef struct tagPose_ConOfficial_Response:DISTRIBUTED_HEADER
{
	int		Error_Code;					//���������0���ɹ���1:��Ȩ�ޣ�2���޷�����߼����ְλ��3,��Ա����;������ʧ��
	inline void serialize(int agent,int playerid,int code);
}Pose_ConOfficial_Response,*LPPose_ConOfficial_Response;
inline void tagPose_ConOfficial_Response::serialize(int agent,int playerid,int code)
{
	Error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CONSORTIA,playerid,CC_S_POSE_OFFICIAL);
}

//��ְCC_C_DEPOSE_OFFICIAL/CC_S_DEPOSE_OFFICIAL
typedef struct tagDepose_ConOfficial_Request:DISTRIBUTED_HEADER
{
	int		PlayerID;						//����ְ���ID
	inline void serialize(int playerid,int depose_playerid);
}Depose_ConOfficial_Request,*LPDepose_ConOfficial_Request;
inline void tagDepose_ConOfficial_Request::serialize(int playerid,int depose_playerid)
{
	PlayerID = depose_playerid;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,MDM_GP_MAP_CONSORTIA,CC_C_DEPOSE_OFFICIAL);
}

typedef struct tagDepose_ConOfficial_Response:DISTRIBUTED_HEADER
{
	int		Error_Code;				//���������0���ɹ���������ʧ��
	inline void serialize(int agent,int playerid,int code);
}Depose_ConOfficial_Response,*LPDepose_ConOfficial_Response;
inline void tagDepose_ConOfficial_Response::serialize(int agent,int playerid,int code)
{
	Error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CONSORTIA,playerid,CC_S_DEPOSE_OFFICIAL);
}

//��ѯְλCC_C_QUERY_OFFICIAL/CC_S_QUERY_OFFICIAL
typedef struct tagQuery_ConOfficial_Request:DISTRIBUTED_HEADER
{
	int Page;
	inline void serialize(int playerid,int page);
}Query_ConOfficial_Request,*LPQuery_ConOfficial_Request;
inline void tagQuery_ConOfficial_Request::serialize(int playerid,int page)
{
	Page = page;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,MDM_GP_MAP_CONSORTIA,CC_C_QUERY_OFFICIAL);
}

typedef struct tagConOfficial_Info
{
	int			OfficialID;				//ְλ���
	char		OfficialName[SIZE_16];	//ְλ����
	int			Purview;				//Ȩ��ֵ(��λ���)
	short		Level;					//ְλ����0��������ߣ��᳤��
}ConOfficial_Info,*LPConOfficial_Info;
typedef struct tagQuery_ConOfficial_Response:DISTRIBUTED_HEADER
{
	int		Count;						//ְλ�������ýṹ�����Count��ConOfficial_Info��0:��ְλ��Ϣ��<0���д�����
	int		Page;
	int		PageCount;					//ҳ����
	inline void serialize(int length,int agent,int playerid,int count,int page,int pagecount);
}Query_ConOfficial_Response,*LPQuery_ConOfficial_Response;
inline void tagQuery_ConOfficial_Response::serialize(int length,int agent,int playerid,int count,int page,int pagecount)
{
	Count = count;
	Page = page;
	PageCount = pagecount;
	DISTRIBUTED_HEADER::serialize(length,agent,MDM_GP_MAP_CONSORTIA,playerid,CC_S_QUERY_OFFICIAL);
}

//��ѯ�����������������CC_C_QUERY_CREATELIMIT/CC_S_QUERY_CREATELIMIT
typedef struct tagQuery_CreateConLimit_Request:DISTRIBUTED_HEADER
{
	inline void serialize(int playerid);
}Query_CreateConLimit_Request,*LPQuery_CreateConLimit_Request;
inline void tagQuery_CreateConLimit_Request::serialize(int playerid)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,MDM_GP_MAP_CONSORTIA,CC_C_QUERY_CREATELIMIT);
}

typedef struct tagCreateConLimitInfo
{
	int			Level;						//��������ĵȼ�Ҫ��
	int			JoinLevel;					//���빫��ĵȼ�Ҫ�� 
	int			Money;						//��Ǯ
	int			FoodCount;					//��ʳ����
	int			WoodCount;					//ľ������
	int			IronCount;					//������
	int			SkinCount;					//Ƥ����
	int			RedStoneCount;				//�챦ʯ
	int			YollowStoneCount;			//�Ʊ�ʯ
	int			GreenStoneCount;			//�̱�ʯ
	int			BackIronCount;				//����
	int			Personnel;					//��Ա��������24Сʱ�󣬳�Ա������Ҫ��
	int			Times;						//�����೤ʱ�䣨Сʱ����Աδ��Personnel��ʱ����ɢ����
}CreateConLimitInfo,*LPCreateConLimitInfo;


typedef struct tagQuery_CreateConLimit_Response:DISTRIBUTED_HEADER
{
	int						Error_Code;		//���������0���ɹ���������ʧ��
	CreateConLimitInfo		LimitInfo;		//
	inline void serialize(int agent,int playerid,int code);
}Query_CreateConLimit_Response,*LPQuery_CreateConLimit_Response;
inline void tagQuery_CreateConLimit_Response::serialize(int agent,int playerid,int code)
{
	Error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CONSORTIA,playerid,CC_S_QUERY_CREATELIMIT);
}

//�޸Ĺ�����ּCC_C_MODIFY_CONTENET/CC_S_MODIFY_CONTENET
typedef struct tagModify_ConTenet_Request:DISTRIBUTED_HEADER
{
	char Tenet[SIZE_128];						//��ּ
	inline void serialize(int playerid,const char *strTenet =0);
}Modify_ConTenet_Request,*LPModify_ConTenet_Request;
inline void tagModify_ConTenet_Request::serialize(int playerid,const char *strTenet)
{
	if(strTenet)
	{
		strncpy(Tenet,strTenet,SIZE_128);
	}
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,MDM_GP_MAP_CONSORTIA,CC_C_MODIFY_CONTENET);
}

typedef struct tagModify_ConTenet_Response:DISTRIBUTED_HEADER
{
	int						Error_Code;		//���������0���ɹ���������ʧ��
	inline void serialize(int agent,int playerid,int code);
}Modify_ConTenet_Response,*LPModify_ConTenet_Response;
inline void tagModify_ConTenet_Response::serialize(int agent,int playerid,int code)
{
	Error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CONSORTIA,playerid,CC_S_MODIFY_CONTENET);
}

//�ͻ��˵�½CC_C_CLIENTLOGIN/CC_S_CLIENTLOGIN
typedef struct tagClientLogin_Request:DISTRIBUTED_HEADER
{
	inline void serialize(int playerid);
}ClientLogin_Request,*LPClientLogin_Request;
inline void tagClientLogin_Request::serialize(int playerid)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,MDM_GP_MAP_CONSORTIA,CC_C_CLIENTLOGIN);
}

typedef struct tagClientLogin_Response:DISTRIBUTED_HEADER
{
	int Error_Code;							//0���ɹ���������ʧ��
	inline void serialize(int agent,int plyaerid,int coce);
}ClientLogin_Response,*LPClientLogin_Response;
inline void tagClientLogin_Response::serialize(int agent,int playerid,int code)
{
	Error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CONSORTIA,playerid,CC_S_CLIENTLOGIN);
}
//ת�û᳤CC_C_TRANS_CHAIRMAN/CC_S_TRANS_CHAIRMAN
typedef struct tagTrans_Chairman_Request:DISTRIBUTED_HEADER
{
	int PlayerID;						//���λ᳤
	inline void serialize(int playerid,int next_chairman);
}Trans_Chairman_Request,*LPTrans_Chairman_Request;
inline void tagTrans_Chairman_Request::serialize(int playerid,int next_chairman)
{
	PlayerID = next_chairman;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,MDM_GP_MAP_CONSORTIA,CC_C_TRANS_CHAIRMAN);
}

typedef struct tagTrans_Chairman_Response:DISTRIBUTED_HEADER
{
	int Error_Code;					//0:�ɹ���1��δ���빫�᣻2�����ǻ᳤���޷�ת����3���޷�ת�����Ǳ�����ĳ�Ա��������ʧ��
	inline void serialize(int agent,int plyaerid,int code);
}Trans_Chairman_Response,*LPTrans_Chairman_Response;
inline void tagTrans_Chairman_Response::serialize(int agent,int playerid,int code)
{
	Error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CONSORTIA,playerid,CC_S_TRANS_CHAIRMAN);
}

//��������CC_C_CON_RANKING/CC_S_CON_RANKING
typedef struct tagQuery_Con_Ranking_Request:DISTRIBUTED_HEADER
{
	int Page;							//ҳ��
	inline void serialize(int playerid,int page);
}Query_Con_Ranking_Request,*LPQuery_Con_Ranking_Request;
inline void tagQuery_Con_Ranking_Request::serialize(int playerid,int page)
{
	Page = page;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,MDM_GP_MAP_CONSORTIA,CC_C_CON_RANKING);
}

typedef struct tagConsortia_RankingInfo
{
	int		RankingID;						//�������
	char	ConName[SIZE_32];				//��������
	char	Chairman[SIZE_24];				//�᳤����
	int		Level;							//����ȼ�
	int		Count;							//��Ա����, �������ֶ�
	int		Country;						//��Ӫ
	__int64 CreateTime;						//����ʱ��
}Consortia_RankingInfo,*LPConsortia_RankingInfo;

typedef struct tagQuery_Con_Ranking_Response:DISTRIBUTED_HEADER
{
	int		Count;						//ְλ�������ýṹ�����Count��Consortia_RankingInfo��0:��������Ϣ��<0���д�����
	int		Page;
	int		PageCount;					//ҳ����
	inline void serialize(int length,int agent,int playerid,int count,int page,int pagecount);
}Query_Con_Ranking_Response,*LPQuery_Con_Ranking_Response;
inline void tagQuery_Con_Ranking_Response::serialize(int length,int agent,int playerid,int count,int page,int pagecount)
{
	Count = count;
	Page = page;
	PageCount = pagecount;
	DISTRIBUTED_HEADER::serialize(length,agent,MDM_GP_MAP_CONSORTIA,playerid,CC_S_CON_RANKING);
}

//��������CC_C_PLAYER_RANKING/CC_S_PLAYER_RANKING
typedef struct tagQuery_Player_Ranking_Request:DISTRIBUTED_HEADER
{
	int Page;							//ҳ��
	inline void serialize(int playerid,int page);
}Query_Player_Ranking_Request,*LPQuery_Player_Ranking_Request;
inline void tagQuery_Player_Ranking_Request::serialize(int playerid,int page)
{
	Page = page;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,MDM_GP_MAP_CONSORTIA,CC_C_PLAYER_RANKING);
}

typedef struct tagPlayer_RankingInfo
{
	int		RankingID;					//����
	char	Name[SIZE_24];				//�������
	int		Level;						//�ȼ�
	int		MilitaryRank;				//����
	int		Money;						//��Ǯ
	int		ResourceCount;				//��Դ����
	int		CityCount;					//�ݵ���
	int		GeneralCount;				//ɱ�н���������²������
	int		Credit;						//������
	int		Country;					//��Ӫ
	short	OfficerRace;				//����
	char	IsOnline;					//�Ƿ�����
}Player_RankingInfo,*LPPlayer_RankingInfo;
typedef struct tagQuery_Player_Ranking_Response:DISTRIBUTED_HEADER
{
	int		Count;						//ְλ�������ýṹ�����Count��Player_RankingInfo��0:��������Ϣ��<0���д�����
	int		Page;
	int		PageCount;					//ҳ����
	inline void serialize(int length,int agent,int playerid,int count,int page,int pagecount);
}Query_Player_Ranking_Response,*LPQuery_Player_Ranking_Response;
inline void tagQuery_Player_Ranking_Response::serialize(int length,int agent,int playerid,int count,int page,int pagecount)
{
	Count = count;
	Page = page;
	PageCount = pagecount;
	DISTRIBUTED_HEADER::serialize(length,agent,MDM_GP_MAP_CONSORTIA,playerid,CC_S_PLAYER_RANKING);
}

//ÿ����������CC_C_NEWPLAYER_RANKING/CC_S_NEWPLAYER_RANKING
typedef struct tagQuery_NewPlayer_Ranking_Request:DISTRIBUTED_HEADER
{
	int Page;							//ҳ��
	inline void serialize(int playerid,int page);
}Query_NewPlayer_Ranking_Request,*LPQuery_NewPlayer_Ranking_Request;
inline void tagQuery_NewPlayer_Ranking_Request::serialize(int playerid,int page)
{
	Page = page;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,MDM_GP_MAP_CONSORTIA,CC_C_NEWPLAYER_RANKING);
}

typedef struct tagNewPlayer_RankingInfo
{
	int		RankingID;					//����
	char	Name[SIZE_24];				//�������
	int		Level;						//�ȼ�
	int		MilitaryRank;				//����
	int		Money;						//��Ǯ
	int		ResourceCount;				//��Դ����
	int		Country;					//��Ӫ
}NewPlayer_RankingInfo,*LPNewPlayer_RankingInfo;
typedef struct tagQuery_NewPlayer_Ranking_Response:DISTRIBUTED_HEADER
{
	int		Count;						//ְλ�������ýṹ�����Count��NewPlayer_RankingInfo��0:��������Ϣ��<0���д�����
	int		Page;
	int		PageCount;					//ҳ����
	inline void serialize(int length,int agent,int playerid,int count,int page,int pagecount);
}Query_NewPlayer_Ranking_Response,*LPQuery_NewPlayer_Ranking_Response;
inline void tagQuery_NewPlayer_Ranking_Response::serialize(int length,int agent,int playerid,int count,int page,int pagecount)
{
	Count = count;
	Page = page;
	PageCount = pagecount;
	DISTRIBUTED_HEADER::serialize(length,agent,MDM_GP_MAP_CONSORTIA,playerid,CC_S_NEWPLAYER_RANKING);
}

//������ս������ ������CC_C_FRIENDS_COUNT/CC_S_FRIENDS_COUNT
typedef struct tagQuery_Friends_Count_Request:DISTRIBUTED_HEADER
{
	inline void serialize(int playerid);
}Query_Friends_Count_Request,*LPQuery_Friends_Count_Request;

inline void tagQuery_Friends_Count_Request::serialize(int playerid) 
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,MDM_GP_MAP_CONSORTIA,CC_C_FRIENDS_COUNT);
}

typedef struct tagPlayer_FriendsInfo
{
	int RankingID;				//����
	char PlayerName[SIZE_24];	//�������
	int Integral;				//����
	int Credit;					//����������
	int FriendsCount;			//�����������������ֶ�
	int ConName[SIZE_32];		//��������
	int Count;					//�����Ա����
	int Country;				//��Ӫ
}Player_FriendsInfo,*LPlayer_FriendsInfo;

typedef struct tagQuery_Friends_Count_Response:DISTRIBUTED_HEADER
{
	int Count;
	inline void serialize(int length,int agent,int playerid,int count);
}Query_Friends_Count_Response,*LPQuery_Friends_Count_Response;

inline void tagQuery_Friends_Count_Response::serialize(int length,int agent,int playerid,int count)
{
	Count = count;
	DISTRIBUTED_HEADER::serialize(length,agent,MDM_GP_MAP_CONSORTIA,playerid,CC_S_FRIENDS_COUNT);
}

//������ս������ ������CC_C_MONEY_COUNT/CC_S_MONEY_COUNT
typedef struct tagQuery_Money_Count_Request:DISTRIBUTED_HEADER
{
	inline void serialize(int playerid);
}Query_Money_Count_Request,*LPQuery_Money_Count_Request;

inline void tagQuery_Money_Count_Request::serialize(int playerid)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,MDM_GP_MAP_CONSORTIA,CC_C_MONEY_COUNT);
}

typedef struct tagPlayer_MoneyInfo
{
	int RankingID;				//����
	char PlayerName[SIZE_24];	//�������
	int Integral;				//����
	int ConCredit;				//��������
	int PlayerCredit;			//��������
	int Money;					//��Ǯ,�������ֶ�
	int Country;				//��Ӫ
}Player_MoneyInfo,*LPPlayer_MoneyInfo;

typedef struct tagQuery_Money_Count_Response:DISTRIBUTED_HEADER
{
	int Count;
	inline void serialize(int length,int agent,int playerid,int count);
}Query_Money_Count_Response,*LPQuery_Money_Count_Response;

inline void tagQuery_Money_Count_Response::serialize(int length,int agent,int playerid,int count)
{
	count = count;
	DISTRIBUTED_HEADER::serialize(length,agent,MDM_GP_MAP_CONSORTIA,playerid,CC_S_MONEY_COUNT);
}

//������ս������ ������CC_C_GENERAL_CAPTIVE/CC_S_GENERAL_CAPTIVE
typedef struct tagQuery_GeneralCaptive_Count_Request:DISTRIBUTED_HEADER
{
	inline void serialize(int playerid);
}Query_GeneralCaptive_Count_Request,*LPQuery_GeneralCaptive_Count_Request;

inline void tagQuery_GeneralCaptive_Count_Request::serialize(int playerid)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,MDM_GP_MAP_CONSORTIA,CC_C_GENERAL_CAPTIVE_COUNT);
}

typedef struct tagPlayer_GeneralCaptiveInfo
{
	int RankingID;				//����
	char PlayerName[SIZE_24];	//�������
	int Level;					//�ȼ�
	int Integral;				//����
	int CityCount;				//�ݵ���
	int GeneralCount;			//ɱ�н���������²������,�������ֶ�
	int ResourceCount;			//��Դ����
	int Country;				//��Ӫ
}Player_GeneralCaptiveInfo,*LPPlayer_GeneralCaptiveInfo;

typedef struct tagQuery_GeneralCaptive_Count_Response:DISTRIBUTED_HEADER
{
	int Count;
	inline void serialize(int length,int agent,int playerid,int count);
};

inline void tagQuery_GeneralCaptive_Count_Response::serialize(int length, int agent, int playerid, int count)
{
	Count = count;
	DISTRIBUTED_HEADER::serialize(length,agent,MDM_GP_MAP_CONSORTIA,playerid,CC_S_GENERAL_CAPTIVE_COUNT);
}

//��⹫���Ƿ�24Сʱ����δ��CC_C_CHECKCONSORTIA,	
typedef struct tagConsortia_Timer:DISTRIBUTED_HEADER
{
	inline void serialize(int cmd/* = CC_C_CHECKCONSORTIA*/,int playerid);
}Consortia_Timer,*LPConsortia_Timer;
inline void tagConsortia_Timer::serialize(int cmd,int playerid)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,MDM_GP_MAP_CONSORTIA,cmd);
}

//��ѯ����ڹ��ҵ���ϢCC_C_QUERY_USEROFCOUNTRY,	CC_S_QUERY_USEROFCOUNTRY,
typedef struct tagQuery_UserOfCountry_Request:DISTRIBUTED_HEADER
{
	inline void serialize(int playerid);
}Query_UserOfCountry_Request,*LPQuery_UserOfCountry_Request;
inline void tagQuery_UserOfCountry_Request::serialize(int playerid)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,MDM_GP_MAP_CONSORTIA,CC_C_QUERY_USEROFCOUNTRY);
}

typedef struct tagQuery_UserOfCountry_Response:DISTRIBUTED_HEADER
{
	short Error_Code;			//0:��ѯ�ɹ���������ʧ�ܣ�
	short Type;					//���ͣ��ο�ö��Dept_Type
	short Postion;				//ְλ���ο�ö��Postion_Type
	unsigned int Treasury;		//�����ʽ�
	unsigned int DODMoney;		//�������ʽ�
	unsigned int FinanceMoney;	//�������ʽ�
	inline void serialize(int agent,int playerid,short code,short type,short postion,unsigned int treasury,unsigned int dodmoney,unsigned int financemoney);
}Query_UserOfCountry_Response,*LPQuery_UserOfCountry_Response;
inline void tagQuery_UserOfCountry_Response::serialize(int agent,int playerid,short code,short type,short postion,unsigned int treasury,unsigned int dodmoney,unsigned int financemoney)
{
	Error_Code = code;
	Type = type;
	Postion = postion;
	Treasury = treasury;
	DODMoney = dodmoney;
	FinanceMoney = financemoney;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CONSORTIA,playerid,CC_S_QUERY_USEROFCOUNTRY);
}

//��ѯ����Ӫ����Ա�б���������ίԱ�ᡢ����ίԱ�ᡢ����ίԱ��CC_C_QUERY_COUNCILLIST,CC_S_QUERY_COUNCILLIST,
typedef struct tagQuery_CouncilList_Request:DISTRIBUTED_HEADER
{
	short Type;					//���ͣ��ο�Dept_Type.-1��������Ա+������+��������
	inline void serialize(int playerid,short type);
}Query_CouncilList_Request,*LPQuery_CouncilList_Request;
inline void tagQuery_CouncilList_Request::serialize(int playerid,short type)
{
	Type = type;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,MDM_GP_MAP_CONSORTIA,CC_C_QUERY_COUNCILLIST);
}

typedef struct tagCouncil_Info
{
	int		PlayerID;					//��ԱID
	char	Name[SIZE_24];				//��Ա����
	short   Level;						//����
	short   MilitaryRank;				//����
	short	Type;						//���ͣ��ο�Dept_Type
	short	Postion;					//ְλ���ο�ö��Postion_Type
}Council_Info,*LPCouncil_Info;
typedef struct tagQuery_CouncilList_Response:DISTRIBUTED_HEADER
{
	int Count;							//����
	int CountryID;						//��ӪID
	inline void serialize(int agent,int playerid,int count,int countryid,int cmd/* = CC_S_QUERY_COUNCILLIST*/);
	inline Council_Info & operator [](unsigned int);
}Query_CouncilList_Response,*LPQuery_CouncilList_Response;
inline void tagQuery_CouncilList_Response::serialize(int agent,int playerid,int count,int countryid,int cmd)
{
	Count = count;
	CountryID = countryid;
	DISTRIBUTED_HEADER::serialize(sizeof(*this) + count * sizeof(Council_Info),agent,MDM_GP_MAP_CONSORTIA,playerid,cmd);
}
inline Council_Info & tagQuery_CouncilList_Response::operator [](unsigned int index)
{
	return ((LPCouncil_Info)(this + 1))[index];
}
//�μӾ�ѡ������Ϊ��Ա��CC_C_TAKEPARTIN_ELECTION,CC_S_TAKEPARTIN_ELECTION,
typedef struct tagTakePartIn_Election_Request:DISTRIBUTED_HEADER
{
	short Dept;					//��ѡְλ��������������������ο�Dept_Type
	char Creed[100];			//����
	int Money;			//��ѡ����
	inline void serialize(int playerid,short dept,const char *creed,int money);
}TakePartIn_Election_Request,*LPTakePartIn_Election_Request;
inline void tagTakePartIn_Election_Request::serialize(int playerid,short dept,const char *creed,int money)
{
	Dept = dept;
	if(creed)
	{
		strcpy(Creed,creed);
	}
	else
	{
		Creed[0] = 0;
	}
	Money = money;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,MDM_GP_MAP_CONSORTIA,CC_C_TAKEPARTIN_ELECTION);
}

typedef struct tagTakePartIn_Election_Response:DISTRIBUTED_HEADER
{
	short Error_Code;				//0���ɹ���1:GO�㲻����2����Ǯ������3:����Ա;4:�Ѳμӹ���ѡ��������ʧ��
	inline void serialize(int agent,int playerid,short code);
}TakePartIn_Election_Response,*LPTakePartIn_Election_Response;
inline void tagTakePartIn_Election_Response::serialize(int agent,int playerid,short code)
{
	Error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CONSORTIA,playerid,CC_S_TAKEPARTIN_ELECTION);
}

//��ѯ��ѡ���б�(����������������CC_C_QUERY_CANDIDATELIST,CC_S_QUERY_CANDIDATELIST,
typedef struct tagQuery_CandidateList_Request:DISTRIBUTED_HEADER
{
	short Type;					//��ѯ���ţ�����ο�Dept_Type
	inline void serialize(int playerid,short type);
}Query_CandidateList_Request,*LPQuery_CandidateList_Request;
inline void tagQuery_CandidateList_Request::serialize(int playerid,short type)
{
	Type = type;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,MDM_GP_MAP_CONSORTIA,CC_C_QUERY_CANDIDATELIST);
}
typedef struct tagCandidate_Info
{
	int				PlayerID;				//��ѡ��ID
	char			Name[SIZE_24];			//��ѡ������
	short			Level;					//����
	short			MilitaryRank;				//����
	short			Type;					//���ͣ��ο�Dept_Type:��������������
	char			Creed[100];				//��ѡ���죨�������ɣ�
	unsigned int	Expense;				//��ѡ����
	short			Vote;					//��Ʊ��
}Candidate_Info,*LPCandidate_Info;
typedef struct tagQuery_CandidateList_Response:DISTRIBUTED_HEADER
{
	int Count;
	int Times;								//ѡ��ʣ��ʱ��
	inline void serialize(int agent,int playerid,int count,int times);
	inline Candidate_Info & operator [](unsigned int index);
}Query_CandidateList_Response,*LPQuery_CandidateList_Response;
inline void tagQuery_CandidateList_Response::serialize(int agent,int playerid,int count,int times)
{
	Count = count;
	Times = times;
	DISTRIBUTED_HEADER::serialize(sizeof(*this) + count * sizeof(Candidate_Info),agent,MDM_GP_MAP_CONSORTIA,playerid,CC_S_QUERY_CANDIDATELIST);
}
inline Candidate_Info & tagQuery_CandidateList_Response::operator [](unsigned int index)
{
	return ((LPCandidate_Info)(this + 1))[index];
}

//��ҽ���ͶƱѡ��CC_C_PLAYER_BALLOT,	CC_S_PLAYER_BALLOT,
typedef struct tagPlayer_Ballot_Request:DISTRIBUTED_HEADER
{
	int	Candidate;			//��ѡ��ID
	inline void serialize(int playerid,int candiate);
}Player_Ballot_Request,*LPPlayer_Ballot_Request;
inline void tagPlayer_Ballot_Request::serialize(int playerid,int candiate)
{
	Candidate = candiate;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,MDM_GP_MAP_CONSORTIA,CC_C_PLAYER_BALLOT);
}

typedef struct tagPlayer_Ballot_Response:DISTRIBUTED_HEADER
{
	short Error_Code;		//0:�ɹ���1:��ѡ��δ��ѡ��2:��Ͷ��Ʊ�������ظ�ͶƱ��������ʧ��
	int	Candidate;			//��ѡ��ID
	int Type;				//���ͣ��ο�:Dept_Type
	int Vote;				//Ŀǰ��Ʊ��
	inline void serialize(int agent,int playerid,short code,int candidate,int type,int vote);
}Player_Ballot_Response,*LPPlayer_Ballot_Response;
inline void tagPlayer_Ballot_Response::serialize(int agent,int playerid,short code,int candidate,int type,int vote)
{
	Error_Code = code;
	Candidate = candidate;
	Type = type;
	Vote = vote;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CONSORTIA,playerid,CC_S_PLAYER_BALLOT);
}

//����CC_C_COUNTRY_DELATE,CC_S_COUNTRY_DELATE,
typedef struct tagCountry_Delate_Request:DISTRIBUTED_HEADER
{
	short Type;					//�������ͣ��ο�Dept_Type
	inline void serialize(int playerid,short type);
}Country_Delate_Request,*LPCountry_Delate_Request;
inline void tagCountry_Delate_Request::serialize(int playerid,short type)
{
	Type = type;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,MDM_GP_MAP_CONSORTIA,CC_C_COUNTRY_DELATE);
}

typedef struct tagCountry_Delate_Response:DISTRIBUTED_HEADER
{
	short Error_Code;			//0:�ɹ���1:������һ�£�2���ѵ�����3��û��Ȩ�ޣ�4:�������48Сʱ���ܵ�����5:���ܵ����Լ���������ʧ��
	inline void serialize(int agent,int playerid,short code);
}Country_Delate_Response,*LPCountry_Delate_Response;
inline void tagCountry_Delate_Response::serialize(int agent,int playerid,short code)
{
	Error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CONSORTIA,playerid,CC_S_COUNTRY_DELATE);
}

//��ѯ������ϢCC_C_QUERY_DELATEINFO,CC_S_QUERY_DELATEINFO,
typedef struct tagQuery_DelateInfo_Request:DISTRIBUTED_HEADER
{
	inline void serialize(int playerid);
}Query_DelateInfo_Request,*LPQuery_DelateInfo_Request;
inline void tagQuery_DelateInfo_Request::serialize(int playerid)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,MDM_GP_MAP_CONSORTIA,CC_C_QUERY_DELATEINFO);
}

typedef struct tagQuery_DelateInfo_Response:DISTRIBUTED_HEADER
{
	int Type;				//�ο�Dept_Type,-1��û�е�����111��ʧ��
	unsigned int Times;		//ʣ��ʱ��
	int UnderWrite;			//ǩ������
	int Uphold;				//�޳�Ʊ
	int Oppose;				//����Ʊ
	int Nonuser;			//��ȨƱ
	short State;			//�ο�DeptState_Type,�Ƿ��в�����״̬
	inline void serialize(int agent,int playerid,int type,unsigned int times,int underwrite,int uphold,int oppose,int nonuser,short state);
}Query_DelateInfo_Response,*LPQuery_DelateInfo_Response;
inline void tagQuery_DelateInfo_Response::serialize(int agent,int playerid,int type,unsigned int times,int underwrite,int uphold,int oppose,int nonuser,short state)
{
	Type = type;
	Times = times;
	UnderWrite = underwrite;
	Uphold = uphold;
	Oppose = oppose;
	Nonuser = nonuser;
	State = state;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CONSORTIA,playerid,CC_S_QUERY_DELATEINFO);
}

//����ͶƱCC_C_DELATE_BALLOT,	CC_S_DELATE_BALLOT,
typedef struct tagDelate_Ballot_Request:DISTRIBUTED_HEADER
{
	Vote_Type Value;				//ͶƱ���ͣ����޳ɡ����ԡ���Ȩ��ǩ��
	inline void serialize(int playerid,Vote_Type value);
}Delate_Ballot_Request,*LPDelate_Ballot_Request;
inline void tagDelate_Ballot_Request::serialize(int playerid,Vote_Type value)
{
	Value = value;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,MDM_GP_MAP_CONSORTIA,CC_C_DELATE_BALLOT);
}
typedef struct tagDelate_Ballot_Response:DISTRIBUTED_HEADER
{
	short Error_Code;				//0���ɹ�,1:��ͶƱ��2�������޷�����ͶƱ��3:û��Ȩ�ޣ�������ʧ��
	inline void serialize(int agent,int playerid,short code);
}Delate_Ballot_Response,*LPDelate_Ballot_Response;
inline void tagDelate_Ballot_Response::serialize(int agent,int playerid,short code)
{
	Error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CONSORTIA,playerid,CC_S_DELATE_BALLOT);
}

//�޸Ĳ�ѡ����CC_C_MODIFY_ELECTIONCREED,	CC_S_MODIFY_ELECTIONCREED,
typedef struct tagModify_ElectionCreed_Request:DISTRIBUTED_HEADER
{
	int Money;				//������ѡ����
	char Creed[100];		//�¸���
	inline void serialize(int playerid,int money,const char *pCreed);
}Modify_ElectionCreed_Request,*LPModify_ElectionCreed_Request;
inline void tagModify_ElectionCreed_Request::serialize(int playerid,int money,const char *pCreed)
{
	Money = money;
	if(pCreed)
	{
		strcpy(Creed,pCreed);
	}
	else
	{
		Creed[0] = 0;
	}
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,MDM_GP_MAP_CONSORTIA,CC_C_MODIFY_ELECTIONCREED);
}
typedef struct tagModify_ElectionCreed_Response:DISTRIBUTED_HEADER
{
	short Error_Code;			//0���ɹ���1�����δ��ѡ��2����Ǯ������������ʧ��
	inline void serialize(int agent,int playerid,short code);
}Modify_ElectionCreed_Response,*LPModify_ElectionCreed_Response;
inline void tagModify_ElectionCreed_Response::serialize(int agent,int playerid,short code)
{
	Error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CONSORTIA,playerid,CC_S_MODIFY_ELECTIONCREED);
}
//������������������CC_C_CREATE_TIRRIER,CC_S_CREATE_TIRRIER,
typedef struct tagCreateTirrier_Request:DISTRIBUTED_HEADER
{
	int TirrierType;			//0:սʿ��1�����֣�2����ʦ
	inline void serialize(int playerid,int tirriertype);
}CreateTirrier_Request,*LPCreateTirrier_Request;
inline void tagCreateTirrier_Request::serialize(int playerid,int tirriertype)
{
	TirrierType = tirriertype;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,MDM_GP_MAP_CONSORTIA,CC_C_CREATE_TIRRIER);
}

typedef struct tagCreateTirrier_Response:DISTRIBUTED_HEADER
{
	short Error_Code;				//0:�ɹ���1��û��Ȩ�ޣ�2����Ǯ������3:����Ĳ�������4:�����������������ޣ�111��ʧ��
	inline void serialize(int agent,int playerid,short code);
}CreateTirrier_Response,*LPCreateTirrier_Response;
inline void tagCreateTirrier_Response::serialize(int agent,int playerid,short code)
{
	Error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CONSORTIA,playerid,CC_S_CREATE_TIRRIER);
}

//����������CC_C_TIRRIER_GOOUT,	CC_S_TIRRIER_GOOUT,
typedef struct tagTirrierGoOut_Request:DISTRIBUTED_HEADER
{
	int MapID;				//Ŀ�Ŀ�ID
	int Week;				//�������ڣ�3��������5������
	inline void serialize(int playerid,int mapid,int week);
}TirrierGoOut_Request,*LPTirrierGoOut_Request;
inline void tagTirrierGoOut_Request::serialize(int playerid,int mapid,int week)
{
	MapID = mapid;
	Week = week;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,MDM_GP_MAP_CONSORTIA,CC_C_TIRRIER_GOOUT);
}
typedef struct tagTirrierGoOut_Response:DISTRIBUTED_HEADER
{
	short Error_Code;			//0:�ɹ���1��û��Ȩ�ޣ�111��ʧ��
	inline void serialize(int agent,int playerid,short code);
}TirrierGoOut_Response,*LPTirrierGoOut_Response;
inline void tagTirrierGoOut_Response::serialize(int agent,int playerid,short code)
{
	Error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CONSORTIA,playerid,CC_S_TIRRIER_GOOUT);
}
//������������CC_C_UPGRADEBUILD,CC_S_UPGRADEBUILD,
typedef struct tagUpgradeBuild_Request:DISTRIBUTED_HEADER
{
	int MapID;//����ͼID
	int BuildID;//������ID
	int Money;//���׽�Ǯ
	inline void serialize(int playerid,int mapid,int buildid,int money);
}UpgradeBuild_Request,*LPUpgradeBuild_Request;
inline void tagUpgradeBuild_Request::serialize(int playerid,int mapid,int buildid,int money)
{
	MapID = mapid;
	BuildID = buildid;
	Money = money;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,MDM_GP_MAP_CONSORTIA,CC_C_UPGRADEBUILD);
}

typedef struct tagUpgradeBuild_Response:DISTRIBUTED_HEADER
{
	short Error_Code;		//0:�ɹ���1��û��Ȩ�ޣ�2����Ǯ���㣻111������ʧ��
	inline void serialize(int agent,int playerid,short code);
}UpgradeBuild_Response,*LPUpgradeBuild_Response;
inline void tagUpgradeBuild_Response::serialize(int agent,int playerid,short code)
{
	Error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CONSORTIA,playerid,CC_S_UPGRADEBUILD);
}

//�޸�ʵ������CC_C_MODIFYNOTICE,CC_S_MODIFYNOTICE,
typedef struct tagModifyNotice_Request:DISTRIBUTED_HEADER
{
	char	Notice[SIZE_256];		//��������
	inline void serialize(int playerid,const char *notice);
}ModifyNotice_Request,*LPModifyNotice_Request;
inline void tagModifyNotice_Request::serialize(int playerid,const char *notice)
{
	if(notice)
	{
		strcpy(Notice,notice);
	}
	else
	{
		Notice[0] = 0;
	}
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,MDM_GP_MAP_CONSORTIA,CC_C_MODIFYNOTICE);
}

typedef struct tagModifyNotice_Response:DISTRIBUTED_HEADER
{
	short Error_Code;			//0:�ɹ�;1:û��Ȩ�ޣ�111��ʧ��
	inline void serialize(int agent,int playerid,short code);
}ModifyNotice_Response,*LPModifyNotice_Response;
inline void tagModifyNotice_Response::serialize(int agent,int playerid,short code)
{
	Error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CONSORTIA,playerid,CC_S_MODIFYNOTICE);
}
//��ѯ����������CC_C_QUERY_TIRRIERCOUNT,CC_S_QUERY_TIRRIERCOUNT,
typedef struct tagQuery_TirrierCount_Request:DISTRIBUTED_HEADER
{
	inline void serialize(int playerid);
}Query_TirrierCount_Request,*LPQuery_TirrierCount_Request;
inline void tagQuery_TirrierCount_Request::serialize(int playerid)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,MDM_GP_MAP_CONSORTIA,CC_C_QUERY_TIRRIERCOUNT);
}

typedef struct tagQuery_TirrierCount_Response:DISTRIBUTED_HEADER
{
	int TirrierCount[3];//0:սʿ������1������������2����ʦ����
	inline void serialize(int agent,int playerid);
	inline int & operator [](unsigned int index);
}Query_TirrierCount_Response,*LPQuery_TirrierCount_Response;
inline void tagQuery_TirrierCount_Response::serialize(int agent,int playerid)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CONSORTIA,playerid,CC_S_QUERY_TIRRIERCOUNT);
}
inline int & tagQuery_TirrierCount_Response::operator [](unsigned int index)
{
	return TirrierCount[index];
}
//֪ͨ����ͼ����������CC_C_NOTIFY_CREATETIRRIER,CC_S_NOTIFY_CREATETIRRIER,
typedef struct tagNotify_CreateTirrier_Request:DISTRIBUTED_HEADER
{
	int CountryID;				//��Ӫ
	int WeaponType;				//����
	int WeaponLevel;			//��������
	int DefendType;				//��������
	int DefendLevel;			//���߼���
	int SoldierType;			//ʿ������
	int TirrierType;			//0:սʿ��1�����֣�2����ʦ
	inline void serialize(int agent,int playerid,int countryid,int weapontype,int weaponlevel,int defendtype,int defendlevel,int soldiertype,int type);
}Notify_CreateTirrier_Request,*LPNotify_CreateTirrier_Request;
inline void tagNotify_CreateTirrier_Request::serialize(int agent,int playerid,int countryid,int weapontype,int weaponlevel,int defendtype,int defendlevel,int soldiertype,int type)
{
	CountryID = countryid;
	WeaponType = weapontype;
	WeaponLevel = weaponlevel;
	DefendType = defendtype;
	DefendLevel = defendlevel;
	SoldierType = soldiertype;
	TirrierType = type;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,playerid,MDM_GP_MAP_CONSORTIA,CC_C_NOTIFY_CREATETIRRIER);
}

typedef struct tagNotify_CreateTirrier_Response:DISTRIBUTED_HEADER
{
	short Error_Code;			//0:�ɹ���������ʧ��
	int CountryID;
	int TirrierType;			//0:սʿ��1�����֣�2����ʦ
	inline void serialize(int agent,int playerid,short code,int countryid,int type);
}Notify_CreateTirrier_Response,*LPNotify_CreateTirrier_Response;
inline void tagNotify_CreateTirrier_Response::serialize(int agent,int playerid,short code,int countryid,int type)
{
	Error_Code = code;
	CountryID = countryid;
	TirrierType = type;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,playerid,MDM_GP_MAP_CONSORTIA,CC_S_NOTIFY_CREATETIRRIER);
}

//֪ͨ���ͼ����������������CC_C_NOTIFY_TIRRIERGOOUT,CC_S_NOTIFY_TIRRIERGOOUT,
typedef struct tagNotify_TirrierGoOut_Request:DISTRIBUTED_HEADER
{
	int ArmyID;				//����ID
	int MapID;				//Ŀ�Ŀ�
	inline void serialize(int mapid,int armyid,int tomapid);
}Notify_TirrierGoOut_Request,*LPNotify_TirrierGoOut_Request;
inline void tagNotify_TirrierGoOut_Request::serialize(int mapid,int armyid,int tomapid)
{
	MapID = tomapid;
	ArmyID = armyid;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,MDM_GP_MAP_CONSORTIA,mapid,CC_C_NOTIFY_TIRRIERGOOUT);
}

//֪ͨ����ͼ������������CC_C_NOTIFY_UPGRADEBUILD,	CC_S_NOTIFY_UPGRADEBUILD,
typedef struct tagNotify_UpgradeBuild_Request:DISTRIBUTED_HEADER
{
	int CountryID;
	int MapID;
	int BuildID;
	int Money;					//
	inline void serialize(int agent,int playerid,int countryid,int mapid,int buildid,int money);
}Notify_UpgradeBuild_Request,*LPNotify_UpgradeBuild_Request;
inline void tagNotify_UpgradeBuild_Request::serialize(int agent,int playerid,int countryid,int mapid,int buildid,int money)
{
	CountryID = countryid;
	MapID = mapid;
	BuildID = buildid;
	Money = money;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,playerid,MDM_GP_MAP_CONSORTIA,CC_C_NOTIFY_UPGRADEBUILD);
}

typedef struct tagNotify_UpgradeBuild_Response:DISTRIBUTED_HEADER
{
	short Error_Code;
	int CountryID;
	int MapID;
	int BuildID;
	int Money;					//
	int Level;					//����
	inline void serialize(int agent,int playerid,short code,int countryid,int mapid,int buildid,int money,int level);
}Notify_UpgradeBuild_Response,*LPNotify_UpgradeBuild_Response;
inline void tagNotify_UpgradeBuild_Response::serialize(int agent,int playerid,short code,int countryid,int mapid,int buildid,int money,int level)
{
	Error_Code = code;
	CountryID = countryid;
	MapID = mapid;
	BuildID = buildid;
	Money = money;
	Level = level;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,playerid,MDM_GP_MAP_CONSORTIA,CC_S_NOTIFY_UPGRADEBUILD);
}
//֪ͨGM�޸���������CC_C_NOTIFY_MODIFYNOTICE,CC_S_NOTIFY_MODIFYNOTICE,
typedef struct tagNotify_ModifyNotice_Request:DISTRIBUTED_HEADER
{
	int CountryID;					//��Ӫ
	char Notice[SIZE_256];			//��������
	inline void serialize(int agent,int playerid,int countryid,const char *notice);
}Notify_ModifyNotice_Request,*LPNotify_ModifyNotice_Request;
inline void tagNotify_ModifyNotice_Request::serialize(int agent,int playerid,int countryid,const char *notice)
{
	CountryID = countryid;
	if(notice)
	{
		strcpy(Notice,notice);
	}
	else
	{
		Notice[0] = 0;
	}
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,playerid,MDM_GP_MAP_CONSORTIA,CC_C_NOTIFY_MODIFYNOTICE);
}
typedef struct tagNotify_ModifyNotice_Response:DISTRIBUTED_HEADER
{
	short Error_Code;				//0:�ɹ�
	inline void serialize(int agent,int playerid,short code);
}Notify_ModifyNotice_Response,*LPNotify_ModifyNotice_Response;
inline void tagNotify_ModifyNotice_Response::serialize(int agent,int playerid,short code)
{
	Error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,playerid,MDM_GP_MAP_CONSORTIA,CC_S_NOTIFY_MODIFYNOTICE);
}
//�г���˰CC_C_NOTIFY_MARKETRATE,CC_S_NOTIFY_MARKETRATE,
typedef struct tagNotify_MarketRate_Request:DISTRIBUTED_HEADER
{
	int Money;
	inline void serialize(int agent,int playerid,int money);
}Notify_MarketRate_Request,*LPNotify_MarketRate_Request;
inline void tagNotify_MarketRate_Request::serialize(int agent,int playerid,int money)
{
	Money = money;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,playerid,MDM_GP_MAP_CONSORTIA,CC_C_NOTIFY_MARKETRATE);
}

typedef struct tagNotify_MarketRate_Response:DISTRIBUTED_HEADER
{
	int Error_Code;
	inline void serialize(int agent,int playerid,int code);
}Notify_MarketRate_Response,*LPNotify_MarketRate_Response;
inline void tagNotify_MarketRate_Response::serialize(int agent,int playerid,int code)
{
	Error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,playerid,MDM_GP_MAP_CONSORTIA,CC_S_NOTIFY_MARKETRATE);
}

//�㲥�齨�����������Գ�����CC_S_BROAD_ADDARMY,	
//����������CC_S_BROAD_TIRRIERGOOUT
typedef struct tagBroad_TirrierState:DISTRIBUTED_HEADER
{
	int CountryID;				//��Ӫ
	char Name[SIZE_24];			//��������
	union
	{
		int MapID;				//������������Ŀ�Ŀ�ID
		int Money;				//�齨����������Ҫ�ķ���
	};
	inline void serialize(int countryid,const char *name,int value,int cmd);
}Broad_TirrierState,*LPBroad_TirrierState;
inline void tagBroad_TirrierState::serialize(int countryid,const char *name,int value,int cmd)
{
	CountryID = countryid;
	MapID = value;
	if(name)
	{
		strcpy(Name,name);
	}
	else
	{
		Name[0] = 0;
	}
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,MDM_GP_MAP_CONSORTIA,0,cmd);
}

//�μӾ�ѡ�㲥��ϢCC_S_BROAD_ELECTION,	
typedef struct tagBroad_Election_Response:DISTRIBUTED_HEADER
{
	int CountryID;					//��Ӫ
	char Name[SIZE_24];				//��ѡ������
	int Type;						//�ο�Dept_Type
	int	Money;						//��ѡ����
	inline void serialize(int countryid,const char *name,int type,int money);
}Broad_Election_Response,*LPBroad_Election_Response;
inline void tagBroad_Election_Response::serialize(int countryid,const char *name,int type,int money)
{
	CountryID = countryid;
	if(name)
	{
		strcpy(Name,name);
	}
	else
	{
		Name[0] = 0;
	}
	Type = type;
	Money = money;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,MDM_GP_MAP_CONSORTIA,0,CC_S_BROAD_ELECTION);
}
//������������CC_S_BROAD_UPGRADEBUILD,
typedef struct tagBroad_UpgradeBuild:DISTRIBUTED_HEADER
{
	int CountryID;				//��Ӫ
	char Name[SIZE_24];			//��������
	int MapID;					//����ͼID
	int BuildID;				//��������ID
	int Money;					//���׽��
	int Level;					//�����ļ���
	inline void serialize(int countryid,const char *name,int mapid,int buildid,int money,int level);
}Broad_UpgradeBuild,*LPBroad_UpgradeBuild;
inline void tagBroad_UpgradeBuild::serialize(int countryid,const char *name,int mapid,int buildid,int money,int level)
{
	CountryID = countryid;
	if(name)
	{
		strcpy(Name,name);
	}
	else
	{
		Name[0] = 0;
	}
	MapID = mapid;
	BuildID = buildid;
	Money = money;
	Level = level;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,MDM_GP_MAP_CONSORTIA,0,CC_S_BROAD_UPGRADEBUILD);
}

//����״̬CC_S_BROAD_DELATESTATE
typedef struct tagBroad_DelateState:DISTRIBUTED_HEADER
{
	int CountryID;
	int	Type;	//�������ͣ��ο�DetpType
	int State;		//0:����ǩ����ʼ��1������ͶƱ��ʼ��2������ͶƱ������3������ǩ����������ʾǩ��ʧ�ܣ�
	inline void serialize(int countryid,int type,int state);
}Broad_DelateState,*LPBroad_DelateState;
inline void tagBroad_DelateState::serialize(int countryid,int type,int state)
{
	CountryID = countryid;
	Type = type;
	State = state;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,MDM_GP_MAP_CONSORTIA,0,CC_S_BROAD_DELATESTATE);
}
//������������̨����������������CC_S_BROAD_EXCHANGEMINISTER
typedef  struct tagBroad_ExchangeMinister:DISTRIBUTED_HEADER
{
	int CountryID;		//��ӪID
	int Dept;			//�ο�Dept_Type
	char OldMinister[SIZE_24];//ԭ��������
	char NewMinister[SIZE_24];//���β�������
	inline void serialize(int countryid,int dept,const char *oldname,const char *newname);
}Broad_ExchangeMinister,*LPBroad_ExchangeMinister;
inline void tagBroad_ExchangeMinister::serialize(int countryid,int dept,const char *oldname,const char *newname)
{
	CountryID = countryid;
	Dept = dept;
	if(oldname)
	{
		strcpy(OldMinister,oldname);
	}
	else
	{
		OldMinister[0] = 0;
	}
	if(newname)
	{
		strcpy(NewMinister,newname);
	}
	else
	{
		NewMinister[0] = 0;
	}
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,MDM_GP_MAP_CONSORTIA,0,CC_S_BROAD_EXCHANGEMINISTER);
}
//add 20100302
//��ȨCC_C_MODIFY_PURVIEW,CC_S_MODIFY_PURVIEW,
typedef struct tagModify_Purview_Request:DISTRIBUTED_HEADER
{
	int playerID;				//����Ȩ��
	int purview;				//�µ�Ȩ��
	inline void serialize(int playerid,int authorizedID,int purv);
}Modify_Purview_Request,*LPtagModify_Purview_Request;
inline void tagModify_Purview_Request::serialize(int playerid,int authorizedID,int purv)
{
	this->playerID = authorizedID;
	this->purview = purv;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,MDM_GP_MAP_CONSORTIA,CC_C_MODIFY_PURVIEW);
}
typedef struct tagModify_Purview_Response:DISTRIBUTED_HEADER
{
	short error_Code;					//0:�ɹ���1��û��Ȩ�ޣ�2������Ȩ��δ����ù��᣻111��ʧ��
	inline void serialize(int agent,int playerid,short code);
}Modify_Purview_Response,*LPModify_Purview_Response;
inline void tagModify_Purview_Response::serialize(int agent,int playerid,short code)
{
	error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CONSORTIA,playerid,CC_S_MODIFY_PURVIEW);
}
//����CC_C_UPDATECONSORTIA,CC_S_UPDATECONSORTIA,
typedef struct tagUpdateConsortia_Request:DISTRIBUTED_HEADER
{
	inline void serialize(int playerid);
}UpdateConsortia_Request,*LPUpdateConsortia_Request;
inline void tagUpdateConsortia_Request::serialize(int playerid)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,MDM_GP_MAP_CONSORTIA,CC_C_UPDATECONSORTIA);
}
typedef struct tagUpdateConsortia_Response:DISTRIBUTED_HEADER
{
	short error_Code;			//0:�ɹ�;1:û��Ȩ�ޣ�2:�������ʼҾ���ʧ�ܣ�û�г��򣩣�3��GO�㲻�㣻4����Ǯ���㣻5�����鲻�㣻6����������;7:�������ʼҾ��ţ�Ϊռ�г���111��ʧ��
	inline void serialize(int agent,int playerid,short code);
}UpdateConsortia_Response,*LPUpdateConsortia_Response;
inline void tagUpdateConsortia_Response::serialize(int agent,int playerid,short code)
{
	error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CONSORTIA,playerid,CC_S_UPDATECONSORTIA);
}
//��Ǯ������CC_C_CONTRIBUTEMONEY,CC_S_CONTRIBUTEMONEY,
typedef struct tagContributeMoney_Request:DISTRIBUTED_HEADER
{
	int money;
	inline void serialize(int playerid,int money);
}ContributeMoney_Request,*LPContributeMoney_Request;
inline void tagContributeMoney_Request::serialize(int playerid,int conMoney)
{
	money  = conMoney;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,MDM_GP_MAP_CONSORTIA,CC_C_CONTRIBUTEMONEY);
}
typedef struct tagContributeMoney_Response:DISTRIBUTED_HEADER
{
	short error_Code;			//0:�ɹ���1��δ������ţ�2����Ǯ����;111:ʧ��
	inline void serialize(int agent,int playerid,short code);
}ContributeMoney_Response,*LPContributeMoney_Response;
inline void tagContributeMoney_Response::serialize(int agent,int playerid,short code)
{
	error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CONSORTIA,playerid,CC_S_CONTRIBUTEMONEY);
}
//�칤��CC_C_SALARIED,CC_S_SALARIED,
typedef struct tagSalaried_Request:DISTRIBUTED_HEADER
{
	inline void serialize(int playerid);
}Salaried_Request,*LPalaried_Request;
inline void tagSalaried_Request::serialize(int playerid)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,MDM_GP_MAP_CONSORTIA,CC_C_SALARIED);
}
typedef struct tagSalaried_Response:DISTRIBUTED_HEADER
{
	int money;				//-3:���ųƺŲ����ڣ�-2��δ���빤�ᣬ-1:���첻����ȡ��0:��δ�о��ųƺţ�>0����ȡ�Ĺ�����
	inline void serialize(int agent,int playerid,int wage);
}Salaried_Response,*LPSalaried_Response;
inline void tagSalaried_Response::serialize(int agent,int playerid,int wage)
{
	money = wage;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CONSORTIA,playerid,CC_S_SALARIED);
}
//�޸ľ��ű�ʶCC_C_MODIFY_SIGNID,CC_S_MODIFY_SIGNID,
typedef struct tagModify_SignID_Request:DISTRIBUTED_HEADER
{
	int newSignID;
	inline void serialize(int playerid,int signid);
}Modify_SignID_Request,*LPModify_SignID_Request;
inline void tagModify_SignID_Request::serialize(int playerid,int signid)
{
	newSignID = signid;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,MDM_GP_MAP_CONSORTIA,CC_C_MODIFY_SIGNID);
}
typedef struct tagModify_SignID_Response:DISTRIBUTED_HEADER
{
	short error_Code;					//0:�ɹ���1��û��Ȩ�ޣ�2���ǻʼҾ��ţ�111��ʧ��
	inline void serialize(int agent,int playerid,short code);
}Modify_SignID_Response,*LPModify_SignID_Response;
inline void tagModify_SignID_Response::serialize(int agent,int playerid,short code)
{
	error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CONSORTIA,playerid,CC_S_MODIFY_SIGNID);
}
//֪ͨ�շ�ϵͳ��������CC_C_NOTIFY_UPDATECONSORTIA,CC_S_NOTIFY_UPDATECONSORTIA,
typedef struct tagNotify_UpdateConsortia_Request:DISTRIBUTED_HEADER
{
	int playerID;			//���ID
	int consortiaID;		//����ID
	int goDian;				//�۳�GO��
	inline void serialize(int agent,int playerid,int consid,int godian);
}Notify_UpdateConsortia_Request,*LPNotify_UpdateConsortia_Request;
inline void tagNotify_UpdateConsortia_Request::serialize(int agent,int playerid,int consid,int godian)
{
	playerID = playerid;
	consortiaID = consid;
	goDian = godian;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CONSORTIA,MDM_GP_MAP_CHARGE,CC_C_NOTIFY_UPDATECONSORTIA);
}
typedef struct tagNotify_UpdateConsortia_Response:DISTRIBUTED_HEADER
{
	int playerID;
	int consortiaID;
	inline void serialize(int agent,int playerid,int consid);
}Notify_UpdateConsortia_Response,*LPNotify_UpdateConsortia_Response;
inline void tagNotify_UpdateConsortia_Response::serialize(int agent,int playerid,int consid)
{
	playerID = playerid;
	consortiaID = consid;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CHARGE,MDM_GP_MAP_CONSORTIA,CC_S_NOTIFY_UPDATECONSORTIA);
}
/*�漰���ʼ��Ľṹ��*/
//�����������
typedef struct tagEmaiBody_Union
{
	int ConsortiaID;		//����1ID
	int PlayerID;			//������ID
}EmailBody_Union,*LPEmailBody_Union;
//����Լս����
typedef struct tagEmailBody_Fight
{
	int		ConsortiaID;				//����ID
	int		ApplyPlayerID;				//������
	int		MapID;						//Լս��
	int		CityID;						//Լս����
	__int64 Time;						//Լսʱ��
}EmailBody_Fight,*LPEmailBody_Fight;
//������빫��
typedef struct tagEmailBody_JoinCon
{
	int PlayerID;			//������ID
	int ConsortiaID;		//�������Ĺ���ID
}EmailBody_JoinCon,*LPEmailBody_JoinCon;

//������빫��
typedef struct tagEmailBody_Invite
{
	int ConsortiaID;		//����ID
	int PlayerID;			//������ID
}EmailBody_Invite,*LPEmailBody_Invite;


#endif;