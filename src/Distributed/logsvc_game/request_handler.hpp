#ifndef __REQUEST_HANDLER_HPP__
#define __REQUEST_HANDLER_HPP__

#include "header.hpp"
#include "reader_handler.hpp"

class connection;
class writer_handler;

class request_handler : private boost::noncopyable
{
public:
	typedef boost::shared_ptr<connection> handler_ptr;

	explicit request_handler() {};

	bool start();
	void stop();

	bool handle_message(connection *, char *, std::size_t);
	void handle_connect(connection *){};
	void handle_disconnect(connection *, const boost::system::error_code& err){};

private:
	writer_handler * writer_;
	reader_handler reader_;
};

#endif // __REQUEST_HANDLER_HPP__
