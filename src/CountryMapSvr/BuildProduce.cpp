#include "BuildProduce.h"
//#include "LogicSystem.h"
#include "commInterface.h"
#include "ConsortiaProtocol.h"
#include "log.hpp"

#include "DataServer.h"

#pragma warning(disable:4100)
#pragma warning(disable:4189)
#pragma warning(disable:4127)
#pragma warning(disable:4244)
#pragma warning(disable:4389)
CBuildProduce::CBuildProduce()
{
	m_lFightMapID = 0L;
}
CBuildProduce::~CBuildProduce()
{
	//m_mapPlayerConstr.clear();
	//m_mapConstrCount.clear();
}
//����
bool CBuildProduce::OnBuildConstruction(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID)
{
	if (iDataSize < sizeof(CITY_INFO)) 
	{
		LOG_TRACE_EX(false,("CBuildProduce::OnBuildConstruction--->��Ϣ���Ƿ�:���%d\n",lPlayerID));
		return 0;
	}

	CITY_INFO * pCity = (CITY_INFO *)pDataBuffer;
	OnConstruction(handle, pCity->lMapID, pCity->lCityID, lPlayerID);
	
	return true;
}

//����������
int CBuildProduce::OnStartBuildCon(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID, int iAuto)
{
	if (iDataSize < sizeof(BUILD_CONSTRUCTION)) 
	{
		LOG_TRACE_EX(false,("CBuildProduce::OnStartBuildCon-->��Ϣ���Ƿ�:%d\n",lPlayerID));
		return 0;
	}
	
	BUILD_CONSTRUCTION *pPlayerCon = (BUILD_CONSTRUCTION *)pDataBuffer;
	unsigned long lSortID = pPlayerCon->lSortID;
	unsigned long lMapID = pPlayerCon->lMapID;
	unsigned long lConID = pPlayerCon->lConstructionID;
	int iLevel = pPlayerCon->iLevel;
	int ret = 0;
	int iSLevel = iLevel;
	int iFoodCount=0,iWoodCount=0,iIronCount=0,iSkinCount=0,iMoney=0,iScience=0;
	
	SUCCEED_INFO SucceedInfo;
	ERROR_INFO ErrorInfo;
	if (lConID == 5 && iSLevel == 1) return 0;
	char * buf = 0;
	int count = 0;
	//add by xujincai 20090202 �½�������Ҫ�ж�����
	if(lSortID <=0)
	{
		//���Ϊ�Ƽ�ѧԺ�����жϸÿ��Ƿ�Ϊ����ҵ����ִ壬���������ִ����������Ƽ�ѧԺ��20090605BOSS������
		if(lConID == 6)
		{
			ERROR_INFO ErrorInfo;
			PlayerCity playercity;
			playercity.setmasterid(lPlayerID);
			playercity.setdetailid(lMapID);
			if(g_Proxy.db_select(playercity) != DB_Proxy::DB_SUCCEED)
			{
				ErrorInfo.serialize(EN_DATABASE_CONNECT_FAILED, lMapID, lPlayerID);
				asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
				return ret;//EN_CLIENT_DATA_ERROR
			}
			if(playercity.state_ != 100)
			{
				ErrorInfo.serialize(EN_CLIENT_DATA_ERROR, lMapID, lPlayerID);
				asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
				return ret;//
			}
		}
		int ConCount = 0;
		PlayerConstruction PlayerConstr;
		memset(&PlayerConstr, 0, sizeof(PlayerConstruction));
		PlayerConstr.setmasterid(lMapID, pPlayerCon->lCityID, lPlayerID);
		if (g_Proxy.db_multi_select(PlayerConstr, buf, count) == DB_Proxy::DB_SUCCEED)
		{
			ConCount = count/sizeof(PlayerConstruction);
			//add by xujincai 20081215
			if(buf)
			{
				delete[] buf;
				buf = 0;
			}
			if (ConCount >=15)
			{
				if (iAuto == 1) return EN_CONSTRUCTION_UPPER;

				ErrorInfo.serialize(EN_CONSTRUCTION_UPPER, lMapID, lPlayerID);
				ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
				return 0;
			}
		}
		//���ڽ����е��½���������
		ConstructionBuild conbuilding;
		ConstructionBuild *pConBuilding = 0;
		conbuilding.setmasterid(lMapID,pPlayerCon->lCityID,lPlayerID);
		if(g_Proxy.db_multi_select(conbuilding, buf, count) == DB_Proxy::DB_SUCCEED)
		{
			count = count/sizeof(ConstructionBuild);
			pConBuilding = (ConstructionBuild*)buf;
			for(int i=0;i<count;i++)
			{
				if(pConBuilding[i].action_ ==0)
					ConCount++;
			}
			if(buf)
			{
				delete[] buf;
				buf = 0;
			}
		}
		if (ConCount >=15)
		{
			if (iAuto == 1) return EN_CONSTRUCTION_UPPER;

			ErrorInfo.serialize(EN_CONSTRUCTION_UPPER, lMapID, lPlayerID);
			ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
			return 0;
		}
	}

	ConstructionConfigure * pConstrParam = g_LoadDBinfo.getConstructionConfigure(lConID, iSLevel);
	if (pConstrParam == NULL) 
		return 0;
	
	{
		iFoodCount = pConstrParam->foodcount_;
		iWoodCount = pConstrParam->woodcount_;
		iIronCount = pConstrParam->ironcount_;
		iSkinCount = pConstrParam->skincount_;
		iMoney = pConstrParam->money_;
		iScience = pConstrParam->science_;
	}
	    	
	PALEYER_RESOURCE PlayerRes;
	memset(&PlayerRes, 0, sizeof(PALEYER_RESOURCE));
	m_UserInfo.PlayerResourceCount(0, pPlayerCon->lMapID, pPlayerCon->lCityID, lPlayerID, PlayerRes, lMapID, lPlayerID);

	//���㽨������
	if (iFoodCount > PlayerRes.iFoodCount)
	{
		if (iAuto == 1) return EN_FOOD_COUNT;

		ErrorInfo.serialize(EN_FOOD_COUNT, lMapID, lPlayerID);
		ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
		
		return ret;
	}
	if (iWoodCount > PlayerRes.iWoodCount)
	{
		if (iAuto == 1) return EN_WOOD_COUNT;

		ErrorInfo.serialize(EN_WOOD_COUNT, lMapID, lPlayerID);
		ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
		
		return ret;
	}
	if (iIronCount > PlayerRes.iIronCount)
	{
		if (iAuto == 1) return EN_IRON_COUNT;

		ErrorInfo.serialize(EN_IRON_COUNT, lMapID, lPlayerID);
		ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
		
		return ret;
	}
	if (iSkinCount > PlayerRes.iSkinCount)
	{
		if (iAuto == 1) return EN_SKIN_COUNT;

		ErrorInfo.serialize(EN_SKIN_COUNT, lMapID, lPlayerID);
		ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
		return ret;
	}
	if (iMoney > PlayerRes.iMoney)
	{
		if (iAuto == 1) return EN_MONEY_COUNT;

		ErrorInfo.serialize(EN_MONEY_COUNT, lMapID, lPlayerID);
		ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
		
		return ret;
	}
	if (iScience > PlayerRes.iScience)
	{
		if (iAuto == 1) return EN_SCIENCE_COUNT;

		ErrorInfo.serialize(EN_SCIENCE_COUNT, lMapID, lPlayerID);
		ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
		
		return ret;
	}
	if (pPlayerCon->iLevel<1 || pPlayerCon->iLevel>MAX_CONSTRUCTION_LEVEL)
	{
		if (iAuto == 1) return EN_LEVEL_UPPER;

		ErrorInfo.serialize(EN_LEVEL_UPPER, lMapID, lPlayerID);
		ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
		
		return ret;
	}
	//�ֿ�   �ȼ�1-10 
	if ((lConID == 5)&&(pPlayerCon->iLevel > 10))
	{
		if (iAuto == 1) return EN_LEVEL_UPPER;

		ErrorInfo.serialize(EN_LEVEL_UPPER, lMapID, lPlayerID);
		ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
		
		return ret;
	}
	//�Ƽ�ѧԺ   �ȼ�1-5 
	if ((lConID == 6)&&(pPlayerCon->iLevel > 5))
	{
		if (iAuto == 1) return EN_LEVEL_UPPER;

		ErrorInfo.serialize(EN_LEVEL_UPPER, lMapID, lPlayerID);
		ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
		
		return ret;
	}
	//��Ӫ ѱ�޳� ���⹤������ ũ��
	if ((lConID ==7)&&(pPlayerCon->iLevel > 1))
	{
		if (iAuto == 1) return EN_LEVEL_UPPER;

		ErrorInfo.serialize(EN_LEVEL_UPPER, lMapID, lPlayerID);
		ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
		
		return ret;
	}
	//�۳���Դ
	PlayerResource UpdataRes;
	memset(&UpdataRes, 0, sizeof(UpdataRes));

	UpdataRes.foodcount_ = -iFoodCount;
	UpdataRes.woodcount_ = -iWoodCount;
	UpdataRes.ironcount_ = -iIronCount;
	UpdataRes.skincount_ = -iSkinCount;
	UpdataRes.content_ = iFoodCount+iWoodCount+iIronCount+iSkinCount;

	UpdataRes.setmasterid(pPlayerCon->lCityID, lMapID);
	UpdataRes.setdetailid(lPlayerID);
	ret = g_Proxy.db_increase(UpdataRes);
	if (ret != DB_Proxy::DB_SUCCEED)
	{
		if (iAuto == 1) return EN_DATABASE_CONNECT_FAILED;

		ERROR_INFO ErrorInfo;
		ErrorInfo.serialize(EN_DATABASE_CONNECT_FAILED, lMapID, lPlayerID);
		asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
		return ret;
	}
	//�۳���Ǯ
	PlayerChatRoomTable PlayerMoney;
	if (iMoney > 0 || iScience > 0)
	{
		memset(&PlayerMoney, 0, sizeof(PlayerMoney));
		
		PlayerMoney.money_ = -iMoney;
		PlayerMoney.science_ = -iScience;
		PlayerMoney.setid(lPlayerID);
		g_Proxy.db_increase(PlayerMoney);
	}
	ConstructionBuild Constr;
	memset(&Constr, 0, sizeof(ConstructionBuild));

	Constr.conid_ = pPlayerCon->lConstructionID;
	Constr.level_ = iLevel;
	Constr.mapid_ = lMapID;
	Constr.cityid_ = pPlayerCon->lCityID;
	Constr.playerid_ = lPlayerID;
	Constr.action_ = pPlayerCon->lSortID;

	int iCompleteTime = int(pConstrParam->completetimes_);
	//iCompleteTime /= g_SpeedCard.GetConstrValue(lPlayerID);
	if (iCompleteTime < 1) iCompleteTime = 60;
	int iflag = 0;
	if (SelectPlayerBuild(lMapID,pPlayerCon->lCityID,lPlayerID)==1)
	{//����������ڽ���Ľ�����ý������������У��򲻻�������ʱ��
		Constr.starttime_ = 0;
		iflag = 1;
	}
	else
	{//δ���ڽ��������ʼ����ý�����������ʱ����
		Constr.starttime_ = IceUtil::Time::now().toSeconds()+iCompleteTime;
	}

	if (g_Proxy.db_insert(Constr) != DB_Proxy::DB_SUCCEED)
	{
		if (iAuto == 1) return EN_DATABASE_CONNECT_FAILED;

		ERROR_INFO ErrorInfo;
		ErrorInfo.serialize(EN_DATABASE_CONNECT_FAILED, lMapID, lPlayerID);
		ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
		LOG_TRACE_EX(false,("CBuildProduce::OnStartBuildCon ���%ld ׼�����콨����(%d)ʧ��......\n",lPlayerID,lConID));
		return ret;
	}
	if (iLevel <= 1)
	{
		lSortID = Constr.sortid_;
	}
	else
	{
		//PlayerConstruction PlayerCon;
		//memset(&PlayerCon,0,sizeof(PlayerConstruction));
		//PlayerCon.setmasterid(lMapID, pPlayerCon->lCityID, lPlayerID);
		//PlayerCon.setdetailid(pPlayerCon->lSortID);
		//g_Proxy.db_delete(PlayerCon);
	}
	
	if (iflag == 1)
	{
		if (iAuto == 1) return EN_WAIT_BUILD_CON;

		SucceedInfo.serialize(pPlayerCon->lCityID, lSortID, lConID, iLevel, 1, EN_WAIT_BUILD_CON, lMapID, lPlayerID);
		ret = asynch_write(handle, (char *)&SucceedInfo, SucceedInfo.length);
		return ret;
	}
	if (iAuto == 1)
	{
		return EN_START_BUILD_CON;
	}
	else
	{
		SucceedInfo.serialize(pPlayerCon->lCityID, lSortID, lConID, iLevel, 1, EN_START_BUILD_CON, lMapID, lPlayerID);
		ret = asynch_write(handle, (char *)&SucceedInfo, SucceedInfo.length);
	}
	//�������춨ʱ��
	BUILD_CONSTRUCTION_NODE * pBuildActivity = new BUILD_CONSTRUCTION_NODE;
	pBuildActivity->serialize(pPlayerCon->lCityID, lPlayerID,  pPlayerCon->lSortID,Constr.sortid_,lConID, iSLevel, iLevel, iAuto,EN_TIMER_CONSTRUCTION);
	long lTimeID = set_timer(iCompleteTime, (const void *)pBuildActivity);
	pBuildActivity->TimerID = lTimeID;
	//m_mapSpeedEvent.insert(make_pair(lPlayerID, pBuildActivity));
	//20100409
	PlayerOfficerTable officer;
	officer.setmasterid(lPlayerID);
	officer.setdetailid(lPlayerID);
	g_Proxy.db_select(officer);
	ISystemLog::getInstance()->playerMoney_BuildCons_Log(officer.playerid_,officer.officername_,
		PlayerRes.iMoney,PlayerMoney.money_,iMoney,Constr.mapid_,Constr.cityid_,Constr.sortid_,Constr.conid_,iSLevel,iLevel,1);
	return ret;

}
//�ѽ����õĽ�����
bool CBuildProduce::OnConstruction(int handle, unsigned long lMapID, unsigned long lCityID, unsigned long lPlayerID)
{
	char * buf = 0;
	int count = 0;
	int ret = 0,n = 0;
	PlayerConstruction Constr;
	memset(&Constr, 0, sizeof(PlayerConstruction));

	Constr.setmasterid(lMapID, lCityID, lPlayerID);
	int dbRet = g_Proxy.db_multi_select(Constr, buf, count);
	if (dbRet == DB_Proxy::DB_EOF)
	{//�ó���û�н�����������һ���ֿ�
		int lConID = 5;
		ConstructionConfigure * pConstrParam = g_LoadDBinfo.getConstructionConfigure(lConID, 1);
		if (pConstrParam == NULL) return 0;
		PlayerConstruction PlayerCon;
		memset(&PlayerCon, 0, sizeof(PlayerConstruction));
		PlayerCon.constructionid_ = lConID;
		PlayerCon.level_ = 1;
		PlayerCon.output_ = pConstrParam->modify_;
		PlayerCon.completetimes_ = IceUtil::Time::now().toSeconds();

		PlayerCon.setmasterid(m_lMapID, lCityID, lPlayerID);
		PlayerCon.mapid_ = lMapID;
		PlayerCon.cityid_ = lCityID;
		PlayerCon.playerid_ = lPlayerID;
		PlayerCon.sortid_ = 1;

		if (g_Proxy.db_insert(PlayerCon) != DB_Proxy::DB_SUCCEED)
		{
			LOG_TRACE_EX(false,("��� %ld ���佨���� %dʧ�� \n",lPlayerID, lConID));
			return 0;
		}
		Constr.setmasterid(lMapID, lCityID, lPlayerID);
		dbRet = g_Proxy.db_multi_select(Constr, buf, count);
	}
	else if (dbRet == DB_Proxy::DB_FAILED)
	{
		ERROR_INFO ErrorInfo;
		ErrorInfo.serialize(EN_DATABASE_CONNECT_FAILED, lMapID, lPlayerID);
		asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
		LOG_TRACE_EX(false,("bool CBuildProduce::OnConstruction %ld g_Proxy.db_multi_select(Constr, buf, count) error \n ",lPlayerID));
		return false;
	}
	int ConCount = count/sizeof(PlayerConstruction);
	PlayerConstruction *pPlayerCon = (PlayerConstruction *)buf;

	char * tmp = new char[sizeof(PLAYER_CONSTR_LIST)+sizeof(tagPlayerConstrList)*ConCount];
	PLAYER_CONSTR_LIST * SendCon = (PLAYER_CONSTR_LIST *)tmp;

	PALEYER_RESOURCE PlayerRes;
	memset(&PlayerRes, 0, sizeof(PALEYER_RESOURCE));

	m_UserInfo.PlayerResourceCount(0, lMapID, lCityID, lPlayerID, PlayerRes, lMapID, lPlayerID);

	(*SendCon)[n].lSortID = 0;
	(*SendCon)[n].lConstrID = 0;
	(*SendCon)[n].iLevel = 0;
	(*SendCon)[n].iUpGrade = 0;
	ConstructionConfigure *pConsCfg = 0;
	for (int i = 0; i < ConCount; i++)
	{
		if (pPlayerCon[i].constructionid_ <= 0) 
			continue;
		pConsCfg = g_LoadDBinfo.getConstructionConfigure(pPlayerCon[i].constructionid_,pPlayerCon[i].level_);
		if (pConsCfg == NULL)
		{
			continue;
		}

		(*SendCon)[n].lSortID = pPlayerCon[i].sortid_;
		(*SendCon)[n].lConstrID = pPlayerCon[i].constructionid_;
		(*SendCon)[n].iLevel = pPlayerCon[i].level_;
		(*SendCon)[n].iUpGrade = 0;

		//������������
		if ((pConsCfg->foodcount_ <= PlayerRes.iFoodCount)&&(pConsCfg->woodcount_ <= PlayerRes.iWoodCount)&&
			(pConsCfg->ironcount_ <= PlayerRes.iIronCount)&&(pConsCfg->money_ <= PlayerRes.iMoney)&&
			(pConsCfg->science_ <= PlayerRes.iScience)&&((*SendCon)[n].iLevel<=MAX_CONSTRUCTION_LEVEL))

			(*SendCon)[n].iUpGrade = 1;
		//�ֿ� �Ƽ�ѧԺ   �ȼ�1-10 
		if ((*SendCon)[n].lConstrID == 5)
		{
			if ((*SendCon)[n].iLevel >= 10)
				(*SendCon)[n].iUpGrade = 0;
		}
		//�Ƽ�ѧԺ   �ȼ�1-5 
		if ((*SendCon)[n].lConstrID == 6)
		{
			if ((*SendCon)[n].iLevel >= 5)
				(*SendCon)[n].iUpGrade = 0;
		}
		//��Ӫ ѱ�޳� ���⹤������ ũ��
		if ((*SendCon)[n].lConstrID ==7)
		{
			(*SendCon)[n].iUpGrade = 0;
		}
	
		n++;

	}
	SendCon->serialize(n, lMapID, lPlayerID);
	ret = asynch_write(handle, (char *)SendCon, SendCon->length);
	if(tmp != NULL)
	{
		delete []tmp;
		tmp = 0;
	}
	
	OnBuildingConstruction(handle, lMapID, lCityID, lPlayerID);
	if (buf != NULL)
	{
		delete [] buf;
		buf = 0;
	}
	return ret;
}
//�������ڳ����е����н�����
void CBuildProduce::OnDemolishConstruction(int handle, void * pDataBuffer, int iDataSize)
{
	if (iDataSize < sizeof(DEMOLISH_CONSTRUCTION)) 
	{
		LOG_TRACE_EX(false,("CBuildProduce::OnDemolishConstruction-->��Ϣ���Ƿ�\n"));
		return;
	}

	DEMOLISH_CONSTRUCTION * pDemolishCon = (DEMOLISH_CONSTRUCTION *)pDataBuffer;
	//add by xujincai 20090211ɾ�����ڽ����еĽ���
	//ɾ�����н���
	PlayerConstruction Constr;

	Constr.setmasterid(m_lMapID, pDemolishCon->lCityID, pDemolishCon->lPlayerID);
	char * buf = 0;
	int count = 0;

	if (g_Proxy.db_multi_select (Constr, buf, count) != DB_Proxy::DB_SUCCEED)
	{
		printf("�������ڳ����е����н����� g_Proxy.db_multi_select (Constr, buf, count) error !\n");
		return;
	}
	int ConCount = count/sizeof(PlayerConstruction);

	PlayerConstruction *pPlayerCon = (PlayerConstruction *)buf;
	for (int n = 0; n < ConCount; n++)
	{
		if (g_Proxy.db_delete(pPlayerCon[n]) != DB_Proxy::DB_SUCCEED)
		{
			LOG_TRACE_EX(false,("�������ڳ����е����н����� g_Proxy.db_delete(PlayerCon) error !\n"));
			continue;
		}
		
	}
	if (buf != NULL)
	{
		delete [] buf;
		buf = 0;
	}
	//add by xujincai 20090211ɾ�����ڽ����еĽ���
	
}
//�������ڳ����е�1������
void CBuildProduce::OnDemolishAConstruction(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID)
{
	if (iDataSize < sizeof(BUILD_CONSTRUCTION)) 
	{
		LOG_TRACE_EX(false,("CBuildProduce::OnDemolishAConstruction-->��Ϣ���Ƿ������%d\n",lPlayerID));
		return;
	}

	BUILD_CONSTRUCTION * pDemolishCon = (BUILD_CONSTRUCTION *)pDataBuffer;
	unsigned long lConID = pDemolishCon->lConstructionID;
	unsigned long lMapID = pDemolishCon->lMapID;
	//�ֿⲻ�ܲ��
	if (lConID == 5) return;
	PlayerConstruction Constr;

	Constr.setmasterid(m_lMapID, pDemolishCon->lCityID, lPlayerID);
	Constr.setdetailid(pDemolishCon->lSortID);
	//20100409
	if(g_Proxy.db_select(Constr) != DB_Proxy::DB_SUCCEED)
		return;

	if(g_Proxy.db_delete(Constr) != DB_Proxy::DB_SUCCEED)
	{
		LOG_TRACE_EX(false,("CBuildProduce::OnDemolishAConstruction-->��ѯ���%d�ڿ�%d-%d�Ľ���%d - %dʧ��\n",lPlayerID,m_lMapID, pDemolishCon->lCityID,lConID,pDemolishCon->lSortID));
		return;
	}

	SUCCEED_INFO SucceedInfo;
	SucceedInfo.serialize(pDemolishCon->lCityID, pDemolishCon->lSortID, lConID, pDemolishCon->iLevel, 1, EN_DEMOLISH_A_CONSTRUCTION, m_lMapID, lPlayerID);
	asynch_write(handle, (char *)&SucceedInfo, SucceedInfo.length);

	ConstructionConfigure * pConstrParam = g_LoadDBinfo.getConstructionConfigure(lConID, pDemolishCon->iLevel);
	if (pConstrParam == NULL) 
	{
		LOG_TRACE_EX(false,("CBuildProduce::OnDemolishAConstruction-->��ѯ���%d�ڿ�%d-%d�Ľ���%d - %dʧ��,��������%d\n",
			lPlayerID,m_lMapID, pDemolishCon->lCityID,lConID,pDemolishCon->lSortID,pDemolishCon->iLevel));
		return;
	}
	int iContent = 0;
	int iCount = 0;
	PlayerResource PlayerRes;
	PlayerRes.setmasterid(pDemolishCon->lCityID,m_lMapID);
	PlayerRes.setdetailid(lPlayerID);
	if(g_Proxy.db_select(PlayerRes) != DB_Proxy::DB_SUCCEED)
	{
		LOG_TRACE_EX(false,("CBuildProduce::OnDemolishAConstruction-->��ѯ���%d�ڿ�%d-%d�Ľ���%d - %dʧ��,��������%d\n",
			lPlayerID,m_lMapID, pDemolishCon->lCityID,lConID,pDemolishCon->lSortID,pDemolishCon->iLevel));
		return;
	}
	iContent = PlayerRes.content_;
	if(iContent <=0)
	{
		return;
	}

	memset(&PlayerRes, 0, sizeof(PlayerRes));
	PlayerRes.setmasterid(pDemolishCon->lCityID,m_lMapID);
	PlayerRes.setdetailid(lPlayerID);

	//��ٽ����� ��ԭ������Դ��50% 
	iCount = pConstrParam->foodcount_/2;
	if(iCount <=iContent)
	{
		PlayerRes.foodcount_ = iCount;
		iContent -= iCount;
	}
	else if(iContent >0)
	{
		iCount  = iContent;
		PlayerRes.foodcount_ = iCount;
		iContent -= iCount;
	}
	iCount = pConstrParam->woodcount_/2;
	if(iCount <=iContent)
	{
		PlayerRes.woodcount_ = iCount;
		iContent -= iCount;
	}
	else if(iContent >0)
	{
		iCount  = iContent;
		PlayerRes.woodcount_ = iCount;
		iContent -= iCount;
	}
	iCount = pConstrParam->ironcount_/2;
	if(iCount <=iContent)
	{
		PlayerRes.ironcount_ = iCount;
		iContent -= iCount;
	}
	else if(iContent >0)
	{
		iCount  = iContent;
		PlayerRes.ironcount_ = iCount;
		iContent -= iCount;
	}
	iCount = pConstrParam->skincount_/2;
	if(iCount <=iContent)
	{
		PlayerRes.skincount_ = iCount;
		iContent -= iCount;
	}
	else if(iContent >0)
	{
		iCount  = iContent;
		PlayerRes.skincount_ = iCount;
		iContent -= iCount;
	}
	/*
	PlayerRes.foodcount_ = pConstrParam->foodcount_/2;
	PlayerRes.woodcount_ = pConstrParam->woodcount_/2;
	PlayerRes.ironcount_ = pConstrParam->ironcount_/2;
	PlayerRes.skincount_ = pConstrParam->skincount_/2;
	*/
	PlayerRes.content_ = -(PlayerRes.foodcount_+PlayerRes.woodcount_+PlayerRes.ironcount_+PlayerRes.skincount_);

	PlayerRes.redstonecount_ = 0;
	PlayerRes.yollowstonecount_ = 0; 
	PlayerRes.greenstonecount_ = 0;
	PlayerRes.backironcount_ = 0;
	if(g_Proxy.db_increase(PlayerRes) != DB_Proxy::DB_SUCCEED)
	{
		LOG_TRACE_EX(false,("CBuildProduce::OnDemolishAConstruction-->��ѯ���%d�ڿ�%d-%d�Ľ���%d - %dʧ��,��������%d\n",
			lPlayerID,m_lMapID, pDemolishCon->lCityID,lConID,pDemolishCon->lSortID,pDemolishCon->iLevel));
		//return;
	}
	//20100409
	PlayerOfficerTable officer;
	officer.setmasterid(lPlayerID);
	officer.setdetailid(lPlayerID);
	g_Proxy.db_select(officer);
	PlayerChatRoomTable PlayerMoney;
	if (pConstrParam->money_ > 0 || pConstrParam->science_ > 0)
	{
		int tmpMoney = pConstrParam->money_/2;
		PlayerMoney.money_ =tmpMoney;
		PlayerMoney.science_ = pConstrParam->science_/2;
		PlayerMoney.setid(lPlayerID);
		g_Proxy.db_increase(PlayerMoney);
		ISystemLog::getInstance()->playerMoney_DestroyCons_Log(officer.playerid_,officer.officername_,
			PlayerMoney.money_ - tmpMoney,PlayerMoney.money_,tmpMoney,
			Constr.mapid_,Constr.cityid_,Constr.sortid_,Constr.constructionid_,Constr.level_);
	}
	char *buf = 0;
	int conscount = 0;
	PlayerConstruction allCons;
	allCons.setmasterid(Constr.mapid_,Constr.cityid_,Constr.playerid_);
	g_Proxy.db_multi_select(allCons,buf,conscount);
	conscount = conscount/sizeof(PlayerConstruction);
	if (buf)
	{
		delete[] buf;
		buf = 0;
	}
	ISystemLog::getInstance()->playerCons_Destroy_Log(officer.playerid_,officer.officername_,
		Constr.mapid_,Constr.cityid_,conscount+1,conscount,Constr.constructionid_);
}
//ȡ�������ｨ��
void CBuildProduce::OnCancelBuild(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID)
{
	if (iDataSize != sizeof(CANCEL_BUILD)) 
	{
		LOG_TRACE_EX(false,("CBuildProduce::OnCancelBuild-->��Ϣ���Ƿ������%d\n",lPlayerID));
		return;
	}

	CANCEL_BUILD * pBuild = (CANCEL_BUILD *)pDataBuffer;

	unsigned long lMapID = pBuild->lMapID;
	unsigned long lConID = pBuild->lConID;
	int iLevel = pBuild->iLevel;

	ConstructionConfigure * pConstrParam = g_LoadDBinfo.getConstructionConfigure(lConID, iLevel);
	if (pConstrParam == NULL) 
	{
		LOG_TRACE_EX(false,("CBuildProduce::OnCancelBuild-->δ֪��������%d��������%d - %d\n",lPlayerID,lConID,iLevel));
		return;
	}
	
	ConstructionBuild Constr;
	
	PlayerResource PlayerRes;

	PlayerRes.mapid_ = m_lMapID;
	PlayerRes.cityid_ = pBuild->lCityID;
	PlayerRes.playerid_ = lPlayerID;
	int iContent =0;
	if(g_Proxy.db_select(PlayerRes) != DB_Proxy::DB_SUCCEED)
	{
		return;
	}
	iContent = PlayerRes.content_;

	memset(&PlayerRes, 0, sizeof(PlayerRes));
	if(pConstrParam->foodcount_ <=iContent)
	{
		PlayerRes.foodcount_ = pConstrParam->foodcount_;
		iContent -= pConstrParam->foodcount_;
	}
	else if(iContent >0)
	{
		PlayerRes.foodcount_ = iContent;
		iContent -= iContent;
	}

	if(pConstrParam->woodcount_ <=iContent)
	{
		PlayerRes.woodcount_ = pConstrParam->woodcount_;
		iContent -= pConstrParam->woodcount_;
	}
	else if(iContent >0)
	{
		PlayerRes.woodcount_ = iContent;
		iContent -= iContent;
	}

	if(pConstrParam->ironcount_ <=iContent)
	{
		PlayerRes.ironcount_ = pConstrParam->ironcount_;
		iContent -= pConstrParam->ironcount_;
	}
	else if(iContent >0)
	{
		PlayerRes.ironcount_ = iContent;
		iContent -= iContent;
	}
	

	if(pConstrParam->skincount_ <=iContent)
	{
		PlayerRes.skincount_ = pConstrParam->skincount_;
		iContent -= pConstrParam->skincount_;
	}
	else if(iContent >0)
	{
		PlayerRes.skincount_ = iContent;
		iContent -= iContent;
	}

	//PlayerRes.foodcount_ = pConstrParam->foodcount_;
	//PlayerRes.woodcount_ = pConstrParam->woodcount_;
	//PlayerRes.ironcount_ = pConstrParam->ironcount_;
	//PlayerRes.skincount_ = pConstrParam->skincount_;
	PlayerRes.content_ = -(PlayerRes.foodcount_+PlayerRes.woodcount_+PlayerRes.ironcount_+PlayerRes.skincount_);

	PlayerRes.redstonecount_ = 0;
	PlayerRes.yollowstonecount_ = 0; 
	PlayerRes.greenstonecount_ = 0;
	PlayerRes.backironcount_ = 0;

	unsigned long lSortID = pBuild->lSortID;
	int iflag = 0;
	if (iLevel > 1)
	{
		char * buf = 0;
		int count = 0;

		//pConstrParam = g_LoadDBinfo.GetConstrConfig(lConID, iLevel-1);
		//if (pConstrParam == NULL) return;

		Constr.setmasterid(pBuild->lMapID,pBuild->lCityID, lPlayerID);
		if (g_Proxy.db_multi_select(Constr, buf, count) != DB_Proxy::DB_SUCCEED)
		{
			ERROR_INFO ErrorInfo;
			ErrorInfo.serialize(EN_DATABASE_CONNECT_FAILED, lMapID, lPlayerID);
			asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
			return;
		}
		int iItem = count/sizeof(ConstructionBuild);
		ConstructionBuild * pConstr = (ConstructionBuild *)buf;
		int n = 0;
		for(; n < iItem; n++)
		{
			if ((pConstr[n].action_ == lSortID)&&(pConstr[n].conid_ == lConID)&&(pConstr[n].level_ == iLevel))
			{
				Constr = pConstr[n];
				break;
			}
		}
		if(n >= iItem)
		{
			if (buf != NULL)
			{
				delete [] buf;
				buf = 0;
			}
			//LOG_TRACE_EX(false,("���%ld �ڿ�%ld ������%ld ȡ�����������Ľ�����%ld �ȼ�%d sortid %d ..faild\n",lPlayerID,m_lMapID,pBuild->lCityID,lConID,iLevel,lSortID));
			return;
		}
		unsigned long lActionID = pConstr[n].action_;
		lSortID = pConstr[n].sortid_;
		if (pConstr[n].starttime_ > 0)
			iflag = 1;
		pConstr[n].setmasterid(pConstr[n].mapid_,pConstr[n].cityid_, lPlayerID);
		pConstr[n].setdetailid(lConID, iLevel,pConstr[n].sortid_);
		if (g_Proxy.db_delete (pConstr[n]) != DB_Proxy::DB_SUCCEED)
		{
			ERROR_INFO ErrorInfo;
			ErrorInfo.serialize(EN_DATABASE_CONNECT_FAILED, lMapID, lPlayerID);
			asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);//add by xujinca 20081215
			if (buf != NULL)
			{
				delete [] buf;
				buf = 0;
			}
			return;
		}
		if (buf != NULL)
		{
			delete [] buf;
			buf = 0;
		}
	}
	else
	{
		Constr.mapid_ = pBuild->lMapID;
		Constr.cityid_ = pBuild->lCityID;
		Constr.playerid_ = lPlayerID;
		Constr.conid_ = lConID;
		Constr.level_ = iLevel;
		Constr.sortid_ = lSortID;

		Constr.setmasterid(pBuild->lMapID, pBuild->lCityID, lPlayerID);
		Constr.setdetailid(lConID, iLevel,lSortID);
		if (g_Proxy.db_delete (Constr) != DB_Proxy::DB_SUCCEED)
		{
			ERROR_INFO ErrorInfo;
			ErrorInfo.serialize(EN_DATABASE_CONNECT_FAILED, lMapID, lPlayerID);
			asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
			//LOG_TRACE_EX(false,("���%ld �ڿ�%ld ������%ld ȡ����Ҫ����Ľ�����%ld �ȼ�%d sortid %d ..faild\n",lPlayerID,m_lMapID,pBuild->lCityID,lConID,iLevel,pBuild->lSortID));
			return;
		}
	}

	PlayerRes.setmasterid(pBuild->lCityID, pBuild->lMapID);
	PlayerRes.setdetailid(lPlayerID);
	int ret = g_Proxy.db_increase(PlayerRes);
	if (ret != DB_Proxy::DB_SUCCEED)
	{
		ERROR_INFO ErrorInfo;
		ErrorInfo.serialize(EN_DATABASE_CONNECT_FAILED, lMapID, lPlayerID);
		asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
		LOG_TRACE_EX(false,("CBuildProduce::OnCancelBuild-->�޸����%d�ĳ���%d-%d��Դʧ��\n",lPlayerID,pBuild->lMapID,pBuild->lCityID));
		return ;
	}
	PlayerChatRoomTable PlayerMoney;
	if (pConstrParam->money_ > 0 || pConstrParam->science_ > 0)
	{
		memset(&PlayerMoney, 0, sizeof(PlayerMoney));
		
		PlayerMoney.money_ = pConstrParam->money_;
		PlayerMoney.science_ = pConstrParam->science_;
		PlayerMoney.setid(lPlayerID);
		g_Proxy.db_increase(PlayerMoney);
	}
	SUCCEED_INFO SucceedInfo;
	SucceedInfo.serialize(pBuild->lCityID, pBuild->lSortID, lConID, iLevel, 1, EN_CANCEL_BUILD_CONSTRUCTION, pBuild->lMapID, lPlayerID);
	ret = asynch_write(handle, (char *)&SucceedInfo, SucceedInfo.length);

	if (iflag == 1)
		BuildNextConstruction(m_lMapID, pBuild->lCityID, lPlayerID);
	//20100409
	PlayerOfficerTable officer;
	officer.setmasterid(lPlayerID);
	officer.setdetailid(lPlayerID);
	g_Proxy.db_select(officer);
	ISystemLog::getInstance()->playerMoney_CancelCons_Log(officer.playerid_,officer.officername_,
		PlayerMoney.money_ - pConstrParam->money_,PlayerMoney.money_,pConstrParam->money_,
		Constr.mapid_,Constr.cityid_,Constr.sortid_,lConID,iLevel,iLevel,1);

}
//����������ʱ���¼�
void CBuildProduce::ConTimeEvent(void * pDataBuffer, int iDataSize)
{
	if (iDataSize != sizeof(BUILD_CONSTRUCTION_NODE))
	{
		LOG_TRACE_EX(false,("CBuildProduce::ConTimeEvent-->��Ϣ���Ƿ�\n"));
		return;
	}

	BUILD_CONSTRUCTION_NODE * pBuild = (BUILD_CONSTRUCTION_NODE *)pDataBuffer;

	unsigned long lCity = pBuild->lCityID;
	unsigned long lConID = pBuild->lConID;
	unsigned long lPlayerID = pBuild->lPlayerID;
	int iLevel = pBuild->iLevel;
	unsigned long lSortID = pBuild->lSortID;
	unsigned long lActionID = pBuild->lActionID;
	
	if (lConID > MAX_CONSTRUCTION_NUM || iLevel > MAX_CONSTRUCTION_LEVEL) return;

	ConstructionConfigure * pConstrParam = g_LoadDBinfo.getConstructionConfigure(lConID, iLevel);
	if (pConstrParam == NULL) 
	{
		LOG_TRACE_EX(false,("CBuildProduce::ConTimeEvent-->δ֪��������������%d,������%d - %d\n",lPlayerID,lConID,iLevel));
		return;
	}
	
	ConstructionBuild Constr;
	PlayerConstruction PlayerCon;
	memset(&PlayerCon,0,sizeof(PlayerConstruction));

	Constr.setmasterid(m_lMapID,lCity, lPlayerID);
	Constr.setdetailid(lConID, iLevel,lActionID);
	if (g_Proxy.db_select(Constr) == DB_Proxy::DB_SUCCEED)
	{
		Constr.setmasterid(m_lMapID,lCity, lPlayerID);
		Constr.setdetailid(lConID, iLevel,lActionID);
		g_Proxy.db_delete (Constr);
	}
	else
	{
		return ;
	}
	if (iLevel > 1) 
	{
		lSortID = pBuild->lActionID;
		lActionID = pBuild->lSortID;
	}
	
	if (iLevel <= 1)
	{
		PlayerCon.sortid_ = lActionID+10;
		PlayerCon.constructionid_ = lConID;
		PlayerCon.level_ = iLevel;
		PlayerCon.output_ = pConstrParam->modify_;

		PlayerCon.mapid_ = m_lMapID;
		PlayerCon.cityid_ = lCity;
		PlayerCon.playerid_ = lPlayerID;
		PlayerCon.completetimes_ = IceUtil::Time::now().toSeconds();

		if (g_Proxy.db_insert(PlayerCon) != DB_Proxy::DB_SUCCEED)
		{
			PlayerCon.sortid_ = lActionID+500;
			if (g_Proxy.db_insert(PlayerCon) != DB_Proxy::DB_SUCCEED)
			{
				LOG_TRACE_EX(false,("��� %ld �����ȼ� %d �Ľ�����ʧ�� %d lSortID %d lActionID %d\n",lPlayerID, lConID, iLevel,pBuild->lSortID,pBuild->lActionID));
			}
		}
	}
	else
	{
		PlayerCon.sortid_ = lActionID;
		PlayerCon.constructionid_ = lConID;
		PlayerCon.level_ = iLevel;
		PlayerCon.output_ = pConstrParam->modify_;
		PlayerCon.mapid_ = m_lMapID;
		PlayerCon.cityid_ = lCity;
		PlayerCon.playerid_ = lPlayerID;
		PlayerCon.completetimes_ = IceUtil::Time::now().toSeconds();

		PlayerCon.setmasterid(m_lMapID, lCity, lPlayerID);
		PlayerCon.setdetailid(lActionID);
		if (g_Proxy.db_update(PlayerCon,PlayerCon) != DB_Proxy::DB_SUCCEED)
		{
			LOG_TRACE_EX(false,("************��� %ld �����ȼ� %d �Ľ�����ʧ�� %d lSortID %d\n",lPlayerID, lConID, iLevel,pBuild->lSortID));
		}
	}
	//���ڲֿ����������Ҫ���Ӹó��еĲֿ�����
	if (lConID == 5 && iLevel>1)
	{
		//add by xujincai 20081224
		ConstructionConfigure * pPreConstrParam = g_LoadDBinfo.getConstructionConfigure(lConID, iLevel-1);
		if(pPreConstrParam)
		{
			PlayerResource ResByPlayer;
			ResByPlayer.setmasterid(lCity, m_lMapID);
			ResByPlayer.setdetailid(lPlayerID);
			ResByPlayer.content_ = pConstrParam->modify_ - pPreConstrParam->modify_;
			if(g_Proxy.db_increase(ResByPlayer) != DB_Proxy::DB_SUCCEED)
			{
				LOG_TRACE_EX(false,("add city content fault(playerid:%d,mapid:%d,cityid:%d,content:%d\n",
					lPlayerID,m_lMapID,lCity,ResByPlayer.content_));
			}
		}
	}
	BuildNextConstruction(m_lMapID, lCity, lPlayerID);
	g_DataServer.broadMsgAndNotify(lPlayerID,m_lMapID,lConID,iLevel,0,EN_S_NOTIFYCONSTRUCTION,EN_S_BROADCONSTRUCTION,m_lMapID);
	//20100409
	PlayerOfficerTable officer;
	officer.setmasterid(lPlayerID);
	officer.setdetailid(lPlayerID);
	g_Proxy.db_select(officer);
	PlayerConstruction allCons;
	allCons.setmasterid(PlayerCon.mapid_,PlayerCon.cityid_,PlayerCon.playerid_);
	int conscount = 0;
	char *cons_buf = 0;
	g_Proxy.db_multi_select(allCons,cons_buf,conscount);
	conscount = conscount/sizeof(PlayerConstruction);
	if (cons_buf)
	{
		delete[] cons_buf;
		cons_buf = 0;
	}
	ISystemLog::getInstance()->playerCons_Complete_Log(officer.playerid_,officer.officername_,
		PlayerCon.mapid_,PlayerCon.cityid_,conscount -1,conscount,PlayerCon.constructionid_);
}
//������һ��������
void CBuildProduce::BuildNextConstruction(unsigned long lMapID, unsigned long lCityID, unsigned long lPlayerID)
{
	char * buf = 0;
	int count = 0;
	ConstructionBuild Building;
	memset(&Building, 0, sizeof(ConstructionBuild));

	Building.setmasterid(lMapID,lCityID,lPlayerID);
	int ret = g_Proxy.db_multi_select(Building, buf, count);
	if ( ret == DB_Proxy::DB_EOF)
	{
		PlayerProduceState State;
		State.setmasterid(Auto_Card);
		State.setdetailid(lPlayerID);
		if (g_Proxy.db_select(State) == DB_Proxy::DB_SUCCEED)
		{
			if (State.value_ == 1)
			{
				tagNotify_BuildComplete_Response BuildInfo;
				BuildInfo.serialize(lPlayerID, lMapID, lCityID, 0);
				event_notify(MDM_GP_MAP_CHARGE , (char * )&BuildInfo, BuildInfo.length);
			}
		}
		
		return;
	}
	if ( ret == DB_Proxy::DB_FAILED) return;
	int iItem = count/sizeof(ConstructionBuild);
	ConstructionBuild *pPlayerCon = (ConstructionBuild *)buf;

	int p = 0, iData = pPlayerCon[0].sortid_;
	if (iItem >= 2) iData = pPlayerCon[1].sortid_;
	for (int n = 0; n < iItem; n++)
	{
		if (pPlayerCon[n].starttime_ > 0)
		{
			p = n;
			iData = pPlayerCon[n].sortid_ ;
			break;
		}
		if (pPlayerCon[n].sortid_ < iData)
		{
			p = n;
			iData = pPlayerCon[n].sortid_ ;
		}
	}
	ConstructionConfigure * pConstrParam = g_LoadDBinfo.getConstructionConfigure(pPlayerCon[p].conid_, pPlayerCon[p].level_);
	if (pConstrParam == NULL) 
	{
		//add by xujincai 20081215
		if(buf)
		{
			delete[] buf;
			buf = 0;
		}
		return;
	}
	int iTimeL = int(pConstrParam->completetimes_);
	//iTimeL /= g_SpeedCard.GetConstrValue(lPlayerID);
	if (iTimeL < 1) iTimeL = 1;
	ConstructionBuild ConBuild;
	memset(&ConBuild, 0, sizeof(ConBuild));
	
	ConBuild.conid_ = pPlayerCon[p].conid_;
	ConBuild.mapid_ = pPlayerCon[p].mapid_;
	ConBuild.cityid_ = pPlayerCon[p].cityid_;
	ConBuild.action_ = pPlayerCon[p].action_;
	ConBuild.level_ = pPlayerCon[p].level_;
	ConBuild.playerid_ = pPlayerCon[p].playerid_;
	ConBuild.sortid_ = pPlayerCon[p].sortid_;
	ConBuild.starttime_ = IceUtil::Time::now().toSeconds()+iTimeL;

	ConBuild.setmasterid(lMapID, pPlayerCon[p].cityid_, lPlayerID);
	ConBuild.setdetailid(pPlayerCon[p].conid_,pPlayerCon[p].level_,pPlayerCon[p].sortid_);
	if (g_Proxy.db_update(pPlayerCon[p],ConBuild) != DB_Proxy::DB_SUCCEED)
	{
	}
	
	BUILD_CONSTRUCTION_NODE * pBuildActivity = new BUILD_CONSTRUCTION_NODE;

	unsigned long lSortID = pPlayerCon[p].action_;
	if (pPlayerCon[p].level_ <= 1) lSortID = pPlayerCon[p].sortid_;
	pBuildActivity->serialize(lCityID, lPlayerID, lSortID, pPlayerCon[p].sortid_,pPlayerCon[p].conid_, pPlayerCon[p].level_, pPlayerCon[p].level_, 0, EN_TIMER_CONSTRUCTION);
	long lTimeID = set_timer(iTimeL, (const void *)pBuildActivity);
	pBuildActivity->TimerID= lTimeID;
	if (buf != NULL)
	{
		delete [] buf;
		buf = 0;
	}
}
//���ڽ����еĽ�����
void CBuildProduce::OnBuildingConstruction(int handle, unsigned long lMapID, unsigned long lCityID, unsigned long lPlayerID)
{
	char * buf = 0;
	int count = 0,itmp = 0;
	ConstructionBuild Building;
	memset(&Building, 0, sizeof(ConstructionBuild));

	Building.setmasterid(lMapID,lCityID,lPlayerID);
	int ret = g_Proxy.db_multi_select(Building, buf, count);
	if (ret == DB_Proxy::DB_EOF)
	{
		PlayerProduceState State;
		State.setmasterid(Auto_Card);
		State.setdetailid(lPlayerID);
		if (g_Proxy.db_select(State) == DB_Proxy::DB_SUCCEED)
		{
			if (State.value_ == 1)
			{
				tagNotify_BuildComplete_Response BuildInfo;
				BuildInfo.serialize(lPlayerID, lMapID, lCityID, 0);
				event_notify(MDM_GP_MAP_CHARGE , (char * )&BuildInfo, BuildInfo.length);
			}
		}
		return;
	}
	if (ret ==DB_Proxy::DB_FAILED) return;
	int64 lTimeLen = IceUtil::Time::now().toSeconds();
	int iItem = count/sizeof(ConstructionBuild);
	ConstructionBuild *pPlayerCon = (ConstructionBuild *)buf;

	char * tmp = new char[sizeof(BUILDING_CONSTR_LIST)+sizeof(tagBuildingConstrList)*iItem];
	BUILDING_CONSTR_LIST * SendCon = (BUILDING_CONSTR_LIST *)tmp;
	int iflag = 0;
	int n = 0,i = 0;
	for (n = 0,i = 0; n < iItem; n++)
	{
		if (pPlayerCon[n].level_ <= 1)
			(*SendCon)[i].lSortID = pPlayerCon[n].sortid_;
		else
			(*SendCon)[i].lSortID = pPlayerCon[n].action_;
		(*SendCon)[i].lConstrID = pPlayerCon[n].conid_;
		(*SendCon)[i].iLevel = pPlayerCon[n].level_;
		int TimeL =  int(pPlayerCon[n].starttime_-lTimeLen);
		if (pPlayerCon[n].starttime_ > 0)
		{
			(*SendCon)[i].iTime = TimeL;
			iflag = 1;
		}
		else
			(*SendCon)[i].iTime  = 0;

		if ((*SendCon)[i].iTime <= 0 && pPlayerCon[n].starttime_ > 0)
		{
			pPlayerCon[n].setmasterid(pPlayerCon[n].mapid_, pPlayerCon[n].cityid_, lPlayerID);
			pPlayerCon[n].setdetailid(pPlayerCon[n].conid_, pPlayerCon[n].level_,pPlayerCon[n].sortid_);
			g_Proxy.db_delete(pPlayerCon[n]);
			//------------------------------------------------------
				int iLevel = pPlayerCon[n].level_;
				unsigned long lConID = pPlayerCon[n].conid_;
			
				ConstructionConfigure * pConstrParam = g_LoadDBinfo.getConstructionConfigure(pPlayerCon[n].conid_, pPlayerCon[n].level_);
				if (pConstrParam == NULL) continue;
				unsigned long lSortID = pPlayerCon[0].action_;
				unsigned long lActionID = pPlayerCon[0].sortid_;
				
				if (iLevel > 1) 
				{
					lSortID = pPlayerCon[0].sortid_;
					lActionID = pPlayerCon[0].action_;
				}
				PlayerConstruction PlayerCon;
				memset(&PlayerCon,0,sizeof(PlayerConstruction));
				PlayerCon.sortid_ = lActionID;
				PlayerCon.constructionid_ = lConID;
				PlayerCon.level_ = iLevel;
				PlayerCon.output_ = pConstrParam->modify_;
				PlayerCon.mapid_ = lMapID;
				PlayerCon.cityid_ = lCityID;
				PlayerCon.playerid_ = lPlayerID;
				PlayerCon.completetimes_ = IceUtil::Time::now().toSeconds()+pConstrParam->completetimes_;

				PlayerCon.setmasterid(lMapID, lCityID, lPlayerID);
				PlayerCon.setdetailid(lActionID);
				if (iLevel <= 1)
				{
					PlayerCon.sortid_ = lActionID+10;
					
					if (g_Proxy.db_insert(PlayerCon) != DB_Proxy::DB_SUCCEED)
					{
						PlayerCon.sortid_ = lActionID+600;
						g_Proxy.db_insert(PlayerCon);
					}
					else
					{
					}
				}
				else
				{
					PlayerCon.sortid_ = lActionID;
					PlayerCon.constructionid_ = lConID;
					PlayerCon.level_ = iLevel;
					PlayerCon.output_ = pConstrParam->modify_;

					PlayerCon.mapid_ = lMapID;
					PlayerCon.cityid_ = lCityID;
					PlayerCon.playerid_ = lPlayerID;
					PlayerCon.completetimes_ = IceUtil::Time::now().toSeconds()+pConstrParam->completetimes_;

					PlayerCon.setmasterid(lMapID, lCityID, lPlayerID);
					PlayerCon.setdetailid(lActionID);

					if (g_Proxy.db_update(PlayerCon,PlayerCon) != DB_Proxy::DB_SUCCEED)
					{
						printf("************��� %ld �����ȼ� %d �Ľ���22��ʧ�� %d lSortID %d\n",lPlayerID, lConID, iLevel,lSortID);
					}
				}
				//---------------------------------------------------------
			if (iItem > 1)
			{
				iflag = 1;
				(*SendCon)[0].iTime = 1;
				BuildNextConstruction(lMapID, lCityID, lPlayerID);
				itmp = 1;
				
			}
			continue;
		}
		i++;
	}
	if (iflag == 0)
	{
		BuildNextConstruction(lMapID, lCityID, lPlayerID);
		itmp = 1;
		(*SendCon)[0].iTime = 1;
	}
	if (itmp == 1)
	{
		OnConstruction(handle, lMapID, lCityID, lPlayerID);
		//add by xujincai 20081215
		if (buf != NULL)
		{
			delete [] buf;
			buf = 0;
		}
		if(tmp)
		{
			delete [] tmp;
			tmp = 0;
		}
		return ;
	}
	SendCon->serialize(i, lMapID, lPlayerID);
	asynch_write(handle, (char *)SendCon, SendCon->length);
	if (buf != NULL)
	{
		delete [] buf;
		buf = 0;
	}
	if (tmp != NULL)
	{
		delete [] tmp;
		tmp = 0;
	}
}
//��ѯ����Ƿ��н��������ڽ���
int CBuildProduce::SelectPlayerBuild(unsigned long lMapID, unsigned long lCityID, unsigned long lPlayerID)
{
	char * buf = 0;
	int iRet = 0;//add by xujincai 20081215
	int count = 0;
	ConstructionBuild Building;
	memset(&Building, 0, sizeof(ConstructionBuild));

	Building.setmasterid(lMapID,lCityID,lPlayerID);
	if (g_Proxy.db_multi_select(Building, buf, count) != DB_Proxy::DB_SUCCEED)
	{
		return 0;
	}
	int iItem = count/sizeof(ConstructionBuild);
	ConstructionBuild *pPlayerCon = (ConstructionBuild *)buf;

	for (int n = 0; n < iItem; n++)
	{
		if (pPlayerCon[n].starttime_ > 0)
		{
			//return 1;
			iRet = 1;
			break;
		}
	}
	if (buf != NULL)
	{
		delete [] buf;
		buf = 0;
	}
	//return 0;
	return iRet;
}
//�����¼�
int CBuildProduce::OnProduceEvent(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID, int iType, int iMaxLevel, int iTimeEvent)
{
	if (iDataSize < sizeof(PRODUCT_ITEM))
	{
		LOG_TRACE_EX(false,("CBuildProduce::OnProduceEvent-->��Ϣ���Ƿ������%d\n",lPlayerID));
		return 0;
	}

	PRODUCT_ITEM * pProduct = (PRODUCT_ITEM *)pDataBuffer;
	int iItemID = pProduct->iItem;
	int iCount = pProduct->iCount;
	unsigned long lMapID = pProduct->lMapID;
	unsigned long lCityID = pProduct->lCityID;
	int ret = 0;
	iType = pProduct->iType;
	SUCCEED_INFO SucceedInfo;
	ERROR_INFO ErrorInfo;
	memset(&ErrorInfo,0,sizeof(ERROR_INFO));
	//�����жϣ���������ֻ��>2
	int iProCount = ProduceListFlow(lMapID,lCityID,iType,lPlayerID);
	if (iProCount>2)
	{
		ErrorInfo.serialize(EN_PRODUCE_ITEM_UPPER, lMapID, lPlayerID);
		ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
		return ret;
	}
	//�ж��Ƿ����ж�Ӧ�Ľ���
	ret = HaveConstruction(handle, lPlayerID, lMapID, lCityID, iType);
	if (ret == 0)
	{
		ErrorInfo.serialize(EN_BUILD_CONSTRUCTION_ERROR, lMapID, lPlayerID);
		ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
		return ret;
	}
	//�ж��Ƿ�����ɶ�Ӧ�Ŀ���
	ret = HaveResearch(handle, lPlayerID, iType, iItemID);
	if (ret == 0)
	{
		ErrorInfo.serialize(EN_RESEARCH_ITEM_ERROR, lMapID, lPlayerID);
		ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
		return ret;
	}
	//��ȡ��������
	ProduceConfigure * pProParam = g_LoadDBinfo.GetProduceConfig(iItemID);
	if (pProParam == NULL) return 0;
	int iConID = 9;
	if ( iType == WEAPON_ITEM)
	{//����-����Ӧ������ID 9
		if (iItemID < 1 || iItemID >= 100)
		{
			ErrorInfo.serialize(EN_ITEM_ID_ERROR, lMapID, lPlayerID);
			ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
			printf("****************���: %ld ����ID(%d)����1������Ч��Χ�� ******\n",lPlayerID,iItemID);
			return ret;
		}
	}
	else if ( iType == DEFEND_ITEM)
	{//����-����Ӧ������ID 10
		if (iItemID < 200 || iItemID > 250)
		{
			ErrorInfo.serialize(EN_ITEM_ID_ERROR, lMapID, lPlayerID);
			ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
			printf("****************���: %ld ����ID(%d)����2������Ч��Χ�� ******\n",lPlayerID,iItemID);
			return ret;
		}
		iConID = 10;
	}
	else if ( iType == ANIMALS_TAME_ITEM)
	{//��ȡ��
		if (iItemID < 100 || iItemID >= 150)
		{
			ErrorInfo.serialize(EN_ITEM_ID_ERROR, lMapID, lPlayerID);
			ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
			printf("****************���: %ld ����ID(%d)����3������Ч��Χ�� ******\n",lPlayerID,iItemID);
			return ret;
		}
		iConID = 8;
	}
	else if ( iType == SPECIAL_TOOL_ITEM)
	{//���⹤��-����Ӧ������ID 11
		if (iItemID < 150 || iItemID >= 200)
		{
			ErrorInfo.serialize(EN_ITEM_ID_ERROR, lMapID, lPlayerID);
			ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
			printf("****************���: %ld ����ID(%d)����4������Ч��Χ�� ******\n",lPlayerID,iItemID);
			return ret;
		}
		iConID = 11;
	}

	PALEYER_RESOURCE PlayerRes;
	//memset(&PlayerRes, 0, sizeof(PALEYER_RESOURCE));

	m_UserInfo.PlayerResourceCount(0, lMapID, lCityID, lPlayerID, PlayerRes, lMapID, lPlayerID);

	//modify by xujincai 20090309�����γ˷�����	

	PlayerResource UpdataRes;	
	int iNeedMoney = 0;
	int iNeedScience = 0;
	//memset(&UpdataRes, 0, sizeof(PlayerResource));

	UpdataRes.mapid_ = lMapID;
	UpdataRes.playerid_ = lPlayerID;
	UpdataRes.cityid_ = lCityID;
	UpdataRes.foodcount_ = pProParam->foodcount_*iCount;
	UpdataRes.woodcount_ = pProParam->woodcount_*iCount;
	UpdataRes.ironcount_ = pProParam->ironcount_*iCount;
	UpdataRes.skincount_ = pProParam->skincount_*iCount;

	UpdataRes.redstonecount_ = pProParam->redstonecount_*iCount;
	UpdataRes.yollowstonecount_ = pProParam->yellowstonecount_*iCount;
	UpdataRes.greenstonecount_ = pProParam->greenstonecount_*iCount;
	UpdataRes.backironcount_ = pProParam->backironcount_*iCount;

	iNeedMoney = pProParam->money_*iCount;
	iNeedScience = pProParam->science_*iCount;
	//����������������
	if (UpdataRes.foodcount_ > PlayerRes.iFoodCount)
	{
		ErrorInfo.serialize(EN_FOOD_COUNT, lMapID, lPlayerID);
		ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
		return ret;
	}
	if (UpdataRes.woodcount_ > PlayerRes.iWoodCount)
	{
		ErrorInfo.serialize(EN_WOOD_COUNT, lMapID, lPlayerID);
		ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
		return ret;
	}
	if (UpdataRes.ironcount_ > PlayerRes.iIronCount)
	{
		ErrorInfo.serialize(EN_IRON_COUNT, lMapID, lPlayerID);
		ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
		return ret;
	}
	if (UpdataRes.skincount_ > PlayerRes.iSkinCount)
	{
		ErrorInfo.serialize(EN_SKIN_COUNT, lMapID, lPlayerID);
		ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
		return ret;
	}
	if (UpdataRes.redstonecount_ > PlayerRes.iRedStoneCount)
	{
		ErrorInfo.serialize(EN_RED_STONE_COUNT, lMapID, lPlayerID);
		ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
		return ret;
	}
	if (UpdataRes.yollowstonecount_ > PlayerRes.iYellowStoneCount)
	{
		ErrorInfo.serialize(EN_YELLOW_STONE_COUNT, lMapID, lPlayerID);
		ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
		return ret;
	}
	if (UpdataRes.greenstonecount_ > PlayerRes.iGreenStoneCount)
	{
		ErrorInfo.serialize(EN_GREEN_STONE_COUNT, lMapID, lPlayerID);
		ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
		return ret;
	}
	if (UpdataRes.backironcount_ > PlayerRes.iBackIronCount)
	{
		ErrorInfo.serialize(EN_BACK_IRON_COUNT, lMapID, lPlayerID);
		ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
		return ret;
	}
	if (iNeedMoney > PlayerRes.iMoney)
	{
		ErrorInfo.serialize(EN_MONEY_COUNT, lMapID, lPlayerID);
		ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
		return ret;
	}
	if (iNeedScience > PlayerRes.iScience)
	{
		ErrorInfo.serialize(EN_SCIENCE_COUNT, lMapID, lPlayerID);
		ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
		return ret;
	}

	//�۳���Դ
	UpdataRes.content_ = UpdataRes.foodcount_+UpdataRes.woodcount_+UpdataRes.ironcount_+UpdataRes.skincount_+
		UpdataRes.redstonecount_+UpdataRes.yollowstonecount_+UpdataRes.greenstonecount_+UpdataRes.backironcount_;	

	UpdataRes.foodcount_ = -UpdataRes.foodcount_;
	UpdataRes.woodcount_ = -UpdataRes.woodcount_;
	UpdataRes.ironcount_ = -UpdataRes.ironcount_;
	UpdataRes.skincount_ = -UpdataRes.skincount_;

	UpdataRes.redstonecount_ = -UpdataRes.redstonecount_;
	UpdataRes.yollowstonecount_ = -UpdataRes.yollowstonecount_;
	UpdataRes.greenstonecount_ = -UpdataRes.greenstonecount_;
	UpdataRes.backironcount_ = -UpdataRes.backironcount_;
	//UpdataRes.content_ -= iCount;del by xujincai 20081216
	UpdataRes.content_ = UpdataRes.content_ - iCount;//������ʼ��ʱ���Ѿ�ռ�òֿ�������ȡ������ʱ����Ҫ����ֿ�ʣ������

	UpdataRes.setmasterid(lCityID, lMapID);
	UpdataRes.setdetailid(lPlayerID);
	ret = g_Proxy.db_increase(UpdataRes);
	if (ret != DB_Proxy::DB_SUCCEED)
	{
		ERROR_INFO ErrorInfo;
		ErrorInfo.serialize(EN_DATABASE_CONNECT_FAILED, lMapID, lPlayerID);
		asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
		LOG_TRACE_EX(false,("CBuildProduce::OnProduceEvent-->���%d,����%d\n",lPlayerID,lMapID));
		return ret;
	}
	//20100409
	int oldMoney = 0;
	PlayerChatRoomTable PlayerMoney;
	if (iNeedMoney > 0 || iNeedScience > 0)
	{
		PlayerMoney.setid(lPlayerID);
		g_Proxy.db_select(PlayerMoney);
		oldMoney = PlayerMoney.money_;
		memset(&PlayerMoney, 0, sizeof(PlayerMoney));

		PlayerMoney.money_ = -iNeedMoney;
		PlayerMoney.science_ = -iNeedScience;
		PlayerMoney.setid(lPlayerID);
		g_Proxy.db_increase(PlayerMoney);
	}
	//��¼�������
	ProductionActivity ProActivity;

	ProActivity.action_ = 0;
	ProActivity.productionid_ = iItemID;
	ProActivity.type_ = iType;
	ProActivity.level_ = 1;
	ProActivity.count_ = iCount;
	ProActivity.mapid_ = lMapID;
	ProActivity.cityid_ = lCityID;
	ProActivity.playerid_ = lPlayerID;

	int iTimeL = (int)pProParam->completetimes_;
	if (iTimeL < 1) iTimeL = 10;
	//modify by xujincai 20090107 ����ʹ�ðٷֱ�,ͬʱΪ�˼��ٳ˷�����
	int iCurRate = g_SpeedCard.GetProduceValue(lPlayerID);
	if(iCurRate == 100)
	{
		iTimeL = iTimeL*iCount;
	}
	else
	{
		iTimeL = iTimeL*iCount*100/iCurRate;
	}
	iCurRate = ModifyValue(lMapID, lCityID, lPlayerID, iConID);
	if(iCurRate != 100)
	{
		iTimeL = iTimeL*100/iCurRate;
	}
	if (iTimeL <=0)
	{
		iTimeL = 1;
	}
	int iflag = 0;
	if (SelectPlayerProduce(lMapID,lCityID,iType,lPlayerID)==1)
	{//������н����е����������Ŷӣ���������ʱ��
		ProActivity.starttime_ = 0;
		iflag = 1;
	}
	else
	{//��ʼ������������ʱ��
		ProActivity.starttime_ = IceUtil::Time::now().toSeconds()+iTimeL;
	}
	
	if (g_Proxy.db_insert(ProActivity) != DB_Proxy::DB_SUCCEED)
	{
	}
	if (iflag == 1)
	{
		SucceedInfo.serialize(lCityID, ProActivity.sortid_, iItemID, 1, iCount, EN_WAIT_PRODUCE_ITEM, lMapID, lPlayerID);
		ret = asynch_write(handle, (char *)&SucceedInfo, SucceedInfo.length);
		return ret;
	}
	//���ö�ʱ��
	PRODUCE_ITEM_NODE * pProduceNode = new PRODUCE_ITEM_NODE;

	pProduceNode->serialize(lCityID, lPlayerID, iItemID, iType, ProActivity.sortid_, iCount, EN_TIMER_PRODUCE_WEAPON);

	set_timer(iTimeL, (const void *)pProduceNode);

	SucceedInfo.serialize(lCityID, ProActivity.sortid_, iItemID, 1, iCount, EN_START_PRODUCE, lMapID, lPlayerID);
	ret = asynch_write(handle, (char *)&SucceedInfo, SucceedInfo.length);
	//20100409
	if(iNeedMoney >0)
	{
		PlayerOfficerTable officer;
		officer.setmasterid(lPlayerID);
		officer.setdetailid(lPlayerID);
		g_Proxy.db_select(officer);
		ISystemLog::getInstance()->playerMoney_Produce_Log(officer.playerid_,officer.officername_,
			oldMoney,PlayerMoney.money_,iNeedMoney,lMapID,lCityID,iType,iItemID,iCount);
	}
	return ret;
}
//����ʱ���¼�
void CBuildProduce::OnProduceTimeEvent(void * pDataBuffer, int iDataSize)
{
	if (iDataSize != sizeof(PRODUCE_ITEM_NODE))
	{
		LOG_TRACE_EX(false,("CBuildProduce::OnProduceTimeEvent-->��Ϣ���Ƿ�\n"));
		return;
	}

	PRODUCE_ITEM_NODE * pProduce = (PRODUCE_ITEM_NODE *)pDataBuffer;

	unsigned long lSortID = pProduce->lSortID;
	unsigned long lCity = pProduce->lCityID;
	int iItemID = pProduce->iItemID;
	unsigned long lPlayerID = pProduce->lPlayerID;
	int	iCount = pProduce->iCount;
	int iType = pProduce->iType;

	ProductionActivity ProActivity;
	ProductionAmount ProAmount;
	memset(&ProAmount,0,sizeof(ProductionAmount));
	memset(&ProActivity,0,sizeof(ProductionActivity));

	ProActivity.setmasterid(m_lMapID, iType,lCity,lPlayerID);
	ProActivity.setdetailid(iItemID,1, lSortID);
	
	if(g_Proxy.db_delete(ProActivity)!=DB_Proxy::DB_SUCCEED)
	{
		//ProduceNextItem(m_lMapID, lCity, iType, lPlayerID);
		ProduceNextItemEx(lPlayerID,m_lMapID,lCity);
		return ;
	}

	ProAmount.productionid_ = iItemID;
	ProAmount.productioncount_ = iCount;
	ProAmount.type_ = iType;
	ProAmount.mapid_ = m_lMapID;
	ProAmount.cityid_ = lCity;
	ProAmount.playerid_ = lPlayerID;
	
	ERROR_INFO ErrorInfo;
	memset(&ErrorInfo, 0, sizeof(ERROR_INFO));
	ProductionAmount Pro;
	memset(&Pro, 0 ,sizeof(ProductionAmount));

	Pro.setmasterid(m_lMapID, lCity, lPlayerID, iType);
	Pro.setdetailid(iItemID);
	if (g_Proxy.db_select(Pro) == DB_Proxy::DB_SUCCEED)
	{
		Pro.productioncount_ += iCount;		
		Pro.setmasterid(m_lMapID, lCity, lPlayerID, iType);
		Pro.setdetailid(iItemID);
		if (g_Proxy.db_update(Pro,Pro) != DB_Proxy::DB_SUCCEED)
		{
			printf("-----��� %ld �˵���(%d)���������� ������ʧ�� %d \n",lPlayerID,iItemID,iCount);
		}
	}
	else
	{
		ProAmount.productionid_ = iItemID;
		ProAmount.productioncount_ = iCount;
		ProAmount.type_ = iType;
		ProAmount.mapid_ = m_lMapID;
		ProAmount.cityid_ = lCity;
		ProAmount.playerid_ = lPlayerID;
		if (g_Proxy.db_insert(ProAmount) != DB_Proxy::DB_SUCCEED)
		{
			printf("-----��� %ld �˵���(%d)������ ���� %d Ŀǰdb_insertʧ��\n",lPlayerID,iItemID,iCount);
		}
	}
	ProduceNextItemEx(lPlayerID,m_lMapID,lCity);
	//20100409
	PlayerOfficerTable officer;
	officer.setmasterid(lPlayerID);
	officer.setdetailid(lPlayerID);
	g_Proxy.db_select(officer);
	ISystemLog::getInstance()->playerProduce_FinPro_Log(officer.playerid_,officer.officername_,
		m_lMapID,lCity,iType,iItemID,Pro.productioncount_,Pro.productioncount_+iCount,iCount);
	//������Ϣ
	if (iCount <100)
	{
		return;
	}
	int iLevel = -1;
	/*
	WEAPON_ITEM=1,			//��������
		DEFEND_ITEM=2,			//��������
		ANIMALS_TAME_ITEM=3,	//Ұ��ѱ��
		SPECIAL_TOOL_ITEM=4,	//���⹤��
	*/
	if (iType == WEAPON_ITEM || iType == SPECIAL_TOOL_ITEM)
	{
		WeaponAttribute *pWeaponAttr = g_LoadDBinfo.GetWeaponAttribute(iItemID);
		if (pWeaponAttr)
		{
			iLevel = pWeaponAttr->level;
		}
	}
	else if(iType == DEFEND_ITEM)
	{
		DefendAttribute *pDefendAttr = g_LoadDBinfo.GetDefendAttribute(iItemID);
		if(pDefendAttr)
		{
			iLevel = pDefendAttr->level;
		}
	}
	if (iLevel >=0)
	{
		g_DataServer.broadMsgAndNotify(lPlayerID,m_lMapID,iItemID,iLevel,iType,EN_S_NOTIFYPRODUCT,EN_S_BROADPRODUCT,m_lMapID);
	}
}
//���������еĵ���
void CBuildProduce::OnProduceingItem(int handle, unsigned long lMapID, unsigned long lCityID, unsigned long lPlayerID, int iType)
{
	char * buf = 0;
	int count = 0,iItem = 0,TimeL = 0;
	int iflag = 0,n = 0,i = 0,itmp = 0;
	int64 lTimeLen = IceUtil::Time::now().toSeconds();
	char * tmp = new char[sizeof(PRODUCEING_ITEM_LIST)+sizeof(tagProduceingItemList)*SPECIAL_TOOL_ITEM];
	PRODUCEING_ITEM_LIST * SendCon1 = (PRODUCEING_ITEM_LIST *)tmp;
	ProductionActivity ProActivity;
	memset(&ProActivity, 0, sizeof(ProductionActivity));

	(*SendCon1)[0].lSortID = 0;
	(*SendCon1)[0].iItemID = 0;
	(*SendCon1)[0].iLevel = 0;
	(*SendCon1)[0].iType = 0;
	(*SendCon1)[0].iCount = 0;
	(*SendCon1)[0].iCompletetime  = 0;

	for (int m = WEAPON_ITEM; m <= SPECIAL_TOOL_ITEM; m++)
	{
		memset(&ProActivity, 0, sizeof(ProductionActivity));
		ProActivity.setmasterid(lMapID,m,lCityID,lPlayerID);
		if (g_Proxy.db_multi_select(ProActivity, buf, count) != DB_Proxy::DB_SUCCEED)
		{
			itmp++;
			continue;
		}

		int iItem = count/sizeof(ProductionActivity);
		ProductionActivity *pPlayerCon1 = (ProductionActivity *)buf;
		for (int i = 0; i < iItem; i++)
		{
			(*SendCon1)[n].lSortID = pPlayerCon1[i].sortid_;
			(*SendCon1)[n].iItemID = pPlayerCon1[i].productionid_;
			(*SendCon1)[n].iLevel = pPlayerCon1[i].level_;
			(*SendCon1)[n].iType = pPlayerCon1[i].type_;
			(*SendCon1)[n].iCount = pPlayerCon1[i].count_;
			TimeL =  int(pPlayerCon1[i].starttime_-lTimeLen);
			if (pPlayerCon1[i].starttime_ > 0)
			{
				(*SendCon1)[n].iCompletetime = TimeL;
				iflag = 1;
			}
			else
				(*SendCon1)[n].iCompletetime  = 0;
		
			if (TimeL<=0 && pPlayerCon1[i].starttime_>0)
			{
				ProductionActivity ProActivity;
				memset(&ProActivity, 0, sizeof(ProductionActivity));
				ProActivity.setmasterid(lMapID, pPlayerCon1[i].type_,lCityID,lPlayerID);
				ProActivity.setdetailid(pPlayerCon1[i].productionid_,1, pPlayerCon1[i].sortid_);
				if (g_Proxy.db_delete(ProActivity) != DB_Proxy::DB_SUCCEED) continue;

				ProductionAmount Pro;
				memset(&Pro, 0 ,sizeof(ProductionAmount));
				Pro.setmasterid(lMapID, lCityID, lPlayerID, pPlayerCon1[i].type_);
				Pro.setdetailid(pPlayerCon1[i].productionid_);
				if (g_Proxy.db_select(Pro) == DB_Proxy::DB_SUCCEED)
				{
					Pro.productioncount_ += pPlayerCon1[i].count_;
			
					Pro.setmasterid(lMapID, lCityID, lPlayerID, pPlayerCon1[i].type_);
					Pro.setdetailid(pPlayerCon1[i].productionid_);
					if (g_Proxy.db_update(Pro,Pro) != DB_Proxy::DB_SUCCEED)
					{
						printf("-----��� %ld �˵���(%d)���������� ������11ʧ��(*SendCon)[n]. %d \n",lPlayerID,pPlayerCon1[i].productionid_,pPlayerCon1[i].count_);
					}	
				}
				else
				{
					Pro.productionid_ = pPlayerCon1[i].productionid_;
					Pro.productioncount_ = pPlayerCon1[i].count_;
					Pro.type_ = pPlayerCon1[i].type_;
					Pro.mapid_ = lMapID;
					Pro.cityid_ = lCityID;
					Pro.playerid_ = lPlayerID;
					if (g_Proxy.db_insert(Pro) != DB_Proxy::DB_SUCCEED)
					{
						printf("-----��� %ld �˵���(%d)����11�� ����(*SendCon)[n]. %d Ŀǰdb_insertʧ��\n",lPlayerID,pPlayerCon1[i].productionid_, pPlayerCon1[i].count_);
					}
				}

				//ProduceNextItem(lMapID, lCityID, pPlayerCon1[i].type_,lPlayerID);
				ProduceNextItemEx(lPlayerID,lMapID,lCityID);
				OnProduceingItem(handle,lMapID,lCityID,lPlayerID,pPlayerCon1[i].type_);
				if (tmp != NULL)
				{
					delete [] tmp;
					tmp = 0;
				}
				//add by xujincai 20081215
				if(buf)
				{
					delete[] buf;
					buf = 0;
				}
				return ;
			}
			

			if(n++ > 2) break;
		}
		//add by xujincai 20081215
		if(buf)
		{
			delete[] buf;
			buf = 0;
		}
	}
	if (iflag == 0 && itmp < 3)
	{
		//ProduceNextItem(lMapID, lCityID,(*SendCon1)[0].iType,lPlayerID);
		ProduceNextItemEx(lPlayerID,lMapID,lCityID);
		OnProduceingItem(handle,lMapID,lCityID,lPlayerID,(*SendCon1)[0].iType);
		if (tmp != NULL)
		{
			delete [] tmp;
			tmp = 0;
		}
		return ;
	}
	SendCon1->serialize(n, lMapID, lPlayerID,EN_S_PRODUCEING_ITEM);
	asynch_write(handle, (char *)SendCon1, SendCon1->length);
	if (tmp != NULL)
	{
		delete [] tmp;
		tmp = 0;
	}
}
//������һ����������angelia
void CBuildProduce::ProduceNextItemEx(unsigned long playerid,unsigned long mapid,unsigned long cityid)
{
	//�Ӷ�����ѡ��һ�������������Ϊ��ǰ����
	ProductionActivity proActivity;
	ProductionActivity *pProActivityList = 0;
	char *buf[5] = {0};
	int count[5] = {0};
	int iType = 0;
	int iIndex = 0;
	int iSortID = -1;
	bool bHavActivity = false;
	int i = 0;
	for(i = WEAPON_ITEM;i<= SPECIAL_TOOL_ITEM && (!bHavActivity);i++)
	{
		proActivity.setmasterid(mapid,i,cityid,playerid);
		if(g_Proxy.db_multi_select(proActivity,buf[i],count[i]) != DB_Proxy::DB_SUCCEED)
		{
			continue;
		}
		count[i] = count[i]/sizeof(ProductionActivity);
		pProActivityList = (ProductionActivity*)buf[i];
		if(iSortID < 0)
		{
			iType = i;
			iSortID = pProActivityList[0].sortid_ ;
			iIndex = 0;
		}
		for(int j=0;j<count[i];j++)
		{
			if(pProActivityList[j].starttime_ >0)
			{//������
				iType = i;
				//iSortID = pProActivityList[j].sortid_ ;
				iIndex = j;
				bHavActivity = true;
				break;
			}
			else if(pProActivityList[j].sortid_ < iSortID)
			{
				iSortID = pProActivityList[j].sortid_ ;
				iType = i;
				iIndex = j;
			}
		}
	}

	if(iType <=0)
	{//����Ϊ��
	}
	else
	{
		pProActivityList = (ProductionActivity*)buf[iType];

		ProduceConfigure * pProParam = g_LoadDBinfo.GetProduceConfig(pProActivityList[iIndex].productionid_);
		if(pProParam == NULL)
		{
			for(i = WEAPON_ITEM;i<= SPECIAL_TOOL_ITEM;i++)
			{
				if(buf[i])
				{
					delete[] buf[i];
					buf[i] = 0;
				}
			}
			
			return;
		}
		int iConID = 9;
		if (iType == DEFEND_ITEM)
		{
			iConID = 10;
		}
		else if (iType == ANIMALS_TAME_ITEM)
		{
			iConID = 8;
		}
		else if (iType == SPECIAL_TOOL_ITEM)
		{
			iConID = 11;
		}
		int iTimeL = 0;
		if(pProActivityList[iIndex].starttime_ <=0)
		{
			iTimeL = (int)pProParam->completetimes_;
			if (iTimeL < 1) iTimeL = 10;

			int iCurRate = g_SpeedCard.GetProduceValue(playerid);
			int iCount = pProActivityList[iIndex].count_;
			if(iCurRate == 100)
			{
				iTimeL = iTimeL*iCount;
			}
			else
			{
				iTimeL = iTimeL*iCount*100/iCurRate;
			}
			iCurRate = ModifyValue(mapid, cityid, playerid, iConID);
			if(iCurRate != 100)
			{
				iTimeL = iTimeL*100/iCurRate;
			}
			pProActivityList[iIndex].starttime_ = IceUtil::Time::now().toSeconds()+iTimeL;
			if (g_Proxy.db_update(pProActivityList[iIndex],pProActivityList[iIndex]) != DB_Proxy::DB_SUCCEED)
			{
			}
		}
		else
		{
			iTimeL =(int)( pProActivityList[iIndex].starttime_  - IceUtil::Time::now().toSeconds());
			if(iTimeL <=0)
			{
				iTimeL = 1;
			}
		}
		PRODUCE_ITEM_NODE * pProduceNode = new PRODUCE_ITEM_NODE;
		pProduceNode->serialize(cityid, playerid, pProActivityList[iIndex].productionid_, pProActivityList[iIndex].type_, pProActivityList[iIndex].sortid_, pProActivityList[iIndex].count_, EN_TIMER_PRODUCE_WEAPON);
		if(set_timer(iTimeL, (const void *)pProduceNode) <=0)
		{
			LOG_TRACE_EX(false,("���ö�ʱ��ʧ��-->���%d�ھ�%d-%d��ǰ��������:���ͣ�%d,SortID:%d,ItemID:%d,Count:%d\n",playerid,mapid,cityid,iType,
			pProActivityList[iIndex].sortid_,pProActivityList[iIndex].productionid_,pProActivityList[iIndex].count_));
		}
	}
	for(i = WEAPON_ITEM;i<= SPECIAL_TOOL_ITEM;i++)
	{
		if(buf[i])
		{
			delete[] buf[i];
			buf[i] = 0;
		}
	}
}
//����ֵ
int CBuildProduce::ModifyValue(unsigned long lMapID, unsigned long lCityID, unsigned long lPlayerID,int iConID)
{
	int iValue = 0,count = 0;
	char * buf = 0;
	PlayerConstruction Constr;
	memset(&Constr, 0, sizeof(PlayerConstruction));

	Constr.setmasterid(lMapID, lCityID, lPlayerID);
	if (g_Proxy.db_multi_select(Constr, buf, count) == DB_Proxy::DB_SUCCEED)
	{
		int iCount = count/sizeof(PlayerConstruction);
		PlayerConstruction *pPlayerCon = (PlayerConstruction *)buf;
		for (int n = 0; n < iCount; n++)
		{
			if (pPlayerCon[n].constructionid_ == iConID)
			{
				iValue += pPlayerCon[n].output_;
				/*del by xujincai 20090609 
				ConstructionConfigure * pConstrParam = g_LoadDBinfo.getConstructionConfigure(iConID, pPlayerCon[n].output_);
				if (pConstrParam != NULL)
				{
					iValue += pConstrParam->modify_;
				}
				*/
			}
		}
	}
	//add by xujincai 20081215
	if(buf)
	{
		delete[] buf;
		buf = 0;
	}
	if (iValue < 100) iValue = 100;
	return iValue;
}
//��ѯ����Ƿ��е���������������
int CBuildProduce::SelectPlayerProduce(unsigned long lMapID, unsigned long lCityID, int iType, unsigned long lPlayerID)
{
	char * buf = 0;
	int count = 0,iIndex = 0;
	ProductionActivity Produceing;
	memset(&Produceing, 0, sizeof(ProductionActivity));

	int iRet = 0;
	for (int m = WEAPON_ITEM; m <= SPECIAL_TOOL_ITEM; m++)
	{
		buf = 0;
		count = 0;
		Produceing.setmasterid(lMapID,m,lCityID,lPlayerID);
		if (g_Proxy.db_multi_select(Produceing, buf, count) != DB_Proxy::DB_SUCCEED)
		{
			continue;
		}
		
		int iItem = count/sizeof(ProductionActivity);
		ProductionActivity *pPlayerCon = (ProductionActivity *)buf;

		for (int n = 0; n < iItem; n++)
		{
			if (pPlayerCon[n].starttime_ > 0)
			{
			/*	return 1; modify by xujincai 20081215*/
				iRet = 1;
				break;
			}
		}
	}
	if (buf != NULL)
	{
		delete [] buf;
		buf = 0;
	}
	return iRet;
}
//�����ж�����
int CBuildProduce::ProduceListFlow(unsigned long lMapID, unsigned long lCityID, int iType, unsigned long lPlayerID)
{
	char * buf = 0;
	int count = 0,iIndex = 0;
	ProductionActivity Produceing;

	for (int m = WEAPON_ITEM; m <= SPECIAL_TOOL_ITEM; m++)
	{
		count = 0;
		Produceing.setmasterid(lMapID,m,lCityID,lPlayerID);
		if (g_Proxy.db_multi_select(Produceing, buf, count) != DB_Proxy::DB_SUCCEED)
		{
			continue;
		}		
		if(buf)
		{
			delete[] buf;
			buf = 0;
		}
		count = count/sizeof(ProductionActivity);
		iIndex += count;
		if(iIndex >=3)
			return 3;
	}

	return iIndex;
}
//ȡ����������
void CBuildProduce::OnCancelProduce(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID)
{
	if (iDataSize != sizeof(CANCEL_PRODUCE))
	{
		LOG_TRACE_EX(false,("CBuildProduce::OnCancelProduce-->��Ϣ���Ƿ������%d\n",lPlayerID));
		return;
	}

	CANCEL_PRODUCE * pBuild = (CANCEL_PRODUCE *)pDataBuffer;
	
	ERROR_INFO ErrorInfo;

	unsigned long lSortID = pBuild->lSortID;
	unsigned long lCityID = pBuild->lCityID;
	int iType = pBuild->iType;
	int iItem = pBuild->iItemID;
	int iCount = pBuild->iCount;
	int iflag = 0;
	int64 lTimeLen = IceUtil::Time::now().toSeconds();

	ProduceConfigure * pProParam = g_LoadDBinfo.GetProduceConfig(iItem);
	if (pProParam == NULL) return;
	
	ProductionActivity ProduceItem;
	memset(&ProduceItem, 0, sizeof(ProductionActivity));
	
	ProduceItem.setmasterid(pBuild->lMapID, iType, lCityID, lPlayerID);
	ProduceItem.setdetailid(iItem, 1, lSortID);
	if (g_Proxy.db_select(ProduceItem) != DB_Proxy::DB_SUCCEED)
	{
		return;
	}
	int iConID = 9;
	if (iType == DEFEND_ITEM)
	{
		iConID = 10;
	}
	else if (iType == ANIMALS_TAME_ITEM)
	{		
		iConID = 8;
	}
	else if (iType == SPECIAL_TOOL_ITEM)
	{			
		iConID = 11;
	}
	int n = 0;
	int TimeL = int(ProduceItem.starttime_-lTimeLen);
	//modify by xujincai 20090203 ����δ��ʼ������ֱ��ȡ��ȫ������������Ҫ����δ��ɵ�����,�����������ȡ����������Ӱ��
	if (ProduceItem.starttime_ > 0)
	{//��������
		iflag = 1;
		int iTolTimeL = (int)pProParam->completetimes_;
		if (iTolTimeL < 1) iTolTimeL = 10;
		//modify by xujincai 20090107 ����ʹ�ðٷֱ�,ͬʱΪ�˼��ٳ˷�����
		int iCurRate = g_SpeedCard.GetProduceValue(lPlayerID);
		if(iCurRate == 100)
		{
			iTolTimeL = iTolTimeL*ProduceItem.count_;
		}
		else
		{
			iTolTimeL = iTolTimeL*ProduceItem.count_*100/iCurRate;
		}
		int iModifyValue = ModifyValue(pBuild->lMapID, lCityID, lPlayerID, iConID);
		if(iModifyValue != 100)
		{
			iTolTimeL = iTolTimeL*100/iModifyValue;
		}
		TimeL = iTolTimeL - TimeL;
		if(TimeL >0)
		{
			int iTP = (int)pProParam->completetimes_;//*100/g_SpeedCard.GetProduceValue(lPlayerID);//modify by xujincai 20090107 ����ʹ�ðٷֱ� *100
			if(iCurRate != 100)
			{
				iTP = iTP *100 /iCurRate;
			}
			if(iModifyValue != 100)
			{
				iTP = iTP*100/iModifyValue;
			}
			if (iTP >0)
			{
				iCount = TimeL/iTP;//����������
			}
			else
			{
				iCount = TimeL/(int)pProParam->completetimes_;
			}
		}
		else
		{
			TimeL = 0;
			iCount = 0;
		}
		if (iCount > ProduceItem.count_)
			iCount = ProduceItem.count_;
		else
			n = ProduceItem.count_-iCount;//��û���������.
	}
	else
	{//δ��ʼ����
		iCount = 0;
		n = ProduceItem.count_;
	}
	//�ж�����
	PlayerResource PlayerRes;
	int iContent = 0;
	PlayerRes.setmasterid(lCityID,pBuild->lMapID);
	PlayerRes.setdetailid(lPlayerID);
	if(g_Proxy.db_select(PlayerRes) != DB_Proxy::DB_SUCCEED)
	{
		ErrorInfo.serialize(EN_OPERATE_DBG_FAILED,pBuild->lMapID,lPlayerID);
		asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
		return;
	}
	iContent = PlayerRes.content_;

	memset(&PlayerRes, 0, sizeof(PlayerRes));
	
	PlayerRes.setmasterid(lCityID,pBuild->lMapID);
	PlayerRes.setdetailid(lPlayerID);
	PlayerRes.foodcount_ = pProParam->foodcount_*n;
	PlayerRes.woodcount_ = pProParam->woodcount_*n;
	PlayerRes.ironcount_ = pProParam->ironcount_*n;
	PlayerRes.skincount_ = pProParam->skincount_*n;
	PlayerRes.redstonecount_ = pProParam->redstonecount_*n;
	PlayerRes.yollowstonecount_ = pProParam->yellowstonecount_*n; 
	PlayerRes.greenstonecount_ = pProParam->greenstonecount_*n;
	PlayerRes.backironcount_ = pProParam->backironcount_*n;
	PlayerRes.content_ = -PlayerRes.foodcount_-PlayerRes.woodcount_-PlayerRes.ironcount_-PlayerRes.skincount_-
		PlayerRes.redstonecount_-PlayerRes.yollowstonecount_-PlayerRes.greenstonecount_-PlayerRes.backironcount_;
	PlayerRes.content_ += n;
	if(PlayerRes.content_ + iContent <0)
	{//��������
		ErrorInfo.serialize(EN_CITY_NO_CONTENT,pBuild->lMapID,lPlayerID);
		asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
		return;
	}

	ProduceItem.setmasterid(pBuild->lMapID, iType, lCityID, lPlayerID);
	ProduceItem.setdetailid(iItem, 1, lSortID);
	if (g_Proxy.db_delete (ProduceItem) != DB_Proxy::DB_SUCCEED)
	{
		return;
	}

	SUCCEED_INFO SucceedInfo;
	SucceedInfo.serialize(lCityID, lSortID, iItem, 1, iCount, EN_CANCEL_PRODUCE_ITEM, pBuild->lMapID, lPlayerID);
	int ret = asynch_write(handle, (char *)&SucceedInfo, SucceedInfo.length);

	if (iflag > 0)
		//ProduceNextItem(pBuild->lMapID, lCityID, iType, lPlayerID);
		ProduceNextItemEx(lPlayerID,pBuild->lMapID,lCityID);

	PlayerRes.setmasterid(lCityID, pBuild->lMapID);
	PlayerRes.setdetailid(lPlayerID);
	if(g_Proxy.db_increase(PlayerRes) != DB_Proxy::DB_SUCCEED)
	{
	}
	PlayerChatRoomTable PlayerMoney;
	int oldMoney = 0;
	int chgMoney =  pProParam->money_*n;
	int chgScience = pProParam->science_*n;
	if (chgMoney > 0 || chgScience> 0)
	{
		PlayerMoney.setid(lPlayerID);
		g_Proxy.db_select(PlayerMoney);
		oldMoney = PlayerMoney.money_;
		memset(&PlayerMoney, 0, sizeof(PlayerMoney));

		PlayerMoney.money_ = chgMoney;
		PlayerMoney.science_ = chgScience;
		PlayerMoney.setid(lPlayerID);
		g_Proxy.db_increase(PlayerMoney);

		PlayerOfficerTable officer;
		officer.setmasterid(lPlayerID);
		officer.setdetailid(lPlayerID);
		g_Proxy.db_select(officer);
		ISystemLog::getInstance()->playerMoney_CancelProduce_Log(officer.playerid_,officer.officername_,
			oldMoney,PlayerMoney.money_,chgMoney,pBuild->lMapID,lCityID,iType,iItem,n);
	}
	if (iCount < 1) return;
	ProductionAmount ProAmount;
	memset(&ProAmount,0,sizeof(ProductionAmount));
	ProAmount.productionid_ = iItem;
	ProAmount.productioncount_ = iCount;
	ProAmount.type_ = iType;
	ProAmount.mapid_ = pBuild->lMapID;
	ProAmount.cityid_ = lCityID;
	ProAmount.playerid_ = lPlayerID;

	memset(&ErrorInfo, 0, sizeof(ERROR_INFO));
	ProductionAmount Pro;
	memset(&Pro, 0 ,sizeof(ProductionAmount));

	Pro.setmasterid(pBuild->lMapID, lCityID, lPlayerID, iType);
	Pro.setdetailid(iItem);
	if (g_Proxy.db_select(Pro) == DB_Proxy::DB_SUCCEED)
	{
		Pro.productioncount_ += iCount;
		
		Pro.setmasterid(m_lMapID, lCityID, lPlayerID, iType);
		Pro.setdetailid(iItem);
		if (g_Proxy.db_update(Pro,Pro) != DB_Proxy::DB_SUCCEED)
		{
			printf("-----��� %ld �˵���(%d)���������� ����12��ʧ�� %d \n",lPlayerID,iItem,iCount);
		}
	}
	else
	{
		ProAmount.productionid_ = iItem;
		ProAmount.productioncount_ = iCount;
		ProAmount.type_ = iType;
		ProAmount.mapid_ = pBuild->lMapID;
		ProAmount.cityid_ = lCityID;
		ProAmount.playerid_ = lPlayerID;
		if (g_Proxy.db_insert(ProAmount) != DB_Proxy::DB_SUCCEED)
		{
			printf("-----��� %ld �˵���(%d)������ ���� %d Ŀ12ǰdb_insertʧ��\n",lPlayerID,iItem,iCount);
		}
	}
}
//�˿�����ս��,ֹͣ����,����
void CBuildProduce::SetFightMapID(unsigned long lMapID)
{ 
	if(m_lFightMapID != m_lMapID) return;
	m_lFightMapID = lMapID;
}
//��������
void CBuildProduce::WeaponProduce(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID)
{
	CITY_INFO * pWeapon = (CITY_INFO *)pDataBuffer;
//	WeaponProduce(handle, pWeapon->lCityID, lPlayerID);
	OnProduceingItem(handle, pWeapon->lMapID, pWeapon->lCityID, lPlayerID, 0);
}
//��������
void CBuildProduce::DefendProduce(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID)
{
	CITY_INFO * pDefend = (CITY_INFO *)pDataBuffer;
	//DefendProduce(handle, pDefend->lCityID, lPlayerID);
}
//ѱ�޼�������
void CBuildProduce::AnimalProduce(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID)
{
	CITY_INFO * pDefend = (CITY_INFO *)pDataBuffer;
	//AnimalProduce(handle, pDefend->lCityID, lPlayerID);
}
void CBuildProduce::SpecialProduce(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID)
{
	CITY_INFO * pDefend = (CITY_INFO *)pDataBuffer;
	//SpecialProduce(handle, pDefend->lCityID, lPlayerID);
}
//��ȡ�������ñ�
void CBuildProduce::ReadContributeConfig()
{
}
//��ȡ�����������ñ�
void CBuildProduce::ReadWeaponConfig(int iType)
{
}

