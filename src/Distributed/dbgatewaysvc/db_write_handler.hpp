#ifndef __DB_WRITE_HANDLER_HPP__
#define __DB_WRITE_HANDLER_HPP__

#include "log.hpp"

class db_write_handler : private boost::noncopyable
{
	enum { max_queue_size = 200000 };
	typedef boost::shared_ptr<boost::thread> thread_ptr;

public:
	explicit db_write_handler();

	static db_write_handler & instance()
	{static db_write_handler dh; return dh;}

	bool start(const char *);
	bool stop();

	bool push(char *);

	std::string db_con_str_;

	void print();
	long select_;
	long update_;
	long insert_;
	long delete_;
	long oper_sql_count_;
	thread_ptr thread_print_;
	std::ofstream fout_;


private:
	bool pop(char **&, int &);
	void run();

private:
	otl_connect db_;

	bool run_;
	int nelts_;

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

void db_write_handler::print()
{
	long lselect, lupdate, ldelete, linsert, count;
	long sss;
	char buf[256];

	while (run_)
	{
		lselect = ::InterlockedExchange(&select_, 0);
		lupdate = ::InterlockedExchange(&update_, 0);
		ldelete = ::InterlockedExchange(&delete_, 0);
		linsert = ::InterlockedExchange(&insert_, 0);
		sss = ::InterlockedExchange(&oper_sql_count_, 0);
		count = lselect + linsert + ldelete + lupdate;

		if (count > 0 || sss > 0)
		{
			sprintf(buf, "%d,%d,%d,%d,%d,%d\n", lselect, linsert, ldelete, lupdate, count, sss);
			fout_ << buf;
		}

		Sleep(1000);
	}
}

db_write_handler::db_write_handler() :
bounds_(max_queue_size)
{
	run_ = false;
	nelts_ = not_full_wait_ = not_empty_wait_ = 0;

	select_ = update_ = delete_ = insert_ = 0;
	oper_sql_count_ = 0;

	char path[MAX_PATH];
	long path_len = ::GetModuleFileName(0, path, MAX_PATH);
	while (path[path_len--] != '\\');
	path[path_len + 2] = 0;

	strcat(path, "log_csv\\");
	if (access(path, 0)) mkdir(path);

	char name[MAX_PATH];
	long name_len = ::GetConsoleTitle(name, MAX_PATH);
	while (name_len && name[name_len--] != '\\');
	name_len = (name_len < 1 ? 0 : name_len + 2);
	strcat(path, name + name_len);

	time_t t = ::time(0);
	tm * fat = ::gmtime(&t);

	char tmc[20];
	sprintf(tmc, " %4.4d-%2.2d-%2.2d.csv", fat->tm_year + 1900,
		fat->tm_mon + 1, fat->tm_mday);

	strcat(path, tmc);

	fout_.open(path, std::ios::out | std::ios::app);

	fout_ << "查询,添加,删除,更新,合计,操作DB\n";
}

bool db_write_handler::start(const char * connstr)
{
	if (run_) return false;

	boost::asio::detail::mutex::scoped_lock lock(mutex_);
	try 
	{ 
		db_.rlogon(connstr);
	}
	catch(otl_exception&)
	{ 
		db_.logoff();
		return false;
	}

	db_con_str_ = connstr;
	run_ = true;
	curr_ = msg_1_;
	nelts_ = not_full_wait_ = not_empty_wait_ = 0;

	thread_.reset(new boost::thread(
		boost::bind(&db_write_handler::run, this)));

	thread_print_.reset(new boost::thread(
		boost::bind(&db_write_handler::print, this)));

	return true;
}

bool db_write_handler::stop()
{
	if (!run_) return false;
	{
		boost::asio::detail::mutex::scoped_lock lock(mutex_);
		run_ = false;
		not_empty_.notify_all();
		not_full_.notify_all();
	}

	thread_->join();

	thread_print_->join();

	not_full_wait_ = not_empty_wait_ = 0;
	nelts_ = 0;
	curr_ = msg_1_;
	db_.logoff();

	return true;
}

bool db_write_handler::push(char * msg)
{
	if (!run_) return false;

	::InterlockedIncrement(&oper_sql_count_);

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
	if (not_empty_wait_ > 0) not_empty_.notify_one();

	return true;
}

bool db_write_handler::pop(char **& msg, int & num)
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

