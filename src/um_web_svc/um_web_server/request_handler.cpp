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
#include "web_server_public.h"
#include "WebServerIniFile.h"


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

}

void request_handler::run()
{
	char * buffer;
	handler_ptr sckt_ptr;
	header * p_head;
	web_server_public spublic;
	spublic.ConData(CWebIniFile::Instance().dsn().c_str(),CWebIniFile::Instance().user().c_str(),CWebIniFile::Instance().pwd().c_str());
	while (pop(sckt_ptr, buffer))
	{
		p_head =  (header*)buffer;

		switch (p_head->command) 
		{
		case 	CM_CHECK_USER		:
			{
				spublic.OnCheckUser(sckt_ptr.get(),buffer);
				break;
			}
		case	CM_REGUSER			:
			{
				spublic.OnRegUser(sckt_ptr.get(),buffer);
				break;
			}
		case	CM_USER_LOING		:
			{
				spublic.OnUserLoginPlus(sckt_ptr.get(),buffer);
				break;	
			}
		case	CM_MODIFY_PASSWORD	:
			{
				spublic.OnModifyPasswordPlus(sckt_ptr.get(),buffer);
				break;
			}
		case CM_MODIFY_PASSWORD_NO	:
			{
				spublic.OnModifyPasswordNoCheck(sckt_ptr.get(),buffer);
				break;
			
			}
		case	CM_ADDMONEY			:
			{
				spublic.OnAddMoneyPlus(sckt_ptr.get(),buffer);
				break;
			}
		case   CM_UPDATEEMAIL		:
			{
				spublic.OnUpdateEmail(sckt_ptr.get(),buffer);
				break;
			}
		case	CM_GETANSWER		:
			{
				spublic.OnGetAnswer(sckt_ptr.get(),buffer);
				break;
			}
		case	CM_SUBMONEY			:
			{
				spublic.OnSubGoDian(sckt_ptr.get(),buffer);
				break;
			}
		case	CM_GETMONEY			:
			{
				spublic.OnGetGoDian(sckt_ptr.get(),buffer);
				break;
			}
		case	CM_PLAYERLIST		:
			{
				spublic.OnGetPlayerList(sckt_ptr.get(),buffer);
				break;
			}
		case	CM_GONGHUILIST		:
			{
				spublic.OnGetGongHuiList(sckt_ptr.get(),buffer);
				break;
			}
		case	CM_GETSCORE:
			{
				spublic.OnGetScore(sckt_ptr.get(),buffer);
				break;
			}
		case	CM_SUBSCORE:
			{
				spublic.OnSubScore(sckt_ptr.get(),buffer);
				break;
			}
		case   CM_GETEMAIL:
			{
				spublic.OnGetEmail(sckt_ptr.get(),buffer);
				break;
			}
		case CM_GMUSERLIST:
			{
				spublic.OnGetGmUserList(sckt_ptr.get(),buffer);
				break;
			}
		case CM_UPDATEUSEREXTINFO:
			{
				spublic.OnUpdateUserExtInfo(sckt_ptr.get(),buffer);
				break;
			}
		case CM_GETUSEREXTINFO:
			{
				spublic.OnGetUserExtInfo(sckt_ptr.get(),buffer);
				break;
			}
		case CM_MODIFY_PASSWORD_EXT:
			{
				spublic.OnModifyPasswordExt(sckt_ptr.get(),buffer);
				break;
			}
		case CM_ADDMONEYTOTAL:
		case CM_ADDMONEYTOTALEX:
		case CM_ADDMONEYTOTALTHIRD:
		case CM_PLAYERLISTTOTAL:
		case CM_USERLISTTOTAL:
		case CM_USERLISTTOTAL3:
		case CM_PLAYERFRIENDLIST:
			{
				spublic.OnDataTotal(sckt_ptr.get(),buffer);
				break;
			}
		case CM_USERBYPIDTOTAL:
			{
				spublic.OnGetUserByIDTotal(sckt_ptr.get(),buffer);
				break;
			}
		case CM_GETRECORDCOUNT:
			{
				spublic.OnGetRecordCount(sckt_ptr.get(),buffer);
				break;
			}

		case CM_GETLOGINFO:
			{
				spublic.OnGetLogInfo(sckt_ptr.get(),buffer);
				break;
			}
		case CM_GETLOGONLINE:
			{
				spublic.OnGetLogOnline(sckt_ptr.get(),buffer);
				break;
			}
		case CM_GETLOGPERF:
			{
				spublic.OnGetLogPerf(sckt_ptr.get(),buffer);
				break;
			}

		case CM_GETRESULTBYPROC:
			{
				spublic.OnGetResultByProc(sckt_ptr.get(),buffer);
				break;
			}

		case CM_GETLOGONLINEEX:
			{
				spublic.OnGetLogOnlineEx(sckt_ptr.get(),buffer);
				break;
			}

		case CM_GETGODIANLOG:
			{
				spublic.OnGetGodianLog(sckt_ptr.get(),buffer);
				break;
			}
			
		default: 
			{
				break;
			}
		}

		sckt_ptr.reset();
		delete [] buffer;
	}
	spublic.DisConnData();
}

void request_handler::exit(char * buffer)
{
	logic_check(buffer, ("buffer = 0x%08x", buffer));
	delete [] buffer;
}
