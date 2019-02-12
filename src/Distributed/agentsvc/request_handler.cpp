#include "request_handler.hpp"
#include "connection.hpp"
#include "distributed_protocol.hpp"

request_handler::request_handler() :
role_id_(0)
{
}

bool request_handler::start(const char* mgr_addr, u_short mgr_port, const char * group_name, int role_id, 
							u_short listen_port, const char * login_addr, 
							u_short login_port, const char * url, boost::asio::io_service& ios)
{
	logic_check(!mgr_addr, ("manager address = 0x%08x, %s", mgr_addr, 
		errorcode::error_message(errorcode::local_error_address_0)));

	logic_check(!mgr_port/* || mgr_port > 65000*/, ("manager port = %d, %s", mgr_port, 
		errorcode::error_message(errorcode::local_error_port_overflow)));

	logic_check(!listen_port/* || listen_port > 65000*/, ("listen_port = %d, %s", 
		listen_port, errorcode::error_message(errorcode::local_error_port_overflow)));

	logic_check(!group_name, ("group name = 0x%08x, %s", group_name, 
		errorcode::error_message(errorcode::local_error_address_0)));

	logic_check(role_id < SERVER_BOUND_AGENT_BEGIN || role_id > SERVER_BOUND_AGENT_END, 
		("%d<=role_id<%d, %s", SERVER_BOUND_AGENT_BEGIN, SERVER_BOUND_AGENT_END,
		errorcode::error_message(errorcode::local_error_id_overflow)));

	if (role_id_) return false;

	handler_ptr mgr(new connection(ios, this));

	boost::system::error_code err_mgr;
	mgr->socket().connect(boost::asio::ip::tcp::endpoint(boost::asio::
		ip::address::from_string(mgr_addr), mgr_port), err_mgr);

	logic_check(err_mgr, ("address(%s), port(%d), %s", 
		mgr_addr, mgr_port, err_mgr.message().c_str()));

	group_name_ = group_name;
	role_id_ = role_id;

	SERVER_REGISTER_MANAGER req;
	req.serialize(role_id, listen_port);
	boost::asio::write(mgr->socket(), boost::asio::buffer((char*)&req, req.length));

	char buffer[2048];
	MANAGER_REGISTER_RESP_SERVER * resp = (MANAGER_REGISTER_RESP_SERVER *)buffer;
	boost::asio::read(mgr->socket(), boost::asio::buffer(buffer, sizeof(*resp)));

	logic_check(resp->code != errorcode::register_succeed, 
		("%s", errorcode::error_message(resp->code)));

	logic_check(resp->num >= SERVER_MANAGER_ID, 
		("Protocol error server_num > %d", SERVER_MANAGER_ID));

	if (resp->num)
	{
		boost::asio::read(mgr->socket(), boost::asio::buffer(
			buffer + sizeof(*resp), resp->length - sizeof(*resp)));

		for (int i = 0; i < resp->num; i++)
		{
			NODE_ADDRESS & node = (*resp)[i];
			handler_ptr conn(new connection(ios, this));

			logic_check(node.id < SERVER_BOUND_MAP_BEGIN || node.id > SERVER_BOUND_LIGHT_END, 
				("%d<=server id<%d, %s", SERVER_BOUND_MAP_BEGIN, SERVER_BOUND_LIGHT_END,
				errorcode::error_message(errorcode::local_error_id_overflow)));

			logic_check(!node.port /*|| node.port > 65000*/, ("server port = %d, %s", 
				node.port, errorcode::error_message(errorcode::local_error_port_overflow)));

			boost::asio::ip::address_v4 addr(node.address);

			boost::system::error_code err;
			conn->socket().connect(boost::asio::ip::tcp::endpoint(addr, node.port), err);

			logic_check(err, ("address(%s), port(%d), %s", 
				addr.to_string().c_str(), node.port, err.message().c_str()));

			SERVER_HANDSHAKE_SERVER req;
			req.serialize(role_id);
			boost::asio::write(conn->socket(), boost::asio::buffer((char*)&req, req.length));

			SERVER_HANDSHAKE_RESP_SERVER resp;
			boost::asio::read(conn->socket(), boost::asio::buffer((char *)&resp, sizeof(resp)));

			logic_check(resp.code != errorcode::register_succeed,
				("%s", errorcode::error_message(resp.code)));

			city_handler_ptr city(new city_handler);
			city->start(role_id_, node.id, conn.get());
			citys_.insert(std::pair<int, city_handler_ptr>(node.id, city));
			conn->start_async_read();
		}
	}

	city_handler_ptr city(new city_handler);
	city->start(role_id_, SERVER_MANAGER_ID, mgr.get());
	citys_.insert(std::pair<int, city_handler_ptr>(SERVER_MANAGER_ID, city));
	mgr->start_async_read();

	login_svc_.reset(new connection(ios, this));

	boost::asio::ip::tcp::endpoint endpoint(boost::asio::
		ip::address::from_string(login_addr), login_port);

	boost::system::error_code err;
	login_svc_->socket().connect(endpoint, err);

	logic_check(err, ("address(%s), port(%d), %s", 
		login_addr, login_port, err.message().c_str()));

	AGENT_REPORT_LOGIN hello;
	hello.serialize(this->role_id_, this->group_name_.c_str(), url, listen_port);
	boost::asio::write(login_svc_->socket(), boost::asio::buffer((char*)&hello, hello.length));

	login_svc_->id() = LOGIN_FLAG;
	login_svc_->start_async_read();

	timer_ = new boost::asio::deadline_timer(ios, boost::posix_time::seconds(300));
	timer_->async_wait(boost::bind(&request_handler::handle_timeout, this, boost::asio::placeholders::error));

	return true;
}

