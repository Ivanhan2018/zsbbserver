#include "server.hpp"
#include "timer_handler.hpp"
//#include "log_handler.hpp"
#include "log_game_handler.hpp"

#ifdef _WIN32
#   include <sys/timeb.h>
#   include <time.h>
#else
#   include <sys/time.h>
#endif

boost::thread * __run_thread = 0;
request_handler * __handler = 0;
server * __svc = 0;
timer_handler * __timer = 0;
//log_handler * __log = 0;
log_game_handler * __log_game = 0;

void * operator new(std::size_t size)
{
	return ::malloc(((size >> 7) + !(!(size&127))) << 7);
}

void * operator new[](std::size_t size)
{
	return ::malloc(((size >> 7) + !(!(size&127))) << 7);
}

int SOCKETSVC_API init(const char * manager_address,
					   unsigned short manager_port,
					   const char * log_address,
					   unsigned short log_port,
					   int local_server_id,
					   unsigned int num_threads, 
					   DATA_RECEIVER receiver_func, 
					   EVENT_NOTIFY notify_func,
					   TIMER_NOTIFY timer_func,
					   SOCKET_DISCONNECT disconn_func,
					   LOGIC_EXCEPTION exception_func)
{
	if (__run_thread) return 0;

	__handler = new request_handler;
	__svc = new server(num_threads, __handler, 0);

	if (!__handler->start(manager_address, manager_port, num_threads, 
		local_server_id, __svc->get_listen_port(), __svc->get_io_service(), 
		receiver_func, notify_func, disconn_func, exception_func))
	{
		delete __svc;
		__svc = 0;
		delete __handler;
		__handler = 0;
		return 0;
	}

	//__log = new log_handler();
	//__log->start(local_server_id, log_address, log_port);

	__log_game = new log_game_handler();
	__log_game->start(__handler->log_game_address(), __handler->log_game_port());

	__run_thread = new boost::thread(boost::bind(&server::
		io_service_run_event_loop, __svc));

	__timer = new timer_handler();
	__timer->start(timer_func);

	return 1;
}

void SOCKETSVC_API fini()
{
	if (!__run_thread) return ;

	__svc->io_service_end_event_loop();
	__handler->stop();
	__run_thread->join();
	__timer->stop();
	//__log->stop();
	__log_game->stop();

	delete __svc;
	__svc = 0;

	delete __handler;
	__handler = 0;

	delete __run_thread;
	__run_thread = 0;

	delete __timer;
	__timer = 0;

	//delete __log;
	//__log = 0;

	delete __log_game;
	__log_game = 0;
}

int SOCKETSVC_API asynch_write(int agent_id, char * buffer, int length)
{
	if (!__run_thread) return 0;

	if (!__handler->start_async_write(agent_id, buffer, length))
		return 0;
	return 1;
}

int SOCKETSVC_API event_notify(int server_id, char * buffer, int length)
{
	if (!__run_thread) return 0;

	if (!__handler->event_notify(server_id, buffer, length))
		return 0;
	return 1;
}

int SOCKETSVC_API broadcast(char * buffer, int length)
{
	if (!__run_thread) return 0;

	if (!__handler->broadcast(__handler->role_id(), buffer, length))
		return 0;
	return 1;
}

int SOCKETSVC_API broadcast_ex(int id, char * buffer, int length)
{
	if (!__run_thread) return 0;

	if (!__handler->broadcast(id, buffer, length))
		return 0;
	return 1;
}

long SOCKETSVC_API set_timer(unsigned long interval, const void * param_1, const void * param_2)
{
	if (!__run_thread) return 0;
	return __timer->schedule_timer(interval, param_1, param_2);
}

long SOCKETSVC_API set_timer_ms(unsigned long interval, const void * param_1, const void * param_2)
{
	if (!__run_thread) return 0;
	return __timer->schedule_timer_ms(interval, param_1, param_2);
}

int SOCKETSVC_API cancel_timer(long id)
{
	if (!__run_thread) return 0;
	return __timer->cancel_timer(id);
}

const char * SOCKETSVC_API get_error_message(int code)
{
	return errorcode::error_message(code);
}

int SOCKETSVC_API report(const char * msg)
{
	if (!__run_thread) return 0;

	//if (__log->report(msg))
	return 1;
	return 0;
}

int SOCKETSVC_API online(int num)
{
	if (!__run_thread) return 0;

	//if (__log->online(num))
	return 1;
	return 0;
}

int SOCKETSVC_API log_game_report(const char * fmt, ...)
{
	if (!__run_thread) return 0;


	if (!__log_game->connected()) return 0;

	struct _timeb tb;
	_ftime(&tb);
	tb.time -= (tb.timezone * 60);
	tm * fat = ::gmtime(&tb.time);

	char * buf = new char[1024];
	int len = sprintf(buf, "%d,%d,%d,%04d-%02d-%02d %02d:%02d:%02d.%03d,", 
		__handler->game_id(), __handler->group_id(), __handler->role_id(), 
		fat->tm_year + 1900, fat->tm_mon + 1, fat->tm_mday, fat->tm_hour, 
		fat->tm_min, fat->tm_sec, tb.millitm);

	va_list ap;
	va_start(ap,fmt);
	len += vsprintf(buf + len, fmt, ap);
	va_end(ap);

	if (__log_game->push(buf, len))
		return 1;

	delete [] buf;

	return 0;

	return 1;
}

