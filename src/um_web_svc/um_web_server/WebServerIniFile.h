#ifndef __WebServerIniFile_H__
#define __WebServerIniFile_H__

#include  "IniFile.h"
#include "header.hpp"
#include <crtdbg.h>
#include "aes_plus.h"
#include <sys/timeb.h>


#define OTL_STL
#define OTL_ODBC

#if defined (WIN32)
#define OTL_BIGINT __int64
#else
#define OTL_BIGINT long long
#endif


#include "otlv4.h"


#if defined(__BCPLUSPLUS__) || defined(_MSC_VER)
//
// On Windows, long is always 32-bit
//
typedef __int64 Int64;
#elif defined(ICE_64)
typedef long Int64;
#else
typedef long long Int64;
#endif


//
// ICE_INT64: macro for Int64 literal values
//
#if defined(__BCPLUSPLUS__) || defined(_MSC_VER)
#   define ICE_INT64(n) n##i64
#elif defined(ICE_64)
#   define ICE_INT64(n) n##L
#else
#   define ICE_INT64(n) n##LL
#endif


using namespace std;

class CWebIniFile
{
private:
	CWebIniFile():alive_(false) {}
public:
	~CWebIniFile() {}
	static CWebIniFile& Instance()
	{
		static CWebIniFile ww;
		return ww;
	}

	static Int64 GetTimeNow()
	{
		#ifdef _WIN32
		#  if defined(_MSC_VER)
			struct _timeb tb;
			_ftime(&tb);
		#  elif defined(__BCPLUSPLUS__)
			struct timeb tb;
			ftime(&tb);
		#  endif
			return ( static_cast<Int64>(tb.time) * ICE_INT64(1000000) + 
						tb.millitm * 1000 ) / 1000;
		#else
			struct timeval tv;
			gettimeofday(&tv, 0);
			return ( tv.tv_sec * ICE_INT64(1000000) + tv.tv_usec ) / 1000;
		#endif
	}

	static std::string toDatetime(Int64 sec)
	{
		time_t time = static_cast<long>(sec / 1000000);

		struct tm* t;
		#ifdef _WIN32
			t = localtime(&time);
		#else
			struct tm tr;
			localtime_r(&time, &tr);
			t = &tr;
		#endif

		char buf[32];
		strftime(buf, sizeof(buf), "%x %H:%M:%S", t);

		std::ostringstream os;
		os << buf << ".";
		os.fill('0');
		os.width(3);
		os << static_cast<long>(sec % 1000000 / 1000);
		return os.str();
	}


