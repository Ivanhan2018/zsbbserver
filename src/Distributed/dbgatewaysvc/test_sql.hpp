#ifndef __INCLUDE_TEST_SQL_H__
#define __INCLUDE_TEST_SQL_H__

#include <iostream>
#include <stdio.h>
#include "db_table.h"
#include <time.h>

#define OTL_STL
#define OTL_ODBC

#if defined (WIN32)
#define OTL_BIGINT __int64
#else
#define OTL_BIGINT long long
#endif
#include "otlv4.h"

otl_connect db_;
char buf[10000000];

template < class type, class func > void sql_test_perf(func f)
{
	type t;
	db_.set_max_long_size(10000000);
	otl_stream o;
	std::string count = "select count(1) from ";
	count += type::getname();

	int pos, i;
	long it, ut, dt;

	std::string del = "delete from ";
	del += type::getname();

	otl_cursor::direct_exec(db_, del.c_str());
	db_.commit();

	pos = 0;
	pos += sprintf(buf, "%s", "exec(\'");
	it = GetTickCount();
	for (i = 0; i < 2000; i++)
	{
		f(t, i + 1);
		pos += t.get_insert_str(buf + pos);
	}
	pos += sprintf(buf + pos, "%s", " \')");
	buf[pos] = '\0';
	otl_cursor::direct_exec(db_, buf);
	db_.commit();
	it = GetTickCount() - it;

	o.open(count.size(), count.c_str(), db_);
	o >> pos;
	o.close();
	if (pos == 0) throw std::runtime_error("记录添加失败");

	pos = 0;
	pos += sprintf(buf, "%s", "exec(\'");
	ut = GetTickCount();
	for (i = 0; i < 2000; i++)
	{
		f(t, i + 1);
		pos += t.get_update_str(buf + pos, t);
	}
	pos += sprintf(buf + pos, "%s", " \')");
	buf[pos] = '\0';
	otl_cursor::direct_exec(db_, buf);
	db_.commit();
	ut = GetTickCount() - ut;

	pos = 0;
	pos += sprintf(buf, "%s", "exec(\'");
	dt = GetTickCount();
	for (i = 0; i < 2000; i++)
	{
		f(t, i + 1);
		pos += t.get_delete_str(buf + pos);
	}
	pos += sprintf(buf + pos, "%s", " \')");
	buf[pos] = '\0';
	otl_cursor::direct_exec(db_, buf);
	db_.commit();
	dt = GetTickCount() - dt;

	o.open(count.size(), count.c_str(), db_);
	o >> pos;
	o.close();
	if (pos != 0) throw std::runtime_error("记录删除失败");

	printf("表 %s 操作次数 %d 插入耗时(%d) 更新耗时(%d) 删除耗时(%d)\n", type::getname(), i * 3, it, ut, dt);
	Sleep(1);
}

//void GameCityInfo_func(GameCityInfo & t, int i)
//{
//	t.setmasterid(i);
//	t.setdetailid(i);
//}

void PlayerSoldierRace_func(PlayerSoldierRace & t, int i)
{
	t.setmasterid(i, i);
	t.completetimes_ = time(0);
	t.setdetailid(i);
}

void ProductionAmount_func(ProductionAmount & t, int i)
{
	t.setmasterid(i, i, i, i);
	t.setdetailid(i);
}

//void ConstructionConfigure_func(ConstructionConfigure & t, int i)
//{
//	t.setid(i, i);
//	t.completetimes_ = time(0);
//}

//void ResearchConfigure_func(ResearchConfigure & t, int i)
//{
//	t.setmasterid(i);
//	t.setdetailid(i, i);
//	t.completetimes_ = time(0);
//}

void PlayerConstruction_func(PlayerConstruction & t, int i)
{
	t.setmasterid(i, i, i);
	t.setdetailid(i);
	t.completetimes_ = time(0);
}

void PlayerResource_func(PlayerResource & t, int i)
{
	t.setmasterid(i, i);
	t.setdetailid(i);
	t.completetimes_ = time(0);
}

void ArmyTable_func(ArmyTable & t, int i)
{
	t.setmasterid(i);
	t.setdetailid(i);
}

