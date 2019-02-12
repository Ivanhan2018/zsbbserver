/***************************************************************************
*   Copyright (C) 2007 by wangshenwang                                    *
*   wangshenwang1@gmail.com                                               *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
*   This program is distributed in the hope that it will be useful,       *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
*   GNU General Public License for more details.                          *
*                                                                         *
*   You should have received a copy of the GNU General Public License     *
*   along with this program; if not, write to the                         *
*   Free Software Foundation, Inc.,                                       *
*   59 Temple Place - Suite 330, Boston, MA  02111-1307, CHN.             *
***************************************************************************/

#include "ClientSocket.h"
#include <winsock2.h>
#include <crtdbg.h>
#include <stdio.h>

ClientSocket::ClientSocket() : fd_(INVALID_SOCKET)
{
	WSADATA wsaData;
	WSAStartup( 0x0202, &wsaData );
}

ClientSocket::~ClientSocket()
{
	_ASSERT(this->fd_ == INVALID_SOCKET);
	WSACleanup();
}

bool ClientSocket::Connect(const char * addr, short port)
{
	_ASSERT(addr && port > 0);

	if (this->fd_ != INVALID_SOCKET)
		return false;

	struct in_addr ip;
	hostent * st = gethostbyname(addr);
	if(!st) return false;

	memcpy(&ip, st->h_addr_list[0], 4);

	sockaddr_in sa;
	sa.sin_addr = ip;
	sa.sin_port = htons(port);
	sa.sin_family = AF_INET;

	this->fd_ = (int)::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (this->fd_ == INVALID_SOCKET)
		return false;

	if (::connect(this->fd_, (sockaddr *)&sa, sizeof(sa)) == SOCKET_ERROR)
	{
		::closesocket(this->fd_);
		this->fd_ = INVALID_SOCKET;
		return false;
	}

	return true;
}

void ClientSocket::Disconnect()
{
	if (this->fd_ == INVALID_SOCKET)
		return ;

	::shutdown(this->fd_, SD_BOTH);
	::closesocket(this->fd_);
	this->fd_ = INVALID_SOCKET;
}

int ClientSocket::Send(const char * buf, int len)
{
	_ASSERT(buf && len > 0);

	for ( int sent = 0, tmp = 0 ; ; )
	{
		tmp = ::send(this->fd_, buf + sent, len - sent, 0);
		if (tmp < 1) return SOCKET_ERROR;

		sent += tmp;
		if (len == sent) return sent;
	}

	return SOCKET_ERROR;
}

