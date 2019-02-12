#ifndef __CITY_HANDLER_HPP__
#define __CITY_HANDLER_HPP__

#include "task_base_ex.hpp"
#include "log_handler.hpp"

class connection;

extern log_handler * __log_handler;

class city_handler : public task_base_ex<connection, char *>
{
	typedef stdext::hash_map<int, handler_ptr> clients;
public:
	enum { event_disconnect = 1000};

	explicit city_handler();

	bool start(int, int, connection *);
	bool stop();

private:
	bool zlib_compress(char *&, char *);
	bool zlib_uncompress(char *&, char *);

	void run();
	void exit(char *);

	clients cnts_;
	int agent_id_;
	handler_ptr city_;
};

#endif // __CITY_HANDLER_HPP__
