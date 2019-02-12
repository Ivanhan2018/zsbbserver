#ifndef __INCLUDE_TEST_DGB_PERF_H__
#define __INCLUDE_TEST_DGB_PERF_H__

#include "DB_Table.h"
#include "DB_Proxy.h"
#include <boost/bind.hpp>
#include <boost/thread.hpp>

template < class type, class typeset> void dbg_test_perf(typeset tset)
{
	DB_Proxy proxy;
	if (!proxy.init("127.0.0.1", 8899))
	{
		printf("连接DBG失败\n");
		return ;
	}

	type t_ins[2000];
	int i;
	long it = GetTickCount();

	for (i = 0; i < 2000; i++)
	{
		tset(t_ins[i], i+1);

		if (proxy.db_insert(t_ins[i]) != DB_Proxy::DB_SUCCEED)
		{
			printf("%s 添加: 失败\n", type::getname());
			proxy.fini();
			return ;
		}
	}
	it = GetTickCount() - it;

	long dt = GetTickCount();
	for (i = 0; i < 2000; i++)
	{
		if (proxy.db_delete(t_ins[i]) != DB_Proxy::DB_SUCCEED)
		{
			printf("%s 删除: 失败\n", type::getname());
			proxy.fini();
			return ;
		}
	}
	dt = GetTickCount() - dt;

	proxy.fini();
	printf("表 %s 操作次数 %d 插入耗时(%d) 删除耗时(%d)\n", type::getname(), i * 2, it, dt);
}

long opt_count = 0;
bool run = true;

template < class type, class typeset> void dbg_test_perf_ex(typeset tset, int no)
{
	DB_Proxy proxy;
	if (!proxy.init("127.0.0.1", 8899,11))
	{
		printf("连接DBG失败\n");
		return ;
	}

	type t_ins;
	while (run)
	{
		tset(t_ins, no);
		if (proxy.db_insert(t_ins) != DB_Proxy::DB_SUCCEED)
		{
			printf("%s 添加: 失败\n", type::getname());
			break;
		}
		::InterlockedIncrement(&opt_count);
		Sleep(1);

		if (proxy.db_delete(t_ins) != DB_Proxy::DB_SUCCEED)
		{
			printf("%s 删除: 失败\n", type::getname());
			break;
		}
		::InterlockedIncrement(&opt_count);
		Sleep(1);
	}
	proxy.fini();
}

void print_opt_count()
{
	while (run)
	{
		printf("每秒操作次数: %d\n", ::InterlockedExchange(&opt_count, 0));
		Sleep(1000);
	}
}

//void GameCityInfo_set_1(GameCityInfo & t, int i)
//{
//	t.setmasterid(i);
//	t.setdetailid(i);
//}

void PlayerSoldierRace_set_1(PlayerSoldierRace & t, int i)
{
	t.setmasterid(i, i);
	t.completetimes_ = time(0);
	t.setdetailid(i);
}

void ProductionAmount_set_1(ProductionAmount & t, int i)
{
	t.setmasterid(i, i, i, i);
	t.setdetailid(i);
}

//void ConstructionConfigure_set_1(ConstructionConfigure & t, int i)
//{
//	t.setid(i, i);
//	t.completetimes_ = time(0);
//}

//void ResearchConfigure_set_1(ResearchConfigure & t, int i)
//{
//	t.setmasterid(i);
//	t.setdetailid(i, i);
//	t.completetimes_ = time(0);
//}

void PlayerConstruction_set_1(PlayerConstruction & t, int i)
{
	t.setmasterid(i, i, i);
	t.setdetailid(i);
	t.completetimes_ = time(0);
}

void PlayerResource_set_1(PlayerResource & t, int i)
{
	t.setmasterid(i, i);
	t.setdetailid(i);
	t.completetimes_ = time(0);
}

void ArmyTable_set_1(ArmyTable & t, int i)
{
	t.setmasterid(i);
	t.setdetailid(i);
}

void Market_set_1(Market & t, int i)
{
	t.setmasterid(i, i);
	t.setdetailid(i);
	t.starttime_ = time(0);
}

void ConstructionBuild_set_1(ConstructionBuild & t, int i)
{
	t.setmasterid(i, i, i);
	t.setdetailid(i, i, i);
	t.starttime_ = time(0);
}

void ProductionActivity_set_1(ProductionActivity & t, int i)
{
	t.setmasterid(i, i, i, i);
	t.setdetailid(i, i, i);
	t.starttime_ = time(0);
}

void ConsortiaInfo_set_1(ConsortiaInfo & t, int i)
{
	t.setid(i);
	t.createtime_ = time(0);
}

void ConsortiaBank_set_1(ConsortiaBank & t, int i)
{
	t.setmasterid(i);
	t.setdetailid(i);
	t.itemcount_ = i;
}

void ConsortiaUnion_set_1(ConsortiaUnion & t, int i)
{
	t.setid(i, i);
}

void PlayerOfficerItemTable_set_1(PlayerOfficerItemTable & t, int i)
{
	t.setmasterid(i, i);
	t.setdetailid(i);
}

