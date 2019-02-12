#ifndef __gmServerIniFile_H__
#define __gmServerIniFile_H__

#include  "IniFile.h"
#include <crtdbg.h>
#include <map>
#include "IceUtil.h"
#include "gmGlobalHead.h"
#include "log.hpp"
#pragma warning(disable:4244)
using namespace std;

//GM命令种类个数(常数)
enum emGMKeyCount{CmdCount=18};

//GM命令编号;编号对应的中文描述
struct tagKey
{
	short iKey;
	string cData;
	tagKey(){}
	tagKey(int key,const char * data)
	:iKey(key),cData(data){}
};

//命令字符串;对应的中文描述--保留
struct keyData
{
	string cCmd;
	string cData;
	keyData(){}
	keyData(const char * cmd,const char * data)
	:cCmd(cmd),cData(data){}
};

//系统配置参数
class CgmServerIniFile
{
private:
	CgmServerIniFile():alive_(false) {}
public:
	typedef map<string,tagKey> cmmap;
	typedef map<string,tagKey>::iterator cmit;

	~CgmServerIniFile() {}

	static CgmServerIniFile& Instance()
	{
		static CgmServerIniFile aa;
		return aa;
	}

	bool Alive() const 
	{
		return alive_;
	}

	void ReadData(const string& sfile)
	{
		inifile_.SetFileName(sfile);
		
		//数据库连接参数
		inifile_.setSection("config");
		dsn_=inifile_.readStr("dsn","LocalServer");
		user_=inifile_.readStr("user","sa");
		pwd_=inifile_.readStr("pwd","");

		//套接字服务参数
		inifile_.setSection("SocketSvc");
		sockip_=inifile_.readStr("ip","192.168.1.18");
		sockport_=inifile_.readInt("port",512);
		
		//DBG参数
		inifile_.setSection("DBG");
		dbgip_=inifile_.readStr("ip","192.168.1.18");
		dbgport_=inifile_.readInt("port",8899);
		
		//中心数据库
		inifile_.setSection("webserver");
		webip_=inifile_.readStr("ip","");
		webport_=inifile_.readInt("port",0);
		timeout_=inifile_.readInt("timeout",0);
		//日志数据库
		inifile_.setSection("LogServer");
		m_LogSvrIP=inifile_.readStr("ip","");
		m_LogSvrPort=inifile_.readInt("port",0);
		
		//势力公告参数
		inifile_.setSection("NoticeForces");
		strNotice_1 = inifile_.readStr("Notice_1"," ");
		strNotice_2 = inifile_.readStr("Notice_2"," ");
		
		//命令字参数
		mCommand_.clear();
		inifile_.setSection("GmCommand");
		mCommand_[inifile_.readStr("key_01","gmChat")]=tagKey(1,";GM私聊（保留）");
		mCommand_[inifile_.readStr("key_02","addMoney")]=tagKey(2,";添加金钱");
		mCommand_[inifile_.readStr("key_03","addTechnique")]=tagKey(3,";添加科技点");
		mCommand_[inifile_.readStr("key_04","experience")]=tagKey(4,";添加首领英雄经验值");
		mCommand_[inifile_.readStr("key_05","honors")]=tagKey(5,";添加首领英雄荣誉点");
		mCommand_[inifile_.readStr("key_06","clearSkill")]=tagKey(6,";清除首领英雄所有技能");
		mCommand_[inifile_.readStr("key_07","addGoods")]=tagKey(7,";添加仓库内指定物品");
		mCommand_[inifile_.readStr("key_08","addBackpackGoods")]=tagKey(8,";添加英雄背包内指定物品");
		mCommand_[inifile_.readStr("key_09","addArms")]=tagKey(9,";增加指定兵种数量");
		mCommand_[inifile_.readStr("key_10","addResearch")]=tagKey(10,";指定的科研项立即研究完成");
		mCommand_[inifile_.readStr("key_11","speakingBanned")]=tagKey(11,";禁言");
		mCommand_[inifile_.readStr("key_12","loginBanned")]=tagKey(12,";禁登录");
		mCommand_[inifile_.readStr("key_13","noticePost")]=tagKey(13,";普通公告(组播)");
		mCommand_[inifile_.readStr("key_14","noticeBroadcast")]=tagKey(14,";普通公告(广播)");
		mCommand_[inifile_.readStr("key_15","noticeForces")]=tagKey(15,";修改势力公告");
		mCommand_[inifile_.readStr("key_16","showNoticeforces")]=tagKey(16,";回显势力公告");
		mCommand_[inifile_.readStr("key_17","npc")]=tagKey(17,";NCP反击");
		mCommand_[inifile_.readStr("key_18","dnpc")]=tagKey(18,";删除NPC部队");
		
		PrintCommand();
		alive_=true;
	}

