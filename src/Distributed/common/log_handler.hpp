#ifndef __LOG_HANDLER_HPP__
#define __LOG_HANDLER_HPP__

#include "header.hpp"

class log_handler : private boost::noncopyable
{
	typedef boost::shared_ptr<boost::thread> threads;
public:
	log_handler();

	void start(int, const char *, short);
	void stop();

	bool report(const char *);
	void log(const char *);
	bool online(int);
	bool online(const char *, int);

	void add_bandwidth(unsigned short bandwidth, unsigned short type = 1, unsigned short num = 1);

private:
	bool get_cpu_memory_io_usage(int &, int &, int &);
	unsigned __int64 file_time_2_utc(const FILETIME * ftime);

	void print();

	bool run_;

	long bandwidth_sec_;

	long broadcast_bandwidth_sec_;
	threads threads_;

	boost::asio::io_service io_service_;
	boost::asio::ip::tcp::socket socket_;
	char proc_name_[128];
};

#endif // __LOG_HANDLER_HPP__
