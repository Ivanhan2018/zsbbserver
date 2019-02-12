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

#ifndef __REQUEST_HANDLER_HPP__
#define __REQUEST_HANDLER_HPP__

#include "task_base_ex.hpp"

class internal_socket;

class request_handler : public task_base_ex<internal_socket, char *>
{
public:
	explicit request_handler(){};
	
	void handle_connect(internal_socket *);
	bool handle_message(internal_socket *, char *, int);
	void handle_disconnect(internal_socket *);

private:
	void run();
	void exit(char *);

	//DATA_RECEIVER receiver_func_;
	//EVENT_NOTIFY notify_func_;
	//SOCKET_DISCONNECT disconn_func_;
	//LOGIC_EXCEPTION exception_func_;
};

#endif // __REQUEST_HANDLER_HPP__
