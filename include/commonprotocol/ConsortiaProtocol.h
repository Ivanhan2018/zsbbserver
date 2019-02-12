#ifndef CONSORTIAPROTOCOL_H__
#define CONSORTIAPROTOCOL_H__

#include <string.h>
#include "Distributed_Header.h"
#include "Global_Macro.h"
#ifndef	MAX_LOGO_SIZE
#	define MAX_LOGO_SIZE			4096	//自定义Logo的最大限制
#endif

#ifdef PRESS_TEST
#define MAX_COUNT_PAGE				1		//每页最大的记录数
#else
#define MAX_COUNT_PAGE				30		//每页最大的记录数
#endif
/****************************公会权限定义********************************/
#define PURVIEW_AGRESS_JOIN			0x000001	//批准加入公会的权限
#define PURVIEW_INVITE_JOIN			0x000002	//邀请的权限
#define PURVIEW_KICK_USER			0x000004	//踢人的权限
#define PURVIEW_OFFICIAL			0x000008	//官职任
#define PURVIEW_MODIFY_PLACARD		0x000010	//修改公告
#define PURVIEW_MODIFY_LOGO			0x000020	//修改LOGO
#define PURVIEW_DISBAND_CON			0x000040	//解散公会
#define PURVIEW_MANAGE_BANK			0x000080	//支配公会银行
#define PURVIEW_MODIFY_TENT			0x000100	//修改公会宗旨
#define PURVIEW_FINANCE				0x000200	//财务权限
#define PURVIEW_WAR					0x000400	//宣战权限
#define PURVIEW_REMOVAL				0x000800	//官职免	
#define PURVIEW_AUTHORIZE			0x001000	//授权权限
#define PURVIEW_UPDATECONSORTIA		0x002000	//升级军团权限
#define PURVIEW_MODIFYSIGNID		0x004000	//修改军团标识
/****************************通讯协议定义********************************/
//军团职位表
enum Official_Type
{
	NORMAL_JOB,				//普通
	CHARIRMAN_JOB,			//军团长
	OFFICIAL_JOB,			//官员
	LEAGUER_JOB				//会员
};
enum Vote_Type
{
	NONUSER_VOTE,			//弃权
	UPHOLD_VOTE,			//赞成票
	OPPOSE_VOTE,			//反对票
	DELATE_VOTE				//弹劾(签名)
};
enum Postion_Type
{
	NORMAL_POSTION,			//无
	MILITARY_POSTION,		//军事委员会
	ECONOMY_POSTION,		//经济委员会
	CONTRIBUTE_POSTION,		//公益委员会
	MINISTER_POSTION,		//部长
	UNDERSECRETARY_POSTION	//副部长
};

enum Dept_Type
{
	BOULE_DEPT,				//众议院
	DOD_DEPT,				//国防部
	INTERIOR_DEPT			//内政部
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
	NORMAL_DEPTSTATE = 0x0,//没有国防部长及内政部长
	DODMINISTER_DEPTSTATE = 0x01,//有国防部长
	INTERIOR_DEPTSTATE = 0x02   //有内政部长
};
//军团称号类型
enum LegionTitle_Type
{
	LOVE_LEGIONTITLE,			//爱心
	EXP_LEGIONTITLE,			//经验
};
typedef struct tagCareerConfig_Item
{
	int	DeptID;			//部门，Dept_Type
	int JobID;
	char JobName[SIZE_32];
}CareerConfig_Item,*LPCareerConfig_Item;

//公会创建CC_C_CREATE_CONSORTIA/CC_S_CREATE_CONSORTIA
typedef struct tagCreate_Consortia_Request:DISTRIBUTED_HEADER
{
	char	ConName[SIZE_32];					//公会名称
	char	Placard[SIZE_256];					//公会公告
	char	Tenet[SIZE_128];					//公会宗旨
	int		FaceID;								//LogoID,；
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
	{//自定义
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
    int		Error_Code;						//操作结果，0：成功；1:重名；2：等级不够；3：资源不足；其他：未知错误
    int		ConsortiaID;					//创建的公会ID；
	inline void serialize(int agent,int playerid,int code,int consortiaid);
}Create_Consortia_Response,*LPCreate_Consortia_Response;
inline void tagCreate_Consortia_Response::serialize(int agent,int playerid,int code,int consortiaid)
{
	Error_Code = code;						
	ConsortiaID = consortiaid;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CONSORTIA,playerid,CC_S_CREATE_CONSORTIA);
}

//公会查询CC_C_QUERY_CONSORTIA/CC_S_QUERY_CONSORTIA
typedef struct tagQuery_Consortia_Request:DISTRIBUTED_HEADER
{
	int		Page;					//页数，0：第一页
	inline void serialize(int playerid,int page);
}Query_Consortial_Request,*LPQuery_Consortial_Request;
inline void tagQuery_Consortia_Request::serialize(int playerid,int page)
{
	Page = page;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,MDM_GP_MAP_CONSORTIA,CC_C_QUERY_CONSORTIA);
}

typedef struct tagConsortia_Info
{
	int		ConsortiaID;					//公会编号
	char	ConName[SIZE_32];				//公会名称
	int		Level;							//公会等级
	int		Count;							//成员数量；
	int		FaceID;							//公会LOGO
	int		ChairmanID;						//会长ID
	char	ChairmanName[SIZE_32];			//会长名称
	char	Tenet[SIZE_128];				//宗旨
	char	IsUnion;						//是否同盟公会
	char	IsFight;						//是否约战公会

	//20100301
	char	IsRoyal;						//是否为皇家军团，0：否；1：是
	int		MaxUserNum;						//人数上限
	int		Money;							//金钱
	int		Exp;							//经验
	int		Credit;							//军团荣誉
	int		SignID;							//军团标志-皇家军团才有

	int     countryID;                      // 阵营ID
}Consortia_Info,*LPConsortia_Info;
typedef struct tagQuery_Consortia_Response:DISTRIBUTED_HEADER
{
	int		Count;							//公会数量,0：无公会；<0：有错误发生；当有公会信息时，该结构体紧跟Count个Consortia_Info
	int		Page;							//当前页
	int		PageCount;						//总页数
	inline void serialize(int length,int agent,int playerid,int count,int page,int pagecount);
}Query_Consortia_Response,*LPQuery_Consortia_Response;
inline void tagQuery_Consortia_Response::serialize(int length,int agent,int playerid,int count,int page,int pagecount)
{
	Count = count;
	Page = page;
	PageCount = pagecount;
	DISTRIBUTED_HEADER::serialize(length,agent,MDM_GP_MAP_CONSORTIA,playerid,CC_S_QUERY_CONSORTIA);
}

//公会解散CC_C_DELETE_CONSORTIA/CC_S_DELETE_CONSORTIA
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
	int Error_Code;					//操作结果，0：成功；1：没有权限，2：不存在；其他：失败
	inline void serialize(int agent,int playerid,int code);
}Delete_Consortia_Response,*LPDelete_Consortia_Response;
inline void tagDelete_Consortia_Response::serialize(int agent,int playerid,int code)
{
	Error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CONSORTIA,playerid,CC_S_DELETE_CONSORTIA);
}

//查询玩家所属公会CC_C_QUERY_USER_CON/CC_S_QUERY_USER_CON
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
	tagConsortia_Info conInfo;				//公会基础信息
	char	Placard[SIZE_256];				//公会公告
	int		ConJob;							//职位编号
	int		Purview;						//权限
	//add 20100301
	int		LegionTitle;					//军团称号
	int		Contribute;						//捐献度
	int		Credit;							//荣誉点
	int		Exp;							//经验值
	int		Money;							//捐献的金钱数
	char	isCanSalaried;					//是否可以领取工资,0:不可以，1：可以
}UserConInfo,*LPUserConInfo;

typedef struct tagQuery_User_Con_Response:DISTRIBUTED_HEADER
{
	int			Error_Code;					//操作结果，0：成功，1：玩家未加入公会；<0:有错误发生
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

//修改公告CC_C_MODIFY_PLACARD/CC_S_MODIFY_PLACARD
typedef struct tagModify_Con_Placard_Request:DISTRIBUTED_HEADER
{
	char		Placard[SIZE_256];			//公告内容
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
	int		Error_Code;							//操作结果，0：成功；1:未加入公会，其他：失败;
	inline void serialize(int agent,int playerid,int code);
}Modify_Con_Placard_Response,*LPModify_Con_Placard_Response;
inline void tagModify_Con_Placard_Response::serialize(int agent,int playerid,int code)
{
	Error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CONSORTIA,playerid,CC_S_MODIFY_PLACARD);
}

