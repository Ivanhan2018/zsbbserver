#ifndef __INCLUDE_TEST_DGB_H__
#define __INCLUDE_TEST_DGB_H__

#include <windows.h>
#include "DB_Table.h"
#include "DB_Proxy.h"

template < class type > bool mem_check(type & t1, type & t2)
{
	return (memcmp((void *)&t1, (void *)&t2, sizeof(type)) == 0);
}

template < class type, class typeset> void dbg_test(typeset tset)
{
	DB_Proxy proxy;
	if (!proxy.init("127.0.0.1", 8899,11))
	{
		printf("连接DBG失败\n");
		return ;
	}

	type t_ins, t_upt;
	tset(t_ins, t_upt);

	if (proxy.db_insert(t_ins) != DB_Proxy::DB_SUCCEED)
	{
		printf("%s 添加: 失败\n", type::getname());
		proxy.fini();
		return ;
	}

	type t_slt_ins = t_ins;
	if (proxy.db_select(t_slt_ins) != DB_Proxy::DB_SUCCEED)
	{
		printf("%s 添加: 查询失败\n", type::getname());
		proxy.fini();
		return ;
	}

	if (!mem_check<type>(t_ins, t_slt_ins))
	{
		printf("%s 添加: 数据不一至\n", type::getname());
		proxy.fini();
		return ;
	}

	t_upt.setkeyvalue(t_ins.getkeyvalue());
	if (proxy.db_update(t_ins, t_upt) != DB_Proxy::DB_SUCCEED)
	{
		printf("%s 更新: 失败\n", type::getname());
		proxy.fini();
		return ;
	}
	
	type t_slt_upt = t_upt;
	if (proxy.db_select(t_slt_upt) != DB_Proxy::DB_SUCCEED)
	{
		printf("%s 更新: 查询失败\n", type::getname());
		proxy.fini();
		return ;
	}

	if (!mem_check<type>(t_upt, t_slt_upt))
	{
		printf("%s 更新: 数据不一至\n", type::getname());
		proxy.fini();
		return ;
	}

	if (proxy.db_delete(t_upt) != DB_Proxy::DB_SUCCEED)
	{
		printf("%s 删除: 失败\n", type::getname());
		proxy.fini();
		return ;
	}

	printf("%s 测试成功\n", type::getname());
}

//void GameCityInfo_set(GameCityInfo & t1, GameCityInfo & t2)
//{
//	memset(&t1, 1, sizeof(t1));
//	t1.setmasterid(1);
//	t1.setdetailid(1);
//
//	memset(&t2, 2, sizeof(t2));
//	t2.setmasterid(1);
//	t2.setdetailid(1);
//}

void PlayerSoldierRace_set(PlayerSoldierRace & t1, PlayerSoldierRace & t2)
{
	memset(&t1, 1, sizeof(t1));
	t1.setmasterid(1, 1);
	t1.setdetailid(1);

	memset(&t2, 2, sizeof(t2));
	t2.setmasterid(1, 1);
	t2.setdetailid(1);
}

void ProductionAmount_set(ProductionAmount & t1, ProductionAmount & t2)
{
	memset(&t1, 1, sizeof(t1));
	t1.setmasterid(1, 1, 1, 1);
	t1.setdetailid(1);

	memset(&t2, 2, sizeof(t2));
	t2.setmasterid(1, 1, 1, 1);
	t2.setdetailid(1);
}

//void ConstructionConfigure_set(ConstructionConfigure & t1, ConstructionConfigure & t2)
//{
//	t1.setid(1, 1);
//	t1.woodcount_ = 100;
//
//	t2.setid(1, 1);
//	t2.woodcount_ = 200;
//}

//void ResearchConfigure_set(ResearchConfigure & t1, ResearchConfigure & t2)
//{
//	memset(&t1, 1, sizeof(t1));
//	t1.setmasterid(1);
//	t1.setdetailid(1, 1);
//
//	memset(&t2, 2, sizeof(t2));
//	t2.setmasterid(1);
//	t2.setdetailid(1, 1);
//}

