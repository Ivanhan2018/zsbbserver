/***************************************************************************
*   Copyright (C) 2007 by wangshenwang                                    *
*   wangshenwang1@gmail.com                                               *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
*   This program is distributed in the hope that it will be useful,       *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
*   GNU General Public License for more details.                          *
*                                                                         *
*   You should have received a copy of the GNU General Public License     *
*   along with this program; if not, write to the                         *
*   Free Software Foundation, Inc.,                                       *
*   59 Temple Place - Suite 330, Boston, MA  02111-1307, CHN.             *
***************************************************************************/

#ifndef __TASK_BASE_EX_HPP__
#define __TASK_BASE_EX_HPP__

#include "header.hpp"

template < class handler, class type>
class task_base_ex : private boost::noncopyable
{
	enum { max_queue_size = 1024 };

	typedef boost::shared_ptr<boost::thread> threads;
	typedef std::vector<threads> threads_ptr;

public:
	enum { FAILED = 0x00, SUCCEED = 0x01, TIMEOUT = 0x02 };

	typedef boost::shared_ptr<handler> handler_ptr;

	task_base_ex();

	virtual bool start(std::size_t = 1);
	virtual bool stop();

	int push(const handler_ptr &, type, boost::posix_time::seconds * = 0);
	int pop(handler_ptr &, type &, boost::posix_time::seconds * = 0);

	void thread_sleep(std::size_t);
	bool is_run() const { return run_; }
	std::size_t size() const { return nelts_; }

	virtual void run() = 0;
	virtual void exit(type) = 0;

private:
	bool run_;
	std::size_t nelts_;
	std::size_t in_;
	std::size_t out_;

	type msg_[max_queue_size];
	handler_ptr hdr_ptr_[max_queue_size];
	const std::size_t bounds_;

	boost::condition not_full_;
	std::size_t not_full_wait_;

	boost::condition not_empty_;
	std::size_t not_empty_wait_;

	boost::asio::detail::mutex mutex_;
	threads_ptr threads_;
};

template < class handler, class type >
task_base_ex<handler, type>::task_base_ex() : 
bounds_(max_queue_size)
{
	run_ = false;
	not_full_wait_ = not_empty_wait_ = 0;
	nelts_ = in_ = out_ = 0;
}

template < class handler, class type >
bool task_base_ex<handler, type>::start(std::size_t n_threads)
{
	logic_check(!n_threads, ("n_threads=%d", n_threads));

	if (run_) return false;

	boost::asio::detail::mutex::scoped_lock lock(mutex_);

	run_ = true;
	not_full_wait_ = not_empty_wait_ = 0;
	nelts_ = in_ = out_ = 0;

	for (std::size_t i = 0; i < n_threads; ++i)
	{
		boost::shared_ptr<boost::thread> thread(
			new boost::thread(boost::bind(&task_base_ex::run, this)));
		threads_.push_back(thread);
	}

	return true;
}

template < class handler, class type >
bool task_base_ex<handler, type>::stop()
{
	if (!run_) return false;

	{
		boost::asio::detail::mutex::scoped_lock lock(mutex_);
		run_ = false;
		not_empty_.notify_all();
	}
	{
		boost::asio::detail::mutex::scoped_lock lock(mutex_);
		not_full_.notify_all();
	}

	for (std::size_t i = 0; i < threads_.size(); ++i)
		threads_[i]->join();

	while (nelts_)
	{
		hdr_ptr_[out_].reset();
		exit(msg_[out_]);
		out_ = (out_ + 1) % bounds_;
		nelts_--;
	}

	not_full_wait_ = not_empty_wait_ = 0;
	nelts_ = in_ = out_ = 0;
	return true;
}

template < class handler, class type >
int task_base_ex<handler, type>::push(const handler_ptr & hdr_ptr, type msg, 
									   boost::posix_time::seconds * tm)
{
	if (!run_) return FAILED;

	boost::asio::detail::mutex::scoped_lock lock(mutex_);
	while (nelts_ == bounds_)
	{
		if (!run_) return FAILED;

		++not_full_wait_;
		if (!tm)
		{
			not_full_.wait(lock);
		}
		else if (!not_full_.timed_wait(lock, *tm))
		{
			--not_full_wait_;
			return TIMEOUT;
		}
		--not_full_wait_;
	}

	if (!run_) return FAILED;

	msg_[in_] = msg;
	hdr_ptr_[in_] = hdr_ptr;
	in_ = (in_ + 1) % bounds_;
	nelts_++;
	if (not_empty_wait_ > 0) not_empty_.notify_one();

	return SUCCEED;
}

template < class handler, class type >
int task_base_ex<handler, type>::pop(handler_ptr & hdr_ptr, type & msg,
									  boost::posix_time::seconds * tm)
{
	if (!run_) return FAILED;

	boost::asio::detail::mutex::scoped_lock lock(mutex_);
	while (nelts_ == 0)
	{
		if (!run_) return FAILED;

		++not_empty_wait_;
		if (!tm)
		{
			not_empty_.wait(lock);
		}
		else if (!not_empty_.timed_wait(lock, *tm))
		{
			--not_empty_wait_;
			return TIMEOUT;
		}
		--not_empty_wait_;
	}

	if (!run_) return FAILED;

	msg = msg_[out_];
	hdr_ptr = hdr_ptr_[out_];
	hdr_ptr_[out_].reset();
	out_ = (out_ + 1) % bounds_;
	nelts_--;
	if (not_full_wait_ > 0)	not_full_.notify_one();

	return SUCCEED;
}

template < class handler, class type >
void task_base_ex<handler, type>::thread_sleep(std::size_t ms)
{
#if defined(_WIN32)
	Sleep(ms);
#else
	usleep(ms * 1000);
#endif
}

#endif // __TASK_BASE_EX_HPP__