//查询LOGO:CC_C_QUERY_LOGO/CC_S_QUERY_LOGO
typedef struct tagQuery_Con_LOGO_Request:DISTRIBUTED_HEADER
{
	int		ConsortiaID;					//公会ID
	inline void serialize(int playerid,int consortiaid);
}Query_Con_LOGO_Request,*LPQuery_Con_LOGO_Request;
inline void tagQuery_Con_LOGO_Request::serialize(int playerid,int consortiaid)
{
	ConsortiaID = consortiaid;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,MDM_GP_MAP_CONSORTIA,CC_C_QUERY_LOGO);
}

typedef struct	tagQuery_Con_LOGO_Response:DISTRIBUTED_HEADER
{
	int		Error_Code;						//查询结果，0：成功；
	int		FaceID;							//LOGO编号
	char	FaceBuf[MAX_LOGO_SIZE];			//LOGO数据
	inline void serialize(int agent,int playerid,int code);
}Query_Con_LOGO_Response,*LPQuery_Con_LOGO_Response;
inline void tagQuery_Con_LOGO_Response::serialize(int agent,int playerid,int code)
{
	Error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CONSORTIA,playerid,CC_S_QUERY_LOGO);
}

//修改LOGO：CC_C_MODIFY_LOGO/CC_S_MODIFY_LOGO
typedef struct tagModify_Con_LOGO_Request:DISTRIBUTED_HEADER
{
	int		FaceID;							//LOGO编号
	char	FaceBuf[MAX_LOGO_SIZE];			//LOGO数据
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
	{//自定义
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
	int		Error_Code;						//操作结果，0：成功；其他：失败；
	inline void serialize(int agent,int playerid,int code);
}Modify_Con_LOGO_Response,*LPModify_Con_LOGO_Response;
inline void tagModify_Con_LOGO_Response::serialize(int agent,int playerid,int code)
{
	Error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CONSORTIA,playerid,CC_S_MODIFY_LOGO);
}

//捐献资源到公会银行CC_C_CONTRIBUTE_BANK/CC_S_CONTRIBUTE_BANK
typedef struct tagContribute_Con_Bank_Request:DISTRIBUTED_HEADER
{
	int		ItemID;						//捐献资源ID;
	int		ItemCount;					//数量
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
	int		Error_Code;					//操作结果，0：成功；其他：失败；
	inline void serialize(int agent,int playerid,int code);
}Contribute_Con_Bank_Response,*LPContribute_Con_Bank_Response;
inline void tagContribute_Con_Bank_Response::serialize(int agent,int playerid,int code)
{
	Error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CONSORTIA,playerid,CC_S_CONTRIBUTE_BANK);
}

//支出公会资源CC_C_PAYOUT_BANK/CC_S_PAYOUT_BANK(暂不实现)
typedef struct tagPayout_Con_Bank_Request:DISTRIBUTED_HEADER
{
	int		ItemID;						//捐献资源ID;
	int		ItemCount;					//数量
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
	int		Error_Code;					//操作结果，0：成功；其他：失败
	inline void serialize(int agent,int playerid,int code);
}Payout_Con_Bank_Response,*LPPayout_Con_Bank_Response;
inline void tagPayout_Con_Bank_Response::serialize(int agent,int playerid,int code)
{
	Error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CONSORTIA,playerid,CC_S_PAYOUT_BANK);
}

//公会银行查询CC_C_QUERY_BANK/CC_S_QUERY_BANK
typedef struct tagQuery_Con_Bank_Request:DISTRIBUTED_HEADER
{
	int		Page;						//页数，0：第一页
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
	int		Count;						//返回记录数,该结构后紧跟的是Count个Bank_ItemInfo；0：无资源信息；<0:有错误发生
	int		Page;						//当前页
	int		PageCount;					//总页数
	inline void serialize(int length,int agent,int playerid,int count,int page,int pagecount);
}Query_Con_Bank_Response,*LPQuery_Con_Bank_Response;
inline void tagQuery_Con_Bank_Response::serialize(int length,int agent,int playerid,int count,int page,int pagecount)
{
	Count = count;
	Page = page;
	PageCount = pagecount;
	DISTRIBUTED_HEADER::serialize(length,agent,MDM_GP_MAP_CONSORTIA,playerid,CC_S_QUERY_BANK);
}

//结盟申请CC_C_ALLY_APPLY_CON/CC_S_ALLY_APPLY_CON
typedef struct tagAlly_Apply_Con_Request:DISTRIBUTED_HEADER
{
	int		AllyConsortiaID;			//预结盟的公会ID
	inline void serialize(int playerid,int allyconsortiaid);
}Ally_Apply_Con_Request,*LPAlly_Apply_Con_Request;
inline void tagAlly_Apply_Con_Request::serialize(int playerid,int allyconsortiaid)
{
	AllyConsortiaID = allyconsortiaid;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,MDM_GP_MAP_CONSORTIA,CC_C_ALLY_APPLY_CON);
}

typedef struct tagAlly_Apply_Con_Response:DISTRIBUTED_HEADER
{
	int		Error_Code;					//操作结果，0：成功；1：玩家不是会长；2:对方公会不存在；3:本公会无法结盟；其他：失败
	inline void serialize(int agent,int playerid,int code);
}Ally_Apply_Con_Response,*LPAlly_Apply_Con_Response;
inline void tagAlly_Apply_Con_Response::serialize(int agent,int playerid,int code)
{
	Error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CONSORTIA,playerid,CC_S_ALLY_APPLY_CON);
}

//同意结盟CC_C_ALLY_CONSORTIA_AGREE/CC_S_ALLY_CONSORTIA_AGREE
typedef struct tagAlly_Consortia_Agree_Request:DISTRIBUTED_HEADER
{
	int		ConsortiaID;				//公会ID
	int		ApplyPlayerID;				//申请人ID
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
	int Error_Code;						//操作结果，0：成功；1:不是会长;2:已是同盟；3：同一公会；其他：失败
	inline void serialize(int agent,int playerid,int code);
}Ally_Consortia_Agree_Response,*LPAlly_Consortia_Agree_Response;
inline void tagAlly_Consortia_Agree_Response::serialize(int agent,int playerid,int code)
{
	Error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CONSORTIA,playerid,CC_S_ALLY_CONSORTIA_AGREE);
}

//拒绝结盟CC_C_ALLY_CONSORTIA_REFUSE/CC_S_ALLY_CONSORTIA_REFUSE
typedef struct tagAlly_Consortia_Refuse_Request:DISTRIBUTED_HEADER
{
	int		ConsortiaID;				//公会ID
	int		ApplyPlayerID;				//申请人ID
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
	int Error_Code;						//操作结果，0：成功；其他：失败
	inline void serialize(int agent,int playerid,int code);
}Ally_Consortia_Refuse_Response,*LPAlly_Consortia_Refuse_Response;
inline void tagAlly_Consortia_Refuse_Response::serialize(int agent,int playerid,int code)
{
	Error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CONSORTIA,playerid,CC_S_ALLY_CONSORTIA_REFUSE);
}

//解盟CC_C_UNALLY_CONSORTIA/CC_S_UNALLY_CONSORTIA
typedef struct tagUnally_Consortia_Request:DISTRIBUTED_HEADER
{
	int		ConsortiaID;				//公会ID
	inline void serialize(int playerid,int consortiaid);
}Unally_Consortia_Request,*LPUnally_Consortia_Request;
inline void tagUnally_Consortia_Request::serialize(int playerid,int consortiaid)
{
	ConsortiaID = consortiaid;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,MDM_GP_MAP_CONSORTIA,CC_C_UNALLY_CONSORTIA);
}

