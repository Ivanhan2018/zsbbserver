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

#include "server.hpp"
#include "request_handler.hpp"
#include "xmldoc.hpp"

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

void read_xml_config(u_short & port, int & thread_pool, std::string & db_uid, 
					 std::string & db_pwd, std::string & db_dsn)
{
	xmlreader::xmldoc doc("config.xml");

	if (!doc.load_xml_doc()) throw std::runtime_error("load_xml_doc error");

	xmlreader::xmlnode * config_node = doc.get_root_node("config");
	if (!config_node) throw std::runtime_error("get_root_node(config) error");

	xmlreader::xmlnode * dbgatewaysvc_node = config_node->get_child("dbgatewaysvc");
	if (!dbgatewaysvc_node) throw std::runtime_error("get_child(dbgatewaysvc) error");

	port = ::atoi(dbgatewaysvc_node->property("listen_port").c_str());
	thread_pool = ::atoi(dbgatewaysvc_node->property("thread_pool").c_str());

	db_pwd = dbgatewaysvc_node->property("db_password").c_str();
	db_uid = dbgatewaysvc_node->property("db_username").c_str();
	db_dsn = dbgatewaysvc_node->property("db_dsn").c_str();
}

int main(int argc, char* argv[])
{
	exception_handler::init_exception_handler();

	u_short port;
	int thread_pool;
	std::string db_uid;
	std::string db_pwd;
	std::string db_dsn;

	read_xml_config(port, thread_pool, db_uid, db_pwd, db_dsn);

	request_handler handler(db_uid.c_str(), db_pwd.c_str(), db_dsn.c_str());
	server s(thread_pool, &handler, port);

	handler.start();

	console_ctrl_function = boost::bind(&server::io_service_end_event_loop, &s);
	SetConsoleCtrlHandler(console_ctrl_handler, TRUE);

	std::cout << "DBGServer v1.01\nPress CTRL + C key to exit\n";

	s.io_service_run_event_loop();
	handler.stop();

	return 0;
}

#endif // defined(_WIN32)
