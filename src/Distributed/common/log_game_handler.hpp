#ifndef __LOG_GAME_HANDLER_HPP__
#define __LOG_GAME_HANDLER_HPP__

#include "header.hpp"

class log_game_handler : private boost::noncopyable
{
	enum { max_queue_size = 100000 };
	typedef boost::shared_ptr<boost::thread> thread_ptr;
	typedef boost::posix_time::milliseconds milliseconds;

	enum { succeed = 0x00, failed = 0x01 };
	enum { timeout = 0x02, again = 0x03 };

public:
	log_game_handler();

	bool start(u_long, short);
	bool stop();

	bool connected();
	bool push(char *, int);

private:
	int pop(char **&, int &, int &, milliseconds *);
	bool connect();
	void run();

private:
	bool run_;
	int nelts_;
	int length_;

	char ** curr_;
	char * msg_1_[max_queue_size];
	char * msg_2_[max_queue_size];

	const int bounds_;

	boost::condition not_full_;
	int not_full_wait_;

	boost::condition not_empty_;
	int not_empty_wait_;

	boost::asio::detail::mutex mutex_;
	thread_ptr thread_;
	
	boost::asio::io_service ios_;
	boost::asio::ip::tcp::socket sckt_;

	u_long address_;
	short port_;
};

#endif // __LOG_GAME_HANDLER_HPP__
