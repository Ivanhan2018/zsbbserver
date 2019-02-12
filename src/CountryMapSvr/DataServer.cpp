#include "DataServer.h"
#include "socketsvc.hpp"
#include "ConsortiaProtocol.h"
#include "log.hpp"
//#include "LogicSystem.h"
#pragma warning(disable:4100)
#pragma warning(disable:4189)
#pragma warning(disable:4127)
#pragma warning(disable:4244)
#pragma warning(disable:4389)
#pragma warning(disable:4800)
CDataServer g_DataServer ;
IFightProcesser *g_IFightProcesser = 0;

CDataServer::CDataServer(void)
{
}

CDataServer::~CDataServer(void)
{
}
bool CDataServer::ReadPacket(char* block , PROTOCOL_FIGHT* cmdID , void * buffer )
{
	return true ;
}

void CDataServer::GetData(char * block)
{
	PROTOCOL_FIGHT Cmd = static_cast<PROTOCOL_FIGHT>(0) ;
	DISTRIBUTED_HEADER* head = (DISTRIBUTED_HEADER*)block;
	bool bIsFightMsg = false;
	if (head->length <27)
	{
		printf("packet error from %d to %d command %d",head->from, head->to ,head->command);
		return ;
	}
	else
	{
		switch (head->command)
		{
		/***************************map info*********************************************/
		case EN_C_VALIDATE:					//��֤
			MapServerInfo(head->agent, block ,head->length, head->from);
			break;
		case EN_C_COMEIN_MAP:				//���뿤��
			m_LevySoldier.MapOfArmys(head->agent, block ,head->length, head->from);
			bIsFightMsg = true;
			break;
		case EN_C_LEVY_SOLDIER:				//������
			{
				m_LevySoldier.OnLevySoldier(head->agent, block ,head->length, head->from);
			}
			break;
		case EN_C_MODIFY_OFFICER_NAME:			//�޸�ָ�ӹ�����
			m_OtherManager.OnModifyOfficerName(head->agent, block ,head->length, head->from);
			break;
		case EN_C_RECRUIT_OFFICER:				//��ļָ�ӹ�
			m_OtherManager.OnRecruitOfficer(head->agent,head->from);
			break;
		case EN_C_GET_OFFICER_TECH:					//�õ�ָ�ӹټ���
			m_OtherManager.onOfficerTechList(block);
			break;
		case EN_C_STUDY_OFFICER_TECH:				//ѧϰָ�ӹټ���
			m_OtherManager.onOfficerStudyTech(block);
			break;
		case EN_C_GET_OFFICERS:						//ָ�ӹ��б�
			m_OtherManager.OnOfficerList(head->agent,head->from);
			break;
		case EN_C_OFFICERO_ATTRIBUTE:					//�õ�Ӣ������
			m_OtherManager.GetOfficerAttribute(head->agent, block ,head->length, head->from);
			break;
		case EN_C_DISMISS_HERO:					//����Ӣ��
			m_OtherManager.OnDismissHero(head->agent, block ,head->length, head->from);
			break;
		case EN_C_REDEEM_HERO:						//���Ӣ��
			m_OtherManager.RedeemHero(head->agent, block ,head->length, head->from);
			break;
		case EN_C_EQUIPS_LIST:				//�õ�����װ��
			m_OtherManager.OnGetHeroEquip(head->agent, block ,head->length, head->from);
			break;
		case EN_C_PACK_EXCHANGE:			//����װ��
			//m_OtherManager.OnExchangeHeroEquip(head->agent, block ,head->length, head->from);
			m_OtherManager.OnExchangeHeroEquipEx(block);
			break;
		case EN_C_DESTROY_EQUIP:				//���ٱ����е�װ��
			m_OtherManager.DestroyPackEquip(head->agent, block ,head->length, head->from);
			break;
		case EN_C_HERO_WAR_RECORD:				//�õ�Ӣ��ս����¼
			m_OtherManager.OnHeroWarRecord(head->agent, block ,head->length, head->from);
			break;
		case EN_C_HERO_GROWUP_RECORD:			//�õ�Ӣ�۳ɳ���¼
			m_OtherManager.OnHeroGrowUpRecord(head->agent, block ,head->length, head->from);
			break;
		case EN_C_LEVY_SOLDIER_COUNT:		//��������
			m_LevySoldier.OnLevySoldierCount(head->agent, block ,head->length, head->from);
			break;
		case EN_C_EXE_SOLDIER:				//ѵ��������
			{
				//m_LevySoldier.OnExeSoldier(handle, pDataBuffer ,iDataSize, lPlayerID);
			}
			break;
		case EN_C_ARMY_CREATE:				//���Ӵ���			
			m_LevySoldier.OnArmyCreate(head->agent, block ,head->length, head->from);
			break;
		case EN_C_CREATE_ARMY:				//��������
			{
				m_LevySoldier.ReqCreateArmy((CREATE_ARMY*)block);
			}
			break;
		case EN_C_ARMY_EDIT:				//�������
			m_LevySoldier.OnArmyEdit(head->agent, block ,head->length, head->from);
			break;
		case EN_C_EDIT_ARMY:				//��ಿ��
			if(m_LevySoldier.OnEditArmy(head->agent, block ,head->length, head->from) == true)
			{
				bIsFightMsg = true;
			}
			break;
		case EN_C_GET_ARMY_INFO:			//�õ�������Ϣ
			{
				m_LevySoldier.OnGetArmyInfo(head->agent, block ,head->length, head->from);
			}
			break;
		case EN_C_ARMYS_TRANSFER:			//����ת��
			m_OtherManager.OnArmyTransfer(head->agent, block ,head->length,  SERVER_BOUND_MAP_OTHER, head->from);
			break;
		case EN_C_EXE_SOLDIER_COUNT:		//ѵ��������
			{
				//m_LevySoldier.OnExeSoldierCount(handle, pDataBuffer ,iDataSize, lPlayerID);
			}
			break;
		case EN_C_GET_CONSTRUCTION_COUNT:		//����������
			{
				m_BuildProduce.OnBuildConstruction(head->agent, block ,head->length, head->from);
				break;
			}
		case EN_C_BUILD_CONSTRUCTION:			//����������
			m_BuildProduce.OnStartBuildCon(head->agent, block ,head->length, head->from);
			break;
		case EN_C_DEMOLISH_CONSTRUCTION:		//��ٽ�����
			m_BuildProduce.OnDemolishConstruction(head->agent, block ,head->length);
			break;
		case EN_C_DEMOLISH_A_CONSTRUCTION:		//���A������
			m_BuildProduce.OnDemolishAConstruction(head->agent, block ,head->length, head->from);
			break;
		case EN_C_CANCEL_BUILD:					//ȡ�������ｨ��
			m_BuildProduce.OnCancelBuild(head->agent, block ,head->length, head->from);
			break;
		case EN_C_PRODUCE_WEAPON_COUNT:			//��������
			{
				m_BuildProduce.WeaponProduce(head->agent, block ,head->length, head->from);
			}
			break;
		case EN_C_PRODUCE_WEAPON:				//��������
			{
				m_BuildProduce.OnProduceEvent(head->agent, block ,head->length, head->from, WEAPON_ITEM, MAX_WEAPON_LEVEL, EN_TIMER_PRODUCE_WEAPON);
			}
			break;
		case EN_C_PRODUCE_DEFEND_COUNT:			//��������
			m_BuildProduce.DefendProduce(head->agent, block ,head->length, head->from);
			break;
		case EN_C_PRODUCE_DEFEND:				//��������
			m_BuildProduce.OnProduceEvent(head->agent, block ,head->length, head->from, DEFEND_ITEM, MAX_DEFEND_LEVEL, EN_TIMER_PRODUCE_DEFEND);
			break;
		case EN_C_PRODUCE_SPECIAL_COUNT:		//���⹤����������
			m_BuildProduce.SpecialProduce (head->agent, block ,head->length, head->from);
			break;
		case EN_C_PRODUCE_SPECIAL:				//�������⹤��
			m_BuildProduce.OnProduceEvent(head->agent, block ,head->length, head->from, SPECIAL_TOOL_ITEM, MAX_SPECIAL_LEVEL, EN_TIMER_PRODUCE_SPECIAL);
			break;
		case EN_C_PRODUCE_ANIMAL_COUNT:			//ѱ����������
			m_BuildProduce.AnimalProduce (head->agent, block ,head->length, head->from);
			break;
		case EN_C_PRODUCE_ANIMAL:				//����ѱ��
			m_BuildProduce.OnProduceEvent(head->agent, block ,head->length, head->from, ANIMALS_TAME_ITEM, MAX_ANIMALS_LEVEL, EN_TIMER_PRODUCE_ANIMAL);
			break;
		case EN_C_CANCEL_PRODUCE:				//ȡ����������
			m_BuildProduce.OnCancelProduce(head->agent, block ,head->length, head->from);
			break;
		case EN_C_CANCEL_RESEARCH:				//ȡ�����е���
			m_ResearchManager.OnCancelResearchItem(head->agent, block ,head->length, head->from);
			break;
		case EN_C_RESEARCH_ITEM:					//�о�����
			m_ResearchManager.OnResearchItem(head->agent, block ,head->length, head->from);
			break;
		case EN_C_GET_ALL_RESEARCH_ITEM:			//�õ������Ѿ��о��˵ĵ����б�
			m_ResearchManager.OnGetAllResearchList(head->agent,  head->from);
			break;
		case EN_C_GET_CITY_INFO:			//��ȡ������Ϣ
			m_LevySoldier.OnCityInfo(head->agent, block, head->length, head->from);
			break;
		case EN_C_COUNTRY_MARKET:				//�����г�
			m_MarketManager.CountryMarket(head->agent, block,  head->length, head->from);
			break;
		case EN_C_COUNTRY_MARKET_BUSINESS:			//�����г�
			m_MarketManager.CountryMarketBusiness(head->agent, block,  head->length, head->from);
			break;
		case EN_C_MARKET_WEAPON_LIST:				//�����б�
			m_MarketManager.OnWeaponList(head->agent, block,  head->length, head->from);
			break;
		case EN_C_MARKET_DEFEND_LIST:				//�����б�
			m_MarketManager.OnDefendList(head->agent, block,  head->length, head->from);
			break;
		case EN_C_MARKET_OFFICER_ITEM_LIST:			//ָ�ӹٵ����б�
			m_MarketManager.OfficerItemList(head->agent, block,  head->length, head->from);
			break;
		case EN_C_BASE_RESOURCE_LIST:				//������Դ�б�
			m_MarketManager.PlayerResourceCount(head->agent, block, head->from);
			break;
		case EN_C_MARKET_GOODS_LIST:				//��Ʒ�б�
			m_MarketManager.PeopleMarket(head->agent, block, head->length, head->from);
			break;
		case EN_C_MARKET_GOODS_NEXT:				//��Ʒ�б���һҳ
			m_MarketManager.PeopleMarket(head->agent, block, head->length, head->from,1);
			break;
		case EN_C_MARKET_PUTUP:					//����
			m_MarketManager.OnPutUp(head->agent, head->from, block, head->length);
			break;
		case EN_C_MARKET_BIDDEN:					//����
			m_MarketManager.OnBidden(head->agent, head->from, block, head->length);
			break;
		case EN_C_MARKET_BUY:						//һ�ڼ�
			m_MarketManager.OnOnePrice(head->agent, head->from, block, head->length);
			break;
			//ս��Ʒ����
		case EN_C_VENDUE_GOODS_LIST:				//��Ʒ�б�
			m_MarketManager.VendueGoods(head->agent, block, head->length, head->from);
			break;
		case EN_C_VENDUE_GOODS_NEXT:			//��Ʒ�б���һҳ
			m_MarketManager.VendueGoods(head->agent, block, head->length, head->from,1);
			break;
		case EN_C_VENDUE_BIDDEN:					//����
			m_MarketManager.OnBidden(head->agent, head->from, block, head->length, 10);
			break;
		case EN_C_VENDUE_BUY:						//һ�ڼ�
			m_MarketManager.OnOnePrice(head->agent, head->from, block, head->length, 10);
			break;
		case EN_C_CONTRIBUTE_INFO:					//������Ϣ
			m_BuildProduce.OnContributeInfo(head->agent, block,  head->length, head->from);
			break;
		case EN_C_CONTRIBUTE:						//����
			m_BuildProduce.Contribute(head->agent, block,  head->length, head->from);
			break;
		case EN_C_CREATE_OFFICER:					//����ָ�ӹ�
			m_OtherManager.OnCreateOfficer(head->agent, block,  head->length, head->from);
			break;
		case EN_C_LOAD_MATERIAL:				//װ������
			m_LevySoldier.OnLoadMaterial(head->agent, block, head->length, head->from);
			break;
		case EN_C_UNINSTALL_MATERIAL:		//ж������
			m_LevySoldier.OnUninstallMaterial(head->agent, block, head->length, head->from);
			break;
		case EN_C_LOOK_MATERIAL:				//���ʲ鿴
			m_LevySoldier.OnLookMaterial(head->agent, block, head->length, head->from);
			break;
		case EN_C_COLONIZE_CITY:				//ֳ�����
			m_LevySoldier.OnColonizeCity(head->agent, block, head->length, head->from);
			break;
		case EN_C_EXIT_MAP_SERVER:						//�˳���Ϸ
			OnExitServer(head->from);
			break;
		case CMD_END_FIGHT:								//����ս��
			m_BuildProduce.OnMapEnFight( block,  head->length);
			m_OtherManager.OnMapEnFight( block,  head->length);
			break;
		case CMD_ARMY_REACH:							//���ӳ���
			m_OtherManager.OnOutArmyEx( block,  head->length,0,false);//modify by xujincai 20081224
			break;
		case CGC_C_NOTITY_BUILDCONSTRUCTION:	//֪ͨ���콨���������й���
			m_BuildProduce.ConstrAutoBulid( block,  head->length);
			break;
		case CGC_C_NOTIFY_INCREASESPEED:		//����֪ͨ
			g_SpeedCard.IncreaseSpeed( block,  head->length);
			break;
		case CGC_C_NOTIFY_NORMALSPEED:			//�ָ������ٶ�
			g_SpeedCard.NormalSpeed( block,  head->length);
			break;
		case CGC_C_NOTIFY_COLLECTRESOURCE:		//�ռ���Դ
			m_BuildProduce.OnCollectResource( block,  head->length);
			break;
		case CGC_C_NOTIFY_INCREASEOTHERSPEED:
			g_DataServer.OnOnceSpeed(block,  head->length);
			break;
		case CMD_ARMY_REACH_OK:
			m_OtherManager.OnArmyReachOKEx(block,  head->length);
			break;
			//add by xujincai 20081217
		case EN_C_UNLAYARMY://��ɢ����
			{
				m_LevySoldier.ReqUnlayArmy(block);
			}
			break;
		case EN_C_DELETEARMY://ɾ������,�����ѹ��ս���߳�
			{
				if(m_LevySoldier.ReqDeleteArmy(block) ==0)
				{
					bIsFightMsg = true;
				}
			}
			break;
		case EN_C_NEWTREASUREPACK://��ӱ���
			{
				m_OtherManager.OnNewTreasurePack(block);
			}
			break;
		case EN_C_OPENTREASUREPACK://�򿪱���
			{
				m_OtherManager.OnOpenTreasurePack(block);
			}
			break;
		case EN_C_HEROGBH_NOTIFY://Ӣ������֪ͨ��Ϣ
			{
				m_OtherManager.OnHeroGBHNotify(block);
			}
			break;
		case EN_C_IMMEDCOMPLETE://�������
			{
				OnImmedComplete(block);
			}
			break;
		case CGC_S_NOTIFY_IMMEDCOMPLETE://�շ�ϵͳ֪ͨ
			{
				OnImmedCompleteNotifyMsg(block);
			}
			break;
		case EN_C_TIMER_NOTIFY://��ʱ��
			{
				TIMER_HEADER *pTimerHeader = (TIMER_HEADER*)block;
				if(OnSetTimerEvent(pTimerHeader->timer_command,block,pTimerHeader->length) !=0)
				{
					bIsFightMsg = true;
				}
			}
			break;
		case EN_C_RESETMAPINFO://���õ�ͼ����
			{
				LPResetMapInfo_Request resetmap_msg = (LPResetMapInfo_Request)block;
				m_LevySoldier.OnResetMapInfo(resetmap_msg);
				m_BuildProduce.OnResetMapInfo(resetmap_msg);
			}
			break;
		case EN_C_GETARMYLIST://��ȡ������в����б�
			{
				m_OtherManager.OnGetArmyList(block);
			}
			break;
		case EN_C_MODIFYARMYSTATE://ս�����������͵Ĳ���״̬�ı����Ϣ
			{
				m_OtherManager.OnModifyArmyState(block);
			}
			break;
		case CC_C_NOTIFY_CREATETIRRIER://֪ͨ����ͼ����������
			{
				m_LevySoldier.ReqCreateTirrier(block);
			}
			break;
		case CC_C_NOTIFY_TIRRIERGOOUT://֪ͨ���ͼ����������������
			{
				m_OtherManager.OnTirrierGoOut(block);
			}
			break;
		case CC_C_NOTIFY_UPGRADEBUILD://֪ͨ����ͼ������������
			{
				m_BuildProduce.OnUpgrateBuild(block);
			}
			break;
		case EN_C_DESTROYCITY://��Ҳ���ݵ�
			{
				m_BuildProduce.OnDestroyCity(block);
			}
			break;
		case EN_C_QUERYCAPTIVELIST://��ѯ��²�б�
			{
				m_OtherManager.OnQueryCaptiveList(block);
			}
			break;
		case EN_C_CREATENPCARMY://����NPC����
			{
				m_OtherManager.NotifyCreateNPCArmy(block);
			}
			break;
		case EN_C_NOTIFY_MAPSTATE://��״̬�ı�
			{
				m_OtherManager.OnNotifyMapState(block);
			}
			break;
		case EN_C_INLAYEQUIP://������Ʒ
			{
				m_OtherManager.onInlayEquipment(block);
			}
			break;
		case CGC_C_NOTIFY_ADDOFFICERSKILL://�շ�ϵͳ֪ͨʹ�ü��ܿ�ѧϰ���⼼��
			{
				m_OtherManager.onNotifyAddOfficerSkill(block);
			}
			break;
		case CGC_C_NOTIFY_BUYCARD://�շ�ϵͳ�򿨣�֪ͨ���뱳��
			{
				m_OtherManager.onNotifyBuyCard(block);
			}
			break;
		case EN_C_CONVERT:
			{
				m_OtherManager.onConvert(block);
#ifdef _DEBUG
				printf("���յ����%d�ľ��ƶһ���Ϣ��\n",head->from);
#endif
			}
			break;
		case EN_C_COMMONOPENCARD://ͳһ�Ĵ򿪿��ӿڣ�ֻ�������Ҽ�����ֱ�Ӵ����͵Ŀ���
			{
				m_OtherManager.onCommonOpenCard(block);
			}
			break;
		case EN_C_MODIFYHERONAME://������
			{
				m_OtherManager.onModifyHeroName(block);
			}
			break;
		case EN_S_NOTIFYPLAYERONLINE://֪ͨ�Ƿ�����
			{
				onNotifyPlayerOnline(block);
			}
			break;
		case EN_C_RELEASECAPTIVE://�ͷ�Ӣ��
			{
				m_OtherManager.releaseCaptive(block);
			}
			break;
		default:
			bIsFightMsg = true;
			break;
		}
	}
	if(bIsFightMsg)
	{
		g_IFightProcesser->pushDataToManager(block);
	}
	else
	{
		delete [] block;
		block = 0;
	}
}

