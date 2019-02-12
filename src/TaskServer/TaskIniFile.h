#ifndef __TaskIniFile_H__
#define __TaskIniFile_H__
#include  "IniFile.h"
#include <crtdbg.h>
#include "log.hpp"
using namespace std;


class CTaskIniFile
{
public:
	CTaskIniFile():alive_(false) {}
	~CTaskIniFile() {}

	void ReadData(const string& sfile)
	{
		inifile_.SetFileName(sfile);
		inifile_.setSection("SocketSvc");
		sockip_=inifile_.readStr("ip","127.0.0.1");
		sockport_=inifile_.readInt("port",512);
		inifile_.setSection("DBG");
		dbgip_=inifile_.readStr("ip","127.0.0.");
		dbgport_=inifile_.readInt("port",8899);
		inifile_.setSection("LogServer");
		logsvrip_=inifile_.readStr("ip","127.0.0.");
		logsvrport_=inifile_.readInt("port",3000);
		inifile_.setSection("System");
		gameGroupName = inifile_.readStr("GameGroupName","");
		std::string tmpStr = inifile_.readStr("ClearDayTaskTime","");
		if (tmpStr.length() <=3)
		{
			tmpStr = "3:30";
			inifile_.write("ClearDayTaskTime",tmpStr);
		}
		sscanf(tmpStr.c_str(),"%d:%d",&clearDayTaskHour,&clearDayTaskMin);
		alive_=true;
	}
	const string & SockIp() const 
	{
		//_ASSERT(alive_);
		LOG_TRACE_EX(alive_,("TaskIniFile SockIp()"));
		return sockip_;
	}
	const string & DbgIp() const
	{
		//_ASSERT(alive_);
		LOG_TRACE_EX(alive_,("TaskIniFile DbgIp()"));
		return dbgip_;
	}
	const int&  SockPort() const
	{
		//_ASSERT(alive_);
		LOG_TRACE_EX(alive_,("TaskIniFile SockPort()"));
		return sockport_;
	}
	const int& DbgPort() const
	{
		//_ASSERT(alive_);
		LOG_TRACE_EX(alive_,("TaskIniFile DbgPort()"));
		return dbgport_;
	}
	bool Alive() const 
	{
		return alive_;
	}
	const string &LogSvrIP() const
	{
		LOG_TRACE_EX(alive_,("TaskIniFile LogSvrIP()"));
		return logsvrip_;
	}
	const int& LogSvrPort() const
	{
		//_ASSERT(alive_);
		LOG_TRACE_EX(alive_,("TaskIniFile LogSvrPort()"));
		return logsvrport_;
	}
	const std::string & getGameGroupName() const
	{
		return gameGroupName;
	}
	int getClearDayTaskHour()
	{
		return clearDayTaskHour;
	}
	int getClearDayTaskMin()
	{
		return clearDayTaskMin;
	}

	__int64 getTaskEndtime(int taskid)
	{
		if (!alive_)
		{
			return 0;
		}
		inifile_.setSection("DayTaskParam");
		char key_buf[16] = {0};
		sprintf(key_buf,"%d",taskid);
		std::string tmpStr = inifile_.readStr(key_buf,"");
		__int64 endTime = 0;
		sscanf(tmpStr.c_str(),"%I64d;",&endTime);
		return endTime;
	}
	void writeTaskEndtime(int taskid,__int64 endtime)
	{
		if (!alive_)
		{
			return ;
		}
		inifile_.setSection("DayTaskParam");
		char key_buf[16] = {0};
		char value_buf[64] = {0};

		sprintf(key_buf,"%d",taskid);
		sprintf(value_buf,"%I64d;%s",endtime,IceUtil::Time::seconds(endtime).toDateTime().c_str());
		inifile_.write(key_buf,value_buf);
	}
private:
	string sockip_;
	string dbgip_;
	int   sockport_;
	int   dbgport_;
	CIniFile inifile_;
	bool alive_;
	string logsvrip_;
	int    logsvrport_;
	int clearDayTaskHour;
	int clearDayTaskMin;
	std::string gameGroupName;
};

#endif
