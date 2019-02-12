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

#include "header.hpp"

class connection;
class db_handler;

class request_handler
{
	enum {MAX_DB_NUM = 100};

public:
	typedef boost::shared_ptr<connection> handler_ptr;

	explicit request_handler(const char *, const char *, const char *);
	virtual ~request_handler();

	bool start();
	bool stop();

	bool handle_message(connection *, char *, int);
	void handle_connect(connection *){};
	void handle_disconnect(connection *, const boost::system::error_code& err);

private:
	bool run_;
	std::string con_str_;
	db_handler * handlers_[MAX_DB_NUM];
};

#endif // __REQUEST_HANDLER_HPP__