typedef struct tagUnally_Consortia_Response:DISTRIBUTED_HEADER
{
	int Error_Code;						//操作结果，0：成功；1:没有权限；2：不是同盟公会；其他：失败
	inline void serialize(int agent,int playerid,int code);
}Unally_Consortia_Response,*LPUnally_Consortia_Response;
inline void tagUnally_Consortia_Response::serialize(int agent,int playerid,int code)
{
	Error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CONSORTIA,playerid,CC_S_UNALLY_CONSORTIA);
}

//约战申请CC_C_FIGHT_APPLY_CON/CC_S_FIGHT_APPLY_CON	
typedef struct tagFight_Apply_Con_Request:DISTRIBUTED_HEADER
{
	int		AllyConsortiaID;			//预约战的公会ID
	int		MapID;						//约战郡
	int		CityID;						//约战城市
	__int64 Time;						//约战时间
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
	int		Error_Code;					//操作结果，0：成功；其他：失败
	inline void serialize(int agent,int playerid,int code);
}Fight_Apply_Con_Response,*LPFight_Apply_Con_Response;
inline void tagFight_Apply_Con_Response::serialize(int agent,int playerid,int code)
{
	Error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CONSORTIA,playerid,CC_S_FIGHT_APPLY_CON);
}

//同意约战CC_C_FIGHT_CONSORTIA_AGREE/CC_S_FIGHT_CONSORTIA_AGREE
typedef struct tagFight_Consortia_Agree_Request:DISTRIBUTED_HEADER
{
	int		ConsortiaID;				//公会ID
	int		ApplyPlayerID;				//申请人
	int		MapID;						//约战郡
	int		CityID;						//约战城市
	__int64 Time;						//约战时间
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
	int Error_Code;						//操作结果，0：成功；其他：失败
	inline void serialize(int agent,int playerid,int code);
}Fight_Consortia_Agree_Response,*LPFight_Consortia_Agree_Response;
inline void tagFight_Consortia_Agree_Response::serialize(int agent,int playerid,int code)
{
	Error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CONSORTIA,playerid,CC_S_FIGHT_CONSORTIA_AGREE);
}

//拒绝约战CC_C_FIGHT_CONSORTIA_REFUSE/CC_S_FIGHT_CONSORTIA_REFUSE
typedef struct tagFight_Consortia_Refuse_Request:DISTRIBUTED_HEADER
{
	int		ConsortiaID;				//公会ID
	int		ApplyPlayerID;				//申请人
	int		MapID;						//约战郡
	int		CityID;						//约战城市
	__int64 Time;						//约战时间
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
	int Error_Code;						//操作结果，0：成功；其他：失败
	inline void serialize(int agent,int playerid,int code);
}Fight_Consortia_Refuse_Response,*LPFight_Consortia_Refuse_Response;
inline void tagFight_Consortia_Refuse_Response::serialize(int agent,int playerid,int code)
{
	Error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CONSORTIA,playerid,CC_S_FIGHT_CONSORTIA_REFUSE);
}
//约战查询CC_C_QUERY_FIGHT/CC_S_QUERY_FIGHT
typedef struct tagQuery_Con_Fight_Request:DISTRIBUTED_HEADER
{
	int		Page;						//查询页数
	inline void serialize(int playerid,int page);
}Query_Con_Fight_Request,*LPQuery_Con_Fight_Request;
inline void tagQuery_Con_Fight_Request::serialize(int playerid,int page)
{
	Page = page;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,MDM_GP_MAP_CONSORTIA,CC_C_QUERY_FIGHT);
}

typedef struct tagCon_FightInfo
{
	int		FightID;				//约战ID
	int		Consortia1ID;			//
	char	Consortia1Name[SIZE_32];//		
	int		Consortia2ID;			//
	char	Consortia2Name[SIZE_32];//		
	int		MapID;					//开战的郡ID  开战地点
	int		CityID;					//开战的城市ID
	int		Time;					//开战时间
}Con_FightInfo,*LPCon_FightInfo;

typedef struct tagQuery_Con_Fight_Response:DISTRIBUTED_HEADER
{
	int		Count;					//约战记录个数，该结构后紧跟Count个Con_FightInfo结构体，0：无约战信息；<0：有错误发生
	int		Page;
	int		PageCount;				//总页数	
	inline void serialize(int length,int agent,int playerid,int count,int page,int pagecount);
}Query_Con_Fight_Response,*LPQuery_Con_Fight_Response;
inline void tagQuery_Con_Fight_Response::serialize(int length,int agent,int playerid,int count,int page,int pagecount)
{
	Count = count;
	Page = page;
	PageCount = pagecount;
	DISTRIBUTED_HEADER::serialize(length,agent,MDM_GP_MAP_CONSORTIA,playerid,CC_S_QUERY_FIGHT);
}

//申请加入公会CC_C_JOIN_APPLY_CON/CC_S_JOIN_APPLY_CON
typedef struct tagJoin_Apply_Con_Request:DISTRIBUTED_HEADER
{
	int		ConsortiaID;			//预加入的公会ID
	inline void serialize(int playerid,int consortiaid);
}Join_Apply_Con_Request,*LPJoin_Apply_Con_Request;
inline void tagJoin_Apply_Con_Request::serialize(int playerid,int consortiaid)
{
	ConsortiaID = consortiaid;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,MDM_GP_MAP_CONSORTIA,CC_C_JOIN_APPLY_CON);
}

typedef struct tagJoin_Apply_Con_Response:DISTRIBUTED_HEADER
{
	int		Error_Code;				//操作结果，0：成功；1等级不够；2，不属于同一阵营，无法加入；8:人数已满；其他：失败
	inline void serialize(int agent,int playerid,int code);
}Join_Apply_Con_Response,*LPJoin_Apply_Con_Response;
inline void tagJoin_Apply_Con_Response::serialize(int agent,int playerid,int code)
{
	Error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CONSORTIA,playerid,CC_S_JOIN_APPLY_CON);
}

//会长同意加入工会CC_C_JOIN_CONSORTIA_AGREE/CC_S_JOIN_CONSORTIA_AGREE
typedef struct tagJoin_Consortia_Agree_Request:DISTRIBUTED_HEADER
{
	int		PlayerID;					//待加入公会的玩家ID
	inline void serialize(int playerid,int joinplayer);
}Join_Consortia_Agree_Request,*LPJoin_Consortia_Agree_Request;
inline void tagJoin_Consortia_Agree_Request::serialize(int playerid,int joinplayer)
{
	PlayerID = joinplayer;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,MDM_GP_MAP_CONSORTIA,CC_C_JOIN_CONSORTIA_AGREE);
}

typedef struct tagJoin_Consortia_Agree_Response:DISTRIBUTED_HEADER
{
	int		Error_Code;					//操作结果，0：成功；1,已加入公会；2:没有权限；8:人数已满；其他：失败
	inline void serialize(int agent,int playerid,int code);
}Join_Consortia_Agree_Response,*LPJoin_Consortia_Agree_Response;
inline void tagJoin_Consortia_Agree_Response::serialize(int agent,int playerid,int code)
{
	Error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CONSORTIA,playerid,CC_S_JOIN_CONSORTIA_AGREE);
}

//会长拒绝加入工会CC_C_JOIN_CONSORTIA_REFUSE/CC_S_JOIN_CONSORTIA_REFUSE
typedef struct tagJoin_Consortia_Refuse_Request:DISTRIBUTED_HEADER
{
	int		PlayerID;					//待加入公会的玩家ID
	inline void serialize(int playerid,int joinplayer);
}Join_Consortia_Refuse_Request,*LPJoin_Consortia_Refuse_Request;
inline void tagJoin_Consortia_Refuse_Request::serialize(int playerid,int joinplayer)
{
	PlayerID = joinplayer;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,MDM_GP_MAP_CONSORTIA,CC_C_JOIN_CONSORTIA_REFUSE);
}

