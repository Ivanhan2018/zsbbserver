#ifndef __INCLUDE_DB_TABLE_H__
#define __INCLUDE_DB_TABLE_H__

#include <string>
#include "Global_Macro.h"

#pragma pack(push,1)

//徐进才删除
//struct GameCityInfo
//{
//	int cityid_; // cityid_ + mapid_ = ID 一条
//	int mapid_; // mapid_ = ID 多条
//	int foodcount_;
//	int woodcount_;
//	int ironcount_;
//	int skincount_;
//	int x_;
//	int y_;
//	int human_;			//zeng 06/10
//	int mannikin_;
//	int fairy_;
//	int dandiprat_;
//	int halfanimal_;
//	int cattle_;
//	int blackfairy_;
//	int decpesrson_;
//
//
//	static int gettype() 
//	{ return CMD_GAME_CITY_INFO;}
//
//	static const char * getname() 
//	{ return "GameCityInfo";}
//
//	inline void setmasterid(int mapid) 
//	{mapid_ = mapid;}
//
//	inline void getmasterid(char * buf) 
//	{sprintf(buf, "%d", mapid_);}
//
//	inline void setdetailid(int cityid) 
//	{cityid_ = cityid;}
//
//	inline void getdetailid(char * buf) 
//	{sprintf(buf, "%d", cityid_);}
//
//	inline void setkeyvalue(int key){}
//
//	inline int getkeyvalue(){ return 0; }
//
//	int get_insert_str(char * buf)
//	{ 
//		//return sprintf(buf, "insert into GameCityInfo(cityid,mapid,foodcount,woodcount,ironcount,skincount,x,y,human,"
//		//	"mannikin,fairy,dandiprat,halfanimal,cattle,blackfairy,decpesrson) values(%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d) ",
//		return sprintf(buf, "insert into GameCityInfo values(%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d) ",
//			cityid_, mapid_, foodcount_, woodcount_, ironcount_, skincount_, x_, y_, human_, 
//			mannikin_, fairy_, dandiprat_, halfanimal_, cattle_, blackfairy_, decpesrson_);
//	}
//
//	int get_update_str(char * buf, GameCityInfo & old)
//	{ 
//		return sprintf(buf, "update GameCityInfo set foodcount=%d,woodcount=%d,ironcount=%d,"
//			"skincount=%d,x=%d,y=%d,human=%d,mannikin=%d,fairy=%d,dandiprat=%d,halfanimal=%d,"
//			"cattle=%d,blackfairy=%d,decpesrson=%d where mapid=%d and cityid=%d ",
//			 foodcount_, woodcount_, ironcount_, skincount_, x_, y_, human_, mannikin_, fairy_, 
//			dandiprat_, halfanimal_, cattle_, blackfairy_, decpesrson_, old.mapid_, old.cityid_);
//	}
//
//	int get_delete_str(char * buf)
//	{ 
//		return sprintf(buf, "delete from GameCityInfo where mapid=%d and cityid=%d ", mapid_, cityid_);
//	}
//
//	static const char * get_select_str()
//	{ return "select * from GameCityInfo";}
//
//	template < class otl_stream >
//	friend otl_stream& operator>>(otl_stream& os, GameCityInfo& p)
//	{
//		os >> p.cityid_ >> p.mapid_ >> p.foodcount_ >> p.woodcount_ >> p.ironcount_ >> p.skincount_ >> p.x_ >> p.y_ >> p.human_ >> p.mannikin_ >> p.fairy_ >> p.dandiprat_ >> p.halfanimal_ >> p.cattle_ >> p.blackfairy_ >> p.decpesrson_;
//		return os;
//	}
//
//	GameCityInfo& operator+=(const GameCityInfo& p)
//	{
//		foodcount_ += p.foodcount_;
//		woodcount_ += p.woodcount_;
//		ironcount_ += p.ironcount_;
//		skincount_ += p.skincount_;
//		x_ += p.x_;
//		y_ += p.y_;
//		human_ += p.human_;
//		mannikin_ += p.mannikin_;
//		fairy_ += p.fairy_;
//		dandiprat_ += p.dandiprat_;
//		halfanimal_ += p.halfanimal_;
//		cattle_ += p.cattle_;
//		blackfairy_ += p.blackfairy_;
//		decpesrson_ += p.decpesrson_;
//		return *this;
//	}
//
//	GameCityInfo()
//	{
//		memset((char *)this, 0, sizeof(*this));
//	}
//};

struct PlayerSoldierRace
{
	int playerid_; // mapid_ + cityid_ + playerid_ = ID 一条
	int cityid_;
	int mapid_; // mapid_ + cityid_ = ID 多条
	int human_;
	int mannikin_;
	int fairy_;
	int dandiprat_;
	int halfanimal_;
	int cattle_;
	int blackfairy_;
	int decpesrson_;
	int64 completetimes_;

	static int gettype() 
	{ return CMD_PLAYER_SOLDIER_RACE;}

	static const char * getname() 
	{ return "PlayerSoldierRace";}

	inline void setmasterid(int mapid, int cityid) 
	{mapid_ = mapid;cityid_=cityid;}

	inline void getmasterid(char * buf) 
	{sprintf(buf, "%d-%d", mapid_, cityid_);}

	inline void setdetailid(int playerid) 
	{playerid_ = playerid;}

	inline void getdetailid(char * buf) 
	{sprintf(buf, "%d", playerid_);}

	inline void setkeyvalue(int key){}

	inline int getkeyvalue() { return 0; }

	int get_insert_str(char * buf)
	{ 
		//return sprintf(buf, "insert into PlayerSoldierRace(playerid,cityid,mapid,human,mannikin,fairy,dandiprat,"
		//	"halfanimal,cattle,blackfairy,decpesrson,completetimes) values(%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%I64d) ",
		return sprintf(buf, "insert into PlayerSoldierRace values(%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%I64d) ",
			playerid_, cityid_, mapid_, human_, mannikin_, fairy_, dandiprat_, 
			halfanimal_, cattle_, blackfairy_, decpesrson_, completetimes_);
	}

	int get_update_str(char * buf, PlayerSoldierRace & old)
	{ 
		return sprintf(buf, "update PlayerSoldierRace set human=%d,mannikin=%d,"
			"fairy=%d,dandiprat=%d,halfanimal=%d,cattle=%d,blackfairy=%d,decpesrson=%d,"
			"completetimes=%I64d where mapid=%d and cityid=%d and playerid=%d ",
			human_, mannikin_, fairy_, dandiprat_, halfanimal_, cattle_, 
			blackfairy_, decpesrson_, completetimes_, old.mapid_, old.cityid_, old.playerid_);
	}

	int get_delete_str(char * buf)
	{ 
		return sprintf(buf, "delete from PlayerSoldierRace where mapid=%d and cityid=%d and playerid=%d ", 
			mapid_, cityid_, playerid_);
	}

	static const char * get_select_str()
	{ return "select * from PlayerSoldierRace";}

	template < class otl_stream >
	friend otl_stream& operator>>(otl_stream& os, PlayerSoldierRace& p)
	{
		os >> p.playerid_ >> p.cityid_ >> p.mapid_ >> p.human_ >> p.mannikin_ >> p.fairy_ >> p.dandiprat_ >> p.halfanimal_ >> p.cattle_ >> p.blackfairy_ >> p.decpesrson_ >> p.completetimes_;
		return os;
	}

	PlayerSoldierRace& operator+=(const PlayerSoldierRace& p)
	{
		human_ += p.human_;
		mannikin_ += p.mannikin_;
		fairy_ += p.fairy_;
		dandiprat_ += p.dandiprat_;
		halfanimal_ += p.halfanimal_;
		cattle_ += p.cattle_;
		blackfairy_ += p.blackfairy_;
		decpesrson_ += p.decpesrson_;
		completetimes_ += p.completetimes_;
		return *this;
	}

	PlayerSoldierRace()
	{
		memset((char *)this, 0, sizeof(*this));
	}
};

struct ProductionAmount
{
	// int level_删除
	int productionid_;
	int type_;
	int mapid_; 
	int cityid_; // mapid_ + cityid_ + playerid_ + type_ = id 多条
	int playerid_; // mapid_ + cityid_ + playerid_ + type_  + productionid_ = ID 一条
	int productioncount_;

	static int gettype() 
	{ return CMD_PRODUCTION_AMOUNT;}

	static const char * getname() 
	{ return "ProductionAmount";}

	inline void setmasterid(int mapid, int cityid, int playerid, int type) 
	{mapid_ = mapid;cityid_=cityid; playerid_=playerid;type_=type;}

	inline void getmasterid(char * buf) 
	{sprintf(buf, "%d-%d-%d-%d", mapid_, cityid_, playerid_, type_);}

	inline void setdetailid(int productionid) 
	{
		productionid_ = productionid;
	}

	inline void getdetailid(char * buf) 
	{sprintf(buf, "%d", productionid_);}

	inline void setkeyvalue(int key){}

	inline int getkeyvalue() { return 0; }

	int get_insert_str(char * buf)
	{ 
		//return sprintf(buf, "insert into ProductionAmount(productionid,type,mapid,cityid,playerid,productioncount) values(%d,%d,%d,%d,%d,%d) ",
		return sprintf(buf, "insert into ProductionAmount values(%d,%d,%d,%d,%d,%d) ",
			productionid_, type_, mapid_, cityid_, playerid_, productioncount_);
	}

	int get_update_str(char * buf, ProductionAmount & old)
	{ 
		return sprintf(buf, "update ProductionAmount set productioncount=%d"
			" where mapid=%d and cityid=%d and playerid=%d and type=%d and productionid=%d ", 
			productioncount_, old.mapid_, old.cityid_, old.playerid_, old.type_, old.productionid_);
	}

	int get_delete_str(char * buf)
	{ 
		return sprintf(buf, "delete from ProductionAmount where mapid=%d and cityid=%d "
			"and playerid=%d and type=%d and productionid=%d ", 
			mapid_, cityid_, playerid_, type_, productionid_);
	}

	static const char * get_select_str()
	{ return "select * from ProductionAmount";}

	template < class otl_stream >
	friend otl_stream& operator>>(otl_stream& os, ProductionAmount& p)
	{
		os>> p.productionid_>> p.type_>> p.mapid_>> p.cityid_>> p.playerid_>> p.productioncount_;
		return os;
	}

	ProductionAmount& operator+=(const ProductionAmount& p)
	{
		productioncount_ += p.productioncount_;
		return *this;
	}

	ProductionAmount()
	{
		memset((char *)this, 0, sizeof(*this));
	}
};

//徐进才删除
//struct ConstructionConfigure
//{
//	int constructionid_;
//	int level_; // constructionid_ + level_ = id 一条
//	int64 completetimes_;
//	int money_;
//	int foodcount_;
//	int woodcount_;
//	int ironcount_;
//	int skincount_;
//	int science_;
//	int modify_;
//
//	static int gettype() 
//	{ return CMD_CONSTRUCTION_CONFIGURE;}
//
//	static const char * getname() 
//	{ return "ConstructionConfigure";}
//
//	inline void setid(int constructionid, int level) 
//	{constructionid_ = constructionid;level_=level;}
//
//	inline void getid(char * buf) 
//	{sprintf(buf, "%d-%d", constructionid_, level_);}
//
//	inline void setkeyvalue(int key) {}
//	inline int getkeyvalue() { return 0; }
//
//	int get_insert_str(char * buf)
//	{ 
//		//return sprintf(buf, "insert into ConstructionConfigure(constructionid,level,completetimes,money,foodcount,"
//		//	"woodcount,ironcount,skincount,science,modify) values(%d,%d,%I64d,%d,%d,%d,%d,%d,%d,%d) ",
//		return sprintf(buf, "insert into ConstructionConfigure values(%d,%d,%I64d,%d,%d,%d,%d,%d,%d,%d) ",
//			constructionid_, level_, completetimes_, money_, foodcount_, 
//			woodcount_, ironcount_, skincount_, science_, modify_);
//	}
//
//	int get_update_str(char * buf, ConstructionConfigure & old)
//	{ 
//		return sprintf(buf, "update ConstructionConfigure set completetimes=%I64d,money=%d,foodcount=%d,"
//			"woodcount=%d,ironcount=%d,skincount=%d,science=%d,modify=%d where constructionid=%d and level=%d ", 
//			completetimes_, money_, foodcount_, woodcount_, ironcount_, skincount_, science_, modify_, old.constructionid_, old.level_);
//	}
//
//	int get_delete_str(char * buf)
//	{ 
//		return sprintf(buf, "delete from ConstructionConfigure where constructionid=%d and level=%d ", 
//			constructionid_, level_);
//	}
//
//	static const char * get_select_str()
//	{ return "select * from ConstructionConfigure";}
//
//	template < class otl_stream >
//	friend otl_stream& operator>>(otl_stream& os, ConstructionConfigure& p)
//	{
//		os >> p.constructionid_>> p.level_>> p.completetimes_>> p.money_>> p.foodcount_>> p.woodcount_>> p.ironcount_>> p.skincount_>> p.science_>> p.modify_;
//		return os;
//	}
//
//	ConstructionConfigure& operator+=(const ConstructionConfigure& p)
//	{
//		completetimes_ += p.completetimes_;
//		money_ += p.money_;
//		foodcount_ += p.foodcount_;
//		woodcount_ += p.woodcount_;
//		ironcount_ += p.ironcount_;
//		skincount_ += p.skincount_;
//		science_ += p.science_;
//		modify_ += p.modify_;
//		return *this;
//	}
//
//	ConstructionConfigure()
//	{
//		memset((char *)this, 0, sizeof(*this));
//	}
//};

//徐进才删除
//struct ResearchConfigure
//{
//	int itemid_;
//	int level_; // type_ = ID 多条
//	int type_; // type_ + itemid_  + level_ = ID 一条
//	int money_;
//	int science_;
//	int64 completetimes_;
//
//	static int gettype() 
//	{ return CMD_RESEARCH_CONFIGURE;}
//
//	static const char * getname() 
//	{ return "ResearchConfigure";}
//
//	inline void setmasterid(int type) 
//	{type_=type;}
//
//	inline void getmasterid(char * buf) 
//	{sprintf(buf, "%d", type_);}
//
//	inline void setdetailid(int itemid, int level) 
//	{itemid_ = itemid;level_ = level;}
//
//	inline void getdetailid(char * buf) 
//	{sprintf(buf, "%d-%d", itemid_, level_);}
//
//	inline void setkeyvalue(int key){}
//	inline int getkeyvalue() { return 0; }
//
//	int get_insert_str(char * buf)
//	{ 
//		//return sprintf(buf, "insert into ResearchConfigure(itemid,level,type,money,science,completetimes) values(%d,%d,%d,%d,%d,%I64d) ",
//		return sprintf(buf, "insert into ResearchConfigure values(%d,%d,%d,%d,%d,%I64d) ",
//			itemid_, level_, type_, money_, science_, completetimes_);
//	}
//
//	int get_update_str(char * buf, ResearchConfigure & old)
//	{ 
//		return sprintf(buf, "update ResearchConfigure set money=%d,science=%d,"
//			"completetimes=%I64d where type=%d and itemid=%d and level=%d ",
//			money_, science_, completetimes_, old.type_, old.itemid_, old.level_);
//	}
//
//	int get_delete_str(char * buf)
//	{ 
//		return sprintf(buf, "delete from ResearchConfigure where type=%d and itemid=%d and level=%d ", 
//			type_, itemid_, level_);
//	}
//
//	static const char * get_select_str()
//	{ return "select * from ResearchConfigure";}
//
//	template < class otl_stream >
//	friend otl_stream& operator>>(otl_stream& os, ResearchConfigure& p)
//	{
//		os >> p.itemid_ >> p.level_ >> p.type_ >> p.money_ >> p.science_ >> p.completetimes_;
//		return os;
//	}
//
//	ResearchConfigure& operator+=(const ResearchConfigure& p)
//	{
//		money_ += p.money_;
//		science_ += p.science_;
//		completetimes_ += p.completetimes_;
//		return *this;
//	}
//
//	ResearchConfigure()
//	{
//		memset((char *)this, 0, sizeof(*this));
//	}
//};

struct PlayerConstruction
{	
	int sortid_;		//2008/06/19 by zengqinghui  此字段生产编号
	int constructionid_; //mapid_ + cityid_ + playerid_ + sortid_ = id 一条
	int mapid_;
	int cityid_;
	int playerid_; //mapid_ + cityid_ + playerid_ = id 多条
	int level_;
	int64 completetimes_;
	int output_;

	static int gettype() 
	{ return CMD_PLAYER_CONSTRUCTION;}

	static const char * getname() 
	{ return "PlayerConstruction";}

	inline void setmasterid(int mapid, int cityid, int playerid) 
	{mapid_ = mapid;cityid_=cityid;playerid_=playerid;}

	inline void getmasterid(char * buf) 
	{sprintf(buf, "%d-%d-%d", mapid_, cityid_, playerid_);}

	inline void setdetailid(int sortid) 
	{sortid_ = sortid;}

	inline void getdetailid(char * buf) 
	{sprintf(buf, "%d", sortid_);}

	inline void setkeyvalue(int key){}
	inline int getkeyvalue() { return 0; }

	int get_insert_str(char * buf)
	{ 
		//return sprintf(buf, "insert into PlayerConstruction(sortid,constructionid,mapid,cityid,playerid,level,completetimes,output) "
		return sprintf(buf, "insert into PlayerConstruction values(%d,%d,%d,%d,%d,%d,%I64d,%d) ",
			sortid_, constructionid_, mapid_, cityid_, playerid_, level_, completetimes_, output_);
	}

	int get_update_str(char * buf, PlayerConstruction & old)
	{ 
		return sprintf(buf, "update PlayerConstruction set constructionid=%d,"
			"level=%d,completetimes=%I64d,output=%d where mapid=%d and cityid=%d and playerid=%d and sortid=%d ",
			constructionid_, level_, completetimes_, output_, old.mapid_, old.cityid_, old.playerid_, old.sortid_);
	}

	int get_delete_str(char * buf)
	{ 
		return sprintf(buf, "delete from PlayerConstruction where mapid=%d and "
			"cityid=%d and playerid=%d and sortid=%d ", 
			mapid_, cityid_, playerid_, sortid_);
	}

	static const char * get_select_str()
	{ return "select * from PlayerConstruction";}

	template < class otl_stream >
		friend otl_stream& operator>>(otl_stream& os, PlayerConstruction& p)
	{
		os  >> p.sortid_ >> p.constructionid_ >> p.mapid_ >> p.cityid_ 
			>> p.playerid_ >> p.level_ >> p.completetimes_ >> p.output_;
		return os;
	}

	PlayerConstruction& operator+=(const PlayerConstruction& p)
	{
		constructionid_ += p.constructionid_;
		level_ += p.level_;
		completetimes_ += p.completetimes_;
		output_ += p.output_;
		return *this;
	}

	PlayerConstruction()
	{
		memset((char *)this, 0, sizeof(*this));
	}
};

struct PlayerResource
{
	int playerid_;//cityid_ + mapid_ + playerid_ = ID   一条
	int mapid_; // cityid_ + mapid_ = ID // 多条
	int cityid_;
	int foodcount_;
	int woodcount_;
	int ironcount_;
	int skincount_;
	int redstonecount_;
	int yollowstonecount_;
	int greenstonecount_;
	int backironcount_;
	int64 completetimes_;
	int content_;

	static int gettype() 
	{ return CMD_PLAYER_RESOURCE;}

	static const char * getname() 
	{ return "PlayerResource";}

	inline void setmasterid(int cityid, int mapid) 
	{cityid_ = cityid, mapid_ = mapid;}

	inline void getmasterid(char * buf) 
	{sprintf(buf, "%d-%d", cityid_, mapid_);}

	inline void setdetailid(int playerid) 
	{playerid_ = playerid;}

	inline void getdetailid(char * buf) 
	{sprintf(buf, "%d", playerid_);}

	inline void setkeyvalue(int key) {}

	inline int getkeyvalue() { return 0; }

	int get_insert_str(char * buf)
	{ 
		//return sprintf(buf, "insert into PlayerResource(playerid,mapid,cityid,foodcount,woodcount,ironcount,skincount,redstonecount,"
		//	"yollowstonecount,greenstonecount,backironcount,completetimes,content) values(%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%I64d,%d) ",
		return sprintf(buf, "insert into PlayerResource values(%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%I64d,%d) ",
			playerid_, mapid_, cityid_, foodcount_, woodcount_, ironcount_, skincount_, redstonecount_,
			yollowstonecount_, greenstonecount_, backironcount_, completetimes_, content_);
	}

	int get_update_str(char * buf, PlayerResource & old)
	{ 
		return sprintf(buf, "update PlayerResource set foodcount=%d,woodcount=%d,ironcount=%d,skincount=%d,redstonecount=%d,"
			"yollowstonecount=%d,greenstonecount=%d,backironcount=%d,completetimes=%I64d,content=%d where cityid=%d and mapid=%d and playerid=%d ",
			foodcount_, woodcount_, ironcount_, skincount_, redstonecount_,
			yollowstonecount_, greenstonecount_, backironcount_, completetimes_, content_, old.cityid_, old.mapid_, old.playerid_);
	}

	int get_delete_str(char * buf)
	{ 
		return sprintf(buf, "delete from PlayerResource where cityid=%d and mapid=%d and playerid=%d ", 
			cityid_, mapid_, playerid_);
	}

	static const char * get_select_str()
	{ return "select * from PlayerResource";}

	template < class otl_stream >
		friend otl_stream& operator>>(otl_stream& os, PlayerResource& p)
	{
		os  >> p.playerid_ >> p.mapid_ >> p.cityid_ >> p.foodcount_ >> p.woodcount_ 
			>> p.ironcount_ >> p.skincount_ >> p.redstonecount_ >> p.yollowstonecount_ 
			>> p.greenstonecount_ >> p.backironcount_ >> p.completetimes_ >> p.content_;
		return os;
	}

	PlayerResource& operator+=(const PlayerResource& p)
	{
		foodcount_ += p.foodcount_;
		woodcount_ += p.woodcount_;
		ironcount_ += p.ironcount_;
		skincount_ += p.skincount_;
		redstonecount_ += p.redstonecount_;
		yollowstonecount_ += p.yollowstonecount_;
		greenstonecount_ += p.greenstonecount_;
		backironcount_ += p.backironcount_;
		completetimes_ += p.completetimes_;
		content_ += p.content_;
		return *this;
	}

	PlayerResource()
	{
		memset((char *)this, 0, sizeof(*this));
	}
};

struct ArmyTable
{
	int armyid_; // mapid_ = ID 多条
	char armyname_[SIZE_32];//2008/06/17 by zeng
	int officerid_;
	int playerid_;
	int cityid_;
	int mapid_;// mapid_ + armyid_ = id 一条
	int soldiertype_;
	int raceid_;
	int firstcount_;
	int fsoldierlevel_;
	int fweapon_;
	int fsexp_;
	int fweaponlevel_;
	int fdefendtype_;
	int fdefendlevel_;
	int state_;
	int x;
	int y;
	int direction;
	int countryid_;

	static int gettype() 
	{ return CMD_ARMY_TABLE;}

	static const char * getname() 
	{ return "ArmyTable";}

	inline void setmasterid(int mapid) 
	{mapid_ = mapid;}

	inline void getmasterid(char * buf) 
	{sprintf(buf, "%d", mapid_);}

	inline void setdetailid(int armyid) 
	{armyid_ = armyid;}

	inline void getdetailid(char * buf) 
	{sprintf(buf, "%d", armyid_);}

	inline void setkeyvalue(int key)
	{ armyid_ = key; }

	inline int getkeyvalue() { return armyid_; }

	int get_insert_str(char * buf)
	{ 
		//return sprintf(buf, "insert into ArmyTable(armyid,armyname,officerid,playerid,cityid,mapid,soldiertype,raceid,firstcount,"
		//	"fsoldierlevel,fweapon,fsexp,fweaponlevel,fdefendtype,fdefendlevel,state,x,y,direction,countryid) "
		//	"values(%d,\'\'%s\'\',%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d) ",
		return sprintf(buf, "insert into ArmyTable values(%d,\'\'%s\'\',%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d) ",
			armyid_, armyname_, officerid_, playerid_, cityid_, mapid_, soldiertype_, raceid_, firstcount_, 
			fsoldierlevel_, fweapon_, fsexp_, fweaponlevel_, fdefendtype_, fdefendlevel_, state_, x, y, direction, countryid_);
	}

	// 需要更改主键
	int get_update_str(char * buf, ArmyTable & old)
	{ 
		return sprintf(buf, "update ArmyTable set mapid=%d,armyname=\'\'%s\'\',officerid=%d,playerid=%d,cityid=%d,soldiertype=%d,raceid=%d,firstcount=%d,"
			"fsoldierlevel=%d,fweapon=%d,fsexp=%d,fweaponlevel=%d,fdefendtype=%d,fdefendlevel=%d,state=%d,x=%d,y=%d,direction=%d,countryid=%d"
			" where mapid=%d and armyid=%d ",
			mapid_, armyname_, officerid_, playerid_, cityid_, soldiertype_, raceid_, firstcount_, 
			fsoldierlevel_, fweapon_, fsexp_, fweaponlevel_, fdefendtype_, fdefendlevel_, state_, x, y, direction, countryid_, old.mapid_, old.armyid_);
	}

	int get_delete_str(char * buf)
	{ 
		return sprintf(buf, "delete from ArmyTable where mapid=%d and armyid=%d ", mapid_, armyid_);
	}

	static const char * get_select_str()
	{ return "select * from ArmyTable";}

	template < class otl_stream >
		friend otl_stream& operator>>(otl_stream& os, ArmyTable& p)
	{
		os  >> p.armyid_ >> p.armyname_ >> p.officerid_ >> p.playerid_ 
			>> p.cityid_ >> p.mapid_ >> p.soldiertype_ >> p.raceid_ 
			>> p.firstcount_ >> p.fsoldierlevel_ >> p.fweapon_ >> p.fsexp_ 
			>> p.fweaponlevel_ >> p.fdefendtype_ >> p.fdefendlevel_ 
			>> p.state_ >> p.x >> p.y >> p.direction >> p.countryid_;
		return os;
	}

