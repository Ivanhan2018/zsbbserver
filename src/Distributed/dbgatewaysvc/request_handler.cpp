#include "request_handler.hpp"
#include "db_handler.hpp"
#include "connection.hpp"
#include "distributed_protocol.hpp"
#include "db_allocator.hpp"
#include "DB_Table.h"

#define OTL_STL
#define OTL_ODBC

#if defined (WIN32)
#define OTL_BIGINT __int64
#else
#define OTL_BIGINT long long
#endif

#include "otlv4.h"
#include "db_write_handler.hpp"

#include "db_single_index_handler.hpp"
#include "db_multi_index_handler.hpp"


request_handler::request_handler() :
run_(false)
{
	::memset(this->handlers_, 0, MAX_DB_NUM * sizeof(task_base_ex<connection, char *> *));

//	this->handlers_[CMD_GAME_CITY_INFO] = new db_multi_index_handler<GameCityInfo>();
//	this->handlers_[CMD_EXE_SOLDIER] = new db_single_index_handler<ExeSoldier>();
//	this->handlers_[CMD_RESEARCH_CONFIGURE] = new db_multi_index_handler<ResearchConfigure>();
//	this->handlers_[CMD_OFFICER_MEDAL] = new db_single_index_handler<OfficerMedal>();
//	this->handlers_[CMD_PLAYER_SOLDIER_TABLE] = new db_multi_index_handler<PlayerSoldierTable>();
//	this->handlers_[CMD_CONSTRUCTION_CONFIGURE] = new db_single_index_handler<ConstructionConfigure>();
//	this->handlers_[CMD_PRIZE_VENDUE] = new db_multi_index_handler<PrizeVendue>();
//	this->handlers_[CMD_PRIZE_DISTRIBUTE] = new db_multi_index_handler<PrizeDistribute>();
//	this->handlers_[CMD_CREATE_CONSORTIA] = new db_single_index_handler<CreateConsortia>();
//	this->handlers_[CMD_CONTRIBUTE_CONFIGURE] = new db_single_index_handler<ContributeConfigure>();
//	this->handlers_[CMD_TASK_BASE_TABLE] = new db_single_index_handler<TaskBaseTable>();
//	this->handlers_[CMD_PRODUCE_CONFIGURE] = new db_multi_index_handler<ProduceConfigure>();
//	this->handlers_[CMD_ALLOT_OFFICER] = new db_single_index_handler<AllotOfficer>();
//	this->handlers_[CMD_OFFICER_ITEM_ALLOT] = new db_single_index_handler<OfficerItemAllot>();
//	this->handlers_[CMD_OFFICER_JOB_ATTRIBUTE] = new db_single_index_handler<OfficerJobAttribute>();
//	this->handlers_[CMD_WEAPON_ATTRIBUTE] = new db_multi_index_handler<WeaponAttribute>();
//	this->handlers_[CMD_DEFEND_ATTRIBUTE] = new db_single_index_handler<DefendAttribute>();
//	this->handlers_[CMD_OFFICER_TECH] = new db_multi_index_handler<OfficerTech>();
//	this->handlers_[CMD_COUNT_RESOURCE] = new db_multi_index_handler<CountResource>();
//	this->handlers_[CMD_EQUIP_ATTRIBUTE] = new db_single_index_handler<EquipAttribute>();
//	this->handlers_[CMD_CONSORTIA_OFFICIAL] = new db_single_index_handler<ConsortiaOfficial>();
//	this->handlers_[CMD_CARD_CONFIG] = new db_single_index_handler<CardConfig>();
//	this->handlers_[CMD_PUBLIC_BUILD_PARAM_TABLE] = new db_single_index_handler<PublicBuildParamTable>();
//	this->handlers_[CMD_JOB_PARAM] = new db_single_index_handler<JobParam>();
//	this->handlers_[CMD_BIN_CONFIG] = new db_single_index_handler<BinConfig>();

	this->handlers_[CMD_PLAYER_SOLDIER_RACE] = new db_multi_index_handler<PlayerSoldierRace>();
	this->handlers_[CMD_ARMY_TABLE] = new db_multi_index_handler<ArmyTable>();
	this->handlers_[CMD_MARKET] = new db_multi_index_handler<Market>();
	this->handlers_[CMD_CONSTRUCTION_BUILD] = new db_multi_index_handler<ConstructionBuild>();
	this->handlers_[CMD_PRODUCTION_ACTIVITY] = new db_multi_index_handler<ProductionActivity>();
	this->handlers_[CMD_CONSORTIA_INFO] = new db_single_index_handler<ConsortiaInfo>();
	this->handlers_[CMD_CONSORTIA_BANK] = new db_multi_index_handler<ConsortiaBank>();
	this->handlers_[CMD_CONSORTIA_UNION] = new db_single_index_handler<ConsortiaUnion>();
	this->handlers_[CMD_PLAYER_OFFICER_ITEM_TABLE] = new db_multi_index_handler<PlayerOfficerItemTable>();
	this->handlers_[CMD_PLAYER_ITEM_STORAGE_TABLE] = new db_multi_index_handler<PlayerItemStorageTable>();
	this->handlers_[CMD_SOLDIER_PARAM_TABLE] = new db_single_index_handler<SoldierParamTable>();
	this->handlers_[CMD_PLAYER_OFFICER_TABLE] = new db_multi_index_handler<PlayerOfficerTable>();
	this->handlers_[CMD_PLAYER_RESOURCE] = new db_multi_index_handler<PlayerResource>();
	this->handlers_[CMD_PLAYER_CONSTRUCTION] = new db_multi_index_handler<PlayerConstruction>();
	this->handlers_[CMD_PLAYER_TASK] = new db_multi_index_handler<PlayerTask>();
	this->handlers_[CMD_CONSORTIA_PERSONNEL] = new db_multi_index_handler<ConsortiaPersonnel>();
	this->handlers_[CMD_CELL_PRICE] = new db_single_index_handler<CellPrice>();
	this->handlers_[CMD_CITY_ATTRIBUTE] = new db_multi_index_handler<CityAttribute>();
	this->handlers_[CMD_CONSORTIA_FIGHT] = new db_multi_index_handler<ConsortiaFight>();
	this->handlers_[CMD_PRODUCTION_AMOUNT] = new db_multi_index_handler<ProductionAmount>();
	this->handlers_[CMD_CHAT_ROOM_TABLE] = new db_single_index_handler<ChatRoomTable>();
	this->handlers_[CMD_PLAYER_CHAT_ROOM_TABLE] = new db_single_index_handler<PlayerChatRoomTable>();
	this->handlers_[CMD_PLAYER_CITY] = new db_multi_index_handler<PlayerCity>();
	this->handlers_[CMD_OFFICER_RACE_ATTRIBUTE] = new db_single_index_handler<OfficerRaceAttribute>();
	this->handlers_[CMD_ARMY_OUT_TABLE] = new db_multi_index_handler<ArmyOutTable>();
	this->handlers_[CMD_NPC] = new db_single_index_handler<NPC>();
	this->handlers_[CMD_MAP_INFO_TABLE] = new db_single_index_handler<MapInfoTable>();
	this->handlers_[CMD_RESEARCH_ITEM] = new db_multi_index_handler<ResearchItem>();
	this->handlers_[CMD_ECTYPE_BASE_TABLE] = new db_single_index_handler<EctypeBaseTable>();
	this->handlers_[CMD_NPC_ARMY_TABLE] = new db_multi_index_handler<NPCArmyTable>();
	this->handlers_[CMD_PLAYER_CAPTIVE_HERO] = new db_multi_index_handler<PlayerCaptiveHero>();
	this->handlers_[CMD_WAR_NOTE] = new db_multi_index_handler<WarNote>();
	this->handlers_[CMD_PULLULATE_NOTE] = new db_multi_index_handler<PullulateNote>();
	this->handlers_[CMD_MAIL_TABLE] = new db_multi_index_handler<MailTable>();
	this->handlers_[CMD_FIREND_TABLE] = new db_multi_index_handler<FirendTable>();
	this->handlers_[CMD_NEWS_TABLE] = new db_single_index_handler<NewsTable>();
	this->handlers_[CMD_LOAD_MATERIAL] = new db_multi_index_handler<LoadMaterial>();
	this->handlers_[CMD_TREASURY] = new db_single_index_handler<Treasury>();
	this->handlers_[CMD_PUBLIC_BUILD_TABLE] = new db_multi_index_handler<PublicBuildTable>();
	this->handlers_[CMD_PLAYER_PRODUCE_STATE] = new db_multi_index_handler<PlayerProduceState>();
	this->handlers_[CMD_CHARGE_SVR_TIMER] = new db_multi_index_handler<ChargeSvrTimer>();
	this->handlers_[CMD_EQUIP_ANTAGONISM] = new db_single_index_handler<EquipAntagonism>();
	this->handlers_[CMD_BATTTLE_FIELD] = new db_single_index_handler<BatttleField>();
	this->handlers_[CMD_PLAYER_COMPARE] = new db_single_index_handler<PlayerCompare>();
	this->handlers_[CMD_CONSORTIA_COMPARE] = new db_single_index_handler<ConsortiaCompare>();
	this->handlers_[CMD_COUNCIL] = new db_multi_index_handler<Council>();
	this->handlers_[CMD_CANDIDATE] = new db_multi_index_handler<Candidate>();
	this->handlers_[CMD_BALLOT] = new db_multi_index_handler<Ballot>();
	this->handlers_[CMD_COUNTRY] = new db_single_index_handler<Country>();
	this->handlers_[CMD_RESEARCH_ACTIVITY] = new db_multi_index_handler<ResearchActivity>();
	this->handlers_[CMD_LEVY_SOLDIER_ACTIVITY] = new db_multi_index_handler<LevySoldierActivity>();
	this->handlers_[CMD_PLAYER_SKILL_TABLE] = new db_multi_index_handler<PlayerSkillTable>();
	this->handlers_[CMD_LIMIT_TABLE] = new db_multi_index_handler<LimitTable>();

	this->handlers_[CMD_PLAYER_DAY_TASK] = new db_multi_index_handler<PlayerDayTask>();
	this->handlers_[CMD_CONSORTIA_CITY_INFO] = new db_multi_index_handler<ConsortiaCityInfo>();

	this->handlers_[CMD_TRAINING_SYS_TABLE] = new db_multi_index_handler<TrainingSysTable>();
}

