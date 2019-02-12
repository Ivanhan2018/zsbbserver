#ifndef __INCLUDE_GLOBAL_MACRO_H__
#define __INCLUDE_GLOBAL_MACRO_H__

#if defined (WIN32)
typedef __int64 int64; // windows
#else
typedef long long int64; // linux
#endif


#define SAFEDELBUFFER(buf) if(buf){delete[] (char*)buf;buf=0;}

#define  BIG_MAP_SIZE							60					//郡的数量
#define MAX_INLAY_COUNT							5					//最大镶嵌次数（装备升级次数）
/***************************************************************************
// 公共信息
***************************************************************************/
enum COMMON
{
	SIZE_8	=8,
	MAX_KEY_SIZE = 16,
	SIZE_16 = 16,
	SIZE_20 = 20,
	SIZE_24 = 24,
	SIZE_32 = 32,
	SIZE_64 = 64,
	SIZE_128 = 128,
	SIZE_256 = 256,
	SIZE_512 = 512
};

enum COMPRESS
{
	LARGE_BOUND = 300
};

/* 任务协议命令字常数 */
enum TaskCommand
{
	//玩家获取任务列表
	TC_GET_TASK_LIST						=	0x7001,
	TC_GET_TASK_COUNT,
	TC_REP_TASK_COUNT,
	//玩家设置用户状态
	TC_SET_TASK_STATE,						
	TC_REP_GET_TASK_LIST,					
	TC_REP_SET_TASK_STATE,					
	TC_ISSUE_USER_TASK,						
	TC_USER_PLAYERLEVEL_PLAYERRANK_EVENT,	
	TC_USER_TASKITEM_EVENT,					
	TC_USER_TASK_COMPLETE_EVENT,				
	TC_ECTYPE_EVENT,							
	TC_REQ_CHECK_TASK_OK,						
	TC_REP_TASKSTATE_COMPLETE,
};

/* GM 协议命令字 */
enum GmCommand
{
	GC_LOGIN			= 0x7070,		//登录GM
	GC_LOGOUT					,		//登出GM
	GC_SPEAKDISABLED			,		//禁言
	GC_LOGINDISABLED			,		//禁登录
	GC_NOTICEFORCES				,		//势力公告
	GC_GMCOMMAND				,		//GM常规命令
	GC_NOTICEBREADCAST			,		//GM普通公告
	GC_NOTICEFORCES_EVENT		,		//势力公告事件
	GC_HEROADD_EVENT					//首领英难新增事件
};

//频道类型(GM）
enum ChannelType			 
{	
	ctUnion  =1,	//联盟
	ctTribal =2		//部落
};

//公告类型 
enum NoticeType
{
	ntNoticeForces = 1,   //势力公告
	ntNoticePost = 2	  //组播公告	
};

//GM管理(GM内容最大长度)
enum GM_About
{
	GM_DATA_SIZE		=	255 

};

typedef enum
{
	ARMET=1,						//1.头盔
	CORSELET,						//2.护甲
	CAESTUS,						//3 腰带
	GLOVE,							//4 手套
	BOOT,							//5 靴子
	MOUNT,							//6 坐骑
	ARM,							//7 武器
	ACC_1,							//8 饰物1
	ACC_2,							//9 饰物2
	ACC_3,							//10 饰物3
	RESOURCECARD,					//11基本资源卡,兑换基本资源
	RESOURCESPEEDCARD,				//12基本资源生产加速卡
	OTHERSPEEDCARD,					//13建造、科研、征兵加速卡，只加速某一具体项
	RECRUITHERO,					//14招募英雄卡
	SPECIALSKILLCARD,				//15特殊技能卡
	HEROSAFECARD,					//16英雄安全卡，被动使用
	HEROEQUIPCARD,					//17英雄装备卡,兑换英雄装备
	AUTOCARD,						//18托管卡
	EXPANDPACKCARD,					//19背包扩展卡
	TREASUREPACK,					//20宝箱
	INLAYRESOURCE,					//21精练原料，各种镶嵌到装备上的材料
	LUCKCARD,						//22幸运符类卡,包括幸运符和保底卡
	ARMYBRAND,						//23军牌
	FIGHTCARD=24,					//24战斗类型卡：自保卡，援兵卡
	NA, //25非可装备 


}USE_PART;



/****************************军团系统命令字常数定义********************************/
enum ConsortiaCommand
{
	CC_C_CREATE_CONSORTIA			= 0x8001,	//军团创建
	CC_S_CREATE_CONSORTIA,

	CC_C_QUERY_CONSORTIA,						//军团查询
	CC_S_QUERY_CONSORTIA,

	CC_C_DELETE_CONSORTIA,						//军团解散
	CC_S_DELETE_CONSORTIA,

	CC_C_QUERY_USER_CON,						//查询玩家所属军团
	CC_S_QUERY_USER_CON,

	CC_C_MODIFY_PLACARD,						//修改公告
	CC_S_MODIFY_PLACARD,

	CC_C_QUERY_LOGO,							//查询LOGO:
	CC_S_QUERY_LOGO,

	CC_C_MODIFY_LOGO,							//修改LOGO：
	CC_S_MODIFY_LOGO,

	CC_C_CONTRIBUTE_BANK,						//捐献资源到军团银行
	CC_S_CONTRIBUTE_BANK,

	CC_C_PAYOUT_BANK,							//支出军团资源
	CC_S_PAYOUT_BANK,

	CC_C_QUERY_BANK,							//军团银行查询
	CC_S_QUERY_BANK,

	CC_C_ALLY_APPLY_CON,						//结盟申请
	CC_S_ALLY_APPLY_CON,

	CC_C_ALLY_CONSORTIA_AGREE,					//同意结盟
	CC_S_ALLY_CONSORTIA_AGREE,

	CC_C_ALLY_CONSORTIA_REFUSE,					//拒绝结盟
	CC_S_ALLY_CONSORTIA_REFUSE,

	CC_C_UNALLY_CONSORTIA,						//解盟
	CC_S_UNALLY_CONSORTIA,

	CC_C_FIGHT_APPLY_CON,						//约战申请
	CC_S_FIGHT_APPLY_CON,

	CC_C_FIGHT_CONSORTIA_AGREE,					//同意约战
	CC_S_FIGHT_CONSORTIA_AGREE,

	CC_C_FIGHT_CONSORTIA_REFUSE,				//拒绝约战
	CC_S_FIGHT_CONSORTIA_REFUSE,

	CC_C_QUERY_FIGHT,							//约战查询
	CC_S_QUERY_FIGHT,

	CC_C_JOIN_APPLY_CON,						//申请加入军团
	CC_S_JOIN_APPLY_CON,

	CC_C_JOIN_CONSORTIA_AGREE,					//会长同意加入工会
	CC_S_JOIN_CONSORTIA_AGREE,

	CC_C_JOIN_CONSORTIA_REFUSE,					//会长拒绝加入工会
	CC_S_JOIN_CONSORTIA_REFUSE,

	CC_C_QUERY_CONUSER,							//军团成员查询
	CC_S_QUERY_CONUSER,

	CC_C_KICK_CONUSER,							//Kick成员
	CC_S_KICK_CONUSER,

	CC_C_QUIT_CONSORTIA,						//退出军团
	CC_S_QUIT_CONSORTIA,

	CC_C_INVITE_JOIN_CON,						//邀请加入军团
	CC_S_INVITE_JOIN_CON,

	CC_C_INVITE_JOIN_AGREE,						//同意邀请加入工会
	CC_S_INVITE_JOIN_AGREE,

	CC_C_INVITE_JOIN_REFUSE,					//拒绝邀请加入工会
	CC_S_INVITE_JOIN_REFUSE,

	CC_C_POSE_OFFICIAL,							//任职
	CC_S_POSE_OFFICIAL,

	CC_C_DEPOSE_OFFICIAL,						//免职
	CC_S_DEPOSE_OFFICIAL,

	CC_C_QUERY_OFFICIAL,						//查询职位
	CC_S_QUERY_OFFICIAL,

	CC_C_QUERY_CREATELIMIT,						//查询创建军团的限制条件
	CC_S_QUERY_CREATELIMIT,

	CC_C_MODIFY_CONTENET,						//修改军团宗旨
	CC_S_MODIFY_CONTENET,
	
	CC_C_CLIENTLOGIN,							//客户端登陆
	CC_S_CLIENTLOGIN,

	CC_C_TRANS_CHAIRMAN,						//转让会长
	CC_S_TRANS_CHAIRMAN,

	CC_C_CON_RANKING,							//军团排行
	CC_S_CON_RANKING,

	CC_C_PLAYER_RANKING,						//个人排行
	CC_S_PLAYER_RANKING,
	//20100508为有种男战场排行增加
	CC_C_NEWPLAYER_RANKING,						//每周新人排行
	CC_S_NEWPLAYER_RANKING,

	CC_C_FRIENDS_COUNT,							//人气型排行
	CC_S_FRIENDS_COUNT,

	CC_C_MONEY_COUNT,							//巨商型排行
	CC_S_MONEY_COUNT,

	CC_C_GENERAL_CAPTIVE_COUNT,					//策略型排行
	CC_S_GENERAL_CAPTIVE_COUNT,

	//20090323为国家系统增加
	CC_C_CHECKCONSORTIA,						//检测公会是否24小时人数未满

	CC_C_QUERY_USEROFCOUNTRY,					//查询玩家在国家的信息
	CC_S_QUERY_USEROFCOUNTRY,