//��ȡ������Ϣ
void CBuildProduce::ReadProduceState()
{
	PlayerProduceState prostatetb;
	PlayerProduceState *pProstate = 0;
	char * buf = 0;
	int count = 0;
	int i = 0;
	//Produce_Speed_Card:		//�������ٿ���������Դ����������
	prostatetb.setmasterid(AllResource_Speed_Card);
	if(g_Proxy.db_multi_select(prostatetb,buf,count) == DB_Proxy::DB_SUCCEED)
	{
		count = count/sizeof(PlayerProduceState);
		pProstate = (PlayerProduceState *)buf;
		for(i = 0;i<count;i++)
		{
			if(!g_SpeedCard.FindPlayer(pProstate[i].playerid_))
			{
				g_SpeedCard.AddPlayer(pProstate[i].playerid_);
			}
			g_SpeedCard.SetFoodValue(pProstate[i].playerid_, pProstate[i].value_);
			g_SpeedCard.SetWoodValue(pProstate[i].playerid_, pProstate[i].value_);
			g_SpeedCard.SetSkinValue(pProstate[i].playerid_, pProstate[i].value_);
			g_SpeedCard.SetIronValue(pProstate[i].playerid_, pProstate[i].value_);
		}
		delete[] buf;
		buf = 0;
		count = 0;
	}

	//Food_Speed_Card:				//��ʳ���ٿ�	
	prostatetb.setmasterid(Food_Speed_Card);
	if(g_Proxy.db_multi_select(prostatetb,buf,count) == DB_Proxy::DB_SUCCEED)
	{
		count = count/sizeof(PlayerProduceState);
		pProstate = (PlayerProduceState *)buf;
		for(i = 0;i<count;i++)
		{
			if(!g_SpeedCard.FindPlayer(pProstate[i].playerid_))
			{
				g_SpeedCard.AddPlayer(pProstate[i].playerid_);
			}
			g_SpeedCard.SetFoodValue(pProstate[i].playerid_, pProstate[i].value_);
		}
		delete[] buf;
		buf = 0;
		count = 0;
	}

	//Wood_Speed_Card:				//ľ�����ٿ�
	prostatetb.setmasterid(Wood_Speed_Card);
	if(g_Proxy.db_multi_select(prostatetb,buf,count) == DB_Proxy::DB_SUCCEED)
	{
		count = count/sizeof(PlayerProduceState);
		pProstate = (PlayerProduceState *)buf;
		for(i = 0;i<count;i++)
		{
			if(!g_SpeedCard.FindPlayer(pProstate[i].playerid_))
			{
				g_SpeedCard.AddPlayer(pProstate[i].playerid_);
			}
			g_SpeedCard.SetWoodValue(pProstate[i].playerid_, pProstate[i].value_);
		}
		delete[] buf;
		buf = 0;
		count = 0;
	}

	//Skin_Speed_Card:				//Ƥ�����ٿ�
	prostatetb.setmasterid(Skin_Speed_Card);
	if(g_Proxy.db_multi_select(prostatetb,buf,count) == DB_Proxy::DB_SUCCEED)
	{
		count = count/sizeof(PlayerProduceState);
		pProstate = (PlayerProduceState *)buf;
		for(i = 0;i<count;i++)
		{
			if(!g_SpeedCard.FindPlayer(pProstate[i].playerid_))
			{
				g_SpeedCard.AddPlayer(pProstate[i].playerid_);
			}
			g_SpeedCard.SetSkinValue(pProstate[i].playerid_, pProstate[i].value_);
		}
		delete[] buf;
		buf = 0;
		count = 0;
	}
	//Iron_Speed_Card:				//�������ٿ�
	prostatetb.setmasterid(Iron_Speed_Card);
	if(g_Proxy.db_multi_select(prostatetb,buf,count) == DB_Proxy::DB_SUCCEED)
	{
		count = count/sizeof(PlayerProduceState);
		pProstate = (PlayerProduceState *)buf;
		for(i = 0;i<count;i++)
		{
			if(!g_SpeedCard.FindPlayer(pProstate[i].playerid_))
			{
				g_SpeedCard.AddPlayer(pProstate[i].playerid_);
			}
			g_SpeedCard.SetIronValue(pProstate[i].playerid_, pProstate[i].value_);
		}
		delete[] buf;
		buf = 0;
		count = 0;
	}
}
int64 CBuildProduce::GetCurrentTime()
{
	IceUtil::Time t = IceUtil::Time::now();
	unsigned __int64 lTimeLen = t.toLongDateTime();
	return t.toLongDateTime();
}

