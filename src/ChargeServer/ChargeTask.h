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
		LOGIC_EXCEPTION exception_func);//�������Ӳ����󣬵��øú�������������
	virtual void stopTask();
	virtual void run();
	virtual void exit(char *msg);
	void ClearParamData();

	int ProcTimer(const void * param);
	LPCardParam GetCardParam(int cardid);					//����ItemID��ȡĳ����������Ϣ
	HeroEquipAttribute* GetEquipAttribute(int itemid);					//����װ��ID��ȡװ����������Ϣ

	char *SetLogFormat(LogType logtype,const char *strFormat);
	char *GetLogFormat(LogType logtype);

	char *SetLogPath(const char *path);						//������־��·��
	char *GetLogPath();										//

	int AddTimerNotifyMsg(int playerid,int type,LPNotify_Timer param);			//��ʱ����������MAP��
	int DelTimerNotifyMsg(int playerid,int type);
	LPNotify_Timer GetTimerID(int playerid,int type);

	//�й��б�Ĳ���
	int AddAutoPlayer(AutoPlayer_Ptr &player_ptr);
	int GetAutoPlayer(int playerid,AutoPlayer_Ptr &player_ptr);
	int DelAutoPlayer(int playerid);
	int GetConstruction(int playerid,int mapid,int cityid,AutoConstruction &autocons,DB_Proxy *pDBProxy,int &count);

	//�����ռ���Դ��ʱ�����
	int SetCheckResourceTime(int time);
	int GetCheckResourceTime();

	//Go��һ�����
	int SetExchageRate(int type,int rate);
	int GetExchageRate(int type);

	//���ٲ���
	LPSpeedParam_Node GetSpeedParam(int type,int value);
	int ReconnectWebServer();

	//�����ص�����
	PackStoreParam *GetPackStoreParam(int postion);

	//��ȡ�����������
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


	//��ȡ�������ñ�
	void readConstructionConfigure();
	void clearConstructionConfigure();

private:
	bool				m_bRuning;
	ServerParam			m_SvrParam;
	int					m_CheckTime;		//�ռ���Դ��ʱ��ʱ����
	int					m_GO2Money_Rate;	//1Go����Զһ��Ľ�Ǯ
	int					m_Money2GO_Rate;	//�һ�1Go����Ҫ�Ľ�Ǯ
	CardParam_Map		m_CardParamList;	//���濨Ƭ��������Ϣ
	HeroEquipAttribute_Map		m_EquipAttributeList;
	SpeedParam_Map		m_SpeedParamList;//������ٲ���
	PackStoreParam_Map	m_PackStoreParamMap;

	//�й�����б�	
	AutoPlayer_Map		m_AutoPlayerList;//�й�����б�
	IceUtil::RWRecMutex m_AutoPlayerMutex;//��

	//����ʱ���Ĳ���
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