	ArmyTable& operator+=(const ArmyTable& p)
	{
		officerid_ += p.officerid_;
		playerid_ += p.playerid_;
		cityid_ += p.cityid_;
		soldiertype_ += p.soldiertype_;
		raceid_ += p.raceid_;
		firstcount_ += p.firstcount_;
		fsoldierlevel_ += p.fsoldierlevel_;
		fweapon_ += p.fweapon_;
		fsexp_ += p.fsexp_;
		fweaponlevel_ += p.fweaponlevel_;
		fdefendtype_ += p.fdefendtype_;
		fdefendlevel_ += p.fdefendlevel_;
		state_ += p.state_;
		x += p.x;
		y += p.y;
		direction += p.direction;
		countryid_ += p.countryid_;
		return *this;
	}

	ArmyTable()
	{
		memset((char *)this, 0, sizeof(*this));
	}
};

struct Market
{
	int numid_;
	int playerid_;
	int64 itemid_;
	int level_;
	int type_;
	int count_;
	int minprice_;
	int currentprice_;
	int onceprice_;
	int bidplayerid_;
	int64 starttime_;
	int country_;	// country_ + type_ = id 多条
	int mapid_;		// country_ + type_ + numid_ = id 一条
	int cityid_;
	int bidmapid_;
	int bidcityid_;
	char playername_[SIZE_24];

	static int gettype() 
	{ return CMD_MARKET;}

	static const char * getname() 
	{ return "Market";}

	inline void setmasterid(int country, int type) 
	{country_=country; type_=type;}

	inline void getmasterid(char * buf) 
	{sprintf(buf, "%d-%d", country_, type_);}

	inline void setdetailid(int numid) 
	{numid_= numid;}

	inline void getdetailid(char * buf) 
	{sprintf(buf, "%d", numid_);}

	inline void setkeyvalue(int key)
	{ numid_ = key; }

	inline int getkeyvalue() { return numid_; }

	int get_insert_str(char * buf)
	{ 
		//return sprintf(buf, "insert into Market(numid,playerid,itemid,level,type,count,minprice,currentprice,onceprice,"
		//	"bidplayerid,starttime,country,mapid,cityid,bidmapid,bidcityid,playername_) "
		//	"values(%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%I64d,%d,%d,%d,%d,%d,\'\'%s\'\') ",
		return sprintf(buf, "insert into Market values(%d,%d,%I64d,%d,%d,%d,%d,%d,%d,%d,%I64d,%d,%d,%d,%d,%d,\'\'%s\'\') ",
			numid_, playerid_, itemid_, level_, type_, count_, minprice_, currentprice_, onceprice_, 
			bidplayerid_, starttime_, country_, mapid_, cityid_, bidmapid_, bidcityid_, playername_);
	}

	int get_update_str(char * buf, Market & old)
	{
		return sprintf(buf, "update Market set playerid=%d,itemid=%I64d,level=%d,count=%d,minprice=%d,"
			"currentprice=%d,onceprice=%d,bidplayerid=%d,starttime=%I64d,mapid=%d,cityid=%d,bidmapid=%d,bidcityid=%d,playername_=\'\'%s\'\'"
			" where country=%d and type=%d and numid=%d ",
			playerid_, itemid_, level_, count_, minprice_, currentprice_, onceprice_, 
			bidplayerid_, starttime_, mapid_, cityid_, bidmapid_, bidcityid_, playername_, old.country_, old.type_, old.numid_);
	}

	int get_delete_str(char * buf)
	{ 
		return sprintf(buf, "delete from Market where country=%d and type=%d and numid=%d ", country_, type_, numid_);
	}

	static const char * get_select_str()
	{ return "select * from Market";}

	template < class otl_stream >
		friend otl_stream& operator>>(otl_stream& os, Market& p)
	{
		os  >> p.numid_ >> p.playerid_ >> p.itemid_ >> p.level_ 
			>> p.type_ >> p.count_ >> p.minprice_ >> p.currentprice_ 
			>> p.onceprice_ >> p.bidplayerid_ >> p.starttime_ >> p.country_
			>> p.mapid_ >> p.cityid_ >> p.bidmapid_ >> p.bidcityid_ >> p.playername_;
		return os;
	}

	Market& operator+=(const Market& p)
	{
		playerid_ += p.playerid_;
		itemid_ += p.itemid_;
		level_ += p.level_;
		count_ += p.count_;
		minprice_ += p.minprice_;
		currentprice_ += p.currentprice_;
		onceprice_ += p.onceprice_;
		bidplayerid_ += p.bidplayerid_;
		starttime_ += p.starttime_;
		mapid_ += p.mapid_;
		cityid_ += p.cityid_;
		bidmapid_ += p.bidmapid_;
		bidcityid_ += p.bidcityid_;
		return *this;
	}

	Market()
	{
		memset((char *)this, 0, sizeof(*this));
	}
};

struct ConstructionBuild
{
	int sortid_;
	int action_;
	int playerid_;
	int conid_;
	int level_;
	int cityid_; // mapid_ + cityid_ + playerid_ + conid_ + level_ + sortid_= id 一条
	int mapid_; // mapid_ + cityid_+ playerid_ = id 多条
	int64 starttime_;

	static int gettype() 
	{ return CMD_CONSTRUCTION_BUILD;}

	static const char * getname() 
	{ return "ConstructionBuild";}

	inline void setmasterid(int mapid, int cityid, int playerid) 
	{mapid_ = mapid;cityid_ = cityid; playerid_ = playerid;}

	inline void getmasterid(char * buf) 
	{sprintf(buf, "%d-%d-%d", mapid_, cityid_, playerid_);}

	inline void setdetailid(int conid, int level, int sortid) 
	{conid_=conid;level_=level;sortid_=sortid;}

	inline void getdetailid(char * buf) 
	{sprintf(buf, "%d-%d-%d", conid_,level_,sortid_);}

	inline void setkeyvalue(int key){sortid_=key;}

	inline int getkeyvalue() { return sortid_; }

	int get_insert_str(char * buf)
	{ 
		//return sprintf(buf, "insert into ConstructionBuild(sortid,action,playerid,conid,level,cityid,mapid,starttime) values(%d,%d,%d,%d,%d,%d,%d,%I64d) ",
		return sprintf(buf, "insert into ConstructionBuild values(%d,%d,%d,%d,%d,%d,%d,%I64d) ",
			sortid_, action_, playerid_, conid_, level_, cityid_, mapid_, starttime_);
	}

	int get_update_str(char * buf, ConstructionBuild & old)
	{
		return sprintf(buf, "update ConstructionBuild set action=%d,starttime=%I64d"
			" where mapid=%d and cityid=%d and playerid=%d and conid=%d and level=%d and sortid=%d ",
			action_, starttime_, old.mapid_, old.cityid_, old.playerid_, old.conid_, old.level_, old.sortid_);
	}

	int get_delete_str(char * buf)
	{ 
		return sprintf(buf, "delete from ConstructionBuild where mapid=%d and cityid=%d and playerid=%d and conid=%d and level=%d and sortid=%d ", 
			mapid_, cityid_, playerid_, conid_, level_, sortid_);
	}

	static const char * get_select_str()
	{ return "select * from ConstructionBuild";}

	template < class otl_stream >
		friend otl_stream& operator>>(otl_stream& os, ConstructionBuild& p)
	{
		os >> p.sortid_ >> p.action_ >> p.playerid_ >> p.conid_ 
			>> p.level_ >> p.cityid_ >> p.mapid_ >> p.starttime_;
		return os;
	}

	ConstructionBuild& operator+=(const ConstructionBuild& p)
	{
		action_ += p.action_;
		starttime_ += p.starttime_;
		return *this;
	}

	ConstructionBuild()
	{
		memset((char *)this, 0, sizeof(*this));
	}
};

struct ProductionActivity
{
	int sortid_;
	int playerid_;
	int action_;
	int productionid_; 
	int level_;
	int type_;
	int cityid_; // mapid_ + type_ + cityid_ + playerid_ + productionid_ + level_ +sortid_= ID 一条
	int mapid_; // mapid_ + type_  + cityid_ + playerid_= id 多条
	int count_;
	int64 starttime_;

	static int gettype() 
	{ return CMD_PRODUCTION_ACTIVITY;}

	static const char * getname() 
	{ return "ProductionActivity";}

	inline void setmasterid(int mapid, int type, int cityid, int playerid) 
	{mapid_ = mapid;type_=type;cityid_=cityid;playerid_=playerid;}

	inline void getmasterid(char * buf) 
	{sprintf(buf, "%d-%d-%d-%d", mapid_, type_, cityid_, playerid_);}

	inline void setdetailid(int productionid, int level, int sortid) 
	{productionid_=productionid;level_=level;sortid_=sortid;}

	inline void getdetailid(char * buf) 
	{sprintf(buf, "%d-%d-%d", productionid_,level_,sortid_);}

	inline void setkeyvalue(int key){sortid_=key;}

	inline int getkeyvalue() { return sortid_; }

	int get_insert_str(char * buf)
	{ 
		//return sprintf(buf, "insert into ProductionActivity(sortid,playerid,action,productionid,level,type,cityid,mapid,count,starttime) values(%d,%d,%d,%d,%d,%d,%d,%d,%d,%I64d) ",
		return sprintf(buf, "insert into ProductionActivity values(%d,%d,%d,%d,%d,%d,%d,%d,%d,%I64d) ",
			sortid_, playerid_, action_, productionid_, level_, type_, cityid_, mapid_, count_, starttime_);
	}

	int get_update_str(char * buf, ProductionActivity & old)
	{
		return sprintf(buf, "update ProductionActivity set action=%d,count=%d,starttime=%I64d"
			" where mapid=%d and type=%d and cityid=%d and playerid=%d and productionid=%d and level=%d and sortid=%d ",
			action_, count_, starttime_, 
			old.mapid_, old.type_, old.cityid_, old.playerid_,  old.productionid_, old.level_, old.sortid_);
	}

	int get_delete_str(char * buf)
	{ 
		return sprintf(buf, "delete from ProductionActivity where mapid=%d and type=%d and cityid=%d and "
			"playerid=%d and productionid=%d and level=%d and sortid=%d ", 
			mapid_, type_, cityid_, playerid_, productionid_, level_, sortid_);
	}

	static const char * get_select_str()
	{ return "select * from ProductionActivity";}

	template < class otl_stream >
		friend otl_stream& operator>>(otl_stream& os, ProductionActivity& p)
	{
		os >> p.sortid_ >> p.playerid_ >> p.action_ >> p.productionid_ >> p.level_ 
			>> p.type_ >> p.cityid_ >> p.mapid_ >> p.count_ >> p.starttime_;
		return os;
	}

	ProductionActivity& operator+=(const ProductionActivity& p)
	{
		action_ += p.action_;
		count_ += p.count_;
		starttime_ += p.starttime_;
		return *this;
	}

	ProductionActivity()
	{
		memset((char *)this, 0, sizeof(*this));
	}
};

struct ConsortiaInfo
{
	int consortiaid_; // consortiaid_= id 一条
	char name_[SIZE_32];
	int mapid_;
	int cityid_;
	short level_;
	int chairmanid_;
	int techid_;
	int count_;
	int faceid_;
	char placard_[SIZE_256];
	char tenet_[SIZE_128];
	int64 createtime_;
	int maxusernum_;
	int money_;
	int exp_;
	int honors_;
	int signid_;
	char shortname_[5];

	static int gettype() 
	{ return CMD_CONSORTIA_INFO;}

	static const char * getname() 
	{ return "ConsortiaInfo";}

	inline void setid(int consortiaid) 
	{consortiaid_ = consortiaid;}

	inline void getid(char * buf) 
	{sprintf(buf, "%d", consortiaid_);}

	inline void setkeyvalue(int key){}

	inline int getkeyvalue() { return 0;}

	int get_insert_str(char * buf)
	{ 
		//return sprintf(buf, "insert into ConsortiaInfo(consortiaid,name,mapid,cityid,level,chairmanid,techid,count,faceid,placard,tenet,createtime)"
		//	"values(%d,\'\'%s\'\',%d,%d,%d,%d,%d,%d,%d,\'\'%s\'\',\'\'%s\'\',%I64d) ",
		return sprintf(buf, "insert into ConsortiaInfo values(%d,\'\'%s\'\',%d,%d,%d,%d,%d,%d,%d,\'\'%s\'\',\'\'%s\'\',%I64d,%d,%d,%d,%d,%d,\'\'%s\'\') ",
			consortiaid_, name_, mapid_, cityid_, level_, chairmanid_, techid_, count_, faceid_, placard_, tenet_, 
			createtime_, maxusernum_, money_, exp_, honors_, signid_, shortname_);
	}

	int get_update_str(char * buf, ConsortiaInfo & old)
	{
		return sprintf(buf, "update ConsortiaInfo set name=\'\'%s\'\',mapid=%d,cityid=%d,level=%d,chairmanid=%d,"
			"techid=%d,count=%d,faceid=%d, placard=\'\'%s\'\',tenet=\'\'%s\'\',createtime=%I64d,maxusernum=%d,money=%d,exp=%d,honors=%d,signid=%d,shortname=\'\'%s\'\' where consortiaid=%d ",
			name_, mapid_, cityid_, level_, chairmanid_, techid_, count_, faceid_, placard_, tenet_, createtime_, maxusernum_, money_, exp_, honors_, signid_, shortname_, old.consortiaid_);
	}

	int get_delete_str(char * buf)
	{ 
		return sprintf(buf, "delete from ConsortiaInfo where consortiaid=%d ", consortiaid_);
	}

	static const char * get_select_str()
	{ return "select * from ConsortiaInfo";}

	template < class otl_stream >
		friend otl_stream& operator>>(otl_stream& os, ConsortiaInfo& p)
	{
		os >> p.consortiaid_ >> p.name_ >> p.mapid_ >> p.cityid_ >> p.level_ 
			>> p.chairmanid_ >> p.techid_ >> p.count_ >> p.faceid_ >> p.placard_ 
			>> p.tenet_ >> p.createtime_ >> p.maxusernum_ >> p.money_ >> p.exp_ 
			>> p.honors_ >> p.signid_ >> p.shortname_;
		return os;
	}

	ConsortiaInfo& operator+=(const ConsortiaInfo& p)
	{
		mapid_ += p.mapid_;
		cityid_ += p.cityid_;
		level_ += p.level_;
		chairmanid_ += p.chairmanid_;
		techid_ += p.techid_;
		count_ += p.count_;
		faceid_ += p.faceid_;
		createtime_ += p.createtime_;
		maxusernum_ += p.maxusernum_;
		money_ += p.money_;
		exp_ += p.exp_;
		honors_ += p.honors_;
		signid_ += p.signid_;
		return *this;
	}

	ConsortiaInfo()
	{
		memset((char *)this, 0, sizeof(*this));
	}
};

struct ConsortiaBank
{
	int consortiaid_; // consortiaid_ = id 多条
	int itemid_; // consortiaid_ + itemid_ = id 一条
	short itemcount_;

	static int gettype() 
	{ return CMD_CONSORTIA_BANK;}

	static const char * getname() 
	{ return "ConsortiaBank";}

	inline void setmasterid(int consortiaid) 
	{consortiaid_ = consortiaid;}

	inline void getmasterid(char * buf) 
	{sprintf(buf, "%d", consortiaid_);}

	inline void setdetailid(int itemid) 
	{itemid_=itemid;}

	inline void getdetailid(char * buf) 
	{sprintf(buf, "%d", itemid_);}

	inline void setkeyvalue(int key){}

	inline int getkeyvalue() { return 0; }

	int get_insert_str(char * buf)
	{ 
		//return sprintf(buf, "insert into ConsortiaBank(consortiaid,itemid,itemcount) values(%d,%d,%d) ",consortiaid_, itemid_, itemcount_);
		return sprintf(buf, "insert into ConsortiaBank values(%d,%d,%d) ",consortiaid_, itemid_, itemcount_);
	}

	int get_update_str(char * buf, ConsortiaBank & old)
	{
		return sprintf(buf, "update ConsortiaBank set itemcount=%d where consortiaid=%d and itemid=%d ",
			itemcount_, old.consortiaid_, old.itemid_);
	}

	int get_delete_str(char * buf)
	{ 
		return sprintf(buf, "delete from ConsortiaBank where consortiaid=%d and itemid=%d ", consortiaid_, itemid_);
	}

	static const char * get_select_str()
	{ return "select * from ConsortiaBank";}

	template < class otl_stream >
		friend otl_stream& operator>>(otl_stream& os, ConsortiaBank& p)
	{
		os >> p.consortiaid_ >> p.itemid_ >> p.itemcount_;
		return os;
	}

	ConsortiaBank& operator+=(const ConsortiaBank& p)
	{
		itemcount_ += p.itemcount_;
		return *this;
	}

	ConsortiaBank()
	{
		memset((char *)this, 0, sizeof(*this));
	}
};

struct ConsortiaUnion
{
	int unionid_;
	int consortia1id_;
	int consortia2id_; // consortia1id_ + consortia2id_ = id 一条

	static int gettype() 
	{ return CMD_CONSORTIA_UNION;}

	static const char * getname() 
	{ return "ConsortiaUnion";}

	inline void setid(int consortia1id, int consortia2id) 
	{consortia1id_ = consortia1id;consortia2id_=consortia2id;}

	inline void getid(char * buf) 
	{sprintf(buf, "%d-%d", consortia1id_, consortia2id_);}

	inline void setkeyvalue(int key)
	{ unionid_ = key; }

	inline int getkeyvalue() { return unionid_; }

	int get_insert_str(char * buf)
	{ 
		//return sprintf(buf, "insert into ConsortiaUnion(unionid,consortia1id,consortia2id) values(%d,%d,%d) ",unionid_, consortia1id_, consortia2id_);
		return sprintf(buf, "insert into ConsortiaUnion values(%d,%d,%d) ",unionid_, consortia1id_, consortia2id_);
	}

	int get_update_str(char * buf, ConsortiaUnion & old)
	{
		return sprintf(buf, "update ConsortiaUnion set unionid=%d where consortia1id=%d and consortia2id=%d ",
			unionid_, old.consortia1id_, old.consortia2id_);
	}

	int get_delete_str(char * buf)
	{ 
		return sprintf(buf, "delete from ConsortiaUnion where consortia1id=%d and consortia2id=%d ", consortia1id_, consortia2id_);
	}

	static const char * get_select_str()
	{ return "select * from ConsortiaUnion";}

	template < class otl_stream >
		friend otl_stream& operator>>(otl_stream& os, ConsortiaUnion& p)
	{
		os >> p.unionid_ >> p.consortia1id_ >> p.consortia2id_;
		return os;
	}

	ConsortiaUnion& operator+=(const ConsortiaUnion& p)
	{
		unionid_ += p.unionid_;
		return *this;
	}

	ConsortiaUnion()
	{
		memset((char *)this, 0, sizeof(*this));
	}
};

struct PlayerOfficerItemTable
{
	int playerid_;
	int officerid_; // playerid_ + officerid_ = id 多条
	int itemid_;
	int pos_; // playerid_ + officerid_ + pos_ = id 一条
	int itemlevl_;

	static int gettype() 
	{ return CMD_PLAYER_OFFICER_ITEM_TABLE;}

	static const char * getname() 
	{ return "PlayerOfficerItemTable";}

	inline void setmasterid(int playerid, int officerid) 
	{playerid_ = playerid;officerid_=officerid;}

	inline void getmasterid(char * buf) 
	{sprintf(buf, "%d-%d", playerid_, officerid_);}

	inline void setdetailid(int pos) 
	{pos_ = pos;}

	inline void getdetailid(char * buf) 
	{sprintf(buf, "%d", pos_);}

	inline void setkeyvalue(int key){}

	inline int getkeyvalue() { return 0; }

	int get_insert_str(char * buf)
	{ 
		//return sprintf(buf, "insert into PlayerOfficerItemTable(playerid,officerid,itemid,pos,itemlevl) values(%d,%d,%d,%d,%d) ",playerid_, officerid_, itemid_, pos_, itemlevl_);
		return sprintf(buf, "insert into PlayerOfficerItemTable values(%d,%d,%d,%d,%d) ",playerid_, officerid_, itemid_, pos_, itemlevl_);
	}

	int get_update_str(char * buf, PlayerOfficerItemTable & old)
	{
		return sprintf(buf, "update PlayerOfficerItemTable set itemid=%d,itemlevl=%d where playerid=%d and officerid=%d and pos=%d ",
			itemid_, itemlevl_, old.playerid_, old.officerid_, old.pos_);
	}

	int get_delete_str(char * buf)
	{ 
		return sprintf(buf, "delete from PlayerOfficerItemTable where playerid=%d and officerid=%d and pos=%d ", playerid_, officerid_, pos_);
	}

	static const char * get_select_str()
	{ return "select * from PlayerOfficerItemTable";}

	template < class otl_stream >
		friend otl_stream& operator>>(otl_stream& os, PlayerOfficerItemTable& p)
	{
		os >> p.playerid_ >> p.officerid_ >> p.itemid_ >> p.pos_ >> p.itemlevl_;
		return os;
	}

	PlayerOfficerItemTable& operator+=(const PlayerOfficerItemTable& p)
	{
		itemid_ += p.itemid_;
		itemlevl_ += p.itemlevl_;
		return *this;
	}

	PlayerOfficerItemTable()
	{
		memset((char *)this, 0, sizeof(*this));
	}
};

struct PlayerItemStorageTable
{
	int sortid_;
	int playerid_;
	int officerid_;
	int type_;
	int64 itemid_; // officerid_ + playerid_= id 多条
	int level_; // officerid_ + playerid_ + sortid_ = id 一条
	int pos_;
	int showpos_;
	int count_;
	int tech_;
	int techlevel_;

	static int gettype() 
	{ return CMD_PLAYER_ITEM_STORAGE_TABLE;}

	static const char * getname() 
	{ return "PlayerItemStorageTable";}

	inline void setmasterid(int officerid, int playerid) 
	{officerid_ = officerid;playerid_=playerid;}

	inline void getmasterid(char * buf) 
	{sprintf(buf, "%d-%d", officerid_, playerid_);}

	inline void setdetailid(int sortid) 
	{sortid_ = sortid;}

	inline void getdetailid(char * buf) 
	{sprintf(buf, "%d", sortid_);}

	inline void setkeyvalue(int key){ sortid_ = key; }

	inline int getkeyvalue() { return sortid_; }

	int get_insert_str(char * buf)
	{ 
		//return sprintf(buf, "insert into PlayerItemStorageTable(sortid,playerid,officerid,type,itemid,level,pos,showpos,count) "
		return sprintf(buf, "insert into PlayerItemStorageTable values(%d,%d,%d,%d,%I64d,%d,%d,%d,%d,%d,%d) ",
			sortid_, playerid_, officerid_, type_, itemid_, level_, pos_, showpos_, count_, tech_, techlevel_);
	}

	int get_update_str(char * buf, PlayerItemStorageTable & old)
	{
		return sprintf(buf, "update PlayerItemStorageTable set officerid=%d,type=%d,itemid=%I64d,level=%d,"
			"pos=%d,showpos=%d,count=%d,tech=%d,techlevel=%d where officerid=%d and playerid=%d and sortid=%d ",
			officerid_, type_, itemid_, level_, pos_, showpos_, count_, tech_, techlevel_, old.officerid_, old.playerid_, old.sortid_);
	}

	int get_delete_str(char * buf)
	{ 
		return sprintf(buf, "delete from PlayerItemStorageTable where officerid=%d and playerid=%d and sortid=%d ",
			officerid_, playerid_, sortid_);
	}

	static const char * get_select_str()
	{ return "select * from PlayerItemStorageTable";}

	template < class otl_stream >
		friend otl_stream& operator>>(otl_stream& os, PlayerItemStorageTable& p)
	{
		os >> p.sortid_ >> p.playerid_ >> p.officerid_ >> p.type_ >> p.itemid_ 
			>> p.level_ >> p.pos_ >> p.showpos_ >> p.count_ >> p.tech_ >> p.techlevel_;
		return os;
	}

	PlayerItemStorageTable& operator+=(const PlayerItemStorageTable& p)
	{
		type_ += p.type_;
		itemid_ += p.itemid_;
		level_ += p.level_;
		pos_ += p.pos_;
		showpos_ += p.showpos_;
		count_ += p.count_;
		tech_ += p.tech_;
		techlevel_ += p.techlevel_;
		return *this;
	}

	PlayerItemStorageTable()
	{
		memset((char *)this, 0, sizeof(*this));
	}
};

struct SoldierParamTable
{
	int soldiertype_; // ID
	int hp_;
	int physicevade_;
	int magicevade_;
	int movefrequency_;
	int visualrange_;
	int attackscop_;
	int duckrate_;
	int shooting_;

	static int gettype() 
	{ return CMD_SOLDIER_PARAM_TABLE;}

	static const char * getname() 
	{ return "SoldierParamTable";}

	inline void setid(int soldiertype) 
	{soldiertype_ = soldiertype;}

	inline void getid(char * buf) 
	{sprintf(buf, "%d", soldiertype_);}

	inline void setkeyvalue(int key){}

	inline int getkeyvalue() { return 0; }

	int get_insert_str(char * buf)
	{ 
		//return sprintf(buf, "insert into SoldierParamTable(soldiertype,hp,physicevade,magicevade,movefrequency,visualrange,attackscop,duckrate,shooting)"
		return sprintf(buf, "insert into SoldierParamTable values(%d,%d,%d,%d,%d,%d,%d,%d,%d) ",
			soldiertype_, hp_, physicevade_, magicevade_, movefrequency_, visualrange_, attackscop_, duckrate_,shooting_);
	}

	int get_update_str(char * buf, SoldierParamTable & old)
	{
		return sprintf(buf, "update SoldierParamTable set hp=%d,physicevade=%d,magicevade=%d,"
			"movefrequency=%d,visualrange=%d,attackscop=%d,duckrate=%d,shooting=%d where soldiertype=%d ",
			hp_, physicevade_, magicevade_, movefrequency_, visualrange_, attackscop_, duckrate_,shooting_, old.soldiertype_);
	}

	int get_delete_str(char * buf)
	{ 
		return sprintf(buf, "delete from SoldierParamTable where soldiertype=%d ", soldiertype_);
	}

	static const char * get_select_str()
	{ return "select * from SoldierParamTable";}

