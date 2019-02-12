/*=================================================================*/
/*�з��Ŷ�  : �������·���
/* �ļ���   : ServerManager.h
/* �ļ����� : ���ͼ������
/* ����ʱ�� : 2008-3-20
/* ������   : �����
/* ��ע     :
/*
/*-----------------------------------------------------------------*/
/* �޸ļ�¼1:
/* �޸�ʱ�� :
/* . . .
/*=================================================================*/
#ifndef __SERVER_MANAGER__
#define __SERVER_MANAGER__
#include "DBTableHeader.h"
#include "DB_Proxy.h"
#include "MapHeader.h"
#include "Time_Protocol.h"
#include "TreasurePack.h"
#include "ZSBB_DataDefine.h"
#include "ISystemLog.h"

struct tagRaceAttribute
{
	int iRecruitFree;
	int iTechPoint;
	int iOrigEnd;
	int iOrigStr;
	int iOrigAgi;
	int iOrigInt;
	int iEndurance;
	int iStrength;
	int iAgility;
	int iIntelligence;
};
struct tagHeroRaceParam
{
	int iEndurance;
	int iStrength;
	int iAgility;
	int iIntelligence;
};
struct tagHeroJobParam
{
	int iKnight;
	int iSoldier;
	int iShooter;
	int iMaster;
	int iPastor;
};

/*
typedef struct tagArmyOutTimer
{
	long TimerID;
	char *TimerParam;
}ArmyOutTimer,*LPArmyOutTimer;

#define HERO_RESUME_TIME	10800//3Сʱ
*/

typedef struct tagMapWarmInfo
{
	long TimerID;
	unsigned int UnionCount;	//���˲�������
	unsigned int ClanCount;		//���䲿������
	unsigned int AboriginalCount;//������������
}MapWarmInfo,*LPMapWarmInfo;
class COtherManager 
{
private:
	long									m_lMapID;				
	MapInfoTable							m_MapState[BIG_MAP_SIZE];
	tagRaceAttribute						m_HeroAttri[8];						//�����츳
	map<unsigned long ,MapInfoTable>		m_mapMapInfo;
	CUserInfo								m_UserInfo;
	int										m_MapInfo[BIG_MAP_SIZE][BIG_MAP_SIZE];
	//map<unsigned long, int>					m_mapOnLinePlayer;					//�������
	map<unsigned long, __int64>				m_mapRecruitingOfficer;				//������ļӢ��...
	map<int,tagSpecialHero>					m_mapSpecialHero;						//����Ӣ�ѱ�

	map<int,string>							m_mapHeroName;						//Ӣ������
	map<int,tagHeroRaceParam>				m_mapRaceParam;
	map<int,tagHeroJobParam>				m_mapJobParam;
	map<int, int>							m_mapRecruitMoney;
	int										m_MapWarm[BIG_MAP_SIZE];

	//add by xujincai 20081218
	map<int,CTreasurePack*>					m_TreasureList;						//�������ñ�
	//add by xujincai 20090106 Ϊ����ת�����������
	map<unsigned long,OUT_ARMY_PATH*>		m_ArmyOutTimerList;					//���ӳ�����ʱ��
	IceUtil::Mutex							m_TimerMutex;
	
	MapWarmInfo								m_MapWarmInfo[BIG_MAP_SIZE];		//Ԥ����Ϣ��
	vector<int>								m_UnionSafeMap;						//���˰�ȫ��
	vector<int>								m_TribeSafeMap;						//���䰲ȫ��

	InitConstruction_Map					m_InitConstructionMap;				//��ʼ��ҽ���
//��������
public:
	COtherManager();
	virtual ~COtherManager();
	//�ӿں���
private:
	//add by xujincai 20090106
	int AddArmyOutTimer(unsigned long playerid,int armyid,OUT_ARMY_PATH *param);
	long GetArmyOutTimer(unsigned long playerid,int armyid);
	OUT_ARMY_PATH* DelArmyOutTimer(unsigned long playerid,int armyid);

public:
	//��״̬��Ϣ
	int MapStateList(int handle, unsigned long lPlayerID);
	//��ҳ�����Ϣ
	int PlayerCityInfo(int handle, unsigned long lPlayerID);
	//��һ�����Ϣ
	int PlayerBaseInfo(int handle, unsigned long lPlayerID);
	//֪ͨ�������С��г��ȷ�������������ߡ�����
	void notifyPlayerState2OtherSvr(int playerid,int agent);
public:
	//����ָ�ӹ�
	int OnCreateOfficer(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID);
	//��ѯָ�ӹ�����
	int GetOfficerAttribute(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID);
	//���Ӣ��,�漰��Ӣ��״̬�ĸı䡢˫���Ľ�Ǯ��־(3��)
	int RedeemHero(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID);
	//���ٱ����е�װ�����漰��װ���ĸı䡢��Ǯ��־(2��)
	int DestroyPackEquip(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID);
	//�õ�Ӣ��װ��
	int OnGetHeroEquip(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID);
	//����Ӣ��װ��
	//���Ӣ���Ƿ��Ѵ��ϸ����͵�װ��
	bool haveDressEquipOfPostion(int playerid,int officerid,int postion,int sortid);
	int OnExchangeHeroEquip(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID);//������
	int OnExchangeHeroEquipEx(const char * pDataBuffer);
	//Ӣ��ս����¼
	int OnHeroWarRecord(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID);
	//Ӣ�ۼ�ɳ���¼
	int OnHeroGrowUpRecord(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID);
	//ָ�ӹ��б�
	void OnOfficerList(int handle, unsigned long lPlayerID);
	//��ļָ�ӹ�,�漰����Ǯ��Ӣ����Ϣ��Ӣ��������־��3����
	int OnRecruitOfficer(int handle, unsigned long lPlayerID);
	//��ļӢ��ʱ���¼�
	int OnRecruitHeroEvent(const void * pDataBuffer, int iDataSize);
	//����Ӣ��,�漰��Ӣ��������־��1����
	int OnDismissHero(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID);
	//ɾ��Ӣ�۵����м���
	void clearAllOfficerSkill(int playerid,int officerid);
	//�õ�ָ�ӹټ���
	int onOfficerTechList(const char *msg);
	//ָ�ӹ�ѧϰ����,�漰�����ܵ㡢������������־��2����
	int onOfficerStudyTech(const char *msg);
	//�޸�ָ�ӹ�����
	void OnModifyOfficerName(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID);

	//����ת��
	int OnArmyTransfer(int handle, void * pDataBuffer, int iDataSize, unsigned long lMapID, unsigned long lPlayerID) ;
	//�����ڳ����еĲ����б�//add by xujincai 20090106
	int OnOutingArmysListEx(int handle, unsigned long lMapID, unsigned long lPlayerID);
	//����ͼ��Ѱ·��
	int BigMapFindPath(int start,int End ,list<int>* path);
	int BigMapFindPath(int MapInfo[BIG_MAP_SIZE][BIG_MAP_SIZE],int start,int End ,list<int>* path);
	int ResetMapInfo(int MapInfo[BIG_MAP_SIZE][BIG_MAP_SIZE],int countryid,int startmapid,int destmapid);
	//�����˻�ʱ���ص������Ŀ�����ѡ��������
	int GetNextMapID(int countryid,int start,int end);
	//���㲿�ӳ���ʱ��
	int GetMapTimes(unsigned long lPlayerID, unsigned long lMapID, unsigned long lTargetMapID, unsigned long lArmyID, int iTimeLen);
	//ɾ����������
	void OnDeleteOutArmy(unsigned long lPlayerID, unsigned long lArmyID);
	//ȷ�����ӳ���OK
	int OnArmyReachOKEx(const void *pDataBuffer,int iDataSize);
	//�˿��ܷ�ﵽ
	int OnReachMap(int handle, unsigned long lPlayerID, unsigned long lMapID, unsigned long lTargetMapID, unsigned long lArmyID, list<int>* path, int iCase);
	//����ս��
	void OnMapEnFight(const void * pDataBuffer, int iDataSize);

	void ArmyWarmTimeEvent(void * pDataBuffer, int iDataSize);
	//δ��
	void OnRecruitSpeed(unsigned long lPlayerID, int iValue);

	//add by xujincai 20081217,�漰�����ͼ������
	//��ӱ���,�漰װ����־
	int OnNewTreasurePack(const char *msg);
	//�򿪱���,����װ����־����Ǯ��־��
	int OnOpenTreasurePack(const char *msg);

	//add by xujincai 20081223 ��д���ӳ���������Ԥ������
	int OnOutArmyEx(const void * pDataBuffer, int iDataSize, int iCase,bool bInit);
	void ArmyOutTimeEventEx(void * pDataBuffer, int iDataSize);
	void TirrierGoHome(const void *msg);

