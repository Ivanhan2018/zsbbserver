// ChatServer.cpp : 定义控制台应用程序的入口点。
//

#include <stdio.h>
#include "NetTask.h"
#include "ChatRoomManager.h"
#include "ChatUserManager.h"
#include "NewsReport.h"
#include "exception_handler.hpp"
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
	exception_handler::init_exception_handler(MDM_GP_MAP_CHAT);
	SetConsoleCtrlHandler(console_ctrl_handler, TRUE);
	printf("Start Chat System");
	if(!g_NetTask.InitTask())
		return 0;
	
	g_ChatUserManager = new CChatUserManager();
	g_ChatRoomManager = new CChatRoomManager;
	g_ChatUserManager->InitUserNameList();
	g_ChatUserManager->InitMapBelong();

	g_NewsReport.InitNews();
	

	cout<< "PLEASE CTRL + C FOR EXIT!" << endl;
	while(g_bIsExit == false){
		_sleep(100);
	}
	
	g_NetTask.UnInitTask();
	g_ChatRoomManager = NULL;
	delete g_ChatUserManager;

	return 0;
}