void PlayerConstruction_set(PlayerConstruction & t1, PlayerConstruction & t2)
{
	memset(&t1, 1, sizeof(t1));
	t1.setmasterid(1, 1, 1);
	t1.setdetailid(1);

	memset(&t2, 2, sizeof(t2));
	t2.setmasterid(1, 1, 1);
	t2.setdetailid(1);
}

void PlayerResource_set(PlayerResource & t1, PlayerResource & t2)
{
	memset(&t1, 1, sizeof(t1));
	t1.setmasterid(1, 1);
	t1.setdetailid(1);

	memset(&t2, 2, sizeof(t2));
	t2.setmasterid(1, 1);
	t2.setdetailid(1);
}

void ArmyTable_set(ArmyTable & t1, ArmyTable & t2)
{
	t1.setmasterid(1);
	t1.setdetailid(1);

	t2.setmasterid(2);
	t2.setdetailid(1);
}

void Market_set(Market & t1, Market & t2)
{
	t1.setmasterid(1, 1);
	t1.setdetailid(1);
	t1.minprice_ = 100;

	t2.setmasterid(1, 1);
	t2.setdetailid(1);
	t2.minprice_ = 200;
}

void ConstructionBuild_set(ConstructionBuild & t1, ConstructionBuild & t2)
{
	memset(&t1, 1, sizeof(t1));
	t1.setmasterid(1, 1, 1);
	t1.setdetailid(1, 1, 1);

	memset(&t2, 2, sizeof(t2));
	t2.setmasterid(1, 1, 1);
	t2.setdetailid(1, 1, 1);
}

void ProductionActivity_set(ProductionActivity & t1, ProductionActivity & t2)
{
	memset(&t1, 1, sizeof(t1));
	t1.setmasterid(1, 1, 1, 1);
	t1.setdetailid(1, 1, 1);

	memset(&t2, 2, sizeof(t2));
	t2.setmasterid(1, 1, 1, 1);
	t2.setdetailid(1, 1, 1);
}

void ConsortiaInfo_set(ConsortiaInfo & t1, ConsortiaInfo & t2)
{
	t1.setid(1);
	t1.count_ = 100;

	t2.setid(1);
	t2.count_ = 120;
}

void ConsortiaBank_set(ConsortiaBank & t1, ConsortiaBank & t2)
{
	memset(&t1, 1, sizeof(t1));
	t1.setmasterid(1);
	t1.setdetailid(1);

	memset(&t2, 2, sizeof(t2));
	t2.setmasterid(1);
	t2.setdetailid(1);
}

void ConsortiaUnion_set(ConsortiaUnion & t1, ConsortiaUnion & t2)
{
	t1.setid(1, 1);
	t1.unionid_ = 100;

	t2.setid(1, 1);
	t2.unionid_ = 200;
}

void PlayerOfficerItemTable_set(PlayerOfficerItemTable & t1, PlayerOfficerItemTable & t2)
{
	memset(&t1, 1, sizeof(t1));
	t1.setmasterid(1, 1);
	t1.setdetailid(1);

	memset(&t2, 2, sizeof(t2));
	t2.setmasterid(1, 1);
	t2.setdetailid(1);
}

void PlayerItemStorageTable_set(PlayerItemStorageTable & t1, PlayerItemStorageTable & t2)
{
	t1.setmasterid(1, 1);
	t1.setdetailid(1);

	t2.setmasterid(1, 1);
	t2.setdetailid(1);
}


void SoldierParamTable_set(SoldierParamTable & t1, SoldierParamTable & t2)
{
	t1.setid(1);
	t1.hp_ = 1000;

	t2.setid(1);
	t2.hp_ = 2000;
}

void PlayerOfficerTable_set(PlayerOfficerTable & t1, PlayerOfficerTable & t2)
{
	t1.setmasterid(1);
	t1.setdetailid(1);
	t1.jobid_ = 100;

	t2.setmasterid(1);
	t2.setdetailid(1);
	t2.jobid_ = 200;
}

