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
		case EN_C_VALIDATE:					//验证
			MapServerInfo(head->agent, block ,head->length, head->from);
			break;
		case EN_C_COMEIN_MAP:				//进入郡内
			m_LevySoldier.MapOfArmys(head->agent, block ,head->length, head->from);
			bIsFightMsg = true;
			break;
		case EN_C_LEVY_SOLDIER:				//征兵处
			{
				m_LevySoldier.OnLevySoldier(head->agent, block ,head->length, head->from);
			}
			break;
		case EN_C_MODIFY_OFFICER_NAME:			//修改指挥官名字
			m_OtherManager.OnModifyOfficerName(head->agent, block ,head->length, head->from);
			break;
		case EN_C_RECRUIT_OFFICER:				//招募指挥官
			m_OtherManager.OnRecruitOfficer(head->agent,head->from);
			break;
		case EN_C_GET_OFFICER_TECH:					//得到指挥官技能
			m_OtherManager.onOfficerTechList(block);
			break;
		case EN_C_STUDY_OFFICER_TECH:				//学习指挥官技能
			m_OtherManager.onOfficerStudyTech(block);
			break;
		case EN_C_GET_OFFICERS:						//指挥官列表
			m_OtherManager.OnOfficerList(head->agent,head->from);
			break;
		case EN_C_OFFICERO_ATTRIBUTE:					//得到英雄属性
			m_OtherManager.GetOfficerAttribute(head->agent, block ,head->length, head->from);
			break;
		case EN_C_DISMISS_HERO:					//开除英雄
			m_OtherManager.OnDismissHero(head->agent, block ,head->length, head->from);
			break;
		case EN_C_REDEEM_HERO:						//赎回英雄
			m_OtherManager.RedeemHero(head->agent, block ,head->length, head->from);
			break;
		case EN_C_EQUIPS_LIST:				//得到所有装备
			m_OtherManager.OnGetHeroEquip(head->agent, block ,head->length, head->from);
			break;
		case EN_C_PACK_EXCHANGE:			//操作装备
			//m_OtherManager.OnExchangeHeroEquip(head->agent, block ,head->length, head->from);
			m_OtherManager.OnExchangeHeroEquipEx(block);
			break;
		case EN_C_DESTROY_EQUIP:				//销毁背包中的装备
			m_OtherManager.DestroyPackEquip(head->agent, block ,head->length, head->from);
			break;
		case EN_C_HERO_WAR_RECORD:				//得到英雄战争记录
			m_OtherManager.OnHeroWarRecord(head->agent, block ,head->length, head->from);
			break;
		case EN_C_HERO_GROWUP_RECORD:			//得到英雄成长记录
			m_OtherManager.OnHeroGrowUpRecord(head->agent, block ,head->length, head->from);
			break;
		case EN_C_LEVY_SOLDIER_COUNT:		//征兵数量
			m_LevySoldier.OnLevySoldierCount(head->agent, block ,head->length, head->from);
			break;
		case EN_C_EXE_SOLDIER:				//训练兵参数
			{
				//m_LevySoldier.OnExeSoldier(handle, pDataBuffer ,iDataSize, lPlayerID);
			}
			break;
		case EN_C_ARMY_CREATE:				//部队创建			
			m_LevySoldier.OnArmyCreate(head->agent, block ,head->length, head->from);
			break;
		case EN_C_CREATE_ARMY:				//创建部队
			{
				m_LevySoldier.ReqCreateArmy((CREATE_ARMY*)block);
			}
			break;
		case EN_C_ARMY_EDIT:				//部队组编
			m_LevySoldier.OnArmyEdit(head->agent, block ,head->length, head->from);
			break;
		case EN_C_EDIT_ARMY:				//组编部队
			if(m_LevySoldier.OnEditArmy(head->agent, block ,head->length, head->from) == true)
			{
				bIsFightMsg = true;
			}
			break;
		case EN_C_GET_ARMY_INFO:			//得到部队信息
			{
				m_LevySoldier.OnGetArmyInfo(head->agent, block ,head->length, head->from);
			}
			break;
		case EN_C_ARMYS_TRANSFER:			//部队转移
			m_OtherManager.OnArmyTransfer(head->agent, block ,head->length,  SERVER_BOUND_MAP_OTHER, head->from);
			break;
		case EN_C_EXE_SOLDIER_COUNT:		//训练兵数量
			{
				//m_LevySoldier.OnExeSoldierCount(handle, pDataBuffer ,iDataSize, lPlayerID);
			}
			break;
		case EN_C_GET_CONSTRUCTION_COUNT:		//建筑物数量
			{
				m_BuildProduce.OnBuildConstruction(head->agent, block ,head->length, head->from);
				break;
			}
		case EN_C_BUILD_CONSTRUCTION:			//建筑建筑物
			m_BuildProduce.OnStartBuildCon(head->agent, block ,head->length, head->from);
			break;
		case EN_C_DEMOLISH_CONSTRUCTION:		//拆毁建筑物
			m_BuildProduce.OnDemolishConstruction(head->agent, block ,head->length);
			break;
		case EN_C_DEMOLISH_A_CONSTRUCTION:		//拆毁A建筑物
			m_BuildProduce.OnDemolishAConstruction(head->agent, block ,head->length, head->from);
			break;
		case EN_C_CANCEL_BUILD:					//取消建筑物建筑
			m_BuildProduce.OnCancelBuild(head->agent, block ,head->length, head->from);
			break;
		case EN_C_PRODUCE_WEAPON_COUNT:			//武器生产
			{
				m_BuildProduce.WeaponProduce(head->agent, block ,head->length, head->from);
			}
			break;
		case EN_C_PRODUCE_WEAPON:				//生产武器
			{
				m_BuildProduce.OnProduceEvent(head->agent, block ,head->length, head->from, WEAPON_ITEM, MAX_WEAPON_LEVEL, EN_TIMER_PRODUCE_WEAPON);
			}
			break;
		case EN_C_PRODUCE_DEFEND_COUNT:			//生产防具
			m_BuildProduce.DefendProduce(head->agent, block ,head->length, head->from);
			break;
		case EN_C_PRODUCE_DEFEND:				//生产防具
			m_BuildProduce.OnProduceEvent(head->agent, block ,head->length, head->from, DEFEND_ITEM, MAX_DEFEND_LEVEL, EN_TIMER_PRODUCE_DEFEND);
			break;
		case EN_C_PRODUCE_SPECIAL_COUNT:		//特殊工具生产数量
			m_BuildProduce.SpecialProduce (head->agent, block ,head->length, head->from);
			break;
		case EN_C_PRODUCE_SPECIAL:				//生产特殊工具
			m_BuildProduce.OnProduceEvent(head->agent, block ,head->length, head->from, SPECIAL_TOOL_ITEM, MAX_SPECIAL_LEVEL, EN_TIMER_PRODUCE_SPECIAL);
			break;
		case EN_C_PRODUCE_ANIMAL_COUNT:			//驯兽生产数量
			m_BuildProduce.AnimalProduce (head->agent, block ,head->length, head->from);
			break;
		case EN_C_PRODUCE_ANIMAL:				//生产驯兽
			m_BuildProduce.OnProduceEvent(head->agent, block ,head->length, head->from, ANIMALS_TAME_ITEM, MAX_ANIMALS_LEVEL, EN_TIMER_PRODUCE_ANIMAL);
			break;
		case EN_C_CANCEL_PRODUCE:				//取消生产道具
			m_BuildProduce.OnCancelProduce(head->agent, block ,head->length, head->from);
			break;
		case EN_C_CANCEL_RESEARCH:				//取消科研道具
			m_ResearchManager.OnCancelResearchItem(head->agent, block ,head->length, head->from);
			break;
		case EN_C_RESEARCH_ITEM:					//研究道具
			m_ResearchManager.OnResearchItem(head->agent, block ,head->length, head->from);
			break;
		case EN_C_GET_ALL_RESEARCH_ITEM:			//得到所有已经研究了的道具列表
			m_ResearchManager.OnGetAllResearchList(head->agent,  head->from);
			break;
		case EN_C_GET_CITY_INFO:			//获取城市信息
			m_LevySoldier.OnCityInfo(head->agent, block, head->length, head->from);
			break;
		case EN_C_COUNTRY_MARKET:				//国家市场
			m_MarketManager.CountryMarket(head->agent, block,  head->length, head->from);
			break;
		case EN_C_COUNTRY_MARKET_BUSINESS:			//国家市场
			m_MarketManager.CountryMarketBusiness(head->agent, block,  head->length, head->from);
			break;
		case EN_C_MARKET_WEAPON_LIST:				//武器列表
			m_MarketManager.OnWeaponList(head->agent, block,  head->length, head->from);
			break;
		case EN_C_MARKET_DEFEND_LIST:				//防具列表
			m_MarketManager.OnDefendList(head->agent, block,  head->length, head->from);
			break;
		case EN_C_MARKET_OFFICER_ITEM_LIST:			//指挥官道具列表
			m_MarketManager.OfficerItemList(head->agent, block,  head->length, head->from);
			break;
		case EN_C_BASE_RESOURCE_LIST:				//基本资源列表
			m_MarketManager.PlayerResourceCount(head->agent, block, head->from);
			break;
		case EN_C_MARKET_GOODS_LIST:				//物品列表
			m_MarketManager.PeopleMarket(head->agent, block, head->length, head->from);
			break;
		case EN_C_MARKET_GOODS_NEXT:				//物品列表下一页
			m_MarketManager.PeopleMarket(head->agent, block, head->length, head->from,1);
			break;
		case EN_C_MARKET_PUTUP:					//挂牌
			m_MarketManager.OnPutUp(head->agent, head->from, block, head->length);
			break;
		case EN_C_MARKET_BIDDEN:					//出价
			m_MarketManager.OnBidden(head->agent, head->from, block, head->length);
			break;
		case EN_C_MARKET_BUY:						//一口价
			m_MarketManager.OnOnePrice(head->agent, head->from, block, head->length);
			break;
			//战利品拍卖
		case EN_C_VENDUE_GOODS_LIST:				//物品列表
			m_MarketManager.VendueGoods(head->agent, block, head->length, head->from);
			break;
		case EN_C_VENDUE_GOODS_NEXT:			//物品列表下一页
			m_MarketManager.VendueGoods(head->agent, block, head->length, head->from,1);
			break;
		case EN_C_VENDUE_BIDDEN:					//出价
			m_MarketManager.OnBidden(head->agent, head->from, block, head->length, 10);
			break;
		case EN_C_VENDUE_BUY:						//一口价
			m_MarketManager.OnOnePrice(head->agent, head->from, block, head->length, 10);
			break;
		case EN_C_CONTRIBUTE_INFO:					//捐献信息
			m_BuildProduce.OnContributeInfo(head->agent, block,  head->length, head->from);
			break;
		case EN_C_CONTRIBUTE:						//捐献
			m_BuildProduce.Contribute(head->agent, block,  head->length, head->from);
			break;
		case EN_C_CREATE_OFFICER:					//创建指挥官
			m_OtherManager.OnCreateOfficer(head->agent, block,  head->length, head->from);
			break;
		case EN_C_LOAD_MATERIAL:				//装载物资
			m_LevySoldier.OnLoadMaterial(head->agent, block, head->length, head->from);
			break;
		case EN_C_UNINSTALL_MATERIAL:		//卸载物资
			m_LevySoldier.OnUninstallMaterial(head->agent, block, head->length, head->from);
			break;
		case EN_C_LOOK_MATERIAL:				//物资查看
			m_LevySoldier.OnLookMaterial(head->agent, block, head->length, head->from);
			break;
		case EN_C_COLONIZE_CITY:				//殖民城市
			m_LevySoldier.OnColonizeCity(head->agent, block, head->length, head->from);
			break;
		case EN_C_EXIT_MAP_SERVER:						//退出游戏
			OnExitServer(head->from);
			break;
		case CMD_END_FIGHT:								//结束战斗
			m_BuildProduce.OnMapEnFight( block,  head->length);
			m_OtherManager.OnMapEnFight( block,  head->length);
			break;
		case CMD_ARMY_REACH:							//部队出征
			m_OtherManager.OnOutArmyEx( block,  head->length,0,false);//modify by xujincai 20081224
			break;
		case CGC_C_NOTITY_BUILDCONSTRUCTION:	//通知建造建筑，用于托管类
			m_BuildProduce.ConstrAutoBulid( block,  head->length);
			break;
		case CGC_C_NOTIFY_INCREASESPEED:		//提速通知
			g_SpeedCard.IncreaseSpeed( block,  head->length);
			break;
		case CGC_C_NOTIFY_NORMALSPEED:			//恢复正常速度
			g_SpeedCard.NormalSpeed( block,  head->length);
			break;
		case CGC_C_NOTIFY_COLLECTRESOURCE:		//收集资源
			m_BuildProduce.OnCollectResource( block,  head->length);
			break;
		case CGC_C_NOTIFY_INCREASEOTHERSPEED:
			g_DataServer.OnOnceSpeed(block,  head->length);
			break;
		case CMD_ARMY_REACH_OK:
			m_OtherManager.OnArmyReachOKEx(block,  head->length);
			break;
			//add by xujincai 20081217
		case EN_C_UNLAYARMY://解散部队
			{
				m_LevySoldier.ReqUnlayArmy(block);
			}
			break;
		case EN_C_DELETEARMY://删除部队,处理后压入战斗线程
			{
				if(m_LevySoldier.ReqDeleteArmy(block) ==0)
				{
					bIsFightMsg = true;
				}
			}
			break;
		case EN_C_NEWTREASUREPACK://添加宝箱
			{
				m_OtherManager.OnNewTreasurePack(block);
			}
			break;
		case EN_C_OPENTREASUREPACK://打开宝箱
			{
				m_OtherManager.OnOpenTreasurePack(block);
			}
			break;
		case EN_C_HEROGBH_NOTIFY://英雄重伤通知消息
			{
				m_OtherManager.OnHeroGBHNotify(block);
			}
			break;
		case EN_C_IMMEDCOMPLETE://立即完成
			{
				OnImmedComplete(block);
			}
			break;
		case CGC_S_NOTIFY_IMMEDCOMPLETE://收费系统通知
			{
				OnImmedCompleteNotifyMsg(block);
			}
			break;
		case EN_C_TIMER_NOTIFY://定时器
			{
				TIMER_HEADER *pTimerHeader = (TIMER_HEADER*)block;
				if(OnSetTimerEvent(pTimerHeader->timer_command,block,pTimerHeader->length) !=0)
				{
					bIsFightMsg = true;
				}
			}
			break;
		case EN_C_RESETMAPINFO://重置地图数据
			{
				LPResetMapInfo_Request resetmap_msg = (LPResetMapInfo_Request)block;
				m_LevySoldier.OnResetMapInfo(resetmap_msg);
				m_BuildProduce.OnResetMapInfo(resetmap_msg);
			}
			break;
		case EN_C_GETARMYLIST://获取玩家所有部队列表
			{
				m_OtherManager.OnGetArmyList(block);
			}
			break;
		case EN_C_MODIFYARMYSTATE://战斗、副本发送的部队状态改变的消息
			{
				m_OtherManager.OnModifyArmyState(block);
			}
			break;
		case CC_C_NOTIFY_CREATETIRRIER://通知郡地图创建国防军
			{
				m_LevySoldier.ReqCreateTirrier(block);
			}
			break;
		case CC_C_NOTIFY_TIRRIERGOOUT://通知大地图服务器国防军出征
			{
				m_OtherManager.OnTirrierGoOut(block);
			}
			break;
		case CC_C_NOTIFY_UPGRADEBUILD://通知郡地图升级公共建筑
			{
				m_BuildProduce.OnUpgrateBuild(block);
			}
			break;
		case EN_C_DESTROYCITY://玩家拆除据点
			{
				m_BuildProduce.OnDestroyCity(block);
			}
			break;
		case EN_C_QUERYCAPTIVELIST://查询俘虏列表
			{
				m_OtherManager.OnQueryCaptiveList(block);
			}
			break;
		case EN_C_CREATENPCARMY://创建NPC部队
			{
				m_OtherManager.NotifyCreateNPCArmy(block);
			}
			break;
		case EN_C_NOTIFY_MAPSTATE://郡状态改变
			{
				m_OtherManager.OnNotifyMapState(block);
			}
			break;
		case EN_C_INLAYEQUIP://精练物品
			{
				m_OtherManager.onInlayEquipment(block);
			}
			break;
		case CGC_C_NOTIFY_ADDOFFICERSKILL://收费系统通知使用技能卡学习特殊技能
			{
				m_OtherManager.onNotifyAddOfficerSkill(block);
			}
			break;
		case CGC_C_NOTIFY_BUYCARD://收费系统买卡，通知加入背包
			{
				m_OtherManager.onNotifyBuyCard(block);
			}
			break;
		case EN_C_CONVERT:
			{
				m_OtherManager.onConvert(block);
#ifdef _DEBUG
				printf("接收到玩家%d的军牌兑换消息。\n",head->from);
#endif
			}
			break;
		case EN_C_COMMONOPENCARD://统一的打开卡接口，只适用于右键可以直接打开类型的卡。
			{
				m_OtherManager.onCommonOpenCard(block);
			}
			break;
		case EN_C_MODIFYHERONAME://改名卡
			{
				m_OtherManager.onModifyHeroName(block);
			}
			break;
		case EN_S_NOTIFYPLAYERONLINE://通知是否在线
			{
				onNotifyPlayerOnline(block);
			}
			break;
		case EN_C_RELEASECAPTIVE://释放英雄
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
	m_BuildProduce.ReadProduceState();//读取资源采集加速参数

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
	//时间事件
	case EN_TIMER_ARMY_OUT:							//部队达到时间事件
		m_OtherManager.ArmyOutTimeEventEx(pDataBuffer, iDataSize);
		break;
	case EN_TIMER_CONSTRUCTION:						//建筑建筑物
		m_BuildProduce.ConTimeEvent(pDataBuffer, iDataSize);
		break;
	case EN_TIMER_PRODUCE_WEAPON:						//生产武器
		m_BuildProduce.OnProduceTimeEvent(pDataBuffer, iDataSize);
		break;
	case EN_TIMER_PRODUCE_DEFEND:						//生产防具
		m_BuildProduce.OnProduceTimeEvent(pDataBuffer, iDataSize);
		break;
	case EN_TIMER_PRODUCE_ANIMAL:						//生产驯兽技术
		m_BuildProduce.OnProduceTimeEvent(pDataBuffer, iDataSize);
		break;
	case EN_TIMER_PRODUCE_SPECIAL:						//生产特殊技术
		m_BuildProduce.OnProduceTimeEvent(pDataBuffer, iDataSize);
		break;
	case EN_TIMER_RESEARCH:								//科研道具技术
		m_ResearchManager.OnResearchTimeEvent(pDataBuffer, iDataSize);
		break;
	case EN_TIMER_LEVYING:								//招兵
		m_LevySoldier.OnLevyTimeEvent(pDataBuffer, iDataSize);	
		break;
	case EN_TIMER_RECRUITING_HERO:								//招募英雄时间事件
		m_OtherManager.OnRecruitHeroEvent(pDataBuffer, iDataSize);	
		break;
	case EN_TIMER_ARMY_WARM:						//警报信息
		m_OtherManager.ArmyWarmTimeEvent(pDataBuffer, iDataSize);
		break;
	case EN_TIMER_HERO_RESUME://英雄恢复
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
//地图服务器信息
bool CDataServer::MapServerInfo(int handle, void * pDataBuffer, int iDataSize, unsigned long lPlayerID)
{
	MAP_SERVER_VALIDATE *pServerValidate = (MAP_SERVER_VALIDATE *)pDataBuffer;
	//游戏中的时间
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
	//发送退出游戏通知到郡服务器、科研、市场
	//m_MarketManager.OnExitMapServer(lPlayerID);
}
//结束战斗
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
		LOG_TRACE_EX(false,("CDataServer::OnImmedComplete-->消息包非法:玩家：%d\n",req_msg->from));
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
//处理收费系统扣费后的通知消息
void CDataServer::OnImmedCompleteNotifyMsg(void *msg)
{
	LPNotify_ImmedComplete_Response req_msg = (LPNotify_ImmedComplete_Response)msg;
	if(req_msg->length != sizeof(Notify_ImmedComplete_Response))
	{
		LOG_TRACE_EX(false,("CDataServer::OnImmedCompleteNotifyMsg-->消息包非法:玩家%d\n",req_msg->from));
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
			strcpy(Table.armyname_,"联盟国防军");
			Table.raceid_ = 1;
		}
		else
		{
			Table.raceid_ = 5;
			strcpy(Table.armyname_,"部落国防军");
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
	//下发给玩家通知消息
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
		//发送广播消息
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
