#ifndef __TIMER_HANDLER_HPP__
#define __TIMER_HANDLER_HPP__

#include "task_base.hpp"
#include "socketsvc.hpp"

class mytimer : public boost::asio::deadline_timer
{
public:
	mytimer(boost::asio::io_service&, const duration_type&);

	long id_;
	bool cancel_;
	const void * param1_;
	const void * param2_;
};

class timer_handler
{
	typedef boost::shared_ptr<boost::
		asio::io_service> io_service_ptr;

	typedef boost::shared_ptr<boost::
		asio::io_service::work> work_ptr;

	typedef boost::shared_ptr<boost::
		thread> threads_ptr;

	typedef stdext::hash_map<long, 
		mytimer *> timer_hash;

public:
	bool start(TIMER_NOTIFY timer_notify);
	bool stop();

	long schedule_timer(u_long, const void *, const void *);
	long schedule_timer_ms(u_long, const void *, const void *);
	bool cancel_timer(long);

private:
	void handle_timeout(const boost::system::error_code&, mytimer *);

	io_service_ptr io_service_ptr_;
	work_ptr work_ptr_;
	threads_ptr threads_ptr_;

	boost::mutex mutex_;
	timer_hash timers_;

	long tid_;
	TIMER_NOTIFY timer_notify_;
};

#endif // __TIMER_HANDLER_HPP__