void PlayerTask_set(PlayerTask & t1, PlayerTask & t2)
{
	memset(&t1, 1, sizeof(t1));
	t1.setmasterid(1);
	t1.setdetailid(1);

	memset(&t2, 2, sizeof(t2));
	t2.setmasterid(1);
	t2.setdetailid(1);
}

void ConsortiaPersonnel_set(ConsortiaPersonnel & t1, ConsortiaPersonnel & t2)
{
	memset(&t1, 1, sizeof(t1));
	t1.setmasterid(1);
	t1.setdetailid(1);

	memset(&t2, 2, sizeof(t2));
	t2.setmasterid(1);
	t2.setdetailid(1);
}

void CellPrice_set(CellPrice & t1, CellPrice & t2)
{
	t1.setid(1);
	t1.foodprice_ = 100;

	t2.setid(1);
	t2.foodprice_ = 200;
}

void CityAttribute_set(CityAttribute & t1, CityAttribute & t2)
{
	memset(&t1, 1, sizeof(t1));
	t1.setmasterid(1);
	t1.setdetailid(1);

	memset(&t2, 2, sizeof(t2));
	t2.setmasterid(1);
	t2.setdetailid(1);
}

//void CreateConsortia_set(CreateConsortia & t1, CreateConsortia & t2)
//{
//	t1.setid(1);
//	t1.woodcount_ = 100;
//
//	t2.setid(1);
//	t2.woodcount_ = 200;
//}

void ConsortiaFight_set(ConsortiaFight & t1, ConsortiaFight & t2)
{
	memset(&t1, 1, sizeof(t1));
	t1.setmasterid(1);
	t1.setdetailid(1, 1);

	memset(&t2, 2, sizeof(t2));
	t2.setmasterid(1);
	t2.setdetailid(1, 1);
}

//void TaskBaseTable_set(TaskBaseTable & t1, TaskBaseTable & t2)
//{
//	t1.setid(1);
//	t1.tasklevel_ = 100;
//
//	t2.setid(1);
//	t2.tasklevel_ = 200;
//}

//void ProduceConfigure_set(ProduceConfigure & t1, ProduceConfigure & t2)
//{
//	memset(&t1, 1, sizeof(t1));
//	t1.setmasterid(1);
//	t1.setdetailid(1, 1);
//
//	memset(&t2, 2, sizeof(t2));
//	t2.setmasterid(1);
//	t2.setdetailid(1, 1);
//}

void ChatRoomTable_set(ChatRoomTable & t1, ChatRoomTable & t2)
{
	t1.setid(1);
	t1.player2_ = 100;

	t2.setid(1);
	t2.player2_ = 200;
}

void PlayerChatRoomTable_set(PlayerChatRoomTable & t1, PlayerChatRoomTable & t2)
{
	t1.setid(1);
	t1.money_ = 100;

	t2.setid(1);
	t2.money_ = 200;
}

void PlayerCity_set(PlayerCity & t1, PlayerCity & t2)
{
	memset(&t1, 1, sizeof(t1));
	t1.setmasterid(1);
	t1.setdetailid(1);

	memset(&t2, 2, sizeof(t2));
	t2.setmasterid(1);
	t2.setdetailid(1);
}

void OfficerRaceAttribute_set(OfficerRaceAttribute & t1, OfficerRaceAttribute & t2)
{
	t1.setid(1);
	t1.money_ = 100;

	t2.setid(1);
	t2.money_ = 200;
}

//void WeaponAttribute_set(WeaponAttribute & t1, WeaponAttribute & t2)
//{
//	memset(&t1, 1, sizeof(t1));
//	t1.setmasterid(1);
//	t1.setdetailid(1);
//
//	memset(&t2, 2, sizeof(t2));
//	t2.setmasterid(1);
//	t2.setdetailid(1);
//}

//void DefendAttribute_set(DefendAttribute & t1, DefendAttribute & t2)
//{
//	t1.setid(1);
//	t1.cost_ = 101;
//
//	t2.setid(1);
//	t2.cost_ = 201;
//}