	template < class otl_stream >
		friend otl_stream& operator>>(otl_stream& os, SoldierParamTable& p)
	{
		os >> p.soldiertype_ >> p.hp_ >> p.physicevade_ >> p.magicevade_ >> p.movefrequency_ 
			>> p.visualrange_ >> p.attackscop_ >> p.duckrate_ >> p.shooting_;
		return os;
	}

	SoldierParamTable& operator+=(const SoldierParamTable& p)
	{
		hp_ += p.hp_;
		physicevade_ += p.physicevade_;
		magicevade_ += p.magicevade_;
		movefrequency_ += p.movefrequency_;
		visualrange_ += p.visualrange_;
		attackscop_ += p.attackscop_;
		duckrate_ += p.duckrate_;
		shooting_ += p.shooting_;
		return *this;
	}

	SoldierParamTable()
	{
		memset((char *)this, 0, sizeof(*this));
	}
};

struct PlayerOfficerTable
{
	int officerid_;
	int playerid_; // officerid_ + playerid_ = ID 一条 // playerid_ = ID 多条
	char officername_[SIZE_24];
	int officerrace_;
	int level_;
	int militaryrank_;
	int jobid_;
	int faceid_;
	int exp_;
	int credit_;
	int enemycount_;
	int generalcount_;
	int mapid_;
	int cityid_;
	int armyid_;
	int state_;

	// 徐进才删除
	//int tech1id_;
	//int tech1level_;
	//int tech2id_;
	//int tech2level_;
	//int tech3id_;
	//int tech3level_;
	//int tech4id_;
	//int tech4level_;
	/////////////////


	int stamina_;
	int power_;
	int celerity_;
	int wit_;
	int techdot_;
	int attack_;
	int physicrecovery_;
	int magicrecovery_;
	int actiondot_;
	int64 updatetime_;
	int64 gbhtime_;
	int levelstamina_;
	int levelpower_;
	int levelcelerity_;
	int levelwit_;
	int equipstamina_;
	int equippower_;
	int equipcelerity_;
	int equipwit_;
	int magicattack_;
	int herohp_;
	int injurerate_;
	int capturerate_;
	int beinjurerate_;
	int becapturerate_;
	int forcerate_;

	// 徐进才增加
	int lsttotalattri_;
	int lstmainattri_;
	int lststamina_;
	int grouprate_;
	//////////////

	static int gettype() 
	{ return CMD_PLAYER_OFFICER_TABLE;}

	static const char * getname() 
	{ return "PlayerOfficerTable";}

	inline void setmasterid(int playerid) 
	{playerid_ = playerid;}

	inline void getmasterid(char * buf) 
	{sprintf(buf, "%d", playerid_);}

	inline void setdetailid(int officerid) 
	{officerid_ = officerid;}

	inline void getdetailid(char * buf) 
	{sprintf(buf, "%d", officerid_);}

	inline void setkeyvalue(int key){}

	inline int getkeyvalue() { return 0; }

	int get_insert_str(char * buf)
	{ 
		//return sprintf(buf, "insert into PlayerOfficerTable(officerid,playerid,officername,officerrace,level,militaryrank,"
		//	"jobid,faceid,exp,credit,enemycount,generalcount,mapid,cityid,armyid,state,tech1id,tech1level,tech2id,tech2level,tech3id,"
		//	"tech3level,tech4id,tech4level,stamina,power,celerity,wit,techdot,attack,physicrecovery,magicrecovery,actiondot,"
		//	"updatetime,gbhtime) values(%d,%d,\'\'%s\'\',%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,"
		//	"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%I64d,%I64d) ",
		return sprintf(buf, "insert into PlayerOfficerTable values(%d,%d,\'\'%s\'\',%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,"
			"%d,%d,%d,%d,%d,%d,%d,%d,%d,%I64d,%I64d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d) ",
			officerid_, playerid_, officername_, officerrace_, level_, militaryrank_, 
			jobid_, faceid_, exp_, credit_, enemycount_, generalcount_, mapid_, cityid_, 
			armyid_, state_, stamina_, power_, celerity_, wit_, 
			techdot_, attack_, physicrecovery_, magicrecovery_, actiondot_, updatetime_, gbhtime_,
			levelstamina_, levelpower_, levelcelerity_, levelwit_, equipstamina_, equippower_, 
			equipcelerity_, equipwit_, magicattack_, herohp_, injurerate_, capturerate_, 
			beinjurerate_, becapturerate_, forcerate_,lsttotalattri_,lstmainattri_,lststamina_,grouprate_);
	}

	int get_update_str(char * buf, PlayerOfficerTable & old)
	{
		return sprintf(buf, "update PlayerOfficerTable set officername=\'\'%s\'\',officerrace=%d,level=%d,militaryrank=%d,"
			"jobid=%d,faceid=%d,exp=%d,credit=%d,enemycount=%d,generalcount=%d,mapid=%d,cityid=%d,armyid=%d,state=%d,stamina=%d,"
			"power=%d,celerity=%d,wit=%d,techdot=%d,attack=%d,physicrecovery=%d,magicrecovery=%d,actiondot=%d,updatetime=%I64d,"
			"gbhtime=%I64d,levelstamina=%d,levelpower=%d,levelcelerity=%d,levelwit=%d,equipstamina=%d,equippower=%d," 
			"equipcelerity=%d,equipwit=%d,magicattack=%d,herohp=%d,injurerate=%d,capturerate=%d,beinjurerate=%d,"
			"lsttotalattri=%d,lstmainattri=%d,lststamina=%d,grouprate=%d,"
			"becapturerate=%d,forcerate=%d where playerid=%d and officerid=%d ",
			officername_, officerrace_, level_, militaryrank_, 
			jobid_, faceid_, exp_, credit_, enemycount_, generalcount_, mapid_, cityid_, 
			armyid_, state_, stamina_, power_, celerity_, wit_, 
			techdot_, attack_, physicrecovery_, magicrecovery_, actiondot_, updatetime_, gbhtime_, 
			levelstamina_, levelpower_, levelcelerity_, levelwit_, equipstamina_, equippower_, 
			equipcelerity_, equipwit_, magicattack_, herohp_, injurerate_, capturerate_, 
			beinjurerate_, lsttotalattri_,lstmainattri_,lststamina_,grouprate_,
			becapturerate_, forcerate_, old.playerid_, old.officerid_);
	}

	int get_delete_str(char * buf)
	{ 
		return sprintf(buf, "delete from PlayerOfficerTable where playerid=%d and officerid=%d ", playerid_, officerid_);
	}

	static const char * get_select_str()
	{ return "select * from PlayerOfficerTable";}

	template < class otl_stream >
		friend otl_stream& operator>>(otl_stream& os, PlayerOfficerTable& p)
	{
		os >> p.officerid_ >> p.playerid_ >> p.officername_ >> p.officerrace_ 
			>> p.level_ >> p.militaryrank_ >> p.jobid_ >> p.faceid_ >> p.exp_ 
			>> p.credit_ >> p.enemycount_ >> p.generalcount_ >> p.mapid_ >> p.cityid_ 
			>> p.armyid_ >> p.state_ >> p.stamina_ >> p.power_ >> p.celerity_ >> p.wit_ 
			>> p.techdot_ >> p.attack_ >> p.physicrecovery_ >> p.magicrecovery_ 
			>> p.actiondot_ >> p.updatetime_ >> p.gbhtime_ >> p.levelstamina_ 
			>> p.levelpower_ >> p.levelcelerity_ >> p.levelwit_ >> p.equipstamina_ 
			>> p.equippower_ >> p.equipcelerity_ >> p.equipwit_ >> p.magicattack_ 
			>> p.herohp_ >> p.injurerate_ >> p.capturerate_ >> p.beinjurerate_ 
			>> p.becapturerate_ >> p.forcerate_
			>> p.lsttotalattri_ >> p.lstmainattri_ >> p.lststamina_ >> p.grouprate_;
		return os;
	}

	PlayerOfficerTable& operator+=(const PlayerOfficerTable& p)
	{
		officerrace_ += p.officerrace_;
		level_ += p.level_;
		militaryrank_ += p.militaryrank_;
		jobid_ += p.jobid_;
		faceid_ += p.faceid_;
		exp_ += p.exp_;
		credit_ += p.credit_;
		enemycount_ += p.enemycount_;
		generalcount_ += p.generalcount_;
		mapid_ += p.mapid_;
		cityid_ += p.cityid_;
		armyid_ += p.armyid_;
		state_ += p.state_;
		stamina_ += p.stamina_;
		power_ += p.power_;
		celerity_ += p.celerity_;
		wit_ += p.wit_;
		techdot_ += p.techdot_;
		attack_ += p.attack_;
		physicrecovery_ += p.physicrecovery_;
		magicrecovery_ += p.magicrecovery_;
		actiondot_ += p.actiondot_;
		updatetime_ += p.updatetime_;
		gbhtime_ += p.gbhtime_;
		levelstamina_ += p.levelstamina_;
		levelpower_ += p.levelpower_;
		levelcelerity_ += p.levelcelerity_;
		levelwit_ += p.levelwit_;
		equipstamina_ += p.equipstamina_;
		equippower_ += p.equippower_;
		equipcelerity_ += p.equipcelerity_;
		equipwit_ += p.equipwit_;
		magicattack_ += p.magicattack_;
		herohp_ += p.herohp_;
		injurerate_ += p.injurerate_;
		capturerate_ += p.capturerate_;
		beinjurerate_ += p.beinjurerate_;
		becapturerate_ += p.becapturerate_;
		forcerate_ += p.forcerate_;

		lsttotalattri_ += p.lsttotalattri_;
		lstmainattri_ += p.lstmainattri_;
		lststamina_ += p.lststamina_;
		grouprate_ += p.grouprate_;

		return *this;
	}

	PlayerOfficerTable()
	{
		memset((char *)this, 0, sizeof(*this));
	}
};

struct PlayerTask
{
	int playerid_; // playerid_ = id 多条
	int taskid_; // playerid_ + taskid_ = id 一条
	int state_;
	int64 tasktime_;

	static int gettype() 
	{ return CMD_PLAYER_TASK;}

	static const char * getname() 
	{ return "PlayerTask";}

	inline void setmasterid(int playerid) 
	{playerid_ = playerid;}

	inline void getmasterid(char * buf) 
	{sprintf(buf, "%d", playerid_);}

	inline void setdetailid(int taskid) 
	{taskid_ = taskid;}

	inline void getdetailid(char * buf) 
	{sprintf(buf, "%d", taskid_);}

	inline void setkeyvalue(int key) {}

	inline int getkeyvalue() { return 0; }

	int get_insert_str(char * buf)
	{ 
		//return sprintf(buf, "insert into PlayerTask(playerid,taskid,state,tasktime) values(%d,%d,%d,%I64d) ", playerid_, taskid_, state_, tasktime_);
		return sprintf(buf, "insert into PlayerTask values(%d,%d,%d,%I64d) ", playerid_, taskid_, state_, tasktime_);
	}

	int get_update_str(char * buf, PlayerTask & old)
	{
		return sprintf(buf, "update PlayerTask set state=%d,tasktime=%I64d where playerid=%d and taskid=%d ",
			state_, tasktime_, old.playerid_, old.taskid_);
	}

	int get_delete_str(char * buf)
	{ 
		return sprintf(buf, "delete from PlayerTask where playerid=%d and taskid=%d ", playerid_, taskid_);
	}

	static const char * get_select_str()
	{ return "select * from PlayerTask";}

	template < class otl_stream >
		friend otl_stream& operator>>(otl_stream& os, PlayerTask& p)
	{
		os >> p.playerid_ >> p.taskid_ >> p.state_ >> p.tasktime_;
		return os;
	}

	PlayerTask& operator+=(const PlayerTask& p)
	{
		state_ += p.state_;
		tasktime_ += p.tasktime_;
		return *this;
	}

	PlayerTask()
	{
		memset((char *)this, 0, sizeof(*this));
	}
};

struct ConsortiaPersonnel
{
	int consortiaid_;
	int playerid_; // consortiaid_ + playerid_ = id 一条
	int conjob_;
	int purview_;
	int type_; // consortiaid_ = id 多条
	int contribute_;
	int legiontitle_;
	char salariedday_[9];
	int honors_;
	int exp_;
	int money_;

	static int gettype() 
	{ return CMD_CONSORTIA_PERSONNEL;}

	static const char * getname() 
	{ return "ConsortiaPersonnel";}

	inline void setmasterid(int consortiaid) 
	{consortiaid_ = consortiaid;}

	inline void getmasterid(char * buf) 
	{sprintf(buf, "%d", consortiaid_);}

	inline void setdetailid(int playerid) 
	{playerid_ = playerid;}

	inline void getdetailid(char * buf) 
	{sprintf(buf, "%d", playerid_);}

	inline void setkeyvalue(int key){}

	inline int getkeyvalue() { return 0; }

	int get_insert_str(char * buf)
	{ 
		//return sprintf(buf, "insert into ConsortiaPersonnel(consortiaid,playerid,conjob,purview,type) values(%d,%d,%d,%d,%d) ", 
		return sprintf(buf, "insert into ConsortiaPersonnel values(%d,%d,%d,%d,%d,%d,%d,\'\'%s\'\',%d,%d,%d) ", 
			consortiaid_, playerid_, conjob_, purview_, type_, contribute_, legiontitle_, salariedday_, honors_, exp_, money_);
	}

	int get_update_str(char * buf, ConsortiaPersonnel & old)
	{
		return sprintf(buf, "update ConsortiaPersonnel set conjob=%d,"
			"purview=%d,type=%d,contribute=%d,legiontitle=%d,salariedday=\'\'%s\'\',honors=%d,exp=%d,money=%d where consortiaid=%d and playerid=%d ",
			conjob_, purview_, type_, contribute_, legiontitle_, salariedday_, honors_, exp_, money_, old.consortiaid_, old.playerid_);
	}

	int get_delete_str(char * buf)
	{ 
		return sprintf(buf, "delete from ConsortiaPersonnel where consortiaid=%d and playerid=%d ", consortiaid_, playerid_);
	}

	static const char * get_select_str()
	{ return "select * from ConsortiaPersonnel";}

	template < class otl_stream >
		friend otl_stream& operator>>(otl_stream& os, ConsortiaPersonnel& p)
	{
		os >> p.consortiaid_ >> p.playerid_ >> p.conjob_ >> p.purview_ >> p.type_ >> p.contribute_ 
			>> p.legiontitle_ >> p.salariedday_ >> p.honors_ >> p.exp_ >> p.money_;
		return os;
	}

	ConsortiaPersonnel& operator+=(const ConsortiaPersonnel& p)
	{
		conjob_ += p.conjob_;
		purview_ += p.purview_;
		type_ += p.type_;
		contribute_ += p.contribute_;
		legiontitle_ += p.legiontitle_;
		honors_ += p.honors_;
		exp_ += p.exp_;
		money_ += p.money_;
		return *this;
	}

	ConsortiaPersonnel()
	{
		memset((char *)this, 0, sizeof(*this));
	}
};

struct CellPrice
{
	int priceid_; // 一条
	int foodprice_;
	int woodprice_;
	int ironprice_;
	int skinprice_;
	int redstoneprice_;
	int yollowstoneprice_;
	int greenstoneprice_;
	int backironprice_;

	static int gettype() 
	{ return CMD_CELL_PRICE;}

	static const char * getname() 
	{ return "CellPrice";}

	inline void setid(int priceid) 
	{priceid_ = priceid;}

	inline void getid(char * buf) 
	{sprintf(buf, "%d", priceid_);}

	inline void setkeyvalue(int key){}

	inline int getkeyvalue() { return 0; }

	int get_insert_str(char * buf)
	{ 
		//return sprintf(buf, "insert into CellPrice(priceid,foodprice,woodprice,ironprice,skinprice,"
		//	"redstoneprice,yollowstoneprice,greenstoneprice,backironprice) values(%d,%d,%d,%d,%d,%d,%d,%d,%d) ", 
		return sprintf(buf, "insert into CellPrice values(%d,%d,%d,%d,%d,%d,%d,%d,%d) ", 
			priceid_, foodprice_, woodprice_, ironprice_, skinprice_, 
			redstoneprice_, yollowstoneprice_, greenstoneprice_, backironprice_);
	}

	int get_update_str(char * buf, CellPrice & old)
	{
		return sprintf(buf, "update CellPrice set foodprice=%d,woodprice=%d,ironprice=%d,skinprice=%d,"
			"redstoneprice=%d,yollowstoneprice=%d,greenstoneprice=%d,backironprice=%d where priceid=%d ",
			foodprice_, woodprice_, ironprice_, skinprice_, redstoneprice_, 
			yollowstoneprice_, greenstoneprice_, backironprice_, old.priceid_);
	}

	int get_delete_str(char * buf)
	{ 
		return sprintf(buf, "delete from CellPrice where priceid=%d ", priceid_);
	}

	static const char * get_select_str()
	{ return "select * from CellPrice";}

	template < class otl_stream >
		friend otl_stream& operator>>(otl_stream& os, CellPrice& p)
	{
		os >> p.priceid_ >> p.foodprice_ >> p.woodprice_ >> p.ironprice_ 
			>> p.skinprice_ >> p.redstoneprice_ >> p.yollowstoneprice_ 
			>> p.greenstoneprice_ >> p.backironprice_;
		return os;
	}

	CellPrice& operator+=(const CellPrice& p)
	{
		foodprice_ += p.foodprice_;
		woodprice_ += p.woodprice_;
		ironprice_ += p.ironprice_;
		skinprice_ += p.skinprice_;
		redstoneprice_ += p.redstoneprice_;
		yollowstoneprice_ += p.yollowstoneprice_;
		greenstoneprice_ += p.greenstoneprice_;
		backironprice_ += p.backironprice_;
		return *this;
	}

	CellPrice()
	{
		memset((char *)this, 0, sizeof(*this));
	}
};

struct CityAttribute
{
	int mapid_; // mapid_ = id 一条
	int cityid_; // mapid_ + cityid_ = id 一条
	int hp_;
	int attack_;

	static int gettype() 
	{ return CMD_CITY_ATTRIBUTE;}

	static const char * getname() 
	{ return "CityAttribute";}

	inline void setmasterid(int mapid) 
	{mapid_ = mapid;}

	inline void getmasterid(char * buf) 
	{sprintf(buf, "%d", mapid_);}

	inline void setdetailid(int cityid) 
	{cityid_ = cityid;}

	inline void getdetailid(char * buf) 
	{ sprintf(buf, "%d", cityid_);}

	inline void setkeyvalue(int key){}

	inline int getkeyvalue() { return 0; }

	int get_insert_str(char * buf)
	{ 
		//return sprintf(buf, "insert into CityAttribute(mapid,cityid,hp,attack) values(%d,%d,%d,%d) ",
		return sprintf(buf, "insert into CityAttribute values(%d,%d,%d,%d) ",
			mapid_, cityid_, hp_, attack_);
	}

	int get_update_str(char * buf, CityAttribute & old)
	{
		return sprintf(buf, "update CityAttribute set hp=%d,attack=%d where mapid=%d and cityid=%d ",
			hp_, attack_, old.mapid_, old.cityid_);
	}

	int get_delete_str(char * buf)
	{ 
		return sprintf(buf, "delete from CityAttribute where mapid=%d and cityid=%d ", mapid_, cityid_);
	}

	static const char * get_select_str()
	{ return "select * from CityAttribute";}

	template < class otl_stream >
		friend otl_stream& operator>>(otl_stream& os, CityAttribute& p)
	{
		os >> p.mapid_ >> p.cityid_ >> p.hp_ >> p.attack_;
		return os;
	}

	CityAttribute& operator+=(const CityAttribute& p)
	{
		hp_ += p.hp_;
		attack_ += p.attack_;
		return *this;
	}

	CityAttribute()
	{
		memset((char *)this, 0, sizeof(*this));
	}
};

//徐进才删除
//struct CreateConsortia
//{
//	int resid_; // 一条
//	int foodcount_;
//	int woodcount_;
//	int ironcount_;
//	int skincount_;
//	int redstonecount_;
//	int yollowstonecount_;
//	int greenstonecount_;
//	int backironcount_;
//	int times_;
//	int personnel_;
//	int science_;
//	int money_;
//	int level_;
//	int joinlevel_;
//
//	static int gettype() 
//	{ return CMD_CREATE_CONSORTIA;}
//
//	static const char * getname() 
//	{ return "CreateConsortia";}
//
//	inline void setid(int resid) 
//	{resid_ = resid;}
//
//	inline void getid(char * buf) 
//	{ sprintf(buf, "%d", resid_);}
//
//	inline void setkeyvalue(int key){}
//
//	inline int getkeyvalue() { return 0; }
//
//	int get_insert_str(char * buf)
//	{ 
//		//return sprintf(buf, "insert into CreateConsortia(resid,foodcount,woodcount,ironcount,skincount,redstonecount,yollowstonecount,"
//		//	"greenstonecount,backironcount,times,personnel,science,money,level,joinlevel) values(%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d) ",
//		return sprintf(buf, "insert into CreateConsortia values(%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d) ",
//			resid_, foodcount_, woodcount_, ironcount_, skincount_, redstonecount_, yollowstonecount_, 
//			greenstonecount_, backironcount_, times_, personnel_, science_, money_, level_, joinlevel_);
//	}
//
//	int get_update_str(char * buf, CreateConsortia & old)
//	{
//		return sprintf(buf, "update CreateConsortia set foodcount=%d,woodcount=%d,ironcount=%d,skincount=%d,"
//			"redstonecount=%d,yollowstonecount=%d,greenstonecount=%d,backironcount=%d,times=%d,"
//			"personnel=%d,science=%d,money=%d,level=%d,joinlevel=%d where resid=%d ",
//			foodcount_, woodcount_, ironcount_, skincount_, redstonecount_, yollowstonecount_, 
//			greenstonecount_, backironcount_, times_, personnel_, science_, money_, level_, joinlevel_, old.resid_);
//	}
//
//	int get_delete_str(char * buf)
//	{ 
//		return sprintf(buf, "delete from CreateConsortia where resid=%d ", resid_);
//	}
//
//	static const char * get_select_str()
//	{ return "select * from CreateConsortia";}
//
//	template < class otl_stream >
//		friend otl_stream& operator>>(otl_stream& os, CreateConsortia& p)
//	{
//		os >> p.resid_>> p.foodcount_>> p.woodcount_>> p.ironcount_
//			>> p.skincount_>> p.redstonecount_>> p.yollowstonecount_
//			>> p.greenstonecount_>> p.backironcount_>> p.times_
//			>> p.personnel_>> p.science_>> p.money_>> p.level_>> p.joinlevel_;
//		return os;
//	}
//
//	CreateConsortia& operator+=(const CreateConsortia& p)
//	{
//		foodcount_ += p.foodcount_;
//		woodcount_ += p.woodcount_;
//		ironcount_ += p.ironcount_;
//		skincount_ += p.skincount_;
//		redstonecount_ += p.redstonecount_;
//		yollowstonecount_ += p.yollowstonecount_;
//		greenstonecount_ += p.greenstonecount_;
//		backironcount_ += p.backironcount_;
//		times_ += p.times_;
//		personnel_ += p.personnel_;
//		science_ += p.science_;
//		money_ += p.money_;
//		level_ += p.level_;
//		joinlevel_ += p.joinlevel_;
//		return *this;
//	}
//
//	CreateConsortia()
//	{
//		memset((char *)this, 0, sizeof(*this));
//	}
//};

struct ConsortiaFight
{
	int fightid_;
	int consortia1id_;
	int consortia2id_; // mapid_ + consortia1id_ + consortia2id_ = id 一条
	int mapid_; // mapid_ = id 多条
	int cityid_;
	int64 time_;

	static int gettype() 
	{ return CMD_CONSORTIA_FIGHT;}

	static const char * getname() 
	{ return "ConsortiaFight";}

	inline void setmasterid(int mapid) 
	{mapid_ = mapid;}

	inline void getmasterid(char * buf) 
	{ sprintf(buf, "%d", mapid_);}

	inline void setdetailid(int consortia1id, int consortia2id) 
	{consortia1id_ = consortia1id;consortia2id_=consortia2id;}

	inline void getdetailid(char * buf) 
	{ sprintf(buf, "%d-%d", consortia1id_,consortia2id_);}

	inline void setkeyvalue(int key)
	{fightid_=key;}

	inline int getkeyvalue() { return fightid_; }

	int get_insert_str(char * buf)
	{ 
		//return sprintf(buf, "insert into ConsortiaFight(fightid,consortia1id,consortia2id,mapid,cityid,time) values(%d,%d,%d,%d,%d,%I64d) ",
		return sprintf(buf, "insert into ConsortiaFight values(%d,%d,%d,%d,%d,%I64d) ",
			fightid_, consortia1id_, consortia2id_, mapid_, cityid_, time_);
	}

	int get_update_str(char * buf, ConsortiaFight & old)
	{
		return sprintf(buf, "update ConsortiaFight set fightid=%d,"
			"cityid=%d,time=%I64d where mapid=%d and consortia1id=%d and consortia2id=%d ",
			fightid_, cityid_, time_, old.mapid_, old.consortia1id_, old.consortia2id_);
	}

	int get_delete_str(char * buf)
	{ 
		return sprintf(buf, "delete from ConsortiaFight where mapid=%d and consortia1id=%d and consortia2id=%d ", 
			mapid_, consortia1id_, consortia2id_);
	}

	static const char * get_select_str()
	{ return "select * from ConsortiaFight";}

	template < class otl_stream >
		friend otl_stream& operator>>(otl_stream& os, ConsortiaFight& p)
	{
		os >> p.fightid_ >> p.consortia1id_ >> p.consortia2id_
			>> p.mapid_ >> p.cityid_ >> p.time_;
		return os;
	}

	ConsortiaFight& operator+=(const ConsortiaFight& p)
	{
		fightid_ += p.fightid_;
		cityid_ += p.cityid_;
		time_ += p.time_;
		return *this;
	}
	ConsortiaFight()
	{
		memset((char *)this, 0, sizeof(*this));
	}
};

