/***************************************************************************
*   Copyright (C) 2007 by wangshenwang                                    *
*   wangshenwang1@gmail.com                                               *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
*   This program is distributed in the hope that it will be useful,       *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
*   GNU General Public License for more details.                          *
*                                                                         *
*   You should have received a copy of the GNU General Public License     *
*   along with this program; if not, write to the                         *
*   Free Software Foundation, Inc.,                                       *
*   59 Temple Place - Suite 330, Boston, MA  02111-1307, CHN.             *
***************************************************************************/

#include "dbgviewer.h"
#include "global_macro.h"
#include "db_proxy.h"
#include "db_table.h"

int DBG_VIEWER_API save_to_db(const char * address, short port, int table)
{
	DB_Proxy db;
	if (!db.init(address, port))
		return 0;

	switch (table)
	{
	case CMD_GAME_CITY_INFO :
		{
			GameCityInfo a;
			if (db.db_save_to_db(a) == DB_Proxy::DB_SUCCEED)
				return 1;
			return 0;
		}
		
	case CMD_PLAYER_SOLDIER_RACE :
		{
			PlayerSoldierRace a;
			if (db.db_save_to_db(a) == DB_Proxy::DB_SUCCEED)
				return 1;
			return 0;
		}
			
	case CMD_RESEARCH_CONFIGURE :
		{
			ResearchConfigure a;
			if (db.db_save_to_db(a) == DB_Proxy::DB_SUCCEED)
				return 1;
			return 0;
		}

	case CMD_ARMY_TABLE :
		{
			ArmyTable a;
			if (db.db_save_to_db(a) == DB_Proxy::DB_SUCCEED)
				return 1;
			return 0;
		}

	case CMD_MARKET :
		{
			Market a;
			if (db.db_save_to_db(a) == DB_Proxy::DB_SUCCEED)
				return 1;
			return 0;
		}
		
		
	case CMD_CONSTRUCTION_BUILD :
		{
			ConstructionBuild a;
			if (db.db_save_to_db(a) == DB_Proxy::DB_SUCCEED)
				return 1;
			return 0;
		}
		
	case CMD_PRODUCTION_ACTIVITY :
		{
			ProductionActivity a;
			if (db.db_save_to_db(a) == DB_Proxy::DB_SUCCEED)
				return 1;
			return 0;
		}
		
	case CMD_CONSORTIA_INFO :
		{
			ConsortiaInfo a;
			if (db.db_save_to_db(a) == DB_Proxy::DB_SUCCEED)
				return 1;
			return 0;
		}

	case CMD_CONSORTIA_BANK :
		{
			ConsortiaBank a;
			if (db.db_save_to_db(a) == DB_Proxy::DB_SUCCEED)
				return 1;
			return 0;
		}
		
	case CMD_CONSORTIA_UNION :
		{
			ConsortiaUnion a;
			if (db.db_save_to_db(a) == DB_Proxy::DB_SUCCEED)
				return 1;
			return 0;
		}
		
	case CMD_PLAYER_OFFICER_ITEM_TABLE :
		{
			PlayerOfficerItemTable a;
			if (db.db_save_to_db(a) == DB_Proxy::DB_SUCCEED)
				return 1;
			return 0;
		}

	case CMD_PLAYER_ITEM_STORAGE_TABLE :
		{
			PlayerItemStorageTable a;
			if (db.db_save_to_db(a) == DB_Proxy::DB_SUCCEED)
				return 1;
			return 0;
		}
		
	case CMD_SOLDIER_PARAM_TABLE :
		{
			SoldierParamTable a;
			if (db.db_save_to_db(a) == DB_Proxy::DB_SUCCEED)
				return 1;
			return 0;
		}
		
	case CMD_PLAYER_OFFICER_TABLE :
		{
			PlayerOfficerTable a;
			if (db.db_save_to_db(a) == DB_Proxy::DB_SUCCEED)
				return 1;
			return 0;
		}
		
	case CMD_PLAYER_RESOURCE :
		{
			PlayerResource a;
			if (db.db_save_to_db(a) == DB_Proxy::DB_SUCCEED)
				return 1;
			return 0;
		}
		
	case CMD_CONSTRUCTION_CONFIGURE :
		{
			ConstructionConfigure a;
			if (db.db_save_to_db(a) == DB_Proxy::DB_SUCCEED)
				return 1;
			return 0;
		}
		
	case CMD_PLAYER_CONSTRUCTION :
		{
			PlayerConstruction a;
			if (db.db_save_to_db(a) == DB_Proxy::DB_SUCCEED)
				return 1;
			return 0;
		}
		
	case CMD_PLAYER_TASK :
		{
			PlayerTask a;
			if (db.db_save_to_db(a) == DB_Proxy::DB_SUCCEED)
				return 1;
			return 0;
		}
		
	case CMD_CONSORTIA_PERSONNEL :
		{
			ConsortiaPersonnel a;
			if (db.db_save_to_db(a) == DB_Proxy::DB_SUCCEED)
				return 1;
			return 0;
		}
		
	
	case CMD_CELL_PRICE :
		{
			CellPrice a;
			if (db.db_save_to_db(a) == DB_Proxy::DB_SUCCEED)
				return 1;
			return 0;
		}
		
	case CMD_CITY_ATTRIBUTE :
		{
			CityAttribute a;
			if (db.db_save_to_db(a) == DB_Proxy::DB_SUCCEED)
				return 1;
			return 0;
		}
		
	case CMD_CREATE_CONSORTIA :
		{
			CreateConsortia a;
			if (db.db_save_to_db(a) == DB_Proxy::DB_SUCCEED)
				return 1;
			return 0;
		}
		
	case CMD_CONSORTIA_FIGHT :
		{
			ConsortiaFight a;
			if (db.db_save_to_db(a) == DB_Proxy::DB_SUCCEED)
				return 1;
			return 0;
		}
		
	case CMD_PRODUCTION_AMOUNT :
		{
			ProductionAmount a;
			if (db.db_save_to_db(a) == DB_Proxy::DB_SUCCEED)
				return 1;
			return 0;
		}
		
	case CMD_TASK_BASE_TABLE :
		{
			TaskBaseTable a;
			if (db.db_save_to_db(a) == DB_Proxy::DB_SUCCEED)
				return 1;
			return 0;
		}
		
	case CMD_PRODUCE_CONFIGURE :
		{
			ProduceConfigure a;
			if (db.db_save_to_db(a) == DB_Proxy::DB_SUCCEED)
				return 1;
			return 0;
		}
		
	case CMD_CHAT_ROOM_TABLE :
		{
			ChatRoomTable a;
			if (db.db_save_to_db(a) == DB_Proxy::DB_SUCCEED)
				return 1;
			return 0;
		}
		
	case CMD_PLAYER_CHAT_ROOM_TABLE :
		{
			PlayerChatRoomTable a;
			if (db.db_save_to_db(a) == DB_Proxy::DB_SUCCEED)
				return 1;
			return 0;
		}
		
	case CMD_PLAYER_CITY :
		{
			PlayerCity a;
			if (db.db_save_to_db(a) == DB_Proxy::DB_SUCCEED)
				return 1;
			return 0;
		}
		
	case CMD_OFFICER_TECH :
		{
			OfficerTech a;
			if (db.db_save_to_db(a) == DB_Proxy::DB_SUCCEED)
				return 1;
			return 0;
		}
		
	case CMD_WEAPON_ATTRIBUTE :
		{
			WeaponAttribute a;
			if (db.db_save_to_db(a) == DB_Proxy::DB_SUCCEED)
				return 1;
			return 0;
		}
		
	case CMD_DEFEND_ATTRIBUTE :
		{
			DefendAttribute a;
			if (db.db_save_to_db(a) == DB_Proxy::DB_SUCCEED)
				return 1;
			return 0;
		}
		
	case CMD_OFFICER_RACE_ATTRIBUTE :
		{
			OfficerRaceAttribute a;
			if (db.db_save_to_db(a) == DB_Proxy::DB_SUCCEED)
				return 1;
			return 0;
		}
		
	case CMD_ARMY_OUT_TABLE :
		{
			ArmyOutTable a;
			if (db.db_save_to_db(a) == DB_Proxy::DB_SUCCEED)
				return 1;
			return 0;
		}
		
	case CMD_NPC :
		{
			NPC a;
			if (db.db_save_to_db(a) == DB_Proxy::DB_SUCCEED)
				return 1;
			return 0;
		}
		
	case CMD_MAP_INFO_TABLE :
		{
			MapInfoTable a;
			if (db.db_save_to_db(a) == DB_Proxy::DB_SUCCEED)
				return 1;
			return 0;
		}
		
	case CMD_RESEARCH_ITEM :
		{
			ResearchItem a;
			if (db.db_save_to_db(a) == DB_Proxy::DB_SUCCEED)
				return 1;
			return 0;
		}
		
	case CMD_ECTYPE_BASE_TABLE :
		{
			EctypeBaseTable a;
			if (db.db_save_to_db(a) == DB_Proxy::DB_SUCCEED)
				return 1;
			return 0;
		}
		
	case CMD_PLAYER_CAPTIVE_HERO :
		{
			PlayerCaptiveHero a;
			if (db.db_save_to_db(a) == DB_Proxy::DB_SUCCEED)
				return 1;
			return 0;
		}
		
	case CMD_EQUIP_ATTRIBUTE :
		{
			EquipAttribute a;
			if (db.db_save_to_db(a) == DB_Proxy::DB_SUCCEED)
				return 1;
			return 0;
		}
		
	case CMD_WAR_NOTE :
		{
			WarNote a;
			if (db.db_save_to_db(a) == DB_Proxy::DB_SUCCEED)
				return 1;
			return 0;
		}
		
	case CMD_PULLULATE_NOTE :
		{
			PullulateNote a;
			if (db.db_save_to_db(a) == DB_Proxy::DB_SUCCEED)
				return 1;
			return 0;
		}
		
	case CMD_MAIL_TABLE :
		{
			MailTable a;
			if (db.db_save_to_db(a) == DB_Proxy::DB_SUCCEED)
				return 1;
			return 0;
		}
		
	case CMD_FIREND_TABLE :
		{
			FirendTable a;
			if (db.db_save_to_db(a) == DB_Proxy::DB_SUCCEED)
				return 1;
			return 0;
		}
		
	case CMD_NEWS_TABLE :
		{
			NewsTable a;
			if (db.db_save_to_db(a) == DB_Proxy::DB_SUCCEED)
				return 1;
			return 0;
		}
		
	case CMD_CONSORTIA_OFFICIAL :
		{
			ConsortiaOfficial a;
			if (db.db_save_to_db(a) == DB_Proxy::DB_SUCCEED)
				return 1;
			return 0;
		}
		
	case CMD_LOAD_MATERIAL :
		{
			LoadMaterial a;
			if (db.db_save_to_db(a) == DB_Proxy::DB_SUCCEED)
				return 1;
			return 0;
		}
		
	case CMD_TREASURY :
		{
			Treasury a;
			if (db.db_save_to_db(a) == DB_Proxy::DB_SUCCEED)
				return 1;
			return 0;
		}

	case CMD_CARD_CONFIG :
		{
			CardConfig a;
			if (db.db_save_to_db(a) == DB_Proxy::DB_SUCCEED)
				return 1;
			return 0;
		}

	case CMD_PUBLIC_BUILD_PARAM_TABLE :
		{
			PublicBuildParamTable a;
			if (db.db_save_to_db(a) == DB_Proxy::DB_SUCCEED)
				return 1;
			return 0;
		}

	case CMD_PUBLIC_BUILD_TABLE :
		{
			PublicBuildTable a;
			if (db.db_save_to_db(a) == DB_Proxy::DB_SUCCEED)
				return 1;
			return 0;
		}

	case CMD_PLAYER_PRODUCE_STATE :
		{
			PlayerProduceState a;
			if (db.db_save_to_db(a) == DB_Proxy::DB_SUCCEED)
				return 1;
			return 0;
		}

	case CMD_CHARGE_SVR_TIMER :
		{
			ChargeSvrTimer a;
			if (db.db_save_to_db(a) == DB_Proxy::DB_SUCCEED)
				return 1;
			return 0;
		}
	case CMD_JOB_PARAM :
		{
			JobParam a;
			if (db.db_save_to_db(a) == DB_Proxy::DB_SUCCEED)
				return 1;
			return 0;
		}
	case CMD_BIN_CONFIG :
		{
			BinConfig a;
			if (db.db_save_to_db(a) == DB_Proxy::DB_SUCCEED)
				return 1;
			return 0;
		}

	case CMD_EQUIP_ANTAGONISM :
		{
			EquipAntagonism a;
			if (db.db_save_to_db(a) == DB_Proxy::DB_SUCCEED)
				return 1;
			return 0;
		}
	case CMD_BATTTLE_FIELD :
		{
			BatttleField a;
			if (db.db_save_to_db(a) == DB_Proxy::DB_SUCCEED)
				return 1;
			return 0;
		}
	case CMD_CONSORTIA_COMPARE :
		{
			ConsortiaCompare a;
			if (db.db_save_to_db(a) == DB_Proxy::DB_SUCCEED)
				return 1;
			return 0;
		}
	case CMD_PLAYER_COMPARE :
		{
			PlayerCompare a;
			if (db.db_save_to_db(a) == DB_Proxy::DB_SUCCEED)
				return 1;
			return 0;
		}
	case CMD_COUNCIL :
		{
			Council a;
			if (db.db_save_to_db(a) == DB_Proxy::DB_SUCCEED)
				return 1;
			return 0;
		}
	case CMD_CANDIDATE :
		{
			Candidate a;
			if (db.db_save_to_db(a) == DB_Proxy::DB_SUCCEED)
				return 1;
			return 0;
		}
	case CMD_BALLOT :
		{
			Ballot a;
			if (db.db_save_to_db(a) == DB_Proxy::DB_SUCCEED)
				return 1;
			return 0;
		}
	case CMD_COUNTRY :
		{
			Country a;
			if (db.db_save_to_db(a) == DB_Proxy::DB_SUCCEED)
				return 1;
			return 0;
		}

	case CMD_RESEARCH_ACTIVITY :
		{
			ResearchActivity a;
			if (db.db_save_to_db(a) == DB_Proxy::DB_SUCCEED)
				return 1;
			return 0;
		}

	case CMD_LEVY_SOLDIER_ACTIVITY :
		{
			LevySoldierActivity a;
			if (db.db_save_to_db(a) == DB_Proxy::DB_SUCCEED)
				return 1;
			return 0;
		}
	}

	return 0;
}