void ArmyOutTable_set(ArmyOutTable & t1, ArmyOutTable & t2)
{
	memset(&t1, 1, sizeof(t1));
	t1.setmasterid(1);
	t1.setdetailid(1);

	memset(&t2, 2, sizeof(t2));
	t2.setmasterid(1);
	t2.setdetailid(1);
}

//void OfficerTech_set(OfficerTech & t1, OfficerTech & t2)
//{
//	memset(&t1, 1, sizeof(t1));
//	t1.setmasterid(1);
//	t1.setdetailid(1);
//
//	memset(&t2, 2, sizeof(t2));
//	t2.setmasterid(1);
//	t2.setdetailid(1);
//}

void NPC_set(NPC & t1, NPC & t2)
{
	t1.setid(1);
	t1.level_ = 100;

	t2.setid(1);
	t2.level_ = 200;
}

void MapInfoTable_set(MapInfoTable & t1, MapInfoTable & t2)
{
	t1.setid(1);
	t1.countryid_ = 100;

	t2.setid(1);
	t2.countryid_ = 200;
}

void ResearchItem_set(ResearchItem & t1, ResearchItem & t2)
{
	memset(&t1, 1, sizeof(t1));
	t1.setmasterid(1,1);
	t1.setdetailid(1);

	memset(&t2, 2, sizeof(t2));
	t2.setmasterid(1,1);
	t2.setdetailid(1);
}

void EctypeBaseTable_set(EctypeBaseTable & t1, EctypeBaseTable & t2)
{
	t1.setid(1);
	t1.level_ = 100;

	t2.setid(1);
	t2.level_ = 200;
}

void NPCArmyTable_set(NPCArmyTable & t1, NPCArmyTable & t2)
{
	t1.setmasterid(1);
	t1.setdetailid(1);
	t1.fsoldierlevel_ = 100;

	t2.setmasterid(1);
	t2.setdetailid(1);
	t2.fsoldierlevel_ = 200;
}

void PlayerCaptiveHero_set(PlayerCaptiveHero & t1, PlayerCaptiveHero & t2)
{
	memset(&t1, 1, sizeof(t1));
	t1.setmasterid(1);
	t1.setdetailid(1, 1, 1);

	memset(&t2, 2, sizeof(t2));
	t2.setmasterid(1);
	t2.setdetailid(1, 1, 1);
}

//void EquipAttribute_set(EquipAttribute & t1, EquipAttribute & t2)
//{
//	t1.setid(1);
//	t1.level_ = 100;
//
//	t2.setid(1);
//	t2.level_ = 200;
//}

void WarNote_set(WarNote & t1, WarNote & t2)
{
	t1.setmasterid(1,1);
	t1.setdetailid(1);
	t1.state_ = 100;

	t2.setmasterid(1,1);
	t2.setdetailid(1);
	t2.state_ = 200;
}

void PullulateNote_set(PullulateNote & t1, PullulateNote & t2)
{
	memset(&t1, 1, sizeof(t1));
	t1.setmasterid(1,1);
	t1.setdetailid(1);

	memset(&t2, 2, sizeof(t2));
	t2.setmasterid(1,1);
	t2.setdetailid(1);
}

void FirendTable_set(FirendTable & t1, FirendTable & t2)
{
	t1.setmasterid(1,1);
	t1.setdetailid(1);
	t1.militaryrank_ = 100;

	t2.setmasterid(1,1);
	t2.setdetailid(1);
	t2.militaryrank_ = 200;
}

//void ConsortiaOfficial_set(ConsortiaOfficial & t1, ConsortiaOfficial & t2)
//{
//	t1.setid(1);
//	t1.level_ = 100;
//
//	t2.setid(1);
//	t2.level_ = 200;
//}

void LoadMaterial_set(LoadMaterial & t1, LoadMaterial & t2)
{
	memset(&t1, 1, sizeof(t1));
	t1.setmasterid(1,1);
	t1.setdetailid(1,1);

	memset(&t2, 2, sizeof(t2));
	t2.setmasterid(1,1);
	t2.setdetailid(1,1);
}

