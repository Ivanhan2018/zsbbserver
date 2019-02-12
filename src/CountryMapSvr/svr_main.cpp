#include <stdio.h>
#include <stdlib.h>
#include "NetTask.h"
#include "IniFile.h"
#include "exception_handler.hpp"
#include "DB_ProxyHeader.h"
#include "LoadDBInfo.h"
#include "LoadMapInfo.h"
#pragma warning(disable:4100)
#pragma warning(disable:4996)
bool g_bIsExit = false;
BOOL WINAPI console_ctrl_handler(DWORD ctrl_type)
{
	switch (ctrl_type)
	{
	case CTRL_C_EVENT:
	case CTRL_BREAK_EVENT:
	case CTRL_CLOSE_EVENT:
	case CTRL_SHUTDOWN_EVENT:
		g_bIsExit = true;
		return TRUE;
	default:
		return FALSE;
	}
}

int main(int argc, char* argv[])
{
	cout<<"**************************************************************************"<<endl;
	cout<<"******        start map system ,Input Server ID from 1001 to 1250  *******"<<endl;
	cout<<"******        start npc system ,Input Server ID from 1251 to 1300  *******"<<endl;
	cout<<"**************************************************************************"<<endl;
	unsigned long mapid;
	if (argv[1]== NULL)
	{
		cin>>mapid;

		cout<<"Enter Map :  "<<mapid<<endl;
		while (mapid<1001 || mapid>2000)
		{
			cout<<"input error!!!"<<endl;
			cout<<"input again"<<endl;
			cin>>mapid;
			//return 0;

		}
	}
	else 
	{
		mapid =atoi(argv[1]);
	}
	exception_handler::init_exception_handler(mapid);
	SetConsoleCtrlHandler(console_ctrl_handler, TRUE);
	srand((unsigned int)IceUtil::Time::now().toSeconds());
	{
		g_IniFile.ReadFile("serverconfig.ini");
#ifdef _WIN32
		char str[20]="";
		sprintf(str,"%d",mapid);

		SetConsoleTitle(str);
#endif
		if (!g_Proxy.init(g_IniFile.strDBIP.c_str(),(short)g_IniFile.m_DBPort,mapid))
		{
			cout<<"connect db fail"<<endl;
			return 0;
		}
		//初始化
		{
			DB_Proxy dbProxy;
			if (!dbProxy.init(g_IniFile.strDBIP.c_str(),(short)g_IniFile.m_DBPort,mapid))
			{
				cout<<"connect db fail"<<endl;
				return 0;
			}
			
			if(!g_NetTask.InitNetTask(mapid))
				return 0;
			g_MapInfo = new CLoadMapInfo(mapid);
			MapInfoTable Mapinfo ;
			Mapinfo.setid(mapid);
			int ret = dbProxy.db_select(Mapinfo);
			if (ret == DB_Proxy::DB_SUCCEED)
			{
				g_MapInfo->SetMapOwer(Mapinfo.countryid_);
				g_MapInfo->SetMapState((mapstate)Mapinfo.state_);
			}
			g_LoadDBinfo.LoadParam(&dbProxy,mapid);	

			g_IFightProcesser = FightProcesserFactory::createFightManager(g_MapInfo,&g_LoadDBinfo,&g_IniFile);	
			if (g_IFightProcesser == NULL)
			{
				cout<<"g_IFightProcesser is NULL"<<endl;
				return 0;
			}
			g_DataServer.m_OtherManager.SetMapID(mapid,&dbProxy);
			g_DataServer.m_LevySoldier.SetMapID(mapid);
			g_DataServer.m_BuildProduce.SetMapID(mapid,&dbProxy);
			g_DataServer.m_MarketManager.SetMapID(mapid);//
			g_DataServer.m_ResearchManager.SetMapID(mapid);

			g_DataServer.InitInfo();
			g_NetTask.startTask();
			if (mapid != SERVER_BOUND_MAP_OTHER)
			{//非大地图
				/*StartFightEvent(&Fight , true); 
				ArmyCommandStartFight *pFight = new ArmyCommandStartFight;
				pFight->uiMapID = mapid;
				pFight->bFirst = true;
				*/
			}
			if (mapid == SERVER_BOUND_MAP_OTHER)
			{
				g_DataServer.m_OtherManager.InitArmyOut(mapid,&dbProxy);
				g_DataServer.m_OtherManager.ReadPlayerOfficerTable(&dbProxy);
				g_DataServer.m_OtherManager.ClearNPCArmyOfBigMap(&dbProxy);
			}
			else if (mapid == MDM_GP_MAP_RESEARCH)
			{
				g_DataServer.m_ResearchManager.InitResearchActivity(&dbProxy);
			}
			else if (mapid >1000 && mapid <=1060)
			{
				g_DataServer.m_LevySoldier.InitLevySoldierActivity(&dbProxy);
				g_DataServer.m_BuildProduce.InitProActivity(&dbProxy);
				g_DataServer.m_BuildProduce.InitConsBuildActivity(&dbProxy);
				//添加开启定时器 add by ljw 2009 12 03
				g_IFightProcesser->startTimerCycle(1500);
			}
			dbProxy.fini();
		}
		cout<< "PLEASE CTRL + C FOR EXIT!" << endl;
		while(g_bIsExit == false)
		{
			_sleep(100);
		}
		g_NetTask.UnInitNetTask();
		delete g_IFightProcesser;
		g_IFightProcesser = 0;
		delete g_MapInfo ;
		g_MapInfo = 0;

		g_Proxy.fini();
		g_SpeedCard.ClearPlayer();

	}
	return 0;
}
