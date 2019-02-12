#include "um_getdata_thread.h"
#include "zlib.h"

	
//CGetDataThread::CGetDataThread(int waitfortime)
//:timelen_(waitfortime),brun(false)
//{
//}
//
//
//CGetDataThread::~CGetDataThread()
//{
//}
//
//void CGetDataThread::run()
//{
//	while(brun)
//	{
//		boost::asio::detail::mutex::scoped_lock lock(mutex_);
//		if (m_Semaphore.timed_wait(lock, boost::posix_time::seconds(10)))
//			continue;
//		
//		//执行一次取数据
//		//1.conn socket
//		//2.getPlayerList
//		//3.getGongHuiList
//		//4.close socket
//	}
//	brun = false;
//}
//
//void CGetDataThread::stop()
//{
//	if(!brun) return ;
//	brun = false;
//	m_Semaphore.notify_all();
//}
//

timer_handler::timer_handler(boost::asio::io_service& ios) :
ios_(ios),socket_(INVALID_SOCKET)
{
}


bool timer_handler::start(int waitfortime, const char * dsn,const char * user,const char * password)
{
	try
	{
		std::string constr;
		constr = "UID=";
		constr += user;
		constr += ";PWD=";
		constr += password;
		constr += ";DSN=";
		constr += dsn;
		db_.rlogon(constr.c_str());
	}
	catch(otl_exception& p)
	{ 
		std::cerr <<"********"<<"web_server_public::ConData"<<"********"<<std::endl;
		std::cerr <<"otl_exception::msg : \n";
		std::cerr <<p.msg <<std::endl;
		return false;
	}

	this->waitfortime_ = waitfortime;
	timer_ = new boost::asio::deadline_timer(this->ios_, boost::posix_time::seconds(1));
	timer_->async_wait(boost::bind(&timer_handler::handle_timeout, this, boost::asio::placeholders::error));
	return true;
}

bool timer_handler::stop()
{
	timer_->cancel();
	delete timer_;
	db_.logoff();
	return true;
}

void timer_handler::handle_timeout(const boost::system::error_code& err)
{
	{
		//执行一次取数据
		//1.conn socket
		//2.getPlayerList
		//3.getGongHuiList
		//4.GetPlayerTaskList
		//5.close socket
		if (ConnSocket(CWebIniFile::Instance().LogicIP1().c_str(),CWebIniFile::Instance().LogicPort1()))
		{
			GetPlayerList(CWebIniFile::Instance().LogicGroup1().c_str());
			GetGongHuiList(CWebIniFile::Instance().LogicGroup1().c_str());
			GetPlayerTaskList(CWebIniFile::Instance().LogicGroup1().c_str());
			GetHeroList(CWebIniFile::Instance().LogicGroup1().c_str());
			CloseSocket();
		}

		if (ConnSocket(CWebIniFile::Instance().LogicIP2().c_str(),CWebIniFile::Instance().LogicPort2()))
		{
			GetPlayerList(CWebIniFile::Instance().LogicGroup2().c_str());
			GetGongHuiList(CWebIniFile::Instance().LogicGroup2().c_str());
			GetPlayerTaskList(CWebIniFile::Instance().LogicGroup2().c_str());
			GetHeroList(CWebIniFile::Instance().LogicGroup2().c_str());
			CloseSocket();
		}

		if (ConnSocket(CWebIniFile::Instance().LogicIP3().c_str(),CWebIniFile::Instance().LogicPort3()))
		{
			GetPlayerList(CWebIniFile::Instance().LogicGroup3().c_str());
			GetGongHuiList(CWebIniFile::Instance().LogicGroup3().c_str());
			GetPlayerTaskList(CWebIniFile::Instance().LogicGroup3().c_str());
			GetHeroList(CWebIniFile::Instance().LogicGroup3().c_str());
			CloseSocket();
		}

		if (ConnSocket(CWebIniFile::Instance().LogicIP4().c_str(),CWebIniFile::Instance().LogicPort4()))
		{
			GetPlayerList(CWebIniFile::Instance().LogicGroup4().c_str());
			GetGongHuiList(CWebIniFile::Instance().LogicGroup4().c_str());
			GetPlayerTaskList(CWebIniFile::Instance().LogicGroup4().c_str());
			GetHeroList(CWebIniFile::Instance().LogicGroup4().c_str());
			CloseSocket();
		}

		timer_->expires_at(timer_->expires_at() + boost::posix_time::seconds(this->waitfortime_));
		timer_->async_wait(boost::bind(&timer_handler::handle_timeout, this, boost::asio::placeholders::error));
	}
}