typedef struct tagJoin_Consortia_Refuse_Response:DISTRIBUTED_HEADER
{
	int		Error_Code;					//操作结果，0：成功；其他：失败
	inline void serialize(int agent,int playerid,int code);
}Join_Consortia_Refuse_Response,*LPJoin_Consortia_Refuse_Response;
inline void tagJoin_Consortia_Refuse_Response::serialize(int agent,int playerid,int code)
{
	Error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CONSORTIA,playerid,CC_S_JOIN_CONSORTIA_REFUSE);
}
//公会成员查询CC_C_QUERY_CONUSER/CC_S_QUERY_CONUSER
typedef struct tagQuery_Con_User_Request:DISTRIBUTED_HEADER
{
	int		Page;						//页数
	inline void serialize(int playerid,int page);
}Query_Con_User_Request,*LPQuery_Con_User_Request;
inline void tagQuery_Con_User_Request::serialize(int playerid,int page)
{
	Page = page;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,MDM_GP_MAP_CONSORTIA,CC_C_QUERY_CONUSER);
}
//ID、名称、等级、种族、职系、经验值、军衔、荣誉点、贡献度、公会职位、人员类型
typedef struct tagCon_UserInfo
{
	int		PlayerID;
	char	Name[SIZE_24];
	int		Level;						//级别
	int		OfficerRace;				//种族
	int		JobID;						//职系
	int		Exp;						//经验值
	int		MilitaryRank;				//军衔
	int		Credit;						//荣誉点
	int		Contribute;					//贡献度
	int		ConJob;						//公会职位编码
	char	IsOnline;					//是否在线
	int		LegionTitle;				//军团称号
	int		Purview;					//权限
}Con_UserInfo,*LPCon_UserInfo;

typedef struct tagQuery_Con_User_Response:DISTRIBUTED_HEADER
{
	int		Count;						//成员数量，该结构体后紧跟Count个Con_UserInfo，0：无成员；<0：有错误发生(-1:没有加入公会）
	int		Page;						//当前页
	int		PageCount;					//总页数
	inline void serialize(int length,int agent,int playerid,int count,int page,int pagecount);
}Query_Con_User_Response,*LPQuery_Con_User_Response;
inline void tagQuery_Con_User_Response::serialize(int length,int agent,int playerid,int count,int page,int pagecount)
{
	Count = count;
	Page = page;
	PageCount = pagecount;
	DISTRIBUTED_HEADER::serialize(length,agent,MDM_GP_MAP_CONSORTIA,playerid,CC_S_QUERY_CONUSER);
}

//Kick成员CC_C_KICK_CONUSER/CC_S_KICK_CONUSER
typedef struct Kick_ConUser_Request:DISTRIBUTED_HEADER
{
	int		KickPlayerID;				//被Kick玩家ID
	inline void serialize(int playerid,int kickid);
}Kick_ConUser_Request,*LPKick_ConUser_Request;
inline void Kick_ConUser_Request::serialize(int playerid,int kickid)
{
	KickPlayerID = kickid;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,MDM_GP_MAP_CONSORTIA,CC_C_KICK_CONUSER);
}

typedef struct Kick_ConUser_Response:DISTRIBUTED_HEADER
{
	int		Error_Code;					//操作结果，0：成功；1:没有权限;2:不存在该成员；3：无法Kick会长；其他：失败
	inline void serialize(int agent,int playerid,int code);
}Kick_ConUser_Response,*LPKick_ConUser_Response;
inline void Kick_ConUser_Response::serialize(int agent,int playerid,int code)
{
	Error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CONSORTIA,playerid,CC_S_KICK_CONUSER);
}

//退出公会CC_C_QUIT_CONSORTIA/CC_S_QUIT_CONSORTIA
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
	int		Error_Code;					//操作结果，0：成功；1:会长无法退出；其他：失败
	inline void serialize(int agent,int playerid,int code);
}Quit_Consortia_Response,*LPQuit_Consortia_Response;
inline void tagQuit_Consortia_Response::serialize(int agent,int playerid,int code)
{
	Error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CONSORTIA,playerid,CC_S_QUIT_CONSORTIA);
}

//邀请加入公会CC_C_INVITE_JOIN_CON/CC_S_INVITE_JOIN_CON
typedef struct tagInvite_JoinCon_Request:DISTRIBUTED_HEADER
{
	char	InvitedName[SIZE_32];			//被邀请人
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
	int		Error_Code;						//操作结果，0：成功；1:被邀请人已加入公会，2：不属于同一阵营；3:没有邀请人的权限，4:被邀请人级别不够；8:人数已满；其他：失败
	inline void serialize(int agent,int playerid,int code);
}Invite_JoinCon_Response,*LPInvite_JoinCon_Response;
inline void tagInvite_JoinCon_Response::serialize(int agent,int playerid,int code)
{
	Error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CONSORTIA,playerid,CC_S_INVITE_JOIN_CON);
}
//同意邀请加入工会CC_C_INVITE_JOIN_AGREE/CC_S_INVITE_JOIN_AGREE	
typedef struct tagInvite_JoinCon_Agree_Request:DISTRIBUTED_HEADER
{
	int		ConsortiaID;				//公会ID
	int		InviteID;					//邀请人
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
	int		Error_Code;					//操作结果，0：成功；1:已加入公会；2:等级不够；8:人数已满；其他：失败
	inline void serialize(int agent,int playerid,int code);
}Invite_JoinCon_Agree_Response,*LPInvite_JoinCon_Agree_Response;
inline void tagInvite_JoinCon_Agree_Response::serialize(int agent,int playerid,int code)
{
	Error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CONSORTIA,playerid,CC_S_INVITE_JOIN_AGREE);
}

//拒绝邀请加入工会CC_C_INVITE_JOIN_REFUSE/CC_S_INVITE_JOIN_REFUSE
typedef struct tagInvite_JoinCon_Refuse_Request:DISTRIBUTED_HEADER
{
	int		InviteID;					//邀请人
	inline void serialize(int playerid,int inviteid);
}Invite_JoinCon_Refuse_Request,*LPInvite_JoinCon_Refuse_Request;
inline void tagInvite_JoinCon_Refuse_Request::serialize(int playerid,int inviteid)
{
	InviteID = inviteid;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,MDM_GP_MAP_CONSORTIA,CC_C_INVITE_JOIN_REFUSE);
}

typedef struct tagInvite_JoinCon_Refuse_Response:DISTRIBUTED_HEADER
{
	int		Error_Code;					//操作结果，0：成功；其他：失败
	inline void serialize(int agent,int playerid,int code);
}Invite_JoinCon_Refuse_Response,*LPInvite_JoinCon_Refuse_Response;
inline void tagInvite_JoinCon_Refuse_Response::serialize(int agent,int playerid,int code)
{
	Error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CONSORTIA,playerid,CC_S_INVITE_JOIN_REFUSE);
}
//任职CC_C_POSE_OFFICIAL/CC_S_POSE_OFFICIAL
typedef struct tagPose_ConOfficial_Request:DISTRIBUTED_HEADER
{
	int		PlayerID;					//玩家ID
	int		ConJob;						//职位编号
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
	int		Error_Code;					//操作结果，0：成功；1:无权限；2：无法任免高级别的职位；3,官员已满;其他：失败
	inline void serialize(int agent,int playerid,int code);
}Pose_ConOfficial_Response,*LPPose_ConOfficial_Response;
inline void tagPose_ConOfficial_Response::serialize(int agent,int playerid,int code)
{
	Error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CONSORTIA,playerid,CC_S_POSE_OFFICIAL);
}

//免职CC_C_DEPOSE_OFFICIAL/CC_S_DEPOSE_OFFICIAL
typedef struct tagDepose_ConOfficial_Request:DISTRIBUTED_HEADER
{
	int		PlayerID;						//被免职玩家ID
	inline void serialize(int playerid,int depose_playerid);
}Depose_ConOfficial_Request,*LPDepose_ConOfficial_Request;
inline void tagDepose_ConOfficial_Request::serialize(int playerid,int depose_playerid)
{
	PlayerID = depose_playerid;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,MDM_GP_MAP_CONSORTIA,CC_C_DEPOSE_OFFICIAL);
}

typedef struct tagDepose_ConOfficial_Response:DISTRIBUTED_HEADER
{
	int		Error_Code;				//操作结果，0：成功；其他：失败
	inline void serialize(int agent,int playerid,int code);
}Depose_ConOfficial_Response,*LPDepose_ConOfficial_Response;
inline void tagDepose_ConOfficial_Response::serialize(int agent,int playerid,int code)
{
	Error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CONSORTIA,playerid,CC_S_DEPOSE_OFFICIAL);
}

