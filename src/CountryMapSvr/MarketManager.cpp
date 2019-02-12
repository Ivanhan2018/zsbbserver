#include "MarketManager.h"
#include "commInterface.h"
#include "ConsortiaProtocol.h"
#include "log.hpp"
#include "equipment.h"
#include "LoadDBInfo.h"
#include "DataServer.h"

#pragma warning(disable:4100)
#pragma warning(disable:4189)
#pragma warning(disable:4127)
#pragma warning(disable:4244)
#pragma warning(disable:4389)
CMarketManager::CMarketManager()
{
	m_mapPlayerInfo.clear();
}
CMarketManager::~CMarketManager()
{
}
//���紦����
int CMarketManager::MainMarketManager(void * pDataBuffer, int iDataSize) 
{
	return true;
}
//�����г���Ʒ�б�
int CMarketManager::CountryMarket(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID) 
{
	if (iDataSize != sizeof(CITY_INFO))
	{
		LOG_TRACE_EX(false,("CMarketManager::CountryMarket-->��Ϣ���Ƿ������%d\n",lPlayerID));
		return false;
	}
	CITY_INFO * pCity = (CITY_INFO *)pDataBuffer;

	m_UserInfo.OnWeaponList(handle, pCity->lMapID, pCity->lCityID,MDM_GP_MAP_MARKET,lPlayerID);
	m_UserInfo.OnDefendList(handle, pCity->lMapID, pCity->lCityID,MDM_GP_MAP_MARKET,lPlayerID);

	tagPlayerInfo PlayerInfo = m_mapPlayerInfo[lPlayerID];
	if (PlayerInfo.iCountry <= 0 || PlayerInfo.szAccounts[0] == '\0')
	{
		PlayerOfficerTable Player;
		Player.setmasterid(lPlayerID);
		Player.setdetailid(lPlayerID);
		if (g_Proxy.db_select(Player) == DB_Proxy::DB_SUCCEED)
		{
			PlayerInfo.iCountry = 2;
			if (Player.officerrace_ < 5)
				PlayerInfo.iCountry = 1;
			strcpy(PlayerInfo.szAccounts, Player.officername_);
			m_mapPlayerInfo[lPlayerID] = PlayerInfo;
		}
	}
	return true;
}
//�����г���Ʒ����
int CMarketManager::CountryMarketBusiness(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID) 
{
	if (iDataSize != sizeof(COUNTRY_MARKET_BUSINESS))
	{
		LOG_TRACE_EX(false,("CMarketManager::CountryMarketBusiness-->��Ϣ���Ƿ������%d\n",lPlayerID));
		return 0;
	}

	COUNTRY_MARKET_BUSINESS *pMarket = (COUNTRY_MARKET_BUSINESS *)pDataBuffer;
	if (pMarket->iType == WEAPON_ITEM) //����
	{
		WeaponAttribute * pWeapon = g_LoadDBinfo.GetWeaponAttribute(pMarket->iSaleID);
		if (pWeapon == NULL)
		{
			LOG_TRACE_EX(false,("CMarketManager::CountryMarketBusiness-->�����г������Ƿ������%d,����%d������\n",lPlayerID,pMarket->iSaleID));
			return 0;
		}
		m_UserInfo.OnUpDataItem(handle,pMarket->lMapID, pMarket->lCityID, WEAPON_ITEM, pMarket->iSaleID,pMarket->iLevel, pMarket->iCount, MDM_GP_MAP_MARKET,lPlayerID, pWeapon->cost, pMarket->iMode);
	}
	else if (pMarket->iType == DEFEND_ITEM) //����
	{
		DefendAttribute * pDefend = g_LoadDBinfo.GetDefendAttribute(pMarket->iSaleID);
		if (pDefend == NULL)
		{
			LOG_TRACE_EX(false,("CMarketManager::CountryMarketBusiness-->�����г������Ƿ������%d,����%d������\n",lPlayerID,pMarket->iSaleID));
			return 0;
		}
		m_UserInfo.OnUpDataItem(handle,pMarket->lMapID, pMarket->lCityID, DEFEND_ITEM, pMarket->iSaleID,pMarket->iLevel, pMarket->iCount, MDM_GP_MAP_MARKET,lPlayerID, pDefend->cost, pMarket->iMode);
	}
	else if (pMarket->iType == ANIMALS_TAME_ITEM) //ѱ�޼���
	{
		WeaponAttribute * pWeapon = g_LoadDBinfo.GetWeaponAttribute(pMarket->iSaleID);
		if (pWeapon == NULL)
		{
			LOG_TRACE_EX(false,("CMarketManager::CountryMarketBusiness-->�����г������Ƿ������%d,ѱ��%d������\n",lPlayerID,pMarket->iSaleID));
			return 0;
		}
		m_UserInfo.OnUpDataItem(handle,pMarket->lMapID, pMarket->lCityID, ANIMALS_TAME_ITEM, pMarket->iSaleID,pMarket->iLevel, pMarket->iCount,MDM_GP_MAP_MARKET, lPlayerID, pWeapon->cost, pMarket->iMode);
	}
	else if (pMarket->iType == SPECIAL_TOOL_ITEM) //���⹤��
	{
		WeaponAttribute * pWeapon = g_LoadDBinfo.GetWeaponAttribute(pMarket->iSaleID);
		if (pWeapon == NULL)
		{
			LOG_TRACE_EX(false,("CMarketManager::CountryMarketBusiness-->�����г������Ƿ������%d,���⹤��%d������\n",lPlayerID,pMarket->iSaleID));
			return 0;
		}
		m_UserInfo.OnUpDataItem(handle,pMarket->lMapID, pMarket->lCityID, SPECIAL_TOOL_ITEM, pMarket->iSaleID,pMarket->iLevel, pMarket->iCount,MDM_GP_MAP_MARKET, lPlayerID, pWeapon->cost, pMarket->iMode);
	}
	else if (pMarket->iType == EN_HERO_EQUIP) //ָ�ӹٵ���
	{
		SaleEquip(handle,lPlayerID,pMarket->iSaleID);
	}
	else if (pMarket->iType == EN_BASIC_RESOURCE)		//��Դ
	{
		int iCount = pMarket->iCount;
		int iMoney = 0;
		//
		if (pMarket->iMode == 1)// 1:��
		{
			iCount = -pMarket->iCount;
		}
		else
		{
			pMarket->iMode = 2;
		}
		iMoney = -iCount * m_MarkPrice[pMarket->iMode][pMarket->iSaleID];

		PlayerResource PlayerRes;
		memset(&PlayerRes, 0, sizeof(PlayerResource));
		switch(pMarket->iSaleID)
		{
		case 1:
			PlayerRes.foodcount_ = iCount;
			break;
		case 2:
			PlayerRes.woodcount_ = iCount;
			break;
		case 3:
			PlayerRes.ironcount_ = iCount;
			break;
		case 4:
			PlayerRes.skincount_ = iCount;
			break;
		case 5:
			PlayerRes.yollowstonecount_ = iCount;
			break;
		case 6:
			PlayerRes.redstonecount_ = iCount;
			break;
		case 7:
			PlayerRes.greenstonecount_ = iCount;
			break;
		case 8:
			PlayerRes.backironcount_ = iCount;
			break;
		};
		PlayerChatRoomTable playeMoney;
		playeMoney.setid(lPlayerID);
		g_Proxy.db_select(playeMoney);
		int ret = m_UserInfo.OnUpDataResource(handle, PlayerRes, pMarket->lMapID, pMarket->lCityID, lPlayerID,
			MDM_GP_MAP_MARKET, lPlayerID, pMarket->iMode,pMarket->iSaleID,pMarket->iCount,iMoney);
		if (ret != 1) return false;
		MARKET_INFO MarketInfo;
		PlayerOfficerTable officer;
		officer.setmasterid(lPlayerID);
		officer.setdetailid(lPlayerID);
		g_Proxy.db_select(officer);
		int oldMoney = playeMoney.money_;
		if (pMarket->iMode == 1)// 1:��
		{
			MarketInfo.serialize(6, pMarket->iSaleID, pMarket->iCount, EN_GOODS_SALE_SUCCEED, MDM_GP_MAP_MARKET, lPlayerID);
			ret = asynch_write(handle, (char *)&MarketInfo, MarketInfo.length);			

			playeMoney.setid(lPlayerID);
			g_Proxy.db_select(playeMoney);
			ISystemLog::getInstance()->playerMoney_SaleResCountry_Log(officer.playerid_,officer.officername_,
				oldMoney,playeMoney.money_,iMoney,pMarket->lMapID,pMarket->lCityID,
				pMarket->iSaleID,pMarket->iCount);
			return false;
		}
		else if (pMarket->iMode == 2)
		{
			MarketInfo.serialize(6, pMarket->iSaleID, pMarket->iCount, EN_GOODS_BUY_SUCCEED, MDM_GP_MAP_MARKET, lPlayerID);
			ret = asynch_write(handle, (char *)&MarketInfo, MarketInfo.length);

			playeMoney.setid(lPlayerID);
			g_Proxy.db_select(playeMoney);
			ISystemLog::getInstance()->playerMoney_BuyResCountry_Log(officer.playerid_,officer.officername_,
				oldMoney,playeMoney.money_,-iMoney,pMarket->lMapID,pMarket->lCityID,
				pMarket->iSaleID,pMarket->iCount);
			return false;
		}
	}
	return true;
}
//�����г���Ӣ��װ��
int CMarketManager::SaleEquip(int agent,int playerid,int sortid)
{
	PlayerItemStorageTable equipitem;
	equipitem.setmasterid(1,playerid);
	equipitem.setdetailid(sortid);
	MARKET_INFO rep_msg;
	//EN_GOODS_SALE_FAILED/EN_GOODS_SALE_SUCCEED
	if(g_Proxy.db_select(equipitem) != DB_Proxy::DB_SUCCEED)
	{
		rep_msg.serialize(EN_HERO_EQUIP,sortid,1,EN_GOODS_SALE_FAILED,MDM_GP_MAP_MARKET,playerid);
		asynch_write(agent,(char*)&rep_msg,rep_msg.length);
		return 1;
	}
	//ֻ��װ������Ƕ��ʯ������
	if (equipitem.pos_ <=0 ||(equipitem.pos_>10 && equipitem.pos_ != INLAYRESOURCE))
	{
		rep_msg.serialize(EN_HERO_EQUIP,sortid,1,EN_GOODS_SALE_FAILED,MDM_GP_MAP_MARKET,playerid);
		asynch_write(agent,(char*)&rep_msg,rep_msg.length);
		return 1;
	}
	IEquipment iEquipment(&g_LoadDBinfo,equipitem.itemid_);
	/*
	if (!iEquipment.isCanSale())
	{
		rep_msg.serialize(EN_HERO_EQUIP,sortid,1,EN_GOODS_SALE_FAILED,MDM_GP_MAP_MARKET,playerid);
		asynch_write(agent,(char*)&rep_msg,rep_msg.length);
		return 2;
	}
	*/
	//ɾ��װ��
	if (equipitem.count_ <=1)
	{
		if(g_Proxy.db_delete(equipitem) != DB_Proxy::DB_SUCCEED)
		{
			rep_msg.serialize(EN_HERO_EQUIP,sortid,1,EN_GOODS_SALE_FAILED,MDM_GP_MAP_MARKET,playerid);
			asynch_write(agent,(char*)&rep_msg,rep_msg.length);
			LOG_TRACE_EX(false,("CMarketManager::SaleEquip-->ɾ��װ��ʧ�ܣ����%d,SORTID:%d\n",playerid,sortid));
			return 3;
		}
	}
	else
	{
		memset(&equipitem,0,sizeof(equipitem));
		equipitem.setmasterid(1,playerid);
		equipitem.setdetailid(sortid);
		equipitem.count_ = -1;
		if(g_Proxy.db_increase(equipitem) != DB_Proxy::DB_SUCCEED)
		{
			rep_msg.serialize(EN_HERO_EQUIP,sortid,1,EN_GOODS_SALE_FAILED,MDM_GP_MAP_MARKET,playerid);
			asynch_write(agent,(char*)&rep_msg,rep_msg.length);
			LOG_TRACE_EX(false,("CMarketManager::SaleEquip-->ɾ��װ��ʧ�ܣ����%d,SORTID:%d\n",playerid,sortid));
			return 3;
		}
	}
	//���ӽ�Ǯ
	PlayerChatRoomTable playertb;
	playertb.setid(playerid);
	playertb.money_ = iEquipment.getPrice();
	if(g_Proxy.db_increase(playertb) != DB_Proxy::DB_SUCCEED)
	{
	}
	rep_msg.serialize(EN_HERO_EQUIP,sortid,1,EN_GOODS_SALE_SUCCEED,MDM_GP_MAP_MARKET,playerid);
	asynch_write(agent,(char*)&rep_msg,rep_msg.length);
	//20100412
	PlayerOfficerTable officer;
	officer.setmasterid(playerid);
	officer.setdetailid(playerid);
	g_Proxy.db_select(officer);
	ISystemLog::getInstance()->playerEquip_SaleCountry_Log(officer.playerid_,officer.officername_,
		equipitem.pos_,equipitem.itemid_,1);
	ISystemLog::getInstance()->playerMoney_SaleHeroEquipCountry_Log(officer.playerid_,officer.officername_,
		playertb.money_ - iEquipment.getPrice(),playertb.money_,iEquipment.getPrice(),
		0,0,equipitem.itemid_,1);
	return 0;
}
//����г���Ϣ
int CMarketManager::PeopleMarket(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID, int iNext)
{
	if (iDataSize != sizeof(PEOPLE_MARKET))
	{
		LOG_TRACE_EX(false,("CMarketManager::PeopleMarket-->��Ϣ���Ƿ������%d\n",lPlayerID));
		return 0;
	}
	map<unsigned long ,tagPlayerInfo>::iterator iter = m_mapPlayerInfo.find(lPlayerID);
	if (iter == m_mapPlayerInfo.end())
	{
		tagPlayerInfo PlayerInfo;
		PlayerOfficerTable Player;
		Player.setmasterid(lPlayerID);
		Player.setdetailid(lPlayerID);
		if (g_Proxy.db_select(Player) == DB_Proxy::DB_SUCCEED)
		{
			PlayerInfo.iCountry = 2;
			if (Player.officerrace_ < 5)
				PlayerInfo.iCountry = 1;
			strcpy(PlayerInfo.szAccounts, Player.officername_);
			m_mapPlayerInfo[lPlayerID] = PlayerInfo;
		}
		else
		{
			LOG_TRACE_EX(false,("CMarketManager::PeopleMarket->��ѯ���%d��Ӫʧ��\n",lPlayerID));
			return 0;
		}
	}

	PEOPLE_MARKET * pMarket = (PEOPLE_MARKET *)pDataBuffer;
	
	switch(pMarket->iType)
	{
	case WEAPON_ITEM:				//����
		OnGoodsList(handle, iNext, WEAPON_ITEM, lPlayerID);
		break;
	case DEFEND_ITEM:				//����
		OnGoodsList(handle, iNext, DEFEND_ITEM, lPlayerID);
		break;
	case ANIMALS_TAME_ITEM:				//Ұ�޲���
		OnGoodsList(handle, iNext, ANIMALS_TAME_ITEM, lPlayerID);
		break;
	case SPECIAL_TOOL_ITEM:				//���⹤��
		OnGoodsList(handle, iNext, SPECIAL_TOOL_ITEM, lPlayerID);
		break;
	case EN_HERO_EQUIP:				//Ӣ��װ��
		OnGoodsList(handle, iNext, EN_HERO_EQUIP, lPlayerID);
		break;
	case EN_BASIC_RESOURCE:				//��������
		OnGoodsList(handle, iNext, EN_BASIC_RESOURCE, lPlayerID);
		break;
	};
	return true;
}
//ս��Ʒ������Ϣ
int CMarketManager::VendueGoods(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID, int iNext)
{
	if (iDataSize != sizeof(PEOPLE_MARKET))
	{
		LOG_TRACE_EX(false,("CMarketManager::VendueGoods-->��Ϣ���Ƿ������%d\n",lPlayerID));
		return 0;
	}

	PEOPLE_MARKET * pMarket = (PEOPLE_MARKET *)pDataBuffer;
	OnGoodsList(handle, iNext, pMarket->iType, lPlayerID);
	/*
	switch(pMarket->iType)
	{
	case WEAPON_ITEM:				//����
		OnGoodsList(handle, iNext, WEAPON_ITEM, lPlayerID);
		break;
	case DEFEND_ITEM:				//����
		OnGoodsList(handle, iNext, DEFEND_ITEM, lPlayerID);
		break;
	case ANIMALS_TAME_ITEM:				//Ұ�޲���
		OnGoodsList(handle, iNext, ANIMALS_TAME_ITEM, lPlayerID);
		break;
	case SPECIAL_TOOL_ITEM:				//���⹤��
		OnGoodsList(handle, iNext, SPECIAL_TOOL_ITEM, lPlayerID);
		break;
	case EN_HERO_EQUIP:				//Ӣ��װ��
		OnGoodsList(handle, iNext, EN_HERO_EQUIP, lPlayerID);
		break;
	case EN_BASIC_RESOURCE:				//��������
		OnGoodsList(handle, iNext, EN_BASIC_RESOURCE, lPlayerID);
		break;
	};
	*/
	return true;
}
//�����Դ��
int CMarketManager::PlayerResourceCount(int handle, void * pDataBuffer, unsigned long lPlayerID)
{
	CITY_INFO * pMarket = (CITY_INFO *)pDataBuffer;
	unsigned long lMapID  = pMarket->lMapID;
	unsigned long lCityID = pMarket->lCityID;
	PlayerResource ResByPlayer;
	
	memset(&ResByPlayer, 0,sizeof(ResByPlayer));
	ResByPlayer.setmasterid(lCityID, lMapID);
	ResByPlayer.setdetailid(lPlayerID);
	if (g_Proxy.db_select(ResByPlayer) != DB_Proxy::DB_SUCCEED)
	{
		//return false;
	}
	PALEYER_RESOURCE_LIST PlayerRes;
	PlayerRes.serialize(ResByPlayer.foodcount_, ResByPlayer.woodcount_, ResByPlayer.ironcount_, ResByPlayer.skincount_,ResByPlayer.redstonecount_, 
		ResByPlayer.yollowstonecount_, ResByPlayer.greenstonecount_, ResByPlayer.backironcount_,ResByPlayer.content_, MDM_GP_MAP_MARKET, lPlayerID);
	int ret = asynch_write(handle, (char *)&PlayerRes, PlayerRes.length);
	return ret;
}
//�����б�
void CMarketManager::OnWeaponList(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID)
{

	CITY_INFO * pMarket = (CITY_INFO *)pDataBuffer;
	unsigned long lMapID=pMarket->lMapID, CityID=pMarket->lCityID;
	int count = 0,count1 = 0,count2 = 0,count3 = 0;
	int Item = 0, Item1 = 0, Item2 = 0, Item3 = 0;
	char *Buffer=0,*Buffer1=0,*Buffer2=0,*Buffer3=0;
	ProductionAmount Weapon,AnimalItem,SpecialItem,DefendItem;
	memset(&Weapon, 0, sizeof(ProductionAmount));
	memset(&AnimalItem, 0, sizeof(ProductionAmount));
	memset(&SpecialItem, 0, sizeof(ProductionAmount));
	memset(&DefendItem, 0, sizeof(ProductionAmount));

	Weapon.setmasterid(lMapID, CityID, lPlayerID,WEAPON_ITEM);
	int dbRet = g_Proxy.db_multi_select(Weapon, Buffer, count);
	if ( dbRet == DB_Proxy::DB_EOF) 
	{
	}
	else if (dbRet == DB_Proxy::DB_FAILED)
	{
		ERROR_INFO ErrorInfo;
		ErrorInfo.serialize(EN_DATABASE_CONNECT_FAILED, MDM_GP_MAP_MARKET, lPlayerID);
		asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
		LOG_TRACE_EX(false,("CMarketManager::OnWeaponList-->���%d\n",lPlayerID));
		return ;
	}
	Item = count/sizeof(ProductionAmount);
	ProductionAmount *pProAmount = (ProductionAmount *)Buffer;

	AnimalItem.setmasterid(lMapID, CityID, lPlayerID,ANIMALS_TAME_ITEM);
	if (g_Proxy.db_multi_select(AnimalItem, Buffer1, count1) != DB_Proxy::DB_SUCCEED) 
	{
	}
	SpecialItem.setmasterid(lMapID, CityID, lPlayerID,SPECIAL_TOOL_ITEM);
	if (g_Proxy.db_multi_select(SpecialItem, Buffer2, count2) != DB_Proxy::DB_SUCCEED) 
	{
	}
	DefendItem.setmasterid(lMapID, CityID, lPlayerID,DEFEND_ITEM);
	if (g_Proxy.db_multi_select(DefendItem, Buffer3, count3) != DB_Proxy::DB_SUCCEED) 
	{
	}
	Item1 = count1/sizeof(ProductionAmount);
	Item2 = count2/sizeof(ProductionAmount);
	Item3 = count3/sizeof(ProductionAmount);
	
	char * tmp = new char[sizeof(WEAPON_LIST) + sizeof(tagItem_List)*Item + sizeof(tagItem_List)*Item1 + sizeof(tagItem_List)*Item2 + sizeof(tagItem_List)*Item3];
	WEAPON_LIST *pWeaponList = (WEAPON_LIST *)tmp;

	int n = 0,i = 0;
	for (n = 0; n < Item; n++)
	{
		if (pProAmount[n].productionid_ < 1 || pProAmount[n].productionid_ >= 100 || pProAmount[n].productioncount_ <= 0) continue;
		(*pWeaponList)[i].lMapID = lMapID;
		(*pWeaponList)[i].lCityID = CityID;
		(*pWeaponList)[i].iItemID = pProAmount[n].productionid_;
		(*pWeaponList)[i].iType = WEAPON_ITEM;
		(*pWeaponList)[i].iCount = pProAmount[n].productioncount_;
		i++;
	}
	pProAmount = 0;
	pProAmount = (ProductionAmount *)Buffer1;
	
	for (n = 0; n < Item1; n++)
	{
		if (pProAmount[n].productionid_ < 100 || pProAmount[n].productionid_ >= 150 || pProAmount[n].productioncount_ <= 0) continue;
		(*pWeaponList)[i].lMapID = lMapID;
		(*pWeaponList)[i].lCityID = CityID;
		(*pWeaponList)[i].iItemID = pProAmount[n].productionid_;
		(*pWeaponList)[i].iType = ANIMALS_TAME_ITEM;
		(*pWeaponList)[i].iCount = pProAmount[n].productioncount_ ;

		i++;
	}
	pProAmount = 0;
	pProAmount = (ProductionAmount *)Buffer2;
	
	for (n = 0; n < Item2; n++)
	{
		if (pProAmount[n].productionid_ < 150 || pProAmount[n].productionid_ > 199 || pProAmount[n].productioncount_ <= 0) continue;
		(*pWeaponList)[i].lMapID = lMapID;
		(*pWeaponList)[i].lCityID = CityID;
		(*pWeaponList)[i].iItemID = pProAmount[n].productionid_;
		(*pWeaponList)[i].iType = SPECIAL_TOOL_ITEM;
		(*pWeaponList)[i].iCount = pProAmount[n].productioncount_ ;
		i++;
	}
	pProAmount = 0;
	pProAmount = (ProductionAmount *)Buffer3;
	for (n = 0; n < Item3; n++)
	{
		if (pProAmount[n].productionid_ < 200 || pProAmount[n].productionid_ > 250 || pProAmount[n].productioncount_ <= 0) continue;

		(*pWeaponList)[i].lMapID = lMapID;
		(*pWeaponList)[i].lCityID = CityID;
		(*pWeaponList)[i].iItemID = pProAmount[n].productionid_;
		(*pWeaponList)[i].iType = DEFEND_ITEM;
		(*pWeaponList)[i].iCount = pProAmount[n].productioncount_ ;
		i++;
	}
	pWeaponList->serialize(i, MDM_GP_MAP_MARKET, lPlayerID, EN_S_MARKET_WEAPON_LIST);
	int ret = asynch_write(handle, (char *)pWeaponList, pWeaponList->length);

	if (Buffer != NULL)
	{
		delete [] Buffer;
		Buffer = 0;
	}
	if (Buffer1 != NULL)
	{
		delete [] Buffer1;
		Buffer1 = 0;
	}
	if (Buffer2 != NULL)
	{
		delete [] Buffer2;
		Buffer2 = 0;
	}
	if (Buffer3 != NULL)
	{
		delete [] Buffer3;
		Buffer3 = 0;
	}
	if(tmp)
	{
		delete [] tmp;
		tmp = 0;
	}
}
//�����б�
void CMarketManager::OnDefendList(int handle,  void * pDataBuffer, int iDataSize, unsigned long lPlayerID)
{
	/*
	return;
	if (iDataSize != sizeof(PEOPLE_MARKET))
	{
		LOG_TRACE_EX(false,("CMarketManager::OnDefendList-->��Ϣ���Ƿ������%d\n",lPlayerID));
		return ;
	}

	PEOPLE_MARKET * pMarket = (PEOPLE_MARKET *)pDataBuffer;
	m_UserInfo.OnDefendList(handle,pMarket->lSMapID, pMarket->lSCityID, MDM_GP_MAP_MARKET, lPlayerID);
	*/
	
}
//Ӣ��װ��
void CMarketManager::OfficerItemList(int handle,  void * pDataBuffer, int iDataSize, unsigned long lPlayerID)
{
	//PEOPLE_MARKET * pMarket = (PEOPLE_MARKET *)pDataBuffer;
	m_UserInfo.OnGetHeroEquip(handle, MDM_GP_MAP_MARKET, lPlayerID);
}
//����Ӣ��װ��
int CMarketManager::OnUpDataEquip(unsigned long lPlayerID, __int64 iItemID, unsigned long lSortID)
{
	PlayerItemStorageTable HeroPack;
	if (lSortID > 0)
	{
		HeroPack.setmasterid(1, lPlayerID);
		HeroPack.setdetailid(lSortID);
		
		if(g_Proxy.db_select(HeroPack) != DB_Proxy::DB_SUCCEED)
			return 0;

		if (HeroPack.count_ <=1)
		{
			if(g_Proxy.db_delete(HeroPack) == DB_Proxy::DB_SUCCEED)
				return 1;
			else 
				return 2;
		}
		else
		{
			memset(&HeroPack,0,sizeof(HeroPack));
			HeroPack.setmasterid(1, lPlayerID);
			HeroPack.setdetailid(lSortID);
			HeroPack.count_ = -1;
			if (g_Proxy.db_increase(HeroPack)== DB_Proxy::DB_SUCCEED)
				return 1;
			else
				return 2;
		}
		
	}
	
	IEquipment iEquipment(&g_LoadDBinfo,iItemID);
	PackStoreParam *pPackStoreParam = g_LoadDBinfo.GetPackStoreParam(iEquipment.getPostion());
	bool bOverlap = false;
	if (pPackStoreParam)
	{
		bOverlap = pPackStoreParam->IsOverlap;
	}
	int skill = 0;
	int skillLevel = 0;
	if (iEquipment.getInlayCount()>0)
	{
		skill = iEquipment.getInalySkill();
		skillLevel = iEquipment.getInalySkillLevel();
	}
	else
	{
		skill = iEquipment.getBaseSkill();
		skillLevel = iEquipment.getBaseSkillLevel();
	}
	return OnInsertHeroEquip(g_Proxy, lPlayerID, 1, iItemID, iEquipment.getLevel(), iEquipment.getPostion(),bOverlap,skill,skillLevel,HeroPack);
}
//������Դ,ʵ�ֻ�����Դ�����ӻ����
int CMarketManager::OnUpDataResource (int handle, int ItemID, int Count,unsigned long lPlayerID, unsigned long lMapID, unsigned long CityID)
{
	if(Count == 0)
		return 0;

	PlayerResource playerRes,updatePlayerRes;
	playerRes.setmasterid(CityID,lMapID);
	playerRes.setdetailid(lPlayerID);
	if(g_Proxy.db_select(playerRes) != DB_Proxy::DB_SUCCEED)
	{//��ѯ���ʧ��
		return 0;
	}

	if(Count >0)
	{//����,��Ҫ�жϲֿ�ʣ������
		if(Count > playerRes.content_)
			return 3;
	}
	else
	{//����,��Ҫ�ж���Դ�Ƿ��㹻,�����Ӳֿ�ʣ������
	}
	updatePlayerRes.setmasterid(CityID,lMapID);
	updatePlayerRes.setdetailid(lPlayerID);
	updatePlayerRes.content_ = -Count;
	int iFlag = 1;
	switch(ItemID)
	{
	case EN_FOOD:			//��ʳ
		playerRes.foodcount_ += Count;
		if(playerRes.foodcount_ <0)
		{
			iFlag = 0;
		}
		else
		{
			updatePlayerRes.foodcount_ = Count;
		}
		break;
	case EN_WOOD:			//ľ��
		playerRes.woodcount_ += Count;
		if(playerRes.woodcount_ <0)
		{
			iFlag = 0;
		}
		else
		{
			updatePlayerRes.woodcount_ = Count;
		}
		break;
	case EN_IRON:			//��
		playerRes.ironcount_ += Count;
		if(playerRes.ironcount_ <0)
		{
			iFlag = 0;
		}
		else
		{
			updatePlayerRes.ironcount_ = Count;
		}
		break;
	case EN_SKIN:			//Ƥ
		playerRes.skincount_ += Count;
		if(playerRes.skincount_ <0)
		{
			iFlag = 0;
		}
		else
		{
			updatePlayerRes.skincount_ = Count;
		}
		break;
	case EN_RED_STONE:		//�챦ʯ
		playerRes.redstonecount_ += Count;
		if(playerRes.redstonecount_ <0)
		{
			iFlag = 0;
		}
		else
		{
			updatePlayerRes.redstonecount_ = Count;
		}
		break;
	case EN_YELLOW_STONE:		//�Ʊ�ʯ
		playerRes.yollowstonecount_ += Count;
		if(playerRes.yollowstonecount_ <0)
		{
			iFlag = 0;
		}
		else
		{
			updatePlayerRes.yollowstonecount_ = Count;
		}
		break;
	case EN_GREEN_STONE:	//�̱�ʯ
		playerRes.greenstonecount_ += Count;
		if(playerRes.greenstonecount_ <0)
		{
			iFlag = 0;
		}
		else
		{
			updatePlayerRes.greenstonecount_ = Count;
		}
		break;
	case EN_BLACK_STONE:	//����
		playerRes.backironcount_ += Count;
		if(playerRes.backironcount_ <0)
		{
			iFlag = 0;
		}
		else
		{
			updatePlayerRes.backironcount_ = Count;
		}
		break;
	};
	if(iFlag == 0)
		return 2;//��Դ�����۳�

	if(g_Proxy.db_increase(updatePlayerRes) != DB_Proxy::DB_SUCCEED)
		return 0;

	return 1;
	/*
	PlayerResource ResByPlayer;
	memset(&ResByPlayer, 0,sizeof(ResByPlayer));
	ResByPlayer.setmasterid(CityID, lMapID);
	ResByPlayer.setdetailid(lPlayerID);
	if (g_Proxy.db_select(ResByPlayer) == DB_Proxy::DB_SUCCEED)
	{
		if (Count > 0)
		{
			if (Count > ResByPlayer.content_)
				return 3;
			else
				ResByPlayer.content_ -= Count;
		}
		else
			ResByPlayer.content_ += Count;??
		ResByPlayer.setmasterid(CityID, lMapID);
		ResByPlayer.setdetailid(lPlayerID);
		if (g_Proxy.db_update(ResByPlayer, ResByPlayer) != DB_Proxy::DB_SUCCEED)
			return 0;
	
		int iC = 0;
		switch(ItemID)
		{
		case EN_FOOD:			//��ʳ
			ResByPlayer.foodcount_ += Count;
			iC = ResByPlayer.foodcount_;
			break;
		case EN_WOOD:			//ľ��
			ResByPlayer.woodcount_ += Count;
			iC = ResByPlayer.woodcount_;
			break;
		case EN_IRON:			//��
			ResByPlayer.ironcount_ += Count;
			iC = ResByPlayer.ironcount_;
			break;
		case EN_SKIN:			//Ƥ
			ResByPlayer.skincount_ += Count;
			iC = ResByPlayer.skincount_;
			break;
		case EN_RED_STONE:		//�챦ʯ
			ResByPlayer.redstonecount_ += Count;
			iC = ResByPlayer.redstonecount_;
			break;
		case EN_YELLOW_STONE:		//�Ʊ�ʯ
			ResByPlayer.yollowstonecount_ += Count;
			iC = ResByPlayer.yollowstonecount_;
			break;
		case EN_GREEN_STONE:	//�̱�ʯ
			ResByPlayer.greenstonecount_ += Count;
			iC = ResByPlayer.greenstonecount_;
			break;
		case EN_BLACK_STONE:	//����
			ResByPlayer.backironcount_ += Count;
			iC = ResByPlayer.backironcount_;
			break;
		};

		if (iC < 0) return 2;
		
		ResByPlayer.setmasterid(CityID, lMapID);
		ResByPlayer.setdetailid(lPlayerID);
		if (g_Proxy.db_update(ResByPlayer, ResByPlayer) != DB_Proxy::DB_SUCCEED)
			return 0;
		return 1;
	}
	return 0;
	*/
}
//��Ʒ����
void CMarketManager::OnPutUp(int handle, unsigned long lPlayerID, void * pDataBuffer, int iDataSize)
{
	if (iDataSize != sizeof(PUT_UP))
	{
		LOG_TRACE_EX(false,("CMarketManager::OnPutUp-->��Ϣ���Ƿ������%d\n",lPlayerID));
		return ;
	}

	PUT_UP * pPutUp = (PUT_UP *)pDataBuffer;
	MARKET_INFO MarketInfo;
	//�۸���֤
	if (pPutUp->iMinPrice < 1)//
	{
		MarketInfo.serialize(pPutUp->iType, pPutUp->iSaleID, pPutUp->iCount, EN_GOODS_BASE_PRICE, MDM_GP_MAP_MARKET, lPlayerID);
		asynch_write(handle, (char *)&MarketInfo, MarketInfo.length);
		return;
	}
	if (pPutUp->iOncePrice <= pPutUp->iMinPrice)
	{
		MarketInfo.serialize(pPutUp->iType, pPutUp->iSaleID, pPutUp->iCount, EN_GOODS_ONCE_PRICE, MDM_GP_MAP_MARKET, lPlayerID);
		asynch_write(handle, (char *)&MarketInfo, MarketInfo.length);
		return;
	}
	int iCountry = 0;
	map<unsigned long ,tagPlayerInfo>::iterator ith = m_mapPlayerInfo.find(lPlayerID);
	if (ith == m_mapPlayerInfo.end())
	{
		//_ASSERT(false);
		return ;
	}
	
	Market PeopleMarket;
	memset(&PeopleMarket,0,sizeof(PeopleMarket));

	PeopleMarket.playerid_ = lPlayerID;
	PeopleMarket.type_ = pPutUp->iType;
	PeopleMarket.itemid_ = pPutUp->iSaleID;
	PeopleMarket.minprice_ = pPutUp->iMinPrice;
	PeopleMarket.onceprice_ = pPutUp->iOncePrice;
	PeopleMarket.count_ = pPutUp->iCount;
	PeopleMarket.bidplayerid_ = lPlayerID;
	PeopleMarket.currentprice_ = pPutUp->iMinPrice;
	PeopleMarket.starttime_ = IceUtil::Time::now().toSeconds()+86400;//24 hour
	PeopleMarket.country_ = ith->second.iCountry;
	PeopleMarket.mapid_ = pPutUp->lMapID;
	PeopleMarket.cityid_ = pPutUp->lCityID;
	PeopleMarket.bidmapid_ = pPutUp->lMapID;
	PeopleMarket.bidcityid_ = pPutUp->lCityID;
	strcpy(PeopleMarket.playername_, ith->second.szAccounts);

	int ret = 0;
	WeaponAttribute *pWeaponAttribute = NULL;
	DefendAttribute *pDefendAttribute = NULL;
	int oldProCount = 0,curProCount = 0;
	PlayerOfficerTable officer;
	officer.setmasterid(lPlayerID);
	officer.setdetailid(lPlayerID);
	g_Proxy.db_select(officer);

	switch(pPutUp->iType)
	{
	case WEAPON_ITEM:				//����
	case ANIMALS_TAME_ITEM:			//Ұ�޲���
	case SPECIAL_TOOL_ITEM:			//���⹤��
		pWeaponAttribute = g_LoadDBinfo.GetWeaponAttribute(pPutUp->iSaleID);
		if(pWeaponAttribute == NULL)
		{
			ret = 111;
		}
		else
		{
			if(pPutUp->iCount * pWeaponAttribute->cost > pPutUp->iMinPrice)
			{//��ͼ�̫��
				ret = 100;
			}
			else
			{
				ret = m_UserInfo.OnUpDataItem(pPutUp->iSaleID, pPutUp->iType, -pPutUp->iCount, lPlayerID, pPutUp->lMapID, pPutUp->lCityID,oldProCount,curProCount);
				if (ret)
				{
					ISystemLog::getInstance()->playerProduce_ListingFolk_Log(officer.playerid_,officer.officername_,
						pPutUp->lMapID,pPutUp->lCityID,pPutUp->iType,pPutUp->iSaleID,oldProCount,curProCount,pPutUp->iCount);
				}
			}
		}
		break;
	case DEFEND_ITEM:				//����
		pDefendAttribute = g_LoadDBinfo.GetDefendAttribute(pPutUp->iSaleID);
		if(pDefendAttribute == NULL)
		{
			ret = 111;
		}
		else
		{
			if(pPutUp->iCount * pDefendAttribute->cost > pPutUp->iMinPrice)
			{//��ͼ�̫��
				ret = 100;
			}
			else
			{
				ret = m_UserInfo.OnUpDataItem(pPutUp->iSaleID, pPutUp->iType, -pPutUp->iCount, lPlayerID, pPutUp->lMapID, pPutUp->lCityID,oldProCount,curProCount);	
				if (ret)
				{
					ISystemLog::getInstance()->playerProduce_ListingFolk_Log(officer.playerid_,officer.officername_,
						pPutUp->lMapID,pPutUp->lCityID,pPutUp->iType,pPutUp->iSaleID,oldProCount,curProCount,pPutUp->iCount);
				}
			}
		}
		break;
	case EN_HERO_EQUIP:				//Ӣ��װ��
		{
			PlayerItemStorageTable equipItem;
			equipItem.setmasterid(1,lPlayerID);
			equipItem.setdetailid(pPutUp->lSortID);
			if (g_Proxy.db_select(equipItem)==DB_Proxy::DB_SUCCEED)
			{
				PeopleMarket.itemid_ = equipItem.itemid_;
				IEquipment iEquipment(&g_LoadDBinfo,equipItem.itemid_);
				if(!iEquipment.isCanSale())
				{//��װ����������
					ret = -1;
				}
				else
				{
					if(iEquipment.getPrice()> pPutUp->iMinPrice)
						ret = 100;//��ͼ����ܵ��ڳɱ���
					else
					{
						ret = OnUpDataEquip(lPlayerID, equipItem.itemid_, pPutUp->lSortID);
						if (ret==1)
						{
							ISystemLog::getInstance()->playerEquip_ListingFolk_Log(officer.playerid_,officer.officername_,
								equipItem.pos_,equipItem.itemid_,1);
						}
					}
					
				}
			}

		}
		break;
	case EN_BASIC_RESOURCE:			//��������
		if(pPutUp->iSaleID >=9)
		{
			ret = 111;
		}
		else
		{
			if(pPutUp->iCount * m_MarkPrice[1][pPutUp->iSaleID] > pPutUp->iMinPrice)
			{//��ͼ�̫��
				ret = 100;
			}
			else
			{
				ret = OnUpDataResource(handle, pPutUp->iSaleID, -pPutUp->iCount, lPlayerID, pPutUp->lMapID, pPutUp->lCityID);
			}
		}
		break;
	};
	if (ret == 1)
	{//������Ƽ�¼
		if (g_Proxy.db_insert(PeopleMarket) != DB_Proxy::DB_SUCCEED)
		{
			return;
		}
		MarketInfo.serialize(pPutUp->iType, pPutUp->iSaleID, pPutUp->iCount, EN_GOODS_HANG_SUCCEED, MDM_GP_MAP_MARKET, lPlayerID);
		asynch_write(handle, (char *)&MarketInfo, MarketInfo.length);
	}
	else if (ret == 2)
	{//������Ʒ��������
		MarketInfo.serialize(pPutUp->iType, pPutUp->iSaleID, pPutUp->iCount, EN_GOODS_COUNT, MDM_GP_MAP_MARKET, lPlayerID);
		asynch_write(handle, (char *)&MarketInfo, MarketInfo.length);
	}
	else if(ret == 100)
	{//�ͼ۹��ͣ����ڳɱ���
		MarketInfo.serialize(pPutUp->iType, pPutUp->iSaleID, pPutUp->iCount, EN_GOODS_BASE_PRICE, MDM_GP_MAP_MARKET, lPlayerID);
		asynch_write(handle, (char *)&MarketInfo, MarketInfo.length);
	}
	else if(ret == 111)
	{//ʧ��
		MarketInfo.serialize(pPutUp->iType, pPutUp->iSaleID, pPutUp->iCount, EN_GOODS_HANG_FAILED, MDM_GP_MAP_MARKET, lPlayerID);
		asynch_write(handle, (char *)&MarketInfo, MarketInfo.length);
	}
	else if(ret == -1)
	{//������EN_NOT_CANSALE
		MarketInfo.serialize(pPutUp->iType, pPutUp->iSaleID, pPutUp->iCount, EN_NOT_CANSALE, MDM_GP_MAP_MARKET, lPlayerID);
		asynch_write(handle, (char *)&MarketInfo, MarketInfo.length);
	}
}
//����
void CMarketManager::OnBidden(int handle, unsigned long lPlayerID, void * pDataBuffer, int iDataSize, int iMarketType)
{
	if (iDataSize != sizeof(GOODS_BIDDEN))
	{
		LOG_TRACE_EX(false,("CMarketManager::OnBidden-->��Ϣ���Ƿ������%d\n",lPlayerID));
		return ;
	}

	GOODS_BIDDEN *pBidden = (GOODS_BIDDEN *)pDataBuffer;

	int iCountry = 0;
	map<unsigned long ,tagPlayerInfo>::iterator itp = m_mapPlayerInfo.find(lPlayerID);
	if (itp == m_mapPlayerInfo.end())
	{
		return ;
	}
	iCountry = itp->second.iCountry + iMarketType;
	MARKET_INFO MarketInfo;
	//��ѯ��������Ʒ��Ϣ
	Market PeopleMarket;
	PeopleMarket.setmasterid(iCountry,pBidden->iType);
	PeopleMarket.setdetailid(pBidden->lSortID);
	if (g_Proxy.db_select(PeopleMarket) != DB_Proxy::DB_SUCCEED)
	{
		MarketInfo.serialize(pBidden->iType, pBidden->lSortID, 1, EN_GOODS_BIDDING_FAILED, MDM_GP_MAP_MARKET, lPlayerID);
		asynch_write(handle, (char *)&MarketInfo, MarketInfo.length);
		return ;
	}
	//���۲��ܵ��ڵ�ǰ��
	if (PeopleMarket.currentprice_ >= pBidden->iCurrentPrice)
	{
		MarketInfo.serialize(pBidden->iType, pBidden->lSortID, 1, EN_BIDDEN_ERROR, MDM_GP_MAP_MARKET, lPlayerID);
		asynch_write(handle, (char *)&MarketInfo, MarketInfo.length);
		return;
	}
	//�Լ����ܳ���
	if (PeopleMarket.playerid_ == lPlayerID)
	{
		MarketInfo.serialize(pBidden->iType, pBidden->lSortID, 1, EN_BIDDEN_OWN, MDM_GP_MAP_MARKET, lPlayerID);
		asynch_write(handle, (char *)&MarketInfo, MarketInfo.length);
		return;
	}
	unsigned long lPrePlayerID = PeopleMarket.bidplayerid_;
	long lPreMoney = PeopleMarket.currentprice_;
	//������������
	if (lPrePlayerID == lPlayerID)
	{
		MarketInfo.serialize(pBidden->iType, pBidden->lSortID, 1, EN_ALLOW_BIDDEN, MDM_GP_MAP_MARKET, lPlayerID);
		asynch_write(handle, (char *)&MarketInfo, MarketInfo.length);
		return;
	}
	//�������ֻ��Ϊһ�ڼۣ����൱�����
	if(pBidden->iCurrentPrice >=PeopleMarket.onceprice_)
	{//�൱��һ�ڼ�
		//pBidden->iCurrentPrice = PeopleMarket.onceprice_;
		BUY_GOODS buy_goods;
		buy_goods.serialize(pBidden->lMapID,pBidden->lCityID,PeopleMarket.type_,pBidden->lSortID,lPlayerID,MDM_GP_MAP_MARKET,EN_C_MARKET_BIDDEN);
		OnOnePrice(handle,lPlayerID,&buy_goods,sizeof(BUY_GOODS),0);
		return;
	}
	PlayerChatRoomTable Money;
	memset(&Money, 0, sizeof(PlayerChatRoomTable));
	Money.setid(lPlayerID);
	if (g_Proxy.db_select(Money) != DB_Proxy::DB_SUCCEED)
		return;
	
	if (Money.money_ < pBidden->iCurrentPrice)
	{
		MarketInfo.serialize(pBidden->iType, pBidden->lSortID, 1, EN_GOODS_NOT_ENOUGHT_GOLD, MDM_GP_MAP_MARKET, lPlayerID);
		asynch_write(handle, (char *)&MarketInfo, MarketInfo.length);
		return;
	}

	//���ȿ۳���ҵĳ�������Ľ�Ǯ
	memset(&Money, 0, sizeof(PlayerChatRoomTable));
	Money.money_ = -pBidden->iCurrentPrice;
	Money.setid(lPlayerID);
	if (g_Proxy.db_increase(Money) != DB_Proxy::DB_SUCCEED)
	{
		MarketInfo.serialize(pBidden->iType, pBidden->lSortID, 1, EN_BUY_FAILED, MDM_GP_MAP_MARKET, lPlayerID);
		asynch_write(handle, (char *)&MarketInfo, MarketInfo.length);
		return;
	}
	PlayerOfficerTable officer;
	officer.setmasterid(lPlayerID);
	officer.setdetailid(lPlayerID);
	g_Proxy.db_select(officer);
	PlayerChatRoomTable preMoney;
	if(lPrePlayerID != PeopleMarket.playerid_)
	{
		//�����ϴγ��۵����
		preMoney.money_ = lPreMoney;
		preMoney.setid(lPrePlayerID);
		if (g_Proxy.db_increase(preMoney) != DB_Proxy::DB_SUCCEED)
		{
			MarketInfo.serialize(pBidden->iType, pBidden->lSortID, 1, EN_BUY_FAILED, MDM_GP_MAP_MARKET, lPlayerID);
			asynch_write(handle, (char *)&MarketInfo, MarketInfo.length);
			return;
		}
	}
	switch(PeopleMarket.type_)
	{
	case WEAPON_ITEM:				//����
	case ANIMALS_TAME_ITEM:			//Ұ�޲���
	case SPECIAL_TOOL_ITEM:			//���⹤��
	case DEFEND_ITEM:				//����
		{
			ISystemLog::getInstance()->playerMoney_BidArmyEquipFolk_Log(officer.playerid_,officer.officername_,
				Money.money_ + pBidden->iCurrentPrice,Money.money_,pBidden->iCurrentPrice,
				pBidden->lMapID,pBidden->lCityID,PeopleMarket.type_,PeopleMarket.itemid_,PeopleMarket.count_);
			if(lPrePlayerID != PeopleMarket.playerid_)
			{
				officer.setmasterid(lPrePlayerID);
				officer.setdetailid(lPrePlayerID);
				g_Proxy.db_select(officer);
				ISystemLog::getInstance()->playerMoney_BuyFailArmyEquipFolk_Log(officer.playerid_,officer.officername_,
					preMoney.money_ - lPreMoney,preMoney.money_,lPreMoney,
					PeopleMarket.bidmapid_,PeopleMarket.bidcityid_,
					PeopleMarket.type_,PeopleMarket.itemid_,PeopleMarket.count_);
			}
		}
		break;
	case EN_HERO_EQUIP:				//Ӣ��װ��
		{
			IEquipment euip(&g_LoadDBinfo,PeopleMarket.itemid_);
			ISystemLog::getInstance()->playerMoney_BidHeroEquipFolk_Log(officer.playerid_,officer.officername_,
				Money.money_ + pBidden->iCurrentPrice,Money.money_,pBidden->iCurrentPrice,
				pBidden->lMapID,pBidden->lCityID,euip.getPostion(),PeopleMarket.itemid_,PeopleMarket.count_);
			if(lPrePlayerID != PeopleMarket.playerid_)
			{
				officer.setmasterid(lPrePlayerID);
				officer.setdetailid(lPrePlayerID);
				g_Proxy.db_select(officer);
				ISystemLog::getInstance()->playerMoney_BuyFailHeroEquipFolk_Log(officer.playerid_,officer.officername_,
					preMoney.money_ - lPreMoney,preMoney.money_,lPreMoney,
					PeopleMarket.bidmapid_,PeopleMarket.bidcityid_,
					euip.getPostion(),PeopleMarket.itemid_,PeopleMarket.count_);
			}
		}
		break;
	case EN_BASIC_RESOURCE:			//��Դ
		{
			ISystemLog::getInstance()->playerMoney_BidResFolk_Log(officer.playerid_,officer.officername_,
				Money.money_ + pBidden->iCurrentPrice,Money.money_,pBidden->iCurrentPrice,
				pBidden->lMapID,pBidden->lCityID,PeopleMarket.itemid_,PeopleMarket.count_);
			if(lPrePlayerID != PeopleMarket.playerid_)
			{
				officer.setmasterid(lPrePlayerID);
				officer.setdetailid(lPrePlayerID);
				g_Proxy.db_select(officer);
				ISystemLog::getInstance()->playerMoney_BuyFailResFolk_Log(officer.playerid_,officer.officername_,
					preMoney.money_ - lPreMoney,preMoney.money_,lPreMoney,
					PeopleMarket.bidmapid_,PeopleMarket.bidcityid_,
					PeopleMarket.itemid_,PeopleMarket.count_);
			}
		}
		break;

	}
	//���µ�ǰ�����³���
	PeopleMarket.bidplayerid_ = lPlayerID;
	PeopleMarket.currentprice_ = pBidden->iCurrentPrice;
	PeopleMarket.bidmapid_ = pBidden->lMapID;
	PeopleMarket.bidcityid_ = pBidden->lCityID;

	PeopleMarket.setmasterid(iCountry,pBidden->iType);
	PeopleMarket.setdetailid(pBidden->lSortID);
	if (g_Proxy.db_update(PeopleMarket, PeopleMarket) == DB_Proxy::DB_SUCCEED)
	{
		MarketInfo.serialize(pBidden->iType, pBidden->lSortID, 1, EN_GOODS_BIDDING_SUCCEED, MDM_GP_MAP_MARKET, lPlayerID);
		asynch_write(handle, (char *)&MarketInfo, MarketInfo.length);
	}

}
//һ�ڼ�
void CMarketManager::OnOnePrice(int handle, unsigned long lPlayerID, void * pDataBuffer, int iDataSize, int iMarketType)
{
	if (iDataSize != sizeof(BUY_GOODS))
	{
		LOG_TRACE_EX(false,("CMarketManager::OnOnePrice-->��Ϣ���Ƿ������%d\n",lPlayerID));
		return ;
	}

	BUY_GOODS * pBuyGoods = (BUY_GOODS *)pDataBuffer;

	int iCountry = 0, ret = 0;
	map<unsigned long ,tagPlayerInfo>::iterator itp = m_mapPlayerInfo.find(lPlayerID);
	if (itp->first <= 0 || itp->second.iCountry <= 0)
	{
		return ;
	}
	iCountry = itp->second.iCountry + iMarketType;

	MARKET_INFO MarketInfo;
	//��ѯ������Ʒ��Ϣ
	Market PeopleMarket;
	PeopleMarket.setmasterid(iCountry,pBuyGoods->iType);
	PeopleMarket.setdetailid(pBuyGoods->lSortID);
	if (g_Proxy.db_select(PeopleMarket) != DB_Proxy::DB_SUCCEED)
	{
		MarketInfo.serialize(pBuyGoods->iType, pBuyGoods->lSortID, 1, EN_GOODS_SALE_FAILED, MDM_GP_MAP_MARKET, lPlayerID);
		asynch_write(handle, (char *)&MarketInfo, MarketInfo.length);
		return;
	}
	//���Ҳ��ܹ���
	if (PeopleMarket.playerid_ == lPlayerID)
	{
		MarketInfo.serialize(pBuyGoods->iType, pBuyGoods->lSortID, 1, EN_BIDDEN_OWN, MDM_GP_MAP_MARKET, lPlayerID);
		asynch_write(handle, (char *)&MarketInfo, MarketInfo.length);
		return;
	}
	PlayerChatRoomTable Money;
	Money.setid(lPlayerID);
	if (g_Proxy.db_select(Money) != DB_Proxy::DB_SUCCEED)
	{
		MarketInfo.serialize(pBuyGoods->iType, pBuyGoods->lSortID, 1, EN_GOODS_NOT_ENOUGHT_GOLD, MDM_GP_MAP_MARKET, lPlayerID);
		asynch_write(handle, (char *)&MarketInfo, MarketInfo.length);
		return;
	}

	unsigned long lPrePlayerID = PeopleMarket.bidplayerid_;
	long lPreMoney = PeopleMarket.currentprice_;
	long lNeedMoney = PeopleMarket.onceprice_ ;
	if (lPrePlayerID == lPlayerID)
	{//���һ�γ�����Ϊ����ң�ֻ��֧����ۼ���
		lNeedMoney -= lPreMoney;
	}
	int oldmoney = Money.money_;
	if (Money.money_ < lNeedMoney)
	{
		MarketInfo.serialize(pBuyGoods->iType, pBuyGoods->lSortID, 1, EN_GOODS_NOT_ENOUGHT_GOLD, MDM_GP_MAP_MARKET, lPlayerID);
		asynch_write(handle, (char *)&MarketInfo, MarketInfo.length);
		return;
	}
	int oldProCount=0,curProCount=0;
	PlayerOfficerTable officer;
	officer.setmasterid(lPlayerID);
	officer.setdetailid(lPlayerID);
	g_Proxy.db_select(officer);

	switch(pBuyGoods->iType)
	{
	case WEAPON_ITEM:				//����
	case DEFEND_ITEM:				//����
	case ANIMALS_TAME_ITEM:			//Ұ�޲���
	case SPECIAL_TOOL_ITEM:			//���⹤��
		ret = m_UserInfo.OnUpDataItem(PeopleMarket.itemid_, PeopleMarket.type_, PeopleMarket.count_, lPlayerID, 
			pBuyGoods->lMapID, pBuyGoods->lCityID,oldProCount,curProCount);
		if (ret == 1)
		{
			ISystemLog::getInstance()->playerProduce_BuyFolk_Log(officer.playerid_,officer.officername_,
				pBuyGoods->lMapID,pBuyGoods->lCityID,
				pBuyGoods->iType,PeopleMarket.itemid_,oldProCount,curProCount,PeopleMarket.count_);
		}
		break;
	case EN_HERO_EQUIP:				//Ӣ��װ��
		ret = OnUpDataEquip(lPlayerID, PeopleMarket.itemid_,0);
		if (ret)
		{
			IEquipment equip(&g_LoadDBinfo,PeopleMarket.itemid_);
			ISystemLog::getInstance()->playerEquip_BuyFolk_Log(officer.playerid_,officer.officername_,
				equip.getPostion(),PeopleMarket.itemid_,PeopleMarket.count_);
		}
		break;
	case EN_BASIC_RESOURCE:			//��������
		ret = OnUpDataResource(handle, PeopleMarket.itemid_, PeopleMarket.count_, lPlayerID, pBuyGoods->lMapID, pBuyGoods->lCityID);
		break;
	};	
	if (ret == 0)
	{
		MarketInfo.serialize(pBuyGoods->iType, pBuyGoods->lSortID, 1, EN_BUY_FAILED, MDM_GP_MAP_MARKET, lPlayerID);
		asynch_write(handle, (char *)&MarketInfo, MarketInfo.length);
		return;
	}
	if (ret == 3 && pBuyGoods->iType != EN_HERO_EQUIP)
	{
		MarketInfo.serialize(pBuyGoods->iType, pBuyGoods->lSortID, 1, EN_STORAGE_CONTENT_ERROR, MDM_GP_MAP_MARKET, lPlayerID);
		asynch_write(handle, (char *)&MarketInfo, MarketInfo.length);
		return;
	}
	//�۳����һ�ڼ۹�����Ҫ�Ľ�ǮlNeedMoney
	memset(&Money, 0, sizeof(PlayerChatRoomTable));
	Money.money_ -= lNeedMoney;
	Money.setid(lPlayerID);
	if (g_Proxy.db_increase(Money) != DB_Proxy::DB_SUCCEED)
	{
		MarketInfo.serialize(pBuyGoods->iType, pBuyGoods->lSortID, 0, EN_BUY_FAILED, MDM_GP_MAP_MARKET, lPlayerID);
		asynch_write(handle, (char *)&MarketInfo, MarketInfo.length);
		return;
	}
	PlayerChatRoomTable prePlayerMoney;
	//�����ϴγ��۵���ҽ�Ǯ
	if ((lPrePlayerID != lPlayerID) && (lPrePlayerID != PeopleMarket.playerid_))
	{//
		prePlayerMoney.setid(lPrePlayerID);
		prePlayerMoney.money_ = lPreMoney;
		g_Proxy.db_increase(prePlayerMoney);
	}
	PlayerChatRoomTable salorMoney;
	int salorprice = (int)(PeopleMarket.onceprice_*0.9);//˰�ʹ̶���10%��
	//���������ӽ�Ǯ
	salorMoney.money_ = salorprice;
	int countrymoney = (PeopleMarket.onceprice_ - Money.money_)/2;
	salorMoney.setid(PeopleMarket.playerid_);
	if (g_Proxy.db_increase(salorMoney) != DB_Proxy::DB_SUCCEED)
	{
		MarketInfo.serialize(pBuyGoods->iType, pBuyGoods->lSortID, 1, EN_BUY_FAILED, MDM_GP_MAP_MARKET, lPlayerID);
		asynch_write(handle, (char *)&MarketInfo, MarketInfo.length);
		return;
	}

	switch(pBuyGoods->iType)
	{
	case WEAPON_ITEM:				//����
	case DEFEND_ITEM:				//����
	case ANIMALS_TAME_ITEM:			//Ұ�޲���
	case SPECIAL_TOOL_ITEM:			//���⹤��
		{
			ISystemLog::getInstance()->playerMoney_BidArmyEquipFolk_Log(officer.playerid_,officer.officername_,
				oldmoney,Money.money_,lNeedMoney,pBuyGoods->lMapID,pBuyGoods->lCityID,
				pBuyGoods->iType,PeopleMarket.itemid_,PeopleMarket.count_);
			if ((lPrePlayerID != lPlayerID) && (lPrePlayerID != PeopleMarket.playerid_))
			{
				//�����ϴγ��۵���ҽ�Ǯ
				officer.setmasterid(lPrePlayerID);
				officer.setdetailid(lPrePlayerID);
				g_Proxy.db_select(officer);
				ISystemLog::getInstance()->playerMoney_BuyFailArmyEquipFolk_Log(officer.playerid_,officer.officername_,
					prePlayerMoney.money_ - lPreMoney,prePlayerMoney.money_,lPreMoney,
					PeopleMarket.bidmapid_,PeopleMarket.bidcityid_,
					pBuyGoods->iType,PeopleMarket.itemid_,PeopleMarket.count_);
			}
			//���������ӽ�Ǯ
			{
				officer.setmasterid(PeopleMarket.playerid_);
				officer.setdetailid(PeopleMarket.playerid_);
				g_Proxy.db_select(officer);
				ISystemLog::getInstance()->playerMoney_SaleArmyEquipFolk_Log(officer.playerid_,officer.officername_,
					salorMoney.money_ - salorprice,salorMoney.money_,salorprice,
					PeopleMarket.mapid_,PeopleMarket.cityid_,
					PeopleMarket.type_,PeopleMarket.itemid_,PeopleMarket.count_);
			}
		}
		break;
	case EN_HERO_EQUIP:				//Ӣ��װ��
		{
			IEquipment equip(&g_LoadDBinfo,PeopleMarket.itemid_);
			ISystemLog::getInstance()->playerMoney_BidHeroEquipFolk_Log(officer.playerid_,officer.officername_,
				oldmoney,Money.money_,lNeedMoney,pBuyGoods->lMapID,pBuyGoods->lCityID,
				equip.getPostion(),PeopleMarket.itemid_,PeopleMarket.count_);
			if ((lPrePlayerID != lPlayerID) && (lPrePlayerID != PeopleMarket.playerid_))
			{
				//�����ϴγ��۵���ҽ�Ǯ
				officer.setmasterid(lPrePlayerID);
				officer.setdetailid(lPrePlayerID);
				g_Proxy.db_select(officer);
				ISystemLog::getInstance()->playerMoney_BuyFailHeroEquipFolk_Log(officer.playerid_,officer.officername_,
					prePlayerMoney.money_ - lPreMoney,prePlayerMoney.money_,lPreMoney,
					PeopleMarket.bidmapid_,PeopleMarket.bidcityid_,
					equip.getPostion(),PeopleMarket.itemid_,PeopleMarket.count_);
			}
			//���������ӽ�Ǯ
			{
				officer.setmasterid(PeopleMarket.playerid_);
				officer.setdetailid(PeopleMarket.playerid_);
				g_Proxy.db_select(officer);
				ISystemLog::getInstance()->playerMoney_SaleHeroEquipFolk_Log(officer.playerid_,officer.officername_,
					salorMoney.money_ - salorprice,salorMoney.money_,salorprice,
					PeopleMarket.mapid_,PeopleMarket.cityid_,
					equip.getPostion(),PeopleMarket.itemid_,PeopleMarket.count_);
			}
		}
		break;
	case EN_BASIC_RESOURCE:			//��������
		{
			ISystemLog::getInstance()->playerMoney_BidResFolk_Log(officer.playerid_,officer.officername_,
				oldmoney,Money.money_,lNeedMoney,pBuyGoods->lMapID,pBuyGoods->lCityID,
				PeopleMarket.itemid_,PeopleMarket.count_);
			if ((lPrePlayerID != lPlayerID) && (lPrePlayerID != PeopleMarket.playerid_))
			{
				//�����ϴγ��۵���ҽ�Ǯ
				officer.setmasterid(lPrePlayerID);
				officer.setdetailid(lPrePlayerID);
				g_Proxy.db_select(officer);
				ISystemLog::getInstance()->playerMoney_BuyFailResFolk_Log(officer.playerid_,officer.officername_,
					prePlayerMoney.money_ - lPreMoney,prePlayerMoney.money_,lPreMoney,
					PeopleMarket.bidmapid_,PeopleMarket.bidcityid_,
					PeopleMarket.itemid_,PeopleMarket.count_);
			}
			//���������ӽ�Ǯ
			{
				officer.setmasterid(PeopleMarket.playerid_);
				officer.setdetailid(PeopleMarket.playerid_);
				g_Proxy.db_select(officer);
				ISystemLog::getInstance()->playerMoney_SaleResFolk_Log(officer.playerid_,officer.officername_,
					salorMoney.money_ - salorprice,salorMoney.money_,salorprice,
					PeopleMarket.mapid_,PeopleMarket.cityid_,
					PeopleMarket.itemid_,PeopleMarket.count_);
			}
		}
		break;
	};	
	//50%��˰�ռ������ϵͳ
	if(countrymoney >0)
	{
		Notify_MarketRate_Request notifymsg;
		notifymsg.serialize(0,PeopleMarket.playerid_,countrymoney);
		event_notify(MDM_GP_MAP_CONSORTIA,(char*)&notifymsg,notifymsg.length);
	}
	//ɾ��
	PeopleMarket.setmasterid(iCountry,pBuyGoods->iType);
	PeopleMarket.setdetailid(pBuyGoods->lSortID);
	g_Proxy.db_delete(PeopleMarket);

	MarketInfo.serialize(pBuyGoods->iType, pBuyGoods->lSortID, 1, EN_BUY_SUCCEED, MDM_GP_MAP_MARKET, lPlayerID);
	asynch_write(handle, (char *)&MarketInfo, MarketInfo.length);
	//����֪ͨ
	g_DataServer.broadMsgAndNotify(PeopleMarket.playerid_,MDM_GP_MAP_MARKET,PeopleMarket.itemid_,PeopleMarket.count_,PeopleMarket.type_,EN_S_NOTIFYMARKET,0,MDM_GP_MAP_MARKET);
}
//��Ʒ�б� 
void CMarketManager::OnGoodsList(int handle, int iNext, int iType,unsigned long lPlayerID)
{
	int iCountry = 0;
	map<unsigned long ,tagPlayerInfo>::iterator ith = m_mapPlayerInfo.find(lPlayerID);
	if (ith != m_mapPlayerInfo.end())
	{
		iCountry = ith->second.iCountry;
	}
	int iSendSize = 0, count = 0, iStart = 0;
	char *Buffer;
	char * tmp = 0;
	GOODS_LIST * pGoodsList = 0;
	Market PeopleMarket;
	PeopleMarket.setmasterid(iCountry,iType);
	int iRet = g_Proxy.db_multi_select(PeopleMarket, Buffer, count);
	if(iRet == DB_Proxy::DB_EOF)
	{
		tmp = new char[sizeof(GOODS_LIST)];
		pGoodsList = (GOODS_LIST*)tmp;
		pGoodsList->serialize(0,iType,0,MDM_GP_MAP_MARKET, lPlayerID);
		asynch_write(handle, (char *)pGoodsList, pGoodsList->length);
		if(tmp)
		{
			delete[] tmp;
			tmp = 0;
		}
		return ;
	}
	else if(iRet == DB_Proxy::DB_FAILED)
	{
		ERROR_INFO ErrorInfo;
		ErrorInfo.serialize(EN_DATA_ERROR, MDM_GP_MAP_MARKET, lPlayerID);
		asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
		printf("��Ʒ�б� ��ҵ�ID (%ld) ���ݿ�û������\n",lPlayerID);
		return ;
	}
	/*
	if (g_Proxy.db_multi_select(PeopleMarket, Buffer, count) != DB_Proxy::DB_SUCCEED)
	{
		ErrorInfo.serialize(EN_DATA_ERROR, MDM_GP_MAP_MARKET, lPlayerID);
		asynch_write(handle, (char *)&ErrorInfo, ErrorInfo.length);
		printf("��Ʒ�б� ��ҵ�ID (%ld) ���ݿ�û������\n",lPlayerID);
		return ;
	}
	*/
	int Item = count/sizeof(Market);
	Market *pMarket = (Market *)Buffer;
		
	tmp = new char[sizeof(GOODS_LIST) + sizeof(tagGoodsList)*30];
	pGoodsList = (GOODS_LIST *)tmp;

	if (iNext == 1) iStart = ith->second.iStart;
	int64 lTime = IceUtil::Time::now().toSeconds();
	int TimeLen = 0, n = 0;
	int iCarryTime = 0, iFreight = 0;
	PlayerOfficerTable officer;
	int ret = 0;
	int oldProCount=0,curProCount=0;
	for (int i = iStart; i < Item && iStart < Item; i++)
	{
		//24Сʱ
		TimeLen = int(pMarket[i].starttime_ - lTime);
		if (TimeLen <= 0)
		{
			if (iCountry == 1 || iCountry == 2)
			{
				unsigned long lBuyPlayerID = pMarket[i].playerid_;
				unsigned long lMapID = pMarket[i].mapid_;
				unsigned lCityID = pMarket[i].cityid_;
				int tmpmoney = 0;
				int curmoney = 0;
				if (pMarket[i].playerid_ != pMarket[i].bidplayerid_)
				{
					tmpmoney = OnBuy(pMarket[i].playerid_, pMarket[i].currentprice_,curmoney);
					
					lBuyPlayerID = pMarket[i].bidplayerid_;
					lMapID = pMarket[i].bidmapid_;
					lCityID = pMarket[i].bidcityid_;
				}

				switch(pMarket[i].type_)
				{
				case WEAPON_ITEM:				//����
				case DEFEND_ITEM:				//����
				case ANIMALS_TAME_ITEM:			//Ұ�޲���
				case SPECIAL_TOOL_ITEM:			//���⹤��
					
					ret = m_UserInfo.OnUpDataItem(pMarket[i].itemid_, pMarket[i].type_, pMarket[i].count_, lBuyPlayerID, lMapID, lCityID,oldProCount,curProCount);
					if (pMarket[i].playerid_ != pMarket[i].bidplayerid_)
					{//���ɹ�����¼���ҽ�Ǯ�仯��־
						officer.setmasterid(pMarket[i].playerid_);
						officer.setdetailid(pMarket[i].playerid_);
						g_Proxy.db_select(officer);
						ISystemLog::getInstance()->playerMoney_SaleArmyEquipFolk_Log(officer.playerid_,officer.officername_,
							curmoney - tmpmoney,curmoney,tmpmoney,pMarket[i].mapid_,pMarket[i].cityid_,
							pMarket[i].type_,pMarket[i].itemid_,pMarket[i].count_);
						if (ret)
						{
							officer.setmasterid(lBuyPlayerID);
							officer.setdetailid(lBuyPlayerID);
							g_Proxy.db_select(officer);
							ISystemLog::getInstance()->playerProduce_BuyFolk_Log(officer.playerid_,officer.officername_,
								pMarket[i].bidmapid_,pMarket[i].bidcityid_,pMarket[i].type_,pMarket[i].itemid_,
								oldProCount,curProCount,pMarket[i].count_);
						}
					}
					else
					{
						officer.setmasterid(lBuyPlayerID);
						officer.setdetailid(lBuyPlayerID);
						g_Proxy.db_select(officer);
						ISystemLog::getInstance()->playerProduce_SaleFailFolk_Log(officer.playerid_,officer.officername_,
							pMarket[i].mapid_,pMarket[i].cityid_,pMarket[i].type_,pMarket[i].itemid_,
							oldProCount,curProCount,pMarket[i].count_);
					}
					break;
				case EN_HERO_EQUIP:				//Ӣ��װ��
					{
						ret = OnUpDataEquip(lBuyPlayerID, pMarket[i].itemid_);
						IEquipment iEquipment(&g_LoadDBinfo,pMarket[i].itemid_);
						if (pMarket[i].playerid_ != pMarket[i].bidplayerid_)
						{//���ɹ�����¼���ҽ�Ǯ�仯��־
							officer.setmasterid(pMarket[i].playerid_);
							officer.setdetailid(pMarket[i].playerid_);
							g_Proxy.db_select(officer);
							ISystemLog::getInstance()->playerMoney_SaleHeroEquipFolk_Log(officer.playerid_,officer.officername_,
								curmoney - tmpmoney,curmoney,tmpmoney,pMarket[i].mapid_,pMarket[i].cityid_,
								iEquipment.getPostion(),pMarket[i].itemid_,pMarket[i].count_);
							if (ret)
							{
								officer.setmasterid(lBuyPlayerID);
								officer.setdetailid(lBuyPlayerID);
								g_Proxy.db_select(officer);
								ISystemLog::getInstance()->playerEquip_BuyFolk_Log(officer.playerid_,officer.officername_,
									iEquipment.getPostion(),pMarket[i].itemid_,1);
							}
						}
						else
						{
							officer.setmasterid(lBuyPlayerID);
							officer.setdetailid(lBuyPlayerID);
							g_Proxy.db_select(officer);
							ISystemLog::getInstance()->playerEquip_SaleFailFolk_Log(officer.playerid_,officer.officername_,
								iEquipment.getPostion(),pMarket[i].itemid_,1);
						}
					}
					break;
				case EN_BASIC_RESOURCE:			//��������
					ret = OnUpDataResource(handle, pMarket[i].itemid_, pMarket[i].count_, lBuyPlayerID, lMapID, lCityID);
					if (pMarket[i].playerid_ != pMarket[i].bidplayerid_)
					{//���ɹ�����¼���ҽ�Ǯ�仯��־
						officer.setmasterid(pMarket[i].playerid_);
						officer.setdetailid(pMarket[i].playerid_);
						g_Proxy.db_select(officer);
						ISystemLog::getInstance()->playerMoney_SaleResFolk_Log(officer.playerid_,officer.officername_,
							curmoney - tmpmoney,curmoney,tmpmoney,pMarket[i].mapid_,pMarket[i].cityid_,
							pMarket[i].itemid_,pMarket[i].count_);
						if (ret)
						{
						}
					}
					else
					{
					}
					break;
				};			
			}
			pMarket[i].setmasterid(iCountry,iType);
			pMarket[i].setdetailid(pMarket[i].numid_);
			g_Proxy.db_delete(pMarket[i]);
			if (pMarket[i].playerid_ != pMarket[i].bidplayerid_)
			{//����
				g_DataServer.broadMsgAndNotify(pMarket[i].playerid_,MDM_GP_MAP_MARKET,pMarket[i].itemid_,pMarket[i].count_,pMarket[i].type_,EN_S_NOTIFYMARKET,0,MDM_GP_MAP_MARKET);
			}
			continue;
		}
		(*pGoodsList)[n].lSortID = pMarket[i].numid_;
		(*pGoodsList)[n].iSaleID = pMarket[i].itemid_;
		(*pGoodsList)[n].iCount = pMarket[i].count_ ;
		(*pGoodsList)[n].iMinPrice = pMarket[i].minprice_;
		(*pGoodsList)[n].iCurrentPrice = pMarket[i].currentprice_;
		(*pGoodsList)[n].iOncePrice = pMarket[i].onceprice_;
		(*pGoodsList)[n].iLeavingTime = TimeLen;
		(*pGoodsList)[n].iTax = 10;
		strcpy((*pGoodsList)[n].szPlayerName, pMarket[i].playername_);
		if (n++>=29) break;
	}
	int iPage = 0;
	if (n<=0) 
	{
		//add by xujincai 20081228,��ѯ���Ϊ��ʱ�����ӷ��ء�
		iPage += ith->second.iStart/29;
		pGoodsList->serialize(n, iType, iPage, MDM_GP_MAP_MARKET, lPlayerID);
		asynch_write(handle, (char *)pGoodsList, pGoodsList->length);
	}
	else
	{
		if (iNext == 1)
		{
			if (n<29)
				iPage = 1;
			ith->second.iStart += n;
		}
		else
			ith->second.iStart = n;
		iPage += ith->second.iStart/29;
		pGoodsList->serialize(n, iType, iPage, MDM_GP_MAP_MARKET, lPlayerID);
		int ret = asynch_write(handle, (char *)pGoodsList, pGoodsList->length);
	}
	if (Buffer != NULL)
	{
		delete [] Buffer;
		Buffer = 0;
	}
	if(tmp)
	{
		delete [] tmp;
		tmp = 0;
	}
}
//����
int CMarketManager::OnBuy(unsigned long lPlayerID, int iMoney,int &curMoney)
{
	PlayerChatRoomTable Money;
	int iRate = 0;
	memset(&Money, 0, sizeof(PlayerChatRoomTable));
	int playermoney = int(iMoney*0.9);
	Money.setid(lPlayerID);
	Money.money_ += playermoney;
	iRate = (iMoney - Money.money_)/2;
	Money.setid(lPlayerID);
	if (g_Proxy.db_increase(Money) == DB_Proxy::DB_SUCCEED)
	{
		curMoney = Money.money_;
		//50%��˰�ռ������ϵͳ
		if(iRate >0)
		{
			Notify_MarketRate_Request notifymsg;
			notifymsg.serialize(0,lPlayerID,iRate);
			event_notify(MDM_GP_MAP_CONSORTIA,(char*)&notifymsg,notifymsg.length);
		}
		return playermoney;
	}
	return 0;
}
void CMarketManager::OnExitMapServer(unsigned long lPlayerID)
{
	/*
	map<unsigned long ,tagPlayerInfo>::iterator ith = m_mapPlayerInfo.find(lPlayerID);
	if (ith != m_mapPlayerInfo.end())
	{
		m_mapPlayerInfo.erase(ith);
	}
	*/
	m_mapPlayerInfo.erase(lPlayerID);
}
void CMarketManager::InitMarkPrice()
{	
	FILE *fp = 0;
	char buf[64] = {0};
	fp = fopen("configtable/MarkPrice.csv","r");
	if(fp == NULL)
		return;

	fgets(buf,64,fp);
	int iMode =0;
	int iItemID = 0;
	int iPrice = 0;
	while(!feof(fp))
	{
		if(fgets(buf,64,fp) == NULL)
			break;
		if(sscanf(buf,"%d,%d,%d",&iMode,&iItemID,&iPrice) <3)
			continue;
		m_MarkPrice[iMode][iItemID] = iPrice;
	}
	fclose(fp);
}