void CDataServer::InitInfo()
{
	m_OtherManager.loadBinConfig();
	m_OtherManager.ReadRaceGeniusInfo();
	m_OtherManager.ReadCityInfo();
	m_OtherManager.loadInitConstructions();
	m_BuildProduce.ReadProduceState();//��ȡ��Դ�ɼ����ٲ���

	m_LevySoldier.ReadCityInfo();
	m_LevySoldier.ReadRaceParam();

	m_ResearchManager.ReadWeaponConfig(WEAPON_ITEM);
	m_ResearchManager.ReadWeaponConfig(DEFEND_ITEM);
	m_ResearchManager.ReadWeaponConfig(ANIMALS_TAME_ITEM);
	m_ResearchManager.ReadWeaponConfig(SPECIAL_TOOL_ITEM);
	m_MarketManager.InitMarkPrice();

}
int CDataServer::OnSetTimerEvent(int iCommand,void * pDataBuffer, int iDataSize)
{
	switch(iCommand)
	{
	//ʱ���¼�
	case EN_TIMER_ARMY_OUT:							//���Ӵﵽʱ���¼�
		m_OtherManager.ArmyOutTimeEventEx(pDataBuffer, iDataSize);
		break;
	case EN_TIMER_CONSTRUCTION:						//����������
		m_BuildProduce.ConTimeEvent(pDataBuffer, iDataSize);
		break;
	case EN_TIMER_PRODUCE_WEAPON:						//��������
		m_BuildProduce.OnProduceTimeEvent(pDataBuffer, iDataSize);
		break;
	case EN_TIMER_PRODUCE_DEFEND:						//��������
		m_BuildProduce.OnProduceTimeEvent(pDataBuffer, iDataSize);
		break;
	case EN_TIMER_PRODUCE_ANIMAL:						//����ѱ�޼���
		m_BuildProduce.OnProduceTimeEvent(pDataBuffer, iDataSize);
		break;
	case EN_TIMER_PRODUCE_SPECIAL:						//�������⼼��
		m_BuildProduce.OnProduceTimeEvent(pDataBuffer, iDataSize);
		break;
	case EN_TIMER_RESEARCH:								//���е��߼���
		m_ResearchManager.OnResearchTimeEvent(pDataBuffer, iDataSize);
		break;
	case EN_TIMER_LEVYING:								//�б�
		m_LevySoldier.OnLevyTimeEvent(pDataBuffer, iDataSize);	
		break;
	case EN_TIMER_RECRUITING_HERO:								//��ļӢ��ʱ���¼�
		m_OtherManager.OnRecruitHeroEvent(pDataBuffer, iDataSize);	
		break;
	case EN_TIMER_ARMY_WARM:						//������Ϣ
		m_OtherManager.ArmyWarmTimeEvent(pDataBuffer, iDataSize);
		break;
	case EN_TIMER_HERO_RESUME://Ӣ�ۻָ�
		{
			m_OtherManager.HeroResumeTimerEvent(pDataBuffer);
		}
		break;
	case EN_TIMER_CREATENPCARMY:
		{
			m_OtherManager.CreateNPCArmyTimer((char*)pDataBuffer);
		}
		break;
	default:
		return 1;
		break;
	}
	return 0;
}
//��ͼ��������Ϣ
bool CDataServer::MapServerInfo(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID)
{
	MAP_SERVER_VALIDATE *pServerValidate = (MAP_SERVER_VALIDATE *)pDataBuffer;
	//��Ϸ�е�ʱ��
	IceUtil::Time begin = IceUtil::Time::Date(2000, 1, 1);
	IceUtil::Time now = IceUtil::Time::now();
	long long lGameTime = now.toSeconds() - begin.toSeconds();

	GAME_TIME GameTime;
	GameTime.serialize(lGameTime, SERVER_BOUND_MAP_OTHER, lPlayerID, EN_S_GAME_TIME);
	asynch_write(handle, (char *)&GameTime, GameTime.length);

    int ret = m_OtherManager.MapStateList(handle, lPlayerID);
	if (ret == 1)
		ret = m_OtherManager.PlayerBaseInfo(handle, lPlayerID);
	if (ret == 1)
		ret = m_OtherManager.PlayerCityInfo(handle, lPlayerID);
	if (ret == 1)
		m_OtherManager.OnOutingArmysListEx(handle, SERVER_BOUND_MAP_OTHER, lPlayerID);//modify by xujincai 20090106

	return true;
}
void CDataServer::OnExitServer(unsigned long lPlayerID)
{
	setAgentOfPlayer(lPlayerID,0);
	m_OtherManager.notifyPlayerState2OtherSvr(lPlayerID,0);
	//�����˳���Ϸ֪ͨ���������������С��г�
	//m_MarketManager.OnExitMapServer(lPlayerID);
}
//����ս��
void CDataServer::OnEndFightEvent(const void * pDataBuffer, unsigned int length)
{
//	EndFight * pData = (EndFight *)pDataBuffer;

	m_BuildProduce.OnMapEnFight(pDataBuffer, length);
	m_OtherManager.OnMapEnFight(pDataBuffer, length);
}
void CDataServer::OnOnceSpeed(void * pDataBuffer, int iDataSize)
{
	tagNotify_IncreaseOtherSpeed_Request *pSpeed = (tagNotify_IncreaseOtherSpeed_Request*)pDataBuffer;
	switch(pSpeed->Type)
	{
	case Construction_Speed_Card:
		m_BuildProduce.OnConstrSpeed(pSpeed->PlayerID,pSpeed->CityID, pSpeed->SortID, pSpeed->Value);
		break;
	case Recruit_Card:
		m_OtherManager.OnRecruitSpeed(pSpeed->PlayerID, pSpeed->Value);
		break;
	case Produce_Speed_Card:
		break;
	case Conscription_Speed_Card:
		m_LevySoldier.OnLevySpeed(pSpeed->PlayerID, pSpeed->SortID, pSpeed->Value);
		break;
	case Research_Speed_Card:
		m_ResearchManager.OnResearchSpeed(pSpeed->PlayerID, pSpeed->SortID, pSpeed->Value);
		break;
	};
}

