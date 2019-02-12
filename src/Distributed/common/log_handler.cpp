#include "log_handler.hpp"
#include <io.h>
#include <direct.h>
#include "distributed_protocol.hpp"
#ifdef _WIN32
#   include <sys/timeb.h>
#   include <time.h>
#else
#   include <sys/time.h>
#endif

//#include <psapi.h>
//#pragma comment( lib, "Psapi.lib")

log_handler::log_handler() : run_(false), socket_(io_service_)
{
}

void log_handler::start(int from, const char * ip, short port)
{
	if (run_) return ;

	long path_len = ::GetModuleFileName(0, proc_name_, 128);
	while (proc_name_[path_len--] != '\\');
	sprintf(proc_name_, "%s_%d", proc_name_ + path_len + 2, from);

	boost::asio::ip::tcp::endpoint endpoint(boost::asio::
		ip::address::from_string(ip), port);

	boost::system::error_code err;
	socket_.connect(endpoint, err);

	logic_check(err, ("address(%s), port(%d), %s", 
		ip, port, err.message().c_str()));

	run_ = true;


	//bandwidth_sec_ = broadcast_bandwidth_sec_ = 0;
	//char path[512];
	//long path_len = ::GetModuleFileName(0, path, 512);
	//while (path[path_len--] != '\\');
	//path[path_len + 2] = 0;

	//strcat(path, "performance_log\\");
	//if (::access(path, 0)) ::mkdir(path);

	//char name[MAX_PATH];
	//long name_len = ::GetConsoleTitle(name, MAX_PATH);
	//while (name_len && name[name_len--] != '\\');
	//name_len = (name_len < 1 ? 0 : name_len + 2);
	//strcat(path, name + name_len);

	//time_t t = ::time(0);
	//tm * fat = ::gmtime(&t);

	//char tmc[MAX_PATH];
	//sprintf(tmc, " %d-%d-%d.csv", fat->tm_year + 1900, fat->tm_mon + 1, fat->tm_mday);
	//strcat(path, tmc);

	//fout_.open(path, std::ios::out | std::ios::app);
	//fout_ << "带宽,广播,CPU,内存,虚拟内存,读IO,写IO,时间\n"; 

	//boost::shared_ptr<boost::thread> thread(
	//	new boost::thread(boost::bind(&log_handler::print, this)));
	//threads_ = thread;
}

void log_handler::stop()
{
	if (!run_) return ;

	run_ = false;
	//threads_->join();
	//	fout_.close();
	socket_.close();
}

bool log_handler::report(const char * msg)
{
	//if (!run_ || !msg) return false;

	//if (!socket_.is_open()) return false;

	//int len = strlen(msg);
	//logic_check(len >= 290, ("len = %d, %s", len, 
	//	errorcode::error_message(errorcode::local_error_buffer_lenght)));

	//time_t tb = ::time(0);
	//tm * fat = ::gmtime(&tb);

	//char tmc[MAX_PATH];
	//sprintf(tmc, " %04d-%02d-%02d %02d:%02d:%02d", fat->tm_year + 1900, fat->tm_mon + 1, 
	//	fat->tm_mday, (fat->tm_hour + 8)%24, fat->tm_min, fat->tm_sec);

	//LOG_REPORT_INFO rpt;
	//rpt.serialize(proc_name_, tmc, msg);

	//boost::system::error_code err;
	//boost::asio::write(socket_, boost::asio::buffer((char *)&rpt, rpt.length), boost::asio::transfer_all(), err);
	//if (err) return false;

	return true;
}

bool log_handler::online(int num)
{
	//if (!run_) return false;

	//if (!socket_.is_open()) return false;

	//time_t tb = ::time(0);
	//tm * fat = ::gmtime(&tb);

	//char tmc[MAX_PATH];
	//sprintf(tmc, " %04d-%02d-%02d %02d:%02d:%02d", fat->tm_year + 1900, fat->tm_mon + 1, 
	//	fat->tm_mday, (fat->tm_hour + 8)%24, fat->tm_min, fat->tm_sec);

	//LOG_REPORT_ONLINE rpt;
	//rpt.serialize(proc_name_, tmc, num);

	//boost::system::error_code err;
	//boost::asio::write(socket_, boost::asio::buffer((char *)&rpt, rpt.length), boost::asio::transfer_all(), err);
	//if (err) return false;

	return true;
}

bool log_handler::online(const char * group, int num)
{
	if (!run_) return false;
	if (!socket_.is_open()) return false;
	if (!group) return false;

	struct _timeb tb;
	_ftime(&tb);
	tb.time -= (tb.timezone * 60);
	tm * fat = ::gmtime(&tb.time);

	char tmc[MAX_PATH];
	sprintf(tmc, " %04d-%02d-%02d %02d:%02d:%02d", fat->tm_year + 1900, fat->tm_mon + 1, 
		fat->tm_mday, fat->tm_hour, fat->tm_min, fat->tm_sec);

	LOG_REPORT_GROUP_ONLINE rpt;
	rpt.serialize(group, tmc, num);

	boost::system::error_code err;
	boost::asio::write(socket_, boost::asio::buffer((char *)&rpt, rpt.length), boost::asio::transfer_all(), err);
	if (err) return false;

	return true;
}

