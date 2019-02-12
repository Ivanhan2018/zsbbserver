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

#include "login_handler.hpp"
#include "distributed_protocol.hpp"

#define OTL_STL
#define OTL_ODBC

#if defined (WIN32)
#define OTL_BIGINT __int64
#else
#define OTL_BIGINT long long
#endif

#include "otlv4.h"

using namespace boost::asio::detail;
using namespace boost::asio::detail::socket_ops;

login_handler::login_handler() :
listen_(invalid_socket),
agent_id_(0),
agent_port_(0)
{
}

bool login_handler::start(int agent_id, const char * agent_address, u_short agent_port, 
						  std::size_t n_threads, u_short port, short version,
						  const char * db_uid, const char * db_pwd, const char * db_dsn)
{
	logic_check(agent_id < SERVER_BOUND_AGENT_BEGIN || agent_id > SERVER_BOUND_AGENT_END, 
		("%d<=agent id<%d, %s", SERVER_BOUND_AGENT_BEGIN, SERVER_BOUND_AGENT_END,
		errorcode::error_message(errorcode::local_error_id_overflow)));

	logic_check(!agent_address, ("agent address = 0x%08x, %s", agent_address, 
		errorcode::error_message(errorcode::local_error_address_0)));

	logic_check(!agent_port || agent_port > 65000, ("agent port = %d, %s", agent_port, 
		errorcode::error_message(errorcode::local_error_port_overflow)));

	logic_check(!port || port > 65000, ("manager port = %d, %s", port, 
		errorcode::error_message(errorcode::local_error_port_overflow)));

	logic_check(!n_threads || n_threads > 200, ("n_threads = %d, %s", n_threads, 
		errorcode::error_message(errorcode::local_error_thread_num_overflow)));

	if (is_run()) return false;

	agent_id_ = agent_id;
	agent_address_ = agent_address;
	agent_port_ = agent_port;
	version_ = version;

	db_con_str_ = "UID=";
	db_con_str_ += db_uid;

	db_con_str_ += ";PWD=";
	db_con_str_ += db_pwd;

	db_con_str_ += ";DSN=";
	db_con_str_ += db_dsn;

	boost::system::error_code err;
	sockaddr_in s_in;
	s_in.sin_family = AF_INET;
	s_in.sin_port = ::htons(port);
	s_in.sin_addr.s_addr = INADDR_ANY;

	listen_ = (int)socket(AF_INET, SOCK_STREAM, IPPROTO_TCP, err);
	logic_check(err, ("create socket error %s", err.message().c_str()));

	bind(listen_, (sockaddr *)&s_in, sizeof(s_in), err);
	logic_check(err, ("bind socket error %s", err.message().c_str()));

	listen(listen_, 256, err);
	logic_check(err, ("listen socket error %s", err.message().c_str()));

	if (task_base<int>::start(n_threads))
		return true;

	close(listen_, err);
	return false;
}

void login_handler::run_event_loop()
{
	int fd, timeo = 2000, flag = 1;
	sockaddr_in s_in;
	std::size_t s_len = sizeof(s_in);
	boost::system::error_code err;

	while (true)
	{
		fd = (int)accept(listen_, (sockaddr *)&s_in, &s_len, err);
		if ((fd == invalid_socket) || err) break;

		setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, 
			(char *)&timeo, sizeof(timeo), err);

		logic_check(err, ("setsockopt SO_RCVTIMEO "
			"error %s", err.message().c_str()));

		setsockopt(fd, IPPROTO_TCP, TCP_NODELAY,
			(char*)&flag, sizeof(flag), err);

		logic_check(err, ("setsockopt TCP_NODELAY "
			"error %s", err.message().c_str()));

		if (!push(fd)) close(fd, err);
	}
}

void login_handler::stop_event_loop()
{
	boost::system::error_code err;
	shutdown(listen_, shutdown_both, err);
	close(listen_, err);
	listen_ = invalid_socket;
}

inline int login_handler::recv_n(int fd, char * buf, const int & len)
{
	for (int r = 0, t = 0 ; ; )
	{
		if ((t = ::recv(fd, buf + r, len - r, 0)) < 1)
			return -1;
		r += t;
		if (len == r) return r;
	}

	return 0;
}

inline int login_handler::send_n(int fd, char * buf, const int & len)
{
	for (int r = 0, t = 0 ; ; )
	{
		if ((t = ::send(fd, buf + r, len - r, 0)) < 1)
			return -1;
		r += t;
		if (len == r) return r;
	}

	return 0;
}

