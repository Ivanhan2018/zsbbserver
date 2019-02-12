#ifndef __gmFixData_H__
#define __gmFixData_H__

#include <string>
#include <hash_map>
#include <set>
#include "IceUtil.h"

#define OTL_STL
#define OTL_ODBC

#if defined (WIN32)
#define OTL_BIGINT __int64
#else
#define OTL_BIGINT long long
#endif


#include "otlv4.h"
#include "um_web_client.h"
#include "MapServerProtocolHeader.h"
#include "ZSBB_DataDefine.h"

//#pragma pack(push,1)
//struct tagGmUserInfo
//{
//	int account_id;
//	char user_name[16];
//	int permission;
//};
//#pragma pack(pop)


#if defined(__BCPLUSPLUS__) || defined(_MSC_VER)
//
// On Windows, long is always 32-bit
//
typedef __int64 Int64;
#elif defined(ICE_64)
typedef long Int64;
#else
typedef long long Int64;
#endif


//
// ICE_INT64: macro for Int64 literal values
//
#if defined(__BCPLUSPLUS__) || defined(_MSC_VER)
#   define ICE_INT64(n) n##i64
#elif defined(ICE_64)
#   define ICE_INT64(n) n##L
#else
#   define ICE_INT64(n) n##LL
#endif

using namespace std;

class CGmFixData
{
	typedef stdext::hash_map<std::string,int> HeroContainer;
	typedef std::set<int> DataSet;
	typedef std::set<int>::const_iterator DataCit;
	typedef std::set<int>::iterator DataIt;
	typedef stdext::hash_map<int,int> UserContainer;
	typedef stdext::hash_map<int,tagGmUserInfo> GmUserContainer;

	typedef stdext::hash_map<int,PackStoreParam*> PackStoreParam_Map;
	typedef PackStoreParam_Map::iterator PackStoreParam_Map_Iterator;
private:
	CGmFixData(); 
public:
	~CGmFixData();
	static CGmFixData& Instance();
	static Int64 GetTimeNow();

	bool LoadData(const char * addr, short port,const char * dns,const char * user,const char * pwd,const char* webip,short webport,int timeout);
	void UnLoadData();

	bool LoadHeroData(const char * addr, short port);

	bool ConData(const char * dsn,const char * user,const char * password);
	void DisConnData();
	
	bool LoadLoginDisabled();
	bool LoadSpeakingDisabled();
	bool InsertCmdRec(otl_connect& db,int gmid,const char* gmdata,short isvalid,short key,short rlt);

	bool InsertHero(const char* heroname,int playerid);
	bool FindHero(const char * heroname,int& playerid);

	bool InsertUC(const int playerid,const int agentid);
	bool FindUC(const int playerid,int& agentid);
	bool DeleteUC(const int playerid);
	bool DeleteUCbyAgent(const int agentid);
	
	bool InsertLogin(otl_connect& db,const int playerid);
	bool FindLogin(const int playerid);
	bool DelLogin(otl_connect& db,const int playerid);

	bool InsertSpeak(otl_connect& db,const int playerid);
	bool FindSpeak(const int playerid);
	bool DelSpeak(otl_connect& db,const int playerid);
	bool LoadGmUserList(const char * ip,short port ,int timeout);
	bool FindGmUserInfo(const int playerid,tagGmUserInfo& gmuserinfo); 
	//GM权限
	bool QryPermission(int code,int cmdcode);
	PackStoreParam *GetPackStoreParam(int postion);

private:
	int ReadPackStoreParam();
	void ClearPackStoreParam();
private:
	otl_connect db_;

	//首领英雄名容器 key首领英雄名 玩家ID
	HeroContainer hcHero_;
	//用户表
	UserContainer uc_;
	//禁言表
	DataSet dlogin_;
	//禁登录表
	DataSet dspeaking_;
	//GM账户表
	GmUserContainer gmuser_;

	IceUtil::RWRecMutex heromux_;
	IceUtil::RWRecMutex loginmux_;
	IceUtil::RWRecMutex speakmux_;
	IceUtil::RWRecMutex ucmux_;
	IceUtil::Mutex logmux_;

	PackStoreParam_Map packStoreParamMap_;
};

#endif