	void PrintCommand()
	{
		//_ASSERT(mCommand_.size()==CmdCount);
		LOG_TRACE_EX((mCommand_.size()==CmdCount),("gmServerIniFile PrintCommand()"));


		for(cmit it=mCommand_.begin();it!=mCommand_.end();it++)
		{
			cValue[it->second.iKey-1]=it;
		}
		printf("\n                  GmCommandSet size=%d\n",mCommand_.size());
		std::cout<<"*********************************************************\n";
		printf("%20s    %4s      %s\n","命令字","编号","功能描述");
		for (int i=0; i<CmdCount;i++)
		{
			printf("%20s    %4d      %s\n",cValue[i]->first.c_str(),cValue[i]->second.iKey,cValue[i]->second.cData.c_str());
		}
		std::cout<<"*********************************************************\n\n";
	}

	void WriteNotice(int fid,const string& notice)
	{
		//_ASSERT(fid==ctUnion || fid==ctTribal);
		LOG_TRACE_EX((fid==ctUnion || fid==ctTribal),("gmServerIniFile WriteNotice()\n"));
		
		IceUtil::RWRecMutex::WLock wlock(rwmutex);

		inifile_.setSection("NoticeForces");
		if (fid==ctUnion)
		{
			inifile_.write("Notice_1",notice);
			strNotice_1 = notice;
		}
		else
		{
			inifile_.write("Notice_2",notice);
			strNotice_2 = notice;
		}
	}

	const string getNotice(int fid)  
	{
		//_ASSERT(alive_);
		//_ASSERT(fid==ctUnion || fid==ctTribal);

		LOG_TRACE_EX(alive_,("gmServerIniFile getNotice()\n"));
		LOG_TRACE_EX((fid==ctUnion || fid==ctTribal),("gmServerIniFile getNotice()\n"));

		IceUtil::RWRecMutex::RLock rlock(rwmutex);
		if (fid==ctUnion) 
		{
			return strNotice_1;
		}
		return strNotice_2;
	}

	const string & Dsn()  const 
	{
		//_ASSERT(alive_);
		LOG_TRACE_EX(alive_,("gmServerIniFile Dsn()\n"));
		return dsn_;
	}

	const string & User()  const 
	{
		//_ASSERT(alive_);
		LOG_TRACE_EX(alive_,("gmServerIniFile User\n"));
		return user_;
	};

	const string & Pwd()  const
	{
		//_ASSERT(alive_);
		LOG_TRACE_EX(alive_,("gmServerIniFile Pwd\n"));
		return pwd_;
	}


	const string & SockIp()  const
	{
		//_ASSERT(alive_);
		LOG_TRACE_EX(alive_,("gmServerIniFile SockIp()\n"));
		return sockip_;
	}

	const string & DbgIp() const
	{
		//_ASSERT(alive_);
		LOG_TRACE_EX(alive_,("gmServerIniFile DbgIp()"));
		return dbgip_;
	}

	const int&  SockPort() const
	{
		//_ASSERT(alive_);
		LOG_TRACE_EX(alive_,("gmServerIniFile SockPort()\n"));
		return sockport_;
	}

	const int& DbgPort() const
	{
		//_ASSERT(alive_);
		LOG_TRACE_EX(alive_,("gmServerIniFile DbgPort()\n"));
		return dbgport_;
	}

	const string & WebIP() const
	{
		LOG_TRACE_EX(alive_,("gmServerIniFile WebIP()\n"));
		return webip_;
	}
	const int & WebPort() const
	{
		LOG_TRACE_EX(alive_,("gmServerIniFile WebPort()\n"));
		return webport_;
	}
	const int & TimeOut() const
	{
		LOG_TRACE_EX(alive_,("gmServerIniFile TimeOut()\n"));
		return timeout_;
	}
	const string & LogSvrIP() const
	{
		LOG_TRACE_EX(alive_,("gmServerIniFile LogSvrIP()\n"));
		return m_LogSvrIP;
	}
	const int & LogSvrPort() const
	{
		LOG_TRACE_EX(alive_,("gmServerIniFile LogSvrPort()\n"));
		return m_LogSvrPort;
	}
	//没找到返回 0
	const short FindByCmd(const string& svalue)
	{
		cmit aa =mCommand_.find(svalue);
		if (aa!=mCommand_.end()) 
			return aa->second.iKey;
		else
			return 0;
	}
	
	//同上
	const short FindByKey(const short key,string& scmd,string &sdata)
	{
		if (key>= 1 && key<=(CmdCount+1))
		{
			scmd=cValue[key-1]->first;
			sdata=cValue[key-1]->second.cData;
			return 1;
		}
		else  return 0;
	}
private:
	IceUtil::RWRecMutex rwmutex;
	bool alive_;	
	CIniFile inifile_;

	string sockip_;
	string dbgip_;
	string webip_;
	int   sockport_;
	int   dbgport_;
	int webport_;
	int timeout_;

	string dsn_;
	string user_;
	string pwd_;

	string strNotice_1;
	string strNotice_2;
	//key(命令字符串),value(命令编号，中文描述) 
	cmmap mCommand_;
	//下标（命令编号-1）, 命令字符串,中文描述
    cmit cValue[CmdCount];

	string m_LogSvrIP;
	int m_LogSvrPort;
};

#endif
