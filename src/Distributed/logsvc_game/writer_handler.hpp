#ifndef __WRITER_HANDLER_HPP__
#define __WRITER_HANDLER_HPP__

#include "header.hpp"

class writer_handler : private boost::noncopyable
{
	enum { max_queue_size = 200000 };
	typedef boost::shared_ptr<boost::thread> thread_ptr;

public:
	explicit writer_handler();

	bool start();
	bool stop();

	bool push(char *, int);

private:
	bool pop(char **&, int &, int &);
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
};

#endif // __WRITER_HANDLER_HPP__
