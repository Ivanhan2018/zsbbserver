#include "request_handler.hpp"
#include "connection.hpp"
#include "distributed_protocol.hpp"
#include "log.hpp"

bool request_handler::start(const char* mgr_addr, u_short mgr_port, std::size_t n_threads, 
							int role_id, u_short listen_port, boost::asio::io_service& ios, 
							DATA_RECEIVER receiver_func, EVENT_NOTIFY notify_func, 
							SOCKET_DISCONNECT disconn_func, LOGIC_EXCEPTION exception_func)
{
	//logic_check(!mgr_addr, ("manager address = 0x%08x, %s", mgr_addr, 
	//	errorcode::error_message(errorcode::local_error_address_0)));

	//logic_check(!mgr_port || mgr_port > 65000, ("manager port = %d, %s", mgr_port, 
	//	errorcode::error_message(errorcode::local_error_port_overflow)));

	//logic_check(!n_threads || n_threads > 200, ("n_threads = %d, %s", n_threads, 
	//	errorcode::error_message(errorcode::local_error_thread_num_overflow)));

	//logic_check(!listen_port || listen_port > 65000, ("listen_port = %d, %s", 
	//	listen_port, errorcode::error_message(errorcode::local_error_port_overflow)));

	//logic_check(role_id < SERVER_BOUND_MAP_BEGIN || role_id > SERVER_BOUND_LIGHT_END, 
	//	("%d<=role_id<%d, %s", SERVER_BOUND_MAP_BEGIN, SERVER_BOUND_LIGHT_END,
	//	errorcode::error_message(errorcode::local_error_id_overflow)));

	//logic_check(!receiver_func, ("receiver_func = 0x%08x, %s", receiver_func, 
	//	errorcode::error_message(errorcode::local_error_object_null)));

	//logic_check(!notify_func, ("notify_func = 0x%08x, %s", notify_func, 
	//	errorcode::error_message(errorcode::local_error_object_null)));

	//logic_check(!disconn_func, ("disconn_func = 0x%08x, %s", disconn_func, 
	//	errorcode::error_message(errorcode::local_error_object_null)));

	//logic_check(!exception_func, ("exception_func = 0x%08x, %s", exception_func, 
	//	errorcode::error_message(errorcode::local_error_object_null)));

	LOG_ERROR_EX_RETURN(mgr_addr, ("manager address = 0x%08x, %s", mgr_addr, 
		errorcode::error_message(errorcode::local_error_address_0)), false) ;

	LOG_ERROR_EX_RETURN(mgr_port, ("manager port = %d, %s", mgr_port, 
		errorcode::error_message(errorcode::local_error_port_overflow)), false);

	LOG_ERROR_EX_RETURN(n_threads, ("n_threads = %d, %s", n_threads, 
		errorcode::error_message(errorcode::local_error_thread_num_overflow)), false);

	LOG_ERROR_EX_RETURN(listen_port, ("listen_port = %d, %s", 
		listen_port, errorcode::error_message(errorcode::local_error_port_overflow)), false);

	LOG_ERROR_EX_RETURN(role_id >= SERVER_BOUND_MAP_BEGIN && role_id <= SERVER_BOUND_LIGHT_END, 
		("%d<=role_id<%d, %s", SERVER_BOUND_MAP_BEGIN, SERVER_BOUND_LIGHT_END,
		errorcode::error_message(errorcode::local_error_id_overflow)), false);

	LOG_ERROR_EX_RETURN(receiver_func, ("receiver_func = 0x%08x, %s", receiver_func, 
		errorcode::error_message(errorcode::local_error_object_null)), false);

	LOG_ERROR_EX_RETURN(notify_func, ("notify_func = 0x%08x, %s", notify_func, 
		errorcode::error_message(errorcode::local_error_object_null)), false);

	LOG_ERROR_EX_RETURN(disconn_func, ("disconn_func = 0x%08x, %s", disconn_func, 
		errorcode::error_message(errorcode::local_error_object_null)), false);

	LOG_ERROR_EX_RETURN(exception_func, ("exception_func = 0x%08x, %s", exception_func, 
		errorcode::error_message(errorcode::local_error_object_null)), false);

	if (is_run()) return false;

	receiver_func_ = receiver_func;
	notify_func_ = notify_func;
	disconn_func_ = disconn_func;
	exception_func_ = exception_func;

	handler_ptr mgr(new connection(ios, this));

	boost::system::error_code err_mgr;
	mgr->socket().connect(boost::asio::ip::tcp::endpoint(boost::asio::
		ip::address::from_string(mgr_addr), mgr_port), err_mgr);

	//logic_check(err_mgr, ("address(%s), port(%d), %s", 
	//	mgr_addr, mgr_port, err_mgr.message().c_str()));

	LOG_ERROR_EX_RETURN(!err_mgr, ("address(%s), port(%d), %s", 
		mgr_addr, mgr_port, err_mgr.message().c_str()), false);

	role_id_ = role_id;

	SERVER_REGISTER_MANAGER req;
	req.serialize(role_id, listen_port);
	boost::asio::write(mgr->socket(), boost::asio::buffer((char*)&req, req.length));

	char buffer[2048];
	MANAGER_REGISTER_RESP_SERVER * resp = (MANAGER_REGISTER_RESP_SERVER *)buffer;
	boost::asio::read(mgr->socket(), boost::asio::buffer(buffer, sizeof(*resp)));

	//logic_check(resp->code != errorcode::register_succeed, 
	//	("%s", errorcode::error_message(resp->code)));

	//logic_check(resp->num >= SERVER_MANAGER_ID, 
	//	("Protocol error server_num > %d", SERVER_MANAGER_ID));

	LOG_ERROR_EX_RETURN(resp->code == errorcode::register_succeed, 
		("%s", errorcode::error_message(resp->code)), false);

	LOG_ERROR_EX_RETURN(resp->num < SERVER_MANAGER_ID, 
		("Protocol error server_num > %d", SERVER_MANAGER_ID), false);

	game_id_ = resp->game_id;
	group_id_ = resp->group_id;
	log_game_address_ = resp->log_game_address;
	log_game_port_ = resp->log_game_port;

	if (resp->num)
	{
		boost::asio::read(mgr->socket(), boost::asio::buffer(
			buffer + sizeof(*resp), resp->length - sizeof(*resp)));

		for (int i = 0; i < resp->num; i++)
		{
			NODE_ADDRESS & node = (*resp)[i];
			handler_ptr conn(new connection(ios, this));

			boost::asio::ip::address_v4 addr(node.address);

			boost::system::error_code err;
			conn->socket().connect(boost::asio::ip::tcp::endpoint(addr, node.port), err);

			//logic_check(err, ("address(%s), port(%d), %s", 
			//	addr.to_string().c_str(), node.port, err.message().c_str()));

			if (err)
			{
				LOG_ERROR(("address(%s), port(%d), %s", 
					addr.to_string().c_str(), node.port, err.message().c_str()));
				continue;
			}

			SERVER_HANDSHAKE_SERVER req;
			req.serialize(role_id);
			boost::asio::write(conn->socket(), boost::asio::buffer((char*)&req, req.length));

			SERVER_HANDSHAKE_RESP_SERVER resp;
			boost::asio::read(conn->socket(), boost::asio::buffer((char *)&resp, sizeof(resp)));

			conn->id() = node.id;
			cons_.insert(std::pair<int, handler_ptr>(node.id, conn));
			conn->start_async_read();
		}
	}

	cons_.insert(std::pair<int, handler_ptr>(SERVER_MANAGER_ID, mgr));
	mgr->id() = SERVER_MANAGER_ID;
	mgr->start_async_read();

	return task_base_ex<connection, char *>::start(n_threads);
}

