#include "DB_Proxy.h"

#ifdef _WIN32
#	include <winsock.h>
#	pragma comment( lib, "ws2_32" )
#	include <process.h>
#else
#	include <netinet/in.h>
#	include <netinet/tcp.h>
#	include <sys/socket.h>
#	include <arpa/inet.h>
#endif

#include <stdio.h>


DB_Proxy::DB_Proxy () :
fd_(0)
{
#ifndef NDEBUG
	tid_ = 0;
#endif
}

bool
DB_Proxy::init(const char * addr, short port, int city_id)
{
	if (fd_) return false;

	if (!addr || !port) return false;

	city_id_ = city_id;

	hostent * st = ::gethostbyname(addr);
	if(!st) return false;
	
	int fd;
	if ((fd = (int)::socket(AF_INET, SOCK_STREAM, 0)) < 0)
		return false;
	
	in_addr ip;
	memcpy(&ip, st->h_addr_list[0], 4);
	
	sockaddr_in sa;
	sa.sin_addr = ip;
	sa.sin_port = ::htons(port);
	sa.sin_family = AF_INET;
	
	if (::connect(fd, (sockaddr *)&sa, sizeof (sa)) == -1)
	{
#ifdef _WIN32
		::closesocket(fd);
#else
		::close(fd);
#endif
		return false;
	}

	fd_ = fd;
	return true;
}

bool 
DB_Proxy::init_proxy(const char * proxy_addr, short proxy_port, const char * addr, short port, int city_id)
{
	if (fd_) return false;

	if (!addr || !port) return false;

	city_id_ = city_id;

	hostent * st = ::gethostbyname(proxy_addr);
	if(!st) return false;

	int fd;
	if ((fd = (int)::socket(AF_INET, SOCK_STREAM, 0)) < 0)
		return false;

	in_addr ip;
	memcpy(&ip, st->h_addr_list[0], 4);

	sockaddr_in sa;
	sa.sin_addr = ip;
	sa.sin_port = ::htons(proxy_port);
	sa.sin_family = AF_INET;

	if (::connect(fd, (sockaddr *)&sa, sizeof (sa)) == -1)
	{
		::closesocket(fd);
		return false;
	}

	fd_ = fd;

	char buf[256];
	sprintf(buf, "CONNECT %s:%d HTTP/1.0\r\nUser-Agent: Mozilla/4.0\r\nConnection: Keep-Alive\r\n\r\n", addr, port);
	if (send_n(buf, (int)strlen(buf)) != (int)strlen(buf))
	{
		::closesocket(fd_);
		fd_ = 0;
		return false;
	}

	if (::recv(fd_, buf, 250, 0) < 1)
	{
		::closesocket(fd_);
		fd_ = 0;
		return false;
	}

	int mj, mi, c;
	sscanf(buf, "HTTP/%d.%d %d", &mj, &mi, &c);
	if (c != 200)
	{
		::closesocket(fd_);
		fd_ = 0;
		return false;
	}

	fd_ = fd;
	return true;
}


bool
DB_Proxy::fini()
{
	if (!fd_) return false;

#ifdef _WIN32
		::closesocket(fd_);
#else
		::close(fd_);
#endif
	return fd_ = 0, true;
}

int 
DB_Proxy::send_n(const char * buf, int len)
{
	if (!buf || !len || !fd_) return -1;

	for (int s = 0, t = 0 ; ; )
	{
#ifdef _WIN32
		t = send(fd_, buf + s, len - s, 0);
#else
		t = write(fd_, buf + s, len - s, 0);
#endif
		if (t < 1) return -1;
		s += t;
		if (len == s) return s;
	}
	
	return -1;
}

int 
DB_Proxy::recv_n(char * buf, int len)
{
	if (!buf || !len || !fd_) return -1;

	for (int s = 0, t = 0 ; ; )
	{
#ifdef _WIN32
		t = ::recv(fd_, buf + s, len - s, 0);
#else
		t = read(fd_, buf + s, len - s, 0);
#endif
		if (t < 1) return -1;
		s += t;
		if (len == s) return s;
	}

	return -1;
}

int 
DB_Proxy::recv(char * buf, int len)
{
	if (!buf || !len || !fd_) return -1;

	int t;
#ifdef _WIN32
	t = ::recv(fd_, buf, len, 0);
#else
	t = read(fd_, buf, len, 0);
#endif
	if (t < 1) return -1;
	return t;
}

#ifndef NDEBUG
void 
DB_Proxy::test(const char * func, int line)
{
	//long cur = GetCurrentThreadId();
	//long old = InterlockedCompareExchange(&tid_, cur, 0);
	//if (old && old != cur)
	//{
	//	printf("%s(%d) 发现线程A(%d)与线程B(%d)同时使用DB_Proxy\n", func, line, old, cur);
	//	abort();
	//}
}
#endif
