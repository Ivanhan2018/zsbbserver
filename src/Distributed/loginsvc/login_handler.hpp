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

#ifndef __LOGIN_HANDLER_HPP__
#define __LOGIN_HANDLER_HPP__

#include "task_base.hpp"

class login_handler : public task_base<int>
{
public:
	explicit login_handler();

	bool start(int, const char *, u_short, std::size_t, u_short, 
		short, const char *, const char *, const char *);

	void run_event_loop();
	void stop_event_loop();

private:
	inline int recv_n(int, char *, const int &);
	inline int send_n(int, char *, const int &);

	void run();
	void exit(int);

	int listen_;
	int agent_id_;
	std::string agent_address_;
	u_short agent_port_;
	short version_;
	std::string db_con_str_;

	read_write_mutex rw_mutex_;
	std::hash_map<int, char> login_mgr_;
};

#endif // __LOGIN_HANDLER_HPP__
