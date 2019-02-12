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
	//֪ͨ���ͼ���ӵ���
	int NotifyArmyReached(IArmyPtr armyPtr ,int fromMapID,int midMapID,int toMapID);
	//֪ͨ���ͼ��һ�ñ���
	int NotifyGetBox(IArmyPtr armyPtr ,int boxID , USE_PART cardType);
	//֪ͨ������Ҳ�������
	int NotifyArmyDie(IArmyPtr armyPtr, int mapID , int weaponID);
	//����������ķ���֪ͨ
	int NotifyCreateRoomBack(int createState,int ectypeRoomID ,const Notify_Enter_Ectype* lpEvent);
	//����Ƿ��ڸ���֪ͨ
	int NotifyIsInEctype(bool state,ActivityPlayerPtr playerPtr , int entranceID);
	int NotifyIsInEctype(bool state , const Notify_Is_In_Ectype* lpEvent);
	//֪ͨ���ͼ��Ҳ���״̬�޸�
	int NotifyModifyArmyState(unsigned int playerid,unsigned int armyid, int mapid,int armyNum,int state,int weapon);
	//֪ͨ����ͼ����ս�����
	int NotifyEctypeArmyBack(Notify_Back_Army_Ectype& ectypeArmysInfo, int fromMapID,int curProcessID);
};
