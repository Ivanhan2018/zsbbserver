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
	exception_handler::init_exception_handler(MDM_GP_MAP_TASK);
	SetConsoleCtrlHandler(HandlerRoutine, TRUE);
	printf("Please enter Ctrl+C to exit ... \n");
	printf("Start TaskServer system( %d ) ... \n",MDM_GP_MAP_TASK);
	printf("Version 1.0.0.0 \n");

	if (!gNetTask.InitTask())
	{
		printf("Start TaskServer system Failed....\n");
		printf("run error. Please enter any key to exit ... \n");
		return;
	}
	else printf("Start TaskServer system Succeed(ALL) ... \n");

	while(g_bRun)
	{
		_sleep(500);
	}
	gNetTask.UnInitTask();
	return;
}