void Treasury_set(Treasury & t1, Treasury & t2)
{
	t1.setid(1);
	t1.money_ = 100;

	t2.setid(1);
	t2.money_ = 200;
}


//void CardConfig_set(CardConfig & t1, CardConfig & t2)
//{
//	t1.setid(1);
//	t1.type_ = 100;
//
//	t2.setid(1);
//	t2.type_ = 200;
//}


void OfficerJobAttribute_set(OfficerJobAttribute & t1, OfficerJobAttribute & t2)
{
	t1.setid(1);
	t1.evage_ = 100;

	t2.setid(1);
	t2.evage_ = 200;
}

void PublicBuildParamTable_set(PublicBuildParamTable & t1, PublicBuildParamTable & t2)
{
	t1.setid(1,1);
	t1.hp_ = 100;

	t2.setid(1,1);
	t2.hp_ = 200;
}

void PublicBuildTable_set(PublicBuildTable & t1, PublicBuildTable & t2)
{
	memset(&t1, 1, sizeof(t1));
	t1.setmasterid(1);
	t1.setdetailid(1);

	memset(&t2, 2, sizeof(t2));
	t2.setmasterid(1);
	t2.setdetailid(1);
}

void PlayerProduceState_set(PlayerProduceState & t1, PlayerProduceState & t2)
{
	memset(&t1, 1, sizeof(t1));
	t1.setmasterid(1);
	t1.setdetailid(1);

	memset(&t2, 2, sizeof(t2));
	t2.setmasterid(1);
	t2.setdetailid(1);
}

//void JobParam_set(JobParam & t1, JobParam & t2)
//{
//	t1.setid(1);
//	t1.endurance_ = 100;
//
//	t2.setid(1);
//	t2.endurance_ = 200;
//}

//void BinConfig_set(BinConfig & t1, BinConfig & t2)
//{
//	t1.setid(1);
//	strcpy(t1.sureflopitem_, "test");
//
//	t2.setid(1);
//	strcpy(t2.sureflopitem_, "haheh");
//}

void EquipAntagonism_set(EquipAntagonism & t1, EquipAntagonism & t2)
{
	t1.setid(1,1);
	t1.attackodds_ = 100;

	t2.setid(1,1);
	t2.attackodds_ = 200;
}

void BatttleField_set(BatttleField & t1, BatttleField & t2)
{
	t1.setid(1);
	t1.startm_ = 100;

	t2.setid(1);
	t2.startm_ = 200;
}

void ConsortiaCompare_set(ConsortiaCompare & t1, ConsortiaCompare & t2)
{
	t1.setid(1);
	t1.level_ = 100;

	t2.setid(1);
	t2.level_ = 200;
}

void PlayerCompare_set(PlayerCompare & t1, PlayerCompare & t2)
{
	t1.setid(1);
	t1.level_ = 100;

	t2.setid(1);
	t2.level_ = 200;
}

void Council_set(Council & t1, Council & t2)
{
	t1.setmasterid(1,1);
	t1.setdetailid(1);
	t1.vote_ = 100;

	t2.setmasterid(1,1);
	t2.setdetailid(1);
	t2.vote_ = 200;
}

void Candidate_set(Candidate & t1, Candidate & t2)
{
	t1.setmasterid(1,1);
	t1.setdetailid(1);
	t1.vote_ = 100;

	t2.setmasterid(1,1);
	t2.setdetailid(1);
	t2.vote_ = 300;
}

void Ballot_set(Ballot & t1, Ballot & t2)
{
	memset(&t1, 1, sizeof(t1));
	t1.setmasterid(1,1);
	t1.setdetailid(1);

	memset(&t2, 2, sizeof(t2));
	t2.setmasterid(1,1);
	t2.setdetailid(1);
}

