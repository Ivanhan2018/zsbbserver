#include "AutoPlayer.h"
#include <stdio.h>
#include "socketsvc.hpp"
#include "ChargeTask.h"
#pragma warning(disable:4706)
#include "log.hpp"
CAutoPlayer::CAutoPlayer(int playerid):m_PlayerID(playerid)
{
}
CAutoPlayer::~CAutoPlayer()
{
	ClearList();
}

int CAutoPlayer::GetPlayerID()
{
	return m_PlayerID;
}

int CAutoPlayer::AddConstruction(LPAutoConstruction pAutoCons)
{
	IceUtil::RWRecMutex::WLock wlock(m_RWMutex);
	__int64 key = pAutoCons->GetKey();//pAutoCons->SortID + pAutoCons->MapID + pAutoCons->CityID + pAutoCons->ConstructionID;
	m_AutoConsList[key] = pAutoCons;
	return 0;
}
int CAutoPlayer::DelConstruction(__int64 sortid)
{
	IceUtil::RWRecMutex::WLock wlock(m_RWMutex);
	AutoConstruction_Iterator iter;
	iter = m_AutoConsList.find(sortid);
	LPAutoConstruction pDelCons = 0;
	if(iter != m_AutoConsList.end())
	{
		if((pDelCons = iter->second))
		{
			delete pDelCons;
			pDelCons = 0;
		}
		m_AutoConsList.erase(sortid);
		return 0;
	}
	return -1;
}
int CAutoPlayer::WriteList2File()
{
	IceUtil::RWRecMutex::RLock rlock(m_RWMutex);
	return WriteFile();
}
int CAutoPlayer::ReadFile()
{
	LPAutoConstruction pCons = 0;
	FILE *fp = 0;
	char filename[64] = {0};
	sprintf(filename,"%s\\%d.dat",SAVEDATAPATH,m_PlayerID);
	fp = fopen(filename,"rb");
	if(fp == NULL)
		return -1;
	while(!feof(fp))
	{
		pCons = new AutoConstruction;
		if(fread(pCons,sizeof(AutoConstruction),1,fp) ==0)
			break;

		if( ferror( fp ) )      
		{
			break;
		}
		if(pCons->MapID >0)
		{
			AddConstruction(pCons);
		}
		else
		{
			delete pCons;
			pCons = 0;
		}

#ifdef PRINT_HINTINFO
		printf("MapID:%d,CityID:%d,SortID:%d,Level:%d\n",pCons->MapID,pCons->CityID,pCons->SortID,pCons->Level);
#endif
	}	
	
	fclose(fp);
	return 0;
}
int CAutoPlayer::GetConstructionList(LPAutoConstruction pListBuf,int &count)
{
	IceUtil::RWRecMutex::RLock rlock(m_RWMutex);
	count = (int)m_AutoConsList.size();
	AutoConstruction_Iterator iter;
	int index = 0;
	for(iter = m_AutoConsList.begin();iter != m_AutoConsList.end();iter++)
	{
		pListBuf[index++] = *(iter->second);
	}
	return 0;
}

