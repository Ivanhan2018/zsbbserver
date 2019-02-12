
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

//初始化广播管理器
bool BroadcastManager::initialize(int curServerID){
	curServerID_ = curServerID;
	broadcastPackData_ = new FightPack;
	if (broadcastPackData_ == NULL){
		return false;
	}
	memset(broadcastPackData_,0,sizeof(FightPack));
	return true;
}
//组包
void BroadcastManager::insertBroadData(DISTRIBUTED_HEADER * broadcastData ,PROTOCOL_FIGHT fightCmd , int dataLen){
	if (broadcastPackData_->length + dataLen >FightPack::maxDataLen){
		//组包数据超过设定长度，则发送数据
		msgBroadcastData();
	}
	broadcastData->serialize(dataLen,4000,curServerID_,0,fightCmd);
	//可以考虑减掉包头
	memcpy(broadcastPackData_->Info + broadcastPackData_->length,broadcastData,dataLen);
	broadcastPackData_->length += dataLen;
	broadcastPackData_->packnum ++;
}
void BroadcastManager::msgBroadcastData(void){
	if (broadcastPackData_->length > 0){
		broadcastPackData_->serialize(broadcastPackData_->length + sizeof(DISTRIBUTED_HEADER) 
			+ sizeof(unsigned short), 4000 , curServerID_ , 0);
		//广播数据到客户端
		broadcast((char*)broadcastPackData_,broadcastPackData_->length);
		//reset broadcastPackData_
		memset(broadcastPackData_,0,sizeof(FightPack));
	}
}