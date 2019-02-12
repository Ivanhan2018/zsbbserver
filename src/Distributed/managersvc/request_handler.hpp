#ifndef __REQUEST_HANDLER_HPP__
#define __REQUEST_HANDLER_HPP__

#include "connection.hpp"

class request_handler : private boost::noncopyable
{
public:
	typedef boost::shared_ptr<connection> handler_ptr;

	explicit request_handler(u_short, u_short, std::string &, u_short);

	void clear_all();

	bool handle_message(connection *, char *, int);
	void handle_connect(connection *){};
	void handle_disconnect(connection *, const boost::system::error_code& err);

private:
	stdext::hash_map<int, handler_ptr> cons_;
	u_short game_id_;
	u_short group_id_;
	u_long log_game_address_;
	u_short log_game_port_;
};

#endif // __REQUEST_HANDLER_HPP__