void CAutoPlayer::ClearList()
{
	IceUtil::RWRecMutex::WLock wlock(m_RWMutex);
	AutoConstruction_Iterator iter;
	LPAutoConstruction pDelCons = 0;
	for(iter = m_AutoConsList.begin();iter != m_AutoConsList.end();iter++)
	{
		if((pDelCons = iter->second))
		{
			delete pDelCons;
			pDelCons = 0;
		}
	}
	m_AutoConsList.clear();
}
int CAutoPlayer::WriteFile()
{	
	FILE *fp =0;
	char filename[64] = {0};
	sprintf(filename,"%s\\%d.dat",SAVEDATAPATH,m_PlayerID);
	fp = fopen(filename,"wb");
	if(fp == NULL)
		return -1;

	AutoConstruction_Iterator iter;
	LPAutoConstruction pCons = 0;
	for(iter = m_AutoConsList.begin();iter != m_AutoConsList.end();iter++)
	{
		//
		pCons = iter->second;
		if(pCons)
			fwrite((void*)pCons,sizeof(AutoConstruction),1,fp);
	}
	fclose(fp);
	return 0;
}
//0:成功；-1：资源不足；-2:该城市托管已结束
int CAutoPlayer::GetConstruction(CChargeTask *pChargeTask,int mapid,int cityid,AutoConstruction &autocons,DB_Proxy *pDBProxy,int &count)
{
	IceUtil::RWRecMutex::WLock wlock(m_RWMutex);
	AutoConstruction_Iterator iter;
	LPAutoConstruction pConsNode = 0;
	LPAutoConstruction pRetNode = 0;
	int iTmpLevel = 100;
	PlayerConstruction playerconstb;
	std::vector<LPAutoConstruction> vDelConsNode;
	int iRet = 0;
	//选择本地图中级别最低的建筑
	for(iter = m_AutoConsList.begin();iter != m_AutoConsList.end();iter++)
	{
		pConsNode = iter->second;
		if( pConsNode ==0)
		{
			LOG_TRACE(("CAutoPlayer::GetConstruction:pConsNode is NULL(palyerid: %d   %d-%d)\n",m_PlayerID,mapid,cityid));
			continue;
		}
		if((pConsNode->MapID == mapid) && (pConsNode->CityID == cityid))
		{//该城市
			playerconstb.setmasterid(pConsNode->MapID,pConsNode->CityID,m_PlayerID);
			playerconstb.setdetailid(pConsNode->SortID);
			if(pDBProxy->db_select(playerconstb) != DB_Proxy::DB_SUCCEED)
				continue;
			//判断升级是否已完成
			if(playerconstb.level_ >= pConsNode->Level)
			{//删除，
				/*m_AutoConsList.erase(pConsNode->GetKey());
				delete pConsNode;*/
				vDelConsNode.push_back(pConsNode);

				continue;
			}
			if(iTmpLevel <=playerconstb.level_)
				continue;

			iTmpLevel = playerconstb.level_;
			pRetNode = pConsNode;
			pRetNode->ConstructionID = playerconstb.constructionid_;
		}
	}
	//删除
	int iDelCount = (int)vDelConsNode.size();
	for(int i=0;i<iDelCount;i++)
	{
		pConsNode = vDelConsNode[i];
		m_AutoConsList.erase(pConsNode->GetKey());
		if(pConsNode)
		{
			delete pConsNode;
			pConsNode = 0;
		}
	}
	//count = (int)m_AutoConsList.size();
	if(pRetNode ==0)
	{
		iRet = -2;
	}
	else
	{//找到级别较低的托管，判断资源是否充足,并判断升一级后是否已完成，若完成则删除
		iTmpLevel++;//下一级
		ConstructionConfigure *pConscfgtb = pChargeTask->getConstructionConfigure(pRetNode->ConstructionID,iTmpLevel);
		if(pConscfgtb == NULL)
		{
			iRet = -1;
			//return -1;
			goto RETCODE;
		}

		//首先判断金额
		PlayerChatRoomTable moneytb;
		moneytb.setid(m_PlayerID);
		if(pDBProxy->db_select(moneytb) != DB_Proxy::DB_SUCCEED)
			return -1;
		if(pConscfgtb->money_ > moneytb.money_ || pConscfgtb->science_ > moneytb.science_)
		{//金钱或科技点不足
			iRet = -1;
			//return -1;
			goto RETCODE;
		}

		//再次判断资源
		PlayerResource resourcetb;
		resourcetb.setmasterid(cityid,mapid);
		resourcetb.setdetailid(m_PlayerID);
		if(pDBProxy->db_select(resourcetb) != DB_Proxy::DB_SUCCEED)
		{
			iRet = -1;
			//return -1;
			goto RETCODE;
		}
		if(resourcetb.foodcount_ < pConscfgtb->foodcount_
			|| resourcetb.woodcount_ < pConscfgtb->woodcount_
			|| resourcetb.ironcount_ < pConscfgtb->ironcount_
			|| resourcetb.skincount_ < pConscfgtb->skincount_)
		{//资源不足
			iRet = -1;
			//return -1;
			goto RETCODE;
		}
		autocons = *pRetNode;
		autocons.Level = iTmpLevel;
		//判断是否最后一级的升级
		if(iTmpLevel >= pRetNode->Level)
		{
			m_AutoConsList.erase(pConsNode->GetKey());
			if(pConsNode)
			{
				delete pConsNode;
				pConsNode = 0;
			}
			iDelCount = 1;
		}
	}
RETCODE:
	count = (int)m_AutoConsList.size();
	if(iDelCount && count)
	{
		WriteFile();
	}
	return iRet;
}
int CAutoPlayer::GetCount()
{
	IceUtil::RWRecMutex::RLock rlock(m_RWMutex);
	return (int)m_AutoConsList.size();
}
