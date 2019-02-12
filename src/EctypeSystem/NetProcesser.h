#ifndef NETPROCESSER_H_
#define NETPROCESSER_H_

#include <vector>
#include <list>
#pragma warning(push)
#pragma warning(disable:4244)
#include <boost/thread/shared_mutex.hpp>
#pragma warning(pop)
#include "EctypeManager.h"
#include "globalmacrodef.h"
class EctypeFightManager;
class FightEvent;
class CNPCInfo;
class DB_Proxy;
class CLoadDBInfo;
struct Notify_Enter_Ectype;
struct Notify_Is_In_Ectype;
struct MsgEctypeDelPlayer;
struct MsgEctypeFightOver;

enum FightManagersState{
	FIGHTMANAGE_NO_CREATE,
	FIGHTMANAGE_IS_CREATING,
	FIGHTMANAGE_IS_RUNING,
	FIGHTMANAGE_HAS_STOPED
};
#define  MAX_FIGHT_MANAGER_NUM 1000
class NetProcesser
{
public:
	static NetProcesser* instance();
public:
	~NetProcesser(void);
public:
	//��ʼ��NetProcesser
	bool initialize();
	//�����ͻ��˷��͵�����
	void parse(char *const buf);
	//���÷�����ID
	void setProcessID(int curProcessID){
		curProcessID_ = curProcessID;
	}
	//�ͷ��ڴ�
	void release();
	//������
	void insertPlayer(ActivityPlayerPtr playerPtr);
	//��������Ϣ
	ActivityPlayerPtr getActivePlayer(unsigned int playerId);
protected:
	//��ÿ��з���
	int getEmptyManager(void);
	void setManagerState(int roomID , FightManagersState state);
	//��֤����ID�Ƿ���Ч
	bool isValidRoomID(int roomID) const;
	//�������� 
	void createRoom(Notify_Enter_Ectype* lpEvent);
	//��֤����Ƿ��ڸ���
	void isInEctype(const Notify_Is_In_Ectype* lpEvent);
	//����ս������
	void processEctypeOver(MsgEctypeFightOver * msgEctypeFightOver);
private:
	NetProcesser(void);
protected:
	bool pushDataToManager(const char * buf,  int roomID);
	bool createActPlayerList(std::list<ActivityPlayerPtr>& actPlayerList, Notify_Enter_Ectype* lpEvent , int roomID);
	void delPlayerInfo(MsgEctypeDelPlayer * msgEctypeDelPlayer);
	void freeArmyFromEctype();
protected:
	typedef std::vector<EctypeFightManager*> EctypeFightManagers;
	//FightManager����
	EctypeFightManagers fightManagers;
	//������״̬
	std::vector<FightManagersState>  managerStates;
	//��̬ʵ��
	static NetProcesser* instance_;
	//����������
	EctypeManager* ectypeManager_;
	//�������ڲ��¼�֪ͨ
	FightEvent*   fightEvent_;
	//DBG�е�һЩ��������
	CLoadDBInfo * dataBaseMemData_;
	//DBG����
	DB_Proxy * dataBaseProxy_;
	//����NPC����TABLE
	CNPCInfo * ectypeNpcInfo_;
	//��ǰ������ע��Ľ���ID
	int curProcessID_;
	//��������д��
	boost::shared_mutex ectypeMutex_;
private:
	NO_COPYABLE_AND_ASSIGN(NetProcesser);
	
};

#endif // NETPROCESSER_H_