	num = nelts_;
	msg = curr_;

	nelts_ = 0;
	curr_  = (curr_ == msg_1_ ? msg_2_ : msg_1_);

	if (not_full_wait_ > 0)	not_full_.notify_one();

	return true;
}

void db_write_handler::run()
{
	int num, pos, len;
	char ** data;
	char sql_buf[8000];
	db_.set_max_long_size(8000);
	static int sql_header_len = (int)strlen("exec(\'");

	while (pop(data, num))
	{
		pos = sprintf(sql_buf, "%s", "exec(\'");
		for (int i = 0; i < num; i++)
		{
			len = *(int *)data[i];
			if ((pos + len) >= 7500)
			{
				pos += sprintf(sql_buf + pos, "%s", " \')");
				sql_buf[pos] = '\0';
				try
				{
					otl_cursor::direct_exec(db_, sql_buf);
					db_.commit();
				}
				catch(otl_exception& p)
				{ 
					std::cerr << p.msg << std::endl;
					std::cerr << p.stm_text << std::endl;
					std::cerr << p.sqlstate << std::endl;
					std::cerr << p.var_info << std::endl;
				}
				pos = sprintf(sql_buf, "%s", "exec(\'");
			}
			memcpy(sql_buf + pos, data[i] + 4, len);
			pos += len;
		}

		if (pos > sql_header_len)
		{
			pos += sprintf(sql_buf + pos, "%s", " \')");
			sql_buf[pos] = '\0';
			try
			{
				otl_cursor::direct_exec(db_, sql_buf);
				db_.commit();
			}
			catch(otl_exception& p)
			{ 
				std::cerr << p.msg << std::endl;
				std::cerr << p.stm_text << std::endl;
				std::cerr << p.sqlstate << std::endl;
				std::cerr << p.var_info << std::endl;
			}
		}

		for (int i = 0; i < num; i++)
			delete [] data[i];
	}

	if (nelts_ < 1) return ;

	data = curr_;
	num = nelts_;

	pos = sprintf(sql_buf, "%s", "exec(\'");
	for (int i = 0; i < num; i++)
	{
		len = *(int *)data[i];
		if ((pos + len) >= 7500)
		{
			pos += sprintf(sql_buf + pos, "%s", " \')");
			sql_buf[pos] = '\0';
			try
			{
				otl_cursor::direct_exec(db_, sql_buf);
				db_.commit();
			}
			catch(otl_exception& p)
			{ 
				std::cerr << p.msg << std::endl;
				std::cerr << p.stm_text << std::endl;
				std::cerr << p.sqlstate << std::endl;
				std::cerr << p.var_info << std::endl;
			}
			pos = sprintf(sql_buf, "%s", "exec(\'");
		}
		memcpy(sql_buf + pos, data[i] + 4, len);
		pos += len;
	}

	if (pos > sql_header_len)
	{
		pos += sprintf(sql_buf + pos, "%s", " \')");
		sql_buf[pos] = '\0';
		try
		{
			otl_cursor::direct_exec(db_, sql_buf);
			db_.commit();
		}
		catch(otl_exception& p)
		{ 
			std::cerr << p.msg << std::endl;
			std::cerr << p.stm_text << std::endl;
			std::cerr << p.sqlstate << std::endl;
			std::cerr << p.var_info << std::endl;
		}
	}

	for (int i = 0; i < num; i++)
		delete [] data[i];
}


#endif // __DB_WRITE_HANDLER_HPP__
