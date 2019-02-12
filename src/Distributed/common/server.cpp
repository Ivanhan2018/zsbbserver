#include "server.hpp"
#include "connection.hpp"

server::server(int io_service_pool_size, request_handler * handler, u_short port) : 
io_service_pool_(io_service_pool_size), handler_(handler)
{
	logic_check(!io_service_pool_size, ("io_service_pool_size = %d, %s", 
		io_service_pool_size, errorcode::error_message(errorcode::local_error_data_0)));

	logic_check(!handler, ("handler = 0x%08x, %s", handler, 
		errorcode::error_message(errorcode::local_error_object_null)));

	acceptor_.reset(new boost::asio::ip::tcp::acceptor(io_service_pool_.get_io_service(), 
		boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port), false));

	port_ = acceptor_->local_endpoint().port();
	new_connection_.reset(new connection(io_service_pool_.get_io_service(), handler_));

	acceptor_->async_accept(new_connection_->socket(), boost::bind(&server::
		handle_accept, this, boost::asio::placeholders::error));
}

boost::asio::io_service& server::get_io_service()
{
	return io_service_pool_.get_io_service();
}

u_short server::get_listen_port()
{
	return port_;
}

void server::io_service_run_event_loop()
{
	io_service_pool_.run_event_loop();
}

void server::io_service_end_event_loop()
{
	io_service_pool_.stop_event_loop();
}

void server::handle_accept(const boost::system::error_code& e)
{
	if (e) return ;

	new_connection_->start_async_read(true);
	new_connection_.reset(new connection(io_service_pool_.get_io_service(), handler_));

	acceptor_->async_accept(new_connection_->socket(),boost::bind(&server::
		handle_accept, this,boost::asio::placeholders::error));
}