bool request_handler::stop()
{
	if (!role_id_) return false;

	login_svc_.reset();

	for (city_map::iterator it = citys_.begin(); 
		it != citys_.end(); it++)
		it->second->stop();

	citys_.clear();

	timer_->cancel();
	delete timer_;
	role_id_ = 0;

	clt_map_.clear();

	return true;
}

bool request_handler::handle_message(connection * conn, char * buffer, std::size_t length)
{
	if (!role_id_) return false;

	DISTRIBUTED_HEADER * h = (DISTRIBUTED_HEADER *)buffer;

	if (h->command == CMD_SERVER_HANDSHAKE_SERVER)
	{
		SERVER_HANDSHAKE_SERVER * req = (SERVER_HANDSHAKE_SERVER *)h;

		if ((req->id < SERVER_BOUND_MAP_BEGIN) || (req->id > SERVER_BOUND_LIGHT_END))
		{
			SERVER_HANDSHAKE_RESP_SERVER * resp = (SERVER_HANDSHAKE_RESP_SERVER *)h;
			resp->serialize(errorcode::handshake_error_to_id_overflow);
			conn->start_async_write(buffer, resp->length);
		}
		else
		{
			city_handler_ptr city(new city_handler), temp;
			city->start(role_id_, req->id, conn);
			{
				city_map::iterator it;
				boost::mutex::scoped_lock lock(mutex1_);
				it = citys_.find(req->id);
				if (it != citys_.end())
				{
					temp = it->second;
					citys_.erase(it);
				}

				citys_.insert(std::pair<int, city_handler_ptr>(req->id, city));
			}
			if (bool(temp)) temp->stop();

			SERVER_HANDSHAKE_RESP_SERVER * resp = (SERVER_HANDSHAKE_RESP_SERVER *)h;
			resp->serialize(errorcode::handshake_succeed);
			conn->start_async_write(buffer, resp->length);
		}
	}
	else
	{
		city_map::iterator it;
		if (conn->id() && conn->id() < SERVER_MANAGER_ID)
		{
			int from_id;
			if (h->command == CMD_SERVER_BROADCAST_CLIENT)
				from_id = h->from;
			else
				from_id = conn->id();

			boost::mutex::scoped_lock lock(mutex1_);
			it = citys_.find(from_id);
			if (it != citys_.end())	
			{
				if (it->second->push(conn->shared_from_this(), buffer))
					return true;
			}

			return false;
		}
		else 
		{
			if (conn->id() >= 100000)
			{
				boost::mutex::scoped_lock lock(mutex_);
				client_map::iterator it = clt_map_.find(conn->id());
				if (it == clt_map_.end()) clt_map_.insert(std::pair<int, int>(conn->id(), 0));
			}

			bool flag = false;
			{
				boost::mutex::scoped_lock lock(mutex1_);
				it = citys_.find(h->to);
				if (flag = (it != citys_.end()))
				{
					if (!it->second->push(conn->shared_from_this(), buffer))
					{
						printf("消息放入模块：%d，失败，将与客户端断开\n", h->to);
						return false;
					}
				}
			}

			if (!flag)
			{
				AGENT_CLIENT_RESPONSE * resp = (AGENT_CLIENT_RESPONSE *)h;
				resp->serialize(errorcode::client_request_error_to_id_nonexistent, role_id_, h->from, h->to);

				__log_handler->add_bandwidth(resp->length);

				conn->start_async_write((char *)resp, resp->length);
			}
		}
	}

	return true;
}

void request_handler::handle_connect(connection * conn)
{
}

void request_handler::handle_disconnect(connection * conn, const boost::system::error_code& err)
{
	if (!role_id_ || !conn->id()) return ;

	if (conn->id() && conn->id() <= SERVER_MANAGER_ID)
	{
		boost::mutex::scoped_lock lock(mutex1_);
		city_map::iterator it = citys_.find(conn->id());
		if (it != citys_.end())
		{
			it->second->stop();
			citys_.erase(it);
		}
	}
	else if (conn->id() == LOGIN_FLAG)
	{
		std::cout << "网络故障，与登录服务器(" 
			<< conn->socket().remote_endpoint().address().to_v4().to_string().c_str() 
			<< ":" << conn->socket().remote_endpoint().port() 
			<< ")断开，错误代码(" << err.value() << "),原因(" 
			<< err.message().c_str() << "), 请重启agentsvc\n";
		exit(0);
	}
	else
	{
		if (conn->id() >= 100000)
		{
			boost::mutex::scoped_lock lock(mutex_);
			clt_map_.erase(conn->id());
		}

		char * buf = new char[sizeof(AGENT_CLIENT_LEAVE_SERVER)];
        AGENT_CLIENT_LEAVE_SERVER * leave = (AGENT_CLIENT_LEAVE_SERVER*)buf;
		leave->serialize(this->role_id_, 0, conn->id());
		login_svc_->start_async_write(buf, leave->length);

		boost::mutex::scoped_lock lock(mutex1_);
		for (city_map::iterator it = citys_.begin(); it != citys_.end(); it++)
			it->second->push(conn->shared_from_this(), (char *)city_handler::event_disconnect);
	}
}

void request_handler::handle_timeout(const boost::system::error_code& err)
{
	if (err) return ;

	__log_handler->online(group_name_.c_str(), clt_map_.size());

	timer_->expires_at(timer_->expires_at() + boost::posix_time::seconds(300));
	timer_->async_wait(boost::bind(&request_handler::handle_timeout, this, boost::asio::placeholders::error));
}