// 徐进才删除
//struct TaskBaseTable
//{
//	int taskid_; // taskid = 一条
//	char taskname_[SIZE_32];
//	int tasktype_;
//	int tasklevel_;
//	int playerlevel_;
//	int playerrank_;
//	int taskitem_;
//	int prvtaskid_;
//	char bonus_[SIZE_128];
//	char contest_[SIZE_128];
//	int mailtype_;
//
//	static int gettype() 
//	{ return CMD_TASK_BASE_TABLE;}
//
//	static const char * getname() 
//	{ return "TaskBaseTable";}
//
//	inline void setid(int taskid) 
//	{taskid_ = taskid;}
//
//	inline void getid(char * buf) 
//	{ sprintf(buf, "%d", taskid_);}
//
//	inline void setkeyvalue(int key)
//	{ taskid_ = key; }
//
//	inline int getkeyvalue() { return taskid_; }
//
//	int get_insert_str(char * buf)
//	{ 
//		//return sprintf(buf, "insert into TaskBaseTable(taskid,taskname,tasktype,tasklevel,plyaerlevel,"
//		//	"playerrank,taskitem,prvtaskid,bonus,contest,mailtype) values(%d,\'\'%s\'\',%d,%d,%d,%d,%d,%d,\'\'%s\'\',\'\'%s\'\',%d) ",
//		return sprintf(buf, "insert into TaskBaseTable values(%d,\'\'%s\'\',%d,%d,%d,%d,%d,%d,\'\'%s\'\',\'\'%s\'\',%d) ",
//			taskid_, taskname_, tasktype_, tasklevel_, playerlevel_, 
//			playerrank_, taskitem_, prvtaskid_, bonus_, contest_, mailtype_);
//	}
//
//	int get_update_str(char * buf, TaskBaseTable & old)
//	{
//		return sprintf(buf, "update TaskBaseTable set taskname=\'\'%s\'\',tasktype=%d,tasklevel=%d,plyaerlevel=%d,"
//			"playerrank=%d,taskitem=%d,prvtaskid=%d,bonus=\'\'%s\'\',contest=\'\'%s\'\',mailtype=%d where taskid=%d ",
//			taskname_, tasktype_, tasklevel_, playerlevel_, playerrank_, 
//			taskitem_, prvtaskid_, bonus_, contest_, mailtype_, old.taskid_);
//	}
//
//	int get_delete_str(char * buf)
//	{ 
//		return sprintf(buf, "delete from TaskBaseTable where taskid=%d ", taskid_);
//	}
//
//	static const char * get_select_str()
//	{ return "select * from TaskBaseTable";}
//
//	template < class otl_stream >
//		friend otl_stream& operator>>(otl_stream& os, TaskBaseTable& p)
//	{
//		os >> p.taskid_ >> p.taskname_ >> p.tasktype_ >> p.tasklevel_
//			>> p.playerlevel_ >> p.playerrank_ >> p.taskitem_ 
//			>> p.prvtaskid_ >> p.bonus_ >> p.contest_ >> p.mailtype_;
//		return os;
//	}
//
//	TaskBaseTable& operator+=(const TaskBaseTable& p)
//	{
//		tasktype_ += p.tasktype_;
//		tasklevel_ += p.tasklevel_;
//		playerlevel_ += p.playerlevel_;
//		playerrank_ += p.playerrank_;
//		taskitem_ += p.taskitem_;
//		prvtaskid_ += p.prvtaskid_;
//		mailtype_ += p.mailtype_;
//		return *this;
//	}
//	TaskBaseTable()
//	{
//		memset((char *)this, 0, sizeof(*this));
//	}
//};

//徐进才删除
//struct ProduceConfigure
//{
//	int itemid_; // itemtype_ + itemid_ + level_ = id 一条
//	int weanpontypeid_;
//	int itemtype_; // itemtype_ = id 多条
//	int level_;
//	int foodcount_;
//	int woodcount_;
//	int ironcount_;
//	int skincount_;
//	int redstonecount_;
//	int yellowstonecount_;
//	int greenstonecount_;
//	int backironcount_;
//	int money_;
//	int science_;
//	int64 completetimes_;
//
//	static int gettype() 
//	{ return CMD_PRODUCE_CONFIGURE;}
//
//	static const char * getname() 
//	{ return "ProduceConfigure";}
//
//	inline void setmasterid(int itemtype) 
//	{itemtype_ = itemtype;}
//
//	inline void getmasterid(char * buf) 
//	{ sprintf(buf, "%d", itemtype_);}
//
//	inline void setdetailid(int itemid, int level) 
//	{itemid_ = itemid;level_=level;}
//
//	inline void getdetailid(char * buf) 
//	{ sprintf(buf, "%d-%d", itemid_,level_);}
//
//	inline void setkeyvalue(int key) {}
//
//	inline int getkeyvalue() { return 0; }
//
//	int get_insert_str(char * buf)
//	{ 
//		//return sprintf(buf, "insert into ProduceConfigure(itemid,weanpontypeid,itemtype,level,foodcount,woodcount,ironcount,skincount,"
//		//	"redstonecount,yellowstonecount,greenstonecount,backironcount,money,science,completetimes) values(%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%I64d) ",
//		return sprintf(buf, "insert into ProduceConfigure values(%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%I64d) ",
//			itemid_, weanpontypeid_, itemtype_, level_, foodcount_, woodcount_, ironcount_, skincount_, 
//			redstonecount_, yellowstonecount_, greenstonecount_, backironcount_, money_, science_, completetimes_);
//	}
//
//	int get_update_str(char * buf, ProduceConfigure & old)
//	{
//		return sprintf(buf, "update ProduceConfigure set weanpontypeid=%d,foodcount=%d,"
//			"woodcount=%d,ironcount=%d,skincount=%d,redstonecount=%d,yellowstonecount=%d,"
//			"greenstonecount=%d,backironcount=%d,money=%d,science=%d,completetimes=%I64d"
//			" where itemtype=%d and itemid=%d and level=%d ",
//			weanpontypeid_, foodcount_, woodcount_, ironcount_, skincount_, 
//			redstonecount_, yellowstonecount_, greenstonecount_, backironcount_, 
//			money_, science_, completetimes_, old.itemtype_, old.itemid_, old.level_);
//	}
//
//	int get_delete_str(char * buf)
//	{ 
//		return sprintf(buf, "delete from ProduceConfigure where itemtype=%d and itemid=%d and level=%d ", 
//			itemtype_, itemid_, level_);
//	}
//
//	static const char * get_select_str()
//	{ return "select * from ProduceConfigure";}
//
//	template < class otl_stream >
//		friend otl_stream& operator>>(otl_stream& os, ProduceConfigure& p)
//	{
//		os >> p.itemid_ >> p.weanpontypeid_ >> p.itemtype_ >> p.level_ 
//			>> p.foodcount_ >> p.woodcount_ >> p.ironcount_ >> p.skincount_ 
//			>> p.redstonecount_ >> p.yellowstonecount_ >> p.greenstonecount_ 
//			>> p.backironcount_ >> p.money_ >> p.science_ >> p.completetimes_;
//		return os;
//	}
//
//	ProduceConfigure& operator+=(const ProduceConfigure& p)
//	{
//		weanpontypeid_ += p.weanpontypeid_;
//		foodcount_ += p.foodcount_;
//		woodcount_ += p.woodcount_;
//		ironcount_ += p.ironcount_;
//		skincount_ += p.skincount_;
//		redstonecount_ += p.redstonecount_;
//		yellowstonecount_ += p.yellowstonecount_;
//		greenstonecount_ += p.greenstonecount_;
//		backironcount_ += p.backironcount_;
//		money_ += p.money_;
//		science_ += p.science_;
//		completetimes_ += p.completetimes_;
//		return *this;
//	}
//	ProduceConfigure()
//	{
//		memset((char *)this, 0, sizeof(*this));
//	}
//};

struct ChatRoomTable
{
	int roomid_; // roomid = 一条
	int player1_;
	int player2_;
	int player3_;
	int player4_;
	int player5_;
	int player6_;
	int player7_;
	int player8_;

	static int gettype() 
	{ return CMD_CHAT_ROOM_TABLE;}

	static const char * getname() 
	{ return "ChatRoomTable";}

	inline void setid(int roomid) 
	{roomid_ = roomid;}

	inline void getid(char * buf) 
	{ sprintf(buf, "%d", roomid_);}

	inline void setkeyvalue(int key) 
	{ roomid_ = key; }

	inline int getkeyvalue() { return roomid_; }

	int get_insert_str(char * buf)
	{ 
		//return sprintf(buf, "insert into ChatRoomTable(roomid,player1,player2,player3,player4,player5,player6,player7,player8)"
		//	"values(%d,%d,%d,%d,%d,%d,%d,%d,%d) ",
		return sprintf(buf, "insert into ChatRoomTable values(%d,%d,%d,%d,%d,%d,%d,%d,%d) ",
			roomid_, player1_, player2_, player3_, player4_, player5_, player6_, player7_, player8_);
	}

	int get_update_str(char * buf, ChatRoomTable & old)
	{
		return sprintf(buf, "update ChatRoomTable set player1=%d,player2=%d,player3=%d,"
			"player4=%d,player5=%d,player6=%d,player7=%d,player8=%d where roomid=%d ",
			player1_, player2_, player3_, player4_, player5_, player6_, player7_, player8_, old.roomid_);
	}

	int get_delete_str(char * buf)
	{ 
		return sprintf(buf, "delete from ChatRoomTable where roomid=%d ", roomid_);
	}

	static const char * get_select_str()
	{ return "select * from ChatRoomTable";}

	template < class otl_stream >
		friend otl_stream& operator>>(otl_stream& os, ChatRoomTable& p)
	{
		os >> p.roomid_ >> p.player1_ >> p.player2_ >> p.player3_ >> p.player4_ 
			>> p.player5_ >> p.player6_ >> p.player7_ >> p.player8_;
		return os;
	}

	ChatRoomTable& operator+=(const ChatRoomTable& p)
	{
		player1_ += p.player1_;
		player2_ += p.player2_;
		player3_ += p.player3_;
		player4_ += p.player4_;
		player5_ += p.player5_;
		player6_ += p.player6_;
		player7_ += p.player7_;
		player8_ += p.player8_;
		return *this;
	}
	ChatRoomTable()
	{
		memset((char *)this, 0, sizeof(*this));
	}
};

struct PlayerChatRoomTable
{
	int playerid_; // playerid_ = id 一条
	int roomid_;
	int consortiaid_;
	int science_;
	int money_;
	int contribute_;
	int country_;//2008/06/17 by zeng
	int maxpacknum_;

	static int gettype() 
	{ return CMD_PLAYER_CHAT_ROOM_TABLE;}

	static const char * getname() 
	{ return "PlayerChatRoomTable";}

	inline void setid(int playerid) 
	{playerid_ = playerid;}

	inline void getid(char * buf) 
	{ sprintf(buf, "%d", playerid_);}

	inline void setkeyvalue(int key) {}

	inline int getkeyvalue() { return 0; }

	int get_insert_str(char * buf)
	{ 
		//return sprintf(buf, "insert into PlayerChatRoomTable(playerid,roomid,consortiaid,science,money,contribute,country,maxpacknum)"
		//	"values(%d,%d,%d,%d,%d,%d,%d,%d) ",
		return sprintf(buf, "insert into PlayerChatRoomTable values(%d,%d,%d,%d,%d,%d,%d,%d) ",
			playerid_, roomid_, consortiaid_, science_, money_, contribute_, country_, maxpacknum_);
	}

	int get_update_str(char * buf, PlayerChatRoomTable & old)
	{
		return sprintf(buf, "update PlayerChatRoomTable set roomid=%d,consortiaid=%d,science=%d,"
			"money=%d,contribute=%d,country=%d,maxpacknum=%d where playerid=%d ",
			roomid_, consortiaid_, science_, money_, contribute_, country_, maxpacknum_, old.playerid_);
	}

	int get_delete_str(char * buf)
	{ 
		return sprintf(buf, "delete from PlayerChatRoomTable where playerid=%d ", playerid_);
	}

	static const char * get_select_str()
	{ return "select * from PlayerChatRoomTable";}

	template < class otl_stream >
		friend otl_stream& operator>>(otl_stream& os, PlayerChatRoomTable& p)
	{
		os >> p.playerid_ >> p.roomid_ >> p.consortiaid_ >> p.science_ 
			>> p.money_ >> p.contribute_ >> p.country_ >> p.maxpacknum_;
		return os;
	}

	PlayerChatRoomTable& operator+=(const PlayerChatRoomTable& p)
	{
		roomid_ += p.roomid_;
		consortiaid_ += p.consortiaid_;
		science_ += p.science_;
		money_ += p.money_;
		contribute_ += p.contribute_;
		country_ += p.country_;
		maxpacknum_ += p.maxpacknum_;
		return *this;
	}
	PlayerChatRoomTable()
	{
		memset((char *)this, 0, sizeof(*this));
	}
};

struct PlayerCity
{
	int playerid_; // playerid_ = id 多条
	int cityid_;
	int mapid_; // playerid_ + mapid_ = 一条
	short state_;

	static int gettype() 
	{ return CMD_PLAYER_CITY;}

	static const char * getname() 
	{ return "PlayerCity";}

	inline void setmasterid(int playerid) 
	{playerid_ = playerid;}

	inline void getmasterid(char * buf) 
	{ sprintf(buf, "%d", playerid_);}

	inline void setdetailid(int mapid) 
	{mapid_ = mapid;}

	inline void getdetailid(char * buf) 
	{ sprintf(buf, "%d", mapid_);}

	inline void setkeyvalue(int key) {}

	inline int getkeyvalue() { return 0; }

	int get_insert_str(char * buf)
	{ 
		//return sprintf(buf, "insert into PlayerCity(playerid,cityid,mapid,state) values(%d,%d,%d,%d) ",
		return sprintf(buf, "insert into PlayerCity values(%d,%d,%d,%d) ",
			playerid_, cityid_, mapid_, state_);
	}

	int get_update_str(char * buf, PlayerCity & old)
	{
		return sprintf(buf, "update PlayerCity set cityid=%d,state=%d where playerid=%d and mapid=%d ",
			cityid_, state_, old.playerid_, old.mapid_);
	}

	int get_delete_str(char * buf)
	{ 
		return sprintf(buf, "delete from PlayerCity where playerid=%d and mapid=%d ", playerid_, mapid_);
	}

	static const char * get_select_str()
	{ return "select * from PlayerCity";}

	template < class otl_stream >
		friend otl_stream& operator>>(otl_stream& os, PlayerCity& p)
	{
		os >> p.playerid_ >> p.cityid_ >> p.mapid_ >> p.state_;
		return os;
	}

	PlayerCity& operator+=(const PlayerCity& p)
	{
		cityid_ += p.cityid_;
		state_ += p.state_;
		return *this;
	}
	PlayerCity()
	{
		memset((char *)this, 0, sizeof(*this));
	}
};

struct OfficerRaceAttribute
{
	int raceid_; // raceid_ = id 一条
	int keepfee_;
	int money_;
	int food_;
	int target_;
	int evade_;
	int hp_;
	int soldier_;
	int huntsman_;
	int prowl_;
	int churchman_;
	int rabbi_;
	int magician_;
	int marksman_;
	int trooper_;

	static int gettype() 
	{ return CMD_OFFICER_RACE_ATTRIBUTE;}

	static const char * getname() 
	{ return "OfficerRaceAttribute";}

	inline void setid(int raceid) 
	{raceid_ = raceid;}

	inline void getid(char * buf) 
	{ sprintf(buf, "%d", raceid_);}

	inline void setkeyvalue(int key) {}

	inline int getkeyvalue() { return 0; }

	int get_insert_str(char * buf)
	{ 
		//return sprintf(buf, "insert into OfficerRaceAttribute(raceid,keepfee,money,food,target,evade,hp,soldier,"
		//	"huntsman,prowl,churchman,rabbi,magician,marksman,trooper) values(%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d) ",
		return sprintf(buf, "insert into OfficerRaceAttribute values(%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d) ",
			raceid_, keepfee_, money_, food_, target_, evade_, hp_, soldier_, 
			huntsman_, prowl_, churchman_,rabbi_, magician_, marksman_, trooper_);
	}

	int get_update_str(char * buf, OfficerRaceAttribute & old)
	{
		return sprintf(buf, "update OfficerRaceAttribute set keepfee=%d,money=%d,food=%d,target=%d,evade=%d,hp=%d,soldier=%d,"
			"huntsman=%d,prowl=%d,churchman=%d,rabbi=%d,magician=%d,marksman=%d,trooper=%d where raceid=%d ",
			keepfee_, money_, food_, target_, evade_, hp_, soldier_, huntsman_, prowl_, 
			churchman_,rabbi_, magician_, marksman_, trooper_, old.raceid_);
	}

	int get_delete_str(char * buf)
	{ 
		return sprintf(buf, "delete from OfficerRaceAttribute where raceid=%d ", raceid_);
	}

	static const char * get_select_str()
	{ return "select * from OfficerRaceAttribute";}

	template < class otl_stream >
		friend otl_stream& operator>>(otl_stream& os, OfficerRaceAttribute& p)
	{
		os >> p.raceid_ >> p.keepfee_ >> p.money_ >> p.food_ >> p.target_ 
			>> p.evade_ >> p.hp_ >> p.soldier_ >> p.huntsman_ >> p.prowl_ 
			>> p.churchman_ >> p.rabbi_ >> p.magician_ >> p.marksman_ >> p.trooper_;
		return os;
	}

	OfficerRaceAttribute& operator+=(const OfficerRaceAttribute& p)
	{
		keepfee_ += p.keepfee_;
		money_ += p.money_;
		food_ += p.food_;
		target_ += p.target_;
		evade_ += p.evade_;
		hp_ += p.hp_;
		soldier_ += p.soldier_;
		huntsman_ += p.huntsman_;
		prowl_ += p.prowl_;
		churchman_ += p.churchman_;
		rabbi_ += p.rabbi_;
		magician_ += p.magician_;
		marksman_ += p.marksman_;
		trooper_ += p.trooper_;
		return *this;
	}
	OfficerRaceAttribute()
	{
		memset((char *)this, 0, sizeof(*this));
	}
};

struct OfficerJobAttribute
{
	int jobid_; // jobid_ = ID 一条
	int target_;
	int evage_;
	int moverate_;
	int hurt_;
	int shot_;

	static int gettype() 
	{ return CMD_OFFICER_JOB_ATTRIBUTE;}

	static const char * getname() 
	{ return "OfficerJobAttribute";}

	inline void setid(int jobid) 
	{jobid_ = jobid;}

	inline void getid(char * buf) 
	{sprintf(buf, "%d", jobid_);}

	inline void setkeyvalue(int key) {}

	inline int getkeyvalue() { return 0; }

	int get_insert_str(char * buf)
	{ 
		return sprintf(buf, "insert into OfficerJobAttribute values(%d,%d,%d,%d,%d,%d) ",
			jobid_, target_, evage_, moverate_, hurt_, shot_);
	}

	int get_update_str(char * buf, OfficerJobAttribute & old)
	{
		return sprintf(buf, "update OfficerJobAttribute set target=%d,evage=%d,moverate=%d,hurt=%d,shot=%d where jobid=%d ",
			target_, evage_, moverate_, hurt_, shot_, old.jobid_);
	}

	int get_delete_str(char * buf)
	{ 
		return sprintf(buf, "delete from OfficerJobAttribute where jobid=%d ", jobid_);
	}

	static const char * get_select_str()
	{ return "select * from OfficerJobAttribute";}

	template < class otl_stream >
		friend otl_stream& operator>>(otl_stream& os, OfficerJobAttribute& p)
	{
		os >> p.jobid_ >> p.target_ >> p.evage_ >> p.moverate_ >> p.hurt_ >> p.shot_;
		return os;
	}

	OfficerJobAttribute& operator+=(const OfficerJobAttribute& p)
	{
		target_ += p.target_;
		evage_ += p.evage_;
		moverate_ += p.moverate_;
		hurt_ += p.hurt_;
		shot_ += p.shot_;
		return *this;
	}
	OfficerJobAttribute()
	{
		memset((char *)this, 0, sizeof(*this));
	}
};

// 徐进才删除
//struct WeaponAttribute
//{
//	// int level_ 删除
//	int weaponid_;// weapontypeid_ + weaponid_ = id 一条
//	int weapontypeid_; // weapontypeid_ = id 多条
//	int cost_;
//	int attack_;
//	int hurt_;
//	int singlehands_;
//	int soldier_;
//	int exp_;
//
//	static int gettype() 
//	{ return CMD_WEAPON_ATTRIBUTE;}
//
//	static const char * getname() 
//	{ return "WeaponAttribute";}
//
//	inline void setmasterid(int weapontypeid) 
//	{weapontypeid_ = weapontypeid;}
//
//	inline void getmasterid(char * buf) 
//	{ sprintf(buf, "%d", weapontypeid_);}
//
//	inline void setdetailid(int weaponid) 
//	{
//		weaponid_ = weaponid;
//	}
//
//	inline void getdetailid(char * buf) 
//	{ sprintf(buf, "%d", weaponid_);}
//
//	inline void setkeyvalue(int key) {}
//
//	inline int getkeyvalue() { return 0; }
//
//	int get_insert_str(char * buf)
//	{ 
//		//return sprintf(buf, "insert into WeaponAttribute(weaponid,weapontypeid,cost,attack,hurt,"
//		//	"singlehands,soldier,exp) values(%d,%d,%d,%d,%d,%d,%d,%d) ",
//		return sprintf(buf, "insert into WeaponAttribute values(%d,%d,%d,%d,%d,%d,%d,%d) ",
//			weaponid_, weapontypeid_, cost_, attack_, hurt_, singlehands_, soldier_, exp_);
//	}
//
//	int get_update_str(char * buf, WeaponAttribute & old)
//	{
//		return sprintf(buf, "update WeaponAttribute set cost=%d,attack=%d,hurt=%d,"
//			"singlehands=%d,soldier=%d,exp=%d where weapontypeid=%d and weaponid=%d ",
//			cost_, attack_, hurt_, singlehands_, soldier_, exp_, old.weapontypeid_, old.weaponid_);
//	}
//
//	int get_delete_str(char * buf)
//	{ 
//		return sprintf(buf, "delete from WeaponAttribute where weapontypeid=%d and weaponid=%d ", weapontypeid_, weaponid_);
//	}
//
//	static const char * get_select_str()
//	{ return "select * from WeaponAttribute";}
//
//	template < class otl_stream >
//		friend otl_stream& operator>>(otl_stream& os, WeaponAttribute& p)
//	{
//		os >> p.weaponid_ >> p.weapontypeid_ >> p.cost_ >> p.attack_ 
//			>> p.hurt_ >> p.singlehands_ >> p.soldier_ >> p.exp_;
//		return os;
//	}
//
//	WeaponAttribute& operator+=(const WeaponAttribute& p)
//	{
//		cost_ += p.cost_;
//		attack_ += p.attack_;
//		hurt_ += p.hurt_;
//		singlehands_ += p.singlehands_;
//		soldier_ += p.soldier_;
//		exp_ += p.exp_;
//		return *this;
//	}
//	WeaponAttribute()
//	{
//		memset((char *)this, 0, sizeof(*this));
//	}
//};

// 徐进才删除
//struct DefendAttribute
//{
//	int defendid_;// defendid_ = id 一条
//	int defendtype_;
//	int cost_;
//	int decrate_;
//	int physicerec_;
//	int magicrec_;
//
//	static int gettype() 
//	{ return CMD_DEFEND_ATTRIBUTE;}
//
//	static const char * getname() 
//	{ return "DefendAttribute";}
//
//	inline void setid(int defendid) 
//	{
//		defendid_ = defendid;
//	}
//
//	inline void getid(char * buf) 
//	{ sprintf(buf, "%d", defendid_);}
//
//	inline void setkeyvalue(int key) {}
//
//	inline int getkeyvalue() { return 0; }
//
//	int get_insert_str(char * buf)
//	{ 
//		//return sprintf(buf, "insert into DefendAttribute(defendid,defendtype,cost,decrate,physicerec,magicrec) values(%d,%d,%d,%d,%d,%d) ",
//		return sprintf(buf, "insert into DefendAttribute values(%d,%d,%d,%d,%d,%d) ",
//			defendid_, defendtype_, cost_, decrate_, physicerec_, magicrec_);
//	}
//
//	int get_update_str(char * buf, DefendAttribute & old)
//	{
//		return sprintf(buf, "update DefendAttribute set defendtype=%d,cost=%d,"
//			"decrate=%d,physicerec=%d,magicrec=%d where defendid=%d ",
//			defendtype_, cost_, decrate_, physicerec_, magicrec_, old.defendid_);
//	}
//
//	int get_delete_str(char * buf)
//	{ 
//		return sprintf(buf, "delete from DefendAttribute where defendid=%d ", defendid_);
//	}
//
//	static const char * get_select_str()
//	{ return "select * from DefendAttribute";}
//
//	template < class otl_stream >
//		friend otl_stream& operator>>(otl_stream& os, DefendAttribute& p)
//	{
//		os >> p.defendid_ >> p.defendtype_ >> p.cost_ >> p.decrate_ >> p.physicerec_ >> p.magicrec_;
//		return os;
//	}
//
//	DefendAttribute& operator+=(const DefendAttribute& p)
//	{
//		defendtype_ += p.defendtype_;
//		cost_ += p.cost_;
//		decrate_ += p.decrate_;
//		physicerec_ += p.physicerec_;
//		magicrec_ += p.magicrec_;
//		return *this;
//	}
//	DefendAttribute()
//	{
//		memset((char *)this, 0, sizeof(*this));
//	}
//};

struct ArmyOutTable
{
	int playerid_; // playerid_ = ID 多条
	int armyid_; // playerid_ + armyid_ = id 一条
	int outmapid_;
	int outcityid_;
	int targetmapid_;
	int64 starttime_;
	int frommapid_;
	int tomapid_;
	int speed_;

	static int gettype() 
	{ return CMD_ARMY_OUT_TABLE;}

	static const char * getname() 
	{ return "ArmyOutTable";}

	inline void setmasterid(int playerid) 
	{playerid_ = playerid;}

	inline void getmasterid(char * buf) 
	{ sprintf(buf, "%d", playerid_);}

	inline void setdetailid(int armyid) 
	{armyid_ = armyid;}

	inline void getdetailid(char * buf) 
	{ sprintf(buf, "%d", armyid_);}

	inline void setkeyvalue(int key) {}

	inline int getkeyvalue() { return 0; }