void PlayerItemStorageTable_set_1(PlayerItemStorageTable & t, int i)
{
	t.setmasterid(i, i);
	t.setdetailid(i);
}


void SoldierParamTable_set_1(SoldierParamTable & t, int i)
{
	t.setid(i);
}

void PlayerOfficerTable_set_1(PlayerOfficerTable & t, int i)
{
	t.setmasterid(i);
	t.setdetailid(i);
	t.updatetime_ = t.gbhtime_ = time(0);
}

void PlayerTask_set_1(PlayerTask & t, int i)
{
	t.setmasterid(i);
	t.setdetailid(i);
	t.tasktime_ = time(0);
}

void ConsortiaPersonnel_set_1(ConsortiaPersonnel & t, int i)
{
	t.setmasterid(i);
	t.setdetailid(i);
}

void CellPrice_set_1(CellPrice & t, int i)
{
	t.setid(i);
}

void CityAttribute_set_1(CityAttribute & t, int i)
{
	t.setmasterid(i);
	t.setdetailid(i);
}

//void CreateConsortia_set_1(CreateConsortia & t, int i)
//{
//	t.setid(i);
//}

void ConsortiaFight_set_1(ConsortiaFight & t, int i)
{
	t.setmasterid(i);
	t.setdetailid(i, i);
	t.time_ = time(0);
}

//void TaskBaseTable_set_1(TaskBaseTable & t, int i)
//{
//	t.setid(i);
//}

//void ProduceConfigure_set_1(ProduceConfigure & t, int i)
//{
//	t.setmasterid(i);
//	t.setdetailid(i, i);
//	t.completetimes_ = time(0);
//}

void ChatRoomTable_set_1(ChatRoomTable & t, int i)
{
	t.setid(i);
}

void PlayerChatRoomTable_set_1(PlayerChatRoomTable & t, int i)
{
	t.setid(i);
}

void PlayerCity_set_1(PlayerCity & t, int i)
{
	t.setmasterid(i);
	t.setdetailid(i);
}

void OfficerRaceAttribute_set_1(OfficerRaceAttribute & t, int i)
{
	t.setid(i);
}

//void WeaponAttribute_set_1(WeaponAttribute & t, int i)
//{
//	t.setmasterid(i);
//	t.setdetailid(i);
//}

//void DefendAttribute_set_1(DefendAttribute & t, int i)
//{
//	t.setid(i);
//}

void ArmyOutTable_set_1(ArmyOutTable & t, int i)
{
	t.setmasterid(i);
	t.setdetailid(i);
	t.starttime_ = time(0);
}

//void OfficerTech_set_1(OfficerTech & t, int i)
//{
//	t.setmasterid(i);
//	t.setdetailid(i);
//}

void NPC_set_1(NPC & t, int i)
{
	t.setid(i);
}

void MapInfoTable_set_1(MapInfoTable & t, int i)
{
	t.setid(i);
}

void ResearchItem_set_1(ResearchItem & t, int i)
{
	t.setmasterid(i,i);
	t.setdetailid(i);
}

void EctypeBaseTable_set_1(EctypeBaseTable & t, int i)
{
	t.setid(i);
}

void NPCArmyTable_set_1(NPCArmyTable & t, int i)
{
	t.setmasterid(i);
	t.setdetailid(i);
}

void PlayerCaptiveHero_set_1(PlayerCaptiveHero & t, int i)
{
	t.setmasterid(i);
	t.setdetailid(i, i, i);
}

//void EquipAttribute_set_1(EquipAttribute & t, int i)
//{
//	t.setid(i);
//}

void WarNote_set_1(WarNote & t, int i)
{
	t.setmasterid(i,i);
	t.setdetailid(i);
}

void PullulateNote_set_1(PullulateNote & t, int i)
{
	t.setmasterid(i,i);
	t.setdetailid(i);
}

void FirendTable_set_1(FirendTable & t, int i)
{
	t.setmasterid(i,i);
	t.setdetailid(i);
}

//void ConsortiaOfficial_set_1(ConsortiaOfficial & t, int i)
//{
//	t.setid(i);
//}

void LoadMaterial_set_1(LoadMaterial & t, int i)
{
	t.setmasterid(i,i);
	t.setdetailid(i,i);
}

void Treasury_set_1(Treasury & t, int i)
{
	t.setid(i);
}


//void CardConfig_set_1(CardConfig & t, int i)
//{
//	t.setid(i);
//}

void PublicBuildParamTable_set_1(PublicBuildParamTable & t, int i)
{
	t.setid(i,i);
}

void PublicBuildTable_set_1(PublicBuildTable & t, int i)
{
	t.setmasterid(i);
	t.setdetailid(i);
}

void PlayerProduceState_set_1(PlayerProduceState & t, int i)
{
	t.setmasterid(i);
	t.setdetailid(i);
}

//void JobParam_set_1(JobParam & t, int i)
//{
//	t.setid(i);
//}

