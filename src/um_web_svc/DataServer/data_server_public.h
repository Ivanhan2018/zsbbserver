#ifndef __data_server_public_h__
#define __data_server_public_h__

#include "internal_socket.hpp"

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


class data_server_public
{
public:
	data_server_public();
	~data_server_public();

	static Int64 GetTimeNow();
	static std::string toDatetime(Int64 sec);
	bool ConData(const char * dsn,const char * user,const char * password);
	void DisConnData();

	void OnGetPlayerList(internal_socket * i_s,const char* buf);
	void OnGetGongHuiList(internal_socket * i_s,const char* buf);
	void OnGetPlayerTaskList(internal_socket * i_s,const char* buf);
	void OnBackWriteTask(internal_socket * i_s,const char* buf);
	void OnGetHeroList(internal_socket * i_s,const char* buf);
	bool zlib_compress(char *& desc, char * src);

private:
	otl_connect db_;
	bool bconn_;
};


#endif