void CDataServer::OnImmedComplete(void *msg)
{
	LPImmedComplete_Request req_msg = (LPImmedComplete_Request)msg;
	if(req_msg->length != sizeof(ImmedComplete_Request))
	{
		LOG_TRACE_EX(false,("CDataServer::OnImmedComplete-->��Ϣ���Ƿ�:��ң�%d\n",req_msg->from));
		return;
	}
	switch(req_msg->Type)
	{
	case Construction_Speed_Card:
	case Produce_Speed_Card:
		m_BuildProduce.OnImmedComplete(req_msg);
		break;
	case Recruit_Card:
		m_OtherManager.OnImmedComplete(req_msg);
		break;
	case Conscription_Speed_Card:
		m_LevySoldier.OnImmedComplete(req_msg);
		break;
	case Research_Speed_Card:
		m_ResearchManager.OnImmedComplete(req_msg);
		break;
	};
}
//�����շ�ϵͳ�۷Ѻ��֪ͨ��Ϣ
void CDataServer::OnImmedCompleteNotifyMsg(void *msg)
{
	LPNotify_ImmedComplete_Response req_msg = (LPNotify_ImmedComplete_Response)msg;
	if(req_msg->length != sizeof(Notify_ImmedComplete_Response))
	{
		LOG_TRACE_EX(false,("CDataServer::OnImmedCompleteNotifyMsg-->��Ϣ���Ƿ�:���%d\n",req_msg->from));
		return;
	}
	switch(req_msg->Param.Type)
	{
	case Construction_Speed_Card:
	case Produce_Speed_Card:
		m_BuildProduce.OnImmedCompleteNotifyMsg(req_msg);
		break;
	case Recruit_Card:
		m_OtherManager.OnImmedCompleteNotifyMsg(req_msg);
		break;
	case Conscription_Speed_Card:
		m_LevySoldier.OnImmedCompleteNotifyMsg(req_msg);
		break;
	case Research_Speed_Card:
		m_ResearchManager.OnImmedCompleteNotifyMsg(req_msg);
		break;
	};
}



