#include "connection.hpp"
#include "request_handler.hpp"
#include "log.hpp"

connection::connection(boost::asio::io_service& io_service, request_handler* handler) :
socket_(io_service), handler_(handler)
{
	logic_check(!handler, ("handler = 0x%08x, %s", handler, 
		errorcode::error_message(errorcode::local_error_object_null)));

	conn_id_ = port_ = online_ = 0;
	length_ = (int *)(header_ + http_header_size);
}

boost::asio::ip::tcp::socket& connection::socket()
{
	return socket_;
}

void connection::start_async_read(bool flag)
{
	if (flag) 
	{
		flag_ = true;
		handler_->handle_connect(this);
	}

	boost::asio::async_read(socket_, boost::asio::buffer(header_, 
		max_header_size), boost::bind(&connection::handle_read_header, 
		shared_from_this(), boost::asio::placeholders::error));
}

void connection::start_async_write(char * buffer, int length)
{
	logic_check(!buffer, ("buffer = 0x%08x, %s", buffer, 
		errorcode::error_message(errorcode::local_error_data_0)));

	logic_check(length <= max_header_size, ("length<=%d, %s", max_header_size, 
		errorcode::error_message(errorcode::local_error_buffer_lenght)));

	boost::asio::async_write(socket_, boost::asio::buffer(
		buffer, length), boost::bind(&connection::handle_write, 
		shared_from_this(), boost::asio::placeholders::error, buffer));
}

void connection::handle_read_header(const boost::system::error_code& err)
{
	if (err) 
	{
		if (err.value() != 2 && err.value() != 10054)
			LOG_ERROR(("handle_read_header(%s:%d),id(%d),code(%d),msg(%s)\n", 
			socket_.remote_endpoint().address().to_v4().to_string().c_str(), 
			socket_.remote_endpoint().port(), conn_id_, err.value(), err.message().c_str()));

		flag_ = false;
		handler_->handle_disconnect(this, err);
	}
	else if ((*length_) <= max_header_size || (*length_) > max_buffer_size)
	{
		int len = http_proxy_header_size - max_header_size;
		char * tmp = new char[len];
		boost::asio::async_read(socket_, boost::asio::buffer(tmp, len), 
			boost::bind(&connection::handle_read_http, shared_from_this(), 
			boost::asio::placeholders::error, tmp));
	}
	else
	{
		char * buffer = new char[(*length_) > default_buffer_size ? 
			(*length_) : default_buffer_size];

		memcpy(buffer, header_, max_header_size);
		boost::asio::async_read(socket_, boost::asio::buffer(
			buffer + max_header_size, (*length_) - max_header_size), 
			boost::bind(&connection::handle_read_body, shared_from_this(), 
			boost::asio::placeholders::error, buffer));
	}
}

void connection::handle_read_http(const boost::system::error_code& err, char * buffer)
{
	if (err || (!::strstr(buffer, "\r\n\r\n")))
	{
		if (err)
		{
			if (err.value() != 2 && err.value() != 10054)
				LOG_ERROR(("handle_read_http(%s:%d),id(%d),code(%d),msg(%s)\n", 
				socket_.remote_endpoint().address().to_v4().to_string().c_str(), 
				socket_.remote_endpoint().port(), conn_id_, err.value(), err.message().c_str()));
		}
		else
		{
			LOG_ERROR(("错误HTTP包，handle_read_http(%s:%d)\n", 
			socket_.remote_endpoint().address().to_v4().to_string().c_str(), 
			socket_.remote_endpoint().port()));
		}

		flag_ = false;
		handler_->handle_disconnect(this, err);
	}
	else
	{
		start_async_read();
	}

	delete [] buffer;
}

void connection::handle_read_body(const boost::system::error_code& err, char * buffer)
{
	if (!err && handler_->handle_message(this, buffer, (*length_)))
	{
		start_async_read();
	}
	else
	{
		if (err)
		{
			if (err.value() != 2 && err.value() != 10054)
				LOG_ERROR(("handle_read_body(%s:%d),id(%d),code(%d),msg(%s)\n", 
				socket_.remote_endpoint().address().to_v4().to_string().c_str(), 
				socket_.remote_endpoint().port(), conn_id_, err.value(), err.message().c_str()));
		}
		else
		{
			LOG_ERROR(("业务不受理，handle_read_body(%s:%d),id(%d),code(%d),msg(%s)\n", 
				socket_.remote_endpoint().address().to_v4().to_string().c_str(), 
				socket_.remote_endpoint().port(), conn_id_, err.value(), err.message().c_str()));
		}
		flag_ = false;
		handler_->handle_disconnect(this, err);
		delete [] buffer;
	}
}

void connection::handle_write(const boost::system::error_code& e, char * buffer)
{
	if (e)
	{
		if (e.value() != 2 && e.value() != 10054)
			LOG_ERROR(("handle_write(%s:%d),id(%d),code(%d),msg(%s)\n", 
			socket_.remote_endpoint().address().to_v4().to_string().c_str(), 
			socket_.remote_endpoint().port(), conn_id_, e.value(), e.message().c_str()));
	}
	delete [] buffer;
}

