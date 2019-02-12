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

#include "report.h"
#include "ClientSocket.h"
#include "../../include/commonprotocol/global_macro.h"
#include "../../include/commonprotocol/Distributed_Header.h"
#include <string>
#include <time.h>


struct LOG_REPORT_INFO : DISTRIBUTED_HEADER
{
	char proc_name[128];
	char datetime[25];
	char msg[300];
	inline void serialize(const char *, const char *, const char *);
};

inline void LOG_REPORT_INFO::serialize(const char * proc_name, const char * datetime, const char * msg)
{
	DISTRIBUTED_HEADER::serialize(sizeof(*this), 0, 0, 0, CMD_LOG_REPORT_INFO);
	strcpy(this->proc_name, proc_name);
	strcpy(this->datetime, datetime);
	strcpy(this->msg, msg);
}

ClientSocket _socket;

int REPORT_API init(const char * ip, short port)
{
	if (_socket.Connect(ip, port))
		return 1;
	return 0;
}

void REPORT_API fini()
{
	_socket.Disconnect();
}

int REPORT_API log(const char * msg, int len)
{
	time_t tb = ::time(0);
	tm * fat = ::gmtime(&tb);

	LOG_REPORT_INFO rpt;
	rpt.serialize("SERVERMONITOR", "", msg);

	sprintf(rpt.datetime, " %04d-%02d-%02d %02d:%02d:%02d", fat->tm_year + 1900, fat->tm_mon + 1, 
		fat->tm_mday, (fat->tm_hour + 8)%24, fat->tm_min, fat->tm_sec);

	if (_socket.Send((const char *)&rpt, rpt.length) != len)
		return 0;
	return 1;
}