//查询职位CC_C_QUERY_OFFICIAL/CC_S_QUERY_OFFICIAL
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
	int			OfficialID;				//职位编号
	char		OfficialName[SIZE_16];	//职位名称
	int			Purview;				//权限值(按位异或)
	short		Level;					//职位级别（0：级别最高，会长）
}ConOfficial_Info,*LPConOfficial_Info;
typedef struct tagQuery_ConOfficial_Response:DISTRIBUTED_HEADER
{
	int		Count;						//职位数量，该结构后紧跟Count个ConOfficial_Info，0:无职位信息；<0：有错误发生
	int		Page;
	int		PageCount;					//页总数
	inline void serialize(int length,int agent,int playerid,int count,int page,int pagecount);
}Query_ConOfficial_Response,*LPQuery_ConOfficial_Response;
inline void tagQuery_ConOfficial_Response::serialize(int length,int agent,int playerid,int count,int page,int pagecount)
{
	Count = count;
	Page = page;
	PageCount = pagecount;
	DISTRIBUTED_HEADER::serialize(length,agent,MDM_GP_MAP_CONSORTIA,playerid,CC_S_QUERY_OFFICIAL);
}

//查询创建公会的限制条件CC_C_QUERY_CREATELIMIT/CC_S_QUERY_CREATELIMIT
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
	int			Level;						//创建公会的等级要求
	int			JoinLevel;					//加入公会的等级要求 
	int			Money;						//金钱
	int			FoodCount;					//粮食数量
	int			WoodCount;					//木材数量
	int			IronCount;					//铁数量
	int			SkinCount;					//皮数量
	int			RedStoneCount;				//红宝石
	int			YollowStoneCount;			//黄宝石
	int			GreenStoneCount;			//绿宝石
	int			BackIronCount;				//黑铁
	int			Personnel;					//人员数（创建24小时后，成员的数量要求）
	int			Times;						//连续多长时间（小时）人员未满Personnel人时，解散公会
}CreateConLimitInfo,*LPCreateConLimitInfo;


typedef struct tagQuery_CreateConLimit_Response:DISTRIBUTED_HEADER
{
	int						Error_Code;		//操作结果，0：成功；其他：失败
	CreateConLimitInfo		LimitInfo;		//
	inline void serialize(int agent,int playerid,int code);
}Query_CreateConLimit_Response,*LPQuery_CreateConLimit_Response;
inline void tagQuery_CreateConLimit_Response::serialize(int agent,int playerid,int code)
{
	Error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CONSORTIA,playerid,CC_S_QUERY_CREATELIMIT);
}

//修改公会宗旨CC_C_MODIFY_CONTENET/CC_S_MODIFY_CONTENET
typedef struct tagModify_ConTenet_Request:DISTRIBUTED_HEADER
{
	char Tenet[SIZE_128];						//宗旨
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
	int						Error_Code;		//操作结果，0：成功；其他：失败
	inline void serialize(int agent,int playerid,int code);
}Modify_ConTenet_Response,*LPModify_ConTenet_Response;
inline void tagModify_ConTenet_Response::serialize(int agent,int playerid,int code)
{
	Error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CONSORTIA,playerid,CC_S_MODIFY_CONTENET);
}

//客户端登陆CC_C_CLIENTLOGIN/CC_S_CLIENTLOGIN
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
	int Error_Code;							//0：成功；其他：失败
	inline void serialize(int agent,int plyaerid,int coce);
}ClientLogin_Response,*LPClientLogin_Response;
inline void tagClientLogin_Response::serialize(int agent,int playerid,int code)
{
	Error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CONSORTIA,playerid,CC_S_CLIENTLOGIN);
}
//转让会长CC_C_TRANS_CHAIRMAN/CC_S_TRANS_CHAIRMAN
typedef struct tagTrans_Chairman_Request:DISTRIBUTED_HEADER
{
	int PlayerID;						//下任会长
	inline void serialize(int playerid,int next_chairman);
}Trans_Chairman_Request,*LPTrans_Chairman_Request;
inline void tagTrans_Chairman_Request::serialize(int playerid,int next_chairman)
{
	PlayerID = next_chairman;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,MDM_GP_MAP_CONSORTIA,CC_C_TRANS_CHAIRMAN);
}

typedef struct tagTrans_Chairman_Response:DISTRIBUTED_HEADER
{
	int Error_Code;					//0:成功；1：未加入公会；2：不是会长，无法转交；3：无法转交给非本公会的成员；其他：失败
	inline void serialize(int agent,int plyaerid,int code);
}Trans_Chairman_Response,*LPTrans_Chairman_Response;
inline void tagTrans_Chairman_Response::serialize(int agent,int playerid,int code)
{
	Error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CONSORTIA,playerid,CC_S_TRANS_CHAIRMAN);
}

//公会排行CC_C_CON_RANKING/CC_S_CON_RANKING
typedef struct tagQuery_Con_Ranking_Request:DISTRIBUTED_HEADER
{
	int Page;							//页数
	inline void serialize(int playerid,int page);
}Query_Con_Ranking_Request,*LPQuery_Con_Ranking_Request;
inline void tagQuery_Con_Ranking_Request::serialize(int playerid,int page)
{
	Page = page;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,MDM_GP_MAP_CONSORTIA,CC_C_CON_RANKING);
}

typedef struct tagConsortia_RankingInfo
{
	int		RankingID;						//排行序号
	char	ConName[SIZE_32];				//公会名称
	char	Chairman[SIZE_24];				//会长名称
	int		Level;							//公会等级
	int		Count;							//成员数量, 主排名字段
	int		Country;						//阵营
	__int64 CreateTime;						//创建时间
}Consortia_RankingInfo,*LPConsortia_RankingInfo;

typedef struct tagQuery_Con_Ranking_Response:DISTRIBUTED_HEADER
{
	int		Count;						//职位数量，该结构后紧跟Count个Consortia_RankingInfo，0:无排行信息；<0：有错误发生
	int		Page;
	int		PageCount;					//页总数
	inline void serialize(int length,int agent,int playerid,int count,int page,int pagecount);
}Query_Con_Ranking_Response,*LPQuery_Con_Ranking_Response;
inline void tagQuery_Con_Ranking_Response::serialize(int length,int agent,int playerid,int count,int page,int pagecount)
{
	Count = count;
	Page = page;
	PageCount = pagecount;
	DISTRIBUTED_HEADER::serialize(length,agent,MDM_GP_MAP_CONSORTIA,playerid,CC_S_CON_RANKING);
}

//个人排行CC_C_PLAYER_RANKING/CC_S_PLAYER_RANKING
typedef struct tagQuery_Player_Ranking_Request:DISTRIBUTED_HEADER
{
	int Page;							//页数
	inline void serialize(int playerid,int page);
}Query_Player_Ranking_Request,*LPQuery_Player_Ranking_Request;
inline void tagQuery_Player_Ranking_Request::serialize(int playerid,int page)
{
	Page = page;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,MDM_GP_MAP_CONSORTIA,CC_C_PLAYER_RANKING);
}

typedef struct tagPlayer_RankingInfo
{
	int		RankingID;					//排行
	char	Name[SIZE_24];				//玩家名称
	int		Level;						//等级
	int		MilitaryRank;				//军衔
	int		Money;						//金钱
	int		ResourceCount;				//资源数量
	int		CityCount;					//据点数
	int		GeneralCount;				//杀敌将次数（俘虏次数）
	int		Credit;						//荣誉点
	int		Country;					//阵营
	short	OfficerRace;				//种族
	char	IsOnline;					//是否在线
}Player_RankingInfo,*LPPlayer_RankingInfo;
typedef struct tagQuery_Player_Ranking_Response:DISTRIBUTED_HEADER
{
	int		Count;						//职位数量，该结构后紧跟Count个Player_RankingInfo，0:无排行信息；<0：有错误发生
	int		Page;
	int		PageCount;					//页总数
	inline void serialize(int length,int agent,int playerid,int count,int page,int pagecount);
}Query_Player_Ranking_Response,*LPQuery_Player_Ranking_Response;
inline void tagQuery_Player_Ranking_Response::serialize(int length,int agent,int playerid,int count,int page,int pagecount)
{
	Count = count;
	Page = page;
	PageCount = pagecount;
	DISTRIBUTED_HEADER::serialize(length,agent,MDM_GP_MAP_CONSORTIA,playerid,CC_S_PLAYER_RANKING);
}

