#include "um_public_client.h"

um_public_client::um_public_client():
socket_(INVALID_SOCKET),
index_(0),
bactive_(false)
{

}

um_public_client::~um_public_client()
{
	//_ASSERT(socket_==INVALID_SOCKET);
//	LOG_NO_RETURN(socket_==INVALID_SOCKET,("um_public_client::~um_public_client\n"));
}

um_public_client& um_public_client::Instance()
{
	static um_public_client pubclient;
	return pubclient;
}

bool um_public_client::login(const char * ip,short port,int timeout,int reconntime)
{
	//_ASSERT(ip && port>0 && timeout>0);
//	LOG_NO_RETURN(ip && port>0 && timeout>0,("um_public_client::login\n"));

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
	
	strcpy(ip_,ip);
	port_=port;
	timeout_=timeout;
	reconntime_=reconntime;
	return true;
}

bool um_public_client::LoginOut()
{
	if (socket_==INVALID_SOCKET) return false;
	::shutdown(socket_,0x02);  //SD_BOTH
	::closesocket(socket_);
	socket_=INVALID_SOCKET;
	return true;
}

//ÖØÁ¬
bool um_public_client::relogin()
{
	return login(ip_,port_,timeout_,reconntime_);
}

int um_public_client::Recv(char * buf,int len)
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

int um_public_client::Send(const char * buf,int len)
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

int um_public_client::getsocket() const
{
	return socket_;
}

int um_public_client::gettimeout() const
{
	return timeout_;
}

int um_public_client::getreconntime() const
{
	return reconntime_;
}

int um_public_client::getindex()
{
	IceUtil::LockT<IceUtil::Mutex> lock(idx_mux_);
	return index_++;
}

bool um_public_client::InsertData(HDN& dn)
{
	IceUtil::LockT<IceUtil::Mutex> lock(data_mux_);
	std::pair<std::map<int,HDN>::iterator,bool> itrlt;
	itrlt = mdata_.insert(std::map<int,HDN>::value_type(((header*)dn->sbuf)->index,dn) );
	return itrlt.second;
}

bool um_public_client::FindData(int idx,HDN& dn)
{
	IceUtil::LockT<IceUtil::Mutex> lock(data_mux_);
	std::map<int,HDN>::iterator aa=mdata_.find(idx);
	if (aa!=mdata_.end())
	{
		dn=aa->second;
		return true;
	}
	else return false;
}

bool um_public_client::DeleteData(int idx)
{
	IceUtil::LockT<IceUtil::Mutex> lock(data_mux_);
	return  mdata_.erase(idx) >=0;
}

bool um_public_client::UpdateData(int idx,int len,char * rbuf,bool bfailed)
{
	IceUtil::LockT<IceUtil::Mutex> lock(data_mux_);
	std::map<int,HDN>::iterator aa=mdata_.find(idx);
	if (aa!=mdata_.end())
	{
		if (!bfailed)
		{
			aa->second->rlt=true;
			aa->second->rlen=len;
			aa->second->rbuf=rbuf;
			SetEvent((HANDLE)aa->second->event);
		}
		else 
		{
			SetEvent((HANDLE)aa->second->event);
		}
		return true;
	}
	else return false;
}


void um_public_client::SetActive(bool bactive)
{
	bactive_ = bactive;
}

bool um_public_client::GetActive() const
{
	return bactive_;
}



