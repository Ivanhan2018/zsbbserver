#pragma once
#include <map>
#include <boost/shared_ptr.hpp>
#pragma warning(push)
#pragma warning(disable:4244)
#include <boost/thread/shared_mutex.hpp>
#pragma warning(pop)
#include "LoadMapInfo.h"
#include "globalmacrodef.h"
#include "FightSystemHeader.h"
class EctypeManager;
class EctypeManager
{
public:
	EctypeManager(void);
	~EctypeManager(void);

public:
	//���ظ�����ͼ
	void loadEctypeMap();
	//����ض���ͼ��Ϣ
	CLoadMapInfo * getMapInfo(int mapID) const;
	//��ӻ���
	void addActivePlayer(ActivityPlayerPtr activityPlayer);
	//ɾ������
	void delActivePlayer(unsigned int playerID);
	//ɾ������BY����ID
	void delActivePlayersByRoomID(int roomID);
	//��û�����Ϣ
	ActivityPlayerPtr getActivePlayer(unsigned int playerID);
protected:
	//������ͼ����
	map<int , CLoadMapInfo*> mapCollection_;
	//��ǰ��ͼ�Ļ��� 
	map<unsigned int , ActivityPlayerPtr> activityPlayers_; 
	//�����û�������
	boost::shared_mutex ectypeManagerMutex_;
private:
	NO_COPYABLE_AND_ASSIGN(EctypeManager);
};
