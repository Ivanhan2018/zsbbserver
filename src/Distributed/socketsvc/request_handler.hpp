#ifndef __REQUEST_HANDLER_HPP__
#define __REQUEST_HANDLER_HPP__

#include "task_base_ex.hpp"
#include "socketsvc.hpp"

class connection;

class request_handler : public task_base_ex<connection, char *>
{
	enum { event_disconnect = 1000 };
public:
	bool start(const char*, u_short, std::size_t, int, 
		u_short, boost::asio::io_service&, DATA_RECEIVER, 
		EVENT_NOTIFY, SOCKET_DISCONNECT, LOGIC_EXCEPTION);

	bool stop();

	int role_id() { return role_id_; }
	u_short game_id() { return game_id_; }
	u_short group_id() { return group_id_; }
	u_long log_game_address() { return log_game_address_; }
	u_short log_game_port() { return log_game_port_; }

	bool start_async_write(int, char *, int);
	bool event_notify(int, char *, int);
	bool broadcast(int, char *, int);

	bool handle_message(connection *, char *, int);
	void handle_connect(connection *){};
	void handle_disconnect(connection *, const boost::system::error_code& err);

private:
	void run();
	void exit(char *);

	int role_id_;
	u_short game_id_;
	u_short group_id_;
	u_long log_game_address_;
	u_short log_game_port_;

	boost::mutex mutex1_;
	stdext::hash_map<int, handler_ptr> cons_;

	DATA_RECEIVER receiver_func_;
	EVENT_NOTIFY notify_func_;
	SOCKET_DISCONNECT disconn_func_;
	LOGIC_EXCEPTION exception_func_;
};

#endif // __REQUEST_HANDLER_HPP__
