#include <stdio.h>
#include <stdlib.h>
#include "NetTask.h"
#include "exception_handler.hpp"

bool g_bRun = true;
//======================================================
BOOL WINAPI HandlerRoutine(DWORD ctrl_type)
{
	switch (ctrl_type)
	{
	case CTRL_C_EVENT:
	case CTRL_BREAK_EVENT:
	case CTRL_CLOSE_EVENT:
	case CTRL_SHUTDOWN_EVENT:

		g_bRun = false;
		return TRUE;
	default:
		return FALSE;
	}
}
void main(void)
{
	exception_handler::init_exception_handler(MDM_GP_GAME_MANAGE_SERVER);

	SetConsoleCtrlHandler(HandlerRoutine, TRUE);

	printf("Please enter Ctrl+C to exit ... \n");
	printf("Start GAME_MANAGE_SERVER(GM) system( %d ) ... \n",MDM_GP_GAME_MANAGE_SERVER);
	printf("Version 1.0.0.0 \n");

	if (!CNetTask::Instance().InitTask())
	{
		printf("Start GAME_MANAGE_SERVER(GM) system failed....\n");
		return;
	}
	else printf("Start GAME_MANAGE_SERVER(GM) system succeed(all) ... \n");

	while(g_bRun)
	{
		_sleep(500);
	}
	CNetTask::Instance().UnInitTask();
	return;
}