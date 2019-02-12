// main.cpp : 定义控制台应用程序的入口点。
//

#include "NetProcesser.h"
#include <Windows.h>
#include <iostream>
#include <tchar.h>

#include "exception_handler.hpp"
#include "IniFile.h"
#include "socketsvc.hpp"
#include "DB_Proxy.h"

#include "Global_Macro.h"
#include "GameHallCommon.h"


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

int _tmain(int /*argc*/, _TCHAR* /*argv*/[])
{
 	exception_handler::init_exception_handler(MDM_GP_GAME_HALL_SERVER);
	SetConsoleCtrlHandler(console_ctrl_handler, TRUE);

	g_IniFile.ReadFile("serverconfig.ini");

#ifdef _WIN32
	CHAR str[20] = {0};
	sprintf(str,"%d", MDM_GP_GAME_HALL_SERVER);

	SetConsoleTitle(_T(str));
#endif

	DB_Proxy dbProxy;
	
	if (!dbProxy.init(g_IniFile.strDBIP.c_str(), (short)g_IniFile.m_DBPort, MDM_GP_GAME_HALL_SERVER))
	{
		cout<<"connect db fail"<<endl;
		return 0;
	}

	CLoadDBInfo loadDBinfo;
	loadDBinfo.LoadParam(&dbProxy);

	PlayerID2Info playId2Info;
	netprocesser_ = new NetProcesser(&loadDBinfo, &dbProxy, playId2Info, MDM_GP_GAME_HALL_SERVER);

	if (NULL == netprocesser_)
	{
		return 0;
	}

	netprocesser_->startTask();

	std::cout<< "PLEASE CTRL + C FOR EXIT!" << std::endl;

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	netprocesser_->initFieldEctypeRoom();
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	while(g_bIsExit == false)
	{
		Sleep(100);
	}

	netprocesser_->UnInitNetTask();
	dbProxy.fini();

	delete netprocesser_;
	netprocesser_ = NULL;

	return 0;
}

