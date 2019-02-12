#include "writer_handler.hpp"
#include "distributed_protocol.hpp"
#include <io.h>
#include <direct.h>

writer_handler::writer_handler() :
bounds_(max_queue_size)
{
	run_ = false;
	not_full_wait_ = not_empty_wait_ = 0;
	length_ = nelts_ = 0;
}

bool writer_handler::start()
{
	if (run_) return false;

	boost::asio::detail::mutex::scoped_lock lock(mutex_);

	if (run_) return false;

	run_ = true;
	curr_ = msg_1_;
	not_full_wait_ = not_empty_wait_ = 0;
	length_ = nelts_ = 0;

	thread_.reset(new boost::thread(
		boost::bind(&writer_handler::run, this)));

	return true;
}

bool writer_handler::stop()
{
	if (!run_) return false;
	{
		boost::asio::detail::mutex::scoped_lock lock(mutex_);

		if (!run_) return false;

		run_ = false;
		not_empty_.notify_all();
		not_full_.notify_all();
	}

	thread_->join();

	while (nelts_)
	{
		delete [] curr_[nelts_--];
	}

	not_full_wait_ = not_empty_wait_ = 0;
	length_ = nelts_ = 0;
	curr_ = msg_1_;

	return true;
}

bool writer_handler::push(char * msg, int len)
{
	if (!run_) return false;

	boost::asio::detail::mutex::scoped_lock lock(mutex_);
	while (nelts_ == bounds_)
	{
		if (!run_) return false;

		++not_full_wait_;
		not_full_.wait(lock);
		--not_full_wait_;
	}

	if (!run_) return false;

	curr_[nelts_++] = msg;
	length_ += len;

	if (not_empty_wait_ > 0) not_empty_.notify_one();

	return true;
}

bool writer_handler::pop(char **& msg, int & num, int & len)
{
	if (!run_) return false;

	boost::asio::detail::mutex::scoped_lock lock(mutex_);
	while (nelts_ == 0)
	{
		if (!run_) return false;

		++not_empty_wait_;
		not_empty_.wait(lock);
		--not_empty_wait_;
	}

	if (!run_) return false;

	len = length_;
	num = nelts_;
	msg = curr_;

	length_ = nelts_ = 0;
	curr_  = (curr_ == msg_1_ ? msg_2_ : msg_1_);

	if (not_full_wait_ > 0)	not_full_.notify_one();

	return true;
}

void writer_handler::run()
{
	LOG_GAME_WRITER * pckt;
	static int pckt_len = sizeof(LOG_GAME_WRITER);
	static int max_size = 10485760;

	bool change = false;
	FILE * fp = 0;
	char ** data;
	int num, pos, len, filesize = max_size;
	
	int year = 0, mon = 0, day = 0;

	char * buf, path[128];
	int path_len = 0;

	int buf_len = 12582912;
	buf = (char *)::malloc(12582912);

	while (pop(data, num, len))
	{
		len = len - pckt_len * num;
		if (len > buf_len)
		{
			::free(buf);
			buf = (char *)::malloc(len);
			buf_len = len;
		}

		pos = 0;
		for (int i = 0; i < num; i++)
		{
			pckt = (LOG_GAME_WRITER *)data[i];
			memcpy(buf + pos, pckt->buf(), pckt->buf_len());
			pos += pckt->buf_len();
		}

		time_t cur_time = ::time(0);
		tm * time_fat = ::gmtime(&cur_time);
		if ((year != time_fat->tm_year) || (mon != time_fat->tm_mon) || (day != time_fat->tm_mday))
		{
			year = time_fat->tm_year;
			mon = time_fat->tm_mon;
			day = time_fat->tm_mday;

			if (::access("log_game", 0)) ::mkdir("log_game");

			path_len = ::sprintf(path, "log_game/%04d%02d", year + 1900, mon + 1);
			if (::access(path, 0)) ::mkdir(path);

			path_len += ::sprintf(path + path_len, "/%02d", day);
			if (::access(path, 0)) ::mkdir(path);

			change = true;
		}
		
		if (filesize >= max_size || change)
		{
			if (fp) ::fclose(fp);

			::sprintf(path + path_len, "/%02d-%02d-%02d.log", (time_fat->tm_hour + 8)%24, 
				time_fat->tm_min, time_fat->tm_sec);

			fp = ::fopen(path, "at+");
			logic_check(!fp, ("fopen error\n"));

			filesize = 0;
			change = false;
		}

		logic_check(!::fwrite(buf, pos, 1, fp), ("fwrite error\n"));
		::fflush(fp);

		filesize += pos;

		for (int i = 0; i < num; i++)
			delete [] data[i];

	}

	if (fp) ::fclose(fp);
	::free(buf);
}