void Country_set(Country & t1, Country & t2)
{
	t1.setid(1);
	t1.dodmoney_ = 100;

	t2.setid(1);
	t2.dodmoney_ = 200;
}

void ResearchActivity_set(ResearchActivity & t1, ResearchActivity & t2)
{
	memset(&t1, 1, sizeof(t1));
	t1.setmasterid(1);
	t1.setdetailid(1,1);

	memset(&t2, 2, sizeof(t2));
	t2.setmasterid(1);
	t2.setdetailid(1,1);
}

void LevySoldierActivity_set(LevySoldierActivity & t1, LevySoldierActivity & t2)
{
	memset(&t1, 1, sizeof(t1));
	t1.setmasterid(1,1,1);
	t1.setdetailid(1);

	memset(&t2, 2, sizeof(t2));
	t2.setmasterid(1,1,1);
	t2.setdetailid(1);
}

void MailTable_set(MailTable & t1, MailTable & t2)
{
	struct mystruct
	{
		int id;
		char name[14];
		int tel;
		char addr[20];
	};

	t1.setmasterid(1);
	t1.setdetailid(1);
	mystruct * ms1 = (mystruct *)t1.pertain_;
	ms1->id = 1001;
	strcpy(ms1->name, "mystruct1");
	ms1->tel = 1234561;
	strcpy(ms1->addr, "test test1");

	t2.setmasterid(1);
	t2.setdetailid(1);
	mystruct * ms2 = (mystruct *)t2.pertain_;
	ms2->id = 1002;
	strcpy(ms2->name, "mystruct2");
	ms2->tel = 1234562;
	strcpy(ms2->addr, "test test2");
}

void NewsTable_set(NewsTable & t1, NewsTable & t2)
{
	struct mystruct
	{
		int id;
		char name[14];
		int tel;
		char addr[20];
	};

	t1.setid(1);
	t1.newstype_ = 100;
	mystruct * ms1 = (mystruct *)t1.content_;
	ms1->id = 1001;
	strcpy(ms1->name, "mystruct1");
	ms1->tel = 1234561;
	strcpy(ms1->addr, "test test1");

	t2.setid(1);
	t2.newstype_ = 200;
	mystruct * ms2 = (mystruct *)t2.content_;
	ms2->id = 1002;
	strcpy(ms2->name, "mystruct2");
	ms2->tel = 1234562;
	strcpy(ms2->addr, "test test2");
}

void ChargeSvrTimer_set(ChargeSvrTimer & t1, ChargeSvrTimer & t2)
{
	struct mystruct
	{
		int id;
		char name[14];
		int tel;
		char addr[20];
	};

	t1.setmasterid(1);
	t1.setdetailid(1);
	mystruct * ms1 = (mystruct *)t1.param_;
	ms1->id = 1001;
	strcpy(ms1->name, "mystruct1");
	ms1->tel = 1234561;
	strcpy(ms1->addr, "test test1");

	t2.setmasterid(1);
	t2.setdetailid(1);
	mystruct * ms2 = (mystruct *)t2.param_;
	ms2->id = 1002;
	strcpy(ms2->name, "mystruct2");
	ms2->tel = 1234562;
	strcpy(ms2->addr, "test test2");
}