	void ReadData(const string& sfile)
	{
		inifile_.SetFileName(sfile);
		inifile_.setSection("config");
		dsn_=inifile_.readStr("dsn"," ");
		user_=inifile_.readStr("user"," ");
		pwd_=inifile_.readStr("pwd"," ");
		inifile_.setSection("server");
		port_=inifile_.readInt("port",0);

		inifile_.setSection("ConnLogicDB");

		logicip1_=inifile_.readStr("ip1"," ");
		logicgroup1_=inifile_.readStr("group1"," ");
		logicport1_=inifile_.readInt("port1",0);

		logicip2_=inifile_.readStr("ip2"," ");
		logicgroup2_=inifile_.readStr("group2"," ");
		logicport2_=inifile_.readInt("port2",0);

		logicip3_=inifile_.readStr("ip3"," ");
		logicgroup3_=inifile_.readStr("group3"," ");
		logicport3_=inifile_.readInt("port3",0);

		logicip4_=inifile_.readStr("ip4"," ");
		logicgroup4_=inifile_.readStr("group4"," ");
		logicport4_=inifile_.readInt("port4",0);

		timelen_=inifile_.readInt("timelen",3600);

		unsigned char key[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
		if  (c_name(set_key)(key, 16, both, &eak) != aes_good)
		{
			//printf("初始密钥错误\n");
//			LOG_NO_RETURN(false,("初始密钥错误\n"));
			return ;
		}
		alive_=true;
	}
   
	const aes& getAes()
	{
		return eak;
	}

	const string & dsn() const 
	{
		//_ASSERT(alive_);
		return dsn_;
	}

	const string & user() const
	{
		//_ASSERT(alive_);
		return user_;
	}

	const string&  pwd() const
	{
		//_ASSERT(alive_);
		return pwd_;
	}
	
	const short port() const 
	{
		//_ASSERT(alive_);
		return port_;
	}

	const string& LogicIP1() const { return logicip1_; }
	const string& LogicGroup1() const { return logicgroup1_; }
	const short LogicPort1() const { return logicport1_; }

	const string& LogicIP2() const { return logicip2_; }
	const string& LogicGroup2() const { return logicgroup2_; }
	const short LogicPort2() const { return logicport2_; }

	const string& LogicIP3() const { return logicip3_; }
	const string& LogicGroup3() const { return logicgroup3_; }
	const short LogicPort3() const { return logicport3_; }

	const string& LogicIP4() const { return logicip4_; }
	const string& LogicGroup4() const { return logicgroup4_; }
	const short LogicPort4() const { return logicport4_; }

	const int TimeLen() const { return timelen_; }
	bool Alive() const { return alive_; }

	bool ConnDB()
	{
		//try
		//{
		//	std::string constr;
		//	constr = "UID=";
		//	constr += user_;
		//	constr += ";PWD=";
		//	constr += pwd_;
		//	constr += ";DSN=";
		//	constr += dsn_;
		//	db_.rlogon(constr.c_str());
		//}
		//catch(otl_exception& p)
		//{ 
		//	std::cerr <<" CWebIniFile::ConnDB otl_exception::msg = \n";
		//	std::cerr<<p.msg <<std::endl;
		//	return false;
		//}
		return true;
	}
	void CloseDB()
	{
//		db_.logoff();
	}

	bool InsertPlayerList(char* buffer,int bufflen, otl_connect & db, const char * ip)
	{
		char p[100];
		sprintf(p,"delete from PlayerCompare where address=\'%s\'", ip);
		int irlt = otl_cursor::direct_exec(db,p,otl_exception::disabled);
		db.commit();

		try
		{
			std::string str = "insert into PlayerCompare(id,PlayerID,PlayerName,Country,OfficerRace,Level,MilitaryRank,Money,ResourceCount,\
							  CityCount,GeneralCount,Credit,address) values(:f1<int>,:f2<int>,:f3<char[32]>,:f4<int>,:f5<int>,\
								:f6<int>,:f7<int>,:f8<int>,:f9<int>,:f10<int>,:f11<int>,:f12<int>,:f13<char[50]>)";
			otl_stream o((int)str.size(),str.c_str(),db);
			o.set_commit(0);
			
			tagPlayerInfo * pinfo=(tagPlayerInfo*)buffer;
			int rows = bufflen / sizeof(tagPlayerInfo);
			for (int i=0 ; i<rows;i++)
			{
				o<<pinfo[i].id<<pinfo[i].PlayerID<<pinfo[i].PlayerName<<pinfo[i].Country<<pinfo[i].OfficerRace<<pinfo[i].Level<<pinfo[i].MilitaryRank \
				<<pinfo[i].Money<<pinfo[i].ResourceCount<<pinfo[i].CityCount<<pinfo[i].GeneralCount<<pinfo[i].Credit << ip;
			}

			o.flush();
			db.commit();
		}
		catch(otl_exception& p)
		{
			std::cout << "Exception line(" << __LINE__ << "): " << p.msg << "\n";
			db.rollback();
			return false;
		}
		return true;
	}
	bool InsertGongHuiList(char * buffer,int bufflen, otl_connect & db, const char * ip)
	{
		char p[100];
		sprintf(p,"delete from ConsortiaCompare where address=\'%s\'", ip);
		int irlt = otl_cursor::direct_exec(db,p,otl_exception::disabled);
		db.commit();
		try
		{
			std::string str = "insert into ConsortiaCompare(ID,ConID,ConName,Level,UserCount,PlayerID,ChairmanName,Country,CreateTime,address) \
							  values(:f1<int>,:f2<int>,:f3<char[32]>,:f4<int>,:f5<int>,:f6<int>,:f7<char[32]>,:f8<int>,:f9<bigint>,:f10<char[50]>)";
			otl_stream o((int)str.size(),str.c_str(),db);
			o.set_commit(0);

			tagGongHuiInfo * pinfo=(tagGongHuiInfo*)buffer;
			int rows = bufflen / sizeof(tagGongHuiInfo);
			for (int i=0 ; i<rows;i++)
			{
				o<<pinfo[i].ID<<pinfo[i].ConID<<pinfo[i].ConName<<pinfo[i].Level<<pinfo[i].UserCount<<pinfo[i].PlayerID\
				<<pinfo[i].ChairmanName<<pinfo[i].Country<<pinfo[i].CreateTime << ip;
			}

			o.flush();
			db.commit();
		}
		catch(otl_exception& p)
		{
			std::cout << "Exception line(" << __LINE__ << "): " << p.msg << "\n";
			db.rollback();
			return false;
		}
		return true;
	}

	bool InsertHeroList(char* buffer,int bufflen, otl_connect & db, const char * ip)
	{
		char p[100];
		sprintf(p,"delete from playerofficertable where address=\'%s\'", ip);
		int irlt = otl_cursor::direct_exec(db,p,otl_exception::disabled);
		db.commit();

		try
		{
			std::string str = "insert into playerofficertable(officerid,playerid, officername, officerrace, level, militaryrank, "
			"jobid, faceid, exp, credit, enemycount, generalcount, mapid, cityid, armyid, state, stamina, power, celerity, wit,"
			"techdot, attack, physicrecovery, magicrecovery, actiondot, updatetime, gbhtime,levelstamina, levelpower, levelcelerity,"
			"levelwit, equipstamina, equippower, equipcelerity, equipwit, magicattack, herohp, injurerate, capturerate, "
			"beinjurerate, becapturerate, forcerate,lsttotalattri,lstmainattri,lststamina,grouprate,address) "
			"values("
			":f1<int>,:f2<int>,:f3<char[24]>,:f4<int>,:f5<int>,:f6<int>,:f7<int>,:f8<int>,:f9<int>,:f10<int>,"
			":f11<int>,:f12<int>,:f13<int>,:f14<int>,:f15<int>,:f16<int>,:f17<int>,:f18<int>,:f19<int>,:f20<int>,"
			":f21<int>,:f22<int>,:f23<int>,:f24<int>,:f25<int>,:f26<bigint>,:f27<bigint>,:f28<int>,:f29<int>,:f30<int>,"
			":f31<int>,:f32<int>,:f33<int>,:f34<int>,:f35<int>,:f36<int>,:f37<int>,:f38<int>,:f39<int>,:f40<int>,"
			":f41<int>,:f42<int>,:f43<int>,:f44<int>,:f45<int>,:f46<int>,:f47<char[50]>)";

			otl_stream o((int)str.size(),str.c_str(),db);
			o.set_commit(0);
			
			tagGetHeroList * info=(tagGetHeroList*)buffer;
			int rows = bufflen / sizeof(tagGetHeroList);
			for (int idx=0 ; idx<rows;idx++)
			{
				o << info[idx].officerid_ << info[idx].playerid_ << info[idx].officername_ << info[idx].officerrace_ 
					<< info[idx].level_ << info[idx].militaryrank_ << info[idx].jobid_ << info[idx].faceid_ << info[idx].exp_ 
					<< info[idx].credit_ << info[idx].enemycount_ << info[idx].generalcount_ << info[idx].mapid_ << info[idx].cityid_ 
					<< info[idx].armyid_ << info[idx].state_ << info[idx].stamina_ << info[idx].power_ << info[idx].celerity_ << info[idx].wit_ 
					<< info[idx].techdot_ << info[idx].attack_ << info[idx].physicrecovery_ << info[idx].magicrecovery_ 
					<< info[idx].actiondot_ << info[idx].updatetime_ << info[idx].gbhtime_ << info[idx].levelstamina_ 
					<< info[idx].levelpower_ << info[idx].levelcelerity_ << info[idx].levelwit_ << info[idx].equipstamina_ 
					<< info[idx].equippower_ << info[idx].equipcelerity_ << info[idx].equipwit_ << info[idx].magicattack_ 
					<< info[idx].herohp_ << info[idx].injurerate_ << info[idx].capturerate_ << info[idx].beinjurerate_ 
					<< info[idx].becapturerate_ << info[idx].forcerate_
					<< info[idx].lsttotalattri_ << info[idx].lstmainattri_ << info[idx].lststamina_ << info[idx].grouprate_ << ip;
			}

			o.flush();
			db.commit();
		}
		catch(otl_exception& p)
		{
			std::cout << "Exception line(" << __LINE__ << "): " << p.msg << "\n";
			db.rollback();
			return false;
		}
		return true;
	}

	bool InsertPLayerTaskList(char * buffer,int bufflen, otl_connect & db, const char * ip)
	{
		//sort results,first update and insert second
		tagPlayerTaskInfo * info=(tagPlayerTaskInfo*)buffer;
		int rows = bufflen / sizeof(tagPlayerTaskInfo);
		
		int pos=-1;  //split pos
		for(int i=0;i<rows;i++)  
		{
			if (info[i].OpType==1)   
			{
				pos=i;
				break;
			}
		}
		
		std::string str;
		if (pos!=0)  //have update part
		{
			str="Update PlayerTask Set State=:f1<int>,TaskTime=:f2<bigint> where PlayerId=:f3<int> and TaskID=:f4<int> and address=:f5<char[50]>";
			otl_stream o((int)str.size(),str.c_str(),db);
			o.set_commit(0);
			for (int i=0;i<pos;i++)
			{
				try
				{	o<<info[i].State<<info[i].TaskTime<<info[i].PlayerID<<info[i].TaskID << ip;
					o.flush();
				}
				catch(otl_exception& p)
				{
					std::cout << "Exception line(" << __LINE__ << "): " << p.msg << "\n";
					o.clean(1);
				}
			}
			db.commit();
		}

		if (pos!=-1) //have insert part
		{
			str="Insert into PlayerTask Values(:f1<int>,:f2<int>,:f3<int>,:f4<bigint>,:f5<char[50]>)";
			otl_stream o((int)str.size(),str.c_str(),db);
			o.set_commit(0);
			for (int i=pos;i<rows;i++)
			{
				try
				{
					o<<info[i].PlayerID<<info[i].TaskID<<info[i].State<<info[i].TaskTime<<ip;
					o.flush();
				}
				catch(otl_exception& p)
				{
					std::cout << "Exception line(" << __LINE__ << "): " << p.msg << "\n";
					o.clean(1);
				}
			}
			db.commit();
		}
		return true;
	}

	bool InsertLogRec(const tagLogRec& LogRec, otl_connect & db)
	{
        //boost::mutex::scoped_lock scoped_lock(mux_);
		try
		{
			std::string str="insert into WebLogRec(typeid,typename,data,logdate,bsucceed,playerid,godian,usetype) \
							values(:f1<int>,:f2<char[32]>,:f3<char[100]>,:f4<bigint>,:f5<short>,:f6<int>,:f7<int>,:f8<char[50]>)";
			
			otl_stream o((int)str.size(),str.c_str(),db);
			o.set_commit(0);
			
			o<<LogRec.TypeID<<LogRec.TypeName<<LogRec.Data<<LogRec.LogDate<<LogRec.bSucceed<<LogRec.playerid<<LogRec.godian<<LogRec.usetype;
				
			o.flush();
			db.commit();
		}
		catch (otl_exception& p)
		{
			std::cout << "Exception line(" << __LINE__ << "): " << p.msg << "\n";
			db.rollback();
			return false;
		}
		return true;
	}



	const char * GetTypeName(int code)
	{
		switch (code) 
		{
			case emUpdatePassword:
			{
				return "更改密码";
			}
			case emAddMoney:
			{
				return "充值";
			}
			case emSubMoney:
			{
				return "扣值";
			}
			case emUpdateEmail:
			{
				return "更改Email";
			}
			case emSubScore:
			{
				return "扣除积分";
			}
			case emUpdatePasswordNoCheck:
			{
				return "无验证更改密码";
			}
			case emUpdatePasswordExt:
			{
				return "更改密码-ques/ans";
			}
			default :
			{
				return "非法类型-可能版本太旧";
			}
		}
	}

private:
	bool alive_;
	string dsn_;
	string user_;
	string pwd_;
	short port_;

	string logicip1_;
	string logicgroup1_;
	short logicport1_;

	string logicip2_;
	string logicgroup2_;
	short logicport2_;

	string logicip3_;
	string logicgroup3_;
	short logicport3_;

	string logicip4_;
	string logicgroup4_;
	short logicport4_;

	int timelen_;
	
	CIniFile inifile_;
	aes eak;
	//otl_connect db_;
	//boost::mutex mux_;
};

#endif
