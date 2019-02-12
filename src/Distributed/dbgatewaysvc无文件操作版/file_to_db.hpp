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

#ifndef __INCLUDE_FILE_TO_DB_H__
#define __INCLUDE_FILE_TO_DB_H__

#include "header.hpp"
#include "DB_Table.h"
#include "DB_Proxy.h"

const char * address = "127.0.0.1";
short port = 8899;


int file_init(const char *);
void file_fini(int);

int file_write(int, char *, int);
int file_read(int, char *, int);

int file_exists(const char *);
unsigned long file_length(int);
unsigned long file_seek(int, unsigned long);


int file_init(const char * n)
{
	int fd;
	SECURITY_ATTRIBUTES sa;

	if (!n) return -1;

	if (file_exists(n) == -1)
		return -1;

	sa.nLength = sizeof(sa);
	sa.lpSecurityDescriptor = 0;
	sa.bInheritHandle = (GENERIC_READ | GENERIC_WRITE & 0x0080) == 0;

	fd = (int)CreateFile(n, 
		GENERIC_READ, 0, &sa,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (!fd || (fd == 0xffffffff))
		return -1;

	return fd;
}

void file_fini(int fd)
{
	if (!fd || (fd == 0xffffffff)) return;
	CloseHandle((HANDLE)fd);
}

int file_write(int fd, char * buf, int len)
{
	unsigned long t;

	if (!buf || !len) return -1;
	if (!fd || (fd == 0xffffffff)) return -1;

	if (WriteFile((HANDLE)fd, buf, len, &t, 0))
		return t;

	return -1;
}

int file_read(int fd, char * buf, int len)
{
	unsigned long t;

	if (!buf || !len) return -1;
	if (!fd || (fd == 0xffffffff)) return -1;

	if (ReadFile((HANDLE)fd, buf, len, &t, 0))
		return t;

	return -1;
}

int file_exists(const char * file)
{
	HANDLE h;
	WIN32_FIND_DATA f;

	if (!file) return -1;

	h = FindFirstFile(file, &f);
	if (h != INVALID_HANDLE_VALUE)
	{
		FindClose(h);
		return 0;
	}

	return -1;
}

unsigned long file_length(int fd)
{
	unsigned long b, e;

	if (!fd || (fd == 0xffffffff)) return 0;

	b = SetFilePointer((HANDLE)fd, 0, 0, FILE_BEGIN);
	e = SetFilePointer((HANDLE)fd, 0, 0, FILE_END);
	SetFilePointer((HANDLE)fd, 0, 0, FILE_BEGIN);
	return e - b;
}

unsigned long file_seek(int fd, unsigned long pos)
{
	if (!fd || (fd == 0xffffffff)) return 0;
	if (SetFilePointer((HANDLE)fd, pos, 0, FILE_BEGIN) == 0xffffffff)
		return -1;

	return 0;
}

#include <fstream>

std::ofstream _fout("test_dbgatewaysvc.log");

void add_GameCityInfo()
{
	int fd = file_init("GameCityInfo.csv");
	if (fd == -1) 
	{
		_fout << "找不到文件：GameCityInfo.csv\n";
	}

	char buf[1024000];
	int len = file_length(fd);
	if (file_read(fd, buf, len) == -1)
	{
		_fout << "读<GameCityInfo.csv>文件错误\n";
		file_fini(fd);
		return ;
	}
	buf[len] = '\0';
	file_fini(fd);

	DB_Proxy proxy;
	proxy.init(address, port);

	char * p1,*ptr = strstr(buf, "\r\n") + 2;
	while (1)
	{
		if (ptr[0] == '\0') break;

		GameCityInfo gci;
		if (sscanf(ptr, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d", &gci.cityid_, &gci.mapid_,&gci.foodcount_,&gci.woodcount_,
			&gci.ironcount_,&gci.skincount_,&gci.x_,&gci.y_,&gci.human_,&gci.mannikin_,&gci.fairy_,&gci.dandiprat_,
			&gci.halfanimal_,&gci.cattle_,&gci.blackfairy_,&gci.decpesrson_) != 16)
		{
			_fout << "提取文件内容错误<GameCityInfo.csv>\n";
			break;
		}
		proxy.db_insert(gci);
		p1 = ptr;
		ptr = strstr(p1, "\r\n") + 2;
	}

	char * buff;
	int num;
	GameCityInfo es;
	proxy.db_select_all(es, buff, num);
	GameCityInfo * pes = (GameCityInfo *)buff;
	num = num / sizeof(es);
	printf("GameCityInfo--------------------------\n");
	for (int i = 0; i < num; i++)
	{
		printf("%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n", pes[i].cityid_, pes[i].mapid_,pes[i].foodcount_,pes[i].woodcount_,
			pes[i].ironcount_,pes[i].skincount_,pes[i].x_,pes[i].y_,pes[i].human_,pes[i].mannikin_,pes[i].fairy_,pes[i].dandiprat_,
			pes[i].halfanimal_,pes[i].cattle_,pes[i].blackfairy_,pes[i].decpesrson_);
	}
	printf("GameCityInfo--------------------------\n");

	proxy.fini();
}

void add_ConstructionConfigure()
{
	int fd = file_init("ConstructionConfigure.csv");
	if (fd == -1) 
	{
		_fout << "找不到文件：ConstructionConfigure.csv\n";
	}

	char buf[1024000];
	int len = file_length(fd);
	if (file_read(fd, buf, len) == -1)
	{
		_fout << "读<ConstructionConfigure.csv>文件错误\n";
		file_fini(fd);
		return ;
	}
	buf[len] = '\0';
	file_fini(fd);

	DB_Proxy proxy;
	proxy.init(address, port);

	char * p1,*ptr = strstr(buf, "\r\n") + 2;
	while (1)
	{
		if (ptr[0] == '\0') break;

		ConstructionConfigure cc;
		memset(&cc, 0, sizeof(cc));
		if (sscanf(ptr, "%d,%d,%I64d,%d,%d,%d,%d,%d,%d,%d", &cc.constructionid_,&cc.level_,&cc.completetimes_,
			&cc.money_,&cc.foodcount_,&cc.woodcount_,&cc.ironcount_,&cc.skincount_,&cc.science_,&cc.modify_) != 10)
		{
			_fout << "提取文件内容错误<ConstructionConfigure.csv>\n";
			break;
		}
		proxy.db_insert(cc);
		p1 = ptr;
		ptr = strstr(p1, "\r\n") + 2;
	}

	char * buff;
	int num;
	ConstructionConfigure es;
	proxy.db_select_all(es, buff, num);
	ConstructionConfigure * pes = (ConstructionConfigure *)buff;
	num = num / sizeof(es);
	printf("ConstructionConfigure--------------------------\n");
	for (int i = 0; i < num; i++)
	{
		printf("%d,%d,%I64d,%d,%d,%d,%d,%d,%d,%d\n", pes[i].constructionid_,pes[i].level_,pes[i].completetimes_,
			pes[i].money_,pes[i].foodcount_,pes[i].woodcount_,pes[i].ironcount_,pes[i].skincount_,pes[i].science_,pes[i].modify_);
	}
	printf("ConstructionConfigure--------------------------\n");

	proxy.fini();
}

void add_DefendAttribute()
{
	int fd = file_init("DefendAttribute.csv");
	if (fd == -1) 
	{
		_fout << "找不到文件：DefendAttribute.csv\n";
	}

	char buf[1024000];
	int len = file_length(fd);
	if (file_read(fd, buf, len) == -1)
	{
		_fout << "读<DefendAttribute.csv>文件错误\n";
		file_fini(fd);
		return ;
	}
	buf[len] = '\0';
	file_fini(fd);

	DB_Proxy proxy;
	proxy.init(address, port);

	char * p1,*ptr = strstr(buf, "\r\n") + 2;
	while (1)
	{
		if (ptr[0] == '\0') break;

		DefendAttribute cc;
		memset(&cc, 0, sizeof(cc));
		if (sscanf(ptr, "%d,%d,%d,%d,%d,%d", &cc.defendid_,&cc.defendtype_, &cc.cost_,&cc.decrate_,&cc.physicerec_,&cc.magicrec_) != 6)
		{
			_fout << "提取文件内容错误<DefendAttribute.csv>\n";
			break;
		}
		proxy.db_insert(cc);
		p1 = ptr;
		ptr = strstr(p1, "\r\n") + 2;
	}

	char * buff;
	int num;
	DefendAttribute es;
	proxy.db_select_all(es, buff, num);
	DefendAttribute * pes = (DefendAttribute *)buff;
	num = num / sizeof(es);
	printf("DefendAttribute--------------------------\n");
	for (int i = 0; i < num; i++)
	{
		printf("%d,%d,%d,%d,%d,%d\n", pes[i].defendid_,pes[i].defendtype_, pes[i].cost_,
			pes[i].decrate_,pes[i].physicerec_,pes[i].magicrec_);
	}
	printf("DefendAttribute--------------------------\n");

	proxy.fini();
}

void add_OfficerRaceAttribute()
{
	int fd = file_init("OfficerRaceAttribute.csv");
	if (fd == -1) 
	{
		_fout << "找不到文件：OfficerRaceAttribute.csv\n";
	}

	char buf[1024000];
	int len = file_length(fd);
	if (file_read(fd, buf, len) == -1)
	{
		_fout << "读<OfficerRaceAttribute.csv>文件错误\n";
		file_fini(fd);
		return ;
	}
	buf[len] = '\0';
	file_fini(fd);

	DB_Proxy proxy;
	proxy.init(address, port);

	char * p1,*ptr = strstr(buf, "\r\n") + 2;
	while (1)
	{
		if (ptr[0] == '\0') break;

		OfficerRaceAttribute cc;
		memset(&cc, 0, sizeof(cc));
		if (sscanf(ptr, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d", &cc.raceid_,&cc.keepfee_,&cc.money_,&cc.food_,&cc.target_,
			&cc.evade_,&cc.hp_,&cc.soldier_,&cc.huntsman_,&cc.prowl_,&cc.churchman_,&cc.rabbi_,
			&cc.magician_,&cc.marksman_,&cc.trooper_) != 15)
		{
			_fout << "提取文件内容错误<OfficerRaceAttribute.csv>\n";
			break;
		}
		proxy.db_insert(cc);
		p1 = ptr;
		ptr = strstr(p1, "\r\n") + 2;
	}

	char * buff;
	int num;
	OfficerRaceAttribute es;
	proxy.db_select_all(es, buff, num);
	OfficerRaceAttribute * pes = (OfficerRaceAttribute *)buff;
	num = num / sizeof(es);
	printf("OfficerRaceAttribute--------------------------\n");
	for (int i = 0; i < num; i++)
	{
		printf("%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n", pes[i].raceid_,pes[i].keepfee_,pes[i].money_,pes[i].food_,pes[i].target_,
			pes[i].evade_,pes[i].hp_,pes[i].soldier_,pes[i].huntsman_,pes[i].prowl_,pes[i].churchman_,pes[i].rabbi_,
			pes[i].magician_,pes[i].marksman_,pes[i].trooper_);
	}
	printf("OfficerRaceAttribute--------------------------\n");

	proxy.fini();
}

void add_ProduceConfigure()
{
	int fd = file_init("ProduceConfigure.csv");
	if (fd == -1) 
	{
		_fout << "找不到文件：ProduceConfigure.csv\n";
	}

	char buf[1024000];
	int len = file_length(fd);
	if (file_read(fd, buf, len) == -1)
	{
		_fout << "读<ProduceConfigure.csv>文件错误\n";
		file_fini(fd);
		return ;
	}
	buf[len] = '\0';
	file_fini(fd);

	DB_Proxy proxy;
	proxy.init(address, port);

	char * p1,*ptr = strstr(buf, "\r\n") + 2;
	while (1)
	{
		if (ptr[0] == '\0') break;

		ProduceConfigure cc;

		memset(&cc, 0, sizeof(cc));
		if (sscanf(ptr, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%I64d", &cc.itemid_,&cc.weanpontypeid_,&cc.itemtype_,
			&cc.level_,&cc.foodcount_,&cc.woodcount_,&cc.ironcount_,&cc.skincount_,&cc.redstonecount_,&cc.yellowstonecount_,
			&cc.greenstonecount_,&cc.backironcount_,&cc.money_,&cc.science_,&cc.completetimes_) != 15)
		{
			_fout << "提取文件内容错误<ProduceConfigure.csv>\n";
			break;
		}
		proxy.db_insert(cc);
		p1 = ptr;
		ptr = strstr(p1, "\r\n") + 2;
	}

	char * buff;
	int num;
	ProduceConfigure es;
	proxy.db_select_all(es, buff, num);
	ProduceConfigure * pes = (ProduceConfigure *)buff;
	num = num / sizeof(es);
	printf("ProduceConfigure--------------------------\n");
	for (int i = 0; i < num; i++)
	{
		printf("%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%I64d\n", pes[i].itemid_,pes[i].weanpontypeid_,pes[i].itemtype_,
			pes[i].level_,pes[i].foodcount_,pes[i].woodcount_,pes[i].ironcount_,pes[i].skincount_,pes[i].redstonecount_,pes[i].yellowstonecount_,
			pes[i].greenstonecount_,pes[i].backironcount_,pes[i].money_,pes[i].science_,pes[i].completetimes_);
	}
	printf("ProduceConfigure--------------------------\n");

	proxy.fini();
}

void add_ResearchConfigure()
{
	int fd = file_init("ResearchConfigure.csv");
	if (fd == -1) 
	{
		_fout << "找不到文件：ResearchConfigure.csv\n";
	}

	char buf[1024000];
	int len = file_length(fd);
	if (file_read(fd, buf, len) == -1)
	{
		_fout << "读<ResearchConfigure.csv>文件错误\n";
		file_fini(fd);
		return ;
	}
	buf[len] = '\0';
	file_fini(fd);

	DB_Proxy proxy;
	proxy.init(address, port);

	char * p1,*ptr = strstr(buf, "\r\n") + 2;
	while (1)
	{
		if (ptr[0] == '\0') break;

		ResearchConfigure cc;

		memset(&cc, 0, sizeof(cc));
		if (sscanf(ptr, "%d,%d,%d,%d,%d,%I64d", &cc.itemid_,&cc.level_,
			&cc.type_,&cc.money_,&cc.science_,&cc.completetimes_) != 6)
		{
			_fout << "提取文件内容错误<ResearchConfigure.csv>\n";
			break;
		}
		proxy.db_insert(cc);
		p1 = ptr;
		ptr = strstr(p1, "\r\n") + 2;
	}

	char * buff;
	int num;
	ResearchConfigure es;
	proxy.db_select_all(es, buff, num);
	ResearchConfigure * pes = (ResearchConfigure *)buff;
	num = num / sizeof(es);
	printf("ResearchConfigure--------------------------\n");
	for (int i = 0; i < num; i++)
	{
		printf("%d,%d,%d,%d,%d,%I64d\n", pes[i].itemid_,pes[i].level_,
			pes[i].type_,pes[i].money_,pes[i].science_,pes[i].completetimes_);
	}
	printf("ResearchConfigure--------------------------\n");

	proxy.fini();
}


void add_SoldierParamTable()
{
	int fd = file_init("SoldierParamTable.csv");
	if (fd == -1) 
	{
		_fout << "找不到文件：SoldierParamTable.csv\n";
	}

	char buf[1024000];
	int len = file_length(fd);
	if (file_read(fd, buf, len) == -1)
	{
		_fout << "读<SoldierParamTable.csv>文件错误\n";
		file_fini(fd);
		return ;
	}
	buf[len] = '\0';
	file_fini(fd);

	DB_Proxy proxy;
	proxy.init(address, port);

	char * p1,*ptr = strstr(buf, "\r\n") + 2;
	while (1)
	{
		if (ptr[0] == '\0') break;

		SoldierParamTable cc;

		memset(&cc, 0, sizeof(cc));
		if (sscanf(ptr, "%d,%d,%d,%d,%d,%d,%d,%d,%d", &cc.soldiertype_,&cc.hp_,&cc.physicevade_,&cc.magicevade_,
			&cc.movefrequency_,&cc.visualrange_,&cc.attackscop_,&cc.duckrate_,&cc.shooting_) != 9)
		{
			_fout << "提取文件内容错误<SoldierParamTable.csv>\n";
			break;
		}
		proxy.db_insert(cc);
		p1 = ptr;
		ptr = strstr(p1, "\r\n") + 2;
	}

	char * buff;
	int num;
	SoldierParamTable es;
	proxy.db_select_all(es, buff, num);
	SoldierParamTable * pes = (SoldierParamTable *)buff;
	num = num / sizeof(es);
	printf("SoldierParamTable--------------------------\n");
	for (int i = 0; i < num; i++)
	{
		printf("%d,%d,%d,%d,%d,%d,%d,%d,%d\n", pes[i].soldiertype_,pes[i].hp_,pes[i].physicevade_,pes[i].magicevade_,
			pes[i].movefrequency_,pes[i].visualrange_,pes[i].attackscop_,pes[i].duckrate_,pes[i].shooting_);
	}
	printf("SoldierParamTable--------------------------\n");

	proxy.fini();
}

void add_WeaponAttribute()
{
	int fd = file_init("WeaponAttribute.csv");
	if (fd == -1) 
	{
		_fout << "找不到文件：WeaponAttribute.csv\n";
	}

	char buf[1024000];
	int len = file_length(fd);
	if (file_read(fd, buf, len) == -1)
	{
		_fout << "读<WeaponAttribute.csv>文件错误\n";
		file_fini(fd);
		return ;
	}
	buf[len] = '\0';
	file_fini(fd);

	DB_Proxy proxy;
	proxy.init(address, port);

	char * p1,*ptr = strstr(buf, "\r\n") + 2;
	while (1)
	{
		if (ptr[0] == '\0') break;

		WeaponAttribute cc;

		memset(&cc, 0, sizeof(cc));
		if (sscanf(ptr, "%d,%d,%d,%d,%d,%d,%d,%d", &cc.weaponid_,&cc.weapontypeid_,
			&cc.cost_,&cc.attack_,&cc.hurt_,&cc.singlehands_,&cc.soldier_,&cc.exp_) != 8)
		{
			_fout << "提取文件内容错误<WeaponAttribute.csv>\n";
			break;
		}
		proxy.db_insert(cc);
		p1 = ptr;
		ptr = strstr(p1, "\r\n") + 2;
	}

	char * buff;
	int num;
	WeaponAttribute es;
	proxy.db_select_all(es, buff, num);
	WeaponAttribute * pes = (WeaponAttribute *)buff;
	num = num / sizeof(es);
	printf("WeaponAttribute--------------------------\n");
	for (int i = 0; i < num; i++)
	{
		printf("%d,%d,%d,%d,%d,%d,%d,%d\n", pes[i].weaponid_,pes[i].weapontypeid_,
			pes[i].cost_,pes[i].attack_,pes[i].hurt_,pes[i].singlehands_,pes[i].soldier_, pes[i].exp_);
	}
	printf("WeaponAttribute--------------------------\n");

	proxy.fini();
}

void add_OfficerTech()
{
	int fd = file_init("OfficerTech.csv");
	if (fd == -1) 
	{
		_fout << "找不到文件：OfficerTech.csv\n";
	}

	char buf[1024000];
	int len = file_length(fd);
	if (file_read(fd, buf, len) == -1)
	{
		_fout << "读<OfficerTech.csv>文件错误\n";
		file_fini(fd);
		return ;
	}
	buf[len] = '\0';
	file_fini(fd);

	DB_Proxy proxy;
	proxy.init(address, port);

	char * p1,*ptr = strstr(buf, "\r\n") + 2;
	while (1)
	{
		if (ptr[0] == '\0') break;

		OfficerTech cc;

		memset(&cc, 0, sizeof(cc));
		if (sscanf(ptr, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d", &cc.techid_,&cc.techtype_,&cc.techkind_, &cc.career_,&cc.level_,
			&cc.odds_, &cc.effect_, &cc.object_,&cc.stamina_,&cc.power_,&cc.celerity_,&cc.wit_,&cc.techdot_) != 13)
		{
			_fout << "提取文件内容错误<OfficerTech.csv>\n";
			break;
		}
		proxy.db_insert(cc);
		p1 = ptr;
		ptr = strstr(p1, "\r\n") + 2;
	}

	char * buff;
	int num;
	OfficerTech es;
	proxy.db_select_all(es, buff, num);
	OfficerTech * pes = (OfficerTech *)buff;
	num = num / sizeof(es);
	printf("OfficerTech--------------------------\n");
	for (int i = 0; i < num; i++)
	{
		printf("%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n", pes[i].techid_,pes[i].techtype_,pes[i].techkind_,pes[i].career_,pes[i].level_,
			pes[i].odds_, pes[i].effect_, pes[i].object_,pes[i].stamina_,pes[i].power_,pes[i].celerity_,pes[i].wit_,pes[i].techdot_);
	}
	printf("OfficerTech--------------------------\n");

	proxy.fini();
}

void add_MapInfoTable()
{
	int fd = file_init("MapInfoTable.csv");
	if (fd == -1) 
	{
		_fout << "找不到文件：MapInfoTable.csv\n";
	}

	char buf[1024000];
	int len = file_length(fd);
	if (file_read(fd, buf, len) == -1)
	{
		_fout << "读<MapInfoTable.csv>文件错误\n";
		file_fini(fd);
		return ;
	}
	buf[len] = '\0';
	file_fini(fd);

	DB_Proxy proxy;
	proxy.init(address, port);

	char * p1,*ptr = strstr(buf, "\r\n") + 2;
	while (1)
	{
		if (ptr[0] == '\0') break;

		MapInfoTable cc;

		memset(&cc, 0, sizeof(cc));
		if (sscanf(ptr, "%d,%d,%d", &cc.countryid_,&cc.mapid_,&cc.state_) != 3)
		{
			_fout << "提取文件内容错误<MapInfoTable.csv>\n";
			break;
		}
		proxy.db_insert(cc);
		p1 = ptr;
		ptr = strstr(p1, "\r\n") + 2;
	}

	char * buff;
	int num;
	MapInfoTable es;
	proxy.db_select_all(es, buff, num);
	MapInfoTable * pes = (MapInfoTable *)buff;
	num = num / sizeof(es);
	printf("MapInfoTable--------------------------\n");
	for (int i = 0; i < num; i++)
	{
		printf("%d,%d,%d\n", pes[i].countryid_,pes[i].mapid_,pes[i].state_);
	}
	printf("MapInfoTable--------------------------\n");

	proxy.fini();
}

void add_ProductionAmount()
{
	int fd = file_init("ProductionAmount.csv");
	if (fd == -1) 
	{
		_fout << "找不到文件：ProductionAmount.csv\n";
	}

	char buf[1024000];
	int len = file_length(fd);
	if (file_read(fd, buf, len) == -1)
	{
		_fout << "读<ProductionAmount.csv>文件错误\n";
		file_fini(fd);
		return ;
	}
	buf[len] = '\0';
	file_fini(fd);

	DB_Proxy proxy;
	proxy.init(address, port);

	char * p1,*ptr = strstr(buf, "\r\n") + 2;
	while (1)
	{
		if (ptr[0] == '\0') break;

		ProductionAmount cc;

		memset(&cc, 0, sizeof(cc));
		if (sscanf(ptr, "%d,%d,%d,%d,%d,%d", &cc.productionid_, &cc.type_, &cc.mapid_, &cc.cityid_, &cc.playerid_, &cc.productioncount_) != 6)
		{
			_fout << "提取文件内容错误<ProductionAmount.csv>\n";
			break;
		}
		proxy.db_insert(cc);
		p1 = ptr;
		ptr = strstr(p1, "\r\n") + 2;
	}

	char * buff;
	int num;
	ProductionAmount es;
	proxy.db_select_all(es, buff, num);
	ProductionAmount * pes = (ProductionAmount *)buff;
	num = num / sizeof(es);
	printf("ProductionAmount--------------------------\n");
	for (int i = 0; i < num; i++)
	{
		printf("%d,%d,%d,%d,%d,%d\n", pes[i].productionid_, pes[i].type_, pes[i].mapid_, pes[i].cityid_, pes[i].playerid_, pes[i].productioncount_);
	}
	printf("ProductionAmount--------------------------\n");

	proxy.fini();
}

void add_EctypeBaseTable()
{
	int fd = file_init("EctypeBaseTable.csv");
	if (fd == -1) 
	{
		_fout << "找不到文件：EctypeBaseTable.csv\n";
	}

	char buf[1024000];
	int len = file_length(fd);
	if (file_read(fd, buf, len) == -1)
	{
		_fout << "读<EctypeBaseTable.csv>文件错误\n";
		file_fini(fd);
		return ;
	}
	buf[len] = '\0';
	file_fini(fd);

	DB_Proxy proxy;
	proxy.init(address, port);

	char * p1,*ptr = strstr(buf, "\r\n") + 2;
	while (1)
	{
		if (ptr[0] == '\0') break;

		EctypeBaseTable cc;

		memset(&cc, 0, sizeof(cc));
		if (sscanf(ptr, "%d,%d,%d,%[^,],%d", &cc.ectypeid_, &cc.level_, &cc.mapid_, &cc.name_, &cc.doorid_) != 5)
		{
			_fout << "提取文件内容错误<EctypeBaseTable.csv>\n";
			break;
		}
		proxy.db_insert(cc);
		p1 = ptr;
		ptr = strstr(p1, "\r\n") + 2;
	}

	char * buff;
	int num;
	EctypeBaseTable es;
	proxy.db_select_all(es, buff, num);
	EctypeBaseTable * pes = (EctypeBaseTable *)buff;
	num = num / sizeof(es);
	printf("EctypeBaseTable--------------------------\n");
	for (int i = 0; i < num; i++)
	{
		printf("%d,%d,%d,%s,%d\n", pes[i].ectypeid_, pes[i].level_, pes[i].mapid_, pes[i].name_, pes[i].doorid_);
	}
	printf("EctypeBaseTable--------------------------\n");

	proxy.fini();
}

void add_NPCArmyTable()
{
	int fd = file_init("NPCArmyTable.csv");
	if (fd == -1) 
	{
		_fout << "找不到文件：NPCArmyTable.csv\n";
	}

	char buf[1024000];
	int len = file_length(fd);
	if (file_read(fd, buf, len) == -1)
	{
		_fout << "读<NPCArmyTable.csv>文件错误\n";
		file_fini(fd);
		return ;
	}
	buf[len] = '\0';
	file_fini(fd);

	DB_Proxy proxy;
	proxy.init(address, port);

	char * p1,*ptr = strstr(buf, "\r\n") + 2;
	while (1)
	{
		if (ptr[0] == '\0') break;

		NPCArmyTable cc;

		memset(&cc, 0, sizeof(cc));
		if (sscanf(ptr, "%d,%[^,],%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d", 
			&cc.armyid_, &cc.armyname_, &cc.officerid_, &cc.playerid_, &cc.cityid_, 
			&cc.mapid_, &cc.soldiertype_, &cc.raceid_, &cc.firstcount_, &cc.fsoldierlevel_, 
			&cc.fweapon_, &cc.fsexp_, &cc.fweaponlevel_, &cc.fdefendtype_, 
			&cc.fdefendlevel_, &cc.state_, &cc.x, &cc.y, &cc.direction,&cc.dropitem_, &cc.dropodds_) != 21)
		{
			_fout << "提取文件内容错误<NPCArmyTable.csv>\n";
			break;
		}
		proxy.db_insert(cc);
		p1 = ptr;
		ptr = strstr(p1, "\r\n") + 2;
	}

	char * buff;
	int num;
	NPCArmyTable es;
	proxy.db_select_all(es, buff, num);
	NPCArmyTable * pes = (NPCArmyTable *)buff;
	num = num / sizeof(es);
	printf("NPCArmyTable--------------------------\n");

	for (int i = 0; i < num; i++)
	{
		printf("%d,%s,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n", pes[i].armyid_, 
			pes[i].armyname_, pes[i].officerid_, pes[i].playerid_, pes[i].cityid_, 
			pes[i].mapid_, pes[i].soldiertype_, pes[i].raceid_, pes[i].firstcount_, 
			pes[i].fsoldierlevel_, pes[i].fweapon_, pes[i].fsexp_, pes[i].fweaponlevel_,
			pes[i].fdefendtype_, pes[i].fdefendlevel_, pes[i].state_, pes[i].x, pes[i].y, pes[i].direction, pes[i].dropitem_, pes[i].dropodds_);
	}
	printf("NPCArmyTable--------------------------\n");

	proxy.fini();
}

void add_NPC()
{
	int fd = file_init("NPC.csv");
	if (fd == -1) 
	{
		_fout << "找不到文件：NPC.csv\n";
	}

	char buf[1024000];
	int len = file_length(fd);
	if (file_read(fd, buf, len) == -1)
	{
		_fout << "读<NPC.csv>文件错误\n";
		file_fini(fd);
		return ;
	}
	buf[len] = '\0';
	file_fini(fd);

	DB_Proxy proxy;
	proxy.init(address, port);

	char * p1,*ptr = strstr(buf, "\r\n") + 2;
	while (1)
	{
		if (ptr[0] == '\0') break;

		NPC cc;
		memset(&cc, 0, sizeof(cc));
		if (sscanf(ptr, "%d,%[^,],%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d", 
			&cc.npcid_,&cc.name_,&cc.level_,&cc.attack_,&cc.attackkind_,&cc.weapontype_, &cc.defend_,
			&cc.attackrange_,&cc.magdefend_,&cc.phsdefend_,&cc.hp_,
			&cc.movespeed_,&cc.sight_,&cc.dropitem_,
			&cc.dropresource_,&cc.exp_,&cc.skill_) != 17)
		{
			_fout << "提取文件内容错误<NPC.csv>\n";
			break;
		}
		proxy.db_insert(cc);
		p1 = ptr;
		ptr = strstr(p1, "\r\n") + 2;
	}

	char * buff;
	int num;
	NPC es;
	proxy.db_select_all(es, buff, num);
	NPC * pes = (NPC *)buff;
	num = num / sizeof(es);
	printf("NPC--------------------------\n");
	for (int i = 0; i < num; i++)
	{
		printf("%d,%s,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n", 
			pes[i].npcid_,pes[i].name_,pes[i].level_,pes[i].attack_,pes[i].attackkind_,pes[i].weapontype_,pes[i].defend_,
			pes[i].attackrange_,pes[i].magdefend_,pes[i].phsdefend_,pes[i].hp_,
			pes[i].movespeed_,pes[i].sight_,pes[i].dropitem_,
			pes[i].dropresource_,pes[i].exp_,pes[i].skill_);
	}
	printf("NPC--------------------------\n");

	proxy.fini();
}


void add_EquipAttribute()
{
	int fd = file_init("EquipAttribute.csv");
	if (fd == -1) 
	{
		_fout << "找不到文件：EquipAttribute.csv\n";
	}

	char buf[1024000];
	int len = file_length(fd);
	if (file_read(fd, buf, len) == -1)
	{
		_fout << "读<EquipAttribute.csv>文件错误\n";
		file_fini(fd);
		return ;
	}
	buf[len] = '\0';
	file_fini(fd);

	DB_Proxy proxy;
	proxy.init(address, port);

	char * p1,*ptr = strstr(buf, "\r\n") + 2;
	while (1)
	{
		if (ptr[0] == '\0') break;

		EquipAttribute cc;


		memset(&cc, 0, sizeof(cc));
		if (sscanf(ptr, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d", &cc.equipid_,&cc.level_,&cc.pos_,&cc.endurance_,
			&cc.str_,&cc.agi_,&cc.int_,&cc.attack_,&cc.attackrat_,&cc.defend_,&cc.basicprice_) != 11)
		{
			_fout << "提取文件内容错误<EquipAttribute.csv>\n";
			break;
		}
		proxy.db_insert(cc);
		p1 = ptr;
		ptr = strstr(p1, "\r\n") + 2;
	}

	char * buff;
	int num;
	EquipAttribute es;
	proxy.db_select_all(es, buff, num);
	EquipAttribute * pes = (EquipAttribute *)buff;
	num = num / sizeof(es);
	printf("EquipAttribute--------------------------\n");
	for (int i = 0; i < num; i++)
	{
		printf("%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n", 
			pes[i].equipid_,pes[i].level_,pes[i].pos_,pes[i].endurance_,
			pes[i].str_,pes[i].agi_,pes[i].int_,pes[i].attack_,
			pes[i].attackrat_,pes[i].defend_,pes[i].basicprice_);
	}
	printf("EquipAttribute--------------------------\n");

	proxy.fini();
}

void add_TaskBaseTable()
{
	int fd = file_init("TaskBaseTable.csv");
	if (fd == -1) 
	{
		_fout << "找不到文件：TaskBaseTable.csv\n";
	}

	char buf[1024000];
	int len = file_length(fd);
	if (file_read(fd, buf, len) == -1)
	{
		_fout << "读<TaskBaseTable.csv>文件错误\n";
		file_fini(fd);
		return ;
	}
	buf[len] = '\0';
	file_fini(fd);

	DB_Proxy proxy;
	proxy.init(address, port);

	char * p1,*ptr = strstr(buf, "\r\n") + 2;
	while (1)
	{
		if (ptr[0] == '\0') break;

		TaskBaseTable cc;

		memset(&cc, 0, sizeof(cc));
		if (sscanf(ptr, "%d,%[^,],%d,%d,%d,%d,%d,%d,%[^,],%[^,],%d", &cc.taskid_,&cc.taskname_,&cc.tasktype_,&cc.tasklevel_,
			&cc.playerlevel_,&cc.playerrank_,&cc.taskitem_,&cc.prvtaskid_,&cc.bonus_,&cc.contest_, &cc.mailtype_) != 11)
		{
			_fout << "提取文件内容错误<TaskBaseTable.csv>\n";
			break;
		}
		proxy.db_insert(cc);
		p1 = ptr;
		ptr = strstr(p1, "\r\n") + 2;
	}

	char * buff;
	int num;
	TaskBaseTable es;
	proxy.db_select_all(es, buff, num);
	TaskBaseTable * pes = (TaskBaseTable *)buff;
	num = num / sizeof(es);
	printf("TaskBaseTable--------------------------\n");

	for (int i = 0; i < num; i++)
	{
		printf("%d,%s,%d,%d,%d,%d,%d,%d,%s,%s,%d\n", pes[i].taskid_,pes[i].taskname_,pes[i].tasktype_,
			pes[i].tasklevel_,pes[i].playerlevel_,pes[i].playerrank_,pes[i].taskitem_,
			pes[i].prvtaskid_,pes[i].bonus_,pes[i].contest_,pes[i].mailtype_);
	}
	printf("TaskBaseTable--------------------------\n");

	proxy.fini();
}


void add_CreateConsortia()
{
	int fd = file_init("CreateConsortia.csv");
	if (fd == -1) 
	{
		_fout << "找不到文件：CreateConsortia.csv\n";
	}

	char buf[1024000];
	int len = file_length(fd);
	if (file_read(fd, buf, len) == -1)
	{
		_fout << "读<CreateConsortia.csv>文件错误\n";
		file_fini(fd);
		return ;
	}
	buf[len] = '\0';
	file_fini(fd);

	DB_Proxy proxy;
	proxy.init(address, port);

	char * p1,*ptr = strstr(buf, "\r\n") + 2;
	while (1)
	{
		if (ptr[0] == '\0') break;

		CreateConsortia cc;

		memset(&cc, 0, sizeof(cc));
		if (sscanf(ptr, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d", &cc.resid_, &cc.foodcount_, 
			&cc.woodcount_, &cc.ironcount_, &cc.skincount_, &cc.redstonecount_, &cc.yollowstonecount_, 
			&cc.greenstonecount_, &cc.backironcount_, &cc.times_, &cc.personnel_, &cc.science_, 
			&cc.money_, &cc.level_, &cc.joinlevel_) != 15)
		{
			_fout << "提取文件内容错误<CreateConsortia.csv>\n";
			break;
		}
		proxy.db_insert(cc);
		p1 = ptr;
		ptr = strstr(p1, "\r\n") + 2;
	}

	char * buff;
	int num;
	CreateConsortia es;
	proxy.db_select_all(es, buff, num);
	CreateConsortia * pes = (CreateConsortia *)buff;
	num = num / sizeof(es);
	printf("CreateConsortia--------------------------\n");

	for (int i = 0; i < num; i++)
	{
		printf("%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n", pes[i].resid_, pes[i].foodcount_, 
			pes[i].woodcount_, pes[i].ironcount_, pes[i].skincount_, 
			pes[i].redstonecount_, pes[i].yollowstonecount_, pes[i].greenstonecount_, 
			pes[i].backironcount_, pes[i].times_, pes[i].personnel_, pes[i].science_, 
			pes[i].money_, pes[i].level_, pes[i].joinlevel_);
	}
	printf("CreateConsortia--------------------------\n");

	proxy.fini();
}

void add_ConsortiaOfficial()
{
	int fd = file_init("ConsortiaOfficial.csv");
	if (fd == -1) 
	{
		_fout << "找不到文件：ConsortiaOfficial.csv\n";
	}

	char buf[1024000];
	int len = file_length(fd);
	if (file_read(fd, buf, len) == -1)
	{
		_fout << "读<ConsortiaOfficial.csv>文件错误\n";
		file_fini(fd);
		return ;
	}
	buf[len] = '\0';
	file_fini(fd);

	DB_Proxy proxy;
	proxy.init(address, port);

	char * p1,*ptr = strstr(buf, "\r\n") + 2;
	while (1)
	{
		if (ptr[0] == '\0') break;

		ConsortiaOfficial cc;

		memset(&cc, 0, sizeof(cc));
		if (sscanf(ptr, "%d,%[^,],%d,%d", &cc.officialid_,&cc.officialname_,&cc.level_,&cc.purview_) != 4)
		{
			_fout << "提取文件内容错误<ConsortiaOfficial.csv>\n";
			break;
		}
		proxy.db_insert(cc);
		p1 = ptr;
		ptr = strstr(p1, "\r\n") + 2;
	}

	char * buff;
	int num;
	ConsortiaOfficial es;
	proxy.db_select_all(es, buff, num);
	ConsortiaOfficial * pes = (ConsortiaOfficial *)buff;
	num = num / sizeof(es);
	printf("ConsortiaOfficial--------------------------\n");

	for (int i = 0; i < num; i++)
	{
		printf("%d,%s,%d,%d\n", pes[i].officialid_, 
			pes[i].officialname_, pes[i].level_, pes[i].purview_);
	}
	printf("ConsortiaOfficial--------------------------\n");

	proxy.fini();
}

void add_ArmyTable()
{
	int fd = file_init("ArmyTable.csv");
	if (fd == -1) 
	{
		_fout << "找不到文件：ArmyTable.csv\n";
	}

	char buf[1024000];
	int len = file_length(fd);
	if (file_read(fd, buf, len) == -1)
	{
		_fout << "读<ArmyTable.csv>文件错误\n";
		file_fini(fd);
		return ;
	}
	buf[len] = '\0';
	file_fini(fd);

	DB_Proxy proxy;
	proxy.init(address, port);

	char * p1,*ptr = strstr(buf, "\r\n") + 2;
	while (1)
	{
		if (ptr[0] == '\0') break;

		ArmyTable cc;
		cc.countryid_ = 0;

		memset(&cc, 0, sizeof(cc));
		if (sscanf(ptr, "%d,%[^,],%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d", 
			&cc.armyid_, &cc.armyname_, &cc.officerid_, &cc.playerid_, &cc.cityid_, 
			&cc.mapid_, &cc.soldiertype_, &cc.raceid_, &cc.firstcount_, &cc.fsoldierlevel_, 
			&cc.fweapon_, &cc.fsexp_, &cc.fweaponlevel_, &cc.fdefendtype_, 
			&cc.fdefendlevel_, &cc.state_, &cc.x, &cc.y, &cc.direction) != 19)
		{
			_fout << "提取文件内容错误<ArmyTable.csv>\n";
			break;
		}
		proxy.db_insert(cc);
		p1 = ptr;
		ptr = strstr(p1, "\r\n") + 2;
	}

	char * buff;
	int num;
	ArmyTable es;
	proxy.db_select_all(es, buff, num);
	ArmyTable * pes = (ArmyTable *)buff;
	num = num / sizeof(es);
	printf("ArmyTable--------------------------\n");

	for (int i = 0; i < num; i++)
	{
		printf("%d,%s,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n", pes[i].armyid_, 
			pes[i].armyname_, pes[i].officerid_, pes[i].playerid_, pes[i].cityid_, 
			pes[i].mapid_, pes[i].soldiertype_, pes[i].raceid_, pes[i].firstcount_, 
			pes[i].fsoldierlevel_, pes[i].fweapon_, pes[i].fsexp_, pes[i].fweaponlevel_,
			pes[i].fdefendtype_, pes[i].fdefendlevel_, pes[i].state_, pes[i].x, pes[i].y, pes[i].direction);
	}
	printf("ArmyTable--------------------------\n");

	proxy.fini();
}

void add_PlayerOfficerTable()
{
	int fd = file_init("PlayerOfficerTable.csv");
	if (fd == -1) 
	{
		_fout << "找不到文件：PlayerOfficerTable.csv\n";
	}

	char buf[1024000];
	int len = file_length(fd);
	if (file_read(fd, buf, len) == -1)
	{
		_fout << "读<PlayerOfficerTable.csv>文件错误\n";
		file_fini(fd);
		return ;
	}
	buf[len] = '\0';
	file_fini(fd);

	DB_Proxy proxy;
	proxy.init(address, port);

	char * p1,*ptr = strstr(buf, "\r\n") + 2;
	while (1)
	{
		if (ptr[0] == '\0') break;

		PlayerOfficerTable cc;
		memset(&cc, 0, sizeof(cc));
		if (sscanf(ptr, "%d,%d,%[^,],%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d", 
	&cc.officerid_,&cc.playerid_,&cc.officername_,&cc.officerrace_,&cc.level_,&cc.militaryrank_,&cc.jobid_,&cc.faceid_,&cc.exp_,&cc.credit_,&cc.enemycount_,&cc.generalcount_,
	&cc.mapid_,&cc.cityid_,&cc.armyid_,&cc.state_,&cc.tech1id_,&cc.tech1level_,&cc.tech2id_,&cc.tech2level_,&cc.tech3id_,&cc.tech3level_,&cc.tech4id_,&cc.tech4level_,
	&cc.stamina_,&cc.power_,&cc.celerity_,&cc.wit_,&cc.techdot_,&cc.attack_,&cc.physicrecovery_,&cc.magicrecovery_,&cc.actiondot_,&cc.updatetime_,&cc.gbhtime_,
	&cc.levelstamina_,&cc.levelpower_,&cc.levelcelerity_,&cc.levelwit_,&cc.equipstamina_,&cc.equippower_,&cc.equipcelerity_,&cc.equipwit_,&cc.magicattack_,
	&cc.herohp_,&cc.injurerate_,&cc.capturerate_,&cc.beinjurerate_,&cc.becapturerate_,&cc.forcerate_) != 50)
		{
			_fout << "提取文件内容错误<PlayerOfficerTable.csv>\n";
			break;
		}
		proxy.db_insert(cc);
		p1 = ptr;
		ptr = strstr(p1, "\r\n") + 2;
	}

	char * buff;
	int num;
	PlayerOfficerTable es;
	proxy.db_select_all(es, buff, num);
	PlayerOfficerTable * pes = (PlayerOfficerTable *)buff;
	num = num / sizeof(es);
	printf("PlayerOfficerTable--------------------------\n");

	for (int i = 0; i < num; i++)
	{
		printf("%d,%d,%s,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n", 
	pes[i].officerid_,pes[i].playerid_,pes[i].officername_,pes[i].officerrace_,pes[i].level_,pes[i].militaryrank_,pes[i].jobid_,pes[i].faceid_,pes[i].exp_,pes[i].credit_,pes[i].enemycount_,pes[i].generalcount_,
	pes[i].mapid_,pes[i].cityid_,pes[i].armyid_,pes[i].state_,pes[i].tech1id_,pes[i].tech1level_,pes[i].tech2id_,pes[i].tech2level_,pes[i].tech3id_,pes[i].tech3level_,pes[i].tech4id_,pes[i].tech4level_,
	pes[i].stamina_,pes[i].power_,pes[i].celerity_,pes[i].wit_,pes[i].techdot_,pes[i].attack_,pes[i].physicrecovery_,pes[i].magicrecovery_,pes[i].actiondot_,pes[i].updatetime_,pes[i].gbhtime_,
	pes[i].levelstamina_,pes[i].levelpower_,pes[i].levelcelerity_,pes[i].levelwit_,pes[i].equipstamina_,pes[i].equippower_,pes[i].equipcelerity_,pes[i].equipwit_,pes[i].magicattack_,
	pes[i].herohp_,pes[i].injurerate_,pes[i].capturerate_,pes[i].beinjurerate_,pes[i].becapturerate_,pes[i].forcerate_);
	}
	printf("PlayerOfficerTable--------------------------\n");

	proxy.fini();
}

void add_PublicBuildParamTable()
{
	int fd = file_init("PublicBuildParamTable.csv");
	if (fd == -1) 
	{
		_fout << "找不到文件：PublicBuildParamTable.csv\n";
	}

	char buf[1024000];
	int len = file_length(fd);
	if (file_read(fd, buf, len) == -1)
	{
		_fout << "读<PublicBuildParamTable.csv>文件错误\n";
		file_fini(fd);
		return ;
	}
	buf[len] = '\0';
	file_fini(fd);

	DB_Proxy proxy;
	proxy.init(address, port);

	char * p1,*ptr = strstr(buf, "\r\n") + 2;
	while (1)
	{
		if (ptr[0] == '\0') break;

		PublicBuildParamTable cc;
		memset(&cc, 0, sizeof(cc));
		if (sscanf(ptr, "%d,%[^,],%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d", 
			&cc.pbuildingkind_, &cc.name_, &cc.level_, &cc.templateid_, &cc.attack_, 
			&cc.attackkind_, &cc.upgradefee_, &cc.foodcount_, &cc.woodcount_, 
			&cc.ironcount_, &cc.skincount_, &cc.attackscop_, &cc.visualrange_, 
			&cc.denfenid_, &cc.hp_, &cc.exp_, &cc.skill_, &cc.skillodds_, &cc.output_) != 19)
		{
			_fout << "提取文件内容错误<PublicBuildParamTable.csv>\n";
			break;
		}
		proxy.db_insert(cc);
		p1 = ptr;
		ptr = strstr(p1, "\r\n") + 2;
	}

	char * buff;
	int num;
	PublicBuildParamTable es;
	proxy.db_select_all(es, buff, num);
	PublicBuildParamTable * pes = (PublicBuildParamTable *)buff;
	num = num / sizeof(es);
	printf("PublicBuildParamTable--------------------------\n");

	for (int i = 0; i < num; i++)
	{
		printf("%d,%s,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n", 
			pes[i].pbuildingkind_, pes[i].name_, pes[i].level_, pes[i].templateid_, 
			pes[i].attack_, pes[i].attackkind_, pes[i].upgradefee_, pes[i].foodcount_, 
			pes[i].woodcount_, pes[i].ironcount_, pes[i].skincount_, pes[i].attackscop_, 
			pes[i].visualrange_, pes[i].denfenid_, pes[i].hp_, pes[i].exp_, pes[i].skill_, pes[i].skillodds_, pes[i].output_);
	}
	printf("PublicBuildParamTable--------------------------\n");

	proxy.fini();
}

void add_PublicBuildTable()
{
	int fd = file_init("PublicBuildTable.csv");
	if (fd == -1) 
	{
		_fout << "找不到文件：PublicBuildTable.csv\n";
	}

	char buf[1024000];
	int len = file_length(fd);
	if (file_read(fd, buf, len) == -1)
	{
		_fout << "读<PublicBuildTable.csv>文件错误\n";
		file_fini(fd);
		return ;
	}
	buf[len] = '\0';
	file_fini(fd);

	DB_Proxy proxy;
	proxy.init(address, port);

	char * p1,*ptr = strstr(buf, "\r\n") + 2;
	while (1)
	{
		if (ptr[0] == '\0') break;

		PublicBuildTable cc;
		memset(&cc, 0, sizeof(cc));
		if (sscanf(ptr, "%d,%d,%d,%d,%d,%d", 
			&cc.mapid_, &cc.pbuildingid_, &cc.pbuildingkind_, 
			&cc.neutraltrap_, &cc.currentlevel_, &cc.currentmoney_) != 6)
		{
			_fout << "提取文件内容错误<PublicBuildTable.csv>\n";
			break;
		}
		proxy.db_insert(cc);
		p1 = ptr;
		ptr = strstr(p1, "\r\n") + 2;
	}

	char * buff;
	int num;
	PublicBuildTable es;
	proxy.db_select_all(es, buff, num);
	PublicBuildTable * pes = (PublicBuildTable *)buff;
	num = num / sizeof(es);
	printf("PublicBuildTable--------------------------\n");

	for (int i = 0; i < num; i++)
	{
		printf("%d,%d,%d,%d,%d,%d\n", pes[i].mapid_, pes[i].pbuildingid_, pes[i].pbuildingkind_, 
			pes[i].neutraltrap_, pes[i].currentlevel_, pes[i].currentmoney_);
	}
	printf("PublicBuildTable--------------------------\n");

	proxy.fini();
}

void add_JobParam()
{
	int fd = file_init("JobParam.csv");
	if (fd == -1) 
	{
		_fout << "找不到文件：JobParam.csv\n";
	}

	char buf[1024000];
	int len = file_length(fd);
	if (file_read(fd, buf, len) == -1)
	{
		_fout << "读<JobParam.csv>文件错误\n";
		file_fini(fd);
		return ;
	}
	buf[len] = '\0';
	file_fini(fd);

	DB_Proxy proxy;
	proxy.init(address, port);

	char * p1,*ptr = strstr(buf, "\r\n") + 2;
	while (1)
	{
		if (ptr[0] == '\0') break;

		JobParam cc;
		memset(&cc, 0, sizeof(cc));

		if (sscanf(ptr, "%d,%d,%d,%d,%d", 
			&cc.heroraceid_, &cc.endurance_, &cc.strength_, 
			&cc.arility_, &cc.intelligence_) != 5)
		{
			_fout << "提取文件内容错误<JobParam.csv>\n";
			break;
		}
		proxy.db_insert(cc);
		p1 = ptr;
		ptr = strstr(p1, "\r\n") + 2;
	}

	char * buff;
	int num;
	JobParam es;
	proxy.db_select_all(es, buff, num);
	JobParam * pes = (JobParam *)buff;
	num = num / sizeof(es);
	printf("JobParam--------------------------\n");

	for (int i = 0; i < num; i++)
	{
		printf("%d,%d,%d,%d,%d\n", pes[i].endurance_, pes[i].strength_, 
			pes[i].arility_, pes[i].intelligence_);
	}
	printf("JobParam--------------------------\n");

	proxy.fini();
}

void add_CardConfig()
{
	int fd = file_init("CardConfig.csv");
	if (fd == -1) 
	{
		_fout << "找不到文件：CardConfig.csv\n";
	}

	char buf[1024000];
	int len = file_length(fd);
	if (file_read(fd, buf, len) == -1)
	{
		_fout << "读<CardConfig.csv>文件错误\n";
		file_fini(fd);
		return ;
	}
	buf[len] = '\0';
	file_fini(fd);

	DB_Proxy proxy;
	proxy.init(address, port);

	char * p1,*ptr = strstr(buf, "\r\n") + 2;
	while (1)
	{
		if (ptr[0] == '\0') break;

		CardConfig cc;
		memset(&cc, 0, sizeof(cc));

		if (sscanf(ptr, "%d,%[^,],%d,%d,%d,%d,%d", 
			&cc.itemid_, &cc.itemname_, &cc.type_, &cc.price_, &cc.value_, &cc.time_, &cc.pos_) != 7)
		{
			_fout << "提取文件内容错误<CardConfig.csv>\n";
			break;
		}
		proxy.db_insert(cc);
		p1 = ptr;
		ptr = strstr(p1, "\r\n") + 2;
	}

	char * buff;
	int num;
	CardConfig es;
	proxy.db_select_all(es, buff, num);
	CardConfig * pes = (CardConfig *)buff;
	num = num / sizeof(es);
	printf("CardConfig--------------------------\n");

	for (int i = 0; i < num; i++)
	{
		printf("%d,%s,%d,%d,%d,%d,%d\n", pes[i].itemid_, pes[i].itemname_, 
			pes[i].type_, pes[i].price_, pes[i].value_, pes[i].time_, pes[i].pos_);
	}
	printf("CardConfig--------------------------\n");

	proxy.fini();
}

void add_BinConfig()
{
	int fd = file_init("BinConfig.csv");
	if (fd == -1) 
	{
		_fout << "找不到文件：BinConfig.csv\n";
	}

	char buf[1024000];
	int len = file_length(fd);
	if (file_read(fd, buf, len) == -1)
	{
		_fout << "读<BinConfig.csv>文件错误\n";
		file_fini(fd);
		return ;
	}
	buf[len] = '\0';
	file_fini(fd);

	DB_Proxy proxy;
	proxy.init(address, port);

	char * p1,*ptr = strstr(buf, "\r\n") + 2;
	while (1)
	{
		if (ptr[0] == '\0') break;

		BinConfig cc;


		memset(&cc, 0, sizeof(cc));
		if (sscanf(ptr, "%d,%[^,],%[^,],%s", &cc.binid_, &cc.sureflopitem_, &cc.randomflopitem_, &cc.randomflopodds_) != 4)
		{
			_fout << "提取文件内容错误<BinConfig.csv>\n";
			break;
		}
		proxy.db_insert(cc);
		p1 = ptr;
		ptr = strstr(p1, "\r\n") + 2;
	}

	char * buff;
	int num;
	BinConfig es;
	proxy.db_select_all(es, buff, num);
	BinConfig * pes = (BinConfig *)buff;
	num = num / sizeof(es);
	printf("BinConfig--------------------------\n");

	for (int i = 0; i < num; i++)
	{
		printf("%d,%s,%s,%s\n", pes[i].binid_, pes[i].sureflopitem_, pes[i].randomflopitem_, pes[i].randomflopodds_);
	}
	printf("BinConfig--------------------------\n");

	proxy.fini();
}

void add_EquipAntagonism()
{
	int fd = file_init("EquipAntagonism.csv");
	if (fd == -1) 
	{
		_fout << "找不到文件：EquipAntagonism.csv\n";
	}

	char buf[1024000];
	int len = file_length(fd);
	if (file_read(fd, buf, len) == -1)
	{
		_fout << "读<EquipAntagonism.csv>文件错误\n";
		file_fini(fd);
		return ;
	}
	buf[len] = '\0';
	file_fini(fd);

	DB_Proxy proxy;
	proxy.init(address, port);

	char * p1,*ptr = strstr(buf, "\r\n") + 2;
	while (1)
	{
		if (ptr[0] == '\0') break;

		EquipAntagonism cc;
		memset(&cc, 0, sizeof(cc));
		if (sscanf(ptr, "%d,%d,%d", &cc.weaponid_,&cc.defendid_,&cc.attackodds_) != 3)
		{
			_fout << "提取文件内容错误<EquipAntagonism.csv>\n";
			break;
		}
		proxy.db_insert(cc);
		p1 = ptr;
		ptr = strstr(p1, "\r\n") + 2;
	}

	char * buff;
	int num;
	EquipAntagonism es;
	proxy.db_select_all(es, buff, num);
	EquipAntagonism * pes = (EquipAntagonism *)buff;
	num = num / sizeof(es);
	printf("EquipAntagonism--------------------------\n");

	for (int i = 0; i < num; i++)
	{
		printf("%d,%d,%d\n", pes[i].weaponid_,pes[i].defendid_,pes[i].attackodds_);
	}
	printf("EquipAntagonism--------------------------\n");

	proxy.fini();
}

void add_BatttleField()
{
	int fd = file_init("BatttleField.csv");
	if (fd == -1) 
	{
		_fout << "找不到文件：BatttleField.csv\n";
	}

	char buf[1024000];
	int len = file_length(fd);
	if (file_read(fd, buf, len) == -1)
	{
		_fout << "读<BatttleField.csv>文件错误\n";
		file_fini(fd);
		return ;
	}
	buf[len] = '\0';
	file_fini(fd);

	DB_Proxy proxy;
	proxy.init(address, port);

	char * p1,*ptr = strstr(buf, "\r\n") + 2;
	while (1)
	{
		if (ptr[0] == '\0') break;

		BatttleField cc;
		memset(&cc, 0, sizeof(cc));

		if (sscanf(ptr, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d", &cc.id_, &cc.mapid_, &cc.starth_, &cc.startm_, 
			&cc.starts_, &cc.endh_, &cc.endm_, &cc.ends_, &cc.armyquantity_, 
			&cc.killbonus_, &cc.deathbonus_, &cc.vipquantity_) != 12)
		{
			_fout << "提取文件内容错误<BatttleField.csv>\n";
			break;
		}
		proxy.db_insert(cc);
		p1 = ptr;
		ptr = strstr(p1, "\r\n") + 2;
	}

	char * buff;
	int num;
	BatttleField es;
	proxy.db_select_all(es, buff, num);
	BatttleField * pes = (BatttleField *)buff;
	num = num / sizeof(es);
	printf("BatttleField--------------------------\n");

	for (int i = 0; i < num; i++)
	{
		printf("%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n", pes[i].id_, pes[i].mapid_, pes[i].starth_, pes[i].startm_, 
			pes[i].starts_, pes[i].endh_, pes[i].endm_, pes[i].ends_, pes[i].armyquantity_, 
			pes[i].killbonus_, pes[i].deathbonus_, pes[i].vipquantity_);
	}
	printf("BatttleField--------------------------\n");

	proxy.fini();
}

void file_to_db_main()
{
	WSADATA wsaData;
	WSAStartup( 0x0202, &wsaData );

	add_BatttleField();
	add_EquipAntagonism();
	add_BinConfig();
	add_CardConfig();
	add_JobParam();
	add_PublicBuildParamTable();
	add_PublicBuildTable();
	add_ArmyTable();
	add_PlayerOfficerTable();
	add_CreateConsortia();
	add_ConsortiaOfficial();
	add_GameCityInfo();
	add_ConstructionConfigure();
	add_DefendAttribute();
	add_OfficerRaceAttribute();
	add_ProduceConfigure();
	add_ResearchConfigure();
	add_SoldierParamTable();
	add_WeaponAttribute();
	add_OfficerTech();
	add_MapInfoTable();
	add_ProductionAmount();
	add_EctypeBaseTable();
	add_NPCArmyTable();
	add_NPC();
	add_EquipAttribute();
	add_TaskBaseTable();

	WSACleanup();
}


#endif /*__INCLUDE_FILE_TO_DB_H__*/