	int get_insert_str(char * buf)
	{ 
		//return sprintf(buf, "insert into ArmyOutTable(playerid,armyid,outmapid,outcityid,targetmapid,starttime,frommapid,tomapid) "
		return sprintf(buf, "insert into ArmyOutTable values(%d,%d,%d,%d,%d,%I64d,%d,%d,%d) ",
			playerid_, armyid_, outmapid_, outcityid_, targetmapid_, starttime_, frommapid_, tomapid_, speed_);
	}

	int get_update_str(char * buf, ArmyOutTable & old)
	{
		return sprintf(buf, "update ArmyOutTable set outmapid=%d,outcityid=%d,"
			"targetmapid=%d,starttime=%I64d,frommapid=%d,tomapid=%d,speed=%d where playerid=%d and armyid=%d ",
			outmapid_, outcityid_, targetmapid_, starttime_, frommapid_, tomapid_, speed_, old.playerid_, old.armyid_);
	}

	int get_delete_str(char * buf)
	{ 
		return sprintf(buf, "delete from ArmyOutTable where playerid=%d and armyid=%d ", playerid_, armyid_);
	}

	static const char * get_select_str()
	{ return "select * from ArmyOutTable";}

	template < class otl_stream >
		friend otl_stream& operator>>(otl_stream& os, ArmyOutTable& p)
	{
		os >> p.playerid_ >> p.armyid_ >> p.outmapid_ >> p.outcityid_ 
			>> p.targetmapid_ >> p.starttime_ >> p.frommapid_ >> p.tomapid_ >> p.speed_;
		return os;
	}

	ArmyOutTable& operator+=(const ArmyOutTable& p)
	{
		outmapid_ += p.outmapid_;
		outcityid_ += p.outcityid_;
		targetmapid_ += p.targetmapid_;
		starttime_ += p.starttime_;
		frommapid_ += p.frommapid_;
		tomapid_ += p.tomapid_;
		speed_ += p.speed_;
		return *this;
	}
	ArmyOutTable()
	{
		memset((char *)this, 0, sizeof(*this));
	}
};

//徐进才删除
//struct OfficerTech
//{
//	int techid_; // techid_ = ID 多条
//	int techtype_;
//	int techkind_;
//	int career_;
//	int level_; // techid_ + level_ = ID 一条
//	int odds_;
//	int effect_;
//	int object_;
//	int stamina_;
//	int power_;
//	int celerity_;
//	int wit_;
//	int techdot_;
//
//	static int gettype() 
//	{ return CMD_OFFICER_TECH;}
//
//	static const char * getname() 
//	{ return "OfficerTech";}
//
//	inline void setmasterid(int techid) 
//	{techid_ = techid;}
//
//	inline void getmasterid(char * buf) 
//	{ sprintf(buf, "%d", techid_);}
//
//	inline void setdetailid(int level) 
//	{level_ = level;}
//
//	inline void getdetailid(char * buf) 
//	{ sprintf(buf, "%d", level_);}
//
//	inline void setkeyvalue(int key) {}
//
//	inline int getkeyvalue() { return 0; }
//
//	int get_insert_str(char * buf)
//	{ 
//		//return sprintf(buf, "insert into OfficerTech(techid,techtype,techkind,career,level,odds,effect,object,stamina,"
//		//	"power,celerity,wit,techdot) values(%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d) ",
//		return sprintf(buf, "insert into OfficerTech values(%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d) ",
//			techid_, techtype_, techkind_, career_, level_, odds_, 
//			effect_, object_, stamina_, power_, celerity_, wit_, techdot_);
//	}
//
//	int get_update_str(char * buf, OfficerTech & old)
//	{
//		return sprintf(buf, "update OfficerTech set techtype=%d,techkind=%d,career=%d,odds=%d,"
//			"effect=%d,object=%d,stamina=%d,power=%d,celerity=%d,wit=%d,techdot=%d where techid=%d and level=%d ",
//			techtype_, techkind_, career_, odds_, effect_, object_, 
//			stamina_, power_, celerity_, wit_, techdot_, old.techid_, old.level_);
//	}
//
//	int get_delete_str(char * buf)
//	{ 
//		return sprintf(buf, "delete from OfficerTech where techid=%d and level=%d ", techid_, level_);
//	}
//
//	static const char * get_select_str()
//	{ return "select * from OfficerTech";}
//
//	template < class otl_stream >
//		friend otl_stream& operator>>(otl_stream& os, OfficerTech& p)
//	{
//		os >> p.techid_ >> p.techtype_ >> p.techkind_ >> p.career_ >> p.level_ >> p.odds_ >> p.effect_ >> p.object_
//			>> p.stamina_ >> p.power_ >> p.celerity_ >> p.wit_ >> p.techdot_;
//		return os;
//	}
//
//	OfficerTech& operator+=(const OfficerTech& p)
//	{
//		techtype_ += p.techtype_;
//		techkind_ += p.techkind_;
//		career_ += p.career_;
//		odds_ += p.odds_;
//		effect_ += p.effect_;
//		object_ += p.object_;
//		stamina_ += p.stamina_;
//		power_ += p.power_;
//		celerity_ += p.celerity_;
//		wit_ += p.wit_;
//		techdot_ += p.techdot_;
//		return *this;
//	}
//	OfficerTech()
//	{
//		memset((char *)this, 0, sizeof(*this));
//	}
//};

struct NPC
{
	int npcid_; // npcid_ = ID 一条
	char name_[SIZE_20];
	int level_;
	int attack_;
	int attackkind_;
	int weapontype_;
	int defend_;
	int attackrange_;
	int magdefend_;
	int phsdefend_;
	int hp_;
	int movespeed_;
	int sight_;
	int dropitem_;
	int dropresource_;
	int exp_;
	int skill_;

	static int gettype() 
	{ return CMD_NPC;}

	static const char * getname() 
	{ return "NPC";}

	inline void setid(int npcid) 
	{npcid_ = npcid;}

	inline void getid(char * buf) 
	{ sprintf(buf, "%d", npcid_);}

	inline void setkeyvalue(int key) {}

	inline int getkeyvalue() { return 0; }

	int get_insert_str(char * buf)
	{ 
		//return sprintf(buf, "insert into NPC(npcid,name,level,attack,attackkind,weapontype,defend,attackrange,magdefend,"
		//	"phsdefend,hp,movespeed,sight,dropitem,dropresource,exp,skill) values(%d,\'\'%s\'\',%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d) ",
		return sprintf(buf, "insert into NPC values(%d,\'\'%s\'\',%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d) ",
			npcid_, name_, level_, attack_, attackkind_, weapontype_, defend_, attackrange_, magdefend_, 
			phsdefend_, hp_, movespeed_, sight_, dropitem_, dropresource_, exp_, skill_);
	}

	int get_update_str(char * buf, NPC & old)
	{
		return sprintf(buf, "update NPC set name=\'\'%s\'\',level=%d,attack=%d,attackkind=%d,weapontype=%d,"
			"defend=%d,attackrange=%d,magdefend=%d,phsdefend=%d,hp=%d,movespeed=%d,sight=%d,"
			"dropitem=%d,dropresource=%d,exp=%d,skill=%d where npcid=%d ",
			name_, level_, attack_, attackkind_, weapontype_, defend_, attackrange_, magdefend_, 
			phsdefend_, hp_, movespeed_, sight_, dropitem_, dropresource_, exp_, skill_, old.npcid_);
	}

	int get_delete_str(char * buf)
	{ 
		return sprintf(buf, "delete from NPC where npcid=%d ", npcid_);
	}

	static const char * get_select_str()
	{ return "select * from NPC";}

	template < class otl_stream >
		friend otl_stream& operator>>(otl_stream& os, NPC& p)
	{
		os >> p.npcid_ >> p.name_ >> p.level_ >> p.attack_ >> p.attackkind_ >> p.weapontype_ >> p.defend_
			>> p.attackrange_ >> p.magdefend_ >> p.phsdefend_ >> p.hp_ 
			>> p.movespeed_ >> p.sight_ >> p.dropitem_ >> p.dropresource_ >> p.exp_ >> p.skill_;
		return os;
	}

	NPC& operator+=(const NPC& p)
	{
		level_ += p.level_;
		attack_ += p.attack_;
		attackkind_ += p.attackkind_;
		weapontype_ += p.weapontype_;
		defend_ += p.defend_;
		attackrange_ += p.attackrange_;
		magdefend_ += p.magdefend_;
		phsdefend_ += p.phsdefend_;
		hp_ += p.hp_;
		movespeed_ += p.movespeed_;
		sight_ += p.sight_;
		dropitem_ += p.dropitem_;
		dropresource_ += p.dropresource_;
		exp_ += p.exp_;
		skill_ += p.skill_;

		return *this;
	}
	NPC()
	{
		memset((char *)this, 0, sizeof(*this));
	}
};

struct MapInfoTable // add by zengqinghui 2008/06/23
{
	int countryid_;
	int mapid_; //mapid_ = ID 一条
	int state_;

	static int gettype() 
	{ return CMD_MAP_INFO_TABLE;}

	static const char * getname() 
	{ return "MapInfoTable";}

	inline void setid(int mapid) 
	{mapid_ = mapid;}

	inline void getid(char * buf) 
	{ sprintf(buf, "%d", mapid_);}

	inline void setkeyvalue(int key) {}

	inline int getkeyvalue() { return 0; }

	int get_insert_str(char * buf)
	{ 
		//return sprintf(buf, "insert into MapInfoTable(countryid,mapid,state) values(%d,%d,%d) ",
		return sprintf(buf, "insert into MapInfoTable values(%d,%d,%d) ",
			countryid_, mapid_, state_);
	}

	int get_update_str(char * buf, MapInfoTable & old)
	{
		return sprintf(buf, "update MapInfoTable set countryid=%d,state=%d where mapid=%d ",
			countryid_, state_, old.mapid_);
	}

	int get_delete_str(char * buf)
	{ 
		return sprintf(buf, "delete from MapInfoTable where mapid=%d ", mapid_);
	}

	static const char * get_select_str()
	{ return "select * from MapInfoTable";}

	template < class otl_stream >
		friend otl_stream& operator>>(otl_stream& os, MapInfoTable& p)
	{
		os >> p.countryid_ >> p.mapid_ >> p.state_;
		return os;
	}

	MapInfoTable& operator+=(const MapInfoTable& p)
	{
		countryid_ += p.countryid_;
		state_ += p.state_;
		return *this;
	}
	MapInfoTable()
	{
		memset((char *)this, 0, sizeof(*this));
	}
};

struct ResearchItem// add by zengqinghui 2008/06/23
{
	int playerid_; // playerid_ + type_  = ID 多条
	int type_;// playerid_ + type_ +Itemid_ = ID 一条
	int itemid_;
	int level_;	
	int count_;

	static int gettype() 
	{ return CMD_RESEARCH_ITEM;}

	static const char * getname() 
	{ return "ResearchItem";}

	inline void setmasterid(int playerid, int type) 
	{playerid_ = playerid;type_=type;}

	inline void getmasterid(char * buf) 
	{ sprintf(buf, "%d-%d", playerid_, type_);}

	inline void setdetailid(int itemid) 
	{itemid_ = itemid;}

	inline void getdetailid(char * buf) 
	{ sprintf(buf, "%d", itemid_);}

	inline void setkeyvalue(int key) {}

	inline int getkeyvalue() { return 0; }

	int get_insert_str(char * buf)
	{ 
		//return sprintf(buf, "insert into ResearchItem(playerid,type,itemid,level,count) values(%d,%d,%d,%d,%d) ",
		return sprintf(buf, "insert into ResearchItem values(%d,%d,%d,%d,%d) ",
			playerid_, type_, itemid_, level_, count_);
	}

	int get_update_str(char * buf, ResearchItem & old)
	{
		return sprintf(buf, "update ResearchItem set level=%d,count=%d"
		" where playerid=%d and type=%d and itemid=%d ",
			level_, count_, old.playerid_, old.type_, old.itemid_);
	}

	int get_delete_str(char * buf)
	{ 
		return sprintf(buf, "delete from ResearchItem where playerid=%d and type=%d and itemid=%d ", 
			playerid_, type_, itemid_);
	}

	static const char * get_select_str()
	{ return "select * from ResearchItem";}

	template < class otl_stream >
		friend otl_stream& operator>>(otl_stream& os, ResearchItem& p)
	{
		os >> p.playerid_ >> p.type_ >> p.itemid_ >> p.level_ >> p.count_;
		return os;
	}

	ResearchItem& operator+=(const ResearchItem& p)
	{
		level_ += p.level_;	
		count_ += p.count_;
		return *this;
	}
	ResearchItem()
	{
		memset((char *)this, 0, sizeof(*this));
	}
};

struct EctypeBaseTable
{
	int ectypeid_; // ectypeid_ = id 一条
	int level_;
	int mapid_;
	char name_[SIZE_16];
	int doorid_;

	static int gettype() 
	{ return CMD_ECTYPE_BASE_TABLE;}

	static const char * getname() 
	{ return "EctypeBaseTable";}

	inline void setid(int ectypeid) 
	{ectypeid_ = ectypeid;}

	inline void getid(char * buf) 
	{ sprintf(buf, "%d", ectypeid_);}

	inline void setkeyvalue(int key) {}

	inline int getkeyvalue() { return 0; }

	int get_insert_str(char * buf)
	{ 
		//return sprintf(buf, "insert into EctypeBaseTable(ectypeid,level,mapid,name,doorid) values(%d,%d,%d,\'\'%s\'\',%d) ",
		return sprintf(buf, "insert into EctypeBaseTable values(%d,%d,%d,\'\'%s\'\',%d) ",
			ectypeid_, level_, mapid_, name_, doorid_);
	}

	int get_update_str(char * buf, EctypeBaseTable & old)
	{
		return sprintf(buf, "update EctypeBaseTable set level=%d,mapid=%d,name=\'\'%s\'\',doorid=%d where ectypeid=%d ",
			level_, mapid_, name_, doorid_, old.ectypeid_);
	}

	int get_delete_str(char * buf)
	{ 
		return sprintf(buf, "delete from EctypeBaseTable where ectypeid=%d ", ectypeid_);
	}

	static const char * get_select_str()
	{ return "select * from EctypeBaseTable";}

	template < class otl_stream >
		friend otl_stream& operator>>(otl_stream& os, EctypeBaseTable& p)
	{
		os >> p.ectypeid_ >> p.level_ >> p.mapid_ >> p.name_ >> p.doorid_;
		return os;
	}

	EctypeBaseTable& operator+=(const EctypeBaseTable& p)
	{
		level_ += p.level_;
		mapid_ += p.mapid_;
		doorid_ += p.doorid_;
		return *this;
	}
	EctypeBaseTable()
	{
		memset((char *)this, 0, sizeof(*this));
	}
};


struct NPCArmyTable
{
	int armyid_; // mapid_ = ID 多条
	char armyname_[SIZE_32];//2008/06/17 by zeng
	int officerid_;
	int playerid_;
	int cityid_;
	int mapid_;// mapid_ + armyid_ = id 一条
	int soldiertype_;
	int raceid_;
	int firstcount_;
	int fsoldierlevel_;
	int fweapon_;
	int fsexp_;
	int fweaponlevel_;
	int fdefendtype_;
	int fdefendlevel_;
	int state_;
	int x;
	int y;
	int direction;
	int dropitem_;
	int dropodds_;

	static int gettype() 
	{ return CMD_NPC_ARMY_TABLE;}

	static const char * getname() 
	{ return "NPCArmyTable";}

	inline void setmasterid(int mapid) 
	{mapid_ = mapid;}

	inline void getmasterid(char * buf) 
	{ sprintf(buf, "%d", mapid_);}

	inline void setdetailid(int armyid) 
	{armyid_ = armyid;}

	inline void getdetailid(char * buf) 
	{ sprintf(buf, "%d", armyid_);}

	inline void setkeyvalue(int key)
	{ armyid_ = key; }

	inline int getkeyvalue() { return armyid_; }

	int get_insert_str(char * buf)
	{ 
		//return sprintf(buf, "insert into NPCArmyTable(armyid,armyname,officerid,playerid,cityid,mapid,soldiertype,raceid,"
		//	"firstcount,fsoldierlevel,fweapon,fsexp,fweaponlevel,fdefendtype,fdefendlevel,state,x,y,direction,dropitem,dropodds) values(%d,\'\'%s\'\',"
		return sprintf(buf, "insert into NPCArmyTable values(%d,\'\'%s\'\',"
			"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d) ",
			armyid_, armyname_, officerid_, playerid_, cityid_, mapid_, soldiertype_, raceid_, 
			firstcount_, fsoldierlevel_, fweapon_, fsexp_, fweaponlevel_, fdefendtype_,
			fdefendlevel_, state_, x, y, direction, dropitem_, dropodds_);
	}

	int get_update_str(char * buf, NPCArmyTable & old)
	{
		return sprintf(buf, "update NPCArmyTable set armyname=\'\'%s\'\',officerid=%d,playerid=%d,"
			"cityid=%d,soldiertype=%d,raceid=%d,firstcount=%d,fsoldierlevel=%d,fweapon=%d,fsexp=%d,"
			"fweaponlevel=%d,fdefendtype=%d,fdefendlevel=%d,state=%d,x=%d,y=%d,direction=%d,"
			"dropitem=%d,dropodds=%d where mapid=%d and armyid=%d ",
			armyname_, officerid_, playerid_, cityid_, soldiertype_, raceid_, 
			firstcount_, fsoldierlevel_, fweapon_, fsexp_, fweaponlevel_, fdefendtype_,
			fdefendlevel_, state_, x, y, direction, dropitem_, dropodds_, old.mapid_, old.armyid_);
	}

	int get_delete_str(char * buf)
	{ 
		return sprintf(buf, "delete from NPCArmyTable where mapid=%d and armyid=%d ", mapid_, armyid_);
	}

	static const char * get_select_str()
	{ return "select * from NPCArmyTable";}

	template < class otl_stream >
		friend otl_stream& operator>>(otl_stream& os, NPCArmyTable& p)
	{
		os  >> p.armyid_ >> p.armyname_ >> p.officerid_ >> p.playerid_ 
			>> p.cityid_ >> p.mapid_ >> p.soldiertype_ >> p.raceid_ 
			>> p.firstcount_ >> p.fsoldierlevel_ >> p.fweapon_ >> p.fsexp_ 
			>> p.fweaponlevel_ >> p.fdefendtype_ >> p.fdefendlevel_ 
			>> p.state_ >> p.x >> p.y >> p.direction >> p.dropitem_ >> p.dropodds_;
		return os;
	}

	NPCArmyTable& operator+=(const NPCArmyTable& p)
	{
		officerid_ += p.officerid_;
		playerid_ += p.playerid_;
		cityid_ += p.cityid_;
		soldiertype_ += p.soldiertype_;
		raceid_ += p.raceid_;
		firstcount_ += p.firstcount_;
		fsoldierlevel_ += p.fsoldierlevel_;
		fweapon_ += p.fweapon_;
		fsexp_ += p.fsexp_;
		fweaponlevel_ += p.fweaponlevel_;
		fdefendtype_ += p.fdefendtype_;
		fdefendlevel_ += p.fdefendlevel_;
		state_ += p.state_;
		x += p.x;
		y += p.y;
		direction += p.direction;
		dropitem_ += p.dropitem_;
		dropodds_ += p.dropodds_;
		return *this;
	}

	NPCArmyTable()
	{
		memset((char *)this, 0, sizeof(*this));
	}
};

struct PlayerCaptiveHero
{
	int playerid_; // playerid_ = 多条
	int officerid_; // playerid_ + officerid_ + byplayerid_ + byofficerid_ = 一条
	int byplayerid_;
	int byofficerid_;
	int64 captivetimes_;

	static int gettype() 
	{ return CMD_PLAYER_CAPTIVE_HERO;}

	static const char * getname() 
	{ return "PlayerCaptiveHero";}

	inline void setmasterid(int playerid) 
	{playerid_=playerid;}

	inline void getmasterid(char * buf) 
	{ sprintf(buf, "%d", playerid_);}

	inline void setdetailid(int officerid, int byplayerid, int byofficerid) 
	{officerid_=officerid;byplayerid_=byplayerid;byofficerid_=byofficerid;}

	inline void getdetailid(char * buf) 
	{ sprintf(buf, "%d-%d-%d", officerid_, byplayerid_, byofficerid_);}

	inline void setkeyvalue(int key) {}

	inline int getkeyvalue() { return 0; }

	int get_insert_str(char * buf)
	{ 
		//return sprintf(buf, "insert into PlayerCaptiveHero(playerid,officerid,byplayerid,byofficerid,captivetime) values(%d,%d,%d,%d,%I64d) ",
		return sprintf(buf, "insert into PlayerCaptiveHero values(%d,%d,%d,%d,%I64d) ",
			playerid_, officerid_, byplayerid_, byofficerid_, captivetimes_);
	}

	int get_update_str(char * buf, PlayerCaptiveHero & old)
	{
		return sprintf(buf, "update PlayerCaptiveHero set "
			"captivetime=%I64d where playerid=%d and officerid=%d and byplayerid=%d and byofficerid=%d ",
			captivetimes_, old.playerid_, old.officerid_,old.byplayerid_, old.byofficerid_);
	}

	int get_delete_str(char * buf)
	{ 
		return sprintf(buf, "delete from PlayerCaptiveHero where playerid=%d and officerid=%d and byplayerid=%d and byofficerid=%d ", 
			playerid_, officerid_, byplayerid_, byofficerid_);
	}

	static const char * get_select_str()
	{ return "select * from PlayerCaptiveHero";}

	template < class otl_stream >
		friend otl_stream& operator>>(otl_stream& os, PlayerCaptiveHero& p)
	{
		os >> p.playerid_ >> p.officerid_ >> p.byplayerid_ >> p.byofficerid_ >> p.captivetimes_;
		return os;
	}

	PlayerCaptiveHero& operator+=(const PlayerCaptiveHero& p)
	{
		captivetimes_ += p.captivetimes_;
		return *this;
	}
	PlayerCaptiveHero()
	{
		memset((char *)this, 0, sizeof(*this));
	}
};

struct EquipAttribute
{
	int equipid_; // equipid_ = 一条
	int level_;
	int pos_;
	int endurance_;
	int str_;
	int agi_;
	int int_;
	int attack_;
	int attackrat_;
	int defend_;
	int basicprice_;

	static int gettype() 
	{ return CMD_EQUIP_ATTRIBUTE;}

	static const char * getname() 
	{ return "EquipAttribute";}

	inline void setid(int equipid) 
	{equipid_ = equipid;}

	inline void getid(char * buf) 
	{ sprintf(buf, "%d", equipid_);}

	inline void setkeyvalue(int key) {}

	inline int getkeyvalue() { return 0; }

	int get_insert_str(char * buf)
	{ 
		//return sprintf(buf, "insert into EquipAttribute(equipid,level,pos,endurance,str,agi,int,attack,attackrat,defend,basicprice)"
		//	"values(%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d) ",
		return sprintf(buf, "insert into EquipAttribute values(%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d) ",
			equipid_, level_, pos_, endurance_, str_, agi_, int_, attack_, attackrat_, defend_, basicprice_);
	}

	int get_update_str(char * buf, EquipAttribute & old)
	{
		return sprintf(buf, "update EquipAttribute set level=%d,pos=%d,endurance=%d,str=%d,agi=%d,"
			"int=%d,attack=%d,attackrat=%d,defend=%d,basicprice=%d where equipid=%d ",
			level_, pos_, endurance_, str_, agi_, int_, attack_, attackrat_, defend_, basicprice_, old.equipid_);
	}

	int get_delete_str(char * buf)
	{ 
		return sprintf(buf, "delete from EquipAttribute where equipid=%d ", equipid_);
	}

	static const char * get_select_str()
	{ return "select * from EquipAttribute";}

	template < class otl_stream >
		friend otl_stream& operator>>(otl_stream& os, EquipAttribute& p)
	{
		os >> p.equipid_ >> p.level_ >> p.pos_ >> p.endurance_ >> p.str_ >> p.agi_ 
			>> p.int_ >> p.attack_ >> p.attackrat_ >> p.defend_ >> p.basicprice_;
		return os;
	}

	EquipAttribute& operator+=(const EquipAttribute& p)
	{
		level_ += p.level_;
		pos_ += p.pos_;
		endurance_ += p.endurance_;
		str_ += p.str_;
		agi_ += p.agi_;
		int_ += p.int_;
		attack_ += p.attack_;
		attackrat_ += p.attackrat_;
		defend_ += p.defend_;
		basicprice_ += p.basicprice_;
		return *this;
	}
	EquipAttribute()
	{
		memset((char *)this, 0, sizeof(*this));
	}
};

struct WarNote
{
	int sortid_; // playerid_ + officerid_ + sortid_ = 一条
	int playerid_;
	int officerid_; // playerid_ + officerid_ = 多条
	int64 enterdate_;
	int mapid_;
	char armyname_[SIZE_32];
	int enemycount_;
	int generalcount_;
	int64 exitdate_;
	int state_;
	int64 freedate_;

	static int gettype() 
	{ return CMD_WAR_NOTE;}

	static const char * getname() 
	{ return "WarNote";}

	inline void setmasterid(int playerid, int officerid) 
	{playerid_ = playerid;officerid_=officerid;}

	inline void getmasterid(char * buf) 
	{ sprintf(buf, "%d-%d", playerid_, officerid_);}

	inline void setdetailid(int sortid) 
	{sortid_ = sortid;}

	inline void getdetailid(char * buf) 
	{ sprintf(buf, "%d", sortid_);}

	inline void setkeyvalue(int key){ sortid_ = key; }

	inline int getkeyvalue() { return sortid_; }

	int get_insert_str(char * buf)
	{ 
		//return sprintf(buf, "insert into WarNote(sortid,playerid,officerid,enterdate,mapid,armyname,"
		//	"enemycount,generalcount,exitdate,state,freedate) values(%d,%d,%d,%I64d,%d,\'\'%s\'\',%d,%d,%I64d,%d,%I64d) ",
		return sprintf(buf, "insert into WarNote values(%d,%d,%d,%I64d,%d,\'\'%s\'\',%d,%d,%I64d,%d,%I64d) ",
			sortid_, playerid_, officerid_, enterdate_, mapid_, armyname_, 
			enemycount_, generalcount_, exitdate_, state_, freedate_);
	}