request_handler::~request_handler()
{
	BOOST_ASSERT(!this->run_);

	for (int i = 0; i < MAX_DB_NUM; i++)
		if (this->handlers_[i])
			delete this->handlers_[i];
}

bool request_handler::start(const char * sql_str, const char * path, bool fromfile)
{
	if (run_) return false;

	if (fromfile)
	{
		if (file_exists(path) == -1)
		{
			LOG_ERROR(("非法路径：%s",path));
			return false;
		}
	}

	otl_connect::otl_initialize();

	for (int i = 0; i < MAX_DB_NUM; i++)
		if (this->handlers_[i])
		{
			if (fromfile)
			{
				if (!this->handlers_[i]->start(path, true))
					return false;
			}
			else
			{
				if (!this->handlers_[i]->start(sql_str, false))
					return false;
			}
		}

	db_write_handler::instance().start(sql_str);

	run_ = true;
	return true;
}

bool request_handler::stop()
{
	if (!run_) return false;

	__filetime = time(0);
	db_write_handler::instance().stop();

	for (int i = 0; i < MAX_DB_NUM; i++)
		if (this->handlers_[i])
			this->handlers_[i]->stop();

	run_ = false;
	return true;
}

bool request_handler::handle_message(connection * conn, char * buffer, int length)
{
	DISTRIBUTED_HEADER * h = (DISTRIBUTED_HEADER *)buffer;

	if (((h->to < CMD_GAME_CITY_INFO) || (h->to > CMD_TRAINING_SYS_TABLE)) || (!this->handlers_[h->to]))
	{
		printf("db_ table id = %d, nonexistent\n", h->to);
		return false;
	}

	if (this->handlers_[h->to]->push(conn->shared_from_this(), buffer))
		return true;

	return false;
}

void request_handler::handle_disconnect(connection * conn, const boost::system::error_code& err)
{
}

void request_handler::save_to_file()
{
	handler_ptr tmp;
	__filetime = time(0);
	tm * fat = ::gmtime(&__filetime);

	printf("文件保存到：%04d%02d%02d%02d%02d%02d 目录中\n", fat->tm_year + 1900,
		fat->tm_mon + 1, fat->tm_mday, (fat->tm_hour + 8) % 24, fat->tm_min, fat->tm_sec);
	for (int i = 0; i < MAX_DB_NUM; i++)
	{
		if (this->handlers_[i])
			this->handlers_[i]->push(tmp, (char *)0xCCCC);
	}
}

void request_handler::save_to_db_all()
{
	handler_ptr tmp;
	for (int i = 0; i < MAX_DB_NUM; i++)
	{
		if (this->handlers_[i])
			this->handlers_[i]->push(tmp, (char *)0xCBCB);
	}
}