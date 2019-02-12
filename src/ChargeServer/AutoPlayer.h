#ifndef AUTOPLAYER_H__
#define AUTOPLAYER_H__
#include <map>
#include <vector>
#include "IceUtil.h"
#include "DB_ProxyHeader.h"
#include "DBTableHeader.h"
#include "ChargeProtocol.h"
#include "ZSBB_DataDefine.h"
#define SAVEDATAPATH		".\\data"

class CChargeTask;
class CAutoPlayer:public IceUtil::Shared
{
	typedef std::map<__int64,LPAutoConstruction> AutoConstruction_Map;
	typedef std::map<__int64,LPAutoConstruction>::iterator AutoConstruction_Iterator;
public:
	CAutoPlayer(int playerid);
	~CAutoPlayer();

	int GetPlayerID();
	int AddConstruction(LPAutoConstruction pAutoCons);
	int DelConstruction(__int64 sortid);
	int WriteList2File();
	int ReadFile();
	int GetConstructionList(LPAutoConstruction pListBuf,int &count);

	int GetConstruction(CChargeTask *pChargeTask,int mapid,int cityid,AutoConstruction &autocons,DB_Proxy *pDBProxy,int &count);
	int GetCount();

private:
	void ClearList();
	int WriteFile();

private:
	int m_PlayerID;
	AutoConstruction_Map m_AutoConsList;
	IceUtil::RWRecMutex m_RWMutex;
};


typedef IceUtil::Handle<CAutoPlayer> AutoPlayer_Ptr;
typedef std::map<int,AutoPlayer_Ptr> AutoPlayer_Map;
typedef std::map<int,AutoPlayer_Ptr>::iterator AutoPlayer_Iterator;

#endif