void Market_func(Market & t, int i)
{
	t.setmasterid(i, i);
	t.setdetailid(i);
	t.starttime_ = time(0);
}

void ConstructionBuild_func(ConstructionBuild & t, int i)
{
	t.setmasterid(i, i, i);
	t.setdetailid(i, i, i);
	t.starttime_ = time(0);
}

void ProductionActivity_func(ProductionActivity & t, int i)
{
	t.setmasterid(i, i, i, i);
	t.setdetailid(i, i, i);
	t.starttime_ = time(0);
}

void ConsortiaInfo_func(ConsortiaInfo & t, int i)
{
	t.setid(i);
	t.createtime_ = time(0);
}

void ConsortiaBank_func(ConsortiaBank & t, int i)
{
	t.setmasterid(i);
	t.setdetailid(i);
	t.itemcount_ = i;
}

void ConsortiaUnion_func(ConsortiaUnion & t, int i)
{
	t.setid(i, i);
}

void PlayerOfficerItemTable_func(PlayerOfficerItemTable & t, int i)
{
	t.setmasterid(i, i);
	t.setdetailid(i);
}

void PlayerItemStorageTable_func(PlayerItemStorageTable & t, int i)
{
	t.setmasterid(i, i);
	t.setdetailid(i);
}


void SoldierParamTable_func(SoldierParamTable & t, int i)
{
	t.setid(i);
}

void PlayerOfficerTable_func(PlayerOfficerTable & t, int i)
{
	t.setmasterid(i);
	t.setdetailid(i);
	t.updatetime_ = t.gbhtime_ = time(0);
}

void PlayerTask_func(PlayerTask & t, int i)
{
	t.setmasterid(i);
	t.setdetailid(i);
	t.tasktime_ = time(0);
}

void ConsortiaPersonnel_func(ConsortiaPersonnel & t, int i)
{
	t.setmasterid(i);
	t.setdetailid(i);
}

void CellPrice_func(CellPrice & t, int i)
{
	t.setid(i);
}

void CityAttribute_func(CityAttribute & t, int i)
{
	t.setmasterid(i);
	t.setdetailid(i);
}

//void CreateConsortia_func(CreateConsortia & t, int i)
//{
//	t.setid(i);
//}

void ConsortiaFight_func(ConsortiaFight & t, int i)
{
	t.setmasterid(i);
	t.setdetailid(i, i);
	t.time_ = time(0);
}

//void TaskBaseTable_func(TaskBaseTable & t, int i)
//{
//	t.setid(i);
//}

//void ProduceConfigure_func(ProduceConfigure & t, int i)
//{
//	t.setmasterid(i);
//	t.setdetailid(i, i);
//	t.completetimes_ = time(0);
//}

void ChatRoomTable_func(ChatRoomTable & t, int i)
{
	t.setid(i);
}

void PlayerChatRoomTable_func(PlayerChatRoomTable & t, int i)
{
	t.setid(i);
}

void PlayerCity_func(PlayerCity & t, int i)
{
	t.setmasterid(i);
	t.setdetailid(i);
}

void OfficerRaceAttribute_func(OfficerRaceAttribute & t, int i)
{
	t.setid(i);
}

//void WeaponAttribute_func(WeaponAttribute & t, int i)
//{
//	t.setmasterid(i);
//	t.setdetailid(i);
//}

//void DefendAttribute_func(DefendAttribute & t, int i)
//{
//	t.setid(i);
//}

void ArmyOutTable_func(ArmyOutTable & t, int i)
{
	t.setmasterid(i);
	t.setdetailid(i);
	t.starttime_ = time(0);
}

//void OfficerTech_func(OfficerTech & t, int i)
//{
//	t.setmasterid(i);
//	t.setdetailid(i);
//}

void NPC_func(NPC & t, int i)
{
	t.setid(i);
}

void MapInfoTable_func(MapInfoTable & t, int i)
{
	t.setid(i);
}

void ResearchItem_func(ResearchItem & t, int i)
{
	t.setmasterid(i,i);
	t.setdetailid(i);
}

void EctypeBaseTable_func(EctypeBaseTable & t, int i)
{
	t.setid(i);
}

void NPCArmyTable_func(NPCArmyTable & t, int i)
{
	t.setmasterid(i);
	t.setdetailid(i);
}