//每周新人排行CC_C_NEWPLAYER_RANKING/CC_S_NEWPLAYER_RANKING
typedef struct tagQuery_NewPlayer_Ranking_Request:DISTRIBUTED_HEADER
{
	int Page;							//页数
	inline void serialize(int playerid,int page);
}Query_NewPlayer_Ranking_Request,*LPQuery_NewPlayer_Ranking_Request;
inline void tagQuery_NewPlayer_Ranking_Request::serialize(int playerid,int page)
{
	Page = page;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,MDM_GP_MAP_CONSORTIA,CC_C_NEWPLAYER_RANKING);
}

typedef struct tagNewPlayer_RankingInfo
{
	int		RankingID;					//排行
	char	Name[SIZE_24];				//玩家名称
	int		Level;						//等级
	int		MilitaryRank;				//军衔
	int		Money;						//金钱
	int		ResourceCount;				//资源数量
	int		Country;					//阵营
}NewPlayer_RankingInfo,*LPNewPlayer_RankingInfo;
typedef struct tagQuery_NewPlayer_Ranking_Response:DISTRIBUTED_HEADER
{
	int		Count;						//职位数量，该结构后紧跟Count个NewPlayer_RankingInfo，0:无排行信息；<0：有错误发生
	int		Page;
	int		PageCount;					//页总数
	inline void serialize(int length,int agent,int playerid,int count,int page,int pagecount);
}Query_NewPlayer_Ranking_Response,*LPQuery_NewPlayer_Ranking_Response;
inline void tagQuery_NewPlayer_Ranking_Response::serialize(int length,int agent,int playerid,int count,int page,int pagecount)
{
	Count = count;
	Page = page;
	PageCount = pagecount;
	DISTRIBUTED_HEADER::serialize(length,agent,MDM_GP_MAP_CONSORTIA,playerid,CC_S_NEWPLAYER_RANKING);
}

//有种男战场排行 人气型CC_C_FRIENDS_COUNT/CC_S_FRIENDS_COUNT
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
	int RankingID;				//排行
	char PlayerName[SIZE_24];	//玩家名称
	int Integral;				//积分
	int Credit;					//个人荣誉点
	int FriendsCount;			//好友数量，主排名字段
	int ConName[SIZE_32];		//公会名称
	int Count;					//公会成员数量
	int Country;				//阵营
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

//有种男战场排行 巨商型CC_C_MONEY_COUNT/CC_S_MONEY_COUNT
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
	int RankingID;				//排行
	char PlayerName[SIZE_24];	//玩家名称
	int Integral;				//积分
	int ConCredit;				//公会荣誉
	int PlayerCredit;			//个人荣誉
	int Money;					//金钱,主排名字段
	int Country;				//阵营
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

//有种男战场排行 策略型CC_C_GENERAL_CAPTIVE/CC_S_GENERAL_CAPTIVE
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
	int RankingID;				//排行
	char PlayerName[SIZE_24];	//玩家名称
	int Level;					//等级
	int Integral;				//积分
	int CityCount;				//据点数
	int GeneralCount;			//杀敌将次数（俘虏次数）,主排名字段
	int ResourceCount;			//资源数量
	int Country;				//阵营
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

//检测公会是否24小时人数未满CC_C_CHECKCONSORTIA,	
typedef struct tagConsortia_Timer:DISTRIBUTED_HEADER
{
	inline void serialize(int cmd/* = CC_C_CHECKCONSORTIA*/,int playerid);
}Consortia_Timer,*LPConsortia_Timer;
inline void tagConsortia_Timer::serialize(int cmd,int playerid)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,MDM_GP_MAP_CONSORTIA,cmd);
}

//查询玩家在国家的信息CC_C_QUERY_USEROFCOUNTRY,	CC_S_QUERY_USEROFCOUNTRY,
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
	short Error_Code;			//0:查询成功；其他：失败；
	short Type;					//类型，参看枚举Dept_Type
	short Postion;				//职位，参看枚举Postion_Type
	unsigned int Treasury;		//国库资金
	unsigned int DODMoney;		//国防部资金
	unsigned int FinanceMoney;	//内政部资金
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

//查询本阵营的议员列表，包括军事委员会、经济委员会、公益委员会CC_C_QUERY_COUNCILLIST,CC_S_QUERY_COUNCILLIST,
typedef struct tagQuery_CouncilList_Request:DISTRIBUTED_HEADER
{
	short Type;					//类型，参看Dept_Type.-1：所有议员+国防部+内政部；
	inline void serialize(int playerid,short type);
}Query_CouncilList_Request,*LPQuery_CouncilList_Request;
inline void tagQuery_CouncilList_Request::serialize(int playerid,short type)
{
	Type = type;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,MDM_GP_MAP_CONSORTIA,CC_C_QUERY_COUNCILLIST);
}

typedef struct tagCouncil_Info
{
	int		PlayerID;					//议员ID
	char	Name[SIZE_24];				//议员名称
	short   Level;						//级别
	short   MilitaryRank;				//军衔
	short	Type;						//类型，参看Dept_Type
	short	Postion;					//职位，参看枚举Postion_Type
}Council_Info,*LPCouncil_Info;
typedef struct tagQuery_CouncilList_Response:DISTRIBUTED_HEADER
{
	int Count;							//数量
	int CountryID;						//阵营ID
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
//参加竞选（必须为议员）CC_C_TAKEPARTIN_ELECTION,CC_S_TAKEPARTIN_ELECTION,
typedef struct tagTakePartIn_Election_Request:DISTRIBUTED_HEADER
{
	short Dept;					//参选职位，如国防部、内政部，参看Dept_Type
	char Creed[100];			//纲领
	int Money;			//参选费用
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
	short Error_Code;				//0：成功；1:GO点不够；2：金钱不够；3:非议员;4:已参加过竞选；其他：失败
	inline void serialize(int agent,int playerid,short code);
}TakePartIn_Election_Response,*LPTakePartIn_Election_Response;
inline void tagTakePartIn_Election_Response::serialize(int agent,int playerid,short code)
{
	Error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CONSORTIA,playerid,CC_S_TAKEPARTIN_ELECTION);
}

//查询候选人列表(国防部、内政部）CC_C_QUERY_CANDIDATELIST,CC_S_QUERY_CANDIDATELIST,
typedef struct tagQuery_CandidateList_Request:DISTRIBUTED_HEADER
{
	short Type;					//查询部门，具体参看Dept_Type
	inline void serialize(int playerid,short type);
}Query_CandidateList_Request,*LPQuery_CandidateList_Request;
inline void tagQuery_CandidateList_Request::serialize(int playerid,short type)
{
	Type = type;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,MDM_GP_MAP_CONSORTIA,CC_C_QUERY_CANDIDATELIST);
}
typedef struct tagCandidate_Info
{
	int				PlayerID;				//候选人ID
	char			Name[SIZE_24];			//候选人名称
	short			Level;					//级别
	short			MilitaryRank;				//军衔
	short			Type;					//类型，参看Dept_Type:国防部、内政部
	char			Creed[100];				//参选纲领（或弹劾理由）
	unsigned int	Expense;				//参选费用
	short			Vote;					//得票数
}Candidate_Info,*LPCandidate_Info;
typedef struct tagQuery_CandidateList_Response:DISTRIBUTED_HEADER
{
	int Count;
	int Times;								//选举剩余时间
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

//玩家进行投票选举CC_C_PLAYER_BALLOT,	CC_S_PLAYER_BALLOT,
typedef struct tagPlayer_Ballot_Request:DISTRIBUTED_HEADER
{
	int	Candidate;			//候选人ID
	inline void serialize(int playerid,int candiate);
}Player_Ballot_Request,*LPPlayer_Ballot_Request;
inline void tagPlayer_Ballot_Request::serialize(int playerid,int candiate)
{
	Candidate = candiate;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,MDM_GP_MAP_CONSORTIA,CC_C_PLAYER_BALLOT);
}