//����ս��
void CBuildProduce::OnMapEnFight(const void * pDataBuffer, int iDataSize)
{
}
void CBuildProduce::SetMapID(unsigned long lMapID,DB_Proxy *pDBProxy)
{ 
	m_lMapID = lMapID; 
	char * buf = 0;
	int count = 0;
	PublicBuildTable PublicBuild;
	PublicBuild.setmasterid(m_lMapID);
	if (pDBProxy->db_multi_select(PublicBuild, buf, count) == DB_Proxy::DB_SUCCEED)
	{
		int iConCount = count/sizeof(PublicBuildTable);
		PublicBuildTable *pBuild = (PublicBuildTable *)buf;
		for (int n = 0; n < iConCount; n++)
		{
			if (pBuild[n].pbuildingkind_<7) continue;
			PublicBuildParamTable * pBuildParam = g_LoadDBinfo.GetPublicBuildParam(pBuild[n].pbuildingkind_, pBuild[n].currentlevel_);
			if (pBuildParam == NULL) return;


			/*
			ARCH_LUMBERYARD=6,     //ľ��3
			ARCH_STOPE     =7,     //�ɿ�1
			ARCH_GRINDERY  =8,     //ĥ��0
			ARCH_SKIN      =9,     //Ƥ�ϼӹ���2
			*/
			switch(pBuild[n].pbuildingkind_)
			{
			case ARCH_GRINDERY:
				m_UserInfo.SetOutPut(0, pBuildParam->output_);
				break;
			case ARCH_STOPE:
				m_UserInfo.SetOutPut(1, pBuildParam->output_);
				break;
			case ARCH_SKIN:
				m_UserInfo.SetOutPut(2, pBuildParam->output_);
				break;
			case ARCH_LUMBERYARD:
				m_UserInfo.SetOutPut(3, pBuildParam->output_);
				break;
			};
		}
	}
	if (buf != NULL)
	{
		delete [] buf;
		buf = 0;
	}
}
// ����ʱ �Ƿ������Ӧ�Ľ�����
int CBuildProduce::HaveConstruction(int handle, unsigned long lPlayerID, unsigned long lMapID, unsigned long lCityID, int iType)
{
	char * buf = 0;
	int count = 0;
	int ret = 0,n = 0;
	PlayerConstruction Constr;
	memset(&Constr, 0, sizeof(PlayerConstruction));

	Constr.setmasterid(lMapID, lCityID, lPlayerID);
	int dbRet = g_Proxy.db_multi_select(Constr, buf, count);
	if (dbRet == DB_Proxy::DB_EOF)
	{
		return ret;
	}
	else if (dbRet == DB_Proxy::DB_FAILED)
	{
		ERROR_INFO ErrorInfo;
		ErrorInfo.serialize(EN_DATABASE_CONNECT_FAILED, lMapID, lPlayerID);
		asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
		LOG_TRACE_EX(false,("bool CBuildProduce::HaveConstruction %ld g_Proxy.db_multi_select(Constr, buf, count) error \n ",lPlayerID));
		return 2;
	}
	int ConCount = count/sizeof(PlayerConstruction);
	PlayerConstruction *pPlayerCon = (PlayerConstruction *)buf;

	for (int i=0; i < ConCount; i++)
	{
		if ((iType == 1)&&(pPlayerCon[i].constructionid_ == 9))
			ret = 1;
		else if ((iType == 2)&&(pPlayerCon[i].constructionid_ == 10))
			ret = 1;
		else if ((iType == 3)&&(pPlayerCon[i].constructionid_ == 8))
			ret = 1;
		else if ((iType == 4)&&(pPlayerCon[i].constructionid_ == 11))
			ret = 1;
	}
	if (buf != NULL)
	{
		delete [] buf;
		buf = 0;
	}
	return ret;
}
	//����ʱ �Ƿ������Ӧ���м���
int CBuildProduce::HaveResearch(int handle, unsigned long lPlayerID, int iType, int iItemID)
{
	ResearchItem PlayerResearch;
	memset(&PlayerResearch, 0, sizeof(ResearchItem));
	PlayerResearch.setmasterid(lPlayerID, iType);
	PlayerResearch.setdetailid(iItemID);
	int dbRet = g_Proxy.db_select(PlayerResearch);
	if (dbRet == DB_Proxy::DB_SUCCEED)
	{
		return 1;	
	}
	if (dbRet == DB_Proxy::DB_FAILED)
	{
		ERROR_INFO ErrorInfo;
		ErrorInfo.serialize(EN_DATABASE_CONNECT_FAILED, m_lMapID, lPlayerID);
		asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
		LOG_TRACE_EX(false,("bool CBuildProduce::HaveConstruction  g_Proxy.db_multi_select(Constr, buf, count) error \n "));
		return 2;
	}
	
	return 0;
}
//������Ϣ
void CBuildProduce::OnContributeInfo(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID)
{
	if (iDataSize < sizeof(CITY_INFO))
	{
		LOG_TRACE_EX(false,("CBuildProduce::OnContributeInfo-->��Ϣ���Ƿ������%d\n",lPlayerID));
		return ;
	}

	CITY_INFO * pCity = (CITY_INFO *)pDataBuffer;

	int ret = 0;
	PlayerChatRoomTable ResByPlayer;
	ERROR_INFO ErrorInfo;
	ResByPlayer.setid(lPlayerID);
	if (g_Proxy.db_select(ResByPlayer) != DB_Proxy::DB_SUCCEED)
	{
		ErrorInfo.serialize(EN_DATA_ERROR, m_lMapID, lPlayerID);
		ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
		return;
	}
	Treasury Tr;
	Tr.setid(ResByPlayer.country_);
	ret = g_Proxy.db_select(Tr);
	if (ret == DB_Proxy::DB_FAILED)
	{
		ErrorInfo.serialize(EN_DATABASE_CONNECT_FAILED, m_lMapID, lPlayerID);
		ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
		return;
	}
	else if (ret == DB_Proxy::DB_EOF)
	{
		Tr.money_ = 0;
	}
	
	PublicBuildTable PublicBuild;
	PublicBuild.setmasterid(pCity->lMapID);
	PublicBuild.setdetailid(pCity->lCityID);
	if (g_Proxy.db_select(PublicBuild) != DB_Proxy::DB_SUCCEED)
	{
		return;
	}
	int iTolMoney = 0;
	PublicBuildParamTable * pBuildParam = g_LoadDBinfo.GetPublicBuildParam(PublicBuild.pbuildingkind_, PublicBuild.currentlevel_);
	if (pBuildParam == NULL)
	{
		LOG_ERROR(("CBuildProduce::OnContributeInfo-->�ù�������%d-%d�����ڣ����%d\n",PublicBuild.pbuildingkind_, PublicBuild.currentlevel_,lPlayerID));
		return;
	}
	PublicBuildParamTable * pBuildParam1 = g_LoadDBinfo.GetPublicBuildParam(PublicBuild.pbuildingkind_, PublicBuild.currentlevel_+1);
	if (pBuildParam1 != NULL) iTolMoney=pBuildParam1->upgradefee_;

	CONTRIBUTE_INFO ConInfo;
	ConInfo.serialize(pCity->lCityID, PublicBuild.currentlevel_, PublicBuild.currentmoney_, iTolMoney, pBuildParam->attack_, pBuildParam->hp_,
		pBuildParam->attackscop_, pBuildParam->visualrange_, Tr.money_, ResByPlayer.money_, pCity->lMapID, lPlayerID);
	ret = asynch_write(handle, (char *)&ConInfo, ConInfo.length);
}
//���׽�����
void CBuildProduce::Contribute(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID)
{
	if (iDataSize != sizeof(PLAYER_CONTRIBUTE)) 
	{
		LOG_TRACE_EX(false,("CBuildProduce::Contribute-->��Ϣ���Ƿ������%d\n",lPlayerID));
		return;
	}

	PLAYER_CONTRIBUTE * pContribute = (PLAYER_CONTRIBUTE *)pDataBuffer;
	int ret = 0;
	ERROR_INFO ErrorInfo;
	if (g_MapInfo->GetMapState() == war)
	{
		ErrorInfo.serialize(EN_MAP_INWAR, m_lMapID, lPlayerID);
		ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
		return;
	}
	if (pContribute->iMoney < 1000)
	{
		ErrorInfo.serialize(EN_CONTRIBUTE_1000, m_lMapID, lPlayerID);
		ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
		return;
	}
	PlayerChatRoomTable ResByPlayer;
	ResByPlayer.setid(lPlayerID);
	if (g_Proxy.db_select(ResByPlayer) != DB_Proxy::DB_SUCCEED)
	{
		ErrorInfo.serialize(EN_DATA_ERROR, m_lMapID, lPlayerID);
		ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
		return;
	}
	LPGMCreateNPCParam_Item pHireArmyParam = 0;
	PublicBuildParamTable * pBuildParam = 0;
	if (pContribute->iType <0)
	{
		pHireArmyParam = g_LoadDBinfo.GetHiredNPCArmyParam(ResByPlayer.country_,1);
		if (pHireArmyParam == NULL)
		{
			return;
		}
		pContribute->iMoney = pHireArmyParam->Price;
	}
	else
	{

	}
	if (ResByPlayer.money_ < pContribute->iMoney)
	{
		ErrorInfo.serialize(EN_MONEY_COUNT, m_lMapID, lPlayerID);
		ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
		return;
	}
	int oldMoney = ResByPlayer.money_;
	MapInfoTable Map_Info;
	memset(&Map_Info, 0, sizeof(Map_Info));
	Map_Info.setid(pContribute->lMapID);
	if (g_Proxy.db_select(Map_Info) != DB_Proxy::DB_SUCCEED)
	{
		printf("���� g_Proxy.db_select(Map_Info) \n");
	}
	
	if (Map_Info.countryid_ != ResByPlayer.country_)
	{
		ErrorInfo.serialize(EN_NOT_SELF_MAP, m_lMapID, lPlayerID);
		ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
		return ;
	}
	int iOutPut = 0,  iCurMoney = 0, iL = 0, iType = 0, iCurLevel = 0;
	int buildid = 0;
	if (pContribute->iType >=0 )
	{
		PublicBuildTable PublicBuild;
		PublicBuild.setmasterid(pContribute->lMapID);
		PublicBuild.setdetailid(pContribute->lCityID);
		if (g_Proxy.db_select(PublicBuild) != DB_Proxy::DB_SUCCEED)
		{
			ErrorInfo.serialize(EN_DATA_ERROR, m_lMapID, lPlayerID);
			ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
			return;
		}
		buildid = PublicBuild.pbuildingid_;
		/*
		ARCH_BUILD_TOFT = 50, //�����ػ����������潨�����壬դ���Ƚ���
		ARCH_TRAP      =51,    //����
		ARCH_BARRIER   =52,    //դ��
		*/
		iType = PublicBuild.pbuildingkind_;
		iCurLevel = PublicBuild.currentlevel_;
		if (iType == ARCH_BUILD_TOFT)
		{
			iType = pContribute->iType;
			iCurLevel = 0;
			PublicBuild.currentmoney_ = 0;
		}
		pBuildParam = g_LoadDBinfo.GetPublicBuildParam(iType, iCurLevel+1);//��ȡ��һ����
		if (pBuildParam == NULL) 
		{//EN_LEVEL_UPPER
			ErrorInfo.serialize(EN_LEVEL_UPPER, m_lMapID, lPlayerID);
			ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
			return;
		}
		if (PublicBuild.pbuildingkind_ == ARCH_BUILD_TOFT)
		{
			if (pContribute->iMoney < pBuildParam->upgradefee_)
			{
				ErrorInfo.serialize(EN_MONEY_COUNT, m_lMapID, lPlayerID);
				ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
				return;
			}
			if (ResByPlayer.money_ < pContribute->iMoney)
			{
				ErrorInfo.serialize(EN_MONEY_COUNT, m_lMapID, lPlayerID);
				ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
				return;
			}
		}
		iCurMoney = PublicBuild.currentmoney_+pContribute->iMoney;
		while (iCurMoney >= pBuildParam->upgradefee_)
		{
			++iL;//����
			iOutPut = pBuildParam->output_;
			iCurMoney -=pBuildParam->upgradefee_;
			pBuildParam = g_LoadDBinfo.GetPublicBuildParam(iType, iCurLevel+iL +1);//��ȡ��һ����
			if(pBuildParam == NULL)
				break;
		}
		/*
		iOutPut = pBuildParam->output_;
		if (pBuildParam->upgradefee_ <= iCurMoney)
		{
			++iL;
			iM = iCurMoney - pBuildParam->upgradefee_;
			while(iM >= pBuildParam->upgradefee_)
			{
				pBuildParam = g_LoadDBinfo.GetPublicBuildParam(iType, iCurLevel+iL);
				if (pBuildParam == NULL) break;
				if (pBuildParam->upgradefee_ > iM) break;
				++iL;
				iCurMoney = iM - pBuildParam->upgradefee_;
				if (iCurMoney <= 0) break;
				iM = iCurMoney;
				iOutPut = pBuildParam->output_;
				continue;
			}
			iCurMoney = iM;
		}
		*/
		if (PublicBuild.pbuildingkind_ == ARCH_BUILD_TOFT)
		{
			PublicBuild.pbuildingkind_ = iType;
		}
		PublicBuild.currentlevel_ = iCurLevel+iL;
		PublicBuild.currentmoney_ = iCurMoney;
		PublicBuild.setmasterid(pContribute->lMapID);
		PublicBuild.setdetailid(pContribute->lCityID);
		if (g_Proxy.db_update(PublicBuild, PublicBuild) != DB_Proxy::DB_SUCCEED)
		{
			return;
		}
	}
	else
	{
		if (Map_Info.state_ == 100)
		{
			//EN_NOT_CREATGARRISON_INSAFEMAP
			ErrorInfo.serialize(EN_NOT_CREATGARRISON_INSAFEMAP, m_lMapID, lPlayerID);
			ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
			return ;
		}
		//�����������ж�
		if(GetGarrisonCount() >=50)
		{
			ErrorInfo.serialize(EN_MAX_ARMYCOUNT, m_lMapID, lPlayerID);
			ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
			return;
		}
		Treasury Tr;
		Tr.setid(ResByPlayer.country_);
		ret = g_Proxy.db_select(Tr);
		if (ret == DB_Proxy::DB_FAILED)
		{
			ErrorInfo.serialize(EN_DATABASE_CONNECT_FAILED, m_lMapID, lPlayerID);
			ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
			return;
		}
		else if (ret == DB_Proxy::DB_EOF)
		{
			Tr.country_ = ResByPlayer.country_;
			Tr.money_ = pContribute->iMoney;
			g_Proxy.db_insert(Tr);
		}
		else
		{
			memset(&Tr, 0, sizeof(Tr));
			Tr.money_ = pContribute->iMoney;
			Tr.setid(ResByPlayer.country_);
			if (g_Proxy.db_increase(Tr) != DB_Proxy::DB_SUCCEED)
			{
				ErrorInfo.serialize(EN_DATABASE_CONNECT_FAILED, m_lMapID, lPlayerID);
				ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
				LOG_TRACE_EX(false,("���%���׹�����ʱ��д��Treasuryʧ��\n",lPlayerID));
				return;
			}
		}
		g_DataServer.AddArmyInfo(1,pHireArmyParam->AmyName,pHireArmyParam->PlayerID,pHireArmyParam->OfficerID,pHireArmyParam->RaceID,pHireArmyParam->WeaponType,pHireArmyParam->WeaponLevel,
			pHireArmyParam->DefendType,pHireArmyParam->DefendLevel,pHireArmyParam->SoldierType,pHireArmyParam->SoldierCount);
	}
	memset(&ResByPlayer, 0, sizeof(ResByPlayer));
	ResByPlayer.money_ = -pContribute->iMoney;
	ResByPlayer.contribute_ = pContribute->iMoney;

	ResByPlayer.setid(lPlayerID);
	if (g_Proxy.db_increase(ResByPlayer) != DB_Proxy::DB_SUCCEED)
	{
		ErrorInfo.serialize(EN_DATABASE_CONNECT_FAILED, m_lMapID, lPlayerID);
		ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
		return;
	}	
	PlayerOfficerTable Player;
	Player.setmasterid(lPlayerID);
	Player.setdetailid(lPlayerID);
	if (g_Proxy.db_select(Player) != DB_Proxy::DB_SUCCEED)
	{
		ErrorInfo.serialize(EN_DATABASE_CONNECT_FAILED, m_lMapID, lPlayerID);
		ret = asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
		return;
	}

	int iCredit = pContribute->iMoney /5000;
	BROADCAST_CONTRIBUTE Broadcast;
	if(pContribute->iType >=0)
	{
		Broadcast.serialize(Player.officername_, pContribute->lMapID, pContribute->lCityID, iCurLevel+iL, iType, pContribute->iMoney,iCredit, pContribute->lMapID, lPlayerID);
	}
	else
	{
		Broadcast.serialize(Player.officername_, pContribute->lMapID, pContribute->lCityID, iCurLevel+iL, pContribute->iType, pContribute->iMoney,iCredit, pContribute->lMapID, lPlayerID);
	}
	ret = broadcast((char *)&Broadcast, Broadcast.length);
	//add by xujincai 20090403���ӽ���������
	if(iCredit >0)
	{
		tagHeroParam param;
		memset(&param,0,sizeof(tagHeroParam));
		param.lPlayerID = lPlayerID;
		param.lHeroID = lPlayerID;
		param.iCredit = iCredit;
		OnHeroGrowUp(&g_Proxy,param,CONTRIBUTE_METHOD);
	}
	//20100409
	ISystemLog::getInstance()->playerMoney_Contribute_Log(Player.playerid_,Player.officername_,
		oldMoney,ResByPlayer.money_,pContribute->iMoney,pContribute->lMapID,pContribute->iType,buildid);
	if (iL >= 1)
	{
		/*
		ARCH_LUMBERYARD=6,     //ľ��3
		ARCH_STOPE     =7,     //�ɿ�1
		ARCH_GRINDERY  =8,     //ĥ��0
		ARCH_SKIN      =9,     //Ƥ�ϼӹ���2
		*/
		switch(iType)
		{
			break;
		case ARCH_GRINDERY:
			m_UserInfo.SetOutPut(0, iOutPut);
			break;
		case ARCH_STOPE:
			m_UserInfo.SetOutPut(1, iOutPut);
			break;
		case ARCH_SKIN:
			m_UserInfo.SetOutPut(2, iOutPut);
			break;
		case ARCH_LUMBERYARD:
			m_UserInfo.SetOutPut(3, iOutPut);
			break;
		};
		UpdatePublicBuildLevel(iType,pContribute->lCityID, iCurLevel + iL);
	}
}
//���콨���������й���
void CBuildProduce::ConstrAutoBulid(void * pDataBuffer, int iDataSize)
{
	if (iDataSize != sizeof(tagNotify_BuildConstruction_Request))
	{
		LOG_TRACE_EX(false,("CBuildProduce::ConstrAutoBulid-->��Ϣ���Ƿ�\n"));
		return;
	}

	tagNotify_BuildConstruction_Request *pBuildConstr = (tagNotify_BuildConstruction_Request *)pDataBuffer;
	BUILD_CONSTRUCTION Build;
	Build.serialize(pBuildConstr->ConstructionInfo.MapID, pBuildConstr->ConstructionInfo.CityID, pBuildConstr->ConstructionInfo.SortID,
		pBuildConstr->ConstructionInfo.ConstructionID, pBuildConstr->ConstructionInfo.Level);
	int ret = OnStartBuildCon(0, &Build, Build.length, pBuildConstr->PlayerID, 1);
	if (ret == 1) return;
	int Code = ret;
	if (ret >= EN_FOOD_COUNT && ret <= EN_BACK_IRON_COUNT) Code = 1;
	tagNotify_BuildComplete_Response BuildInfo;
	BuildInfo.serialize(pBuildConstr->PlayerID, pBuildConstr->ConstructionInfo.MapID, pBuildConstr->ConstructionInfo.CityID, Code);
	event_notify(MDM_GP_MAP_CHARGE , (char * )&BuildInfo, BuildInfo.length);
}
//�ռ���Դ
void CBuildProduce::OnCollectResource(void * pDataBuffer, int iDataSize)
{
	if (iDataSize != sizeof(tagNotify_CollectResource_Request))
	{
		LOG_TRACE_EX(false,("CBuildProduce::OnCollectResource-->��Ϣ���Ƿ�\n"));
		return;
	}

	tagNotify_CollectResource_Request *pCollectRes = (tagNotify_CollectResource_Request *)pDataBuffer;

	PALEYER_RESOURCE PlayerRes;
	memset(&PlayerRes, 0, sizeof(PALEYER_RESOURCE));
	int ret = m_UserInfo.PlayerResourceCount(1, pCollectRes->MapID, pCollectRes->CityID, pCollectRes->PlayerID, PlayerRes);
	
	tagNotify_CollectResource_Response CollectRes;
	CollectRes.serialize(pCollectRes->PlayerID, pCollectRes->MapID, pCollectRes->CityID,ret-1);
	event_notify(MDM_GP_MAP_CHARGE , (char * )&CollectRes, CollectRes.length);
}
void CBuildProduce::OnConstrSpeed(unsigned long lPlayerID,int cityid,int iSortID, int iValue)
{
	tagNotify_IncreaseOtherSpeed_Response ReqSpeed;
	map<unsigned long, BUILD_CONSTRUCTION_NODE*>::iterator it = m_mapSpeedEvent.find(lPlayerID);
	if (it == m_mapSpeedEvent.end())
	{
		ReqSpeed.serialize(lPlayerID, Construction_Speed_Card, 1);
		event_notify(MDM_GP_MAP_CHARGE , (char * )&ReqSpeed, ReqSpeed.length);
		return;
	}
	ConstructionBuild Constr;
	Constr.setmasterid(m_lMapID, cityid, lPlayerID);
	Constr.setdetailid(it->second->lConID, it->second->iSLevel, it->second->lActionID);
	if (g_Proxy.db_select(Constr) != DB_Proxy::DB_SUCCEED)
	{
		ReqSpeed.serialize(lPlayerID, Construction_Speed_Card, 1);
		event_notify(MDM_GP_MAP_CHARGE , (char * )&ReqSpeed, ReqSpeed.length);
		return;
	}
	int64 lTimeLen = IceUtil::Time::now().toSeconds();
	int iCompleteTime = int(Constr.starttime_ - lTimeLen);
	iCompleteTime /= iValue;
	if (iCompleteTime < 1) iCompleteTime = 60;
	it->second->serialize(it->second->lCityID, lPlayerID,  it->second->lSortID,it->second->lActionID,it->second->lConID, 
		it->second->iSLevel, it->second->iLevel, it->second->iAuto, EN_TIMER_CONSTRUCTION);

	cancel_timer(it->second->TimerID);
	long lTimeID = set_timer(iCompleteTime, (const void *)it->second);
	it->second->TimerID = lTimeID;
	Constr.starttime_ = IceUtil::Time::now().toSeconds()+iCompleteTime;
	g_Proxy.db_update(Constr, Constr);
	ReqSpeed.serialize(lPlayerID, Construction_Speed_Card, 0);
	event_notify(MDM_GP_MAP_CHARGE , (char * )&ReqSpeed, ReqSpeed.length);
	//m_mapSpeedEvent.insert(make_pair(lPlayerID, it->second));
}

