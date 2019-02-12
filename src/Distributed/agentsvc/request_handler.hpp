#ifndef __REQUEST_HANDLER_HPP__
#define __REQUEST_HANDLER_HPP__

#include "city_handler.hpp"

class request_handler : private boost::noncopyable
{
	typedef boost::shared_ptr<city_handler> city_handler_ptr;
	typedef stdext::hash_map<int, city_handler_ptr> city_map;
	typedef stdext::hash_map<int, int> client_map;

	enum { LOGIN_FLAG = 5555};

public:
	typedef boost::shared_ptr<connection> handler_ptr;

	explicit request_handler();

	bool start(const char*, u_short, const char *, int, u_short, const char *, 
		u_short, const char * url, boost::asio::io_service&);

	bool stop();

	bool handle_message(connection *, char *, std::size_t);
	void handle_connect(connection * conn);
	void handle_disconnect(connection *, const boost::system::error_code& err);

	void handle_timeout(const boost::system::error_code&);

private:
	std::string group_name_;
	int role_id_;

	handler_ptr login_svc_;
	
	boost::mutex mutex1_;
	city_map citys_;

	client_map clt_map_;
	boost::mutex mutex_;

	boost::asio::deadline_timer * timer_;
};

#endif // __REQUEST_HANDLER_HPP__
