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

	explicit request_handler();
	virtual ~request_handler();

	bool start(const char *, const char *, bool);
	bool stop();

	void save_to_file();
	void save_to_db_all();
	bool handle_message(connection *, char *, int);
	void handle_connect(connection *){};
	void handle_disconnect(connection *, const boost::system::error_code& err);

private:
	bool run_;
	db_handler * handlers_[MAX_DB_NUM];
};

#endif // __REQUEST_HANDLER_HPP__
