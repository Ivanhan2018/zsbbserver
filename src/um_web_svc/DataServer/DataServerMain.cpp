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

#if defined(_WIN32)

#include "acceptor.hpp"
#include "proactor_pool.hpp"
#include "request_handler.hpp"
#include "internal_socket.hpp"
#include "Data_server_public.h"
#include "DataServerIniFile.h"
#include "exception_handler.hpp"

boost::function0<void> console_ctrl_function;

BOOL WINAPI console_ctrl_handler(DWORD ctrl_type)
{
	switch (ctrl_type)
	{
	case CTRL_C_EVENT:
	case CTRL_BREAK_EVENT:
	case CTRL_CLOSE_EVENT:
	case CTRL_SHUTDOWN_EVENT:
		console_ctrl_function();
		return TRUE;
	default:
		return FALSE;
	}
}

int main(int argc, char* argv[])
{
	//otl_connect db_;
	//std::string constr;
	//constr = "UID=";
	//constr += "sa";
	//constr += ";PWD=";
	//constr += "sa";
	//constr += ";DSN=";
	//constr += "LocalServer";
	//db_.rlogon(constr.c_str());
	//db.setcommit(0);
	//
	//std::string str("select AccountID,AccountName from GameUser");
	//otl_stream i;
	//i.open((int)str.size(),str.c_str(),db_);
	//std::cout<<*(i.ss).row_count<<std::endl;
	//getchar();
	//return 1;

	exception_handler::init_exception_handler();

	otl_connect::otl_initialize();
	CDataServerIniFile::Instance().ReadData("DataServer.ini");

	proactor_pool proactor(1);
	request_handler handler;
	handler.start(1);
	acceptor<proactor_pool, internal_socket, request_handler> server(proactor, CDataServerIniFile::Instance().port(), handler);

	console_ctrl_function = boost::bind(&proactor_pool::stop_event_loop, &proactor);
	SetConsoleCtrlHandler(console_ctrl_handler, TRUE);

	std::cout << "DataServer v1.00\nPress CTRL + C key to exit\n";

	proactor.run_event_loop();

	handler.stop();

	return 0;
}

#endif // defined(_WIN32)
