#include "FightSystemMacro.h"
#include "FightProcesser.h"
#include "CountryFightManager.h"

FightProcesser::FightProcesser(void):
fightManger_(NULL)
{
}

FightProcesser::~FightProcesser(void)
{
	release();
	CHECK_DELETE(fightManger_);
}
bool FightProcesser::initialize(CLoadMapInfo * mapInfo , CLoadDBInfo * dataBaseMemData , IniFile * iniFile){
	CountryFightManager *tmpfightManger = new CountryFightManager;
	fightManger_ = tmpfightManger;
	if (fightManger_ == NULL){
		return false;
	}
	return tmpfightManger->Initialize(mapInfo , dataBaseMemData , iniFile);
}
bool FightProcesser::pushDataToManager(char * buf){
	return fightManger_->putQueue(buf);
}
ActivityPlayerPtr FightProcesser::getPlayerInfo(unsigned int playerID){
	return fightManger_->getPlayerInfo(playerID);
}	
void FightProcesser::release(void){
	fightManger_->stopTask();
	fightManger_->release();
}
void FightProcesser::startTimerCycle(unsigned int cycle){
	fightManger_->startTimerCycle(cycle);
	fightManger_->startLogTimer();
}