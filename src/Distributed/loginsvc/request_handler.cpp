#include "request_handler.hpp"
#include "connection.hpp"
#include "distributed_protocol.hpp"
#include "zlib.h"
#include "base64.h"

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

bool request_handler::start(std::size_t n_threads, long version, const char * db_uid, 
							const char * db_pwd, const char * db_dsn)
{
	logic_check(!n_threads || n_threads > 200, ("n_threads = %d, %s", n_threads, 
		errorcode::error_message(errorcode::local_error_thread_num_overflow)));

	logic_check(!db_uid, ("db_uid = 0x%08x, %s", db_uid, 
		errorcode::error_message(errorcode::local_error_object_null)));

	logic_check(!db_pwd, ("db_pwd = 0x%08x, %s", db_pwd, 
		errorcode::error_message(errorcode::local_error_object_null)));

	logic_check(!db_dsn, ("db_dsn = 0x%08x, %s", db_dsn, 
		errorcode::error_message(errorcode::local_error_object_null)));

	if (is_run()) return false;

	version_ = version;

	db_con_str_ = "UID=";
	db_con_str_ += db_uid;

	db_con_str_ += ";PWD=";
	db_con_str_ += db_pwd;

	db_con_str_ += ";DSN=";
	db_con_str_ += db_dsn;

	if (!task_base_ex<connection, char *>::start(n_threads))
		return false;

	//threads_ptr_.reset(new boost::thread(boost::bind(
	//	&request_handler::print, this)));

	return true;
}

bool request_handler::stop()
{
	if (!task_base_ex<connection, char *>::stop())
		return false;

	//threads_ptr_->join();

	{
		boost::mutex::scoped_lock lock(mutex_);
		for (group_map::iterator it = group_.begin(); it != group_.end(); it++)
			delete it->second;
	}

	version_ = 0;
	db_con_str_ = "";
	return true;
}

//void request_handler::print()
//{
//	int count = 0;
//	while (is_run())
//	{
//		if (!(++count % 5)) 
//			printf("在线人数: %d\n", user_.size());
//			
//		thread_sleep(1000);
//	}
//}

bool request_handler::handle_message(connection * conn, char * buffer, std::size_t length)
{
	if (!is_run()) return false;
	logic_check(!push(conn->shared_from_this(), buffer), ("login service push error"));
	return true;
}

void request_handler::handle_disconnect(connection * conn, const boost::system::error_code& err)
{
	logic_check(!conn, ("conn = 0x%08x, %s", conn, 
		errorcode::error_message(errorcode::local_error_object_null)));

	if (!conn->id()) return ;

	boost::mutex::scoped_lock lock(mutex_);
	group_map::iterator it = group_.find(conn->group_name());
	if (it == group_.end()) return ;

	agent_map::iterator itagent = it->second->find(conn->id());
	if (itagent == it->second->end()) return ;

	it->second->erase(itagent);

	//user_map::iterator tmp, ita = user_.begin();
	//while (ita != user_.end())
	//{
	//	tmp = ita;
	//	ita++;
	//	if (tmp->second == conn->id())
	//		user_.erase(tmp);
	//}
}

bool decrypt(char * desc, int & desc_len, char * src, int src_len)
{
	static int header_len = sizeof(DISTRIBUTED_HEADER);

	DISTRIBUTED_HEADER * header = (DISTRIBUTED_HEADER *)src;
	char * ptr = src + header_len;
	uLong ptr_len = src_len - header_len;

	if (!header->compress)
	{
		::memcpy(desc, src, src_len);
		return true;
	}

	::memcpy(desc, src, header_len);

	if (uncompress((Bytef *)(desc + header_len),
		(uLong*)&desc_len, (Bytef *)ptr, ptr_len) != Z_OK)
	{
		return false;
	}

	header = (DISTRIBUTED_HEADER *)desc;
	header->length = desc_len + header_len;
	header->src_len = 0;
	header->compress = 0;
	return true;
}

