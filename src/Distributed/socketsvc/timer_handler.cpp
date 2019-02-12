#include "timer_handler.hpp"

mytimer::mytimer(boost::asio::io_service& io_service, const duration_type& expiry_time) :
boost::asio::deadline_timer(io_service, expiry_time)
{
}

bool timer_handler::start(TIMER_NOTIFY timer_notify)
{
	timer_notify_ = timer_notify;

	io_service_ptr_.reset(new boost::asio::io_service);
	work_ptr_.reset(new boost::asio::io_service::work(*io_service_ptr_));

	threads_ptr_.reset(new boost::thread(boost::bind(
		&boost::asio::io_service::run, io_service_ptr_)));

	tid_ = 0;

	return true;
}

bool timer_handler::stop()
{
	{
		boost::mutex::scoped_lock lock(mutex_);
		for (timer_hash::iterator it = timers_.begin(); 
			it != timers_.end(); it++)
		{
			it->second->cancel_ = true;
			it->second->cancel();
		}
	}

	while (timers_.size()) Sleep(1);

	io_service_ptr_->stop();
	threads_ptr_->join();

	return true;
}

long timer_handler::schedule_timer(u_long interval, const void * param_1, const void * param_2)
{
	if (!interval) return false;

	mytimer * t = new mytimer(*io_service_ptr_, boost::posix_time::milliseconds(interval * 1000));

	t->cancel_ = false;
	t->param1_ = param_1;
	t->param2_ = param_2;

	{
		boost::mutex::scoped_lock lock(mutex_);
		t->id_ = ++tid_;
		timers_.insert(std::pair<int, mytimer *>(tid_, t));
	}

	t->async_wait(boost::bind(&timer_handler::handle_timeout, 
		this, boost::asio::placeholders::error, t));

	return tid_;
}

long timer_handler::schedule_timer_ms(u_long interval, const void * param_1, const void * param_2)
{
	if (!interval) return false;

	mytimer * t = new mytimer(*io_service_ptr_, boost::posix_time::milliseconds(interval));

	t->cancel_ = false;
	t->param1_ = param_1;
	t->param2_ = param_2;

	{
		boost::mutex::scoped_lock lock(mutex_);
		t->id_ = ++tid_;
		timers_.insert(std::pair<int, mytimer *>(tid_, t));
	}

	t->async_wait(boost::bind(&timer_handler::handle_timeout, 
		this, boost::asio::placeholders::error, t));

	return tid_;
}

bool timer_handler::cancel_timer(long time_id)
{
	timer_hash::iterator it;
	boost::mutex::scoped_lock lock(mutex_);
	it = timers_.find(time_id);
	if (it == timers_.end()) return false;
	if (it->second->cancel_) return false;

	it->second->cancel_ = true;
	it->second->cancel();
	return true;
}

void timer_handler::handle_timeout(const boost::system::error_code& err, mytimer * t)
{
	long id = t->id_;
	bool flag = t->cancel_;
	const void * param_1 = t->param1_;
	const void * param_2 = t->param2_;
	{
		boost::mutex::scoped_lock lock(mutex_);
		timers_.erase(t->id_);
		delete t;
		if (flag) return ;
	}

	if (!err) timer_notify_(param_1, param_2, id);
}