bool timer_handler::ConnSocket(const char * ip,short port)
{
	if (!ip || !port) return false;

	if (socket_!=INVALID_SOCKET) return false;

	struct in_addr sip;
	hostent * st = gethostbyname(ip);
	if(!st) return false;

	memcpy(&sip, st->h_addr_list[0], 4);

	sockaddr_in sa;
	sa.sin_addr = sip;
	sa.sin_port = htons(port);
	sa.sin_family = AF_INET;

	socket_ = (int)::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (socket_ == INVALID_SOCKET)
		return false;

	if (::connect(socket_, (sockaddr *)&sa, sizeof(sa)) == SOCKET_ERROR)
	{
		::closesocket(socket_);
		socket_ = INVALID_SOCKET;
		return false;
	}

	return true;
}

void timer_handler::CloseSocket()
{
	if (socket_==INVALID_SOCKET) return ;
	::shutdown(socket_,0x02);  //SD_BOTH
	::closesocket(socket_);
	socket_=INVALID_SOCKET;
}

int timer_handler::Recv(char * buf,int len)
{
	for ( int recvl = 0, tmp = 0 ; ; )
	{
		tmp = ::recv(socket_, buf + recvl, len - recvl, 0);
		if (tmp < 1) return SOCKET_ERROR;

		recvl += tmp;
		if (len == recvl) return recvl;
	}

	return SOCKET_ERROR;
}

int timer_handler::Send(const char * buf,int len)
{
	for ( int sent = 0, tmp = 0 ; ; )
	{
		tmp = ::send(socket_, buf + sent, len - sent, 0);
		if (tmp < 1) return SOCKET_ERROR;

		sent += tmp;
		if (len == sent) return sent;
	}

	return SOCKET_ERROR;
}


void timer_handler::GetPlayerList(const char * ip)
{
	//取数据个人排行
	tagGetPlayerList p;
	p.serialize(0,max_rows);
	if (Send((char*)&p,p.len)==SOCKET_ERROR) return ;

	tagRepPlayerList rep;
	if (Recv((char*)&rep,sizeof(tagRepPlayerList))==SOCKET_ERROR) return ;
	
	if (!rep.bSucceed || rep.bufflen==0) return ;

	char * buffer = new char[rep.bufflen];
	if (Recv(buffer,rep.bufflen)==SOCKET_ERROR)
	{
		delete [] buffer;
		return ;
	}

	CWebIniFile::Instance().InsertPlayerList(buffer,rep.bufflen, db_, ip);
	delete [] buffer;
}

void timer_handler::GetGongHuiList(const char * ip)
{
	//取数据工会排行
	tagGetGongHuiList p;
	p.serialize(0,max_rows);
	if (Send((char*)&p,p.len)==SOCKET_ERROR) return ;

	tagRepGongHuiList rep;
	if (Recv((char*)&rep,sizeof(tagRepGongHuiList))==SOCKET_ERROR) return ;
	
	if (!rep.bSucceed || rep.bufflen==0) return ;

	char * buffer = new char[rep.bufflen];
	if (Recv(buffer,rep.bufflen)==SOCKET_ERROR)
	{
		delete [] buffer;
		return ;
	}

	CWebIniFile::Instance().InsertGongHuiList(buffer,rep.bufflen, db_, ip);
	delete [] buffer;
}

