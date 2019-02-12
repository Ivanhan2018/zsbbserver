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

#include "internal_socket.hpp"
#include "request_handler.hpp"


internal_socket::internal_socket(boost::asio::io_service& io_service, request_handler * handler) :
socket_(io_service), id_(0), handler_(handler)
{
	length_ = (int *)(header_+offset);

}

boost::asio::ip::tcp::socket& internal_socket::socket()
{
	return socket_;
}

void internal_socket::initialize()
{
	boost::asio::socket_base::linger linger1(true, 0);
	socket().set_option(linger1);

	//boost::asio::ip::tcp::no_delay no_delay(true);
	//socket().set_option(no_delay);

	boost::system::error_code ret_err;
	boost::asio::detail::socket_ops::clear_error(ret_err);

#if defined(_WIN32)

	u_long tmp;
	tcp_keepalive live = {0}, liveout = {0};
	live.keepaliveinterval = 5000;
	live.keepalivetime = 30000;
	live.onoff = 1;

	boost::asio::detail::socket_ops::error_wrapper(
		::WSAIoctl(socket().native(),
		SIO_KEEPALIVE_VALS, &live, sizeof(live), 
		&liveout, sizeof(liveout), &tmp, 0, 0), ret_err);

	boost::asio::detail::throw_error(ret_err);

#else

	std::size_t keepalive = 1;
	boost::asio::detail::socket_ops::error_wrapper(2
		::setsockopt(socket().native(), 
		SOL_TCP, TCP_KEEPALIVE, (void *)&keepalive, 
		sizeof(keepalive)), ret_err);

	boost::asio::detail::throw_error(ret_err);

	std::size_t keepidle = 30;
	boost::asio::detail::socket_ops::error_wrapper(2
		::setsockopt(socket().native(), 
		SOL_TCP, TCP_KEEPIDLE, (void *)&keepidle, 
		sizeof(keepidle)), ret_err);

	boost::asio::detail::throw_error(ret_err);

	std::size_t keepitnterval = 5;
	boost::asio::detail::socket_ops::error_wrapper(
		::setsockopt(socket().native(), 
		SOL_TCP, TCP_KEEPINTVL, (void *)&keepitnterval, 
		sizeof(keepitnterval)), ret_err);

	boost::asio::detail::throw_error(ret_err);

	std::size_t keepcount = 3;
	boost::asio::detail::socket_ops::error_wrapper(
		::setsockopt(socket().native(), 
		SOL_TCP, TCP_KEEPCNT, (void *)&keepcount, 
		sizeof(keepcount)), ret_err);

	boost::asio::detail::throw_error(ret_err);

#endif

	handler_->handle_connect(this);
	start_async_read();
}

void internal_socket::start_async_read()
{
	boost::asio::async_read(socket_, boost::asio::buffer(header_, 
		defsize), boost::bind(&internal_socket::handle_read_header, 
		shared_from_this(), boost::asio::placeholders::error));
}

void internal_socket::start_async_write(char * buffer, std::size_t length)
{
	logic_check(buffer, ("buffer=0x%08x", buffer));
	logic_check(length, ("length=%d", length));

	boost::asio::async_write(socket_, boost::asio::buffer(
		buffer, length), boost::bind(&internal_socket::handle_write, 
		shared_from_this(), boost::asio::placeholders::error, buffer));
}

void internal_socket::handle_read_header(const boost::system::error_code& err)
{
	if (!err) 
	{
		logic_check(*length_ > defsize, ("length=%d", *length_));

		//char * buffer = new char[*length_ > default_buffer_size ? 
		//	*length_ : default_buffer_size];

		char * buffer = new char[*length_];

		logic_check(buffer, ("buffer=0x%08x", buffer));
		memcpy(buffer, header_, defsize);

		boost::asio::async_read(socket_, boost::asio::buffer(buffer + defsize, 
			*length_ - defsize), boost::bind(&internal_socket::handle_read_body, 
			shared_from_this(), boost::asio::placeholders::error, buffer));
	}
	else
	{
		handler_->handle_disconnect(this);
	}
}

void internal_socket::handle_read_body(const boost::system::error_code& err, char * buffer)
{
	if (err) 
	{
		delete [] buffer;
		handler_->handle_disconnect(this);
	}
	else if (handler_->handle_message(this, buffer, *length_))
	{
		start_async_read();
	}
	else
	{
		handler_->handle_disconnect(this);
	}
}

void internal_socket::handle_write(const boost::system::error_code& err, char * buffer)
{
	delete [] buffer;
	if (err) handler_->handle_disconnect(this);
}

