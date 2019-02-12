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
class FightManager;
class PlayerManager;
class IniFile;
class PlayerManager
{
public:
	PlayerManager(IniFile * iniFile);
	~PlayerManager(void);
public:
	//��ӻ���
	ActivityPlayerPtr addActivePlayer(ActivityPlayerPtr activityPlayer);
	//ɾ������
	void delActivePlayer(unsigned int playerID);
	//ɾ������BY����ID
	void delActivePlayersByRoomID(int roomID);
	//��û�����Ϣ
	ActivityPlayerPtr getActivePlayer(unsigned int playerID);
	//��õ�ǰ�����������
	inline int getPlayerNum(void) {
		return playerNum_;
	}
	//�������б�
	const map<unsigned int , ActivityPlayerPtr> & getPlayerMap(void) const{
		return activityPlayers_;
	}
	//�����ҵ���ӪID

protected:
	//������ͼ����
	map<int , CLoadMapInfo*> mapCollection_;
	//��ǰ��ͼ�Ļ��� 
	map<unsigned int , ActivityPlayerPtr> activityPlayers_; 
	//�����û�������
	boost::shared_mutex ectypeManagerMutex_;
	//�������
	int playerNum_; 
	//�����ļ�
	IniFile * iniFile_;
	//
private:
	NO_COPYABLE_AND_ASSIGN(PlayerManager);
};
