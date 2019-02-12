#ifndef __SERVER_HPP__
#define __SERVER_HPP__

#include "io_service_pool.hpp"
#include "request_handler.hpp"

class server : private boost::noncopyable
{
public:
	explicit server(int, request_handler*, u_short = 0);

	boost::asio::io_service& get_io_service();

	u_short get_listen_port();

	void io_service_run_event_loop();
	void io_service_end_event_loop();

private:
	void handle_accept(const boost::system::error_code& e);

	io_service_pool io_service_pool_;
	std::auto_ptr<boost::asio::ip::tcp::acceptor> acceptor_;
	request_handler::handler_ptr new_connection_;
	request_handler * handler_;
	u_short port_;
};

#endif // __SERVER_HPP__
