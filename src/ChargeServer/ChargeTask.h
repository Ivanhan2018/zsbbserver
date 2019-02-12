#ifndef CHARGETASK_H__
#define CHARGETASK_H__
#include <stdio.h>
#include <stdarg.h>
#include "IceUtil.h"
#include "DB_ProxyHeader.h"
#include "DBTableHeader.h"
#include "ChargeProtocol.h"
#include "um_web_client.h"
#include "AutoPlayer.h"
#include <map>
#include "socketsvc.hpp"
#include "MapServerProtocolHeader.h"
#include "ZSBB_DataDefine.h"
#define MAX_MSG_COUNT	32768
#define CONSUMENOTE_FILENAME	"consume_note.csv"

typedef std::map<long,LPNotify_Timer> TimerParam_Map;
typedef std::map<long,LPNotify_Timer>::iterator TimerParam_Iterator;

class CChargeTask:public IceUtil::Task<char *,MAX_MSG_COUNT>
{
public:
	typedef struct ServerParam
	{
		char			AgentIP[SIZE_32];
		unsigned int	AgentPort;
		char			DBGIP[SIZE_32];
		unsigned int	DBGPort;
		char			WebIP[SIZE_32];
		unsigned int	WebPort;
		char			LogSvrIP[SIZE_32];
		unsigned int	LogSvrPort;
		unsigned int	Timeout;
	}ServerParam,*LPServerParam;

	CChargeTask();
	~CChargeTask();
	int InitServer(LPServerParam param);
	virtual bool startTask(unsigned int num_threads, 
		DATA_RECEIVER receiver_func, 
		EVENT_NOTIFY notify_func,
		TIMER_NOTIFY timer_func,
		SOCKET_DISCONNECT disconn_func,
		LOGIC_EXCEPTION exception_func);//设置连接参数后，调用该函数，启动服务
	virtual void stopTask();
	virtual void run();
	virtual void exit(char *msg);
	void ClearParamData();

	int ProcTimer(const void * param);
	LPCardParam GetCardParam(int cardid);					//根据ItemID获取某卡的相信信息
	HeroEquipAttribute* GetEquipAttribute(int itemid);					//根据装备ID获取装备的相信信息

	char *SetLogFormat(LogType logtype,const char *strFormat);
	char *GetLogFormat(LogType logtype);

	char *SetLogPath(const char *path);						//设置日志的路径
	char *GetLogPath();										//

	int AddTimerNotifyMsg(int playerid,int type,LPNotify_Timer param);			//定时器参数加入MAP中
	int DelTimerNotifyMsg(int playerid,int type);
	LPNotify_Timer GetTimerID(int playerid,int type);

	//托管列表的操作
	int AddAutoPlayer(AutoPlayer_Ptr &player_ptr);
	int GetAutoPlayer(int playerid,AutoPlayer_Ptr &player_ptr);
	int DelAutoPlayer(int playerid);
	int GetConstruction(int playerid,int mapid,int cityid,AutoConstruction &autocons,DB_Proxy *pDBProxy,int &count);

	//设置收集资源的时间参数
	int SetCheckResourceTime(int time);
	int GetCheckResourceTime();

	//Go点兑换参数
	int SetExchageRate(int type,int rate);
	int GetExchageRate(int type);

	//加速参数
	LPSpeedParam_Node GetSpeedParam(int type,int value);
	int ReconnectWebServer();

	//背包重叠参数
	PackStoreParam *GetPackStoreParam(int postion);

	//读取建筑物参数表
	ConstructionConfigure * getConstructionConfigure(int constructionid,int level);

	void WriteLog(DB_Proxy *pDBProxy,int svrid,int playerid,const char *format,...);
private:
	virtual bool startTask(std::size_t = 1);
	void AddCardParam(LPCardParam pCardParam);
	int GetCardParamTable(DB_Proxy *pDBProxy);
	int ReadEquipAttribute();
	void ClearEquipAttribute();
	int GetAutoPlayerTable(DB_Proxy *pDBProxy);
	int GetTimerTable(DB_Proxy *pDBProxy);
	void SendCollectComplete(DB_Proxy *pDBProxy,int playerid);

	int InitSpeedParam();
	int InitPackStoreParam();


	//读取建筑配置表
	void readConstructionConfigure();
	void clearConstructionConfigure();

private:
	bool				m_bRuning;
	ServerParam			m_SvrParam;
	int					m_CheckTime;		//收集资源定时器时间间隔
	int					m_GO2Money_Rate;	//1Go点可以兑换的金钱
	int					m_Money2GO_Rate;	//兑换1Go点需要的金钱
	CardParam_Map		m_CardParamList;	//保存卡片的配置信息
	HeroEquipAttribute_Map		m_EquipAttributeList;
	SpeedParam_Map		m_SpeedParamList;//保存加速参数
	PackStoreParam_Map	m_PackStoreParamMap;

	//托管玩家列表	
	AutoPlayer_Map		m_AutoPlayerList;//托管玩家列表
	IceUtil::RWRecMutex m_AutoPlayerMutex;//锁

	//管理定时器的参数
	TimerParam_Map		m_TimerParamList;
	IceUtil::Mutex		m_TimerParamMutex;

	char				m_LogPath[SIZE_256];
	char				m_strLogFormat[10][SIZE_128];
	FILE*				m_hFile;
	int					m_CurMonth;
	IceUtil::Mutex		m_LogMutex;

	ConstructionConfigure_Multimap					m_ConstructionConfigureMultimap;
};

#endif