void log_handler::add_bandwidth(unsigned short bandwidth, unsigned short type, unsigned short num)
{
	//if (num == 0) return ;

	//if (type == 2) ::InterlockedExchangeAdd(&broadcast_bandwidth_sec_, (bandwidth * num));
	//::InterlockedExchangeAdd(&bandwidth_sec_, (bandwidth * num));
}

unsigned __int64 log_handler::file_time_2_utc(const FILETIME * ftime)   
{   
	//LARGE_INTEGER li;   

	//li.LowPart = ftime->dwLowDateTime;   
	//li.HighPart = ftime->dwHighDateTime;   
	//return li.QuadPart;   
	return 0;
}   

bool log_handler::get_cpu_memory_io_usage(int & cpu, int & mem, int & vmem)
{
	//static int processor_count_ = -1;
	//static __int64 last_time_ = 0;
	//static __int64 last_system_time_ = 0;

	//FILETIME now;
	//FILETIME creation_time;
	//FILETIME exit_time;
	//FILETIME kernel_time;
	//FILETIME user_time;
	//__int64 system_time;
	//__int64 time;
	//__int64 system_time_delta;
	//__int64 time_delta;

	//cpu = -1;


	//if(processor_count_ == -1)
	//{
	//	SYSTEM_INFO info;
	//	GetSystemInfo(&info);
	//	processor_count_ = (int)info.dwNumberOfProcessors;
	//}

	//GetSystemTimeAsFileTime(&now);

	//if (!GetProcessTimes(GetCurrentProcess(), &creation_time, &exit_time,
	//	&kernel_time, &user_time))
	//{
	//	return false;
	//}

	//system_time = (file_time_2_utc(&kernel_time) + file_time_2_utc(&user_time)) / processor_count_;
	//time = file_time_2_utc(&now);

	//if ((last_system_time_ == 0) || (last_time_ == 0))
	//{
	//	last_system_time_ = system_time;
	//	last_time_ = time;
	//	return false;
	//}

	//system_time_delta = system_time - last_system_time_;
	//time_delta = time - last_time_;

	//if (time_delta == 0) return false;

	//cpu = (int)((system_time_delta * 100 + time_delta / 2) / time_delta);
	//last_system_time_ = system_time;
	//last_time_ = time;

	//PROCESS_MEMORY_COUNTERS pmc;
	//if(!GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc)))
	//	return false;

	//mem = (int)(pmc.WorkingSetSize / 1024);
	//vmem = (int)(pmc.PagefileUsage / 1024);
	return true;
}   

void log_handler::print()
{
	//long _bandwidth_sec, _bandwidth_sec_old;
	//long _broadcast_bandwidth_sec, _broadcast_bandwidth_sec_old;
	//int cpu, mem, vmem;
	//int cpu_old, mem_old, vmem_old;

	//LOG_REPORT_PERFORMANCE rpt;

	//boost::system::error_code err;
	//_bandwidth_sec_old = _broadcast_bandwidth_sec_old = 0;
	//cpu_old = mem_old = vmem_old = 0;

	//int count = 0;
	//while (run_)
	//{
	//	_bandwidth_sec = ::InterlockedExchange(&bandwidth_sec_, 0);
	//	_broadcast_bandwidth_sec = ::InterlockedExchange(&broadcast_bandwidth_sec_, 0);

	//	if (get_cpu_memory_io_usage(cpu, mem, vmem))
	//	{
	//		if ((_bandwidth_sec_old == _bandwidth_sec) &&
	//			(_broadcast_bandwidth_sec_old == _broadcast_bandwidth_sec) &&
	//			(cpu_old == cpu) && (mem_old == mem) && (vmem_old == vmem))
	//		{
	//		}
	//		else
	//		{
	//			_bandwidth_sec_old = _bandwidth_sec;
	//			_broadcast_bandwidth_sec_old = _broadcast_bandwidth_sec;
	//			cpu_old = cpu;
	//			mem_old = mem;
	//			vmem_old = vmem;

	//			if (!(++count % 30))
	//			{
	//				time_t tb = ::time(0);
	//				tm * fat = ::gmtime(&tb);

	//				char tmc[MAX_PATH];
	//				sprintf(tmc, " %04d-%02d-%02d %02d:%02d:%02d", fat->tm_year + 1900, fat->tm_mon + 1, 
	//					fat->tm_mday, (fat->tm_hour + 8)%24, fat->tm_min, fat->tm_sec);

	//				rpt.serialize(proc_name_, _bandwidth_sec, _broadcast_bandwidth_sec, cpu, mem, vmem, tmc);
	//				boost::asio::write(socket_, boost::asio::buffer((char*)&rpt, rpt.length), boost::asio::transfer_all(), err);
	//				if (err) break;
	//			}

	//			//fout_ << offset;
	//		}
	//	}

	//	Sleep(1000);
	//}
}