void login_handler::run()
{
	int fd;
	const char * url = 0;
	unsigned char key[] = "123456789ABCDEF";
	std::string qry = "SELECT accountid, password, accounttype "
		"FROM GameUser WHERE accountname=:f<char[16]>";

	std::string inst = "INSERT INTO loginlog(playerid, "
		"playerip, playercpuid, type) VALUES (:a1<int>,"
		":a2<char[50]>,:a3<char[50]>,:a4<char[50]>)";

	int accountid, accounttype;
	char password[SIZE_16];
	boost::system::error_code err;

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

	char buff[1024];

	CLIENT_REGISTER_LOGIN * req = (CLIENT_REGISTER_LOGIN *)buff;
	CLIENT_REGISTER_LOGOUT * req11 = (CLIENT_REGISTER_LOGOUT *)buff;

	LOGIN_REGISTER_RESP_CLIENT resp_err;
	resp_err.serialize(0);

	LOGIN_REGISTER_RESP_CLIENT resp_scc;
	resp_scc.serialize(errorcode::client_login_succeed, 
		0, 0, agent_id_, agent_address_.c_str(), agent_port_, key);

	while (pop(fd))
	{
		//if ((recv_n(fd, (char *)&req, req_len) < 1) ||
		//	(req.length != req_len))
		//{
		//	close(fd, err);
		//	continue;
		//}

		if (::recv(fd, buff, 1024, 0) < 1)
		{
			close(fd, err);
			continue;
		}

		DISTRIBUTED_HEADER * header = (DISTRIBUTED_HEADER *)buff;
		if (header->command == CMD_CLIENT_REGISTER_LOGIN)
		{
			if (req->version != version_)
			{
				resp_err.code = errorcode::client_login_error_version;
				send_n(fd, (char *)&resp_err, resp_err.length);
				close(fd, err);
				continue;
			}

			if (!req->accounts[0])
			{
				resp_err.code = errorcode::client_login_error_username;
				send_n(fd, (char *)&resp_err, resp_err.length);
				close(fd, err);
				continue;
			}

			if (!req->loginpass[0])
			{
				resp_err.code = errorcode::client_login_error_password;
				send_n(fd, (char *)&resp_err, resp_err.length);
				close(fd, err);
				continue;
			}

			otl_stream os((int)qry.size(), qry.c_str(), db);
			os << req->accounts;

			if (os.eof())
			{
				os.close();
				resp_err.code = errorcode::client_login_error_username;
				send_n(fd, (char *)&resp_err, resp_err.length);
				close(fd, err);
				continue;
			}

			os >> accountid >> password >> accounttype;
			os.close();

			if (strcmp(req->loginpass, password))
			{
				resp_err.code = errorcode::client_login_error_password;
				send_n(fd, (char *)&resp_err, resp_err.length);
				close(fd, err);
				continue;
			}

			//bool flag;
			//{
			//	read_write_mutex::scoped_read_lock lock(rw_mutex_);
			//	flag = login_mgr_.find(accountid) != login_mgr_.end();
			//}
			//if (flag)
			//{
			//	resp_err.code = errorcode::client_login_error_repet;
			//	send_n(fd, (char *)&resp_err, resp_err.length);
			//	close(fd, err);
			//	continue;
			//}
			//else
			//{
			//	read_write_mutex::scoped_write_lock lock(rw_mutex_);
			//	login_mgr_.insert(std::pair<int, char>(accountid, 0));
			//}

			sockaddr_in remote_addr;
			int addr_len = sizeof(remote_addr);
			getpeername(fd, (sockaddr*)&remote_addr,&addr_len);
			otl_stream os1((int)inst.size(), inst.c_str(), db);
			
			os1 << accountid << inet_ntoa(remote_addr.sin_addr) << req->mac_addr << "LOGIN";
			os1.close();

			resp_scc.playerid = accountid;
			resp_scc.accounttype = accounttype;
			send_n(fd, (char *)&resp_scc, resp_scc.length);
			close(fd, err);
		}
		else if (header->command == CMD_CLIENT_REGISTER_LOGOUT)
		{
			//read_write_mutex::scoped_write_lock lock(rw_mutex_);
			//login_mgr_.erase(req11->playerid_);
			sockaddr_in remote_addr;
			int addr_len = sizeof(remote_addr);
			getpeername(fd, (sockaddr*)&remote_addr,&addr_len);
			otl_stream os1((int)inst.size(), inst.c_str(), db);
			os1 << req11->playerid_ << inet_ntoa(remote_addr.sin_addr) << req11->mac_addr << "LOGOUT";
			os1.close();
			close(fd, err);
			continue;
		}
		else
		{
			close(fd, err);
		}
	}

	db.logoff();
}

void login_handler::exit(int fd)
{
	boost::system::error_code err;
	shutdown(fd, shutdown_both, err);
	close(fd, err);
}