typedef struct tagPlayer_Ballot_Response:DISTRIBUTED_HEADER
{
	short Error_Code;		//0:成功；1:候选人未参选；2:已投过票，不能重复投票；其他：失败
	int	Candidate;			//候选人ID
	int Type;				//类型，参看:Dept_Type
	int Vote;				//目前得票数
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

//弹劾CC_C_COUNTRY_DELATE,CC_S_COUNTRY_DELATE,
typedef struct tagCountry_Delate_Request:DISTRIBUTED_HEADER
{
	short Type;					//弹劾类型，参看Dept_Type
	inline void serialize(int playerid,short type);
}Country_Delate_Request,*LPCountry_Delate_Request;
inline void tagCountry_Delate_Request::serialize(int playerid,short type)
{
	Type = type;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,MDM_GP_MAP_CONSORTIA,CC_C_COUNTRY_DELATE);
}

typedef struct tagCountry_Delate_Response:DISTRIBUTED_HEADER
{
	short Error_Code;			//0:成功；1:弹劾不一致；2：已弹劾；3：没有权限；4:任期最后48小时不能弹劾；5:不能弹劾自己；其他：失败
	inline void serialize(int agent,int playerid,short code);
}Country_Delate_Response,*LPCountry_Delate_Response;
inline void tagCountry_Delate_Response::serialize(int agent,int playerid,short code)
{
	Error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CONSORTIA,playerid,CC_S_COUNTRY_DELATE);
}

//查询弹劾信息CC_C_QUERY_DELATEINFO,CC_S_QUERY_DELATEINFO,
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
	int Type;				//参看Dept_Type,-1：没有弹劾；111：失败
	unsigned int Times;		//剩余时间
	int UnderWrite;			//签名数量
	int Uphold;				//赞成票
	int Oppose;				//反对票
	int Nonuser;			//弃权票
	short State;			//参看DeptState_Type,是否有部长的状态
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

//弹劾投票CC_C_DELATE_BALLOT,	CC_S_DELATE_BALLOT,
typedef struct tagDelate_Ballot_Request:DISTRIBUTED_HEADER
{
	Vote_Type Value;				//投票类型，如赞成、反对、弃权、签名
	inline void serialize(int playerid,Vote_Type value);
}Delate_Ballot_Request,*LPDelate_Ballot_Request;
inline void tagDelate_Ballot_Request::serialize(int playerid,Vote_Type value)
{
	Value = value;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,MDM_GP_MAP_CONSORTIA,CC_C_DELATE_BALLOT);
}
typedef struct tagDelate_Ballot_Response:DISTRIBUTED_HEADER
{
	short Error_Code;				//0：成功,1:已投票；2：部长无法参与投票；3:没有权限；其他：失败
	inline void serialize(int agent,int playerid,short code);
}Delate_Ballot_Response,*LPDelate_Ballot_Response;
inline void tagDelate_Ballot_Response::serialize(int agent,int playerid,short code)
{
	Error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CONSORTIA,playerid,CC_S_DELATE_BALLOT);
}

//修改参选纲领CC_C_MODIFY_ELECTIONCREED,	CC_S_MODIFY_ELECTIONCREED,
typedef struct tagModify_ElectionCreed_Request:DISTRIBUTED_HEADER
{
	int Money;				//新增竞选费用
	char Creed[100];		//新纲领
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
	short Error_Code;			//0：成功；1：玩家未参选；2：金钱不够；其他：失败
	inline void serialize(int agent,int playerid,short code);
}Modify_ElectionCreed_Response,*LPModify_ElectionCreed_Response;
inline void tagModify_ElectionCreed_Response::serialize(int agent,int playerid,short code)
{
	Error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CONSORTIA,playerid,CC_S_MODIFY_ELECTIONCREED);
}
//国防部长创建国防军CC_C_CREATE_TIRRIER,CC_S_CREATE_TIRRIER,
typedef struct tagCreateTirrier_Request:DISTRIBUTED_HEADER
{
	int TirrierType;			//0:战士；1：箭手；2：法师
	inline void serialize(int playerid,int tirriertype);
}CreateTirrier_Request,*LPCreateTirrier_Request;
inline void tagCreateTirrier_Request::serialize(int playerid,int tirriertype)
{
	TirrierType = tirriertype;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,playerid,MDM_GP_MAP_CONSORTIA,CC_C_CREATE_TIRRIER);
}

typedef struct tagCreateTirrier_Response:DISTRIBUTED_HEADER
{
	short Error_Code;				//0:成功；1：没有权限；2：金钱不够；3:请求的参数有误；4:国防军数量超过上限；111：失败
	inline void serialize(int agent,int playerid,short code);
}CreateTirrier_Response,*LPCreateTirrier_Response;
inline void tagCreateTirrier_Response::serialize(int agent,int playerid,short code)
{
	Error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CONSORTIA,playerid,CC_S_CREATE_TIRRIER);
}

//国防军出征CC_C_TIRRIER_GOOUT,	CC_S_TIRRIER_GOOUT,
typedef struct tagTirrierGoOut_Request:DISTRIBUTED_HEADER
{
	int MapID;				//目的郡ID
	int Week;				//出征日期，3：周三；5：周五
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
	short Error_Code;			//0:成功；1：没有权限；111：失败
	inline void serialize(int agent,int playerid,short code);
}TirrierGoOut_Response,*LPTirrierGoOut_Response;
inline void tagTirrierGoOut_Response::serialize(int agent,int playerid,short code)
{
	Error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CONSORTIA,playerid,CC_S_TIRRIER_GOOUT);
}
//升级公共建筑CC_C_UPGRADEBUILD,CC_S_UPGRADEBUILD,
typedef struct tagUpgradeBuild_Request:DISTRIBUTED_HEADER
{
	int MapID;//郡地图ID
	int BuildID;//建筑物ID
	int Money;//捐献金钱
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
	short Error_Code;		//0:成功；1：没有权限；2：金钱不足；111：其他失败
	inline void serialize(int agent,int playerid,short code);
}UpgradeBuild_Response,*LPUpgradeBuild_Response;
inline void tagUpgradeBuild_Response::serialize(int agent,int playerid,short code)
{
	Error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CONSORTIA,playerid,CC_S_UPGRADEBUILD);
}

//修改实例公告CC_C_MODIFYNOTICE,CC_S_MODIFYNOTICE,
typedef struct tagModifyNotice_Request:DISTRIBUTED_HEADER
{
	char	Notice[SIZE_256];		//势力公告
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
	short Error_Code;			//0:成功;1:没有权限；111：失败
	inline void serialize(int agent,int playerid,short code);
}ModifyNotice_Response,*LPModifyNotice_Response;
inline void tagModifyNotice_Response::serialize(int agent,int playerid,short code)
{
	Error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CONSORTIA,playerid,CC_S_MODIFYNOTICE);
}
//查询国防军数量CC_C_QUERY_TIRRIERCOUNT,CC_S_QUERY_TIRRIERCOUNT,
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
	int TirrierCount[3];//0:战士数量；1：箭手数量；2：法师数量
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
//通知郡地图创建国防军CC_C_NOTIFY_CREATETIRRIER,CC_S_NOTIFY_CREATETIRRIER,
typedef struct tagNotify_CreateTirrier_Request:DISTRIBUTED_HEADER
{
	int CountryID;				//阵营
	int WeaponType;				//武器
	int WeaponLevel;			//武器级别
	int DefendType;				//防具类型
	int DefendLevel;			//防具级别
	int SoldierType;			//士兵类型
	int TirrierType;			//0:战士；1：箭手；2：法师
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
	short Error_Code;			//0:成功；其他：失败
	int CountryID;
	int TirrierType;			//0:战士；1：箭手；2：法师
	inline void serialize(int agent,int playerid,short code,int countryid,int type);
}Notify_CreateTirrier_Response,*LPNotify_CreateTirrier_Response;
inline void tagNotify_CreateTirrier_Response::serialize(int agent,int playerid,short code,int countryid,int type)
{
	Error_Code = code;
	CountryID = countryid;
	TirrierType = type;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,playerid,MDM_GP_MAP_CONSORTIA,CC_S_NOTIFY_CREATETIRRIER);
}

