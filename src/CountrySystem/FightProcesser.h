#pragma once
#include "IFightProcesser.h"
class FightManager;
class CLoadMapInfo;
class CLoadDBInfo;
class IniFile;


//ս��������������
class FightProcesser : public IFightProcesser
{
public:
	FightProcesser(void);
public:
	~FightProcesser(void);
public:
	//
	bool initialize(CLoadMapInfo * mapInfo , CLoadDBInfo * dataBaseMemData , IniFile * iniFile);
	//����Ϣѹ����Ϣ����
	virtual bool pushDataToManager(char * buf);
	//��������Ϣ
	virtual ActivityPlayerPtr getPlayerInfo(unsigned int playerID);
	//����ս��������
	virtual void startTimerCycle(unsigned int cycle);
	//�ͷ�
	void release(void);
private:
	FightManager * fightManger_;
};
