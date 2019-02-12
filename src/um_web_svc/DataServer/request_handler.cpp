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

#include "request_handler.hpp"
#include "internal_socket.hpp"
#include "Data_Server_Public.h"
#include "DataServerIniFile.h"


void request_handler::handle_connect(internal_socket * sckt)
{
	//sckt->socket().remote_endpoint().address().to_string();
	//sckt->socket().remote_endpoint().port();
}

bool request_handler::handle_message(internal_socket * sckt, char * buffer, int length)
{
	logic_check(buffer, ("buffer = 0x%08x", buffer));

	if (push(sckt->shared_from_this(), buffer)) return true;
	return delete [] buffer, false;
}

void request_handler::handle_disconnect(internal_socket * sckt)
{
	//
}

void request_handler::run()
{
	char * buffer;
	handler_ptr sckt_ptr;
	header * p_head;
	data_server_public dspublic;
	dspublic.ConData(CDataServerIniFile::Instance().dsn().c_str(),CDataServerIniFile::Instance().user().c_str(),\
					CDataServerIniFile::Instance().pwd().c_str());
	while (pop(sckt_ptr, buffer))
	{
		p_head =  (header*)buffer;

		switch (p_head->command) 
		{
		case 	CM_PLAYERLIST:
			{
				dspublic.OnGetPlayerList(sckt_ptr.get(),buffer);
				break;
			}
		case	CM_GONGHUILIST:
			{
				dspublic.OnGetGongHuiList(sckt_ptr.get(),buffer);
				break;
			}
		case	CM_GETPLAYERTASK:
			{
				dspublic.OnGetPlayerTaskList(sckt_ptr.get(),buffer);
				break;
			}
		case	CM_BACKWRITETASK:
			{
				dspublic.OnBackWriteTask(sckt_ptr.get(),buffer);
				break;
			}

		case	CM_GETHEROLIST:
			{
				dspublic.OnGetHeroList(sckt_ptr.get(),buffer);
				break;
			}

		default: 
			{
//				LOG_NO_RETURN(false,("收到非法命令字 %d \n",p_head->command));
				break;
			}
		}
		sckt_ptr.reset();
		delete [] buffer;
	}
	dspublic.DisConnData();
}

void request_handler::exit(char * buffer)
{
	logic_check(buffer, ("buffer = 0x%08x", buffer));
	delete [] buffer;
}
