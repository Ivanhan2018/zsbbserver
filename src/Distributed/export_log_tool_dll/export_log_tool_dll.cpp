#include "export_log_tool_dll.hpp"
#include <winsock2.h>
#include <windows.h>
#include "zlib.h"

#include "distributed_protocol.hpp"

#pragma comment( lib, "ws2_32.lib")

static bool zlib_uncompress(char *& desc, char * src)
{
	static int header_len = sizeof(DISTRIBUTED_HEADER);

	DISTRIBUTED_HEADER * header = (DISTRIBUTED_HEADER *)src;
	char * ptr = src + header_len;
	int ptr_len = header->length - header_len;

	uLong desc_len = header->src_len;
	desc = (char *)::malloc(desc_len);
	::memcpy(desc, src, header_len);

	if (uncompress((Bytef *)(desc + header_len),
		&desc_len, (Bytef *)ptr, ptr_len) != Z_OK)
	{
		::free(desc);
		desc = 0;
		return false;
	}

	header = (DISTRIBUTED_HEADER *)desc;
	header->length = desc_len + header_len;
	header->src_len = 0;
	header->compress = 0;
	return true;
}

static int myconnect(const char * address, unsigned short port)
{
	struct in_addr ip;
	struct hostent * st = ::gethostbyname(address);
	if(!st) return INVALID_SOCKET;

	int fd = (int)::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (fd == INVALID_SOCKET) return INVALID_SOCKET;

	memcpy(&ip, st->h_addr_list[0], 4);

	sockaddr_in sa;
	sa.sin_addr = ip;
	sa.sin_port = ::htons(port);
	sa.sin_family = AF_INET;

	unsigned long ul = 1;

	if (::ioctlsocket(fd, FIONBIO, &ul) == SOCKET_ERROR)
	{
		::closesocket(fd);
		return INVALID_SOCKET;
	}

	ul = 0;
	struct linger li = {1, 0};

	struct timeval tv;
	tv.tv_sec = 3;
	tv.tv_usec = 0;

	fd_set fds;
	FD_ZERO(&fds);
	FD_SET(fd, &fds);

	::connect(fd, (sockaddr *)&sa, sizeof(sa));

	if ((::select(0, 0, &fds, 0, &tv) <= 0) ||
		(::ioctlsocket(fd, FIONBIO, &ul) == SOCKET_ERROR) ||
		(::setsockopt(fd, SOL_SOCKET, SO_LINGER, (char *)&li, sizeof(li)) == SOCKET_ERROR))
	{
		::closesocket(fd);
		return INVALID_SOCKET;
	}

	return fd;
}

static int mysend(int fd, const char * buf, int len)
{
	for (int sent = 0, tmp = 0 ; ; )
	{
		tmp = ::send(fd, buf + sent, len - sent, 0);
		if (tmp < 1) return SOCKET_ERROR;

		sent += tmp;
		if (len == sent) return sent;
	}

	return SOCKET_ERROR;
}

static int myrecv(int fd, char * buf, int len)
{
	for (int recvl = 0, tmp = 0 ; ; )
	{
		tmp = ::recv(fd, buf + recvl, len - recvl, 0);
		if (tmp < 1) return SOCKET_ERROR;

		recvl += tmp;
		if (len == recvl) return recvl;
	}

	return SOCKET_ERROR;
}

static int mydownload(int fd, int begin_date,int end_date,DATA_RECEIVER receiver_func)
{
	LOG_GAME_READER req;
	req.serialize(begin_date, end_date);
	if (mysend(fd, (char *)&req, req.length) == SOCKET_ERROR)
		return socket_error;

	char * large_buffer = (char *)::malloc(12582912);
	int large_buf_len = 12582912;
	bool flag;

	while (true)
	{
		DISTRIBUTED_HEADER header;
		if (myrecv(fd, (char *)&header, sizeof(header)) == SOCKET_ERROR)
		{
			::free(large_buffer);
			return socket_error;
		}

		if (header.length > large_buf_len)
		{
			::free(large_buffer);
			large_buffer = (char *)::malloc(header.length);
			large_buf_len = header.length;
		}

		if (header.agent == -1)
		{
			::free(large_buffer);
			return dir_nonexist;
		}

		memcpy(large_buffer, &header, sizeof(header));
		if (myrecv(fd, large_buffer + sizeof(header), 
			header.length - sizeof(header)) == SOCKET_ERROR)
		{
			::free(large_buffer);
			return socket_error;
		}

		char * ptr = 0;
		if (!zlib_uncompress(ptr, large_buffer))
		{
			::free(large_buffer);
			return uncompress_error;
		}

		LOG_GAME_READER_RSP * rsp = (LOG_GAME_READER_RSP *)ptr;

		int pos = (int)strlen(rsp->full_path);
		for (int i = 0; i < pos; i++)
		{
			if (rsp->full_path[i] == '\\')
				rsp->full_path[i] = '-';
		}

		float rate = (float)((float)rsp->agent / (float)rsp->from * 100.00);
		flag = ((!receiver_func(rsp->full_path + 2, rsp->buffer(), 
			rsp->buf_len(), rate)) || (rsp->agent == rsp->from));

		::free(ptr);
		if (flag) break;
	}
	::free(large_buffer);

	return succeed;

}

int EXPORT_LOG_TOOL_DLL_API log_download(const char * address, unsigned short port, int begin_date, int end_date, DATA_RECEIVER receiver_func)
{
	if (!address || !port || !begin_date || !end_date || !receiver_func)
		return false;

	WSADATA wsaData;
	WSAStartup(0x0202, &wsaData);

	int fd = myconnect(address, port);
	if (fd == INVALID_SOCKET) return connect_error;

	int ret = mydownload(fd, begin_date, end_date, receiver_func);

	::closesocket(fd);
	WSACleanup();
	return ret;
}

