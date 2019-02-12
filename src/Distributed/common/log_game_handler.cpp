#include "log_game_handler.hpp"
#include "distributed_protocol.hpp"

log_game_handler::log_game_handler() :
bounds_(max_queue_size), sckt_(ios_)
{
	run_ = false;
	not_full_wait_ = not_empty_wait_ = 0;
	length_ = nelts_ = 0;
}

bool log_game_handler::start(u_long address, short port)
{
	if (run_) return false;

	boost::asio::detail::mutex::scoped_lock lock(mutex_);

	if (run_) return false;

	address_ = address;
	port_ = port;

	run_ = true;
	curr_ = msg_1_;
	not_full_wait_ = not_empty_wait_ = 0;
	length_ = nelts_ = 0;

	connect();

	thread_.reset(new boost::thread(
		boost::bind(&log_game_handler::run, this)));

	return true;
}

bool log_game_handler::stop()
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

	sckt_.close();

	while (nelts_)
	{
		delete [] curr_[nelts_--];
	}

	not_full_wait_ = not_empty_wait_ = 0;
	length_ = nelts_ = 0;
	curr_ = msg_1_;

	return true;
}

bool log_game_handler::connected()
{
	return sckt_.is_open();
}

bool log_game_handler::push(char * msg, int len)
{
	if (!run_) return false;
	if (!msg) return false;

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

int log_game_handler::pop(char **& msg, int & num, int & len, milliseconds * tm)
{
	if (!run_) return failed;

	boost::asio::detail::mutex::scoped_lock lock(mutex_);
	while (nelts_ == 0)
	{
		if (!run_) return failed;

		++not_empty_wait_;
		if (!tm)
		{
			not_empty_.wait(lock);
		}
		else if (!not_empty_.timed_wait(lock, *tm))
		{
			--not_empty_wait_;
			return timeout;
		}
		--not_empty_wait_;
	}

	if (!run_) return failed;

	len = length_;
	num = nelts_;
	msg = curr_;

	length_ = nelts_ = 0;
	curr_  = (curr_ == msg_1_ ? msg_2_ : msg_1_);

	if (not_full_wait_ > 0)	not_full_.notify_one();

	return succeed;
}

bool log_game_handler::connect()
{
	if (!run_) return false;

	if (sckt_.is_open()) return true;

	boost::system::error_code err;

	boost::asio::ip::address_v4 addr(address_);
	sckt_.connect(boost::asio::ip::tcp::endpoint(addr, port_), err);
	if (err) return false;

	return true;
}

void log_game_handler::run()
{
	LOG_GAME_WRITER * pckt;
	static int pckt_len = sizeof(LOG_GAME_WRITER);

	char * ptr = 0;

	char ** data;
	int num, pos, len, tmp;

	int buf_len = 2097152;
	char * buf = (char *)::malloc(2097152);

	boost::system::error_code err;
	milliseconds tm(5000);

	while (run_)
	{
		tmp = pop(data, num, len, &tm);

		if (tmp == failed) 
		{
			break;
		}
		else if (tmp == timeout) 
		{
			connect();
			continue;
		}

		tmp = len;
		len = len + pckt_len;

		if (len > buf_len)
		{
			::free(buf);
			buf = (char *)::malloc(len);
			buf_len = len;
		}

		pckt = (LOG_GAME_WRITER *)buf;
		pckt->serialize(tmp);
		pckt->src_len = num;

		pos = 0;
		for (int i = 0; i < num; i++)
		{
			tmp = strlen(data[i]);
			memcpy(buf + pckt_len + pos, data[i], tmp);
			pos += tmp;
		}

		boost::asio::write(sckt_, boost::asio::buffer(buf, 
			pckt->length), boost::asio::transfer_all(), err);

		if (err) sckt_.close();

		for (int i = 0; i < num; i++)
			delete [] data[i];
	}

	::free(buf);
}