void PlayerCaptiveHero_func(PlayerCaptiveHero & t, int i)
{
	t.setmasterid(i);
	t.setdetailid(i, i, i);
}

//void EquipAttribute_func(EquipAttribute & t, int i)
//{
//	t.setid(i);
//}

void WarNote_func(WarNote & t, int i)
{
	t.setmasterid(i,i);
	t.setdetailid(i);
}

void PullulateNote_func(PullulateNote & t, int i)
{
	t.setmasterid(i,i);
	t.setdetailid(i);
}

void FirendTable_func(FirendTable & t, int i)
{
	t.setmasterid(i,i);
	t.setdetailid(i);
}

//void ConsortiaOfficial_func(ConsortiaOfficial & t, int i)
//{
//	t.setid(i);
//}

void LoadMaterial_func(LoadMaterial & t, int i)
{
	t.setmasterid(i,i);
	t.setdetailid(i,i);
}

void Treasury_func(Treasury & t, int i)
{
	t.setid(i);
}


//void CardConfig_func(CardConfig & t, int i)
//{
//	t.setid(i);
//}

void PublicBuildParamTable_func(PublicBuildParamTable & t, int i)
{
	t.setid(i,i);
}

void PublicBuildTable_func(PublicBuildTable & t, int i)
{
	t.setmasterid(i);
	t.setdetailid(i);
}

void PlayerProduceState_func(PlayerProduceState & t, int i)
{
	t.setmasterid(i);
	t.setdetailid(i);
}

//void JobParam_func(JobParam & t, int i)
//{
//	t.setid(i);
//}

//void BinConfig_func(BinConfig & t, int i)
//{
//	t.setid(i);
//}

void EquipAntagonism_func(EquipAntagonism & t, int i)
{
	t.setid(i,i);
}

void BatttleField_func(BatttleField & t, int i)
{
	t.setid(i);
}

void ConsortiaCompare_func(ConsortiaCompare & t, int i)
{
	t.setid(i);
}

void PlayerCompare_func(PlayerCompare & t, int i)
{
	t.setid(i);
}

void Council_func(Council & t, int i)
{
	t.setmasterid(i,i);
	t.setdetailid(i);
}

void Candidate_func(Candidate & t, int i)
{
	t.setmasterid(i,i);
	t.setdetailid(i);
}

void Ballot_func(Ballot & t, int i)
{
	t.setmasterid(i,i);
	t.setdetailid(i);
}

void Country_func(Country & t, int i)
{
	t.setid(i);
}

void ResearchActivity_func(ResearchActivity & t, int i)
{
	t.setmasterid(i);
	t.setdetailid(i,i);
}

void LevySoldierActivity_func(LevySoldierActivity & t, int i)
{
	t.setmasterid(i,i,i);
	t.setdetailid(i);
}

void MailTable_func(MailTable & t, int i)
{
	t.setmasterid(i);
	t.setdetailid(i);
}

void NewsTable_func(NewsTable & t, int i)
{
	t.setid(i);
}

void OfficerJobAttribute_func(OfficerJobAttribute & t, int i)
{
	t.setid(i);
}

void ChargeSvrTimer_func(ChargeSvrTimer & t, int i)
{
	t.setmasterid(i);
	t.setdetailid(i);
}

void sql_test_mailtable()
{
	struct mystruct
	{
		int id;
		char name[14];
		int tel;
		char addr[20];
	};

	otl_cursor::direct_exec(db_, "delete from MailTable");
	db_.commit();

	MailTable t;

	t.setmasterid(1);
	t.setdetailid(1);

	mystruct * ms = (mystruct *)t.pertain_;

	ms->id = 100;
	strcpy(ms->name, "wangshenwang");
	ms->tel = 1234567;
	strcpy(ms->addr, "test test");

	otl_stream o;
	int len = t.get_insert_str(buf);
	otl_cursor::direct_exec(db_, buf);
	db_.commit();

	std::string str = t.get_select_str();
	o.open(str.size(), str.c_str(), db_);

	MailTable t1;
	o >> t1;

	mystruct * ms1;
	ms1 = (mystruct *)t1.pertain_;

	if ((ms1->id != ms->id) ||
		(strcmp(ms1->name, ms->name)) ||
		(ms1->tel != ms->tel) ||
		(strcmp(ms1->addr, ms->addr)))
		printf("sql_test_mailtable 写内存失败\n");
	else
		printf("sql_test_mailtable 写内存成功\n");
}

