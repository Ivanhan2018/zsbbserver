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

#ifndef __ACCEPTOR_HPP__
#define __ACCEPTOR_HPP__

#include "header.hpp"

template < class proactor, class connection, class handler >
class acceptor : private boost::noncopyable
{
public:
	explicit acceptor(proactor &, u_short, handler &);
	void handle_accept(const boost::system::error_code&);
	u_short port() const { return port_; };

private:
	boost::shared_ptr<connection> conn_ptr_;
	std::auto_ptr<boost::asio::ip::tcp::acceptor> acceptor_;
	proactor & pct_;
	u_short port_;
	handler & hdr_;
};

template < class proactor, class connection, class handler > 
acceptor<proactor, connection, handler>::
acceptor(proactor & pct, u_short port, handler & hdr) : 
pct_(pct), hdr_(hdr)
{
	acceptor_.reset(new boost::asio::ip::tcp::acceptor(
		pct_.get_io_service(), boost::asio::ip::tcp::endpoint(
		boost::asio::ip::tcp::v4(), port)));

	port_ = acceptor_->local_endpoint().port();

	conn_ptr_.reset(new connection(pct_.get_io_service(), &hdr_));

	acceptor_->async_accept(conn_ptr_->socket(), boost::bind(
		&acceptor::handle_accept, this, boost::asio::placeholders::error));
}

template < class proactor, class connection, class handler > 
void acceptor<proactor, connection, handler>::
handle_accept(const boost::system::error_code& err)
{
	if (err) return ;

	conn_ptr_->initialize();
	conn_ptr_.reset(new connection(pct_.get_io_service(), &hdr_));

	acceptor_->async_accept(conn_ptr_->socket(), boost::bind(
		&acceptor::handle_accept, this, boost::asio::placeholders::error));
}

#endif // __ACCEPTOR_HPP__