	CC_C_QUERY_COUNCILLIST,						//查询本阵营的议员列表，包括军事委员会、经济委员会、公益委员会
	CC_S_QUERY_COUNCILLIST,

	CC_C_TAKEPARTIN_ELECTION,					//参加竞选（必须为议员）
	CC_S_TAKEPARTIN_ELECTION,

	CC_C_QUERY_CANDIDATELIST,					//查询候选人列表(国防部、内政部）
	CC_S_QUERY_CANDIDATELIST,

	CC_C_PLAYER_BALLOT,							//玩家进行投票选举
	CC_S_PLAYER_BALLOT,

	CC_C_COUNTRY_DELATE,						//弹劾
	CC_S_COUNTRY_DELATE,

	CC_C_QUERY_DELATEINFO,						//查询弹劾信息
	CC_S_QUERY_DELATEINFO,

	CC_C_DELATE_BALLOT,							//弹劾投票
	CC_S_DELATE_BALLOT,

	CC_C_MODIFY_ELECTIONCREED,					//修改参选纲领
	CC_S_MODIFY_ELECTIONCREED,

	CC_C_CREATE_TIRRIER,						//国防部长创建国防军
	CC_S_CREATE_TIRRIER,

	CC_C_TIRRIER_GOOUT,							//国防军出征
	CC_S_TIRRIER_GOOUT,

	CC_C_UPGRADEBUILD,							//升级公共建筑
	CC_S_UPGRADEBUILD,

	CC_C_MODIFYNOTICE,							//修改实例公告
	CC_S_MODIFYNOTICE,

	CC_C_QUERY_TIRRIERCOUNT,					//查询国防军数量
	CC_S_QUERY_TIRRIERCOUNT,

	CC_C_NOTIFY_CREATETIRRIER,					//通知郡地图创建国防军
	CC_S_NOTIFY_CREATETIRRIER,

	CC_C_NOTIFY_TIRRIERGOOUT,					//通知大地图服务器国防军出征
	CC_S_NOTIFY_TIRRIERGOOUT,

	CC_C_NOTIFY_UPGRADEBUILD,					//通知郡地图升级公共建筑
	CC_S_NOTIFY_UPGRADEBUILD,

	CC_C_NOTIFY_MODIFYNOTICE,					//通知GM修改势力公告
	CC_S_NOTIFY_MODIFYNOTICE,

	CC_C_NOTIFY_MARKETRATE,						//市场收税
	CC_S_NOTIFY_MARKETRATE,

	CC_C_TIMER_VOTECOUNCIL, 					//自动选举出议员
	//CC_C_TIMER_VOTEMINISTER,					//部长选举
	CC_C_TIMER_VOTEEND,							//选举结束
	CC_C_TIMER_UNDERWRITE,						//弹劾签名
	CC_C_TIMER_DELATEEND,						//弹劾结束
	CC_C_TIMER_TIRRIERGOOUT,					//国防军出征定时器

	CC_S_BROAD_VOTESTART,						//选举开始广播消息,包括议员列表
	CC_S_BROAD_VOTEND,							//选举结束广播消息,包括国防部、内政部成员列表
	CC_S_BROAD_ELECTION,						//参加竞选广播消息
	CC_S_BROAD_ADDARMY,							//广播组建国防军（可以出征）
	CC_S_BROAD_TIRRIERGOOUT,					//国防军出征
	CC_S_BROAD_UPGRADEBUILD,					//升级公共建筑
	CC_S_BROAD_DELATESTATE,						//弹劾状态
	CC_S_BROAD_EXCHANGEMINISTER,				//部长被弹劾下台，副部长部长顶替

	CC_C_TIMER_BROADBEVOTE,						//提前通知选举开始,发送邮件通知
	CC_C_TIMER_RANKING,							//排行定时器

	//add 20100302
	CC_C_MODIFY_PURVIEW,						//授权
	CC_S_MODIFY_PURVIEW,

	CC_C_UPDATECONSORTIA,						//升级
	CC_S_UPDATECONSORTIA,

	CC_C_CONTRIBUTEMONEY,						//捐钱到军团
	CC_S_CONTRIBUTEMONEY,

	CC_C_SALARIED,								//领工资
	CC_S_SALARIED,

	CC_C_MODIFY_SIGNID,							//修改军团标识
	CC_S_MODIFY_SIGNID,

	CC_C_NOTIFY_UPDATECONSORTIA,				//通知收费系统军团升级
	CC_S_NOTIFY_UPDATECONSORTIA,
};

/****************************收费系统命令字常数定义********************************/
enum ChargeCommand
{
	CGC_C_QUERYMONEY				=0x9001,	//查询Go点
	CGC_S_QUERYMONEY,

	CGC_C_BUYCARD,								//购买卡
	CGC_S_BUYCARD,								//

	CGC_C_RESOURCECARD,							//使用基本资源卡，购买基本资源
	CGC_S_RESOURCECARD,

	CGC_C_RESOURCESPEEDCARD,					//生产加速卡
	CGC_S_RESOURCESPEEDCARD,

	CGC_C_RESRUITHEROCARD,						//招募英雄卡
	CGC_S_RESRUITHEROCARD,

	CGC_C_SPECIALSKILLCARD,						//特殊技能卡
	CGC_S_SPECIALSKILLCARD,

	CGC_C_OTHERSPEEDCARD,						//建造、科研、征兵加速卡及某项生产
	CGC_S_OTHERSPEEDCARD,
	
	CGC_C_AUTOCARD,								//使用托管卡
	CGC_S_AUTOCARD,	

	CGC_C_CANCELAUTO,							//取消托管
	CGC_S_CANCELAUTO,

	CGC_C_QUERYAUTOLIST,						//查询托管列表
	CGC_S_QUERYAUTOLIST,

	CGC_C_HEROEQUIPCARD,						//装备卡
	CGC_S_HEROEQUIPCARD,

	CGC_C_QUERYRESOURCERATE,					//查询生产速率
	CGC_S_QUERYRESOURCERATE,

	CGC_C_EXPANDPACKCARD,						//使用背包扩展卡
	CGC_S_EXPANDPACKCARD,	

	CGC_C_EXCHANGE,								//兑换
	CGC_S_EXCHANGE,

	CGC_C_INCREASERECOURCERATE,					//提高生产率，不使用卡
	CGC_S_INCREASERECOURCERATE,

	CGC_C_UEEHERORESUME,						//使用恢复卡
	CGC_S_USEHERORESUME,

	CGC_C_TIMERNOTIFY,							//时间通知（定时器）

	//以下为服务端之间的消息通知,此时收费服务作为客户端
	CGC_C_NOTITY_BUILDCONSTRUCTION,				//通知建造建筑，用于托管类
	CGC_S_NOTITY_BUILDCONSTRUCTION,

	CGC_C_NOTIFY_INCREASESPEED,					//提速通知(生产提速）
	CGC_S_NOTIFY_INCREASESPEED,
	
	CGC_C_NOTIFY_NORMALSPEED,					//恢复正常速度
	CGC_S_NOTIFY_NORMALSPEED,

	CGC_C_NOTIFY_INCREASEOTHERSPEED,			//一次性加速，包括建造、科研、征兵加速卡和招募英雄
	CGC_S_NOTIFY_INCREASEOTHERSPEED,

	CGC_C_NOTIFY_COLLECTRESOURCE,				//采集资源的通知
	CGC_S_NOTIFY_COLLECTRESOURCE,				

	CGC_S_NOTIFY_BUILDCOMPLETE,					//建造完成通知，有郡地图服务器自动下发通知

	CGC_C_NOTIFY_IMMEDCOMPLETE,					//立即完成通知消息
	CGC_S_NOTIFY_IMMEDCOMPLETE,

	CGC_C_NOTIFY_ELECTION,						//参加竞选扣费
	CGC_S_NOTIFY_ELECTION,

	CGC_C_NOTIFY_QUERYMONEY,					//任务系统查询GO点
	CGC_S_NOTIFY_QUERYMONEY,

	CGC_C_NOTIFY_ADDMONEY,						//扣除/添加GO点
	CGC_S_NOTIFY_ADDMONEY,

	CGC_C_NOTIFY_ADDOFFICERSKILL,				//使用技能卡通知大地图
	CGC_S_NOTIFY_ADDOFFICERSKILL,

	CGC_C_NOTIFY_USEONECARD,					//使用一种卡，各服务器发送过来的通知
	CGC_S_NOTIFY_USEONECARD,

	CGC_C_NOTIFY_BUYCARD,						//买卡通知，收费服务器通知大地图
	CGC_S_NOTIFY_BUYCARD
};

/***************************************************************************
// 服务器ID范围定义
***************************************************************************/
enum SERVER_BOUND
{
	SERVER_BOUND_MAP_BEGIN = 1000,
	MDM_GP_MAP_SERVER = 1895,				//地图服务消息
	MDM_GP_MAP_MARKET = 1896,				//市场消息
	MDM_GP_MAP_CONSORTIA = 1897,			//军团消息
	MDM_GP_MAP_RESEARCH = 1898,				//科研消息
	SERVER_BOUND_MAP_OTHER = 1899,			//排行,指挥官等消息
	MDM_GP_MAP_CHAT	 = 1900,				//聊天服务器
	MDM_GP_MAP_TASK	 = 1901,				//任务
	MDM_GP_MAP_CHARGE = 1902,				//收费消息
	MDM_GP_GAME_MANAGE_SERVER= 1903,		//GM管理

	MDM_GP_GAME_HALL_SERVER = 1250,         // 游戏大厅服务器


	SERVER_BOUND_MAP_END = 1999,

