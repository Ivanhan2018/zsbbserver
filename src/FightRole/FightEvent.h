#pragma once
#include "IArmy.h"
#include "ProNPCSystem.h"
#include "ServerProtocolHeader.h"
#include "PlayerManager.h"
class FightEvent
{
public:
	FightEvent(void);
public:
	~FightEvent(void);
public:
	//通知大地图部队到达
	int NotifyArmyReached(IArmyPtr armyPtr ,int fromMapID,int midMapID,int toMapID);
	//通知大地图玩家获得宝箱
	int NotifyGetBox(IArmyPtr armyPtr ,int boxID , USE_PART cardType);
	//通知工会玩家部队死掉
	int NotifyArmyDie(IArmyPtr armyPtr, int mapID , int weaponID);
	//副本创建后的返回通知
	int NotifyCreateRoomBack(int createState,int ectypeRoomID ,const Notify_Enter_Ectype* lpEvent);
	//玩家是否在副本通知
	int NotifyIsInEctype(bool state,ActivityPlayerPtr playerPtr , int entranceID);
	int NotifyIsInEctype(bool state , const Notify_Is_In_Ectype* lpEvent);
	//通知大地图玩家部队状态修改
	int NotifyModifyArmyState(unsigned int playerid,unsigned int armyid, int mapid,int armyNum,int state,int weapon);
	//通知郡地图副本战斗完成
	int NotifyEctypeArmyBack(Notify_Back_Army_Ectype& ectypeArmysInfo, int fromMapID,int curProcessID);
};