	int get_update_str(char * buf, WarNote & old)
	{
		return sprintf(buf, "update WarNote set enterdate=%I64d,mapid=%d,armyname=\'\'%s\'\',"
			"enemycount=%d,generalcount=%d,exitdate=%I64d,state=%d,freedate=%I64d"
			" where playerid=%d and officerid=%d and sortid=%d ",
			enterdate_, mapid_, armyname_, enemycount_, generalcount_, 
			exitdate_, state_, freedate_, old.playerid_, old.officerid_, old.sortid_);
	}

	int get_delete_str(char * buf)
	{ 
		return sprintf(buf, "delete from WarNote where playerid=%d and officerid=%d and sortid=%d ", 
			playerid_, officerid_, sortid_);
	}

	static const char * get_select_str()
	{ return "select * from WarNote";}

	template < class otl_stream >
		friend otl_stream& operator>>(otl_stream& os, WarNote& p)
	{
		os >> p.sortid_ >> p.playerid_ >> p.officerid_ >> p.enterdate_ 
			>> p.mapid_ >> p.armyname_ >> p.enemycount_ >> p.generalcount_ 
			>> p.exitdate_ >> p.state_ >> p.freedate_;
		return os;
	}

	WarNote& operator+=(const WarNote& p)
	{
		enterdate_ += p.enterdate_;
		mapid_ += p.mapid_;
		enemycount_ += p.enemycount_;
		generalcount_ += p.generalcount_;
		exitdate_ += p.exitdate_;
		state_ += p.state_;
		freedate_ += p.freedate_;
		return *this;
	}
	WarNote()
	{
		memset((char *)this, 0, sizeof(*this));
	}
};

struct PullulateNote
{
	int sortid_; // playerid_ + officerid_ + sortid_ = 一条
	int playerid_; // playerid_ + officerid_ = 多条
	int officerid_;
	int type_;
	int level_;
	int64 upgradedate_;

	static int gettype() 
	{ return CMD_PULLULATE_NOTE;}

	static const char * getname() 
	{ return "PullulateNote";}

	inline void setmasterid(int playerid, int officerid) 
	{playerid_ = playerid;officerid_=officerid;}

	inline void getmasterid(char * buf) 
	{ sprintf(buf, "%d-%d", playerid_, officerid_);}

	inline void setdetailid(int sortid) 
	{sortid_ = sortid;}

	inline void getdetailid(char * buf) 
	{ sprintf(buf, "%d", sortid_);}

	inline void setkeyvalue(int key){ sortid_ = key; }

	inline int getkeyvalue() { return sortid_; }

	int get_insert_str(char * buf)
	{
		//return sprintf(buf, "insert into PullulateNote(sortid,playerid,officerid,type,level,upgradedate) values(%d,%d,%d,%d,%d,%I64d) ",
		return sprintf(buf, "insert into PullulateNote values(%d,%d,%d,%d,%d,%I64d) ",
			sortid_, playerid_, officerid_, type_, level_, upgradedate_);
	}

	int get_update_str(char * buf, PullulateNote & old)
	{
		return sprintf(buf, "update PullulateNote set type=%d,level=%d,upgradedate=%I64d"
			" where playerid=%d and officerid=%d and sortid=%d ",
			type_, level_, upgradedate_, old.playerid_, old.officerid_, old.sortid_);
	}

	int get_delete_str(char * buf)
	{ 
		return sprintf(buf, "delete from PullulateNote where playerid=%d and officerid=%d and sortid=%d ", 
			playerid_, officerid_, sortid_);
	}

	static const char * get_select_str()
	{ return "select * from PullulateNote";}

	template < class otl_stream >
		friend otl_stream& operator>>(otl_stream& os, PullulateNote& p)
	{
		os >> p.sortid_ >> p.playerid_ >> p.officerid_ >> p.type_ >> p.level_ >> p.upgradedate_;
		return os;
	}

	PullulateNote& operator+=(const PullulateNote& p)
	{
		type_ += p.type_;
		level_ += p.level_;
		upgradedate_ += p.upgradedate_;
		return *this;
	}
	PullulateNote()
	{
		memset((char *)this, 0, sizeof(*this));
	}
};

struct MailTable
{
	int mailid_; // receiverplayerid_ + mailid_ = 一条
	int mailtype_;
	char sendplayername_[SIZE_24];
	int receiverplayerid_; // receiverplayerid_ == 多条
	char title_[SIZE_16];
	char text_[SIZE_256];
	int64 time_;
	short flag_;
	unsigned char pertain_[SIZE_64];

	static int gettype() 
	{ return CMD_MAIL_TABLE;}

	static const char * getname() 
	{ return "MailTable";}

	inline void setmasterid(int receiverplayerid) 
	{receiverplayerid_ = receiverplayerid;}

	inline void getmasterid(char * buf) 
	{ sprintf(buf, "%d", receiverplayerid_);}

	inline void setdetailid(int mailid) 
	{mailid_ = mailid;}

	inline void getdetailid(char * buf) 
	{ sprintf(buf, "%d", mailid_);}

	inline void setkeyvalue(int key){ mailid_ = key; }

	inline int getkeyvalue() { return mailid_; }

	int get_insert_str(char * buf)
	{
		pertain_[63] = '\0';
		//return sprintf(buf, "insert into MailTable(mailid,mailtype,sendplayername,receiverplayerid,title,text,time,flag,pertain) "
		//	"values(%d,%d,\'\'%s\'\',%d,\'\'%s\'\',\'\'%s\'\',%I64d,%d,0x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x"
		return sprintf(buf, "insert into MailTable values(%d,%d,\'\'%s\'\',%d,\'\'%s\'\',\'\'%s\'\',%I64d,%d,0x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x"
			"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x"
			"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x) ",
			mailid_, mailtype_, sendplayername_, receiverplayerid_, title_, text_, time_, flag_, 
			pertain_[0],pertain_[1],pertain_[2],pertain_[3],pertain_[4],pertain_[5],pertain_[6],pertain_[7],pertain_[8],pertain_[9],
			pertain_[10],pertain_[11],pertain_[12],pertain_[13],pertain_[14],pertain_[15],pertain_[16],pertain_[17],pertain_[18],pertain_[19],
			pertain_[20],pertain_[21],pertain_[22],pertain_[23],pertain_[24],pertain_[25],pertain_[26],pertain_[27],pertain_[28],pertain_[29],
			pertain_[30],pertain_[31],pertain_[32],pertain_[33],pertain_[34],pertain_[35],pertain_[36],pertain_[37],pertain_[38],pertain_[39],
			pertain_[40],pertain_[41],pertain_[42],pertain_[43],pertain_[44],pertain_[45],pertain_[46],pertain_[47],pertain_[48],pertain_[49],
			pertain_[50],pertain_[51],pertain_[52],pertain_[53],pertain_[54],pertain_[55],pertain_[56],pertain_[57],pertain_[58],pertain_[59],
			pertain_[60],pertain_[61],pertain_[62],pertain_[63]);
	}

	int get_update_str(char * buf, MailTable & old)
	{
		pertain_[63] = '\0';
		return sprintf(buf, "update MailTable set mailtype=%d,sendplayername=\'\'%s\'\',title=\'\'%s\'\',text=\'\'%s\'\',time=%I64d,flag=%d,pertain="
			"0x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x"
			"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x "
			"where receiverplayerid=%d and mailid=%d ",
			mailtype_, sendplayername_, title_, text_, time_, flag_, 
			pertain_[0],pertain_[1],pertain_[2],pertain_[3],pertain_[4],pertain_[5],pertain_[6],pertain_[7],pertain_[8],pertain_[9],
			pertain_[10],pertain_[11],pertain_[12],pertain_[13],pertain_[14],pertain_[15],pertain_[16],pertain_[17],pertain_[18],pertain_[19],
			pertain_[20],pertain_[21],pertain_[22],pertain_[23],pertain_[24],pertain_[25],pertain_[26],pertain_[27],pertain_[28],pertain_[29],
			pertain_[30],pertain_[31],pertain_[32],pertain_[33],pertain_[34],pertain_[35],pertain_[36],pertain_[37],pertain_[38],pertain_[39],
			pertain_[40],pertain_[41],pertain_[42],pertain_[43],pertain_[44],pertain_[45],pertain_[46],pertain_[47],pertain_[48],pertain_[49],
			pertain_[50],pertain_[51],pertain_[52],pertain_[53],pertain_[54],pertain_[55],pertain_[56],pertain_[57],pertain_[58],pertain_[59],
			pertain_[60],pertain_[61],pertain_[62],pertain_[63], old.receiverplayerid_, old.mailid_);
	}

	int get_delete_str(char * buf)
	{ 
		return sprintf(buf, "delete from MailTable where receiverplayerid=%d and mailid=%d ", 
			receiverplayerid_, mailid_);
	}

	static const char * get_select_str()
	{ return "select * from MailTable";}

	template < class otl_stream >
		friend otl_stream& operator>>(otl_stream& os, MailTable& p)
	{
		otl_long_string pertain(p.pertain_, SIZE_64, SIZE_64);
		os >> p.mailid_ >> p.mailtype_ >> p.sendplayername_ >> p.receiverplayerid_ 
			>> p.title_ >> p.text_ >> p.time_ >> p.flag_ >> pertain;
		return os;
	}

	MailTable& operator+=(const MailTable& p)
	{
		mailtype_ += p.mailtype_;
		time_ += p.time_;
		flag_ += p.flag_;
		return *this;
	}
	MailTable()
	{
		memset((char *)this, 0, sizeof(*this));
	}
};

struct FirendTable
{
	int playerid_; // playerid_ + firendtype_ = 多条
	int firendplayerid_; // playerid_ + firendtype_ + firendplayerid_ = 一条
	short firendtype_;
	char firendname_[SIZE_24];
	int firendlevel_;
	int militaryrank_;

	static int gettype() 
	{ return CMD_FIREND_TABLE;}

	static const char * getname() 
	{ return "FirendTable";}

	inline void setmasterid(int playerid, int firendtype) 
	{playerid_ = playerid;firendtype_=firendtype;}

	inline void getmasterid(char * buf) 
	{ sprintf(buf, "%d-%d", playerid_,firendtype_);}

	inline void setdetailid(int firendplayerid) 
	{firendplayerid_ = firendplayerid;}

	inline void getdetailid(char * buf) 
	{ sprintf(buf, "%d", firendplayerid_);}

	inline void setkeyvalue(int key){}

	inline int getkeyvalue() { return 0; }

	int get_insert_str(char * buf)
	{
		//return sprintf(buf, "insert into FirendTable(playerid,firendplayerid,firendtype,firendname,firendlevel,militaryrank) values(%d,%d,%d,\'\'%s\'\',%d,%d) ",
		return sprintf(buf, "insert into FirendTable values(%d,%d,%d,\'\'%s\'\',%d,%d) ",
			playerid_, firendplayerid_, firendtype_, firendname_, firendlevel_, militaryrank_);
	}

	int get_update_str(char * buf, FirendTable & old)
	{
		return sprintf(buf, "update FirendTable set firendname=\'\'%s\'\',firendlevel=%d,"
			"militaryrank=%d where playerid=%d and firendtype=%d and firendplayerid=%d ",
			firendname_, firendlevel_, militaryrank_, old.playerid_, old.firendtype_, old.firendplayerid_);
	}

	int get_delete_str(char * buf)
	{ 
		return sprintf(buf, "delete from FirendTable where playerid=%d and firendtype=%d and firendplayerid=%d ", 
			playerid_, firendtype_, firendplayerid_);
	}

	static const char * get_select_str()
	{ return "select * from FirendTable";}

	template < class otl_stream >
		friend otl_stream& operator>>(otl_stream& os, FirendTable& p)
	{
		os >> p.playerid_ >> p.firendplayerid_ >> p.firendtype_ 
			>> p.firendname_ >> p.firendlevel_ >> p.militaryrank_;
		return os;
	}

	FirendTable& operator+=(const FirendTable& p)
	{
		firendlevel_ += p.firendlevel_;
		militaryrank_ += p.militaryrank_;
		return *this;
	}
	FirendTable()
	{
		memset((char *)this, 0, sizeof(*this));
	}
};

struct NewsTable
{
	int newsid_; // newsid = id 一条
	int newstype_;
	unsigned char content_[SIZE_128];

	static int gettype() 
	{ return CMD_NEWS_TABLE;}

	static const char * getname() 
	{ return "NewsTable";}

	inline void setid(int newsid) 
	{newsid_ = newsid;}

	inline void getid(char * buf) 
	{ sprintf(buf, "%d", newsid_);}

	inline void setkeyvalue(int key) {newsid_ = key;}

	inline int getkeyvalue() { return newsid_; }

	int get_insert_str(char * buf)
	{
		content_[127] = '\0';
		//return sprintf(buf, "insert into NewsTable(newsid,newtype,content) "
		return sprintf(buf, "insert into NewsTable "
			"values(%d,%d,0x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x"
			"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x"
			"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x"
			"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x"
			"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x"
			"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x"
			"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x) ",
			newsid_, newstype_,
			content_[0],content_[1],content_[2],content_[3],content_[4],content_[5],content_[6],content_[7],content_[8],content_[9],
			content_[10],content_[11],content_[12],content_[13],content_[14],content_[15],content_[16],content_[17],content_[18],content_[19],
			content_[20],content_[21],content_[22],content_[23],content_[24],content_[25],content_[26],content_[27],content_[28],content_[29],
			content_[30],content_[31],content_[32],content_[33],content_[34],content_[35],content_[36],content_[37],content_[38],content_[39],
			content_[40],content_[41],content_[42],content_[43],content_[44],content_[45],content_[46],content_[47],content_[48],content_[49],
			content_[50],content_[51],content_[52],content_[53],content_[54],content_[55],content_[56],content_[57],content_[58],content_[59],
			content_[60],content_[61],content_[62],content_[63],content_[64],content_[65],content_[66],content_[67],content_[68],content_[69],
			content_[70],content_[71],content_[72],content_[73],content_[74],content_[75],content_[76],content_[77],content_[78],content_[79],
			content_[80],content_[81],content_[82],content_[83],content_[84],content_[85],content_[86],content_[87],content_[88],content_[89],
			content_[90],content_[91],content_[92],content_[93],content_[94],content_[95],content_[96],content_[97],content_[98],content_[99],
			content_[100],content_[101],content_[102],content_[103],content_[104],content_[105],content_[106],content_[107],content_[108],content_[109],
			content_[110],content_[111],content_[112],content_[113],content_[114],content_[115],content_[116],content_[117],content_[118],content_[119],
			content_[120],content_[121],content_[122],content_[123],content_[124],content_[125],content_[126],content_[127]);
	}

	int get_update_str(char * buf, NewsTable & old)
	{
		content_[127] = '\0';
		return sprintf(buf, "update NewsTable set newtype=%d,content="
			"0x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x"
			"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x"
			"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x"
			"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x"
			"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x"
			"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x"
			"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x where newsid=%d ",
			newstype_,content_[0],content_[1],content_[2],content_[3],content_[4],content_[5],content_[6],content_[7],content_[8],content_[9],
			content_[10],content_[11],content_[12],content_[13],content_[14],content_[15],content_[16],content_[17],content_[18],content_[19],
			content_[20],content_[21],content_[22],content_[23],content_[24],content_[25],content_[26],content_[27],content_[28],content_[29],
			content_[30],content_[31],content_[32],content_[33],content_[34],content_[35],content_[36],content_[37],content_[38],content_[39],
			content_[40],content_[41],content_[42],content_[43],content_[44],content_[45],content_[46],content_[47],content_[48],content_[49],
			content_[50],content_[51],content_[52],content_[53],content_[54],content_[55],content_[56],content_[57],content_[58],content_[59],
			content_[60],content_[61],content_[62],content_[63],content_[64],content_[65],content_[66],content_[67],content_[68],content_[69],
			content_[70],content_[71],content_[72],content_[73],content_[74],content_[75],content_[76],content_[77],content_[78],content_[79],
			content_[80],content_[81],content_[82],content_[83],content_[84],content_[85],content_[86],content_[87],content_[88],content_[89],
			content_[90],content_[91],content_[92],content_[93],content_[94],content_[95],content_[96],content_[97],content_[98],content_[99],
			content_[100],content_[101],content_[102],content_[103],content_[104],content_[105],content_[106],content_[107],content_[108],content_[109],
			content_[110],content_[111],content_[112],content_[113],content_[114],content_[115],content_[116],content_[117],content_[118],content_[119],
			content_[120],content_[121],content_[122],content_[123],content_[124],content_[125],content_[126],content_[127], old.newsid_);
	}

	int get_delete_str(char * buf)
	{ 
		return sprintf(buf, "delete from NewsTable where newsid=%d ", newsid_);
	}


	static const char * get_select_str()
	{ return "select * from NewsTable";}

	template < class otl_stream >
		friend otl_stream& operator>>(otl_stream& os, NewsTable& p)
	{
		otl_long_string content(p.content_, SIZE_128, SIZE_128);
		os >> p.newsid_ >> p.newstype_ >> content;
		return os;
	}

	NewsTable& operator+=(const NewsTable& p)
	{
		newstype_ += p.newstype_;
		return *this;
	}
	NewsTable()
	{
		memset((char *)this, 0, sizeof(*this));
	}
};

//徐进才删除
//struct ConsortiaOfficial
//{
//	int officialid_; // officialid_ = id 一条
//	char officialname_[SIZE_16];
//	short level_;
//	int purview_;
//
//	static int gettype() 
//	{ return CMD_CONSORTIA_OFFICIAL;}
//
//	static const char * getname() 
//	{ return "ConsortiaOfficial";}
//
//	inline void setid(int officialid) 
//	{officialid_ = officialid;}
//
//	inline void getid(char * buf) 
//	{ sprintf(buf, "%d", officialid_);}
//
//	inline void setkeyvalue(int key) {}
//
//	inline int getkeyvalue() { return 0;}
//
//	int get_insert_str(char * buf)
//	{
//		//return sprintf(buf, "insert into ConsortiaOfficial(officialid,officialname,level,purview) values(%d,\'\'%s\'\',%d,%d) ",
//		return sprintf(buf, "insert into ConsortiaOfficial values(%d,\'\'%s\'\',%d,%d) ",
//			officialid_, officialname_, level_, purview_);
//	}
//
//	int get_update_str(char * buf, ConsortiaOfficial & old)
//	{
//		return sprintf(buf, "update ConsortiaOfficial set officialname=\'\'%s\'\',level=%d,purview=%d where officialid=%d ",
//			officialname_, level_, purview_, old.officialid_);
//	}
//
//	int get_delete_str(char * buf)
//	{ 
//		return sprintf(buf, "delete from ConsortiaOfficial where officialid=%d ", 
//			officialid_);
//	}
//
//	static const char * get_select_str()
//	{ return "select * from ConsortiaOfficial";}
//
//	template < class otl_stream >
//		friend otl_stream& operator>>(otl_stream& os, ConsortiaOfficial& p)
//	{
//		os >> p.officialid_ >> p.officialname_ >> p.level_ >> p.purview_;
//		return os;
//	}
//
//	ConsortiaOfficial& operator+=(const ConsortiaOfficial& p)
//	{
//		level_ += p.level_;
//		purview_ += p.purview_;
//		return *this;
//	}
//	ConsortiaOfficial()
//	{
//		memset((char *)this, 0, sizeof(*this));
//	}
//};

struct LoadMaterial
{
	int playerid_; // playerid_ + armyid_= id 多条
	int armyid_; //playerid_ + armyid_+ type_ + matid_  = id一条
	int type_;
	int matid_;
	int count_;

	static int gettype() 
	{ return CMD_LOAD_MATERIAL;}

	static const char * getname() 
	{ return "LoadMaterial";}

	inline void setmasterid(int playerid, int armyid) 
	{playerid_ = playerid;armyid_=armyid;}

	inline void getmasterid(char * buf) 
	{ sprintf(buf, "%d-%d", playerid_,armyid_);}

	inline void setdetailid(int type, int matid) 
	{type_ = type;matid_=matid;}

	inline void getdetailid(char * buf) 
	{ sprintf(buf, "%d-%d", type_,matid_);}

	inline void setkeyvalue(int key){}

	inline int getkeyvalue() { return 0; }

	int get_insert_str(char * buf)
	{
		//return sprintf(buf, "insert into LoadMaterial(playerid,armyid,type,matid,count) values(%d,%d,%d,%d,%d) ",
		return sprintf(buf, "insert into LoadMaterial values(%d,%d,%d,%d,%d) ",
			playerid_, armyid_, type_, matid_, count_);
	}

	int get_update_str(char * buf, LoadMaterial & old)
	{
		return sprintf(buf, "update LoadMaterial set count=%d"
			" where playerid=%d and armyid=%d and type=%d and matid=%d ",
			count_, old.playerid_, old.armyid_, old.type_, old.matid_);
	}

	int get_delete_str(char * buf)
	{ 
		return sprintf(buf, "delete from LoadMaterial where playerid=%d and armyid=%d and type=%d and matid=%d ", 
			playerid_, armyid_, type_, matid_);
	}

	static const char * get_select_str()
	{ return "select * from LoadMaterial";}

	template < class otl_stream >
		friend otl_stream& operator>>(otl_stream& os, LoadMaterial& p)
	{
		os >> p.playerid_ >> p.armyid_ >> p.type_ >> p.matid_ >> p.count_;
		return os;
	}

	LoadMaterial& operator+=(const LoadMaterial& p)
	{
		count_ += p.count_;
		return *this;
	}

	LoadMaterial()
	{
		memset((char *)this, 0, sizeof(*this));
	}
};

struct Treasury
{
	int country_;// country_  = id 一条
	int money_; 

	static int gettype() 
	{ return CMD_TREASURY;}

	static const char * getname() 
	{ return "Treasury";}

	inline void setid(int country) 
	{country_ = country;}

	inline void getid(char * buf) 
	{ sprintf(buf, "%d", country_);}

	inline void setkeyvalue(int key) {}

	inline int getkeyvalue() { return 0;}

	int get_insert_str(char * buf)
	{
		//return sprintf(buf, "insert into Treasury(country,money) values(%d,%d) ", country_, money_);
		return sprintf(buf, "insert into Treasury values(%d,%d) ", country_, money_);
	}

	int get_update_str(char * buf, Treasury & old)
	{
		return sprintf(buf, "update Treasury set money=%d where country=%d ", money_, old.country_);
	}

	int get_delete_str(char * buf)
	{ 
		return sprintf(buf, "delete from Treasury where country=%d ", country_);
	}

	static const char * get_select_str()
	{ return "select * from Treasury";}

	template < class otl_stream >
		friend otl_stream& operator>>(otl_stream& os, Treasury& p)
	{
		os >> p.country_ >> p.money_;
		return os;
	}

	Treasury& operator+=(const Treasury& p)
	{
		money_ += p.money_;
		return *this;
	}
	Treasury()
	{
		memset((char *)this, 0, sizeof(*this));
	}
};
// 徐进才删除
//struct CardConfig
//{
//	int itemid_; // itemid_  = id 一条
//	char itemname_[SIZE_32];
//	int type_;
//	int price_;
//	int value_;
//	int time_;
//	int pos_;
//
//	static int gettype() 
//	{ return CMD_CARD_CONFIG;}
//
//	static const char * getname() 
//	{ return "CardConfig";}
//
//	inline void setid(int itemid) 
//	{itemid_ = itemid;}
//
//	inline void getid(char * buf) 
//	{ sprintf(buf, "%d", itemid_);}
//
//	inline void setkeyvalue(int key) {}
//
//	inline int getkeyvalue() { return 0;}
//
//	int get_insert_str(char * buf)
//	{
//		//return sprintf(buf, "insert into CardConfig(itemid,itemidname,type,price,value,time,pos) values(%d,\'\'%s\'\',%d,%d,%d,%d,%d) ", 
//		return sprintf(buf, "insert into CardConfig values(%d,\'\'%s\'\',%d,%d,%d,%d,%d) ", 
//			itemid_, itemname_, type_, price_, value_, time_, pos_);
//	}
//
//	int get_update_str(char * buf, CardConfig & old)
//	{
//		return sprintf(buf, "update CardConfig set itemidname=\'\'%s\'\',type=%d,price=%d,value=%d,time=%d,pos=%d where itemid=%d ", 
//			itemname_, type_, price_, value_, time_, pos_, old.itemid_);
//	}
//
//	int get_delete_str(char * buf)
//	{ 
//		return sprintf(buf, "delete from CardConfig where itemid=%d ", itemid_);
//	}
//
//	static const char * get_select_str()
//	{ return "select * from CardConfig";}
//
//	template < class otl_stream >
//		friend otl_stream& operator>>(otl_stream& os, CardConfig& p)
//	{
//		os >> p.itemid_ >> p.itemname_ >> p.type_ >> p.price_ >> p.value_ >> p.time_ >> p.pos_;
//		return os;
//	}
//
//	CardConfig& operator+=(const CardConfig& p)
//	{
//		type_ += p.type_;
//		price_ += p.price_;
//		value_ += p.value_;
//		time_ += p.time_;
//		pos_ += p.pos_;
//		return *this;
//	}
//	CardConfig()
//	{
//		memset((char *)this, 0, sizeof(*this));
//	}
//};

// 柳敬武删除
struct PublicBuildParamTable
{
	int pbuildingkind_; // pbuildingkind_ + level_ = id 一条
	char name_[SIZE_32];
	int level_;
	int templateid_;
	int attack_;
	int attackkind_;
	int upgradefee_;
	int foodcount_;
	int woodcount_;
	int ironcount_;
	int skincount_;
	int attackscop_;
	int visualrange_;
	int denfenid_;
	int hp_;
	int exp_;
	int skill_;
	int skillodds_;
	int output_;
	int armyCardsType_;
	int cardsDropOdds_;

	static int gettype() 
	{ return CMD_PUBLIC_BUILD_PARAM_TABLE;}

	static const char * getname() 
	{ return "PublicBuildParamTable";}

	inline void setid(int pbuildingkind, int level) 
	{pbuildingkind_ = pbuildingkind; level_ = level;}

	inline void getid(char * buf) 
	{ sprintf(buf, "%d-%d", pbuildingkind_, level_);}