	SERVER_BOUND_LIGHT_BEGIN = 2000,
	SERVER_BOUND_LIGHT_END = 2999,

	SERVER_BOUND_LOGIN_BEGIN = 3000,
	SERVER_BOUND_LOGIN_END = 3999,

	SERVER_BOUND_AGENT_BEGIN = 4000,
	SERVER_BOUND_AGENT_END = 4999,

	SERVER_MANAGER_ID = 5000,
	MAX_SERVER_NUM = 5001
};


/***************************************************************************
// 管理服务器通信协议命令号
***************************************************************************/
enum PROTOCOL_MANAGER
{
	CMD_SERVER_REGISTER_MANAGER = 0x2001,
	CMD_MANAGER_REGISTER_RESP_SERVER = 0x2002,
	CMD_AGENT_REPORT_LOGIN = 0x2003,
	CMD_SERVER_NOTIFY_MANAGER_NOTIFY_SERVER = 0x2004,
	CMD_SERVER_NOTIFY_RESP_SERVER = 0x2005,
	CMD_SERVER_HANDSHAKE_SERVER = 0x2006,
	CMD_SERVER_HANDSHAKE_RESP_SERVER = 0x2007,
	CMD_AGENT_CLIENT_LEAVE_SERVER = 0x2008,
	CMD_AGENT_CLIENT_RESPONSE = 0x2009,
	CMD_AGENT_SERVER_RESPONSE = 0x200A,
	CMD_CLIENT_REGISTER_LOGIN = 0x200B,
	CMD_LOGIN_REGISTER_RESP_CLIENT = 0x200C,
	CMD_SERVER_BROADCAST_CLIENT = 0x200D,
	CMD_SERVER_MULTICAST_CLIENT = 0x200E,

	CMD_WEB_LOGIN = 0x200F,
	CMD_WEB_LOGIN_RESP = 0x2010,
	CMD_WEB_REGISTER = 0x2011,
	CMD_WEB_REGISTER_RESP = 0x2012,

	CMD_CLIENT_REGISTER_LOGOUT = 0x2013,

	CMD_LOG_REPORT_INFO = 0x2014,
	CMD_LOG_REPORT_PERFORMANCE = 0x2015,
	CMD_LOG_REPORT_ONLINE = 0x2016,
	CMD_LOG_REPORT_GROUP_ONLINE = 0x2017,

	CMD_LOG_GAME_WRITER = 0x2018,
	CMD_LOG_GAME_READER = 0x2019,
	CMD_LOG_GAME_READER_RSP = 0x201A
};

enum PROTOCOL_EVENT
{
	CMD_STATR_FIGHT			= 0x6000,
	CMD_ARMY_ARRIVED,
	CMD_END_FIGHT			,
	CMD_ARMY_REACH,
	CMD_HERO_PARAM,
	CMD_PLAYER_UPDATA_RESOURCE,
	CMD_ENTER_ECTYPE,
	CMD_BACK_ENTER_ECTYPE,
	CMD_BACK_ARMY_ECTYPE,
	CMD_ADD_MAIN_HERO,
	CMD_ADD_MAIl_NOTIFY,
	CMD_ADD_CONSORTIA_NOTIFY,
	CMD_ADD_NEWS_REPORT_NOTIFY,
	CMD_IS_IN_ECTYPE,
	CMD_BACK_IS_IN_ECTYPE,
	CMD_BACK_ARMY_REACH,
	CMD_ARMY_REACH_OK,
	CMD_HERO_STUDY_SKILL,
	CMD_UPDATE_HERO_PARAM,
	CMD_BROADMAIL_NOTIFY,
	CMD_FIGHT_TIMER_MSG,
	CMD_DEL_ECTYPE_PLAYER,
	CMD_ECTYPE_FIGHT_OVER,
	CMD_HERO_CLEAR_SKILL		//英雄清除技能
};


/***************************************************************************
// 数据库网关服务器通信协议命令号
***************************************************************************/
enum PROTOCOL_DBG
{
//	CMD_PLAYER_ACCOUNTS = 0x01,
	CMD_GAME_CITY_INFO = 0x02,
	CMD_PLAYER_SOLDIER_RACE = 0x03,
	CMD_EXE_SOLDIER = 0x04,
	CMD_RESEARCH_CONFIGURE = 0x05,
	CMD_ARMY_TABLE = 0x06,
	CMD_MARKET = 0x07,
	CMD_OFFICER_MEDAL = 0x08,
	CMD_CONSTRUCTION_BUILD = 0x09,
	CMD_PRODUCTION_ACTIVITY = 0x0A,
	CMD_CONSORTIA_INFO = 0x0B,
	CMD_CONSORTIA_BANK = 0x0C,
	CMD_CONSORTIA_UNION = 0x0D,
	CMD_PLAYER_OFFICER_ITEM_TABLE = 0x0E,
	CMD_PLAYER_ITEM_STORAGE_TABLE = 0x0F,
	CMD_SOLDIER_PARAM_TABLE = 0x10,
//	CMD_RACE_GENIUS_TABLE = 0x11,
	CMD_PLAYER_OFFICER_TABLE = 0x12,
	CMD_PLAYER_SOLDIER_TABLE = 0x13,
	CMD_PLAYER_RESOURCE = 0x14,
	CMD_CONSTRUCTION_CONFIGURE = 0x15,
	CMD_PLAYER_CONSTRUCTION = 0x16,
	CMD_PLAYER_TASK = 0x17,
	CMD_CONSORTIA_PERSONNEL = 0x18,
	CMD_PRIZE_VENDUE = 0x19,
	CMD_PRIZE_DISTRIBUTE = 0x1A,
	CMD_CELL_PRICE = 0x1B,
	CMD_CITY_ATTRIBUTE = 0x1C,
	CMD_CREATE_CONSORTIA = 0x1D,
	CMD_CONSORTIA_FIGHT = 0x1E,
	CMD_CONTRIBUTE_CONFIGURE = 0x1F,
	CMD_PRODUCTION_AMOUNT = 0x20,
	CMD_TASK_BASE_TABLE = 0x21,
	CMD_PRODUCE_CONFIGURE = 0x22,
	CMD_CHAT_ROOM_TABLE = 0x23,
	CMD_PLAYER_CHAT_ROOM_TABLE = 0x24,
	CMD_PLAYER_CITY = 0x25,
	CMD_ALLOT_OFFICER = 0x26,
	CMD_OFFICER_ITEM_ALLOT = 0x27,
	CMD_OFFICER_TECH = 0x28,
	CMD_WEAPON_ATTRIBUTE = 0x29,
	CMD_DEFEND_ATTRIBUTE = 0x2A,
	CMD_OFFICER_JOB_ATTRIBUTE = 0x2B,
	CMD_OFFICER_RACE_ATTRIBUTE = 0x2C,
	CMD_ARMY_OUT_TABLE = 0x2D,
	CMD_NPC = 0x2E,
	CMD_MAP_INFO_TABLE = 0x2F,
	CMD_RESEARCH_ITEM = 0x30,
	CMD_COUNT_RESOURCE = 0x31,
	CMD_ECTYPE_BASE_TABLE = 0x32,
	CMD_NPC_ARMY_TABLE = 0x33,

	CMD_PLAYER_CAPTIVE_HERO = 0x34,
	CMD_EQUIP_ATTRIBUTE = 0x35,
	CMD_WAR_NOTE = 0x36,
	CMD_PULLULATE_NOTE = 0x37,
	CMD_MAIL_TABLE = 0x38,
	CMD_FIREND_TABLE = 0x39,
	CMD_NEWS_TABLE = 0x3A,
	CMD_CONSORTIA_OFFICIAL = 0x3B,
	CMD_LOAD_MATERIAL = 0x3C,
	CMD_TREASURY = 0x3D,
	CMD_CARD_CONFIG = 0x3E,
	CMD_PUBLIC_BUILD_PARAM_TABLE = 0x3F,
	CMD_PUBLIC_BUILD_TABLE = 0x40,
	CMD_PLAYER_PRODUCE_STATE = 0x41,
	CMD_CHARGE_SVR_TIMER = 0x42,
	CMD_JOB_PARAM = 0x43,
	CMD_BIN_CONFIG = 0x44,
	CMD_EQUIP_ANTAGONISM = 0x45,
	CMD_BATTTLE_FIELD = 0x46,
	CMD_CONSORTIA_COMPARE = 0x47,
	CMD_PLAYER_COMPARE = 0x48,
	CMD_COUNCIL = 0x49,
	CMD_CANDIDATE = 0x4A,
	CMD_BALLOT = 0x4B,
	CMD_COUNTRY = 0x4C,
	CMD_RESEARCH_ACTIVITY = 0x4D,
	CMD_LEVY_SOLDIER_ACTIVITY = 0x4E,
	CMD_PLAYER_SKILL_TABLE = 0x4F,
	CMD_LIMIT_TABLE = 0x50,
	CMD_PLAYER_DAY_TASK = 0x51,
	CMD_CONSORTIA_CITY_INFO = 0x52,
	CMD_TRAINING_SYS_TABLE = 0x53,

	CMD_ACTION_APPEND = 0xF1,
	CMD_ACTION_UPDATE = 0xF2,
	CMD_ACTION_DELETE = 0xF3,
	CMD_ACTION_SELECT = 0xF4,
	CMD_ACTION_MULTI_SELECT = 0xF5,
	CMD_ACTION_SELECT_ALL = 0xF6,
	CMD_ACTION_INCREASE = 0xF7,
	CMD_ACTION_SAVE_TO_DB = 0xF8
};



