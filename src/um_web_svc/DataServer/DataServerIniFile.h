#ifndef __DataServerIniFile_H__
#define __DataServerIniFile_H__

#include  "IniFile.h"
#include <crtdbg.h>
#include "aes_plus.h"

using namespace std;

class CDataServerIniFile
{
private:
	CDataServerIniFile():alive_(false) {}
public:
	~CDataServerIniFile() {}
	static CDataServerIniFile& Instance()
	{
		static CDataServerIniFile ww;
		return ww;
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
		unsigned char key[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
		if  (c_name(set_key)(key, 16, both, &eak) != aes_good)
		{
			//printf("≥ı º√‹‘ø¥ÌŒÛ\n");
//			LOG_NO_RETURN(false,("≥ı º√‹‘ø¥ÌŒÛ\n"));
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

	bool Alive() const 
	{
		return alive_;
	}

private:
	bool alive_;
	string dsn_;
	string user_;
	string pwd_;
	short port_;
	CIniFile inifile_;
	aes eak;
};

#endif