bool request_handler::stop()
{
	if (!task_base_ex<connection, char *>::stop())
		return false;

	role_id_ = 0;
	cons_.clear();
	return true;
}

bool request_handler::start_async_write(int id, char * buf, int len)
{
	if (!is_run()) return false;

	//logic_check(!buf, ("buf = 0x%08x, %s", buf, 
	//	errorcode::error_message(errorcode::local_error_data_0)));

	//logic_check(!len, ("len = 0x%08x, %s", len, 
	//	errorcode::error_message(errorcode::local_error_buffer_lenght)));

	//logic_check(id < SERVER_BOUND_AGENT_BEGIN || id > SERVER_BOUND_AGENT_END, 
	//	("%d<=agent id<%d, %s", SERVER_BOUND_AGENT_BEGIN, SERVER_BOUND_AGENT_END,
	//	errorcode::error_message(errorcode::local_error_id_overflow)));

	LOG_ERROR_EX_RETURN(buf, ("buf = 0x%08x, %s", buf, 
		errorcode::error_message(errorcode::local_error_data_0)), false);

	LOG_ERROR_EX_RETURN(len, ("len = 0x%08x, %s", len, 
		errorcode::error_message(errorcode::local_error_buffer_lenght)), false);

	LOG_ERROR_EX_RETURN(id >= SERVER_BOUND_AGENT_BEGIN && id <= SERVER_BOUND_AGENT_END, 
		("%d<=agent id<%d, %s", SERVER_BOUND_AGENT_BEGIN, SERVER_BOUND_AGENT_END,
		errorcode::error_message(errorcode::local_error_id_overflow)), false);

	DISTRIBUTED_HEADER * h = (DISTRIBUTED_HEADER *)buf;

	//logic_check(h->from != role_id_, ("protocol from_id(%d), local(%d), %s", h->from, 
	//	role_id_, errorcode::error_message(errorcode::server_request_error_from_id)));

	//logic_check(!h->command, ("protocol command(%d), %s", h->command, 
	//	errorcode::error_message(errorcode::server_request_error_command_null)));

	//logic_check(h->to < 100000, ("protocol to_id(%d), %s", h->to, 
	//	errorcode::error_message(errorcode::server_request_error_to_id_less_100000)));

	//logic_check(h->length != len, ("protocol length(%d) != len(%d), %s", h->length, len,
	//	errorcode::error_message(errorcode::local_error_buffer_lenght)));

	LOG_ERROR_EX_RETURN(h->from == role_id_, ("protocol from_id(%d), local(%d), %s", h->from, 
		role_id_, errorcode::error_message(errorcode::server_request_error_from_id)), false);

	LOG_ERROR_EX_RETURN(h->command, ("protocol command(%d), %s", h->command, 
		errorcode::error_message(errorcode::server_request_error_command_null)), false);

	LOG_ERROR_EX_RETURN(h->to >= 100000, ("protocol to_id(%d), %s", h->to, 
		errorcode::error_message(errorcode::server_request_error_to_id_less_100000)), false);

	LOG_ERROR_EX_RETURN(h->length == len, ("protocol length(%d) != len(%d), %s", h->length, len,
		errorcode::error_message(errorcode::local_error_buffer_lenght)), false);

	handler_ptr agent;
	{
		boost::mutex::scoped_lock lock(mutex1_);
		stdext::hash_map<int, handler_ptr>::iterator iter = cons_.find(id);
		if (iter == cons_.end()) return false;
		agent = iter->second;
	}

	char * tmp = new char[len];
	memcpy(tmp, buf, len);
	agent->start_async_write(tmp, len);

	return true;
}