void sql_test_NewsTable()
{
	struct mystruct
	{
		int id;
		char name[14];
		int tel;
		char addr[20];
	};

	otl_cursor::direct_exec(db_, "delete from NewsTable");
	db_.commit();

	NewsTable t;

	t.setid(1);

	mystruct * ms = (mystruct *)t.content_;

	ms->id = 100;
	strcpy(ms->name, "wangshenwang");
	ms->tel = 1234567;
	strcpy(ms->addr, "test test");

	otl_stream o;
	int len = t.get_insert_str(buf);
	otl_cursor::direct_exec(db_, buf);
	db_.commit();

	std::string str = t.get_select_str();
	o.open(str.size(), str.c_str(), db_);

	NewsTable t1;
	o >> t1;

	mystruct * ms1;
	ms1 = (mystruct *)t1.content_;

	if ((ms1->id != ms->id) ||
		(strcmp(ms1->name, ms->name)) ||
		(ms1->tel != ms->tel) ||
		(strcmp(ms1->addr, ms->addr)))
		printf("sql_test_NewsTable 写内存失败\n");
	else
		printf("sql_test_NewsTable 写内存成功\n");
}

void sql_test_ChargeSvrTimer()
{
	struct mystruct
	{
		int id;
		char name[14];
		int tel;
		char addr[20];
	};

	otl_cursor::direct_exec(db_, "delete from ChargeSvrTimer");
	db_.commit();

	ChargeSvrTimer t;

	t.setmasterid(1);
	t.setdetailid(1);
	t.time_ = time(0);

	mystruct * ms = (mystruct *)t.param_;

	ms->id = 100;
	strcpy(ms->name, "wangshenwang");
	ms->tel = 1234567;
	strcpy(ms->addr, "test test");

	otl_stream o;
	int len = t.get_insert_str(buf);
	otl_cursor::direct_exec(db_, buf);
	db_.commit();

	std::string str = t.get_select_str();
	o.open(str.size(), str.c_str(), db_);

	ChargeSvrTimer t1;
	o >> t1;

	mystruct * ms1;
	ms1 = (mystruct *)t1.param_;

	if ((ms1->id != ms->id) ||
		(strcmp(ms1->name, ms->name)) ||
		(ms1->tel != ms->tel) ||
		(strcmp(ms1->addr, ms->addr)))
		printf("sql_test_ChargeSvrTimer 写内存失败\n");
	else
		printf("sql_test_ChargeSvrTimer 写内存成功\n");
}

