#include <stdio.h>
#include <time.h>
#include "ConsortiaManage.h"
#include "CommonFunction.h"
#include "exception_handler.hpp"

#include "calendar.h"
CConsortiaManage g_ConsortiaManage;
bool g_bRun = true;
#ifdef PRESS_TEST
bool g_TestTime = true;
#endif
//回调函数
/***************************************************************************
// int 服务器ID
// const char * 接收的数据
// unsigned int 接收数据的长度
// 返回0
***************************************************************************/
int SOCKETSVC_API Event_Notify_CallBack(int server_id,const char * buffer,int length)
{
#ifdef PRINT_HINTINFO
	printf("Event_Notify_CallBack:%d\n",server_id);
#endif
	char *buf = new char[length];
	memcpy(buf,buffer,length);
	if(!g_ConsortiaManage.putQueue(buf))
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
	if(!g_ConsortiaManage.putQueue(buf))
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
int SOCKETSVC_API Timer_Notify_CallBack(const void * param, const void * param1, long timer_id)
{
	//cancel_timer(timer_id);
	//用于检测公会人数是否满10人
#ifdef PRINT_HINTINFO
	DISTRIBUTED_HEADER *head = (DISTRIBUTED_HEADER*)param;
	printf("timer notify(%d)\n",head->command);
#endif
	//g_ConsortiaManage.CheckConsortia();
	if(!g_ConsortiaManage.putQueue((char *const)param))
	{
		delete[] param;
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
#ifdef PRINT_HINTINFO
		printf("Client has disconnected(AgentID:%d,ServerID:%d,PlayerID:%d)\n",agent_id,server_id,player_id);
#endif
		g_ConsortiaManage.SetPlayerOnline(player_id,false,0);
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
#ifdef PRESS_TEST
		g_TestTime = false;
#else		
		g_bRun = false;
#endif
		
		return TRUE;
	default:
		return FALSE;
	}
}
//======================================================
int main(int argc,char* argv[])
{
	exception_handler::init_exception_handler(MDM_GP_MAP_CONSORTIA);
	//
	SetConsoleCtrlHandler(HandlerRoutine, TRUE);
	int iRankingTime = 900;
	{
		/*
		__int64 curtime = IceUtil::Time::now().toSeconds() - IceUtil::Time::Date(2000, 1, 1).toSeconds();
		game_calendar::calendar curgamedate(curtime);
		int year,mon,day,hour,min,sec;
		curgamedate.get_game_calendar(year,mon,day,hour,min,sec);
		printf("year:%d,mon:%d,day:%d\n",year,mon,day);*/
		int nThread = 100;
		char tmpBuf[SIZE_256] = {0};
		Agent_DB_Param param;
		char value[10] = {0};

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

		if(ReadConfig(CFGFILE,"System","ThreadCount",tmpBuf,SIZE_32) ==0)
		{
			nThread = atoi(tmpBuf);
		}
		else
		{
			nThread = 100;
		}

		if(ReadConfig(CFGFILE,"System","RankingTime",tmpBuf,SIZE_32) ==0)
		{
			iRankingTime = atoi(tmpBuf);
		}
		else
		{
			iRankingTime = 900;
		}

		if(ReadConfig(CFGFILE,"System","InsertCount",tmpBuf,SIZE_256) ==0)
		{
			g_ConsortiaManage.SetInsertRankingCount(atoi(tmpBuf));
		}
		else
		{
			g_ConsortiaManage.SetInsertRankingCount(20);
		}

		if(ReadConfig(CFGFILE,"System","CouncilNum",tmpBuf,SIZE_256) ==0)
		{
			if(GetValue(tmpBuf,value,0,';') == 0)
			{
				g_ConsortiaManage.SetMilitaryCount(atoi(value));
				_strset(value,10);
			}
			if(GetValue(tmpBuf,value,1,';') == 0)
			{
				g_ConsortiaManage.SetEconomyCount(atoi(value));
				_strset(value,10);
			}
			if(GetValue(tmpBuf,value,2,';') == 0)
			{
				g_ConsortiaManage.SetContributeCount(atoi(value));
				_strset(value,10);
			}
		}
		if(ReadConfig(CFGFILE,"System","VoteCouncilTime",tmpBuf,SIZE_256) ==0)
		{//4;20:20;24
			int week=4,hour=20,min = 0,holdhours = 30240,votehours = 1440,warningtimes = 1440;
			if(GetValue(tmpBuf,value,0,';') == 0)
			{
				week = atoi(value);
				_strset(value,10);
			}
			if(GetValue(tmpBuf,value,2,';') == 0)
			{
				holdhours = atoi(value);
				_strset(value,10);
			}
			if(GetValue(tmpBuf,value,3,';') == 0)
			{
				votehours = atoi(value);
				_strset(value,10);
			}
			if(GetValue(tmpBuf,value,4,';') == 0)
			{
				warningtimes = atoi(value);
				_strset(value,10);
			}
			if(GetValue(tmpBuf,value,1,';') == 0)
			{
				strcpy(tmpBuf,value);
				if(GetValue(tmpBuf,value,0,':') == 0)
				{
					hour = atoi(value);
					_strset(value,10);
				}
				if(GetValue(tmpBuf,value,1,':') == 0)
				{
					min = atoi(value);
					_strset(value,10);
				}
			}
			//printf("%d;%d:%d;%d\n",week,hour,min,hours);
			g_ConsortiaManage.SetVoteParam(week,hour,min,holdhours,votehours,warningtimes);
		}
		else
		{
			//assert(false);
			return 1;
		}

		if(ReadConfig(CFGFILE,"System","VoteFee",tmpBuf,SIZE_256) ==0)
		{
			int money = 100000,gomoney = 100;
			if(GetValue(tmpBuf,value,0,';') == 0)
			{
				money = atoi(value);
				_strset(value,10);
			}
			if(GetValue(tmpBuf,value,1,';') == 0)
			{
				gomoney = atoi(value);
				_strset(value,10);
			}
			g_ConsortiaManage.SetVoteFee(money,gomoney);
		}
		if(ReadConfig(CFGFILE,"System","DelateParam",tmpBuf,SIZE_256) ==0)
		{
			int underwrite = 12,delate = 12,num=5;
			if(GetValue(tmpBuf,value,0,';') == 0)
			{
				underwrite = atoi(value);
				_strset(value,10);
			}
			if(GetValue(tmpBuf,value,1,';') == 0)
			{
				delate = atoi(value);
				_strset(value,10);
			}
			if(GetValue(tmpBuf,value,2,';') == 0)
			{
				num = atoi(value);
				_strset(value,10);
			}
			g_ConsortiaManage.SetDelateParam(underwrite,delate,num);
		}

		if(ReadConfig(CFGFILE,"System","LanguageFile",tmpBuf,SIZE_256) !=0)
		{
			strcpy(tmpBuf,"Default.dat");
		}

		g_ConsortiaManage.SetRankingTimes(iRankingTime );
		if(!g_ConsortiaManage.InitConManage(&param,tmpBuf))
		{
			printf("Init server fault.\n");
			return 1;
		}
		else
		{
#ifdef PRINT_HINTINFO
			printf("InitConManage successful.\n");
#endif
		}
		/*
		*/
		if(!g_ConsortiaManage.Start(nThread,Data_Receiver_CallBack,Event_Notify_CallBack,Timer_Notify_CallBack,Socket_Disconnect,Logic_Exception))
		{
			printf("start server fault.\n");
			return 2;
		}
	}
	printf("************************************************************\n");
	printf("* Program:  ConsortiaManage server                         *\n");
	printf("* Version:  V1.0.0                                         *\n");
	printf("************************************************************\n");
	printf("ConsortiaManage server(%d) has started.\n",MDM_GP_MAP_CONSORTIA);
	printf("Please enter Ctrl+C key to exit \n");

	//IceUtil::Time time(g_ConsortiaManage.GetNextTime());
	//printf("%s\n",IceUtil::Time::seconds(g_ConsortiaManage.GetNextTime()).toDateTime().c_str());
	//启动排行线程

#ifdef PRESS_TEST
	long curvalue = 0;
	while(g_TestTime)
	{
		Sleep(1000);
		curvalue = InterlockedExchange(&g_ConsortiaManage.pro_count,0);
		printf("msg count:%-10d  %-10d---%d\n",curvalue,g_ConsortiaManage.TaskSize(),GetTickCount());
	}
#else
	while (g_bRun)
	{
		Sleep(1000);
	}
#endif
	g_ConsortiaManage.Stop();
	return 0;
}
