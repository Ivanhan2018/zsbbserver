#pragma once
#include "IFightProcesser.h"
class FightManager;
class CLoadMapInfo;
class CLoadDBInfo;
class IniFile;


//战斗管理器导出类
class FightProcesser : public IFightProcesser
{
public:
	FightProcesser(void);
public:
	~FightProcesser(void);
public:
	//
	bool initialize(CLoadMapInfo * mapInfo , CLoadDBInfo * dataBaseMemData , IniFile * iniFile);
	//将消息压入消息队列
	virtual bool pushDataToManager(char * buf);
	//获得玩家信息
	virtual ActivityPlayerPtr getPlayerInfo(unsigned int playerID);
	//启动战斗管理器
	virtual void startTimerCycle(unsigned int cycle);
	//释放
	void release(void);
private:
	FightManager * fightManger_;
};