//void timer_handler::GetPlayerTaskList(const char * ip)
//{
//	tagReqGetPlayerTask p;
//	p.serialize(0);
//	if (Send((char*)&p,p.len)==SOCKET_ERROR) return ;
//
//	tagRpsGetPlayerTask rep;
//	if (Recv((char*)&rep,sizeof(tagRpsGetPlayerTask))==SOCKET_ERROR) return ;
//	
//	if (!rep.bSucceed || rep.BufLen==0) return ;
//
//	char * buffer = new char[rep.BufLen];
//	if (Recv(buffer,rep.BufLen)==SOCKET_ERROR)
//	{
//		delete [] buffer;
//		return ;
//	}
//
//	if (CWebIniFile::Instance().InsertPLayerTaskList(buffer,rep.BufLen, db_, ip))
//	{
//		tagBackWriteTask bwt;
//		bwt.serialize(0);
//		Send((char*)&bwt,bwt.len);
//	};
//	delete [] buffer;
//}

void timer_handler::GetPlayerTaskList(const char * ip)
{
	tagReqGetPlayerTask p;
	p.serialize(0);
	if (Send((char*)&p,p.len)==SOCKET_ERROR) return ;

	tagRpsGetPlayerTask rep;
	if (Recv((char*)&rep,sizeof(tagRpsGetPlayerTask))==SOCKET_ERROR) return ;

	if (!rep.bSucceed || rep.BufLen==0) return ;

	char * buffer = new char[rep.len];
	memcpy(buffer, &rep, sizeof(tagRpsGetPlayerTask));

	if (Recv((buffer + sizeof(tagRpsGetPlayerTask)),rep.len-sizeof(tagRpsGetPlayerTask))==SOCKET_ERROR)
	{
		delete [] buffer;
		return ;
	}

	char * ptr = 0;

	if (!zlib_uncompress(ptr, buffer))
	{
		delete [] buffer;
		return ;
	}
	else
		delete [] buffer;

	if (CWebIniFile::Instance().InsertPLayerTaskList((ptr + sizeof(tagRpsGetPlayerTask)),rep.BufLen, db_, ip))
	{
		tagBackWriteTask bwt;
		bwt.serialize(0);
		Send((char*)&bwt,bwt.len);
	};
	delete [] ptr;
}

bool timer_handler::zlib_uncompress(char *& desc, char * src)
{
	static int header_len = sizeof(tagRpsGetHeroList);

	tagRpsGetHeroList * header = (tagRpsGetHeroList *)src;
	char * ptr = src + header_len;
	int ptr_len = header->len - header_len;

	uLong desc_len = header->src_len;
	desc = new char[desc_len];
	::memcpy(desc, src, header_len);

	if (uncompress((Bytef *)(desc + header_len),
		&desc_len, (Bytef *)ptr, ptr_len) != Z_OK)
	{
		delete [] desc;
		desc = 0;
		return false;
	}

	header = (tagRpsGetHeroList *)desc;
	header->len = desc_len + header_len;
	header->src_len = 0;
	header->compress = 0;
	return true;
}

void timer_handler::GetHeroList(const char * ip)
{
	tagReqGetHeroList p;
	p.serialize(0);
	if (Send((char*)&p,p.len)==SOCKET_ERROR) return ;

	tagRpsGetHeroList rep;
	if (Recv((char*)&rep,sizeof(tagRpsGetHeroList))==SOCKET_ERROR) return ;

	if (!rep.bSucceed || rep.BufLen==0) return ;

	char * buffer = new char[rep.len];
	memcpy(buffer, &rep, sizeof(tagRpsGetHeroList));

	if (Recv((buffer + sizeof(tagRpsGetHeroList)),rep.len-sizeof(tagRpsGetHeroList))==SOCKET_ERROR)
	{
		delete [] buffer;
		return ;
	}

	char * ptr = 0;

	if (!zlib_uncompress(ptr, buffer))
	{
		delete [] buffer;
		return ;
	}
	else
		delete [] buffer;

	CWebIniFile::Instance().InsertHeroList((ptr + sizeof(tagRpsGetHeroList)),rep.BufLen, db_, ip);
	delete [] ptr;
}