//void BinConfig_set_1(BinConfig & t, int i)
//{
//	t.setid(i);
//}

void EquipAntagonism_set_1(EquipAntagonism & t, int i)
{
	t.setid(i,i);
}

void BatttleField_set_1(BatttleField & t, int i)
{
	t.setid(i);
}

void ConsortiaCompare_set_1(ConsortiaCompare & t, int i)
{
	t.setid(i);
}

void PlayerCompare_set_1(PlayerCompare & t, int i)
{
	t.setid(i);
}

void Council_set_1(Council & t, int i)
{
	t.setmasterid(i,i);
	t.setdetailid(i);
}

void Candidate_set_1(Candidate & t, int i)
{
	t.setmasterid(i,i);
	t.setdetailid(i);
}

void Ballot_set_1(Ballot & t, int i)
{
	t.setmasterid(i,i);
	t.setdetailid(i);
}

void Country_set_1(Country & t, int i)
{
	t.setid(i);
}

void ResearchActivity_set_1(ResearchActivity & t, int i)
{
	t.setmasterid(i);
	t.setdetailid(i,i);
}

void LevySoldierActivity_set_1(LevySoldierActivity & t, int i)
{
	t.setmasterid(i,i,i);
	t.setdetailid(i);
}

void MailTable_set_1(MailTable & t, int i)
{
	t.setmasterid(i);
	t.setdetailid(i);
}

void NewsTable_set_1(NewsTable & t, int i)
{
	t.setid(i);
}

void ChargeSvrTimer_set_1(ChargeSvrTimer & t, int i)
{
	t.setmasterid(i);
	t.setdetailid(i);
}

void OfficerJobAttribute_set_1(OfficerJobAttribute & t, int i)
{
	t.setid(i);
}