/***************************************************************************
// 战斗服务器通信协议命令号
***************************************************************************/
enum PROTOCOL_FIGHT
{
	REQ_ENTER_FIGHT_SYSTEM = 0x4001, //进入战斗系统
	RSP_ENTER_FIGHT_SYSTEM,
	RSP_MOVE_INFO,					//移动信息
	RSP_FIGHT_INFO,					//战斗信息
	REQ_SELECT_ATTACK_TARGET,		//微操选择进攻目标队伍
	RSP_ATTACK_TARGET_INFO,          //返回
	REQ_SELECT_TRUST_STRATEY,		//托管选择战术
	RSP_SELECT_TRUST_STRATEY,
	REQ_SELECT_TRUST_TARGET,		//托管选择攻击目标
	REQ_SELECT_MOVE_POS,			//选择移动到的位置
	RSP_SELECT_MOVE_POS,
	REQ_SET_PATROL_STATE,			//设置巡逻
	RSP_SET_PATROL_STATE,
	REQ_CANCEL_TRUST_INFO,			//取消托管
	RSP_CANCEL_TRUST_INFO,
	REQ_CANCEL_PATROL_INFO,			//取消巡逻
	RSP_CANCEL_PATROL_INFO,
	REQ_SET_ARMY_DEFEND_INFO,       //设置队伍防御状态
	RSP_SET_ARMY_DEFEND_INFO,
	RSP_SEND_VIRSUAL_INFO,   // 发送视野
	DESTROY_ARMY_INFO,				//歼灭一个队伍
	ADD_ARMY_INFO,					//
	REQ_EXIT_FIGHT_SYSTEM_INFO,		//退出战斗系统
	REQ_ARMY_INFO,                  //
	RSP_ARMY_INFO,					//请求队伍信息
	REQ_LEAVE_FIGHT_SYSTEM_INFO,
	RSP_LEAVE_FIGHT_SYSTEM_INFO,
	REQ_ARMY_GO_OUT_INFO,  //队伍出征
	RSP_ARMY_GO_OUT_INFO,
	REQ_ARMY_OUT_CITY_INFO, //队伍出城
	RSP_ARMY_OUT_CITY_INFO, //队伍出城
	REQ_IN_CITY_ARMY_INFO, //获取城内队伍列表
	RSP_IN_CITY_ARMY_INFO, 
	RSP_UPDATE_ARMY_INFO,
	//////////////////ectype/////////////////////
	REQ_ENTER_HALL_INFO, // 进入副本大厅
	RSP_ENTER_HALL_INFO,
	REQ_ENTER_HALL_INFO_FROM_ARMY_LIST,
	REQ_GET_ONE_ROOM_INFO, //获取一个队伍信息
	RSP_GET_ONE_ROOM_INFO,
	REQ_ENTER_ONE_ROOM_INFO, //进入一个队伍
	RSP_ENTER_ONE_ROOM_INFO,
	REQ_CREATE_ROOM_INFO,  //创建一个队伍
	RSP_CREATE_ROOM_INFO,
	REQ_ADD_ECTYPE_ROOM_INFO, //加入到一个队伍
	RSP_ADD_ECTYPE_ROOM_INFO,
	REQ_ADD_ARMY_TO_ROOM_INFO,//添加队伍到一个队伍
	RSP_ADD_ARMY_TO_ROOM_INFO,
	REQ_KICK_OUT_OTHER_INFO, //踢出一个出队伍
	RSP_KICK_OUT_OTHER_INFO,
	REQ_ENTER_ECTYPE_SYSTEM_INFO, //进入副本系统
	RSP_ENTER_ECTYPE_SYSTEM_INFO,
	REQ_LEAVE_ECTYPE_HALL_INFO, //离开副本大厅
	RSP_LEAVE_ECTYPE_HALL_INFO,
	REQ_SEND_NOTICE_INFO, //发送公告
	RSP_SEND_NOTICE_INFO,
	REQ_ECTYPE_READY_INFO, //准备完成
	RSP_ECTYPE_READY_INFO,
	REQ_GET_ARMY_IN_ENTRANCEID, // 副本中获取部队从哪个entranceId 进入的
	RSP_GET_ARMY_IN_ENTRANCEID,
	////////////////npc///////////////////////////
	REQ_ENTER_FIELD_ECTYPE,      // 进入野外副本请求
	RSP_ENTER_FIELD_ECTYPE,      // 
	REQ_ENTER_NPC_SYSTEM_INFO, //进入副本系统
	RSP_ENTER_NPC_SYSTEM_INFO,
	REQ_ENTER_SEE_OTHER_FIGHT_NPC, //进入旁观别人打副本
	RSP_ENTER_SEE_OTHER_FIGHT_NPC,
	REQ_MOVE_ECTYPE_ARMY_INFO,
	RSP_MOVE_ECTYPE_ARMY_INFO, //移动副本中队伍
	REQ_ATTACK_ECTYPE_TARGET_INFO,
	RSP_ATTACK_ECTYPE_TAGGET_INFO,//指令副本中队伍攻击目标
	REQ_LEAVE_ECTYPE_SYSTEM_INFO,
	REQ_EXIT_NPC_SYSTEM_INFO, //退出副本系统
	RSP_EXIT_NPC_SYSTEM_INFO,
	REQ_ECTYPE_GET_ARMY_INFO,//获取副本中队伍的信息
	RSP_ECTYPE_GET_ARMY_INFO,
	REQ_SET_ECTYPE_ARMY_TRUST_INFO, //设置副本中队伍的托管状态
	RSP_SET_ECTYPE_ARMY_TRUST_INFO,
	RSP_NOTICE_END_ECTYPE_INFO, // 通知 结束副本

	REQ_BACK_ECTYPE_HALL_INFO, //返回大厅
	RSP_BACK_ECTYPE_HALL_INFO,

	RSP_BACK_SINGLE_ECTYPE_ROOM_INFO, // 返回单人副本的创建的房间

	REQ_BACK_SINGLE_ECTYPE_INFO, // 退出单人副本
	RSP_BACK_SINGLE_ECTYPE_INFO, 

	RSP_DELETE_ARMY_INFO,     //进入副本通知玩家删除队伍

	RSP_SEND_PUBLIC_BULID_INFO,

	REQ_ECTYPE_DELETE_ARMY_INFO, //删除副本中的队伍
	RSP_ECTYPE_DELETE_ARMY_INFO, //

	//
	RSP_GET_BOOTY_INFO, // 通知获得的站利品

	RSP_FIGHT_PACK_INFO,
	RSP_USE_ECAPE_CARD_INFO,
	RSP_ONE_TRUN_DEAL_NUM,
	RSP_TURN_INFO,
	RSP_ECTYPE_TURN_INFO,
	RSP_HERO_UPGRADE_LEVEL, //英雄升级
	RSP_ECTYPE_GET_RES,//副本中获取资源或则宝箱
	RSP_FIGHT_VOER_STATIC_INFO,//统计战斗结束信息


	///test 
	REQ_TEST_ENTER_ECTYPE_SYSTEM_INFO,
	RSP_TEST_ENTER_ECTYPE_SYSTEM_INFO,

	REQ_ECTYPE_SYSTEM_ARMY_RETREAT,//副本中撤退

	BROAD_MSG_OBTAIN_BOOTY,  // 广播获得物品消息

	ADD_ARMY_TO_ROOM, //向正在运行的副本、战场等房间添加部队

	EVENT_ARMY_LEAVE, //部队离开郡消息

	RSP_ADD_ARMY_TO_ROOM , //回应向正在运行的副本、战场等房间添加部队

	EVENT_ARMY_LEAVE_ECTYPE, // 部队离开副本
	EVENT_ECTYPE_OVER , //副本结束消息

	RSP_BATTLE_FIELD_WAITING_INFO,  // 战场等待命令

	REQ_CANCEL_BATTLE_FIELD_WAITING_INFO,  // 取消战场等待

	RSP_CANCEL_BATTLE_FIELD_WAITING_INFO,  // 取消战场等待，反馈
	MSG_UPDATE_RES , //刷新资源消息
	MSG_SPECIALLY_EFFECT , //特效消息
	MSG_CLEAR_EFFECT , //清除特效消息
	MSG_CHANGE_COUNTRY , //改变阵营
	REQ_USE_PROP_CARD , //请求使用道具卡
	RSP_USE_PROP_CARD, //回复使用道具卡

	EVENT_FORCE_VEER, //强制跳转

	EVENT_ARMY_LEAVE_OR_DIE_IN_ECTYPE, // 部队退出或死亡或副本结束

	ECTYPE_SERVER_REGISTER_IN_GAMEHALL,  // 副本exe起来时，注册到游戏大厅服务器所发的消息
	EVENT_ACTION_ATTACK_TARGET, //攻击特定目标的动作消息
	EVENT_ACTION_ATTACK_PONIT , //攻击特定地点的动作消息
	MSG_CARD_COOLING_INFO //卡片冷却信息
};
/**************************************************************************
//聊天服务器
**************************************************************************/
enum PROTOCOL_CHAT
{
	REQ_ENTER_CHAT_SYSTEM = 0x5001,
	REQ_ENTER_CHAT_ROOM,
	RSP_ENTER_CHAT_ROOM,
	REQ_SNED_CHAT_INFO,
	RSP_SEND_CHAT_INFO,
	REQ_EXIT_CHAT_SYSTEM,
	RSP_EXIT_CHAT_SYSTEM,
	REQ_EXIT_CHAT_ROOM,
	RSP_EXIT_CHAT_ROOM,
	REQ_INVITE_OTHER_ENTER_CHAT_ROOM,
	RSP_INVITE_OTHER_ENTER_CHAT_ROOM,
	RSP_OTHER_ENTER_CHAT_ROOM,
	REQ_CHANGE_MAP_INFO,
	/// mail system//////
	REQ_ENTER_MAIL_SYSTEM_INFO,
	RSP_ENTER_MAIL_SYSTEM_INFO,
	REQ_GET_MAIL_BODY_INFO,
	RSP_GET_MAIL_BODY_INFO,
	REQ_SEND_MAIL_INFO,
	RSP_SEND_MAIL_INFO,
	REQ_DELETE_MAIL_INFO,
	RSP_DELETE_MAIL_INFO,
	RSP_NEW_MAIL_ARRIVE_INFO,

