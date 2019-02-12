
#include "BroadcastManager.h"
#include "Global_Macro.h"
#include "ProFightSystem.h"
#include "socketsvc.hpp"
#include "FightSystemMacro.h"
BroadcastManager::BroadcastManager()
{
}

BroadcastManager::~BroadcastManager(void)
{
	CHECK_DELETE(broadcastPackData_);
}

//��ʼ���㲥������
bool BroadcastManager::initialize(int curServerID){
	curServerID_ = curServerID;
	broadcastPackData_ = new FightPack;
	if (broadcastPackData_ == NULL){
		return false;
	}
	memset(broadcastPackData_,0,sizeof(FightPack));
	return true;
}
//���
void BroadcastManager::insertBroadData(DISTRIBUTED_HEADER * broadcastData ,PROTOCOL_FIGHT fightCmd , int dataLen){
	if (broadcastPackData_->length + dataLen >FightPack::maxDataLen){
		//������ݳ����趨���ȣ���������
		msgBroadcastData();
	}
	broadcastData->serialize(dataLen,4000,curServerID_,0,fightCmd);
	//���Կ��Ǽ�����ͷ
	memcpy(broadcastPackData_->Info + broadcastPackData_->length,broadcastData,dataLen);
	broadcastPackData_->length += dataLen;
	broadcastPackData_->packnum ++;
}
void BroadcastManager::msgBroadcastData(void){
	if (broadcastPackData_->length > 0){
		broadcastPackData_->serialize(broadcastPackData_->length + sizeof(DISTRIBUTED_HEADER) 
			+ sizeof(unsigned short), 4000 , curServerID_ , 0);
		//�㲥���ݵ��ͻ���
		broadcast((char*)broadcastPackData_,broadcastPackData_->length);
		//reset broadcastPackData_
		memset(broadcastPackData_,0,sizeof(FightPack));
	}
}