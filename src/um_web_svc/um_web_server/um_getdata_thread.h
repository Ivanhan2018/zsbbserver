#ifndef __um_getdata_thread_h__
#define __um_getdata_thread_h__

#include "header.hpp"
#include "WebServerIniFile.h"
#include "um_web.h"
//#include <boost/asio.hpp>
//#include <boost/thread/condition.hpp>

//#include <boost/thread.hpp>
//
//class CGetDataThread 
//{
//public:
//	CGetDataThread(int waitfortime);
//	~CGetDataThread();
//
//	void run();
//	void stop();
//private:
//	int timelen_;  //µ•Œª√Î
//	bool brun;
//
//	boost::asio::detail::mutex mutex_;
//	boost::condition m_Semaphore;
//};
//

class timer_handler : private boost::noncopyable
{
public:
	explicit timer_handler(boost::asio::io_service& ios);
	bool start(int waitfortime, const char * dsn,const char * user,const char * password);
	bool stop();

private:
	void handle_timeout(const boost::system::error_code&);
	bool ConnSocket(const char * ip,short port);
	void CloseSocket();
	int Recv(char * buf,int len);
	int Send(const char * buf,int len);


	void GetPlayerList(const char * ip);
	void GetGongHuiList(const char * ip);
	void GetPlayerTaskList(const char * ip);
	void GetHeroList(const char * ip);
	bool zlib_uncompress(char *& desc, char * src);

	boost::asio::deadline_timer * timer_;
	boost::asio::io_service & ios_;
	int waitfortime_;
	int socket_;

	otl_connect db_;
};

#endif