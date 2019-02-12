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

#ifndef __INTERNAL_SOCKET_HPP__
#define __INTERNAL_SOCKET_HPP__

#include "header.hpp"

class request_handler;

class internal_socket : private boost::noncopyable, 
	public boost::enable_shared_from_this<internal_socket>
{
	enum { max_buffer_size = 4096 };
	enum { default_buffer_size = 128 };

public:
	explicit internal_socket(boost::asio::io_service&, request_handler *);
	boost::asio::ip::tcp::socket& socket();

	void initialize();
	void start_async_read();
	void start_async_write(char *, std::size_t);

	int & id(){ return id_; }

private:
	void handle_read_header(const boost::system::error_code&);
	void handle_read_body(const boost::system::error_code&, char *);
	void handle_write(const boost::system::error_code& e, char *);

	boost::asio::ip::tcp::socket socket_;

	int * length_;
	char header_[defsize];

	int id_;
	request_handler * handler_;
};

#endif // __INTERNAL_SOCKET_HPP__
