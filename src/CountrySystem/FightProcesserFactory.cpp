#include "FightProcesserFactory.h"
#include "FightProcesser.h"
IFightProcesser * FightProcesserFactory::createFightManager(CLoadMapInfo * mapInfo , CLoadDBInfo * dataBaseMemData , IniFile * iniFile){
	FightProcesser * fightProcesser = new FightProcesser;
	if (fightProcesser == NULL){
		return NULL;
	}
	if(fightProcesser->initialize(mapInfo , dataBaseMemData , iniFile) == false){
		delete fightProcesser;
		return NULL;
	}
	return fightProcesser;
}

	 