	/////firend system ///
	REQ_ENTER_FRIEND_SYSTEM_INFO,
	RSP_ENTER_FRIEND_SYSTEM_INFO,
	REQ_ADD_FRIEND_INFO,
	RSP_ADD_FRIEND_INFO,
	REQ_DELETE_FRIEND_INFO,
	RSP_DELETE_FRIEND_INFO,
	REQ_ACCEPT_ADD_FRIEND_INFO,
	RSP_ACCEPT_ADD_FRIEND_INFO,
	RSP_INVITE_ADD_FRIEND_INFO,
	RSP_NOTIFY_FRIEND_ONLINE,
	RSP_NOTIFY_FRIEND_DOWNLINE,

	//////////////////////////////News Report/////////////////////////////
	REQ_NEWS_REPORT_INFO,
	RSP_NEWS_REPROT_INFO,
	NOTIFY_ADD_NEWS_REPROT_INFO,

};

/***************************************************************************
// 在地图服务器通信协议命令号
***************************************************************************/
enum PROTOCOL_MAP
{
	EN_C_VALIDATE = 0x1011,						//验证码
	EN_S_MAP_LIST_INFO = 0x1012,				//大地图信息
	EN_S_CITY_LIST_INFO = 0x1013,				//城市列表信息
	EN_S_PLAYER_BASE_INFO = 0x1014,			//玩家基本信息
	EN_S_PLAYER_CITY_LIST = 0x1015,			//玩家城市信息

	EN_C_COMEIN_MAP = 0x1016,				//进入郡内
	EN_S_ARMYS_LIST = 0x1017,				//郡内部队列表信息
	EN_S_CITY_ATTRIBUTE_LIST,				//郡内哨塔等属性 

	EN_C_CREATE_OFFICER,			//创建指挥官
	EN_S_CREATE_OFFICER_FAILD,		//创建指挥官失败
	EN_C_GET_OFFICER_INFO,			//获得指挥官信息
	EN_S_OFFICER_INFO,				//指挥官信息

	EN_C_CONTRIBUTE_INFO,			//捐献信息
	EN_S_CONTRIBUTE_INFO,			//捐献信息
	EN_C_CONTRIBUTE,				//捐献

	EN_C_GET_MAP_INFO,				//获取郡地图信息
	EN_S_SEND_MAP_INFO,				//郡地图信息

	EN_C_GET_CITY_INFO,				//获取城市信息
	EN_S_SEND_CITY_INFO,			//城市信息

	EN_C_GET_LAND_INFO,				//获取地形信息
	EN_S_SEND_LAND_INFO,			//地形信息

	EN_S_PLAYER_RESOURCE_COUNT,		//玩家资源量

	//建筑码
	EN_C_BUILD_INFO,				//建筑信息
	EN_C_GET_CONSTRUCTION_COUNT,	//得到建筑物数量
	EN_S_CONSTRUCTION_COUNT,		//建筑物数量
	EN_C_DEMOLISH_CONSTRUCTION,		//拆毁玩家在城市中的所有建筑物
	EN_C_BUILD_CONSTRUCTION,		//建筑/升级建筑物
	EN_C_CANCEL_BUILD,				//取消建筑物建筑/升级
	EN_C_DEMOLISH_A_CONSTRUCTION,	//拆毁玩家在城市中的1建筑物

	//生产,研究消息码
	EN_C_PRODUCE_WEAPON_COUNT,		//可生产武器数量
	EN_S_PRODUCE_WEAPON_COUNT,		//可生产武器数量
	EN_C_PRODUCE_WEAPON,			//武器生产

	EN_C_PRODUCE_DEFEND_COUNT,		//可生产防具数量
	EN_S_PRODUCE_DEFEND_COUNT,		//可生产防具数量
	EN_C_PRODUCE_DEFEND,			//生产防具

	EN_C_PRODUCE_SPECIAL_COUNT,		//可生产特殊工具数量
	EN_S_PRODUCE_SPECIAL_COUNT,		//可生产特殊工具数量
	EN_C_PRODUCE_SPECIAL,			//生产特殊工具

	EN_C_PRODUCE_ANIMAL_COUNT,		//可生产驯兽数量
	EN_S_PRODUCE_ANIMAL_COUNT,		//可生产驯兽数量
	EN_C_PRODUCE_ANIMAL,			//生产驯兽

	EN_C_RESEARCH_ITEM,				//研究

	EN_C_WEAPON_RESEARCH,			//武器研究
	EN_S_WEAPON_RESEARCH,			//武器研究
	EN_C_RESEARCH_WEAPON,			//研究武器
	
	EN_C_DEFEND_RESEARCH,			//防具研究
	EN_S_DEFEND_RESEARCH,			//防具研究
	EN_C_RESEARCH_DEFEND,			//研究防具

	EN_C_ANIMAL_RESEARCH,			//驯兽研究
	EN_S_ANIMAL_RESEARCH,			//驯兽研究
	EN_C_RESEARCH_ANIMAL,			//研究驯兽

	EN_C_SPECIAL_RESEARCH,			//特殊工具研究
	EN_S_SPECIAL_RESEARCH,			//特殊工具研究
	EN_C_RESEARCH_SPECIAL,			//研究特殊工具

	//征兵,训练消息码
	EN_C_LEVY_SOLDIER,				//征兵处
	EN_S_LEVY_SOLDIER_PARAM,		//征兵参数
	EN_C_LEVY_SOLDIER_COUNT,		//征兵数量
	EN_C_EXE_SOLDIER,				//训练兵种
	EN_S_EXE_SOLDIER_PARAM,			//训练兵种参数
	EN_S_PLAYER_SOLDIER_COUNT,		//玩家士兵数
	EN_S_EXE_SOLDIER,				//
	EN_C_EXE_SOLDIER_COUNT,			//要训练的士兵数

	//部队出征
	EN_C_ARMY_GO_OUT,				//部队出征
	EN_S_ARMY_LIST,					//部队列表
	EN_C_GO_OUT_ARMY,				//出征部队

	EN_C_ARMY_OUT_CITY,				//部队出城
	EN_S_ARMY_OUT_CITY,				//部队出城

	EN_C_ARMY_REACH,				//部队达到
	EN_S_ARMY_REACH,				//部队达到

	//部队消息码
	EN_C_ARMY_EDIT,					//部队组编
	EN_C_EDIT_ARMY,					//组编部队
	EN_S_EDIT_ARMY,
	EN_C_ARMY_CREATE,				//部队创建
	EN_C_CREATE_ARMY,				//创建部队
	EN_S_CREATE_ARMY_FAILD,			//创建部队失败
	EN_C_UNLAYARMY,					//解散部队
	EN_S_UNLAYARMY,
	EN_C_DELETEARMY,				//删除部队
	EN_S_DELETEARMY,
	EN_S_ALL_SOLDIERS,				//玩家所有士兵数
	EN_C_GET_OFFICERS,				//指挥官数
	EN_S_ALL_OFFICERS,				//有所指挥官数
	EN_S_FREE_OFFICERS,				//空闲指挥官列表
	EN_C_GET_ARMY_INFO,				//部队
	EN_S_SEND_ARMY_INFO,			//部队信息
	EN_C_ARMY_MOVE,					//部队要移动的位置
	EN_S_ARMY_MOVE,					//部队移动
	EN_S_WEAPON_LIST,				//武器列表
	EN_S_DEFEND_LIST,				//防具列表

	//市场
	EN_C_COUNTRY_MARKET,				//国家市场
	EN_C_COUNTRY_MARKET_BUSINESS,		//国家市场买卖
	//民间市场
	EN_C_MARKET_WEAPON_LIST,			//武器列表
	EN_S_MARKET_WEAPON_LIST,			//武器列表
	EN_C_MARKET_DEFEND_LIST,			//防具列表
	EN_S_MARKET_DEFEND_LIST,			//防具列表
	EN_C_MARKET_OFFICER_ITEM_LIST,		//英雄装备列表
	EN_S_MARKET_OFFICER_ITEM_LIST,		//英雄装备列表
	EN_C_BASE_RESOURCE_LIST,			//基本资源列表
	EN_S_BASE_RESOURCE_LIST,			//基本资源列表

	EN_C_MARKET_GOODS_LIST,				//物品列表
	EN_S_MARKET_GOODS_LIST,				//物品列表
	EN_C_MARKET_GOODS_NEXT,				//物品列表下一页
	EN_C_MARKET_PUTUP,					//挂牌
	EN_C_MARKET_BIDDEN,					//出价
	EN_C_MARKET_BUY,					//一口价

	//战利品拍卖
	EN_C_VENDUE_GOODS_LIST,			//物品列表
	EN_S_VENDUE_GOODS_LIST,			//物品列表
	EN_C_VENDUE_GOODS_NEXT,			//物品列表下一页
	EN_C_VENDUE_BIDDEN,				//拍卖出价
	EN_C_VENDUE_BUY,				//拍卖一口价