//通知大地图服务器国防军出征CC_C_NOTIFY_TIRRIERGOOUT,CC_S_NOTIFY_TIRRIERGOOUT,
typedef struct tagNotify_TirrierGoOut_Request:DISTRIBUTED_HEADER
{
	int ArmyID;				//部队ID
	int MapID;				//目的郡
	inline void serialize(int mapid,int armyid,int tomapid);
}Notify_TirrierGoOut_Request,*LPNotify_TirrierGoOut_Request;
inline void tagNotify_TirrierGoOut_Request::serialize(int mapid,int armyid,int tomapid)
{
	MapID = tomapid;
	ArmyID = armyid;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,MDM_GP_MAP_CONSORTIA,mapid,CC_C_NOTIFY_TIRRIERGOOUT);
}

//通知郡地图升级公共建筑CC_C_NOTIFY_UPGRADEBUILD,	CC_S_NOTIFY_UPGRADEBUILD,
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
	int Level;					//级别
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
//通知GM修改势力公告CC_C_NOTIFY_MODIFYNOTICE,CC_S_NOTIFY_MODIFYNOTICE,
typedef struct tagNotify_ModifyNotice_Request:DISTRIBUTED_HEADER
{
	int CountryID;					//阵营
	char Notice[SIZE_256];			//势力公告
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
	short Error_Code;				//0:成功
	inline void serialize(int agent,int playerid,short code);
}Notify_ModifyNotice_Response,*LPNotify_ModifyNotice_Response;
inline void tagNotify_ModifyNotice_Response::serialize(int agent,int playerid,short code)
{
	Error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,playerid,MDM_GP_MAP_CONSORTIA,CC_S_NOTIFY_MODIFYNOTICE);
}
//市场收税CC_C_NOTIFY_MARKETRATE,CC_S_NOTIFY_MARKETRATE,
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

//广播组建国防军（可以出征）CC_S_BROAD_ADDARMY,	
//国防军出征CC_S_BROAD_TIRRIERGOOUT
typedef struct tagBroad_TirrierState:DISTRIBUTED_HEADER
{
	int CountryID;				//阵营
	char Name[SIZE_24];			//部长名称
	union
	{
		int MapID;				//国防军出征，目的郡ID
		int Money;				//组建国防军，需要的费用
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

//参加竞选广播消息CC_S_BROAD_ELECTION,	
typedef struct tagBroad_Election_Response:DISTRIBUTED_HEADER
{
	int CountryID;					//阵营
	char Name[SIZE_24];				//竞选者名称
	int Type;						//参看Dept_Type
	int	Money;						//参选费用
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
//升级公共建筑CC_S_BROAD_UPGRADEBUILD,
typedef struct tagBroad_UpgradeBuild:DISTRIBUTED_HEADER
{
	int CountryID;				//阵营
	char Name[SIZE_24];			//部长名称
	int MapID;					//郡地图ID
	int BuildID;				//公共建筑ID
	int Money;					//捐献金额
	int Level;					//建筑的级别
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

//弹劾状态CC_S_BROAD_DELATESTATE
typedef struct tagBroad_DelateState:DISTRIBUTED_HEADER
{
	int CountryID;
	int	Type;	//弹劾类型，参看DetpType
	int State;		//0:弹劾签名开始；1：弹劾投票开始；2：弹劾投票结束；3：弹劾签名结束（表示签名失败）
	inline void serialize(int countryid,int type,int state);
}Broad_DelateState,*LPBroad_DelateState;
inline void tagBroad_DelateState::serialize(int countryid,int type,int state)
{
	CountryID = countryid;
	Type = type;
	State = state;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),0,MDM_GP_MAP_CONSORTIA,0,CC_S_BROAD_DELATESTATE);
}
//部长被弹劾下台，副部长部长顶替CC_S_BROAD_EXCHANGEMINISTER
typedef  struct tagBroad_ExchangeMinister:DISTRIBUTED_HEADER
{
	int CountryID;		//阵营ID
	int Dept;			//参看Dept_Type
	char OldMinister[SIZE_24];//原部长名称
	char NewMinister[SIZE_24];//新任部长名称
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
//授权CC_C_MODIFY_PURVIEW,CC_S_MODIFY_PURVIEW,
typedef struct tagModify_Purview_Request:DISTRIBUTED_HEADER
{
	int playerID;				//被授权人
	int purview;				//新的权限
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
	short error_Code;					//0:成功，1：没有权限；2：被授权人未加入该工会；111：失败
	inline void serialize(int agent,int playerid,short code);
}Modify_Purview_Response,*LPModify_Purview_Response;
inline void tagModify_Purview_Response::serialize(int agent,int playerid,short code)
{
	error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CONSORTIA,playerid,CC_S_MODIFY_PURVIEW);
}
//升级CC_C_UPDATECONSORTIA,CC_S_UPDATECONSORTIA,
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
	short error_Code;			//0:成功;1:没有权限；2:升级到皇家军团失败（没有城镇），3：GO点不足；4：金钱不足；5：经验不足；6：荣誉不足;7:升级到皇家军团，为占有城镇；111：失败
	inline void serialize(int agent,int playerid,short code);
}UpdateConsortia_Response,*LPUpdateConsortia_Response;
inline void tagUpdateConsortia_Response::serialize(int agent,int playerid,short code)
{
	error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CONSORTIA,playerid,CC_S_UPDATECONSORTIA);
}
//捐钱到军团CC_C_CONTRIBUTEMONEY,CC_S_CONTRIBUTEMONEY,
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
	short error_Code;			//0:成功；1：未加入军团；2：金钱不足;111:失败
	inline void serialize(int agent,int playerid,short code);
}ContributeMoney_Response,*LPContributeMoney_Response;
inline void tagContributeMoney_Response::serialize(int agent,int playerid,short code)
{
	error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CONSORTIA,playerid,CC_S_CONTRIBUTEMONEY);
}
//领工资CC_C_SALARIED,CC_S_SALARIED,
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
	int money;				//-3:军团称号不存在；-2：未加入工会，-1:今天不能领取；0:还未有军团称号；>0：领取的工资数
	inline void serialize(int agent,int playerid,int wage);
}Salaried_Response,*LPSalaried_Response;
inline void tagSalaried_Response::serialize(int agent,int playerid,int wage)
{
	money = wage;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CONSORTIA,playerid,CC_S_SALARIED);
}
//修改军团标识CC_C_MODIFY_SIGNID,CC_S_MODIFY_SIGNID,
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
	short error_Code;					//0:成功；1：没有权限；2：非皇家军团；111：失败
	inline void serialize(int agent,int playerid,short code);
}Modify_SignID_Response,*LPModify_SignID_Response;
inline void tagModify_SignID_Response::serialize(int agent,int playerid,short code)
{
	error_Code = code;
	DISTRIBUTED_HEADER::serialize(sizeof(*this),agent,MDM_GP_MAP_CONSORTIA,playerid,CC_S_MODIFY_SIGNID);
}
//通知收费系统军团升级CC_C_NOTIFY_UPDATECONSORTIA,CC_S_NOTIFY_UPDATECONSORTIA,
typedef struct tagNotify_UpdateConsortia_Request:DISTRIBUTED_HEADER
{
	int playerID;			//玩家ID
	int consortiaID;		//公会ID
	int goDian;				//扣除GO点
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
/*涉及到邮件的结构体*/
//公会结盟申请
typedef struct tagEmaiBody_Union
{
	int ConsortiaID;		//公会1ID
	int PlayerID;			//申请人ID
}EmailBody_Union,*LPEmailBody_Union;
//公会约战申请
typedef struct tagEmailBody_Fight
{
	int		ConsortiaID;				//公会ID
	int		ApplyPlayerID;				//申请人
	int		MapID;						//约战郡
	int		CityID;						//约战城市
	__int64 Time;						//约战时间
}EmailBody_Fight,*LPEmailBody_Fight;
//申请加入公会
typedef struct tagEmailBody_JoinCon
{
	int PlayerID;			//申请人ID
	int ConsortiaID;		//申请加入的公会ID
}EmailBody_JoinCon,*LPEmailBody_JoinCon;

//邀请加入公会
typedef struct tagEmailBody_Invite
{
	int ConsortiaID;		//公会ID
	int PlayerID;			//邀请人ID
}EmailBody_Invite,*LPEmailBody_Invite;


#endif;