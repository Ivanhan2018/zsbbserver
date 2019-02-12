#ifndef CONSORTIADATADEFINE_H__
#define CONSORTIADATADEFINE_H__
#include "IceUtil.h"
#include "Global_Macro.h"
#include "PlayerAcc.h"
#include "Consortia.h"
#include "ConsortiaOfficial.h"
#include "ConsortiaProtocol.h"
#include "PlayerRanking.h"
#include "CommonFunction.h"
#include "ConsortiaRanking.h"
#include "ZSBB_DataDefine.h"

#ifndef	MAX_LOGO_SIZE
#	define MAX_LOGO_SIZE			4096	//自定义Logo的最大限制
#endif

#define CFGFILE		"./Consortia.ini"
#define MAX_CONSORTIA_COUNT		500

//==================================================================
typedef enum {
	JoinApply_Email,		//申请加入的邮件配置
	AgreeJoin_Email,		//同意加入
	RefuseJoin_Email,		//拒绝加入
	KickUser_Email,			//Kick成员
	InviteEmail,			//邀请加入
	AgreeInvite_Email,		//同意邀请
	RefuseInvite_Email,		//拒绝邀请
	PoseOfficial_Email,		//任职通知
	DeposeOfficial_Email,	//免职通知
	TranChairman_Email,		//移交会长
	AllyApply_Email,		//结盟申请
	AgreeAlly_Email,		//同意结盟
	RefuseAlly_Email,		//拒绝结盟
	Unally_Email			//解盟
}Email_Param;

typedef enum{
	Default_Notice,			//默认
	CreateCons_Notice,		//创建公会
	DelConsortia_Notice,	//解散公会
	JoinConsortia_Notice,	//加入公会
	KickUser_Notice,		//Kick成员
	QuitConsortia_Notice,	//退出公会
	PoseOfficail_Notice,	//任职
	DeposeOfficail_Notice,	//免职
	TranChairman_Notice,	//移交会长
}Notice_Param;
//连接参数
typedef struct tagAgent_DB_Param
{
	char			AgentIP[SIZE_32];
	unsigned int	AgentPort;
	char			DBGIP[SIZE_32];
	unsigned int	DBGPort;
	char			LogSvrIP[SIZE_32];
	unsigned int	LogSvrPort;
}Agent_DB_Param,*LPAgent_DB_Param;

typedef struct tagEmailParam
{
	char Title[SIZE_16];
	char Body[SIZE_128];
}EmailParam,*LPEmailParam;

//游戏中的所有玩家
typedef IceUtil::Handle<CPlayerAcc> PlayerAcc_Ptr;
typedef std::map<int,PlayerAcc_Ptr> PlayerAcc_Map;
typedef std::map<int,PlayerAcc_Ptr>::iterator PlayerAcc_Iterator;

typedef std::map<std::string,int> PlayerAcc_Name_Map;
typedef std::map<std::string,int>::iterator PlayerAcc_Name_Iterator;

/*公会
*/
typedef IceUtil::Handle<CConsortia> Consortia_Ptr;
typedef std::map<int,Consortia_Ptr> Consortia_Map;
typedef struct tagConsortiaNode
{
	RWMutex rwmutex_;		//公会锁
	Consortia_Ptr con_ptr;	//公会
}ConsortiaNode,*LPConsortiaNode;

//职位信息表
typedef IceUtil::Handle<CConsortiaOfficial> ConsortiaOfficial_Ptr;
typedef std::map<int,ConsortiaOfficial_Ptr> ConsortiaOfficial_Map;
typedef std::map<int,ConsortiaOfficial_Ptr>::iterator ConsortiaOfficial_Iteraotr;


//创建公会的条件表
typedef struct tagCreateConsortiaLimit
{
	int			ResID;					//ID
	int			FoodCount;				//粮食数量
	int			WoodCount;				//木材数量
	int			IronCount;				//铁数量
	int			SkinCount;				//皮数量
	int			RedStoneCount;			//红宝石
	int			YollowStoneCount;		//黄宝石
	int			GreenStoneCount;		//绿宝石
	int			BackIronCount;			//黑铁
	int			Times;					//连续多长时间（小时）人员未满Personnel人时，解散公会
	int			Personnel;				//人员数（创建24小时后，成员的数量要求）
	int			Science;				//科技点
	int			Money;					//金钱
	int			Level;					//等级要求
	int			JoinLevel;				//加入公会的等级要求
}CreateConsortiaLimit,*LPCreateConsortiaLimit;

//加入公会的申请
//typedef IceUtil::Handle


//排行
typedef	IceUtil::Handle<CPlayerRanking> PlayerRanking_Ptr;

typedef	IceUtil::Handle<CConsortiaRanking> ConsortiaRanking_Ptr;

typedef struct tagTirrierParam
{
	int TirrierType;//0:战士；1：箭手；2：法师
	int SoldierType;	//士兵类型
	int WeaponType;		//武器类型
	int WeaponLevel;	//武器级别
	int DefendType;		//防具类型
	int DefendLevel;	//防具级别
	int Money;			//费用
}TirrierParam,*LPTirrierParam;

typedef std::map<int,LPTirrierParam> TirrierParam_Map;
typedef TirrierParam_Map::iterator TirrierParam_Map_Iterator;

#endif