	EN_S_MARKET_INFO,					//市场返回信息
	//军团
	EN_C_CREATE_CONSORTIA,			//创建军团
	EN_S_CREATE_CON_FAIL,			//军团创建失败

	EN_C_CON_LIST,					//军团列表
	EN_S_CON_LIST,					//军团列表

	EN_C_CON_PERSONNEL_LIST,		//军团人员列表
	EN_S_CON_PERSONNEL_LIST,		//军团人员列表
	EN_C_CON_PROPOSER_LIST,			//军团申请人员列表
	EN_S_CON_PROPOSER_LIST,			//军团申请人员列表

	EN_C_CON_LOG_MODIFY,			//军团LOG修改
	EN_C_CON_PLACARD_EDIT,			//军团公告修改

	EN_C_DISENGAGE_CON,				//脱离军团
	EN_S_DISENGAGE_CON,				//脱离军团

	EN_C_ALLOW_CON_OFFICIAL,		//军团官员任职
	EN_C_EXEMPT_CON_OFFICIAL,		//军团官员免职
	EN_C_KICK_CON,					//开除成员

	EN_S_ALLOW_CON_OFFICIAL,		//军团官员任职
	EN_S_EXEMPT_CON_OFFICIAL,		//军团官员免职
	EN_S_KICK_CON,					//开除成员

	EN_C_JOIN_CON,					//加入军团
	EN_S_JOIN_CON,					//加入军团

	EN_C_JOIN_CON_AGREE,			//加入军团同意
	EN_C_JOIN_CON_REFUSE,			//加入军团拒绝
	EN_S_JOIN_CON_NOTIFY,			//加入军团通知

	EN_C_CON_FIGHT,					//约战
	EN_S_CON_FIGHT,					//约战

	EN_C_CON_FIGHT_CONSENT,			//约战许可
	EN_S_CON_FIGHT_CONSENT,			//约战许可

	EN_C_CON_UNION,					//结盟
	EN_S_CON_UNION,					//结盟

	EN_C_CON_UNION_CONSENT,			//结盟许可
	EN_S_CON_UNION_CONSENT,			//结盟许可

	EN_C_CON_UNCOIL_UNION,			//解盟
	EN_S_CON_UNCOIL_UNION,			//解盟

	EN_C_CON_COMPARE,				//军团实力排行       比较
	EN_S_CON_COMPARE,				
	EN_C_PLAYER_COMPARE,			//个人物资排行
	EN_S_PLAYER_COMPARE,
	EN_C_OFFICER_COMPARE,			//军衔排行
	EN_S_OFFICER_COMPARE,

	EN_C_MODIFY_OFFICER_NAME,		//修改指挥官名字
	EN_C_RECRUIT_OFFICER,			//招募指挥官
	EN_S_ERROR_INFO,				//错误信息
	EN_S_SUCCEED_INFO,				//成功信息

	EN_C_GET_OFFICER_TECH,			//得到指挥官技能
	EN_S_OFFICER_TECH,				//指挥官技能

	EN_C_STUDY_OFFICER_TECH,		//学习指挥官技能
	EN_S_STUDY_A_TECH,				//学会了一种技能
	EN_S_RECRUIT_A_OFFICER,			//招募得到一个指挥官

	EN_S_BUILDING_CONSTR,			//正在建筑中的建筑物
	EN_S_PRODUCEING_ITEM,			//正在生产中的道具
	EN_S_RESEARCHING_ITEM,			//正在研究中的道具

	EN_C_CANCEL_PRODUCE,			//取消生产道具

	EN_C_CANCEL_RESEARCH,			//取消科研道具
	
	EN_C_GET_ALL_RESEARCH_ITEM,		//得到所有已经研究了的道具列表
	EN_S_SEND_ALL_RESEARCH_ITEM,	//发送所有已经研究了的道具列表

	EN_S_PLAYER_CITY_MODFIY,		//玩家城市改变
	EN_S_MAP_MODFIY,				//郡的状态改变(战斗开始/结束);

	EN_C_ANIMAL_LIST,				//驯兽技术列表
	EN_S_ANIMAL_LIST,				//驯兽技术列表

	EN_C_SPECIAL_LIST,				//特殊工具列表
	EN_S_SPECIAL_LIST,				//特殊工具列表

	EN_S_LEVYING_SOLDIER,			//正处于招兵中...
	EN_S_LEVY_SOLDIER_INFO,			//征兵返回信息
	EN_S_OUTING_ARMYS_LSIT,			//正处于出征中的部队列表
	EN_C_ARMYS_TRANSFER,			//部队转移

	EN_S_HERO_SYSTEM_INFO,			//英雄系统信息
	EN_S_NOT_HEADAM_HERO,			//没有首领英雄
	EN_S_HAVE_HEADAM_HERO,			//有首领英雄
	EN_S_CREATE_HEADAM_HERO,		//创建首领英雄成功
	EN_S_START_RECRUIT_HERO,		//开始招募英雄
	EN_C_DISMISS_HERO,				//开除英雄
	EN_S_DISMISS_HERO_SUCCEED,		//开除英雄成功
	EN_C_REDEEM_HERO,				//赎回英雄
	EN_S_REDEEM_HERO_SUCCEED,		//赎回英雄成功
	EN_C_OFFICERO_ATTRIBUTE,		//指挥官属性
	EN_S_OFFICERO_ATTRIBUTE,		//指挥官属性
	
	EN_C_EQUIPS_LIST,				//得到所有装备
	EN_S_EQUIPS_LIST,				//所有装备
	
	EN_C_PACK_EXCHANGE,				//操作背包中的装备
	EN_C_DESTROY_EQUIP,				//销毁背包中的装备
	
	EN_S_DESTROY_EQUIP_SUCCEED,		//销毁装备成功
	EN_S_DESTROY_EQUIP_FAILED,		//销毁装备失败

	EN_S_EXCHANGE_EQUIP,			//操作装备

	EN_S_EQUIP_POS_ERROR,			//装备与英雄身上的部位不对应
	EN_S_JACKET_SUCCEED,			//换上装备成功
	EN_S_GETOFF_SUCCEED,			//脱下装备成功
	EN_S_JACKET_FAILED,				//换上装备失败
	EN_S_GETOFF_FAILED,				//脱下装备失败

	EN_S_JACKET_PACK_SUCCEED,		//换入背包中成功
	EN_S_POP_PACK_SUCCEED,			//从背包中取出成功
	EN_S_JACKET_PACK_FAILED,		//换换入背包中失败
	EN_S_POP_PACK_FAILED,			//从背包中取出失败
	EN_S_EQUIP_CANNOET_OVERLAP,		//装备不能重叠

	EN_C_HERO_WAR_RECORD,				//得到英雄战争记录
	EN_S_HERO_WAR_RECORD,				//英雄战争记录
	EN_C_HERO_GROWUP_RECORD,			//得到英雄成长记录
	EN_S_HERO_GROWUP_RECORD,			//英雄简成长记录

	EN_S_GAME_TIME,					//游戏中的时间
	
	EN_C_LOAD_MATERIAL,				//装载物资
	EN_C_UNINSTALL_MATERIAL,		//卸载物资
	EN_C_LOOK_MATERIAL,				//物资查看
	EN_S_LOOK_MATERIAL,				//物资查看
	EN_C_COLONIZE_CITY,				//殖民城市

	EN_S_BROADCAST_CONTRIBUTE,		//广播捐献信息
	EN_S_BROADCAST_MILITARY_RANK,	//军衔晋升
	EN_S_BROADCAST_CAPTIVE_HERO,	//英雄被俘
	EN_S_BROADCAST_COMEIN_EXIT_MAP,//进入/离开地图
	EN_S_BROADCAST_ARMY_WARM,			//部队达到预警
	EN_S_ARMY_MOVE_PATH,			//部队行走路径
	EN_S_ARMY_CANT_MOVE,			//部队不能经过
	EN_S_RESOURCE_RATE,				//资源采集率
	EN_S_RESOURCE_PERCENTRATE,		//资源采集率(百分比)
	EN_C_EXIT_MAP_SERVER,			//退出大地图服务器

	//task 
	EN_S_TASK,
	EN_S_TASK_GET_LIST,				//获取任务列表
	EN_C_TASK_GET_LIST,				//
	EN_S_TASK_ACCEPT,				//接受任务
	EN_C_TASK_ACCEPT,
	EN_S_TASK_COMPLATE,				//提交任务
	EN_C_TASK_COMPLATE,
	EN_S_TASK_DELETE,				//删除任务
	EN_C_TASK_DELETE,				//
	EN_S_TASK_ISSUE	,				//发布任务	
	EB_C_TASK_ISSUE,
	//宝箱相关
	EN_C_NEWTREASUREPACK,			//增加一个新的宝箱到英雄背包
	EN_S_NEWTREASUREPACK,

	EN_C_OPENTREASUREPACK,			//打开宝箱
	EN_S_OPENTREASUREPACK,

	EN_C_HEROGBH_NOTIFY,			//英雄重伤通知,战斗服务器发送给大地图

	EN_C_TIMER_NOTIFY,				//统一的定时器通知事件

	EN_C_IMMEDCOMPLETE,				//立即完成
	EN_S_IMMEDCOMPLETE,

	EN_C_RESETMAPINFO,				//重置郡地图信息

	EN_C_GETARMYLIST,				//获取玩家部队列表
	EN_S_GETARMYLIST,