//�������
void CBuildProduce::OnImmedComplete(LPImmedComplete_Request msg)
{//��������������
	LPImmedComplete_Request req_msg = (LPImmedComplete_Request)msg;
	if(req_msg->length != sizeof(ImmedComplete_Request))
	{
		LOG_TRACE_EX(false,("CBuildProduce::OnImmedComplete-->��Ϣ���Ƿ�\n"));
		return;
	}
	Notify_ImmedComplete_Request notify_msg;
	ImmedComplete_Response rep_msg;
	int iNeedGO = 0;
	int iRet = 1;
	if(req_msg->Type == Construction_Speed_Card)
	{//����ConstructionBuild
		//�����ж��Ƿ����ڽ�����
		ConstructionBuild consbuild;
		consbuild.setmasterid(req_msg->MapID,req_msg->CityID,req_msg->from);
		if(req_msg->Level <= 1)
		{//req_msg->SortID = sortid			
			consbuild.setdetailid(req_msg->ItemID,req_msg->Level,req_msg->SortID);
			if(g_Proxy.db_select(consbuild) == DB_Proxy::DB_SUCCEED)
			{
				iRet = 0;
			}
		}
		else
		{//req_msg->SortID = action
			ConstructionBuild *pRetSet  = 0;
			char *buf = 0;
			int count = 0;
			if(g_Proxy.db_multi_select(consbuild,buf,count) == DB_Proxy::DB_SUCCEED)
			{
				count = count/sizeof(ConstructionBuild);
				pRetSet = (ConstructionBuild*)buf;
				for(int i=0;i<count;i++)
				{
					if(pRetSet[i].action_ == req_msg->SortID 
						&& pRetSet[i].conid_ == req_msg->ItemID 
						&& pRetSet[i].level_ == req_msg->Level)
					{
						iRet = 0;
						memcpy(&consbuild,&pRetSet[i],sizeof(ConstructionBuild));
						break;
					}
				}

				if(buf)
				{
					delete[] buf;
					buf = 0;
				}
			}
		}
		if(iRet)
		{//������
			rep_msg.serialize(req_msg->agent,req_msg->MapID,req_msg->from,2,req_msg->Type,req_msg->SortID,0);
			asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
			return;
		}

		/*
		*/
		if(consbuild.starttime_ <=0)
		{//δ��ʼ
			rep_msg.serialize(req_msg->agent,req_msg->MapID,req_msg->from,2,req_msg->Type,req_msg->SortID,0);
			asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
			return;
		}
		//������Ҫ��GO��,�շѱ�׼ 4GO��/Сʱ
		__int64 iTimes = consbuild.starttime_ - IceUtil::Time::now().toSeconds();
		iNeedGO = int((iTimes)/900);
		if(iNeedGO <=0)
			iNeedGO = 1;

	}
	else if(req_msg->Type == Produce_Speed_Card)
	{//����
		ProductionActivity proactivity;
		proactivity.setmasterid(req_msg->MapID,req_msg->ProductionType,req_msg->CityID,req_msg->from);
		proactivity.setdetailid(req_msg->ItemID,req_msg->Level,req_msg->SortID);
		if(g_Proxy.db_select(proactivity) != DB_Proxy::DB_SUCCEED)
		{
			rep_msg.serialize(req_msg->agent,req_msg->MapID,req_msg->from,2,req_msg->Type,req_msg->SortID,0);
			asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
			return;
		}
		if(proactivity.starttime_ <=0)
		{
			rep_msg.serialize(req_msg->agent,req_msg->MapID,req_msg->from,2,req_msg->Type,req_msg->SortID,0);
			asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
			return;
		}
		//������Ҫ��GO�㣬�շѱ�׼ 8GO��/Сʱ
		__int64 iTimes = proactivity.starttime_ - IceUtil::Time::now().toSeconds();
		iNeedGO = (int)(iTimes)/450;
		if(iNeedGO <=0)
			iNeedGO = 1;
	}
	else
	{
		return;
	}
	notify_msg.serialize(req_msg->agent,req_msg->from,req_msg->Type,req_msg->MapID,req_msg->CityID,
		req_msg->ItemID,req_msg->SortID,req_msg->Level,iNeedGO,req_msg->ProductionType);
	if(event_notify(MDM_GP_MAP_CHARGE,(char*)&notify_msg,notify_msg.length) ==0)
	{//֪ͨʧ��
		rep_msg.serialize(req_msg->agent,req_msg->MapID,req_msg->from,111,req_msg->Type,req_msg->SortID,0);
		asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
	}
}
//�����շ�ϵͳ���ص�֪ͨ��Ϣ
void CBuildProduce::OnImmedCompleteNotifyMsg(LPNotify_ImmedComplete_Response msg)
{
	ImmedComplete_Response rep_msg;
	int iRet  = 1;
	if(msg->Error_Code == 0)
	{//�۷ѳɹ�
		if(msg->Param.Type == Construction_Speed_Card)
		{//����ConTimeEvent(void * pDataBuffer, int iDataSize)
			ConstructionBuild consbuild;
			consbuild.setmasterid(msg->Param.MapID,msg->Param.CityID,msg->from);
			if(msg->Param.Level <= 1)
			{//msg->SortID = sortid			
				consbuild.setdetailid(msg->Param.ItemID,msg->Param.Level,msg->Param.SortID);
				if(g_Proxy.db_select(consbuild) == DB_Proxy::DB_SUCCEED)
				{
					iRet = 0;
				}
			}
			else
			{//msg->SortID = action
				ConstructionBuild *pRetSet  = 0;
				char *buf = 0;
				int count = 0;
				if(g_Proxy.db_multi_select(consbuild,buf,count) == DB_Proxy::DB_SUCCEED)
				{
					count = count/sizeof(ConstructionBuild);
					pRetSet = (ConstructionBuild*)buf;
					for(int i=0;i<count;i++)
					{
						if(pRetSet[i].action_ == msg->Param.SortID 
							&& pRetSet[i].conid_ == msg->Param.ItemID 
							&& pRetSet[i].level_ == msg->Param.Level)
						{
							iRet = 0;
							memcpy(&consbuild,&pRetSet[i],sizeof(ConstructionBuild));
							break;
						}
					}

					if(buf)
					{
						delete[] buf;
						buf = 0;
					}
				}
			}
			if(iRet)
			{//������
				rep_msg.serialize(msg->agent,msg->Param.MapID,msg->from,111,msg->Param.Type,msg->Param.SortID,0);
				asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
				//LOG_TRACE_EX(("���%d���������ڽ���Ľ���\n",msg->from));
				return;
			}
			BUILD_CONSTRUCTION_NODE cons_complete;
			cons_complete.serialize(msg->Param.CityID,msg->from,consbuild.action_,consbuild.sortid_,msg->Param.ItemID,
				msg->Param.Level,msg->Param.Level,0, EN_TIMER_CONSTRUCTION);
			ConTimeEvent(&cons_complete,cons_complete.length);
		}
		else if(msg->Param.Type == Produce_Speed_Card)
		{//����OnProduceTimeEvent(void * pDataBuffer, int iDataSize)PRODUCE_ITEM_NODE
			/*unsigned long							lCityID;					//����ID
			unsigned long							lPlayerID;
			int										iItemID;					//����ID
			int										iCount;						//����
			int										iType;						//
			unsigned long							lSortID;
			*/
			ProductionActivity proactivity;
			proactivity.setmasterid(msg->Param.MapID,msg->Param.ProductionType,msg->Param.CityID,msg->from);
			proactivity.setdetailid(msg->Param.ItemID,msg->Param.Level,msg->Param.SortID);
			if(g_Proxy.db_select(proactivity) != DB_Proxy::DB_SUCCEED)
			{
				rep_msg.serialize(msg->agent,msg->Param.MapID,msg->from,111,msg->Param.Type,msg->Param.SortID,0);
				asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
				return;
			}
			PRODUCE_ITEM_NODE produce_item;
			produce_item.serialize(msg->Param.CityID,msg->from,msg->Param.ItemID,msg->Param.ProductionType,msg->Param.SortID,proactivity.count_,0);
			OnProduceTimeEvent(&produce_item,produce_item.length);
		}
		rep_msg.serialize(msg->agent,msg->Param.MapID,msg->from,msg->Error_Code,msg->Param.Type,msg->Param.SortID,msg->Param.GOCount);
	}
	else
	{//ʧ��
		rep_msg.serialize(msg->agent,msg->Param.MapID,msg->from,msg->Error_Code,msg->Param.Type,msg->Param.SortID,msg->Param.GOCount);
	}
	asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
}