	//add by xujincai 20081226����Ӣ�����˻ָ�
	//Ӣ������֪ͨ
	void OnHeroGBHNotify(void *pDataBuffer);
	//Ӣ�����˻ָ���ʱ��,�漰Ӣ��״̬��־
	void HeroResumeTimerEvent(void * pDataBuffer);
	//�������
	void OnImmedComplete(LPImmedComplete_Request msg);
	//�����շ�ϵͳ���ص�֪ͨ��Ϣ
	void OnImmedCompleteNotifyMsg(LPNotify_ImmedComplete_Response msg);

	//add by xujincai 20090224 ������в�����Ϣ
	//��ȡ�����б�
	void OnGetArmyList(const char *msg);
	//����״̬�ı�
	void OnModifyArmyState(const char *msg);

	//add by xujincai 20090403 ������Ϣ����
	//����������
	void OnTirrierGoOut(const char *msg);	

	bool IsSafeMap(int mapid,int countryid);//�ж��Ƿ�Ϊ���ִ�

	//add by xujincai 20090624
	//��ѯ��²Ӣ��
	void OnQueryCaptiveList(const char *msg);

	//����NPC��������
	void NotifyCreateNPCArmy(const char *msg);
	void CreateNPCArmyTimer(const char *msg);

	//������ͼ��NPC����
	void ClearNPCArmyOfBigMap(DB_Proxy *pDBProxy);

	//����Ӣ���ж�ֵ
	int ProcOfficerActionDot(int millrank,PlayerOfficerTable *pOfficer);
	void OnNotifyMapState(const char *msg);//��״̬�ı�

	//����װ��,�漰װ����־�;�����־
	void onInlayEquipment(const char *msg);
	//ѧϰ����֪ͨ������֪ͨ������ս����
	void studySkillNotify(int mapid,int armyid,int officerid,int skill,int skilllevel,int cmd);
	//�շ�ϵͳ֪ͨѧϰ���ܣ�ʹ�ü��ܿ���,�漰���ܵ�ͼ�����־
	void onNotifyAddOfficerSkill(const char *msg);
	//�ж�Ӣ���Ƿ���ս����
	bool isInWarMapOfOfficer(PlayerOfficerTable *officer);
	//��֪ͨ�����빫���������漰װ����־
	void onNotifyBuyCard(const char *msg);
	//���ƶһ����漰��Ǯ�����������־
	void onConvert(const char *msg); 

	//��ѯ��ҳ������ٿ�
	int getExpeditionSpeedCard(int playerid,PlayerItemStorageTable *item);
	//��ʼ����ҽ���
	void initPlayerConstruction(int playerid,int mapid,int cityid,int &warehouseLevel);

	//ͨ�ô򿪿���ͳһ�Ҽ�������ܴ򿪵Ŀ�
	void onCommonOpenCard(const char *msg);
	//���Ӣ�ۼ��ܣ��漰���ܡ����ܵ���־
	int clearHeroStudySkill(int playerid,int heroid);
	//��ȡһ���������ĵ����м��ܵ�
	int getSumSkillDotOfSkill(int skillid,int level);

	//�޸�Ӣ������
	void onModifyHeroName(const char *msg);
	//�ж��Ƿ��Ѵ��ڸ����ֵ�Ӣ��
	bool isExistOfHeroName(const char *name);
	//�ͷŷ�²���漰Ӣ��״̬��־
	void releaseCaptive(const char *msg);
public:
	void SetMapID(long lMapID,DB_Proxy *pDBProxy);
	int GetMapID() { return m_lMapID; };
	//�����츳
	void ReadRaceGeniusInfo();
	//��ȡ������Ϣ
	void ReadCityInfo();
	//��ȡ����ͼ�б���Ϣ
	void ReadMapListInfo();
	//��ȡ����ͼ����
	void ReadMapData();

	//��ȡ�������ñ�
	void loadBinConfig();
	void clearBinConfig();
	CTreasurePack* getBinConfig(int id);
	//��ȡӢ���б��������˵�Ӣ�۽����Զ��ָ�
	void ReadPlayerOfficerTable(DB_Proxy *pDBProxy);
	//�˳���Ϸ
	void OnExitMapServer(unsigned long lPlayerID);

	void InitArmyOut(int mapid,DB_Proxy *pDBProxy);//�ָ����ӳ���	

	//���س�ʼ��ҽ�����
	void loadInitConstructions();
	void clearInitConstructions();
	
private:
	int64 GetCurrentTime();
	
};

extern DB_Proxy									g_Proxy;
#endif
