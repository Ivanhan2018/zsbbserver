#include <stdio.h>
#include <stdlib.h>
#include <direct.h>
#include "exception_handler.hpp"
#include "CommonFunction.h"
#include "ChargeTask.h"
#include "ProcChargeMsg.h"

#pragma warning(disable:4100)
#pragma warning(disable:4244)
#pragma warning(disable:4706)
//#include "atom.hpp"
#define CFGFILE		".\\ChargeServer.ini"
CChargeTask g_ChargeTask;
IceUtil::Semaphore g_ExitSignal;
//回调函数
/***************************************************************************
// int 服务器ID
// const char * 接收的数据
// unsigned int 接收数据的长度
// 返回0
***************************************************************************/
int SOCKETSVC_API Event_Notify_CallBack(int server_id,const char * buffer,int length)
{
//#ifdef PRINT_HINTINFO
//	printf("Event_Notify_CallBack:%d\n",server_id);
//#endif
	char *buf = new char[length];
	memcpy(buf,buffer,length);
	if(!g_ChargeTask.putQueue(buf))
	{
		delete[] buf;
		buf = 0;
	}
	return 0;
}

/***************************************************************************
// int 代理服务器ID
// int 玩家ID
// const char * 接收的数据
// unsigned int 接收数据的长度
// 返回0
***************************************************************************/
int SOCKETSVC_API Data_Receiver_CallBack(int agent_id,int player_id, char * buffer,int length)
{
	char *buf = new char[length];
	memcpy(buf,buffer,length);
	if(!g_ChargeTask.putQueue(buf))
	{
		delete[] buf;
		buf = 0;
	}
	return 0;
}

/***************************************************************************
// const void * 设置定时器时指定的回传参数
// 返回0
***************************************************************************/
int SOCKETSVC_API Timer_Notify_CallBack(const void * param,const void * param1, long timer_id)
{
	//cancel_timer(timer_id);
	//g_ChargeTask.ProcTimer(param);
	if(!g_ChargeTask.putQueue((char*)param))
	{
		delete[] (char*)param;
	}
	return 0;
}

/***************************************************************************
// int 玩家ID
// 返回0
***************************************************************************/
void SOCKETSVC_API Socket_Disconnect(bool player_all,int agent_id,int server_id,int player_id)
{
	if(player_all)
	{
		printf("Agent has disconnected(AgentID:%d,ServerID:%d)\n",agent_id,server_id);
	}
	else
	{
	}
}