void test_sql_main()
{
	otl_connect::otl_initialize();

	try
	{
		db_.rlogon("UID=sa;PWD=sa;DSN=gsgamedb");

		sql_test_mailtable();
		sql_test_NewsTable();
		sql_test_ChargeSvrTimer();
//		sql_test_perf<GameCityInfo>(GameCityInfo_func);
		sql_test_perf<PlayerSoldierRace>(PlayerSoldierRace_func);
		sql_test_perf<ProductionAmount>(ProductionAmount_func);
//		sql_test_perf<ConstructionConfigure>(ConstructionConfigure_func);
//		sql_test_perf<ResearchConfigure>(ResearchConfigure_func);
		sql_test_perf<PlayerConstruction>(PlayerConstruction_func);
		sql_test_perf<PlayerResource>(PlayerResource_func);
		sql_test_perf<ArmyTable>(ArmyTable_func);
		sql_test_perf<Market>(Market_func);
		sql_test_perf<ConstructionBuild>(ConstructionBuild_func);
		sql_test_perf<ProductionActivity>(ProductionActivity_func);
		sql_test_perf<ConsortiaInfo>(ConsortiaInfo_func);
		sql_test_perf<ConsortiaBank>(ConsortiaBank_func);
		sql_test_perf<ConsortiaUnion>(ConsortiaUnion_func);
		sql_test_perf<PlayerOfficerItemTable>(PlayerOfficerItemTable_func);
		sql_test_perf<PlayerItemStorageTable>(PlayerItemStorageTable_func);
		sql_test_perf<SoldierParamTable>(SoldierParamTable_func);
		sql_test_perf<PlayerOfficerTable>(PlayerOfficerTable_func);
		sql_test_perf<PlayerTask>(PlayerTask_func);
		sql_test_perf<ConsortiaPersonnel>(ConsortiaPersonnel_func);
		sql_test_perf<CellPrice>(CellPrice_func);
		sql_test_perf<CityAttribute>(CityAttribute_func);
//		sql_test_perf<CreateConsortia>(CreateConsortia_func);
		sql_test_perf<ConsortiaFight>(ConsortiaFight_func);
//		sql_test_perf<TaskBaseTable>(TaskBaseTable_func);
//		sql_test_perf<ProduceConfigure>(ProduceConfigure_func);
		sql_test_perf<ChatRoomTable>(ChatRoomTable_func);
		sql_test_perf<PlayerChatRoomTable>(PlayerChatRoomTable_func);
		sql_test_perf<PlayerCity>(PlayerCity_func);
		sql_test_perf<OfficerRaceAttribute>(OfficerRaceAttribute_func);
//		sql_test_perf<WeaponAttribute>(WeaponAttribute_func);
//		sql_test_perf<DefendAttribute>(DefendAttribute_func);
		sql_test_perf<ArmyOutTable>(ArmyOutTable_func);
//		sql_test_perf<OfficerTech>(OfficerTech_func);
		sql_test_perf<NPC>(NPC_func);
		sql_test_perf<MapInfoTable>(MapInfoTable_func);
		sql_test_perf<ResearchItem>(ResearchItem_func);
		sql_test_perf<EctypeBaseTable>(EctypeBaseTable_func);
		sql_test_perf<NPCArmyTable>(NPCArmyTable_func);
		sql_test_perf<PlayerCaptiveHero>(PlayerCaptiveHero_func);
//		sql_test_perf<EquipAttribute>(EquipAttribute_func);
		sql_test_perf<WarNote>(WarNote_func);
		sql_test_perf<PullulateNote>(PullulateNote_func);
		sql_test_perf<FirendTable>(FirendTable_func);
//		sql_test_perf<ConsortiaOfficial>(ConsortiaOfficial_func);
		sql_test_perf<LoadMaterial>(LoadMaterial_func);
		sql_test_perf<Treasury>(Treasury_func);
//		sql_test_perf<CardConfig>(CardConfig_func);
		sql_test_perf<PublicBuildParamTable>(PublicBuildParamTable_func);
		sql_test_perf<PublicBuildTable>(PublicBuildTable_func);
		sql_test_perf<PlayerProduceState>(PlayerProduceState_func);
//		sql_test_perf<JobParam>(JobParam_func);
//		sql_test_perf<BinConfig>(BinConfig_func);
		sql_test_perf<EquipAntagonism>(EquipAntagonism_func);
		sql_test_perf<BatttleField>(BatttleField_func);
		sql_test_perf<ConsortiaCompare>(ConsortiaCompare_func);
		sql_test_perf<PlayerCompare>(PlayerCompare_func);
		sql_test_perf<Council>(Council_func);
		sql_test_perf<Candidate>(Candidate_func);
		sql_test_perf<Ballot>(Ballot_func);
		sql_test_perf<Country>(Country_func);
		sql_test_perf<ResearchActivity>(ResearchActivity_func);
		sql_test_perf<LevySoldierActivity>(LevySoldierActivity_func);
		sql_test_perf<MailTable>(MailTable_func);
		sql_test_perf<NewsTable>(NewsTable_func);
		sql_test_perf<ChargeSvrTimer>(ChargeSvrTimer_func);
		sql_test_perf<OfficerJobAttribute>(OfficerJobAttribute_func);

		printf("测试成功\n");
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}
	catch(otl_exception& p)
	{ 
		std::cerr << p.msg << std::endl;
		std::cerr << p.stm_text << std::endl;
		std::cerr << p.sqlstate << std::endl;
		std::cerr << p.var_info << std::endl;
	}

	db_.logoff();
}

#endif /*__INCLUDE_TEST_SQL_H__*/