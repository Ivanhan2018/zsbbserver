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
#include "web_server_public.h"
#include "WebServerIniFile.h"
#include "exception_handler.hpp"
#include "um_getdata_thread.h"

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

	exception_handler::init_exception_handler();
	otl_connect::otl_initialize();
	CWebIniFile::Instance().ReadData("um_web_server.ini");
	CWebIniFile::Instance().ConnDB();

	proactor_pool proactor(50);  //50
	request_handler handler;
	handler.start(10);			//10
	acceptor<proactor_pool, internal_socket, request_handler> server(proactor, CWebIniFile::Instance().port(), handler);

	timer_handler th(proactor.get_io_service());
	th.start(CWebIniFile::Instance().TimeLen(), CWebIniFile::Instance().dsn().c_str(),CWebIniFile::Instance().user().c_str(),CWebIniFile::Instance().pwd().c_str());

	console_ctrl_function = boost::bind(&proactor_pool::stop_event_loop, &proactor);
	SetConsoleCtrlHandler(console_ctrl_handler, TRUE);

	std::cout << "user manager v1.00\nPress CTRL + C key to exit\n";

	proactor.run_event_loop();


	th.stop();
	handler.stop();
	CWebIniFile::Instance().CloseDB();

	return 0;
}

#endif // defined(_WIN32)