void request_handler::run()
{
	unsigned char key[] = "123456789ABCDEF";
	std::string qry = "SELECT accountid, password, accounttype "
		"FROM GameUser with (nolock) WHERE accountname=:f<char[50]>";

	std::string inst = "INSERT INTO loginlog(playerid, "
		"playerip, playercpuid, type) VALUES (:a1<int>,"
		":a2<char[50]>,:a3<int>,:a4<char[50]>)";

	int accountid, accounttype;
	char encpass[50];
	char decpass[50];
	char buffer[512];


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
	char * buf;
	static std::size_t len = sizeof(LOGIN_REGISTER_RESP_CLIENT);
	CLIENT_REGISTER_LOGIN * req;
	LOGIN_REGISTER_RESP_CLIENT * resp;

	while (pop(conn, buf))
	{
		req = (CLIENT_REGISTER_LOGIN *)buf;
		if (req->command == CMD_AGENT_REPORT_LOGIN)
		{
			AGENT_REPORT_LOGIN * agent = (AGENT_REPORT_LOGIN *)buf;
			conn->id() = agent->id;
			conn->url() = agent->address;
			conn->port() = agent->port;
			conn->group_name() = agent->group_name;

			boost::mutex::scoped_lock lock(mutex_);
			agent_map * agents;
			group_map::iterator it = group_.find(agent->group_name);
			if (it == group_.end())
			{
				agents = new agent_map();
				group_.insert(std::pair<std::string, agent_map *>(agent->group_name, agents));
			}
			else
			{
				agents = it->second;
				agents->erase(agent->id);
			}

			agents->insert(std::pair<int, handler_ptr>(agent->id, conn));
		}
		else if (req->command == CMD_AGENT_CLIENT_LEAVE_SERVER)
		{
			AGENT_CLIENT_LEAVE_SERVER * leave = (AGENT_CLIENT_LEAVE_SERVER *)buf;
			otl_stream os1((int)inst.size(), inst.c_str(), db);
			os1 << leave->to << "UNKNOWN" << 0 << "LOGOUT";
			os1.close();

			//boost::mutex::scoped_lock lock(mutex_);
			//user_.erase(leave->to);
			if (conn->online() > 0) --conn->online();
		}
		else if (req->command == CMD_CLIENT_REGISTER_LOGIN)
		{
			int len_1 = 512;
			memset(buffer, 0, 512);
			decrypt(buffer, len_1, buf, req->length);
			req = (CLIENT_REGISTER_LOGIN *)buffer;

			if (req->version != version_)
			{
				char * tmp = new char[len];
				resp = (LOGIN_REGISTER_RESP_CLIENT *)tmp;
				resp->serialize(errorcode::client_login_error_version);
				conn->start_async_write((char *)resp, resp->length);
			}
			else if (!req->accounts[0])
			{
				char * tmp = new char[len];
				resp = (LOGIN_REGISTER_RESP_CLIENT *)tmp;
				resp->serialize(errorcode::client_login_error_username);
				conn->start_async_write((char *)resp, resp->length);
			}
			else if (!req->loginpass[0])
			{
				char * tmp = new char[len];
				resp = (LOGIN_REGISTER_RESP_CLIENT *)tmp;
				resp->serialize(errorcode::client_login_error_password);
				conn->start_async_write((char *)resp, resp->length);
			}
			else
			{
				otl_stream os((int)qry.size(), qry.c_str(), db);
				os << req->accounts;

				if (os.eof())
				{
					char * tmp = new char[len];
					resp = (LOGIN_REGISTER_RESP_CLIENT *)tmp;
					resp->serialize(errorcode::client_login_error_username);
					conn->start_async_write((char *)resp, resp->length);
				}
				else
				{
					os >> accountid >> encpass >> accounttype;
					os.close();

					base64_decode(encpass, strlen(encpass), decpass);
					if (strcmp(req->loginpass, decpass))
					{
						char * tmp = new char[len];
						resp = (LOGIN_REGISTER_RESP_CLIENT *)tmp;
						resp->serialize(errorcode::client_login_error_password);
						conn->start_async_write((char *)resp, resp->length);
					}
					else
					{
						//bool login_repet = false;
						//{
						//	boost::mutex::scoped_lock lock(mutex_);
						//	login_repet = user_.find(accountid) != user_.end();
						//}

						//if (login_repet)
						//{
						//	char * tmp = new char[len];
						//	resp = (LOGIN_REGISTER_RESP_CLIENT *)tmp;
						//	resp->serialize(errorcode::client_login_error_repet);
						//	conn->start_async_write((char *)resp, resp->length);
						//}
						//else
						{
							handler_ptr temp;
							int agentid = 0;
							int count = 99999;
							{
								boost::mutex::scoped_lock lock(mutex_);
								group_map::iterator git = group_.find(req->group_name);
								if (git != group_.end())
								{
									agent_map::iterator it = git->second->begin();
									while (it != git->second->end())
									{
										if (it->second->online() < count)
										{
											temp = it->second;
											count = it->second->online();
											agentid = it->first;
										}
										it++;
									}
									if (count != 99999)
									{
										++temp->online();
										//user_.insert(std::pair<int, int>(accountid, temp->id()));
									}
								}
							}

							if (count == 99999)
							{
								char * tmp = new char[len];
								resp = (LOGIN_REGISTER_RESP_CLIENT *)tmp;
								resp->serialize(errorcode::client_login_error_server_nonexistent);
								conn->start_async_write((char *)resp, resp->length);
							}
							else
							{
								otl_stream os1((int)inst.size(), inst.c_str(), db);
								os1 << accountid << conn->socket().remote_endpoint().address().
									to_v4().to_string().c_str() << (int)req->computer_id_ << "LOGIN";
								os1.close();

								char * tmp = new char[len];
								resp = (LOGIN_REGISTER_RESP_CLIENT *)tmp;

								resp->serialize(errorcode::client_login_succeed, accountid, accounttype, 
									agentid, temp->url().c_str(), temp->port(), key);

								conn->start_async_write((char *)resp, resp->length);
							}
						}
					}
				}
			}
		}
		delete [] buf;
		conn.reset();
	}
}

void request_handler::exit(char * buf)
{
	logic_check(!buf, ("buffer = 0x%08x, %s", buf, 
		errorcode::error_message(errorcode::local_error_object_null)));

	delete [] buf;
}