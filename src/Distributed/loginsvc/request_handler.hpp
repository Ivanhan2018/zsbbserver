#ifndef __REQUEST_HANDLER_HPP__
#define __REQUEST_HANDLER_HPP__

#include "task_base_ex.hpp"

class connection;

class request_handler : public task_base_ex<connection, char *>
{
	typedef boost::shared_ptr<boost::thread> threads_ptr;
public:
	typedef stdext::hash_map<int, handler_ptr> agent_map;
	typedef stdext::hash_map<std::string, agent_map *> group_map;

	//typedef stdext::hash_map<int, int> user_map;

	explicit request_handler();

	bool start(std::size_t, long, const char *, 
		const char *, const char *);

	bool stop();

	bool handle_message(connection *, char *, std::size_t);
	void handle_connect(connection *){};
	void handle_disconnect(connection *, const boost::system::error_code& err);

private:
	void run();
	void exit(char *);
	void print();

	long version_;
	std::string db_con_str_;

	//threads_ptr threads_ptr_;

	boost::mutex mutex_;
	group_map group_;

	//user_map user_;
};

#endif // __REQUEST_HANDLER_HPP__