//void thread_dbg_test_perf1(int no) { dbg_test_perf_ex<GameCityInfo>(GameCityInfo_set_1, no); }
void thread_dbg_test_perf2(int no) { dbg_test_perf_ex<PlayerSoldierRace>(PlayerSoldierRace_set_1, no); }
void thread_dbg_test_perf3(int no) { dbg_test_perf_ex<ProductionAmount>(ProductionAmount_set_1, no); }
//void thread_dbg_test_perf4(int no) { dbg_test_perf_ex<ConstructionConfigure>(ConstructionConfigure_set_1, no); }
//void thread_dbg_test_perf5(int no) { dbg_test_perf_ex<ResearchConfigure>(ResearchConfigure_set_1, no); }
void thread_dbg_test_perf6(int no) { dbg_test_perf_ex<PlayerConstruction>(PlayerConstruction_set_1, no); }
void thread_dbg_test_perf7(int no) { dbg_test_perf_ex<PlayerResource>(PlayerResource_set_1, no); }
void thread_dbg_test_perf8(int no) { dbg_test_perf_ex<ArmyTable>(ArmyTable_set_1, no); }
void thread_dbg_test_perf9(int no) { dbg_test_perf_ex<Market>(Market_set_1, no); }
void thread_dbg_test_perf10(int no) { dbg_test_perf_ex<ConstructionBuild>(ConstructionBuild_set_1, no); }
void thread_dbg_test_perf11(int no) { dbg_test_perf_ex<OfficerRaceAttribute>(OfficerRaceAttribute_set_1, no); }
void thread_dbg_test_perf12(int no) { dbg_test_perf_ex<PlayerCity>(PlayerCity_set_1, no); }
void thread_dbg_test_perf13(int no) { dbg_test_perf_ex<PlayerChatRoomTable>(PlayerChatRoomTable_set_1, no); }
void thread_dbg_test_perf14(int no) { dbg_test_perf_ex<ChatRoomTable>(ChatRoomTable_set_1, no); }
//void thread_dbg_test_perf15(int no) { dbg_test_perf_ex<ProduceConfigure>(ProduceConfigure_set_1, no); }
//void thread_dbg_test_perf16(int no) { dbg_test_perf_ex<TaskBaseTable>(TaskBaseTable_set_1, no); }
void thread_dbg_test_perf17(int no) { dbg_test_perf_ex<ConsortiaFight>(ConsortiaFight_set_1, no); }
//void thread_dbg_test_perf18(int no) { dbg_test_perf_ex<CreateConsortia>(CreateConsortia_set_1, no); }
void thread_dbg_test_perf19(int no) { dbg_test_perf_ex<CityAttribute>(CityAttribute_set_1, no); }
void thread_dbg_test_perf20(int no) { dbg_test_perf_ex<CellPrice>(CellPrice_set_1, no); }
void thread_dbg_test_perf21(int no) { dbg_test_perf_ex<ConsortiaPersonnel>(ConsortiaPersonnel_set_1, no); }
void thread_dbg_test_perf22(int no) { dbg_test_perf_ex<PlayerTask>(PlayerTask_set_1, no); }
void thread_dbg_test_perf23(int no) { dbg_test_perf_ex<PlayerOfficerTable>(PlayerOfficerTable_set_1, no); }
void thread_dbg_test_perf24(int no) { dbg_test_perf_ex<SoldierParamTable>(SoldierParamTable_set_1, no); }
void thread_dbg_test_perf25(int no) { dbg_test_perf_ex<PlayerItemStorageTable>(PlayerItemStorageTable_set_1, no); }
void thread_dbg_test_perf26(int no) { dbg_test_perf_ex<PlayerOfficerItemTable>(PlayerOfficerItemTable_set_1, no); }
void thread_dbg_test_perf27(int no) { dbg_test_perf_ex<ConsortiaUnion>(ConsortiaUnion_set_1, no); }
void thread_dbg_test_perf28(int no) { dbg_test_perf_ex<ConsortiaBank>(ConsortiaBank_set_1, no); }
void thread_dbg_test_perf29(int no) { dbg_test_perf_ex<ConsortiaInfo>(ConsortiaInfo_set_1, no); }
void thread_dbg_test_perf30(int no) { dbg_test_perf_ex<ProductionActivity>(ProductionActivity_set_1, no); }
//void thread_dbg_test_perf31(int no) { dbg_test_perf_ex<WeaponAttribute>(WeaponAttribute_set_1, no); }
//void thread_dbg_test_perf32(int no) { dbg_test_perf_ex<DefendAttribute>(DefendAttribute_set_1, no); }
void thread_dbg_test_perf33(int no) { dbg_test_perf_ex<ArmyOutTable>(ArmyOutTable_set_1, no); }
//void thread_dbg_test_perf34(int no) { dbg_test_perf_ex<OfficerTech>(OfficerTech_set_1, no); }
void thread_dbg_test_perf35(int no) { dbg_test_perf_ex<NPC>(NPC_set_1, no); }
void thread_dbg_test_perf36(int no) { dbg_test_perf_ex<MapInfoTable>(MapInfoTable_set_1, no); }
void thread_dbg_test_perf37(int no) { dbg_test_perf_ex<ResearchItem>(ResearchItem_set_1, no); }
void thread_dbg_test_perf38(int no) { dbg_test_perf_ex<EctypeBaseTable>(EctypeBaseTable_set_1, no); }
void thread_dbg_test_perf39(int no) { dbg_test_perf_ex<NPCArmyTable>(NPCArmyTable_set_1, no); }
void thread_dbg_test_perf40(int no) { dbg_test_perf_ex<PlayerCaptiveHero>(PlayerCaptiveHero_set_1, no); }
//void thread_dbg_test_perf41(int no) { dbg_test_perf_ex<EquipAttribute>(EquipAttribute_set_1, no); }
void thread_dbg_test_perf42(int no) { dbg_test_perf_ex<WarNote>(WarNote_set_1, no); }
void thread_dbg_test_perf43(int no) { dbg_test_perf_ex<PullulateNote>(PullulateNote_set_1, no); }
void thread_dbg_test_perf44(int no) { dbg_test_perf_ex<FirendTable>(FirendTable_set_1, no); }
//void thread_dbg_test_perf45(int no) { dbg_test_perf_ex<ConsortiaOfficial>(ConsortiaOfficial_set_1, no); }
void thread_dbg_test_perf46(int no) { dbg_test_perf_ex<LoadMaterial>(LoadMaterial_set_1, no); }
void thread_dbg_test_perf47(int no) { dbg_test_perf_ex<Treasury>(Treasury_set_1, no); }
//void thread_dbg_test_perf48(int no) { dbg_test_perf_ex<CardConfig>(CardConfig_set_1, no); }
void thread_dbg_test_perf49(int no) { dbg_test_perf_ex<PublicBuildParamTable>(PublicBuildParamTable_set_1, no); }
void thread_dbg_test_perf50(int no) { dbg_test_perf_ex<PublicBuildTable>(PublicBuildTable_set_1, no); }
void thread_dbg_test_perf51(int no) { dbg_test_perf_ex<PlayerProduceState>(PlayerProduceState_set_1, no); }
//void thread_dbg_test_perf52(int no) { dbg_test_perf_ex<JobParam>(JobParam_set_1, no); }
//void thread_dbg_test_perf53(int no) { dbg_test_perf_ex<BinConfig>(BinConfig_set_1, no); }
void thread_dbg_test_perf54(int no) { dbg_test_perf_ex<EquipAntagonism>(EquipAntagonism_set_1, no); }
void thread_dbg_test_perf55(int no) { dbg_test_perf_ex<BatttleField>(BatttleField_set_1, no); }
void thread_dbg_test_perf56(int no) { dbg_test_perf_ex<ConsortiaCompare>(ConsortiaCompare_set_1, no); }
void thread_dbg_test_perf57(int no) { dbg_test_perf_ex<PlayerCompare>(PlayerCompare_set_1, no); }
void thread_dbg_test_perf58(int no) { dbg_test_perf_ex<Council>(Council_set_1, no); }
void thread_dbg_test_perf59(int no) { dbg_test_perf_ex<Candidate>(Candidate_set_1, no); }
void thread_dbg_test_perf60(int no) { dbg_test_perf_ex<Ballot>(Ballot_set_1, no); }
void thread_dbg_test_perf61(int no) { dbg_test_perf_ex<Country>(Country_set_1, no); }
void thread_dbg_test_perf62(int no) { dbg_test_perf_ex<ResearchActivity>(ResearchActivity_set_1, no); }
void thread_dbg_test_perf63(int no) { dbg_test_perf_ex<LevySoldierActivity>(LevySoldierActivity_set_1, no); }
void thread_dbg_test_perf64(int no) { dbg_test_perf_ex<MailTable>(MailTable_set_1, no); }
void thread_dbg_test_perf65(int no) { dbg_test_perf_ex<NewsTable>(NewsTable_set_1, no); }
void thread_dbg_test_perf66(int no) { dbg_test_perf_ex<ChargeSvrTimer>(ChargeSvrTimer_set_1, no); }
void thread_dbg_test_perf666(int no) { dbg_test_perf_ex<OfficerJobAttribute>(OfficerJobAttribute_set_1, no); }


