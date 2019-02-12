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
#	define MAX_LOGO_SIZE			4096	//�Զ���Logo���������
#endif

//�����Ա
typedef IceUtil::Handle<CConsortiaUser> ConsortiaUser_Ptr;
typedef std::map<int,ConsortiaUser_Ptr> ConsortiaUser_Map;
typedef std::map<int,ConsortiaUser_Ptr>::iterator ConsortiaUser_Iter;

//����ͬ��
typedef IceUtil::Handle<CConsortiaUnion> ConsortiaUnion_Ptr;
typedef std::map<int,ConsortiaUnion_Ptr> ConsortiaUnion_Map;
typedef std::map<int,ConsortiaUnion_Ptr>::iterator ConsortiaUnion_Iter;

//��������
typedef IceUtil::Handle<CConsortiaBank> ConsortiaBank_Ptr;
typedef std::map<int,ConsortiaBank_Ptr> ConsortiaBank_Map;
typedef std::map<int,ConsortiaBank_Ptr>::iterator ConsortiaBank_Iter;

//����Լս
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
	//�������
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

	//20100226 ��������
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
	int getOfficalCount();//��ȡ��Ա����

	/*��Ա����	*/
	int AddUser(int playerid,int jobid,int purview,int type = 0);//��ӳ�Ա
	int AddUser(ConsortiaUser_Ptr &user_ptr);
	int DelUser(int playerid);//ɾ����Ա
	int GetUserJob(int playerid);
	int SetUserJob(int playerid,int jobid);
	int GetUserPurview(int playerid);
	int SetUserPurview(int playerid,int purview);
	int GetUserType(int playerid);
	int GetUserInfo(int playerid,int &jobid,int &purview,int &type);//����ID��ȡ��Ա��Ϣ
	CConsortiaUser* getConsortiaUser(int playerid);
	int SetUserInfo(int playerid,int jobid,int purvew,int type);
	int GetUserList(LPCon_UserInfo userlist,int start,int &count);

	/*ͬ�˲���*/
	int AddUnion(int id,int conid);
	int AddUnion(ConsortiaUnion_Ptr &union_ptr);
	int DelUnion(int conid);
	bool IsUnionConsortia(int consortiaid);

	/*���в���*/
	int AddBankItem(int itemid,int itemcount);
	int DelBankItem(int itemid);
	int SetItemCount(int itemid,int itemcount);
	int GetItemCount(int itemid);
	int GetBankCount();
	int GetBankList(LPBank_ItemInfo itemlist,int start,int &count);

	/*Լս*/
	int AddFight(int id,int conid,int mapid,int cityid,__int64 time);
	int AddFight(ConsortiaFight_Ptr &fight_ptr);
	int DelFight(int conid);
	bool IsFightConsortia(int consortiaid);

	bool isRoyalConsortia();
	int getMapID();//��ȡ����ռ�ݵĳ������ڿ�

	//������׼�������Ȩ�޵Ļ�Ա�����ʼ�
	void sendEmail2Offical(const int playerid,const char *name,Notify_Send_Mail *email);

	//�޸ĵ����ݿ�
	int update2DBG(DB_Proxy *pDBProxy);

private:
	void init();
private:
	int			ConsortiaID;				//������
	int			MapID;						//��ID
	int			CityID;						//���б��
	int			ChairmanID;					//����᳤
	int			Count;						//����
	int			TechID;						//����ID
	short		Level;						//����ȼ�
	char		Name[SIZE_32];				//��������
	char		Placard[SIZE_256];			//���ṫ��
	char		Tenet[SIZE_128];			//������ּ
	int			FaceID;						//Logo���
	char		FaceBuffer[MAX_LOGO_SIZE];	//�Զ���Logo
	__int64		CreateTime;					//����ʱ��
	__int64		FrtNoFullTime;				//�״�δ��10�˵�ʱ��
	int			CountryID;					//��Ӫ���Ի᳤��Ӫ
	int			MaxUserNum;					//��������
	int			Money;						//��Ǯ
	int			Exp;						//����
	int			Credit;						//��������
	int			SignID;						//���ű�־-�ʼҾ��Ų���
	char		ShortName[5];				//���

	int			OfficalCount;				//��Ա����

public://����
	ConsortiaUser_Map 	ConUserList;		//��Ա

	ConsortiaUnion_Map	ConUnionList;		//ͬ�˹����б�

	ConsortiaBank_Map	ConBankInfo;		//��������

	ConsortiaFight_Map	ConFightList;		//Լս
};
#endif