	inline void setkeyvalue(int key) {}

	inline int getkeyvalue() { return 0;}

	int get_insert_str(char * buf)
	{
		//return sprintf(buf, "insert into PublicBuildParamTable(pbuildingkind,level,upgradefee,attack,hp,attackscop,"
		//	"visualrange,output) values(%d,%d,%d,%d,%d,%d,%d,%d) ", 
		return sprintf(buf, "insert into PublicBuildParamTable values(%d,\'\'%s\'\',%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d) ", 
			pbuildingkind_, name_, level_, templateid_, attack_, attackkind_, upgradefee_, foodcount_, woodcount_, ironcount_, 
			skincount_, attackscop_, visualrange_, denfenid_, hp_, exp_, skill_, skillodds_, output_);
	}

	int get_update_str(char * buf, PublicBuildParamTable & old)
	{
		return sprintf(buf, "update PublicBuildParamTable set name=\'\'%s\'\',templateid=%d,attack=%d,attackkind=%d,"
			"upgradefee=%d,foodcount=%d,woodcount=%d,ironcount=%d,skincount=%d,attackscop=%d,visualrange=%d,denfenid=%d,"
			"hp=%d,exp=%d,skill=%d,skillodds=%d,output=%d where pbuildingkind=%d and level=%d ", 
			name_, templateid_, attack_, attackkind_, upgradefee_, foodcount_, woodcount_, ironcount_, 
			skincount_, attackscop_, visualrange_, denfenid_, hp_, exp_, skill_, skillodds_, output_, old.pbuildingkind_,old.level_);
	}

	int get_delete_str(char * buf)
	{ 
		return sprintf(buf, "delete from PublicBuildParamTable where pbuildingkind=%d and level=%d ", pbuildingkind_,level_);
	}

	static const char * get_select_str()
	{ return "select * from PublicBuildParamTable";}

	template < class otl_stream >
		friend otl_stream& operator>>(otl_stream& os, PublicBuildParamTable& p)
	{
		os >> p.pbuildingkind_ >> p.name_ >> p.level_ >> p.templateid_ >> p.attack_ >> p.attackkind_ 
			>> p.upgradefee_ >> p.foodcount_ >> p.woodcount_ >> p.ironcount_ >> p.skincount_ 
			>> p.attackscop_ >> p.visualrange_ >> p.denfenid_ >> p.hp_ >> p.exp_ >> p.skill_ >> p.skillodds_ >> p.output_;
		return os;
	}

	PublicBuildParamTable& operator+=(const PublicBuildParamTable& p)
	{
		templateid_ += p.templateid_;
		attack_ += p.attack_;
		attackkind_ += p.attackkind_;
		upgradefee_ += p.upgradefee_;
		foodcount_ += p.foodcount_;
		woodcount_ += p.woodcount_;
		ironcount_ += p.ironcount_;
		skincount_ += p.skincount_;
		attackscop_ += p.attackscop_;
		visualrange_ += p.visualrange_;
		denfenid_ += p.denfenid_;
		hp_ += p.hp_;
		exp_ += p.exp_;
		skill_ += p.skill_;
		skillodds_ += p.skillodds_;
		output_ += p.output_;
		return *this;
	}
	PublicBuildParamTable()
	{
		memset((char *)this, 0, sizeof(*this));
	}
};

struct PublicBuildTable
{
	int mapid_; // mapid_ = id 多条
	int pbuildingid_; // mapid_ + pbuildingid_ = id 一条
	int pbuildingkind_;
	int neutraltrap_;
	int currentlevel_;
	int currentmoney_;

	static int gettype() 
	{ return CMD_PUBLIC_BUILD_TABLE;}

	static const char * getname() 
	{ return "PublicBuildTable";}

	inline void setmasterid(int mapid) 
	{mapid_ = mapid;}

	inline void getmasterid(char * buf) 
	{ sprintf(buf, "%d", mapid_);}

	inline void setdetailid(int pbuildingid) 
	{pbuildingid_ = pbuildingid;}

	inline void getdetailid(char * buf) 
	{ sprintf(buf, "%d", pbuildingid_);}

	inline void setkeyvalue(int key){}

	inline int getkeyvalue() { return 0; }

	int get_insert_str(char * buf)
	{
		//return sprintf(buf, "insert into PublicBuildTable(mapid,pbuildingid,pbuildingkind,currentlevel,currentmoney) values(%d,%d,%d,%d,%d) ", 
		return sprintf(buf, "insert into PublicBuildTable values(%d,%d,%d,%d,%d,%d) ", 
			mapid_, pbuildingid_, pbuildingkind_, neutraltrap_, currentlevel_, currentmoney_);
	}

	int get_update_str(char * buf, PublicBuildTable & old)
	{
		return sprintf(buf, "update PublicBuildTable set pbuildingkind=%d,neutraltrap=%d,"
			"currentlevel=%d,currentmoney=%d where mapid=%d and pbuildingid=%d ", 
			pbuildingkind_, neutraltrap_, currentlevel_, currentmoney_, old.mapid_, old.pbuildingid_);
	}

	int get_delete_str(char * buf)
	{ 
		return sprintf(buf, "delete from PublicBuildTable where mapid=%d and pbuildingid=%d ", mapid_,pbuildingid_);
	}

	static const char * get_select_str()
	{ return "select * from PublicBuildTable";}

	template < class otl_stream >
		friend otl_stream& operator>>(otl_stream& os, PublicBuildTable& p)
	{
		os >> p.mapid_ >> p.pbuildingid_ >> p.pbuildingkind_ >> p.neutraltrap_ >> p.currentlevel_ >> p.currentmoney_;
		return os;
	}

	PublicBuildTable& operator+=(const PublicBuildTable& p)
	{
		pbuildingkind_ += p.pbuildingkind_;
		neutraltrap_ += p.neutraltrap_;
		currentlevel_ += p.currentlevel_;
		currentmoney_ += p.currentmoney_;
		return *this;
	}

	PublicBuildTable()
	{
		memset((char *)this, 0, sizeof(*this));
	}
};


struct PlayerProduceState
{
	int type_; // type_ = id 多条
	int playerid_; // type_ + playerid_ = id 一条
	int value_;

	static int gettype() 
	{ return CMD_PLAYER_PRODUCE_STATE;}

	static const char * getname() 
	{ return "PlayerProduceState";}

	inline void setmasterid(int type) 
	{type_ = type;}

	inline void getmasterid(char * buf) 
	{ sprintf(buf, "%d", type_);}

	inline void setdetailid(int playerid) 
	{playerid_ = playerid;}

	inline void getdetailid(char * buf) 
	{ sprintf(buf, "%d", playerid_);}

	inline void setkeyvalue(int key){}

	inline int getkeyvalue() { return 0; }

	int get_insert_str(char * buf)
	{
		//return sprintf(buf, "insert into PlayerProduceState(type,playerid,value) values(%d,%d,%d) ", type_, playerid_, value_);
		return sprintf(buf, "insert into PlayerProduceState values(%d,%d,%d) ", type_, playerid_, value_);
	}

	int get_update_str(char * buf, PlayerProduceState & old)
	{
		return sprintf(buf, "update PlayerProduceState set value=%d where type=%d and playerid=%d ", 
			value_, old.type_, old.playerid_);
	}

	int get_delete_str(char * buf)
	{ 
		return sprintf(buf, "delete from PlayerProduceState where type=%d and playerid=%d ", type_, playerid_);
	}

	static const char * get_select_str()
	{ return "select * from PlayerProduceState";}

	template < class otl_stream >
		friend otl_stream& operator>>(otl_stream& os, PlayerProduceState& p)
	{
		os >> p.type_ >> p.playerid_ >> p.value_;
		return os;
	}

	PlayerProduceState& operator+=(const PlayerProduceState& p)
	{
		value_ += p.value_;
		return *this;
	}

	PlayerProduceState()
	{
		memset((char *)this, 0, sizeof(*this));
	}
};

struct ChargeSvrTimer
{
	int timerid_; // serverid_ + timerid_ = id 一条
	int serverid_; // serverid_ = id 多条
	unsigned char param_[SIZE_64];
	int64 time_;

	static int gettype() 
	{ return CMD_CHARGE_SVR_TIMER;}

	static const char * getname() 
	{ return "ChargeSvrTimer";}

	inline void setmasterid(int serverid) 
	{serverid_ = serverid;}

	inline void getmasterid(char * buf) 
	{ sprintf(buf, "%d", serverid_);}

	inline void setdetailid(int timerid) 
	{timerid_ = timerid;}

	inline void getdetailid(char * buf) 
	{ sprintf(buf, "%d", timerid_);}

	inline void setkeyvalue(int key){}

	inline int getkeyvalue() { return 0; }

	int get_insert_str(char * buf)
	{
		param_[63] = '\0';
		//return sprintf(buf, "insert into ChargeSvrTimer(serverid,timerid,param,time) "
		return sprintf(buf, "insert into ChargeSvrTimer values(%d,%d,0x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x"
			"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x"
			"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x,%I64d) ",
			timerid_, serverid_,
			param_[0],param_[1],param_[2],param_[3],param_[4],param_[5],param_[6],param_[7],param_[8],param_[9],
			param_[10],param_[11],param_[12],param_[13],param_[14],param_[15],param_[16],param_[17],param_[18],param_[19],
			param_[20],param_[21],param_[22],param_[23],param_[24],param_[25],param_[26],param_[27],param_[28],param_[29],
			param_[30],param_[31],param_[32],param_[33],param_[34],param_[35],param_[36],param_[37],param_[38],param_[39],
			param_[40],param_[41],param_[42],param_[43],param_[44],param_[45],param_[46],param_[47],param_[48],param_[49],
			param_[50],param_[51],param_[52],param_[53],param_[54],param_[55],param_[56],param_[57],param_[58],param_[59],
			param_[60],param_[61],param_[62],param_[63], time_);
	}

	int get_update_str(char * buf, ChargeSvrTimer & old)
	{
		param_[63] = '\0';
		return sprintf(buf, "update ChargeSvrTimer set time=%I64d,param="
			"0x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x"
			"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x"
			"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x"
			"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x where serverid=%d and timerid=%d ",
			time_, 
			param_[0],param_[1],param_[2],param_[3],param_[4],param_[5],param_[6],param_[7],param_[8],param_[9],
			param_[10],param_[11],param_[12],param_[13],param_[14],param_[15],param_[16],param_[17],param_[18],param_[19],
			param_[20],param_[21],param_[22],param_[23],param_[24],param_[25],param_[26],param_[27],param_[28],param_[29],
			param_[30],param_[31],param_[32],param_[33],param_[34],param_[35],param_[36],param_[37],param_[38],param_[39],
			param_[40],param_[41],param_[42],param_[43],param_[44],param_[45],param_[46],param_[47],param_[48],param_[49],
			param_[50],param_[51],param_[52],param_[53],param_[54],param_[55],param_[56],param_[57],param_[58],param_[59],
			param_[60],param_[61],param_[62],param_[63], old.serverid_, old.timerid_);
	}

	int get_delete_str(char * buf)
	{ 
		return sprintf(buf, "delete from ChargeSvrTimer where serverid=%d and timerid=%d ", 
			serverid_, timerid_);
	}

	static const char * get_select_str()
	{ return "select * from ChargeSvrTimer";}

	template < class otl_stream >
		friend otl_stream& operator>>(otl_stream& os, ChargeSvrTimer& p)
	{
		otl_long_string param(p.param_, SIZE_64, SIZE_64);
		os >> p.timerid_ >> p.serverid_ >> param >> p.time_;
		return os;
	}

	ChargeSvrTimer& operator+=(const ChargeSvrTimer& p)
	{
		time_ += p.time_;
		return *this;
	}

	ChargeSvrTimer()
	{
		memset((char *)this, 0, sizeof(*this));
	}
};

// 徐进才删除
//struct JobParam
//{
//	int heroraceid_; // heroraceid_ = id 一条
//	int endurance_;
//	int strength_;
//	int arility_;
//	int intelligence_;
//
//	static int gettype() 
//	{ return CMD_JOB_PARAM;}
//
//	static const char * getname() 
//	{ return "JobParam";}
//
//	inline void setid(int heroraceid) 
//	{heroraceid_ = heroraceid;}
//
//	inline void getid(char * buf) 
//	{ sprintf(buf, "%d", heroraceid_);}
//
//	inline void setkeyvalue(int key) {}
//
//	inline int getkeyvalue() { return 0;}
//
//	int get_insert_str(char * buf)
//	{
//		//return sprintf(buf, "insert into JobParam(heroraceid,endurance,strength,arility,intelligence) values(%d,%d,%d,%d,%d) ", 
//		return sprintf(buf, "insert into JobParam values(%d,%d,%d,%d,%d) ", 
//			heroraceid_, endurance_, strength_, arility_, intelligence_);
//	}
//
//	int get_update_str(char * buf, JobParam & old)
//	{
//		return sprintf(buf, "update JobParam set endurance=%d,strength=%d,arility=%d,intelligence=%d where heroraceid=%d ", 
//			endurance_, strength_, arility_, intelligence_, old.heroraceid_);
//	}
//
//	int get_delete_str(char * buf)
//	{ 
//		return sprintf(buf, "delete from JobParam where heroraceid=%d ", heroraceid_);
//	}
//
//	static const char * get_select_str()
//	{ return "select * from JobParam";}
//
//	template < class otl_stream >
//		friend otl_stream& operator>>(otl_stream& os, JobParam& p)
//	{
//		os >> p.heroraceid_ >> p.endurance_ >> p.strength_ >> p.arility_ >> p.intelligence_;
//		return os;
//	}
//
//	JobParam& operator+=(const JobParam& p)
//	{
//		endurance_ += p.endurance_;
//		strength_ += p.strength_;
//		arility_ += p.arility_;
//		intelligence_ += p.intelligence_;
//		return *this;
//	}
//
//	JobParam()
//	{
//		memset((char *)this, 0, sizeof(*this));
//	}
//};

// 徐进才删除
//struct BinConfig
//{
//	int binid_; // binid_ = id 一条
//	char sureflopitem_[SIZE_32];
//	char randomflopitem_[SIZE_32];
//	char randomflopodds_[SIZE_64];
//
//	static int gettype() 
//	{ return CMD_BIN_CONFIG;}
//
//	static const char * getname() 
//	{ return "BinConfig";}
//
//	inline void setid(int binid) 
//	{binid_ = binid;}
//
//	inline void getid(char * buf) 
//	{ sprintf(buf, "%d", binid_);}
//
//	inline void setkeyvalue(int key) {}
//
//	inline int getkeyvalue() { return 0;}
//
//	int get_insert_str(char * buf)
//	{
//		//return sprintf(buf, "insert into BinConfig(binid,sureflopitem,randomflopitem,randomflopodds) values(%d,\'\'%s\'\',\'\'%s\'\',\'\'%s\'\') ", 
//		return sprintf(buf, "insert into BinConfig values(%d,\'\'%s\'\',\'\'%s\'\',\'\'%s\'\') ", 
//			binid_, sureflopitem_, randomflopitem_, randomflopodds_);
//	}
//
//	int get_update_str(char * buf, BinConfig & old)
//	{
//		return sprintf(buf, "update BinConfig set sureflopitem=\'\'%s\'\',randomflopitem=\'\'%s\'\',randomflopodds=\'\'%s\'\' where binid=%d ", 
//			sureflopitem_, randomflopitem_, randomflopodds_, old.binid_);
//	}
//
//	int get_delete_str(char * buf)
//	{ 
//		return sprintf(buf, "delete from BinConfig where binid=%d ", binid_);
//	}
//
//	static const char * get_select_str()
//	{ return "select * from BinConfig";}
//
//	template < class otl_stream >
//		friend otl_stream& operator>>(otl_stream& os, BinConfig& p)
//	{
//		os >> p.binid_ >> p.sureflopitem_>> p.randomflopitem_ >> p.randomflopodds_;
//		return os;
//	}
//
//	BinConfig& operator+=(const BinConfig& p)
//	{
//		return *this;
//	}
//
//	BinConfig()
//	{
//		memset((char *)this, 0, sizeof(*this));
//	}
//};

struct EquipAntagonism
{
	int weaponid_; 
	int defendid_;// weaponid_ + defendid_ = id 一条
	int attackodds_;

	static int gettype() 
	{ return CMD_EQUIP_ANTAGONISM;}

	static const char * getname() 
	{ return "EquipAntagonism";}

	inline void setid(int weaponid, int defendid) 
	{weaponid_ = weaponid;defendid_=defendid;}

	inline void getid(char * buf) 
	{ sprintf(buf, "%d-%d", weaponid_,defendid_);}

	inline void setkeyvalue(int key) {}

	inline int getkeyvalue() { return 0;}

	int get_insert_str(char * buf)
	{
		//return sprintf(buf, "insert into EquipAntagonism(weaponid,defendid,attackodds) values(%d,%d,%d) ", 
		return sprintf(buf, "insert into EquipAntagonism values(%d,%d,%d) ", 
			weaponid_, defendid_, attackodds_);
	}

	int get_update_str(char * buf, EquipAntagonism & old)
	{
		return sprintf(buf, "update EquipAntagonism set attackodds=%d where weaponid=%d and defendid=%d ", 
			attackodds_, old.weaponid_, old.defendid_);
	}

	int get_delete_str(char * buf)
	{ 
		return sprintf(buf, "delete from EquipAntagonism where weaponid=%d and defendid=%d ", weaponid_, defendid_);
	}

	static const char * get_select_str()
	{ return "select * from EquipAntagonism";}

	template < class otl_stream >
		friend otl_stream& operator>>(otl_stream& os, EquipAntagonism& p)
	{
		os >> p.weaponid_ >> p.defendid_ >> p.attackodds_;
		return os;
	}

	EquipAntagonism& operator+=(const EquipAntagonism& p)
	{
		attackodds_ += p.attackodds_;
		return *this;
	}

	EquipAntagonism()
	{
		memset((char *)this, 0, sizeof(*this));
	}
};


struct BatttleField
{
	int id_; // id_ = 一条
	int mapid_;
	int starth_;
	int startm_;
	int starts_;
	int endh_;
	int endm_;
	int ends_;
	int armyquantity_;
	int killbonus_;
	int deathbonus_;
	int vipquantity_;

	static int gettype() 
	{ return CMD_BATTTLE_FIELD;}

	static const char * getname() 
	{ return "BatttleField";}

	inline void setid(int id) 
	{id_ = id;}

	inline void getid(char * buf) 
	{ sprintf(buf, "%d", id_);}

	inline void setkeyvalue(int key) {}

	inline int getkeyvalue() { return 0;}

	int get_insert_str(char * buf)
	{
		//return sprintf(buf, "insert into BatttleField(id,mapid,starth,startm,starts,endh,endm,ends,"
		//	"armyquantity,killbonus,deathbonus,vipquantity) values(%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d) ", 
		return sprintf(buf, "insert into BatttleField values(%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d) ", 
			id_, mapid_, starth_, startm_, starts_, endh_, endm_, ends_, 
			armyquantity_, killbonus_, deathbonus_, vipquantity_);
	}

	int get_update_str(char * buf, BatttleField & old)
	{
		return sprintf(buf, "update BatttleField set mapid=%d,starth=%d,startm=%d,starts=%d,endh=%d,"
			"endm=%d,ends=%d,armyquantity=%d,killbonus=%d,deathbonus=%d,vipquantity=%d where id=%d ", 
			mapid_, starth_, startm_, starts_, endh_, endm_, ends_, armyquantity_, 
			killbonus_, deathbonus_, vipquantity_, old.id_);
	}

	int get_delete_str(char * buf)
	{ 
		return sprintf(buf, "delete from BatttleField where id=%d ", id_);
	}

	static const char * get_select_str()
	{ return "select * from BatttleField";}

	template < class otl_stream >
		friend otl_stream& operator>>(otl_stream& os, BatttleField& p)
	{
		os >> p.id_ >> p.mapid_ >> p.starth_ >> p.startm_ >> p.starts_ >> p.endh_ >> p.endm_ 
			>> p.ends_ >> p.armyquantity_ >> p.killbonus_ >> p.deathbonus_ >> p.vipquantity_;
		return os;
	}

	BatttleField& operator+=(const BatttleField& p)
	{
		mapid_ += p.mapid_;
		starth_ += p.starth_;
		startm_ += p.startm_;
		starts_ += p.starts_;
		endh_ += p.endh_;
		endm_ += p.endm_;
		ends_ += p.ends_;
		armyquantity_ += p.armyquantity_;
		killbonus_ += p.killbonus_;
		deathbonus_ += p.deathbonus_;
		vipquantity_ += p.vipquantity_;
		return *this;
	}

	BatttleField()
	{
		memset((char *)this, 0, sizeof(*this));
	}
};

struct ConsortiaCompare
{
	int id_; // id_ = 一条
	int conid_;
	char conname_[SIZE_32];
	int level_;
	int usercount_;
	int playerid_;
	char chairmanname_[SIZE_32];
	int country_;
	int64 createtime_;

	static int gettype() 
	{ return CMD_CONSORTIA_COMPARE;}

	static const char * getname() 
	{ return "ConsortiaCompare";}

	inline void setid(int id) 
	{id_ = id;}

	inline void getid(char * buf) 
	{ sprintf(buf, "%d", id_);}

	inline void setkeyvalue(int key) {}

	inline int getkeyvalue() { return 0;}

	int get_insert_str(char * buf)
	{
		//return sprintf(buf, "insert into ConsortiaCompare(id,conid,conname,level,usercount,playerid,chairmanname,"
		//	"country,createtime) values(%d,%d,\'\'%s\'\',%d,%d,%d,\'\'%s\'\',%d,%I64d) ", 
		return sprintf(buf, "insert into ConsortiaCompare values(%d,%d,\'\'%s\'\',%d,%d,%d,\'\'%s\'\',%d,%I64d) ", 
			id_, conid_, conname_, level_, usercount_, playerid_, chairmanname_, country_, createtime_);
	}

	int get_update_str(char * buf, ConsortiaCompare & old)
	{
		return sprintf(buf, "update ConsortiaCompare set conid=%d,conname=\'\'%s\'\',level=%d,usercount=%d,"
			"playerid=%d,chairmanname=\'\'%s\'\',country=%d,createtime=%I64d where id=%d ", 
			conid_, conname_, level_, usercount_, playerid_, chairmanname_, country_, createtime_, old.id_);
	}

	int get_delete_str(char * buf)
	{ 
		return sprintf(buf, "delete from ConsortiaCompare where id=%d ", id_);
	}

	static const char * get_select_str()
	{ return "select * from ConsortiaCompare";}

	template < class otl_stream >
		friend otl_stream& operator>>(otl_stream& os, ConsortiaCompare& p)
	{
		os >> p.id_ >> p.conid_ >> p.conname_ >> p.level_ >> p.usercount_ >> p.playerid_
			>> p.chairmanname_ >> p.country_ >> p.createtime_;
		return os;
	}

	ConsortiaCompare& operator+=(const ConsortiaCompare& p)
	{
		conid_ += p.conid_;
		level_ += p.level_;
		usercount_ += p.usercount_;
		playerid_ += p.playerid_;
		country_ += p.country_;
		createtime_ += p.createtime_;
		return *this;
	}

	ConsortiaCompare()
	{
		memset((char *)this, 0, sizeof(*this));
	}
};

struct PlayerCompare
{
	int id_; // id_ = 一条
	int playerid_;
	char playername_[SIZE_32];
	int country_;
	int officerrace_;
	int level_;
	int militaryrank_;
	int money_;
	int resourcecount_;
	int citycount_;
	int generalcount_;
	int credit_;

	static int gettype() 
	{ return CMD_PLAYER_COMPARE;}

	static const char * getname() 
	{ return "PlayerCompare";}

	inline void setid(int id) 
	{id_ = id;}

	inline void getid(char * buf) 
	{ sprintf(buf, "%d", id_);}

	inline void setkeyvalue(int key) {}

	inline int getkeyvalue() { return 0;}

	int get_insert_str(char * buf)
	{
		//return sprintf(buf, "insert into PlayerCompare(id,playerid,playername,country,officerrace,level,militaryrank,"
		//	"money,resourcecount,citycount,generalcount,credit) values(%d,%d,\'\'%s\'\',%d,%d,%d,%d,%d,%d,%d,%d,%d) ", 
		return sprintf(buf, "insert into PlayerCompare values(%d,%d,\'\'%s\'\',%d,%d,%d,%d,%d,%d,%d,%d,%d) ", 
			id_, playerid_, playername_, country_, officerrace_, level_, militaryrank_, 
			money_, resourcecount_, citycount_, generalcount_, credit_);
     	}

	int get_update_str(char * buf, PlayerCompare & old)
	{
		return sprintf(buf, "update PlayerCompare set playerid=%d,playername=\'\'%s\'\',country=%d,officerrace=%d,"
			"level=%d,militaryrank=%d,money=%d,resourcecount=%d,citycount=%d,generalcount=%d,credit=%d where id=%d ", 
			playerid_, playername_, country_, officerrace_, level_, militaryrank_, money_, 
			resourcecount_, citycount_, generalcount_, credit_, old.id_);
	}

	int get_delete_str(char * buf)
	{ 
		return sprintf(buf, "delete from PlayerCompare where id=%d ", id_);
	}

	static const char * get_select_str()
	{ return "select * from PlayerCompare";}

	template < class otl_stream >
		friend otl_stream& operator>>(otl_stream& os, PlayerCompare& p)
	{
		os >> p.id_ >> p.playerid_ >> p.playername_ >> p.country_ >> p.officerrace_ >> p.level_ >> p.militaryrank_ 
			>> p.money_ >> p.resourcecount_ >> p.citycount_ >> p.generalcount_ >> p.credit_;
		return os;
	}

	PlayerCompare& operator+=(const PlayerCompare& p)
	{
		playerid_ += p.playerid_;
		country_ += p.country_;
		officerrace_ += p.officerrace_;
		level_ += p.level_;
		militaryrank_ += p.militaryrank_;
		money_ += p.money_;
		resourcecount_ += p.resourcecount_;
		citycount_ += p.citycount_;
		generalcount_ += p.generalcount_;
		credit_ += p.credit_;
		return *this;
	}

	PlayerCompare()
	{
		memset((char *)this, 0, sizeof(*this));
	}
};