/***************************************************************************
// const void * 设置定时器时指定的回传参数
// 返回0
***************************************************************************/
void SOCKETSVC_API Logic_Exception(int code,int agent_id,int from_id,int to_id)
{
#ifndef PRESS_TEST
	printf("Exception(%s)\n, AgentID:%d,From:%d,To:%d\n",get_error_message(code),agent_id,from_id,to_id);
#endif
}
//======================================================
BOOL WINAPI HandlerRoutine(DWORD ctrl_type)
{
	switch (ctrl_type)
	{
	case CTRL_C_EVENT:
	case CTRL_BREAK_EVENT:
	case CTRL_CLOSE_EVENT:
	case CTRL_SHUTDOWN_EVENT:

		g_ExitSignal.post();
		return TRUE;
	default:
		return FALSE;
	}
}
//======================================================
int main(int argc,char *argv[])
{
	exception_handler::init_exception_handler(MDM_GP_MAP_CHARGE);
	SetConsoleCtrlHandler(HandlerRoutine, TRUE);
	//global_atom atom(MDM_GP_MAP_CHARGE);
	_mkdir(SAVEDATAPATH);

	{//读取配置
		CChargeTask::ServerParam param;
		char tmpBuf[SIZE_256] = {0};
		int nThread = 1;
		if(ReadConfig(CFGFILE,"System","LogPath",tmpBuf,SIZE_256))
		{
			g_ChargeTask.SetLogPath(".\\log");
		}
		else
		{
			g_ChargeTask.SetLogPath(tmpBuf);
		}
		_mkdir(g_ChargeTask.GetLogPath());
		if(ReadConfig(CFGFILE,"System","AgentIP",param.AgentIP,SIZE_32))
		{
			strcpy(param.AgentIP,"127.0.0.1");
		}

		if(ReadConfig(CFGFILE,"System","AgentPort",tmpBuf,SIZE_32) ==0)
		{
			param.AgentPort = atoi(tmpBuf);
		}
		else
		{
			param.AgentPort = 512;
		}
		if(ReadConfig(CFGFILE,"System","DBGIP",param.DBGIP,SIZE_32))
		{
			strcpy(param.DBGIP,"127.0.0.1");
		}
		if(ReadConfig(CFGFILE,"System","DBGPort",tmpBuf,SIZE_32) ==0)
		{
			param.DBGPort = atoi(tmpBuf);
		}
		else
		{
			param.DBGPort = 8899;
		}
		
		if(ReadConfig(CFGFILE,"System","WebIP",param.WebIP,SIZE_32))
		{
			strcpy(param.WebIP,"127.0.0.1");
		}
		if(ReadConfig(CFGFILE,"System","WebPort",tmpBuf,SIZE_32) ==0)
		{
			param.WebPort = atoi(tmpBuf);
		}
		else
		{
			param.WebPort = 500;
		}
		if(ReadConfig(CFGFILE,"System","LogServerIP",param.LogSvrIP,SIZE_32))
		{
			strcpy(param.LogSvrIP,"127.0.0.1");
		}
		if(ReadConfig(CFGFILE,"System","LogPort",tmpBuf,SIZE_32) ==0)
		{
			param.LogSvrPort = atoi(tmpBuf);
		}
		else
		{
			param.LogSvrPort = 3000;
		}
		if(ReadConfig(CFGFILE,"System","Timeout",tmpBuf,SIZE_32) ==0)
		{
			param.Timeout = atoi(tmpBuf);
		}
		else
		{
			param.Timeout = 500;
		}

		if(ReadConfig(CFGFILE,"System","ThreadCount",tmpBuf,SIZE_32) ==0)
		{
			nThread = atoi(tmpBuf);
		}
		else
		{
			nThread = 1;
		}

		if(ReadConfig(CFGFILE,"System","CheckTime",tmpBuf,SIZE_32) ==0)
		{
			g_ChargeTask.SetCheckResourceTime(atoi(tmpBuf));
		}
		else
		{
			g_ChargeTask.SetCheckResourceTime(900);
		}
		//读取兑换参数
		if(ReadConfig(CFGFILE,"System","GO2Money_Rate",tmpBuf,SIZE_32) ==0)
		{
			g_ChargeTask.SetExchageRate(GO2Money,atoi(tmpBuf));
		}
		else
		{
			g_ChargeTask.SetExchageRate(GO2Money,100000);
		}
		if(ReadConfig(CFGFILE,"System","Money2GO_Rate",tmpBuf,SIZE_32) ==0)
		{
			g_ChargeTask.SetExchageRate(Money2GO,atoi(tmpBuf));
		}
		else
		{
			g_ChargeTask.SetExchageRate(Money2GO,150000);
		}
		//读取日志配置文件
		if(ReadConfig(CFGFILE,"LogConfig","BuyCard",tmpBuf,SIZE_128) ==0)
		{
			g_ChargeTask.SetLogFormat(BuyCard_Log,tmpBuf);
		}
		if(ReadConfig(CFGFILE,"LogConfig","UseCard",tmpBuf,SIZE_128) ==0)
		{
			g_ChargeTask.SetLogFormat(UseCard_Log,tmpBuf);
		}
		if(ReadConfig(CFGFILE,"LogConfig","ImmedComplete",tmpBuf,SIZE_128) ==0)
		{
			g_ChargeTask.SetLogFormat(ImmedComplete_Log,tmpBuf);
		}
		if(ReadConfig(CFGFILE,"LogConfig","Election",tmpBuf,SIZE_128) ==0)
		{
			g_ChargeTask.SetLogFormat(Election_Log,tmpBuf);
		}
		if(ReadConfig(CFGFILE,"LogConfig","AddGODian",tmpBuf,SIZE_128) ==0)
		{
			g_ChargeTask.SetLogFormat(AddGODian_Log,tmpBuf);
		}
		if(ReadConfig(CFGFILE,"LogConfig","ImmedResourceRate",tmpBuf,SIZE_128) ==0)
		{
			g_ChargeTask.SetLogFormat(ImmedResourceRate_Log,tmpBuf);
		}
		if (ReadConfig(CFGFILE,"LogConfig","UpdateConsortia",tmpBuf,SIZE_128) == 0)
		{
			g_ChargeTask.SetLogFormat(UpdateConsortia_Log,tmpBuf);
		}
		g_ChargeTask.InitServer(&param);
		if(!g_ChargeTask.startTask(nThread,Data_Receiver_CallBack,Event_Notify_CallBack,Timer_Notify_CallBack,Socket_Disconnect,Logic_Exception))
		{
			printf("start server fault.\n");
			return 1;
		}
	} 
	
	printf("************************************************************\n");
	printf("* Program:  Charge server                                  *\n");
	printf("* Version:  V1.0.0                                         *\n");
	printf("************************************************************\n");
	printf("Please enter Ctrl+C key to exit \n");
	{//恢复上次的状态，包括定时器及托管玩家
	}

	g_ExitSignal.wait();
	g_ChargeTask.stopTask();
	g_ChargeTask.ClearParamData();
	return 0;
}
