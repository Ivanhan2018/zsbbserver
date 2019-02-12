#ifndef CONSORTIA_H__
#define CONSORTIA_H__

#include "IceUtil.h"
#include <map>
#include "Global_Macro.h"
#include "ConsortiaUser.h"
#include "ConsortiaUnion.h"
#include "ConsortiaBank.h"
#include "ConsortiaFight.h"
#include "ConsortiaProtocol.h"
#include "socketsvc.hpp"
#include "Server_Protocol.h"

#ifndef	MAX_LOGO_SIZE
#	define MAX_LOGO_SIZE			4096	//自定义Logo的最大限制
#endif

//公会成员
typedef IceUtil::Handle<CConsortiaUser> ConsortiaUser_Ptr;
typedef std::map<int,ConsortiaUser_Ptr> ConsortiaUser_Map;
typedef std::map<int,ConsortiaUser_Ptr>::iterator ConsortiaUser_Iter;

//公会同盟
typedef IceUtil::Handle<CConsortiaUnion> ConsortiaUnion_Ptr;
typedef std::map<int,ConsortiaUnion_Ptr> ConsortiaUnion_Map;
typedef std::map<int,ConsortiaUnion_Ptr>::iterator ConsortiaUnion_Iter;

//公会银行
typedef IceUtil::Handle<CConsortiaBank> ConsortiaBank_Ptr;
typedef std::map<int,ConsortiaBank_Ptr> ConsortiaBank_Map;
typedef std::map<int,ConsortiaBank_Ptr>::iterator ConsortiaBank_Iter;

//公会约战
typedef IceUtil::Handle<CConsortiaFight> ConsortiaFight_Ptr;
typedef std::map<int,ConsortiaFight_Ptr> ConsortiaFight_Map;
typedef std::map<int,ConsortiaFight_Ptr>::iterator ConsortiaFight_Iter;

typedef IceUtil::RWRecMutex	RWMutex;

class CConsortia:public IceUtil::Shared
{
public:
	CConsortia();
	CConsortia(int id,const char *name,int mapid,int cityid,int playerid,__int64 time,int countryid);
	~CConsortia();
	//公会操作
	int SetConsortiaID(int id);
	char* SetName(const char *name);
	int SetMapID(int mapid);
	int SetCityID(int cityid);
	int SetChairmanID(int playerid);
	int SetUserCount(int count);
	int SetTeachID(int teachid);
	short SetLevel(short level);
	int SetFaceID(int faceid);
	char* SetFaceBuff(const char *buffer);
	char* SetPlacard(const char *buffer);
	char* SetTenet(const char *buffer);
	__int64 SetCreateTime(__int64 time);
	__int64 SetFrtNoFullTime(__int64 time);

	int SetCountryID(int countryid);

	int GetConsortiaID();
	char* GetName();
	int GetMapID();
	int GetCityID();
	int GetChairmanID();
	int GetUserCount();
	int GetTeachID();
	short GetLevel();
	int GetFaceID();
	char* GetFaceBuff();
	char* GetPlacard();
	char* GetTenet();
	__int64 GetCreateTime();
	__int64 GetFrtNoFullTime();
	int GetCountryID();

	//20100226 军团新增
	int setMaxUserNum(int num);
	int getMaxUserNum();
	int setMoney(int money);
	int getMoney();
	int addMoney(int money);
	int setExp(int exp);
	int getExp();
	int addExp(int exp);

	int setHonors(int val);
	int getHonors();
	int addHonors(int val);

	int setSignID(int id);
	int getSignID();
	
	void setShortName(const char *name);
	const char *getShortName();

	//
	int addOfficalCount(int num);
	int getOfficalCount();//获取官员数量

	/*成员操作	*/
	int AddUser(int playerid,int jobid,int purview,int type = 0);//添加成员
	int AddUser(ConsortiaUser_Ptr &user_ptr);
	int DelUser(int playerid);//删除成员
	int GetUserJob(int playerid);
	int SetUserJob(int playerid,int jobid);
	int GetUserPurview(int playerid);
	int SetUserPurview(int playerid,int purview);
	int GetUserType(int playerid);
	int GetUserInfo(int playerid,int &jobid,int &purview,int &type);//根据ID获取成员信息
	CConsortiaUser* getConsortiaUser(int playerid);
	int SetUserInfo(int playerid,int jobid,int purvew,int type);
	int GetUserList(LPCon_UserInfo userlist,int start,int &count);

	/*同盟操作*/
	int AddUnion(int id,int conid);
	int AddUnion(ConsortiaUnion_Ptr &union_ptr);
	int DelUnion(int conid);
	bool IsUnionConsortia(int consortiaid);

	/*银行操作*/
	int AddBankItem(int itemid,int itemcount);
	int DelBankItem(int itemid);
	int SetItemCount(int itemid,int itemcount);
	int GetItemCount(int itemid);
	int GetBankCount();
	int GetBankList(LPBank_ItemInfo itemlist,int start,int &count);

	/*约战*/
	int AddFight(int id,int conid,int mapid,int cityid,__int64 time);
	int AddFight(ConsortiaFight_Ptr &fight_ptr);
	int DelFight(int conid);
	bool IsFightConsortia(int consortiaid);

	bool isRoyalConsortia();
	int getMapID();//获取军团占据的城市所在郡

	//向有批准加入军团权限的会员发送邮件
	void sendEmail2Offical(const int playerid,const char *name,Notify_Send_Mail *email);

	//修改到数据库
	int update2DBG(DB_Proxy *pDBProxy);

private:
	void init();
private:
	int			ConsortiaID;				//公会编号
	int			MapID;						//郡ID
	int			CityID;						//城市编号
	int			ChairmanID;					//公会会长
	int			Count;						//数量
	int			TechID;						//技能ID
	short		Level;						//公会等级
	char		Name[SIZE_32];				//公会名称
	char		Placard[SIZE_256];			//公会公告
	char		Tenet[SIZE_128];			//公会宗旨
	int			FaceID;						//Logo编号
	char		FaceBuffer[MAX_LOGO_SIZE];	//自定义Logo
	__int64		CreateTime;					//创建时间
	__int64		FrtNoFullTime;				//首次未满10人的时间
	int			CountryID;					//阵营，以会长阵营
	int			MaxUserNum;					//人数上限
	int			Money;						//金钱
	int			Exp;						//经验
	int			Credit;						//军团荣誉
	int			SignID;						//军团标志-皇家军团才有
	char		ShortName[5];				//简称

	int			OfficalCount;				//官员数量

public://公开
	ConsortiaUser_Map 	ConUserList;		//成员

	ConsortiaUnion_Map	ConUnionList;		//同盟公会列表

	ConsortiaBank_Map	ConBankInfo;		//公会银行

	ConsortiaFight_Map	ConFightList;		//约战
};
#endif