//����ͼ��ռ�죬����ÿ��������������
void CBuildProduce::OnResetMapInfo(LPResetMapInfo_Request msg)
{
	PlayerCity citytb;
	PlayerCity *pCityList = 0;
	char *citybuf = 0;
	int citycount = 0;
	int i =0;
	if(g_Proxy.db_select_all(citytb,citybuf,citycount) != DB_Proxy::DB_SUCCEED)
		return;
	citycount = citycount/sizeof(PlayerCity);
	pCityList = (PlayerCity*)citybuf;

	
	for(i = 0;i<citycount;i++)
	{
		if(pCityList[i].mapid_ != msg->to)
			continue;

		ClearPlayerCity(pCityList[i]);
	}
	if(citybuf)
	{
		delete[] citybuf;
		citybuf = 0;
	}
}
//�����Ҿݵ�
void CBuildProduce::ClearPlayerCity(PlayerCity & playercity)
{
	char *tmpbuf = 0;
	int tmpcount = 0;
	int j =0;
	ConstructionBuild consbuildtb;
	ConstructionBuild *pConsBuild = 0;
	PlayerConstruction playerconstb;
	PlayerConstruction *pPlayerCons = 0;

	PlayerSoldierRace soldiertb;
	PlayerResource resourcetb;

	//�������������
	ClearPlayerProduceActivity(playercity.playerid_,playercity.mapid_,playercity.cityid_);

	//������ڽ����	
	consbuildtb.setmasterid(playercity.mapid_,playercity.cityid_,playercity.playerid_);
	if(g_Proxy.db_multi_select(consbuildtb,tmpbuf,tmpcount) == DB_Proxy::DB_SUCCEED)
	{
		tmpcount = tmpcount/sizeof(ConstructionBuild);
		pConsBuild = (ConstructionBuild*)tmpbuf;
		for(j = 0;j<tmpcount;j++)
		{
			g_Proxy.db_delete(pConsBuild[j]);
		}
		if(tmpbuf)
		{
			delete[] tmpbuf;
			tmpbuf  = 0;
		}
	}
	//�������
	playerconstb.setmasterid(playercity.mapid_,playercity.cityid_,playercity.playerid_);
	if(g_Proxy.db_multi_select(playerconstb,tmpbuf,tmpcount) == DB_Proxy::DB_SUCCEED)
	{
		tmpcount = tmpcount/sizeof(PlayerConstruction);
		pPlayerCons = (PlayerConstruction*)tmpbuf;
		for(j = 0;j<tmpcount;j++)
		{
			g_Proxy.db_delete(pPlayerCons[j]);
		}
		if(tmpbuf)
		{
			delete[] tmpbuf;
			tmpbuf = 0;
		}
	}
	//�������������
	ClearPlayerProduceAmount(playercity.playerid_,playercity.mapid_,playercity.cityid_);
	//���ʿ��
	soldiertb.setmasterid(playercity.mapid_,playercity.cityid_);
	soldiertb.setdetailid(playercity.playerid_);
	g_Proxy.db_delete(soldiertb);
	//����ֿ⡢��Դ
	resourcetb.setmasterid(playercity.cityid_,playercity.mapid_);
	resourcetb.setdetailid(playercity.playerid_);
	g_Proxy.db_delete(resourcetb);
	//�������
	g_Proxy.db_delete(playercity);
	//�������
	g_SpeedCard.DeletePlayer(playercity.playerid_);
	//20100409
	PlayerOfficerTable officer;
	officer.setmasterid(playercity.playerid_);
	officer.setdetailid(playercity.playerid_);
	g_Proxy.db_select(officer);
	char *city_buf = 0;
	int city_count = 0;
	g_Proxy.db_multi_select(playercity,city_buf,city_count);
	city_count = city_count/sizeof(PlayerCity);
	if (city_buf)
	{
		delete[] city_buf;
		city_buf = 0;
	}
	ISystemLog::getInstance()->playerCitys_MapClear_Log(officer.playerid_,officer.officername_,
		city_count+1,city_count,playercity.mapid_,playercity.cityid_);
}
//�������������������е�����
void CBuildProduce::ClearPlayerProduceActivity(int playerid,int mapid,int cityid)
{
	ProductionActivity proActivity;
	ProductionActivity *pProActivityList = 0;
	char *buf = 0;
	int count = 0;

	for(int i = WEAPON_ITEM;i<SPECIAL_TOOL_ITEM;i++)
	{
		proActivity.setmasterid(mapid,i,cityid,playerid);
		if(g_Proxy.db_multi_select(proActivity,buf,count) != DB_Proxy::DB_SUCCEED)
			continue;
		count = count/sizeof(ProductionActivity);
		pProActivityList = (ProductionActivity*)buf;
		for(int j=0;j<count;j++)
		{
			g_Proxy.db_delete(pProActivityList[j]);
		}
		if(buf)
		{
			delete[] buf;
			buf = 0;
		}
	}
}
//��������������������������������
void CBuildProduce::ClearPlayerProduceAmount(int playerid,int mapid,int cityid)
{
	ProductionAmount proAmount;
	ProductionAmount *pProAmount = 0;
	char *buf = 0;
	int count = 0;
	for(int i = WEAPON_ITEM;i<SPECIAL_TOOL_ITEM;i++)
	{
		proAmount.setmasterid(mapid,cityid,playerid,i);
		if(g_Proxy.db_multi_select(proAmount,buf,count) != DB_Proxy::DB_SUCCEED)
			continue;
		count = count/sizeof(ProductionAmount);
		pProAmount = (ProductionAmount*)buf;
		for(int j=0;j<count;j++)
		{
			g_Proxy.db_delete(pProAmount[j]);
		}
		if(buf)
		{
			delete[] buf;
			buf = 0;
		}
	}
}
void CBuildProduce::OnUpgrateBuild(const char *msg)
{
	LPNotify_UpgradeBuild_Request req_msg = (LPNotify_UpgradeBuild_Request)msg;
	Notify_UpgradeBuild_Response rep_msg;

	MapInfoTable Map_Info;
	Map_Info.setid(req_msg->MapID);
	if (g_Proxy.db_select(Map_Info) != DB_Proxy::DB_SUCCEED)
	{
		rep_msg.serialize(req_msg->agent,req_msg->from,111,req_msg->CountryID,req_msg->MapID,req_msg->BuildID,req_msg->Money,0);
		event_notify(MDM_GP_MAP_CONSORTIA,(char*)&rep_msg,rep_msg.length);
		return;
	}
	
	if (Map_Info.countryid_ != req_msg->CountryID)
	{
		rep_msg.serialize(req_msg->agent,req_msg->from,111,req_msg->CountryID,req_msg->MapID,req_msg->BuildID,req_msg->Money,0);
		event_notify(MDM_GP_MAP_CONSORTIA,(char*)&rep_msg,rep_msg.length);
		return ;
	}
	int iOutPut = 0,iCurMoney = 0, iType = 0, iCurLevel = 0;
	PublicBuildTable publicbuild;
	publicbuild.setmasterid(req_msg->MapID);
	publicbuild.setdetailid(req_msg->BuildID);
	if (g_Proxy.db_select(publicbuild) != DB_Proxy::DB_SUCCEED)
	{
		rep_msg.serialize(req_msg->agent,req_msg->from,111,req_msg->CountryID,req_msg->MapID,req_msg->BuildID,req_msg->Money,0);
		event_notify(MDM_GP_MAP_CONSORTIA,(char*)&rep_msg,rep_msg.length);
		return;
	}

	iType = publicbuild.pbuildingkind_;
	iCurLevel = publicbuild.currentlevel_;
	/*
	PublicBuildParamTable * pBuildParam = g_LoadDBinfo.GetPublicBuildParam(iType, iCurLevel);
	if (pBuildParam == NULL) 
	{
		rep_msg.serialize(req_msg->agent,req_msg->from,111,req_msg->CountryID,req_msg->MapID,req_msg->BuildID,req_msg->Money);
		event_notify(MDM_GP_MAP_CONSORTIA,(char*)&rep_msg,rep_msg.length);
		return ;
	}
	iOutPut1 = pBuildParam->output_;
	*/
	iCurMoney = publicbuild.currentmoney_ + req_msg->Money;
	PublicBuildParamTable *pBuildParam = 0;
	while(1)
	{
		pBuildParam = g_LoadDBinfo.GetPublicBuildParam(iType, iCurLevel + 1);
		if(pBuildParam == NULL)
			break;
		if(pBuildParam->upgradefee_ > iCurMoney)
			break;
		iCurLevel++;
		iCurMoney -= pBuildParam->upgradefee_;
		iOutPut = pBuildParam->output_;
	}
	if(pBuildParam == NULL && iCurLevel == publicbuild.currentlevel_)
	{
		rep_msg.serialize(req_msg->agent,req_msg->from,111,req_msg->CountryID,req_msg->MapID,req_msg->BuildID,req_msg->Money,0);
		event_notify(MDM_GP_MAP_CONSORTIA,(char*)&rep_msg,rep_msg.length);
		return;
	}

	publicbuild.currentlevel_ = iCurLevel;
	publicbuild.currentmoney_ = iCurMoney;
	if (g_Proxy.db_update(publicbuild, publicbuild) != DB_Proxy::DB_SUCCEED)
	{
		rep_msg.serialize(req_msg->agent,req_msg->from,111,req_msg->CountryID,req_msg->MapID,req_msg->BuildID,req_msg->Money,0);
		event_notify(MDM_GP_MAP_CONSORTIA,(char*)&rep_msg,rep_msg.length);
		return;
	}

	if (iCurLevel > publicbuild.currentlevel_)
	{/*
	 ARCH_LUMBERYARD=6,     //ľ��3
	 ARCH_STOPE     =7,     //�ɿ�1
	 ARCH_GRINDERY  =8,     //ĥ��0
	 ARCH_SKIN      =9,     //Ƥ�ϼӹ���2
	 */
		switch(iType)
		{
			break;
		case ARCH_GRINDERY:
			m_UserInfo.SetOutPut(0, iOutPut);
			break;
		case ARCH_STOPE:
			m_UserInfo.SetOutPut(1, iOutPut);
			break;
		case ARCH_SKIN:
			m_UserInfo.SetOutPut(2, iOutPut);
			break;
		case ARCH_LUMBERYARD:
			m_UserInfo.SetOutPut(3, iOutPut);
			break;
		};
		UpdatePublicBuildLevel(iType,req_msg->BuildID, iCurLevel);
	}
	rep_msg.serialize(req_msg->agent,req_msg->from,0,req_msg->CountryID,req_msg->MapID,req_msg->BuildID,req_msg->Money,iCurLevel);
	event_notify(MDM_GP_MAP_CONSORTIA,(char*)&rep_msg,rep_msg.length);
}
//add by xujincai 20090516//����ݵ�lMapID
void CBuildProduce::OnDestroyCity(const char *msg)
{
	LPDestroyCity_Request req_msg = (LPDestroyCity_Request)msg;
	DestroyCity_Response rep_msg;
	//�ж��Ƿ��оݵ�
	PlayerCity playercity;
	playercity.setmasterid(req_msg->from);
	playercity.setdetailid(m_lMapID);
	int iRet = g_Proxy.db_select(playercity);
	if(iRet == DB_Proxy::DB_EOF)
	{
		iRet = 1;
	}
	else if(iRet == DB_Proxy::DB_FAILED)
	{
		iRet = 111;
	}
	else
	{
		iRet = 0;
	}
	if(playercity.state_ == 100)
	{
		rep_msg.serialize(req_msg->agent,req_msg->from,m_lMapID,2);
		asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
	}
	else
	{
		rep_msg.serialize(req_msg->agent,req_msg->from,m_lMapID,iRet);
		asynch_write(rep_msg.agent,(char*)&rep_msg,rep_msg.length);
		ClearPlayerCity(playercity);
	}
	//20100409
	PlayerOfficerTable officer;
	officer.setmasterid(req_msg->from);
	officer.setdetailid(req_msg->from);
	g_Proxy.db_select(officer);
	char *city_buf =0;
	int citycount = 0;
	g_Proxy.db_multi_select(playercity,city_buf,citycount);
	citycount = citycount/sizeof(PlayerCity);
	if (city_buf)
	{
		delete[] city_buf;
		city_buf = 0;
	}
	ISystemLog::getInstance()->playerCitys_Destroy_Log(officer.playerid_,officer.officername_,
		citycount+1,citycount,playercity.mapid_,playercity.cityid_);
}
int CBuildProduce::GetGarrisonCount()
{
	int iGarrisonCount = 0;
	ArmyTable armytb;
	ArmyTable *pArmyList = 0;
	char *buf = 0;
	int count = 0;
	armytb.setmasterid(m_lMapID);
	if(g_Proxy.db_multi_select(armytb,buf,count) != DB_Proxy::DB_SUCCEED)
		return 0;
	count = count/sizeof(ArmyTable);
	pArmyList = (ArmyTable *)buf;
	for(int i =0;i<count;i++)
	{
		if(pArmyList[i].playerid_ !=1 && pArmyList[i].playerid_ !=2)
			continue;
		iGarrisonCount++;
	}
	if(buf)
	{
		delete[] buf;
		buf = 0;
	}
	return iGarrisonCount;
}
void CBuildProduce::InitProActivity(DB_Proxy *pDBProxy)
{
	ProductionActivity proActivity;
	char *buf = 0;
	int count = 0;
	ProductionActivity *pProActivityList = 0;
	if(pDBProxy->db_select_all(proActivity,buf,count) != DB_Proxy::DB_SUCCEED)
		return;
	count = count/sizeof(ProductionActivity);
	pProActivityList = (ProductionActivity*)buf;
	int iTimes = 0;
	PRODUCE_ITEM_NODE * pProduceNode =0;
	for(int i=0;i<count;i++)
	{
		if(pProActivityList[i].mapid_ != m_lMapID)
			continue;
		if(pProActivityList[i].starttime_ <=0)
			continue;
		iTimes = (int)(pProActivityList[i].starttime_ - IceUtil::Time::now().toSeconds());
		if(iTimes <=0)
		{
			iTimes = 5;
			pProActivityList[i].starttime_ = IceUtil::Time::now().toSeconds()+5;
			g_Proxy.db_update(pProActivityList[i],pProActivityList[i]);
		}
		pProduceNode = new PRODUCE_ITEM_NODE;
		pProduceNode->serialize(pProActivityList[i].cityid_, pProActivityList[i].playerid_, pProActivityList[i].productionid_, pProActivityList[i].type_, pProActivityList[i].sortid_, pProActivityList[i].count_, EN_TIMER_PRODUCE_WEAPON);
		if(set_timer(iTimes, (const void *)pProduceNode) <=0)
		{
			LOG_TRACE_EX(false,("���ö�ʱ��ʧ��-->���%d�ھ�%d-%d��ǰ��������:���ͣ�%d,SortID:%d,ItemID:%d,Count:%d\n",pProActivityList[i].playerid_,pProActivityList[i].mapid_,pProActivityList[i].cityid_,pProActivityList[i].type_,
			pProActivityList[i].sortid_,pProActivityList[i].productionid_,pProActivityList[i].count_));
		}
	}
	if(buf)
	{
		delete[] buf;
		buf = 0;
	}
}
//�ָ�����
void CBuildProduce::InitConsBuildActivity(DB_Proxy *pDBProxy)
{
	ConstructionBuild consbuild;
	ConstructionBuild *pConsBuild = 0;
	char *buf = 0;
	int count = 0;
	if(pDBProxy->db_select_all(consbuild,buf,count) != DB_Proxy::DB_SUCCEED)
		return;
	count = count/sizeof(ConstructionBuild);
	pConsBuild = (ConstructionBuild*)buf;
	int i = 0;
	int iTimes = 0;
	BUILD_CONSTRUCTION_NODE *pBuildTimer = 0;
	for(i =0;i<count;i++)
	{
		if(pConsBuild[i].mapid_ != m_lMapID)
			continue;
		if(pConsBuild[i].starttime_ <=0)
			continue;
		iTimes = (int)(pConsBuild[i].starttime_ - IceUtil::Time::now().toSeconds());
		if(iTimes <=0)
		{
			iTimes = 5;
			pConsBuild[i].starttime_ = IceUtil::Time::now().toSeconds() + 5;
			pDBProxy->db_update(pConsBuild[i],pConsBuild[i]);
		}
		pBuildTimer = new BUILD_CONSTRUCTION_NODE;
		pBuildTimer->serialize(pConsBuild[i].cityid_, pConsBuild[i].playerid_,pConsBuild[i].action_,pConsBuild[i].sortid_,pConsBuild[i].conid_, pConsBuild[i].level_, pConsBuild[i].level_, 0,EN_TIMER_CONSTRUCTION);
		pBuildTimer->TimerID = set_timer(iTimes, (const void *)pBuildTimer);
	}
	if(buf)
	{
		delete[] buf;
		buf = 0;
	}
}
int CBuildProduce::UpdatePublicBuildLevel(int buildtype,unsigned long buildid ,int level)
{
	g_LoadDBinfo.UpdatePublicBuildLevel(buildid,level,buildtype);
	/*del by xujincai 20090926Ӧѹ��Ϣ��ս���߳�
	PublicBuildTable * pBuild = g_LoadDBinfo.GetPublicBuildInfo(buildid);
	PublicBuildParamTable* param;
	if (pBuild!= NULL)
	{
	CArmy_ptr Army = g_ArmyManager->GetArmy(buildid%10000);
	int type = buildid%10000;
	type/=100;
	if (Army!=0)
	{
	param = g_LoadDBinfo.GetPublicBuildParam(type,pBuild->currentlevel_);
	if (param)
	{
	Army->SetTotalHp(1,param->hp_);
	Army->SetIniHp(param->hp_);
	Army->SetAttackPower(param->attack_);
	Army->SetArmyVisualRange(param->visualrange_);
	Army->m_Group.SetAttackScop(param->attackscop_);
	}			
	}
	}
	*/
	LPModify_PublicBuild_Request pModifyBuildLevel = new Modify_PublicBuild_Request;
	pModifyBuildLevel->serialize(0,0,buildtype,buildid,level);
	g_IFightProcesser->pushDataToManager((char*)pModifyBuildLevel);
	return 1;
}