int CDataServer::AddArmyInfo(int type ,const char *armyname,int playerid,int officerid,int raceid,int weapon,int weaponlevel,int defendtype,int defendlevel,int soldiertype,int soldiercount)
{
	ArmyTable Table;
	ARMY_ADD armyadd ;
	memset(&armyadd,0,sizeof(ARMY_ADD));
	Table.cityid_ = 0;
	Table.countryid_ = g_MapInfo->GetMapOwer();
	Table.direction = 1;
	Table.fdefendlevel_ =defendlevel;
	Table.fdefendtype_ = defendtype;
	Table.firstcount_ =soldiercount;
	Table.fsexp_ =0;
	Table.fsoldierlevel_ =1;
	Table.fweapon_ = weapon;
	Table.fweaponlevel_ = weaponlevel;
	Table.mapid_ = g_MapInfo->getMapID();
	Table.officerid_ =1;
	Table.soldiertype_ = soldiertype;
	Table.raceid_ = raceid;
	Table.playerid_ = playerid;
	Table.officerid_ = officerid;

	if(type ==1)
	{
		strcpy(Table.armyname_,armyname);
	}
	else if(type ==2)
	{
		if(g_MapInfo->GetMapOwer() ==1)
		{
			strcpy(Table.armyname_,"���˹�����");
			Table.raceid_ = 1;
		}
		else
		{
			Table.raceid_ = 5;
			strcpy(Table.armyname_,"���������");
		}
	}

	Table.state_ = 0;
	POS_STRUCT pPos;
	pPos = g_MapInfo->GovPos.GovPos.front();
	Table.x = pPos.PosX;
	Table.y = pPos.PosY;
	int ret = g_Proxy.db_insert(Table);
	if (ret != DB_Proxy::DB_SUCCEED)
	{
		return 1;
	}	

	m_LevySoldier.AddArmyInfo2Fight(0,Table.playerid_,Table.mapid_,Table.armyid_);
	return 0;

}
int CDataServer::getAgentOfPlayer(int playerid)
{
	std::map<int,int>::const_iterator iter;
	iter = m_onLinePlayerMap.find(playerid);
	if (iter == m_onLinePlayerMap.end())
	{
		return 0;
	}
	return iter->second;
}
void CDataServer::setAgentOfPlayer(int playerid,int agent)
{
	if (agent >0)
	{
		m_onLinePlayerMap[playerid] = agent;
	}	
	else
	{
		m_onLinePlayerMap.erase(playerid);
	}
}
void CDataServer::onNotifyPlayerOnline(const char *msg)
{
	LPNotifyPlayerOnline req_msg = (LPNotifyPlayerOnline)msg;
	if(req_msg->agent <=0 && req_msg->to == MDM_GP_MAP_MARKET)
	{
		m_MarketManager.OnExitMapServer(req_msg->playerID);
	}
	setAgentOfPlayer(req_msg->playerID,req_msg->agent);
}
void CDataServer::broadMsgAndNotify(int playerid,int mapid,__int64 itemid,short level,short type,int notifycmd,int broadcmd,int svrid)
{
	//�·������֪ͨ��Ϣ
	if (notifycmd > 0)
	{
		int agent = g_DataServer.getAgentOfPlayer(playerid);
		if(agent >0)
		{
			BaseNotifyPlayerMsg notifyPlayer;
			notifyPlayer.serialize(agent,playerid,mapid,itemid,level,type,notifycmd,svrid);
			asynch_write(agent,(char*)&notifyPlayer,notifyPlayer.length);
		}
	}
	if (broadcmd > 0)
	{
		//���͹㲥��Ϣ
		BroadMsgParam *broadMsgParam = g_LoadDBinfo.getBroadMsgParam(broadcmd);
		if (broadMsgParam == NULL)
		{
			return;
		}
		if (level >= broadMsgParam->minLevel && level <= broadMsgParam->maxLevel)
		{
			PlayerChatRoomTable playerInfo;
			playerInfo.setid(playerid);
			if (g_Proxy.db_select(playerInfo) != DB_Proxy::DB_SUCCEED)
			{
				return;
			}
			PlayerOfficerTable officer;
			officer.setmasterid(playerid);
			officer.setdetailid(playerid);
			if (g_Proxy.db_select(officer) != DB_Proxy::DB_SUCCEED)
			{
				return;
			}
			BaseBroadMsg broadMsg;
			broadMsg.serialize(0,playerInfo.country_,playerid,officer.officername_,mapid,itemid,level,type,broadcmd,svrid);
			broadcast((char*)&broadMsg,broadMsg.length);
		}
	}
}