bool request_handler::event_notify(int id, char * buf, int len)
{
	if (!is_run()) return false;

	//logic_check(!buf, ("buf = 0x%08x, %s", buf, 
	//	errorcode::error_message(errorcode::local_error_data_0)));

	//logic_check(!len, ("len = 0x%08x, %s", len, 
	//	errorcode::error_message(errorcode::local_error_buffer_lenght)));

	//logic_check(((DISTRIBUTED_HEADER *)buf)->length != len, 
	//	("protocol length(%d) != len(%d), %s", ((DISTRIBUTED_HEADER *)buf)->length, len,
	//	errorcode::error_message(errorcode::local_error_buffer_lenght)));

	//logic_check(id < SERVER_BOUND_MAP_BEGIN, 
	//	("server id %d < %d", id, SERVER_BOUND_MAP_BEGIN));

	//logic_check(id > SERVER_BOUND_LIGHT_END, 
	//	("server id %d > %d", id, SERVER_BOUND_LIGHT_END));

	LOG_ERROR_EX_RETURN(buf, ("buf = 0x%08x, %s", buf, 
		errorcode::error_message(errorcode::local_error_data_0)), false);

	LOG_ERROR_EX_RETURN(len, ("len = 0x%08x, %s", len, 
		errorcode::error_message(errorcode::local_error_buffer_lenght)), false);

	LOG_ERROR_EX_RETURN(((DISTRIBUTED_HEADER *)buf)->length == len, 
		("protocol length(%d) != len(%d), %s", ((DISTRIBUTED_HEADER *)buf)->length, len,
		errorcode::error_message(errorcode::local_error_buffer_lenght)), false);

	LOG_ERROR_EX_RETURN(id >= SERVER_BOUND_MAP_BEGIN, 
		("server id %d < %d", id, SERVER_BOUND_MAP_BEGIN), false);

	LOG_ERROR_EX_RETURN(id <= SERVER_BOUND_LIGHT_END, 
		("server id %d > %d", id, SERVER_BOUND_LIGHT_END), false);

	handler_ptr mgr;
	{
		boost::mutex::scoped_lock lock(mutex1_);
		stdext::hash_map<int, handler_ptr>::iterator iter = cons_.find(SERVER_MANAGER_ID);
		if (iter == cons_.end()) return false;
		mgr = iter->second;
	}

	char * tmp = new char[sizeof(SERVER_NOTIFY_MANAGER_NOTIFY_SERVER) + len];
	SERVER_NOTIFY_MANAGER_NOTIFY_SERVER * h = (SERVER_NOTIFY_MANAGER_NOTIFY_SERVER *)tmp;

	h->serialize(role_id_, id, (int)len);
	memcpy(tmp + sizeof(*h), buf, len);
	mgr->start_async_write(tmp, h->length);
	return true;
}