void test_dbg_main()
{
	WSADATA wsaData;
	WSAStartup( 0x0202, &wsaData );

//	dbg_test<GameCityInfo>(GameCityInfo_set);
	dbg_test<PlayerSoldierRace>(PlayerSoldierRace_set);
	dbg_test<ProductionAmount>(ProductionAmount_set);
//	dbg_test<ConstructionConfigure>(ConstructionConfigure_set);
//	dbg_test<ResearchConfigure>(ResearchConfigure_set);
	dbg_test<PlayerConstruction>(PlayerConstruction_set);
	dbg_test<PlayerResource>(PlayerResource_set);
	dbg_test<ArmyTable>(ArmyTable_set);
	dbg_test<Market>(Market_set);
	dbg_test<ConstructionBuild>(ConstructionBuild_set);
	dbg_test<ProductionActivity>(ProductionActivity_set);
	dbg_test<ConsortiaInfo>(ConsortiaInfo_set);
	dbg_test<ConsortiaBank>(ConsortiaBank_set);
	dbg_test<ConsortiaUnion>(ConsortiaUnion_set);
	dbg_test<PlayerOfficerItemTable>(PlayerOfficerItemTable_set);
	dbg_test<PlayerItemStorageTable>(PlayerItemStorageTable_set);
	dbg_test<SoldierParamTable>(SoldierParamTable_set);
	dbg_test<PlayerOfficerTable>(PlayerOfficerTable_set);
	dbg_test<PlayerTask>(PlayerTask_set);
	dbg_test<ConsortiaPersonnel>(ConsortiaPersonnel_set);
	dbg_test<CellPrice>(CellPrice_set);
	dbg_test<CityAttribute>(CityAttribute_set);
//	dbg_test<CreateConsortia>(CreateConsortia_set);
	dbg_test<ConsortiaFight>(ConsortiaFight_set);
//	dbg_test<TaskBaseTable>(TaskBaseTable_set);
//	dbg_test<ProduceConfigure>(ProduceConfigure_set);
	dbg_test<ChatRoomTable>(ChatRoomTable_set);
	dbg_test<PlayerChatRoomTable>(PlayerChatRoomTable_set);
	dbg_test<PlayerCity>(PlayerCity_set);
	dbg_test<OfficerRaceAttribute>(OfficerRaceAttribute_set);
//	dbg_test<WeaponAttribute>(WeaponAttribute_set);
//	dbg_test<DefendAttribute>(DefendAttribute_set);
	dbg_test<ArmyOutTable>(ArmyOutTable_set);
//	dbg_test<OfficerTech>(OfficerTech_set);
	dbg_test<NPC>(NPC_set);
	dbg_test<MapInfoTable>(MapInfoTable_set);
	dbg_test<ResearchItem>(ResearchItem_set);
	dbg_test<EctypeBaseTable>(EctypeBaseTable_set);
	dbg_test<NPCArmyTable>(NPCArmyTable_set);
	dbg_test<PlayerCaptiveHero>(PlayerCaptiveHero_set);
//	dbg_test<EquipAttribute>(EquipAttribute_set);
	dbg_test<WarNote>(WarNote_set);
	dbg_test<PullulateNote>(PullulateNote_set);
	dbg_test<FirendTable>(FirendTable_set);
//	dbg_test<ConsortiaOfficial>(ConsortiaOfficial_set);
	dbg_test<LoadMaterial>(LoadMaterial_set);
	dbg_test<Treasury>(Treasury_set);
//	dbg_test<CardConfig>(CardConfig_set);
	dbg_test<PublicBuildParamTable>(PublicBuildParamTable_set);
	dbg_test<PublicBuildTable>(PublicBuildTable_set);
	dbg_test<PlayerProduceState>(PlayerProduceState_set);
//	dbg_test<JobParam>(JobParam_set);
//	dbg_test<BinConfig>(BinConfig_set);
	dbg_test<EquipAntagonism>(EquipAntagonism_set);
	dbg_test<BatttleField>(BatttleField_set);
	dbg_test<ConsortiaCompare>(ConsortiaCompare_set);
	dbg_test<PlayerCompare>(PlayerCompare_set);
	dbg_test<Council>(Council_set);
	dbg_test<Candidate>(Candidate_set);
	dbg_test<Ballot>(Ballot_set);
	dbg_test<Country>(Country_set);
	dbg_test<ResearchActivity>(ResearchActivity_set);
	dbg_test<LevySoldierActivity>(LevySoldierActivity_set);
	dbg_test<MailTable>(MailTable_set);
	dbg_test<NewsTable>(NewsTable_set);
	dbg_test<ChargeSvrTimer>(ChargeSvrTimer_set);
	dbg_test<OfficerJobAttribute>(OfficerJobAttribute_set);

	WSACleanup();
}


#endif /*__INCLUDE_TEST_DGB_H__*/