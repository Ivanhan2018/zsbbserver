#include "request_handler.hpp"
#include "connection.hpp"
#include "distributed_protocol.hpp"

#define OTL_STL
#define OTL_ODBC

#if defined (WIN32)
#define OTL_BIGINT __int64
#else
#define OTL_BIGINT long long
#endif

#include "otlv4.h"

request_handler::request_handler()
{
}

bool request_handler::start(const char * db_uid, const char * db_pwd, const char * db_dsn)
{
	logic_check(!db_uid, ("db_uid = 0x%08x, %s", db_uid, 
		errorcode::error_message(errorcode::local_error_object_null)));

	logic_check(!db_pwd, ("db_pwd = 0x%08x, %s", db_pwd, 
		errorcode::error_message(errorcode::local_error_object_null)));

	logic_check(!db_dsn, ("db_dsn = 0x%08x, %s", db_dsn, 
		errorcode::error_message(errorcode::local_error_object_null)));

	if (is_run()) return false;

	db_con_str_ = "UID=";
	db_con_str_ += db_uid;

	db_con_str_ += ";PWD=";
	db_con_str_ += db_pwd;

	db_con_str_ += ";DSN=";
	db_con_str_ += db_dsn;

	return task_base_ex<connection, char *>::start();
}

bool request_handler::stop()
{
	if (!task_base_ex<connection, char *>::stop())
		return false;
	db_con_str_ = "";
	return true;
}

bool request_handler::handle_message(connection * conn, char * buffer, std::size_t length)
{
	if (!is_run()) return false;
	logic_check(!push(conn->shared_from_this(), buffer), ("logsvc service push error"));
	return true;
}

void request_handler::handle_disconnect(connection * conn, const boost::system::error_code& err)
{
}

void request_handler::run()
{
	otl_connect db;

	try 
	{
		db.rlogon(db_con_str_.c_str());
	}
	catch(otl_exception& p)
	{ 
		printf("FAILED\n%s\n\n", p.msg);
		db.logoff();
		return ;
	}

	handler_ptr conn;
	char * buffer;
	DISTRIBUTED_HEADER * header;
	LOG_REPORT_PERFORMANCE * perf;
	LOG_REPORT_INFO * info;
	LOG_REPORT_ONLINE * online;
	LOG_REPORT_GROUP_ONLINE * group_online;

	boost::posix_time::seconds timeout(2);

	std::string perf_str = "INSERT INTO log_perf(servername, bandwidth, broadcast"
		", cpu, mem, vmem, datetime, address) VALUES(:a1<char[128]>,:a2<int>,:a3<int>,:a4<int>,"
		":a5<int>,:a6<int>,:a7<char[25]>,:a8<char[50]>)";

	std::string info_str = "INSERT INTO log_info(servername, info, address, datetime)"
		"VALUES(:a1<char[128]>,:a2<char[300]>,:a3<char[50]>,:a4<char[50]>)";

	std::string online_str = "INSERT INTO log_online(servername, online, datetime, address)"
		"VALUES(:a1<char[128]>,:a2<int>,:a3<char[50]>,:a4<char[50]>)";

	std::string group_online_str = "INSERT INTO grouponline(groupname, datetime, online, address)"
		"VALUES(:a1<char[128]>,:a2<char[50]>,:a3<int>,:a4<char[50]>)";

	std::string proc = "{CALL logsvc_handler}";

	while (is_run())
	{
		int ret = pop(conn, buffer, &timeout);
		if (ret == FAILED) break;
		if (ret == TIMEOUT)
		{
			if (perf_.size())
			{
				otl_stream os((int)perf_str.size(), perf_str.c_str(), db);
				while (!perf_.empty())
				{
					buffer = perf_.front();
					perf = (LOG_REPORT_PERFORMANCE *)buffer;
					boost::asio::ip::address_v4 addr(perf->agent);

					os << perf->proc_name << perf->bandwidth << perf->broadcast 
						<< perf->cpu << perf->mem << perf->vmem << perf->datetime 
						<< addr.to_string().c_str();
					perf_.pop();
					delete [] buffer;
				}
			}

			if (info_.size())
			{
				otl_stream os((int)info_str.size(), info_str.c_str(), db);
				while (!info_.empty())
				{
					buffer = info_.front();
					info = (LOG_REPORT_INFO *)buffer;
					boost::asio::ip::address_v4 addr(info->agent);
					os << info->proc_name << info->msg 
						<< addr.to_string().c_str()
						<< info->datetime;
					info_.pop();
					delete [] buffer;
				}
			}

			if (online_.size())
			{
				otl_stream os((int)online_str.size(), online_str.c_str(), db);
				while (!online_.empty())
				{
					buffer = online_.front();
					online = (LOG_REPORT_ONLINE *)buffer;
					boost::asio::ip::address_v4 addr(online->agent);
					os << online->proc_name << online->online 
						<< online->datetime
						<< addr.to_string().c_str();
					online_.pop();
					delete [] buffer;
				}
				
				os.close();
				otl_stream o((int)proc.size(), proc.c_str(), db);
				db.commit();
			}

			if (group_online_.size())
			{
				otl_stream os((int)group_online_str.size(), group_online_str.c_str(), db);
				while (!group_online_.empty())
				{
					buffer = group_online_.front();
					group_online = (LOG_REPORT_GROUP_ONLINE *)buffer;
					boost::asio::ip::address_v4 addr(group_online->agent);
					os << group_online->group << group_online->datetime << group_online->online	<< addr.to_string().c_str();
					group_online_.pop();
					delete [] buffer;
				}
			}
		}
		else
		{
			header = (DISTRIBUTED_HEADER *)buffer;
			header->agent = conn->socket().remote_endpoint().address().to_v4().to_ulong();
			if (header->command == CMD_LOG_REPORT_PERFORMANCE)
			{
				delete [] buffer;
				//perf_.push(buffer);
			}
			else if (header->command == CMD_LOG_REPORT_INFO)
			{
				delete [] buffer;
				//info_.push(buffer);
			}
			else if (header->command == CMD_LOG_REPORT_ONLINE)
			{
				delete [] buffer;
				//online_.push(buffer);
			}
			else if (header->command == CMD_LOG_REPORT_GROUP_ONLINE)
			{
				group_online_.push(buffer);
			}
			conn.reset();
		}
	}
}

void request_handler::exit(char * buf)
{
	logic_check(!buf, ("buffer = 0x%08x, %s", buf, 
		errorcode::error_message(errorcode::local_error_object_null)));

	delete [] buf;
}