bool request_handler::broadcast(int id, char * buf, int len)
{
	if (!is_run()) return false;

	//logic_check(!buf, ("buf = 0x%08x, %s", buf, 
	//	errorcode::error_message(errorcode::local_error_data_0)));

	//logic_check(!len, ("len = 0x%08x, %s", len, 
	//	errorcode::error_message(errorcode::local_error_buffer_lenght)));

	//logic_check(id < SERVER_BOUND_MAP_BEGIN, 
	//	("server id %d < %d", id, SERVER_BOUND_MAP_BEGIN));

	//logic_check(id > SERVER_BOUND_LIGHT_END, 
	//	("server id %d > %d", id, SERVER_BOUND_LIGHT_END));

	//logic_check(((DISTRIBUTED_HEADER *)buf)->length != len, 
	//	("protocol length(%d) != len(%d), %s", ((DISTRIBUTED_HEADER *)buf)->length, len,
	//	errorcode::error_message(errorcode::local_error_buffer_lenght)));

	LOG_ERROR_EX_RETURN(buf, ("buf = 0x%08x, %s", buf, 
		errorcode::error_message(errorcode::local_error_data_0)), false);

	LOG_ERROR_EX_RETURN(len, ("len = 0x%08x, %s", len, 
		errorcode::error_message(errorcode::local_error_buffer_lenght)), false);

	LOG_ERROR_EX_RETURN(id >= SERVER_BOUND_MAP_BEGIN, 
		("server id %d < %d", id, SERVER_BOUND_MAP_BEGIN), false);

	LOG_ERROR_EX_RETURN(id <= SERVER_BOUND_LIGHT_END, 
		("server id %d > %d", id, SERVER_BOUND_LIGHT_END), false);

	LOG_ERROR_EX_RETURN(((DISTRIBUTED_HEADER *)buf)->length == len, 
		("protocol length(%d) != len(%d), %s", ((DISTRIBUTED_HEADER *)buf)->length, len,
		errorcode::error_message(errorcode::local_error_buffer_lenght)), false);

	std::size_t count = 0;
	handler_ptr agent[128];
	{
		boost::mutex::scoped_lock lock(mutex1_);
		stdext::hash_map<int, handler_ptr>::iterator iter;
		for (iter = cons_.begin(); iter != cons_.end(); iter++)
		{
			if (iter->second->id() == SERVER_MANAGER_ID)
				continue;

			agent[count] = iter->second;
			++count;
		}
	}

	for (std::size_t i = 0; i < count; i++)
	{
		char * tmp = new char[sizeof(SERVER_BROADCAST_CLIENT) + len];
		SERVER_BROADCAST_CLIENT * h = (SERVER_BROADCAST_CLIENT *)tmp;

		h->serialize(id, (int)len);
		memcpy(tmp + sizeof(*h), buf, len);
		agent[i]->start_async_write(tmp, h->length);
	}

	return true;
}

