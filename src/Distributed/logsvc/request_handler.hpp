#ifndef __REQUEST_HANDLER_HPP__
#define __REQUEST_HANDLER_HPP__

#include "task_base_ex.hpp"

class connection;

class request_handler : public task_base_ex<connection, char *>
{
public:
	typedef boost::shared_ptr<connection> handler_ptr;

	explicit request_handler();

	bool start(const char *, const char *, const char *);

	bool stop();

	bool handle_message(connection *, char *, std::size_t);
	void handle_connect(connection *){};
	void handle_disconnect(connection *, const boost::system::error_code& err);

private:
	void run();
	void exit(char *);

	std::queue<char *> perf_;
	std::queue<char *> info_;
	std::queue<char *> online_;
	std::queue<char *> group_online_;
	std::string db_con_str_;
};

#endif // __REQUEST_HANDLER_HPP__