	EN_C_MODIFYARMYSTATE,			//战斗服务、副本发给大地图的通知消息，部队状态改变
	EN_S_MODIFYARMYSTATE,			//大地图服务器下发到玩家，部队状态改变

	EN_S_HEROGBH_NOTIFY,			//英雄重伤通知，全服广播

	EN_C_DESTROYCITY,				//玩家拆除一个据点
	EN_S_DESTROYCITY,

	EN_C_QUERYCAPTIVELIST,			//查询俘虏信息
	EN_S_QUERYCAPTIVELIST,

	EN_C_CREATENPCARMY,				//创建NPC部队
	EN_S_CREATENPCARMY,

	EN_C_DELNPCARMY,				//删除NPC部队
	EN_S_DELNPCARMY,

	EN_C_FIGHT_CREATEARMY,			//通知战斗线程新增部队
	EN_S_FIGHT_CREATEARMY,

	EN_C_NOTIFY_MAPSTATE,			//郡服务器通知大地图服务器郡状态改变
	EN_S_NOTIFY_MAPSTATE,

	EN_C_MODIFY_PUBLICBUILD,		//公共建筑升级
	EN_S_MODIFY_PUBLICBUILD,

	EN_C_INLAYEQUIP,				//精练（镶嵌）装备
	EN_S_INLAYEQUIP,

	EN_S_NEWITEMINPACK,				//背包中新增一物品，服务器主动下发

	EN_C_USEADDSOLDIERCARD,						
	EN_S_USEADDSOLDIERCARD,


	EN_C_ECTYPE_CONTRIBUTE_INFO,			//捐献信息

	EN_C_CONVERT,							//兑换
	EN_S_CONVERT,

	EN_S_BROAD_SPECIALHERO,					//招募到特殊英雄进行广播,

	EN_C_COMMONOPENCARD,						//打开卡，统一右键点击就能打开的卡（原有右键打开的采用原有 方式，以后右键打开的卡可采用统一的协议）
	EN_S_COMMONOPENCARD,

	EN_C_MODIFYHERONAME,					//使用改名卡修改英雄的名字
	EN_S_MODIFYHERONAME,

	EN_S_NOTIFYCONSTRUCTION,				//下发建筑完成通知4389
	EN_S_NOTIFYRESEARCH,					//下发科研完成通知4390
	EN_S_NOTIFYPRODUCT,						//下发生产完成通知4391
	EN_S_NOTIFYCONSCRIPTION,				//下发征兵完成通知4392

	EN_S_BROADCONSTRUCTION,					//广播建筑完成消息4393
	EN_S_BROADRESEARCH,						//广播科研完成消息4394
	EN_S_BROADPRODUCT,						//广播生产完成消息4395
	EN_S_BROADCONSCRIPTION,					//广播征兵完成消息4396
	EN_S_BROADHEROEQUIP,					//广播得到装备(打开宝箱)4397
	EN_S_BROADHEROSKILL,					//广播学习技能4398
	EN_S_BROADINLAYEQUIP,					//广播精练消息4399

	EN_S_NOTIFYPLAYERONLINE,				//大地图通知各服务器玩家是否在线4400

	EN_S_NOTIFYMARKET,						//下发市场卖商品成功通知4401
	EN_C_NOTIFY_DAYTASKOFPLAYER,			//玩家待完成的日常任务通知，任务通知战斗
	EN_S_NOTIFY_DAYTASKOFPLAYER,

	EN_C_RELEASECAPTIVE,						//释放英雄
	EN_S_RELEASECAPTIVE,

	EN_C_NOTIFY_EXPORCREDIT,				//英雄获取经验或者荣誉点通知

	EN_REQ_HEROS_HANGINGSYS,                // 双击修炼门获取玩家所有英雄的挂机情况或者当某一个英雄进入修炼门的感应区域发次请求
	EN_RSP_HEROS_HANGINGSYS,                // 返回玩家所有在挂机状态下的英雄或者英雄部队刚进入感应区的影响相关信息

	EN_REQ_HANG_ONEHERO,                  // 请求挂机一个英雄
	EN_RSP_HANG_ONEHERO,                    // 请求挂机一个英雄的反馈
	EN_REQ_CANCEL_HANG_ONEHERO,             // 请求取消一个英雄的挂机
	EN_RSP_CANCEL_HANG_ONEHERO,             // 请求取消一个英雄的挂机的反馈
	EN_RSP_TIMEOUT_HANG_ONEHERO,            // 挂机某个英雄到时了



	EN_S_END_FIGHT	= 0x1300				//结束战斗

};
//英雄状态
enum HERO_STATE
{
	EN_LEISURE=0,										//0,空闲 
	EN_HAVE_ARMY,										//1,带队
	EN_GBH,												//2,重伤 
	EN_BY_CAPTIVE,										//3,被俘 
	EN_CAPTIVE, 										//4 俘虏
	EN_HANGMACHINE,										//5.挂机
};
/*装备类型
enum EQUIP_TYPE
{
	EN_HEADPIECE=1,		//1 头盔（headpiece）
	EN_SHIELD,			//2 护甲(shield) 
	EN_CESTUS,			//3 腰带(cestus) 
	EN_GLOVE,			//4 手套(glove) 
	EN_BOOT,			//5 靴子(boot) 
	EN_RIDE_ON,			//6 坐骑()
	EN_WEAPON,			//7 武器(arms)
	EN_ACCOUTERMENT1,	//8 饰物1(accouterment) 
	EN_ACCOUTERMENT2,	//9 饰物2
	EN_ACCOUTERMENT3,	//10 饰物3
	EN_RESOURCECARD,	//11基本资源卡
	EN_RESOURCESPEEDCARD,//12基本资源生产加速卡
	EN_OTHERSPEEDCARD,	//13建造、科研、征兵加速卡，只加速某一具体项
	EN_RECRUITHERO,		//14招募英雄卡
	EN_SPECIALSKILLCARD,//15特殊技能卡
	EN_HEROSAFECARD,	//16英雄安全卡
	EN_HEROEQUIPCARD,	//17英雄装备卡
	EN_AUTOCARD,		//18托管卡
	EN_NO_EQUIP			//11 非可装备(装备equip)
};*/
//物品类型
enum GOODS_TYPE
{
	WEAPON_ITEM=1,			//武器类型
	DEFEND_ITEM=2,			//防具类型
	ANIMALS_TAME_ITEM=3,	//野兽驯服
	SPECIAL_TOOL_ITEM=4,	//特殊工具
	EN_HERO_EQUIP=5,		//英雄装备
	EN_BASIC_RESOURCE=6		//基本物资
};
//基本资源类型
enum BASE_RESOURCE_TYPE
{
	EN_FOOD=1,			//粮食
	EN_WOOD,			//木材
	EN_IRON,			//铁
	EN_SKIN,			//皮
	EN_YELLOW_STONE,	//黄宝石
	EN_RED_STONE,		//红宝石
	EN_GREEN_STONE,		//绿宝石
	EN_BLACK_STONE		//黑铁
};
//背包
enum PACK_TYPE
{
	EN_HERO_BODY=1,			//1:英雄身上 
	EN_HERO_PACK,			//2:英雄背包
	EN_COMM_PACK,			//3: 公共背包
	EN_PACK_GRID_NUM=50		//背包的格子数
};
//装备操作类型
enum EXCHANGE_EQUIP_MODE
{
	EN_PUT_ON_EQUIP=1,		//1:穿上装备,
	EN_GET_OFF_EQUIP,		//2:脱下装备 
	EN_PUSH_EQUIP,			//3:换入背包中,
	EN_POP_EQUIP,			//4:从背包中取出,
	EN_CHANGE_EQUIP			//5:同个背包中装备交换位置
};


enum CardType
{
	//资源类0-99
	ResourceCard_Begin = 0,
	Food_Card,						//粮食卡 1
	Wood_Card,						//木材卡 2
	Skin_Card,						//皮卡 3
	Iron_Card,						//铁卡 4
	RedStone_Card,					//红宝石卡 5
	YollowStone_Card,				//黄宝石卡 6
	GreenStone_Card,				//绿宝石卡 7
	BackIron_Card,					//黑铁卡 8
	ResourceCard_End = 100,			//最大资源类型 

	//提速类
	Resource_SpeedCard_Begin = 101,
	AllResource_Speed_Card,			//生产提速卡，基本资源生产提升卡 102
	Food_Speed_Card,				//粮食提速卡 103
	Wood_Speed_Card,				//木材提速卡 104
	Skin_Speed_Card,				//皮卡提速卡 105
	Iron_Speed_Card,				//铁卡提速卡 106
	Resource_SpeedCard_End = 150,		//

	OtherSpeedCard_Begin=151,
	Construction_Speed_Card,		//建筑提速卡 152
	Research_Speed_Card,			//科研提速卡 153
	Conscription_Speed_Card,		//征兵提速卡 154
	Recruit_Card,					//招募英雄卡 155
	Produce_Speed_Card,				//生产提速卡 156
	OtherSpeedCard_End = 200,		//最大加速卡类型

	//特殊技能卡
	SkillCard_Begin = 201,
	SpecialSkill_Card,				//特殊技能卡 202
	ExpandPack_Card,				//背包扩展卡 203
	SkillCard_End=300,				//最大特殊技能卡

	//英雄用被动卡
	HeroSafeCard_Begin = 301,
	HeroSafe_Card,					//英雄安全类卡 302
	HeroEscape_Card,				//逃脱卡 303
	ClearSkill_Card,				//清除技能卡 304
	ExpeditionSpeed_Card,			//出征加速卡 305
	ModifyHeroName_Card,			//修改英雄名字 306
	HeroSafeCard_End=400,