bool request_handler::handle_message(connection * conn, char * buffer, int length)
{
	if (!is_run()) return false;

	DISTRIBUTED_HEADER * h = (DISTRIBUTED_HEADER *)buffer;

	if (h->command == CMD_SERVER_HANDSHAKE_SERVER)
	{
		SERVER_HANDSHAKE_SERVER * req = (SERVER_HANDSHAKE_SERVER *)h;

		if ((req->id < SERVER_BOUND_AGENT_BEGIN) || (req->id > SERVER_BOUND_AGENT_END))
		{
			SERVER_HANDSHAKE_RESP_SERVER * resp = (SERVER_HANDSHAKE_RESP_SERVER *)h;
			resp->serialize(errorcode::handshake_error_to_id_overflow);
			conn->start_async_write(buffer, resp->length);
		}
		else
		{
			conn->id() = req->id;
			SERVER_HANDSHAKE_RESP_SERVER * resp = (SERVER_HANDSHAKE_RESP_SERVER *)h;
			resp->serialize(errorcode::handshake_succeed);
			conn->start_async_write(buffer, resp->length);

			boost::mutex::scoped_lock lock(mutex1_);
			cons_.erase(conn->id());
			cons_.insert(std::pair<int, handler_ptr>(conn->id(), conn->shared_from_this()));
		}
		return true;
	}
	else
	{
		logic_check(!conn->id(), ("conn id = %d, %s", conn->id(), 
			errorcode::error_message(errorcode::local_error_id_overflow)));

		if (push(conn->shared_from_this(), buffer))	return true;
	}

	return false;
}

void request_handler::handle_disconnect(connection * conn, const boost::system::error_code& err)
{
	logic_check(!conn, ("conn = 0x%08x, %s", conn, 
		errorcode::error_message(errorcode::local_error_object_null)));

	if (!is_run() || !conn->id()) return ;
	{
		boost::mutex::scoped_lock lock(mutex1_);
		cons_.erase(conn->id());
	}

	push(conn->shared_from_this(), (char *)event_disconnect);
}

void request_handler::run()
{
	handler_ptr conn;
	char * buf;
	DISTRIBUTED_HEADER * h;

	while (pop(conn, buf))
	{
		if (buf == (char *)event_disconnect)
		{
			disconn_func_(true, conn->id(), role_id_, 0);
		}
		else
		{
			h = (DISTRIBUTED_HEADER *)buf;

			if (h->command == CMD_SERVER_NOTIFY_MANAGER_NOTIFY_SERVER)
			{
				SERVER_NOTIFY_MANAGER_NOTIFY_SERVER * req = (SERVER_NOTIFY_MANAGER_NOTIFY_SERVER *)h;
				notify_func_(req->from, ((char *)req) + sizeof(*req), req->length - sizeof(*req));
			}
			else if (h->command == CMD_SERVER_NOTIFY_RESP_SERVER)
			{
				SERVER_NOTIFY_RESP_SERVER * req = (SERVER_NOTIFY_RESP_SERVER *)h;
				exception_func_(req->code, 0, req->from, req->to);
			}
			else if (h->command == CMD_AGENT_CLIENT_LEAVE_SERVER)
			{
				AGENT_CLIENT_LEAVE_SERVER * req = (AGENT_CLIENT_LEAVE_SERVER *)h;
				disconn_func_(false, req->agent, req->from, req->to);
			}
			else if (h->command == CMD_AGENT_SERVER_RESPONSE)
			{
				AGENT_SERVER_RESPONSE * req = (AGENT_SERVER_RESPONSE *)h;
				exception_func_(req->code, req->agent, req->from, req->to);
			}
			else
			{
				logic_check(!conn->id(), ("agent id = %d, %s", conn->id(), 
					errorcode::error_message(errorcode::local_error_id_overflow)));

				receiver_func_(conn->id(), h->from, (char *)h, h->length);
			}

			delete [] buf;
		}
		conn.reset();
	}
}

void request_handler::exit(char * buf)
{
	logic_check(!buf, ("buffer = 0x%08x, %s", buf, 
		errorcode::error_message(errorcode::local_error_object_null)));

	delete [] buf;
}

