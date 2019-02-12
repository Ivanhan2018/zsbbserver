#ifndef __um_server_pulbic_H_
#define __um_server_pulbic_H_

#include "internal_socket.hpp"
#include "WebServerInifile.h"

class web_server_public
{
public:
	web_server_public();
	~web_server_public();

	//static Int64 GetTimeNow();
	//static std::string toDatetime(Int64 sec);
	bool ConData(const char * dsn,const char * user,const char * password);
	void DisConnData();


	void OnCheckUser(internal_socket * i_s,const char* buf);
	void OnRegUser(internal_socket * i_s,const char * buf);
	void OnUserLoginPlus(internal_socket * i_s,const char * buf);
	void OnModifyPasswordPlus(internal_socket * i_s,const char * buf);
	void OnModifyPasswordNoCheck(internal_socket * i_s,const char * buf);
	void OnModifyPasswordExt(internal_socket * i_s,const char * buf);
	void OnAddMoneyPlus(internal_socket * i_s,const char * buf);
	void OnSubGoDian(internal_socket * i_s,const char * buf);
	void OnGetGoDian(internal_socket * i_s,const char * buf);
	void OnGetEmail(internal_socket * i_s,const char * buf);
	void OnUpdateEmail(internal_socket * i_s,const char * buf);
	void OnGetAnswer(internal_socket * i_s,const char * buf);
	void OnGetPlayerList(internal_socket * i_s,const char * buf);
	void OnGetGongHuiList(internal_socket * i_s,const char * buf);
	void OnGetScore(internal_socket * i_s,const char * buf);
	void OnSubScore(internal_socket * i_s,const char * buf);
	void OnGetGmUserList(internal_socket * i_s,const char * buf);
	void OnGetUserExtInfo(internal_socket * i_s,const char * buf);
	void OnUpdateUserExtInfo(internal_socket * i_s,const char * buf);
	void OnDataTotal(internal_socket * i_s,const char * buf);
	void OnGetUserByIDTotal(internal_socket * i_s,const char * buf);
	void OnGetRecordCount(internal_socket * i_s,const char * buf);

	void OnGetLogInfo(internal_socket * i_s,const char * buf);
	void OnGetLogOnline(internal_socket * i_s,const char * buf);
	void OnGetLogOnlineEx(internal_socket * i_s,const char * buf);
	void OnGetLogPerf(internal_socket * i_s,const char * buf);
	void OnGetResultByProc(internal_socket * i_s,const char * buf);

	void OnGetGodianLog(internal_socket * i_s,const char * buf);

private:
	otl_connect db_;
	bool bconn_;
};


#endif