struct Council
{
	int countryid_;
	int type_; // countryid_ + type_ = id 多条
	int playerid_; // countryid_ + type_ + playerid_ = id 一条
	char name_[SIZE_24];
	int battle_;
	int economy_;
	int contribute_;
	int vote_;
	int postion_;

	static int gettype() 
	{ return CMD_COUNCIL;}

	static const char * getname() 
	{ return "Council";}

	inline void setmasterid(int countryid, int type) 
	{countryid_ = countryid; type_ = type;}

	inline void getmasterid(char * buf) 
	{ sprintf(buf, "%d-%d", countryid_, type_);}

	inline void setdetailid(int playerid) 
	{playerid_ = playerid;}

	inline void getdetailid(char * buf) 
	{ sprintf(buf, "%d", playerid_);}

	inline void setkeyvalue(int key){}

	inline int getkeyvalue() { return 0; }

	int get_insert_str(char * buf)
	{
		//return sprintf(buf, "insert into Council(countryid,type,playerid,name,battle,economy,"
		//	"contribute,vote,postion) values(%d,%d,%d,\'\'%s\'\',%d,%d,%d,%d,%d) ", 
		return sprintf(buf, "insert into Council values(%d,%d,%d,\'\'%s\'\',%d,%d,%d,%d,%d) ", 
			countryid_, type_, playerid_, name_, battle_, economy_, contribute_, vote_, postion_);
     }

	int get_update_str(char * buf, Council & old)
	{
		return sprintf(buf, "update Council set name=\'\'%s\'\',battle=%d,economy=%d,"
			"contribute=%d,vote=%d,postion=%d where countryid=%d and type=%d and playerid=%d ", 
			name_, battle_, economy_, contribute_, vote_, postion_, old.countryid_, old.type_, old.playerid_);
	}

	int get_delete_str(char * buf)
	{ 
		return sprintf(buf, "delete from Council where countryid=%d and type=%d and playerid=%d ", countryid_, type_, playerid_);
	}

	static const char * get_select_str()
	{ return "select * from Council";}

	template < class otl_stream >
		friend otl_stream& operator>>(otl_stream& os, Council& p)
	{
		os >> p.countryid_ >> p.type_ >> p.playerid_ >> p.name_ >> p.battle_ 
			>> p.economy_ >> p.contribute_ >> p.vote_ >> p.postion_;
		return os;
	}

	Council& operator+=(const Council& p)
	{
		battle_ += p.battle_;
		economy_ += p.economy_;
		contribute_ += p.contribute_;
		vote_ += p.vote_;
		postion_ += p.postion_;
		return *this;
	}

	Council()
	{
		memset((char *)this, 0, sizeof(*this));
	}
};

struct Candidate
{
	int countryid_;
	int type_; // countryid_ + type_ = id 多条
	int playerid_; // countryid_ + type_ + playerid_ = id 一条
	char name_[SIZE_24];
	char creed_[100];
	int expense_;
	int vote_;
	int64 time_;

	static int gettype() 
	{ return CMD_CANDIDATE;}

	static const char * getname() 
	{ return "Candidate";}

	inline void setmasterid(int countryid, int type) 
	{countryid_ = countryid; type_ = type;}

	inline void getmasterid(char * buf) 
	{ sprintf(buf, "%d-%d", countryid_, type_);}

	inline void setdetailid(int playerid) 
	{playerid_ = playerid;}

	inline void getdetailid(char * buf) 
	{ sprintf(buf, "%d", playerid_);}

	inline void setkeyvalue(int key){}

	inline int getkeyvalue() { return 0; }

	int get_insert_str(char * buf)
	{
		//return sprintf(buf, "insert into Candidate(countryid,type,playerid,name,creed,expense,"
		//	"vote,time) values(%d,%d,%d,\'\'%s\'\',\'\'%s\'\',%d,%d,%I64d) ",
		return sprintf(buf, "insert into Candidate values(%d,%d,%d,\'\'%s\'\',\'\'%s\'\',%d,%d,%I64d) ",
			countryid_, type_, playerid_, name_, creed_, expense_, vote_, time_);
     }

	int get_update_str(char * buf, Candidate & old)
	{
		return sprintf(buf, "update Candidate set name=\'\'%s\'\',creed=\'\'%s\'\',expense=%d,"
			"vote=%d,time=%I64d where countryid=%d and type=%d and playerid=%d ", 
			name_, creed_, expense_, vote_, time_, old.countryid_, old.type_, old.playerid_);
	}

	int get_delete_str(char * buf)
	{
		return sprintf(buf, "delete from Candidate where countryid=%d and type=%d and playerid=%d ", countryid_, type_, playerid_);
	}

	static const char * get_select_str()
	{ return "select * from Candidate";}

	template < class otl_stream >
		friend otl_stream& operator>>(otl_stream& os, Candidate& p)
	{
		os >> p.countryid_ >> p.type_ >> p.playerid_ >> p.name_ 
			>> p.creed_ >> p.expense_ >> p.vote_ >> p.time_;
		return os;
	}

	Candidate& operator+=(const Candidate& p)
	{
		expense_ += p.expense_;
		vote_ += p.vote_;
		time_ += p.time_;
		return *this;
	}

	Candidate()
	{
		memset((char *)this, 0, sizeof(*this));
	}
};

struct Ballot
{
	int countryid_;
	int type_; // countryid_ + type_ = id 多条
	int playerid_; // countryid_ + type_ + playerid_ = id 一条
	int candidate_;
	int value_;

	static int gettype() 
	{ return CMD_BALLOT;}

	static const char * getname() 
	{ return "Ballot";}

	inline void setmasterid(int countryid, int type) 
	{countryid_ = countryid; type_ = type;}

	inline void getmasterid(char * buf) 
	{ sprintf(buf, "%d-%d", countryid_, type_);}

	inline void setdetailid(int playerid) 
	{playerid_ = playerid;}

	inline void getdetailid(char * buf) 
	{ sprintf(buf, "%d", playerid_);}

	inline void setkeyvalue(int key){}

	inline int getkeyvalue() { return 0; }

	int get_insert_str(char * buf)
	{
		//return sprintf(buf, "insert into Ballot(countryid,type,playerid,candidate,value) values(%d,%d,%d,%d,%d) ",
		return sprintf(buf, "insert into Ballot values(%d,%d,%d,%d,%d) ",
			countryid_, type_, playerid_, candidate_, value_);
     }

	int get_update_str(char * buf, Ballot & old)
	{
		return sprintf(buf, "update Ballot set candidate=%d,value=%d where countryid=%d and type=%d and playerid=%d ", 
			candidate_, value_, old.countryid_, old.type_, old.playerid_);
	}

	int get_delete_str(char * buf)
	{
		return sprintf(buf, "delete from Ballot where countryid=%d and type=%d and playerid=%d ", countryid_, type_, playerid_);
	}

	static const char * get_select_str()
	{ return "select * from Ballot";}

	template < class otl_stream >
		friend otl_stream& operator>>(otl_stream& os, Ballot& p)
	{
		os >> p.countryid_ >> p.type_ >> p.playerid_ >> p.candidate_ >> p.value_;
		return os;
	}

	Ballot& operator+=(const Ballot& p)
	{
		candidate_ += p.candidate_;
		value_ += p.value_;
		return *this;
	}

	Ballot()
	{
		memset((char *)this, 0, sizeof(*this));
	}
};

struct Country
{
	int countryid_; // id_ = 一条
	int treasury_;
	int dodmoney_;
	int financemoney_;
	int rate_;
	int64 nexttime1_;
	int64 nexttime2_;
	int mapid_;

	static int gettype() 
	{ return CMD_COUNTRY;}

	static const char * getname() 
	{ return "Country";}

	inline void setid(int countryid) 
	{countryid_ = countryid;}

	inline void getid(char * buf) 
	{ sprintf(buf, "%d", countryid_);}

	inline void setkeyvalue(int key) {}

	inline int getkeyvalue() { return 0;}

	int get_insert_str(char * buf)
	{
		//return sprintf(buf, "insert into Country(countryid,treasury,dodmoney,financemoney,rate,nexttime1,"
		//	"nexttime2,mapid) values(%d,%d,%d,%d,%d,%I64d,%I64d,%d) ",
		return sprintf(buf, "insert into Country values(%d,%d,%d,%d,%d,%I64d,%I64d,%d) ",
			countryid_, treasury_, dodmoney_, financemoney_, rate_, nexttime1_, nexttime2_, mapid_);
     }

	int get_update_str(char * buf, Country & old)
	{
		return sprintf(buf, "update Country set treasury=%d,dodmoney=%d,financemoney=%d,rate=%d,"
			"nexttime1=%I64d,nexttime2=%I64d,mapid=%d where countryid=%d ", 
			treasury_, dodmoney_, financemoney_, rate_, nexttime1_, nexttime2_, mapid_, old.countryid_);
	}

	int get_delete_str(char * buf)
	{
		return sprintf(buf, "delete from Country where countryid=%d ", countryid_);
	}

	static const char * get_select_str()
	{ return "select * from Country";}

	template < class otl_stream >
		friend otl_stream& operator>>(otl_stream& os, Country& p)
	{
		os >> p.countryid_ >> p.treasury_ >> p.dodmoney_ >> p.financemoney_ 
			>> p.rate_ >> p.nexttime1_ >> p.nexttime2_ >> p.mapid_;
		return os;
	}

	Country& operator+=(const Country& p)
	{
		treasury_ += p.treasury_;
		dodmoney_ += p.dodmoney_;
		financemoney_ += p.financemoney_;
		rate_ += p.rate_;
		nexttime1_ += p.nexttime1_;
		nexttime2_ += p.nexttime2_;
		mapid_ += p.mapid_;
		return *this;
	}

	Country()
	{
		memset((char *)this, 0, sizeof(*this));
	}
};

struct ResearchActivity
{
	int playerid_; // playerid_ = id 多条
	int type_;
	int itemid_; // playerid_ + type_ + itemid_ = id 一条
	int level_;
	int64 endtime_;

	static int gettype() 
	{ return CMD_RESEARCH_ACTIVITY;}

	static const char * getname() 
	{ return "ResearchActivity";}

	inline void setmasterid(int playerid) 
	{playerid_ = playerid;}

	inline void getmasterid(char * buf) 
	{ sprintf(buf, "%d", playerid_);}

	inline void setdetailid(int type, int itemid) 
	{type_ = type; itemid_ = itemid;}

	inline void getdetailid(char * buf) 
	{ sprintf(buf, "%d-%d", type_, itemid_);}

	inline void setkeyvalue(int key){}

	inline int getkeyvalue() { return 0; }

	int get_insert_str(char * buf)
	{
		//return sprintf(buf, "insert into ResearchActivity(playerid,type,itemid,level,endtime) values(%d,%d,%d,%d,%I64d) ",
		return sprintf(buf, "insert into ResearchActivity values(%d,%d,%d,%d,%I64d) ",
			playerid_, type_, itemid_, level_, endtime_);
     }

	int get_update_str(char * buf, ResearchActivity & old)
	{
		return sprintf(buf, "update ResearchActivity set level=%d,"
			"endtime=%I64d where playerid=%d and type=%d and itemid=%d ", 
			level_, endtime_, old.playerid_, old.type_, old.itemid_);
	}

	int get_delete_str(char * buf)
	{
		return sprintf(buf, "delete from ResearchActivity where playerid=%d and type=%d and itemid=%d ", playerid_, type_, itemid_);
	}

	static const char * get_select_str()
	{ return "select * from ResearchActivity";}

	template < class otl_stream >
		friend otl_stream& operator>>(otl_stream& os, ResearchActivity& p)
	{
		os >> p.playerid_ >> p.type_ >> p.itemid_ >> p.level_ >> p.endtime_;
		return os;
	}

	ResearchActivity& operator+=(const ResearchActivity& p)
	{
		level_ += p.level_;
		endtime_ += p.endtime_;
		return *this;
	}

	ResearchActivity()
	{
		memset((char *)this, 0, sizeof(*this));
	}
};

struct LevySoldierActivity
{
	int sortid_; // playerid_ + mapid_ + cityid_ + sortid_ = id 一条
	int playerid_;
	int mapid_;
	int cityid_; // playerid_ + mapid_ + cityid_ = id 多条
	int raceid_;
	int count_;
	int64 endtime_;

	static int gettype() 
	{ return CMD_LEVY_SOLDIER_ACTIVITY;}

	static const char * getname() 
	{ return "LevySoldierActivity";}

	inline void setmasterid(int playerid, int mapid, int cityid) 
	{playerid_ = playerid;mapid_ = mapid;cityid_ = cityid;}

	inline void getmasterid(char * buf) 
	{ sprintf(buf, "%d-%d-%d", playerid_, mapid_, cityid_);}

	inline void setdetailid(int sortid) 
	{sortid_ = sortid;}

	inline void getdetailid(char * buf) 
	{ sprintf(buf, "%d", sortid_);}

	inline void setkeyvalue(int key){}

	inline int getkeyvalue() { return 0; }

	int get_insert_str(char * buf)
	{
		//return sprintf(buf, "insert into LevySoldierActivity(sortid,playerid,mapid,cityid,raceid,count,endtime) values(%d,%d,%d,%d,%d,%d,%I64d) ",
		return sprintf(buf, "insert into LevySoldierActivity values(%d,%d,%d,%d,%d,%d,%I64d) ",
			sortid_, playerid_, mapid_, cityid_, raceid_, count_, endtime_);
     }

	int get_update_str(char * buf, LevySoldierActivity & old)
	{
		return sprintf(buf, "update LevySoldierActivity set raceid=%d,count=%d,endtime=%I64d"
			" where playerid=%d and mapid=%d and cityid=%d and sortid=%d ", 
			raceid_, count_, endtime_, old.playerid_, old.mapid_, old.cityid_, old.sortid_);
	}

	int get_delete_str(char * buf)
	{
		return sprintf(buf, "delete from LevySoldierActivity where playerid=%d and mapid=%d and cityid=%d and sortid=%d ", 
			playerid_, mapid_, cityid_, sortid_);
	}

	static const char * get_select_str()
	{ return "select * from LevySoldierActivity";}

	template < class otl_stream >
		friend otl_stream& operator>>(otl_stream& os, LevySoldierActivity& p)
	{
		os >> p.sortid_ >> p.playerid_ >> p.mapid_ >> p.cityid_ >> p.raceid_ >> p.count_ >> p.endtime_;
		return os;
	}

	LevySoldierActivity& operator+=(const LevySoldierActivity& p)
	{
		raceid_ += p.raceid_;
		count_ += p.count_;
		endtime_ += p.endtime_;
		return *this;
	}

	LevySoldierActivity()
	{
		memset((char *)this, 0, sizeof(*this));
	}
};

// 徐进才增加
struct PlayerSkillTable
{
	int playerid_; // playerid_ + officerid_ = id 多条
	int officerid_;
	int skillid_; // playerid_ + officerid_ + skillid_ = id 一条
	int skilltype_;
	int skilllevel_;

	static int gettype() 
	{ return CMD_PLAYER_SKILL_TABLE;}

	static const char * getname() 
	{ return "PlayerSkillTable";}

	inline void setmasterid(int playerid, int officerid) 
	{playerid_ = playerid;officerid_=officerid;}

	inline void getmasterid(char * buf) 
	{ sprintf(buf, "%d-%d", playerid_, officerid_);}

	inline void setdetailid(int skillid) 
	{skillid_=skillid;}

	inline void getdetailid(char * buf) 
	{ sprintf(buf, "%d", skillid_);}

	inline void setkeyvalue(int key){}

	inline int getkeyvalue() { return 0; }

	int get_insert_str(char * buf)
	{
		return sprintf(buf, "insert into PlayerSkillTable values(%d,%d,%d,%d,%d) ",
			playerid_, officerid_, skillid_, skilltype_, skilllevel_);
     }

	int get_update_str(char * buf, PlayerSkillTable & old)
	{
		return sprintf(buf, "update PlayerSkillTable set skilltype=%d,skilllevel=%d"
			" where playerid=%d and officerid=%d and skillid=%d ", 
			skilltype_, skilllevel_, old.playerid_, old.officerid_, old.skillid_);
	}

	int get_delete_str(char * buf)
	{
		return sprintf(buf, "delete from PlayerSkillTable where playerid=%d and officerid=%d and skillid=%d ", 
			playerid_, officerid_, skillid_);
	}

	static const char * get_select_str()
	{ return "select * from PlayerSkillTable";}

	template < class otl_stream >
		friend otl_stream& operator>>(otl_stream& os, PlayerSkillTable& p)
	{
		os >> p.playerid_ >> p.officerid_ >> p.skillid_	>> p.skilltype_	>> p.skilllevel_;
		return os;
	}

	PlayerSkillTable& operator+=(const PlayerSkillTable& p)
	{
		skilltype_ += p.skilltype_;
		skilllevel_ += p.skilllevel_;
		return *this;
	}

	PlayerSkillTable()
	{
		memset((char *)this, 0, sizeof(*this));
	}
};

// 柳敬武增加
struct LimitTable
{
	int limittype_; // limittype_ = id 多条
	int limitkey_;  // limittype_ + limitkey_ = id 一条
	int limitval_;

	static int gettype() 
	{ return CMD_LIMIT_TABLE;}

	static const char * getname() 
	{ return "LimitTable";}

	inline void setmasterid(int limittype) 
	{limittype_ = limittype;}

	inline void getmasterid(char * buf) 
	{ sprintf(buf, "%d", limittype_);}

	inline void setdetailid(int limitkey) 
	{limitkey_=limitkey;}

	inline void getdetailid(char * buf) 
	{ sprintf(buf, "%d", limitkey_);}

	inline void setkeyvalue(int key){}

	inline int getkeyvalue() { return 0; }

	int get_insert_str(char * buf)
	{
		return sprintf(buf, "insert into LimitTable values(%d,%d,%d) ",
			limittype_, limitkey_, limitval_);
     }

	int get_update_str(char * buf, LimitTable & old)
	{
		return sprintf(buf, "update LimitTable set limitval=%d where limittype=%d and limitkey=%d ", 
			limitval_, old.limittype_, old.limitkey_);
	}

	int get_delete_str(char * buf)
	{
		return sprintf(buf, "delete from LimitTable where limittype=%d and limitkey=%d ", 
			limittype_, limitkey_);
	}

	static const char * get_select_str()
	{ return "select * from LimitTable";}

	template < class otl_stream >
		friend otl_stream& operator>>(otl_stream& os, LimitTable& p)
	{
		os >> p.limittype_ >> p.limitkey_ >> p.limitval_;
		return os;
	}

	LimitTable& operator+=(const LimitTable& p)
	{
		limitval_ += p.limitval_;
		return *this;
	}

	LimitTable()
	{
		memset((char *)this, 0, sizeof(*this));
	}
};

// 徐进才增加
struct PlayerDayTask
{
	int playerid_; // playerid_ = id 多条
	int taskid_; // playerid_ + taskid_ = id 一条
	int mapid_;
	int64 starttime_;
	int64 endtime_;
	int state_;

	static int gettype() 
	{ return CMD_PLAYER_DAY_TASK;}

	static const char * getname() 
	{ return "PlayerDayTask";}

	inline void setmasterid(int playerid) 
	{playerid_ = playerid;}

	inline void getmasterid(char * buf) 
	{sprintf(buf, "%d", playerid_);}

	inline void setdetailid(int taskid) 
	{taskid_ = taskid;}

	inline void getdetailid(char * buf) 
	{sprintf(buf, "%d", taskid_);}

	inline void setkeyvalue(int key) {}

	inline int getkeyvalue() { return 0; }

	int get_insert_str(char * buf)
	{ 
		//return sprintf(buf, "insert into PlayerTask(playerid,taskid,state,tasktime) values(%d,%d,%d,%I64d) ", playerid_, taskid_, state_, tasktime_);
		return sprintf(buf, "insert into PlayerDayTask values(%d,%d,%d,%I64d,%I64d,%d) ", playerid_, taskid_, mapid_, starttime_, endtime_, state_);
	}

	int get_update_str(char * buf, PlayerDayTask & old)
	{
		return sprintf(buf, "update PlayerDayTask set mapid=%d,starttime=%I64d,endtime=%I64d,state=%d where playerid=%d and taskid=%d ",
			mapid_, starttime_, endtime_, state_, old.playerid_, old.taskid_);
	}

	int get_delete_str(char * buf)
	{ 
		return sprintf(buf, "delete from PlayerDayTask where playerid=%d and taskid=%d ", playerid_, taskid_);
	}

	static const char * get_select_str()
	{ return "select * from PlayerDayTask";}

	template < class otl_stream >
		friend otl_stream& operator>>(otl_stream& os, PlayerDayTask& p)
	{
		os >> p.playerid_ >> p.taskid_ >> p.mapid_ >> p.starttime_ >> p.endtime_ >> p.state_;
		return os;
	}

	PlayerDayTask& operator+=(const PlayerDayTask& p)
	{
		mapid_ += p.mapid_;
		starttime_ += p.starttime_;
		endtime_ += p.endtime_;
		state_ += p.state_;
		return *this;
	}

	PlayerDayTask()
	{
		memset((char *)this, 0, sizeof(*this));
	}
};

// 徐进才增加
struct ConsortiaCityInfo
{
	int mapid_; // mapid_ = id 多条
	int cityid_; // mapid_ + cityid_ = id 一条
	int consortiaid_;
	int64 starttime_;

	static int gettype() 
	{ return CMD_CONSORTIA_CITY_INFO;}

	static const char * getname() 
	{ return "ConsortiaCityInfo";}

	inline void setmasterid(int mapid) 
	{mapid_ = mapid;}

	inline void getmasterid(char * buf) 
	{sprintf(buf, "%d", mapid_);}

	inline void setdetailid(int cityid) 
	{cityid_ = cityid;}

	inline void getdetailid(char * buf) 
	{sprintf(buf, "%d", cityid_);}

	inline void setkeyvalue(int key) {}

	inline int getkeyvalue() { return 0; }

	int get_insert_str(char * buf)
	{ 
		return sprintf(buf, "insert into ConsortiaCityInfo values(%d,%d,%d,%I64d) ", mapid_, cityid_, consortiaid_, starttime_);
	}

	int get_update_str(char * buf, ConsortiaCityInfo & old)
	{
		return sprintf(buf, "update ConsortiaCityInfo set consortiaid=%d,starttime=%I64d where mapid=%d and cityid=%d ",
			consortiaid_, starttime_, old.mapid_, old.cityid_);
	}

	int get_delete_str(char * buf)
	{ 
		return sprintf(buf, "delete from ConsortiaCityInfo where mapid=%d and cityid=%d ", mapid_, cityid_);
	}

	static const char * get_select_str()
	{ return "select * from ConsortiaCityInfo";}

	template < class otl_stream >
		friend otl_stream& operator>>(otl_stream& os, ConsortiaCityInfo& p)
	{
		os >> p.mapid_ >> p.cityid_ >> p.consortiaid_ >> p.starttime_;
		return os;
	}

	ConsortiaCityInfo& operator+=(const ConsortiaCityInfo& p)
	{
		consortiaid_ += p.consortiaid_;
		starttime_ += p.starttime_;
		return *this;
	}

	ConsortiaCityInfo()
	{
		memset((char *)this, 0, sizeof(*this));
	}
};

// 黄文才增加
struct TrainingSysTable
{
	int playerid_;
	int cityid_;
	int doorid_; // playerid_ + cityid_ + doorid_ = id 多条
	int heroid_; // playerid_ + cityid_ + doorid_ + heroid_ = id 一条
	char heroname_[SIZE_32];
	int hangsystype_;
	int64 starttime_;
	int64 endtime_;
	int expval_;
	int moneyval_;

	static int gettype() 
	{ return CMD_TRAINING_SYS_TABLE;}

	static const char * getname() 
	{ return "TrainingSysTable";}

	inline void setmasterid(int playerid, int cityid, int doorid) 
	{playerid_ = playerid; cityid_ = cityid; doorid_ = doorid; }

	inline void getmasterid(char * buf) 
	{sprintf(buf, "%d-%d-%d", playerid_, cityid_, doorid_);}

	inline void setdetailid(int heroid) 
	{heroid_ = heroid;}

	inline void getdetailid(char * buf) 
	{sprintf(buf, "%d", heroid_);}

	inline void setkeyvalue(int key) {}

	inline int getkeyvalue() { return 0;}

	int get_insert_str(char * buf)
	{ 
		return sprintf(buf, "insert into TrainingSysTable values(%d,%d,%d,%d,\'\'%s\'\',%d,%I64d,%I64d,%d,%d) ", 
			playerid_, cityid_, doorid_, heroid_, heroname_, hangsystype_, starttime_, endtime_, expval_, moneyval_);
	}

	int get_update_str(char * buf, TrainingSysTable & old)
	{
		return sprintf(buf, "update TrainingSysTable set heroname=\'\'%s\'\',hangsystype=%d,starttime=%I64d,endtime=%I64d,expval=%d,"
			"moneyval=%d where playerid=%d and cityid=%d and doorid=%d and heroid=%d ",
			heroname_, hangsystype_, starttime_, endtime_, expval_, moneyval_, old.playerid_, old.cityid_, old.doorid_, old.heroid_);
	}

	int get_delete_str(char * buf)
	{ 
		return sprintf(buf, "delete from TrainingSysTable where playerid=%d and cityid=%d and doorid=%d and heroid=%d ", playerid_, cityid_, doorid_, heroid_);
	}

	static const char * get_select_str()
	{ return "select * from TrainingSysTable";}

	template < class otl_stream >
	friend otl_stream& operator>>(otl_stream& os, TrainingSysTable& p)
	{
		os >> p.playerid_>> p.cityid_>> p.doorid_>> p.heroid_>> p.heroname_
			>> p.hangsystype_>> p.starttime_>> p.endtime_>> p.expval_>> p.moneyval_;
		return os;
	}

	TrainingSysTable& operator+=(const TrainingSysTable& p)
	{
		hangsystype_ += p.hangsystype_;
		starttime_ += p.starttime_;
		endtime_ += p.endtime_;
		expval_ += p.expval_;
		moneyval_ += p.moneyval_;
		return *this;
	}

	TrainingSysTable()
	{
		memset((char *)this, 0, sizeof(*this));
	}
};


#pragma pack(pop)

#endif // __INCLUDE_DB_TABLE_H__