void dbg_test_perf_111()
{
	DB_Proxy proxy;
	if (!proxy.init("192.168.1.244", 8899,11))
	{
		printf("连接DBG失败\n");
		return ;
	}

	for (int aa = 0; aa < 10; aa++)
	{
	ArmyTable at;

	int i;
	long it = GetTickCount();
	for (i = 13000; i <= 17000; i++)
	{
		at.setmasterid(1018);
		at.setdetailid(i);
		proxy.db_select(at);
	}
	it = GetTickCount() - it;
	printf("耗时(%d)\n", it);
	Sleep(1);
	}
	proxy.fini();
}

void test_dbg_perf_main()
{
	WSADATA wsaData;
	WSAStartup( 0x0202, &wsaData );

	dbg_test_perf_111();
	//dbg_test_perf<GameCityInfo>(GameCityInfo_set_1); 
	//dbg_test_perf<PlayerSoldierRace>(PlayerSoldierRace_set_1); 
	//dbg_test_perf<ProductionAmount>(ProductionAmount_set_1); 
	//dbg_test_perf<ConstructionConfigure>(ConstructionConfigure_set_1); 
	//dbg_test_perf<ResearchConfigure>(ResearchConfigure_set_1); 
	//dbg_test_perf<PlayerConstruction>(PlayerConstruction_set_1); 
	//dbg_test_perf<PlayerResource>(PlayerResource_set_1); 
	//dbg_test_perf<ArmyTable>(ArmyTable_set_1); 
	//dbg_test_perf<Market>(Market_set_1); 
	//dbg_test_perf<ConstructionBuild>(ConstructionBuild_set_1); 
	//dbg_test_perf<OfficerRaceAttribute>(OfficerRaceAttribute_set_1); 
	//dbg_test_perf<PlayerCity>(PlayerCity_set_1); 
	//dbg_test_perf<PlayerChatRoomTable>(PlayerChatRoomTable_set_1); 
	//dbg_test_perf<ChatRoomTable>(ChatRoomTable_set_1); 
	//dbg_test_perf<ProduceConfigure>(ProduceConfigure_set_1); 
	//dbg_test_perf<TaskBaseTable>(TaskBaseTable_set_1); 
	//dbg_test_perf<ConsortiaFight>(ConsortiaFight_set_1); 
	//dbg_test_perf<CreateConsortia>(CreateConsortia_set_1); 
	//dbg_test_perf<CityAttribute>(CityAttribute_set_1); 
	//dbg_test_perf<CellPrice>(CellPrice_set_1); 
	//dbg_test_perf<ConsortiaPersonnel>(ConsortiaPersonnel_set_1); 
	//dbg_test_perf<PlayerTask>(PlayerTask_set_1); 
	//dbg_test_perf<PlayerOfficerTable>(PlayerOfficerTable_set_1); 
	//dbg_test_perf<SoldierParamTable>(SoldierParamTable_set_1); 
	//dbg_test_perf<PlayerItemStorageTable>(PlayerItemStorageTable_set_1); 
	//dbg_test_perf<PlayerOfficerItemTable>(PlayerOfficerItemTable_set_1); 
	//dbg_test_perf<ConsortiaUnion>(ConsortiaUnion_set_1); 
	//dbg_test_perf<ConsortiaBank>(ConsortiaBank_set_1); 
	//dbg_test_perf<ConsortiaInfo>(ConsortiaInfo_set_1); 
	//dbg_test_perf<ProductionActivity>(ProductionActivity_set_1); 
	//dbg_test_perf<WeaponAttribute>(WeaponAttribute_set_1); 
	//dbg_test_perf<DefendAttribute>(DefendAttribute_set_1); 
	//dbg_test_perf<ArmyOutTable>(ArmyOutTable_set_1); 
	//dbg_test_perf<OfficerTech>(OfficerTech_set_1); 
	//dbg_test_perf<NPC>(NPC_set_1); 
	//dbg_test_perf<MapInfoTable>(MapInfoTable_set_1); 
	//dbg_test_perf<ResearchItem>(ResearchItem_set_1); 
	//dbg_test_perf<EctypeBaseTable>(EctypeBaseTable_set_1); 
	//dbg_test_perf<NPCArmyTable>(NPCArmyTable_set_1); 
	//dbg_test_perf<PlayerCaptiveHero>(PlayerCaptiveHero_set_1); 
	//dbg_test_perf<EquipAttribute>(EquipAttribute_set_1); 
	//dbg_test_perf<WarNote>(WarNote_set_1); 
	//dbg_test_perf<PullulateNote>(PullulateNote_set_1); 
	//dbg_test_perf<FirendTable>(FirendTable_set_1); 
	//dbg_test_perf<ConsortiaOfficial>(ConsortiaOfficial_set_1); 
	//dbg_test_perf<LoadMaterial>(LoadMaterial_set_1); 
	//dbg_test_perf<Treasury>(Treasury_set_1); 
	//dbg_test_perf<CardConfig>(CardConfig_set_1); 
	//dbg_test_perf<PublicBuildParamTable>(PublicBuildParamTable_set_1); 
	//dbg_test_perf<PublicBuildTable>(PublicBuildTable_set_1); 
	//dbg_test_perf<PlayerProduceState>(PlayerProduceState_set_1); 
	//dbg_test_perf<JobParam>(JobParam_set_1); 
	//dbg_test_perf<BinConfig>(BinConfig_set_1); 
	//dbg_test_perf<EquipAntagonism>(EquipAntagonism_set_1); 
	//dbg_test_perf<BatttleField>(BatttleField_set_1); 
	//dbg_test_perf<ConsortiaCompare>(ConsortiaCompare_set_1); 
	//dbg_test_perf<PlayerCompare>(PlayerCompare_set_1); 
	//dbg_test_perf<Council>(Council_set_1); 
	//dbg_test_perf<Candidate>(Candidate_set_1);
	//dbg_test_perf<Ballot>(Ballot_set_1);
	//dbg_test_perf<Country>(Country_set_1);
	//dbg_test_perf<ResearchActivity>(ResearchActivity_set_1);
	//dbg_test_perf<LevySoldierActivity>(LevySoldierActivity_set_1);
	//dbg_test_perf<MailTable>(MailTable_set_1);
	//dbg_test_perf<NewsTable>(NewsTable_set_1);
	//dbg_test_perf<ChargeSvrTimer>(ChargeSvrTimer_set_1);
	//dbg_test_perf<OfficerJobAttribute>(OfficerJobAttribute_set_1);

	//std::vector<boost::shared_ptr<boost::thread> > threads;

	//boost::shared_ptr<boost::thread> thread(
	//	new boost::thread(boost::bind(print_opt_count)));
	//threads.push_back(thread);

	//for (int i = 0; i < 3; i++)
	//{
	//	boost::shared_ptr<boost::thread> thread1(new boost::thread(boost::bind(thread_dbg_test_perf1,i+1)));
	//	threads.push_back(thread1);
	//	boost::shared_ptr<boost::thread> thread2(new boost::thread(boost::bind(thread_dbg_test_perf2,i+1)));
	//	threads.push_back(thread2);
	//	boost::shared_ptr<boost::thread> thread3(new boost::thread(boost::bind(thread_dbg_test_perf3,i+1)));
	//	threads.push_back(thread3);
	//	boost::shared_ptr<boost::thread> thread4(new boost::thread(boost::bind(thread_dbg_test_perf4,i+1)));
	//	threads.push_back(thread4);
	//	boost::shared_ptr<boost::thread> thread5(new boost::thread(boost::bind(thread_dbg_test_perf5,i+1)));
	//	threads.push_back(thread5);
	//	boost::shared_ptr<boost::thread> thread6(new boost::thread(boost::bind(thread_dbg_test_perf6,i+1)));
	//	threads.push_back(thread6);
	//	boost::shared_ptr<boost::thread> thread7(new boost::thread(boost::bind(thread_dbg_test_perf7,i+1)));
	//	threads.push_back(thread7);
	//	boost::shared_ptr<boost::thread> thread8(new boost::thread(boost::bind(thread_dbg_test_perf8,i+1)));
	//	threads.push_back(thread8);
	//	boost::shared_ptr<boost::thread> thread9(new boost::thread(boost::bind(thread_dbg_test_perf9,i+1)));
	//	threads.push_back(thread9);
	//	boost::shared_ptr<boost::thread> thread10(new boost::thread(boost::bind(thread_dbg_test_perf10,i+1)));
	//	threads.push_back(thread10);
	//	boost::shared_ptr<boost::thread> thread11(new boost::thread(boost::bind(thread_dbg_test_perf11,i+1)));
	//	threads.push_back(thread11);
	//	boost::shared_ptr<boost::thread> thread12(new boost::thread(boost::bind(thread_dbg_test_perf12,i+1)));
	//	threads.push_back(thread12);
	//	boost::shared_ptr<boost::thread> thread13(new boost::thread(boost::bind(thread_dbg_test_perf13,i+1)));
	//	threads.push_back(thread13);
	//	boost::shared_ptr<boost::thread> thread14(new boost::thread(boost::bind(thread_dbg_test_perf14,i+1)));
	//	threads.push_back(thread14);
	//	boost::shared_ptr<boost::thread> thread15(new boost::thread(boost::bind(thread_dbg_test_perf15,i+1)));
	//	threads.push_back(thread15);
	//	boost::shared_ptr<boost::thread> thread16(new boost::thread(boost::bind(thread_dbg_test_perf16,i+1)));
	//	threads.push_back(thread16);
	//	boost::shared_ptr<boost::thread> thread17(new boost::thread(boost::bind(thread_dbg_test_perf17,i+1)));
	//	threads.push_back(thread17);
	//	boost::shared_ptr<boost::thread> thread18(new boost::thread(boost::bind(thread_dbg_test_perf18,i+1)));
	//	threads.push_back(thread18);
	//	boost::shared_ptr<boost::thread> thread19(new boost::thread(boost::bind(thread_dbg_test_perf19,i+1)));
	//	threads.push_back(thread19);
	//	boost::shared_ptr<boost::thread> thread20(new boost::thread(boost::bind(thread_dbg_test_perf20,i+1)));
	//	threads.push_back(thread20);
	//	boost::shared_ptr<boost::thread> thread21(new boost::thread(boost::bind(thread_dbg_test_perf21,i+1)));
	//	threads.push_back(thread21);
	//	boost::shared_ptr<boost::thread> thread22(new boost::thread(boost::bind(thread_dbg_test_perf22,i+1)));
	//	threads.push_back(thread22);
	//	boost::shared_ptr<boost::thread> thread23(new boost::thread(boost::bind(thread_dbg_test_perf23,i+1)));
	//	threads.push_back(thread23);
	//	boost::shared_ptr<boost::thread> thread24(new boost::thread(boost::bind(thread_dbg_test_perf24,i+1)));
	//	threads.push_back(thread24);
	//	boost::shared_ptr<boost::thread> thread25(new boost::thread(boost::bind(thread_dbg_test_perf25,i+1)));
	//	threads.push_back(thread25);
	//	boost::shared_ptr<boost::thread> thread26(new boost::thread(boost::bind(thread_dbg_test_perf26,i+1)));
	//	threads.push_back(thread26);
	//	boost::shared_ptr<boost::thread> thread27(new boost::thread(boost::bind(thread_dbg_test_perf27,i+1)));
	//	threads.push_back(thread27);
	//	boost::shared_ptr<boost::thread> thread28(new boost::thread(boost::bind(thread_dbg_test_perf28,i+1)));
	//	threads.push_back(thread28);
	//	boost::shared_ptr<boost::thread> thread29(new boost::thread(boost::bind(thread_dbg_test_perf29,i+1)));
	//	threads.push_back(thread29);
	//	boost::shared_ptr<boost::thread> thread30(new boost::thread(boost::bind(thread_dbg_test_perf30,i+1)));
	//	threads.push_back(thread30);
	//	boost::shared_ptr<boost::thread> thread31(new boost::thread(boost::bind(thread_dbg_test_perf31,i+1)));
	//	threads.push_back(thread31);
	//	boost::shared_ptr<boost::thread> thread32(new boost::thread(boost::bind(thread_dbg_test_perf32,i+1)));
	//	threads.push_back(thread32);
	//	boost::shared_ptr<boost::thread> thread33(new boost::thread(boost::bind(thread_dbg_test_perf33,i+1)));
	//	threads.push_back(thread33);
	//	boost::shared_ptr<boost::thread> thread34(new boost::thread(boost::bind(thread_dbg_test_perf34,i+1)));
	//	threads.push_back(thread34);
	//	boost::shared_ptr<boost::thread> thread35(new boost::thread(boost::bind(thread_dbg_test_perf35,i+1)));
	//	threads.push_back(thread35);
	//	boost::shared_ptr<boost::thread> thread36(new boost::thread(boost::bind(thread_dbg_test_perf36,i+1)));
	//	threads.push_back(thread36);
	//	boost::shared_ptr<boost::thread> thread37(new boost::thread(boost::bind(thread_dbg_test_perf37,i+1)));
	//	threads.push_back(thread37);
	//	boost::shared_ptr<boost::thread> thread38(new boost::thread(boost::bind(thread_dbg_test_perf38,i+1)));
	//	threads.push_back(thread38);
	//	boost::shared_ptr<boost::thread> thread39(new boost::thread(boost::bind(thread_dbg_test_perf39,i+1)));
	//	threads.push_back(thread39);
	//	boost::shared_ptr<boost::thread> thread40(new boost::thread(boost::bind(thread_dbg_test_perf40,i+1)));
	//	threads.push_back(thread40);
	//	boost::shared_ptr<boost::thread> thread41(new boost::thread(boost::bind(thread_dbg_test_perf41,i+1)));
	//	threads.push_back(thread41);
	//	boost::shared_ptr<boost::thread> thread42(new boost::thread(boost::bind(thread_dbg_test_perf42,i+1)));
	//	threads.push_back(thread42);
	//	boost::shared_ptr<boost::thread> thread43(new boost::thread(boost::bind(thread_dbg_test_perf43,i+1)));
	//	threads.push_back(thread43);
	//	boost::shared_ptr<boost::thread> thread44(new boost::thread(boost::bind(thread_dbg_test_perf44,i+1)));
	//	threads.push_back(thread44);
	//	boost::shared_ptr<boost::thread> thread45(new boost::thread(boost::bind(thread_dbg_test_perf45,i+1)));
	//	threads.push_back(thread45);
	//	boost::shared_ptr<boost::thread> thread46(new boost::thread(boost::bind(thread_dbg_test_perf46,i+1)));
	//	threads.push_back(thread46);
	//	boost::shared_ptr<boost::thread> thread47(new boost::thread(boost::bind(thread_dbg_test_perf47,i+1)));
	//	threads.push_back(thread47);
	//	boost::shared_ptr<boost::thread> thread48(new boost::thread(boost::bind(thread_dbg_test_perf48,i+1)));
	//	threads.push_back(thread48);
	//	boost::shared_ptr<boost::thread> thread49(new boost::thread(boost::bind(thread_dbg_test_perf49,i+1)));
	//	threads.push_back(thread49);
	//	boost::shared_ptr<boost::thread> thread50(new boost::thread(boost::bind(thread_dbg_test_perf50,i+1)));
	//	threads.push_back(thread50);
	//	boost::shared_ptr<boost::thread> thread51(new boost::thread(boost::bind(thread_dbg_test_perf51,i+1)));
	//	threads.push_back(thread51);
	//	boost::shared_ptr<boost::thread> thread52(new boost::thread(boost::bind(thread_dbg_test_perf52,i+1)));
	//	threads.push_back(thread52);
	//	boost::shared_ptr<boost::thread> thread53(new boost::thread(boost::bind(thread_dbg_test_perf53,i+1)));
	//	threads.push_back(thread53);
	//	boost::shared_ptr<boost::thread> thread54(new boost::thread(boost::bind(thread_dbg_test_perf54,i+1)));
	//	threads.push_back(thread54);
	//	boost::shared_ptr<boost::thread> thread55(new boost::thread(boost::bind(thread_dbg_test_perf55,i+1)));
	//	threads.push_back(thread55);
	//	boost::shared_ptr<boost::thread> thread56(new boost::thread(boost::bind(thread_dbg_test_perf56,i+1)));
	//	threads.push_back(thread56);
	//	boost::shared_ptr<boost::thread> thread57(new boost::thread(boost::bind(thread_dbg_test_perf57,i+1)));
	//	threads.push_back(thread57);
	//	boost::shared_ptr<boost::thread> thread58(new boost::thread(boost::bind(thread_dbg_test_perf58,i+1)));
	//	threads.push_back(thread58);
	//	boost::shared_ptr<boost::thread> thread59(new boost::thread(boost::bind(thread_dbg_test_perf59,i+1)));
	//	threads.push_back(thread59);
	//	boost::shared_ptr<boost::thread> thread60(new boost::thread(boost::bind(thread_dbg_test_perf60,i+1)));
	//	threads.push_back(thread60);
	//	boost::shared_ptr<boost::thread> thread666(new boost::thread(boost::bind(thread_dbg_test_perf666,i+1)));
	//	threads.push_back(thread666);

	//	//boost::shared_ptr<boost::thread> thread61(new boost::thread(boost::bind(thread_dbg_test_perf61,i+1)));
	//	//threads.push_back(thread61);
	//	//boost::shared_ptr<boost::thread> thread62(new boost::thread(boost::bind(thread_dbg_test_perf62,i+1)));
	//	//threads.push_back(thread62);
	//	//boost::shared_ptr<boost::thread> thread63(new boost::thread(boost::bind(thread_dbg_test_perf63,i+1)));
	//	//threads.push_back(thread63);
	//	//boost::shared_ptr<boost::thread> thread64(new boost::thread(boost::bind(thread_dbg_test_perf64,i+1)));
	//	//threads.push_back(thread64);
	//	//boost::shared_ptr<boost::thread> thread65(new boost::thread(boost::bind(thread_dbg_test_perf65,i+1)));
	//	//threads.push_back(thread65);
	//	//boost::shared_ptr<boost::thread> thread66(new boost::thread(boost::bind(thread_dbg_test_perf66,i+1)));
	//	//threads.push_back(thread66);
	//}

	getchar();

	//run = false;
	//for (std::size_t i = 0; i < threads.size(); ++i)
	//	threads[i]->join();

	WSACleanup();
}

#endif /*__INCLUDE_TEST_DGB_PERF_H__*/