	//托管卡
	AutoCard_Begin=401,
	Auto_Card,						//托管卡
	AutoCard_End=500,

	//英雄物品卡
	HeroEquipCard_Begin = 501,
	HeroEquip_Card ,					//英雄装备卡 502
	EquipLuck_Card,					//幸运卡 503
	EquipSafe_Card,					//装备安全卡（保底卡）504
	AddSoldier_Card,				//援兵卡 505
	SPEEDUP_CARD,                  //加速卡 506
	SELFBLAST_CARD,                 // 自爆卡 507
	INVINCIBILITY_CARD,            // 无敌卡 508
	MERCENARY_CARD,                //佣兵卡 509
	HeroEquipCard_End=5000
};

enum MailType_Index
{
	MAIL_NEWPLAYER1 = 0,			//新手邮件1					0
	MAIL_NEWPLAYER2,				//							1
	MAIL_NEWPLAYER3,				//新手邮件3					2
	MAIL_UPGRADERANK,				//军衔升级					3
	MAIL_COUNTRY_COUNCIL,			//议员列表					4
	MAIL_COUNTRY_ELECTIONCOUNCIL,   //当选议员					5
	MAIL_COUNTRY_DOD_VOTESTART,		//开始选举国防部长			6
	MAIL_COUNTRY_DOD_VOTEEND,		//选举国防部长结束			7
	MAIL_COUNTRY_DOD_ELECTION,		//当选国防部长				8
	MAIL_COUNTRY_INTERIOR_VOTESTART,		//开始选举内政部长	9
	MAIL_COUNTRY_INTERIOR_VOTEEND,		//选举国防内政结束		10
	MAIL_COUNTRY_INTERIOR_ELECTION,		//当选国防内政			11
	MAIL_TASK_START = 100,			//任务类型以100开始			12
	MAIL_TASK_END = 500
};

typedef enum
{
	IN_CITY		=0, //0 城内
	OUT_CITY_TRUST_CONTROY =1,//1 城外 非托管
	GO_OUT_FOR_BATTLE =2 ,// out for a battle 出征
	OUT_CITY_TRUST_FROUT_N =3,//2 城外 托管 正面进攻 攻击近程
	OUT_CITY_TRUST_FROUT_F =4,//3 城外 托管 正面进攻 攻击远程
	OUT_CITY_TRUST_FLANKS_N =5,//4 城外 托管 迂回进攻 攻击近程
	OUT_CITY_TRUST_FLANKS_F =6,//5 城外 托管 迂回进攻 攻击远程
	OUT_CITY_TRUST_DEFEND	=7,//6 成外 防御 攻击近程
	OUT_CITY_TRUST_PATROL =8,//7 城外 巡逻
	OUT_CITY_TRUST_SPY	=9,//8 城外 侦查
	OUT_CITY_TRUST_ATTACK_CITY=10,//9 城为 托管 攻击城市
	ARMY_FIGHT_ECTYPE =11,   //打副本
	ARMY_ANNIHILATE = 12,  // 队伍被歼灭
	ARMY_FIGHT_FIELD = 13, //部队在野外
	ARMY_FIGHT_LIMIT_BATTLE = 14, //在有限制的战场
	ARMY_FIGHT_UNLIMIT_BATTLE = 15 //在无限制的战场

}ARMY_TRUST_STATE;

enum ARCH_TYPES
{
	ARCH_INVALID = -1,    //非建筑类型
	ARCH_DEFAULT   =0,     //默认非可操作建筑，如树，山...
	ARCH_TOWNHOUSE =1,     //城镇中心
	ARCH_CITY      =2,     //城市
	ARCH_SENTRY    =3,     //岗哨塔
	ARCH_EC_DOOR   =4,     //副本传送门
	ARCH_LOGIN     =5,     //登录点(部队出征到达位置)
	ARCH_LUMBERYARD=6,     //木场
	ARCH_STOPE     =7,     //采矿场
	ARCH_GRINDERY  =8,     //磨房
	ARCH_SKIN      =9,     //皮料加工厂
	ARCH_BUILD_TOFT = 50, //建筑地基，可在上面建设陷阱，栅栏等建筑
	ARCH_TRAP      =51,    //陷阱
	ARCH_BARRIER   =52,    //栅栏
	ARCH_TOFT_END  = 99,   //建筑类型结束，可选建筑类型48种
	ARCH_LOW_LEVEL1 =100,   //特殊类型的建筑，如(路，田)这些做排序使用，要放在所有建筑最下层显示
	ARCH_LOW_LEVEL2 =101,   //特殊类型的建筑，如(河)这些做排序使用，要放在所有建筑最下层显示
	ARCH_LOW_LEVEL3 =102,   //特殊类型的建筑，如(桥 )这些做排序使用，要放在所有建筑最下层显示
	ARCH_WAR_ENTER =103,  //战场传送点
	ARCH_FIELD_COUNTRY = 104, //野外传送点
	ARCH_EC_DOOR3=105,//20100511修炼门
	ARCH_OCCUPY_BUILDING_START = 150, //可占领
	ARCH_RES_BUILDING = 151, //资源建筑
	ARCH_OCCUPY_BUILDING_END = 160, //可占领建筑结束

};

// 地形类型
enum LAND_FORM
{
	LAND_INVALID = -1, //非法地形
	LAND_PLAIN = 0,  // 平原
	LAND_RIVERWAY,    // 河流  1
	LAND_FOREST,     // 森林  2
	LAND_BLOCK       //全阻挡
};

//奖励类型
enum AWARDS_TYPE
{
	AWARDS_SOLDIEREQUIPMENT = 0,		//士兵（部队）装备：武器、防具
	AWARDS_HEROEQUIPMENT,				//英雄装备
	AWARDS_HEROATTRIBUTE,				//英雄属性：荣誉点，经验值，技能点，参看枚举HEROATTRIBUTE_TYPE
	AWARDS_MONEY,						//金钱
	AWARDS_RESOURCE,					//资源：粮食、木材、皮、铁，参看枚举BASE_RESOURCE_TYPE
	AWARDS_CARD,						//卡
	AWARDS_GODIAN,						//GO点
	AWARDS_SOLDIER,						//士兵
};
//英雄属性类型
enum HEROATTRIBUTE_TYPE
{
	ATTRI_HONORPOINT = 0,				//0：荣誉点
	ATTRI_EXPERIENCE,					//经验值
	ATTRI_SKILLPOINT					//技能点
};

enum GAME_HALL_TYPE
{
	HALL_TYPE_INVALID = -1,
	COUNTRYPART = 0,                    // 副本
	FIELD_COUNTRYPART,                  // 野外副本
	LIMIT_BATTLEFIELD,                  //限制型战场
	UNLIMIT_BATTLEFIELD                 //非限制型战场
};
//消息范围
enum MSGRANGE_TYPE
{
	MSGRANGE_TOALL = 0,					//0--所有人可见
	MSGRANGE_TOCOUNTRY,					//1--本阵营
	MSGRANGE_TOPLAYER,					//2--自己方可见
};

//任务完成目标种类
enum TASKCOMPLETETARGETTYPE
{
	TASKTARGET_CONSTRUCTION = 1,		//1：建筑
	TASKTARGET_RESEARCH,				//2：科研
	TASKTARGET_PRODUCTION,				//3：生产道具
	TASKTARGET_SOLDIER,					//4：招募新兵
	TASKTARGET_ARMY,					//5：组建部队:部队数量
	TASKTARGET_SPECIALARMY,				//6：特殊部队：殖民部队、运输部队
	TASKTARGET_PLAYERCITY,				//7：殖民城市:部队数量
	TASKTARGET_CONTRIBUTE,				//8：捐献任务
	TASKTARGET_HERO,					//9：招募英雄
	TASKTARGET_CONSORTIA,				//10：加入军团
	TASKTARGET_FRIENDS,					//11：添加好友
	TASKTARGET_MARKET,					//12：市场挂牌交易
	TASKTARGET_EQUIPMENT,				//13：收集套装
	TASKTARGET_BASERESOURCE,			//14：收集基本资源
	TASKTARGET_MILITARYRANK,			//15：军衔
	TASKTARGET_GOCOUNT,					//16：GO点查询
	TASKTARGET_CARD,					//17：查询卡
	TASKTARGET_ELIMICONSTRUCTION,		//18：消灭某郡中的特定建筑类的目标
	TASKTARGET_ELIMIMONSTER,			//19：消灭某郡中的特定怪物类的目标
	TASKTARGET_ELIMIARMY,				//20：消灭某郡中的特定部队类的目标
	TASKTARGET_COLONIALCITY,			//21：某郡殖民一城市
	TASKTARGET_BATTLEFIELD,				//22：某战场日常任务
};

//从任务头文件ProTaskSystem.h中移过来
/* 任务类型（生产科研，运输，查找，战斗，副本,日常） */
enum TaskType
{
	ttScKy		=	0x01,
	ttYs		=   0x02,
	ttCz		=	0x03,
	ttZd		=	0x04,
	ttFb		=	0x05,
	ttDayTask   =   0x06,			//日常任务
	ttConsortiaTask		=	0x07,			//军团任务
};

/* 任务状态（未接，已接，完成，提交，删除） */
enum TaskState
{
	tsWj		=	0x01,
	tsYj		=	0x02,
	tsWc		=	0x03,
	tsTj		=	0x04,
	tsSc		=	0x05
};
#endif // __INCLUDE_GLOBAL_MACRO_H__
