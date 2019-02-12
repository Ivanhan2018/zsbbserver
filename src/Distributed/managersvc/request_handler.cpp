#include "request_handler.hpp"
#include "distributed_protocol.hpp"

request_handler::request_handler(u_short game_id, u_short group_id, std::string & log_game_address, u_short log_game_port) :
game_id_(game_id), group_id_(group_id)
{
	boost::asio::ip::tcp::endpoint tmp(boost::asio::ip::address::from_string(log_game_address.c_str()), log_game_port);
	log_game_address_ = tmp.address().to_v4().to_ulong();
	log_game_port_ = log_game_port;

}

void request_handler::clear_all()
{
	cons_.clear();
}

bool request_handler::handle_message(connection * conn, char * buffer, int length)
{
	DISTRIBUTED_HEADER * h = (DISTRIBUTED_HEADER *)buffer;

	if (h->command == CMD_SERVER_REGISTER_MANAGER)
	{
		SERVER_REGISTER_MANAGER * req = (SERVER_REGISTER_MANAGER *)h;

		if ((req->id < SERVER_BOUND_MAP_BEGIN) || (req->id > SERVER_BOUND_AGENT_END))
		{
			MANAGER_REGISTER_RESP_SERVER * resp = (MANAGER_REGISTER_RESP_SERVER *)h;
			resp->serialize(errorcode::register_error_overflow, 0, 0, 0, 0, 0);
			conn->start_async_write(buffer, resp->length);
			return true;
		}
		else if (!req->port /*|| req->port > 65000*/)
		{
			MANAGER_REGISTER_RESP_SERVER * resp = (MANAGER_REGISTER_RESP_SERVER *)h;
			resp->serialize(errorcode::register_error_port_nonlicet, 0, 0, 0, 0, 0);
			conn->start_async_write(buffer, resp->length);
			return true;
		}
		else if (cons_.find(req->id) != cons_.end())
		{
			MANAGER_REGISTER_RESP_SERVER * resp = (MANAGER_REGISTER_RESP_SERVER *)h;
			resp->serialize(errorcode::register_error_existent, 0, 0, 0, 0, 0);
			conn->start_async_write(buffer, resp->length);
			return true;
		}
		else
		{
			conn->id() = req->id;
			conn->port() = req->port;
			conn->address() = conn->socket().remote_endpoint().address().to_v4().to_ulong();

			int begin, end;
			if ((req->id >= SERVER_BOUND_MAP_BEGIN) && (req->id <= SERVER_BOUND_LIGHT_END))
			{
				begin = SERVER_BOUND_AGENT_BEGIN;
				end = SERVER_BOUND_AGENT_END;
			}
			else
			{
				begin = SERVER_BOUND_MAP_BEGIN;
				end = SERVER_BOUND_LIGHT_END;
			}

			char * ptr = new char[10240];
			MANAGER_REGISTER_RESP_SERVER * resp = (MANAGER_REGISTER_RESP_SERVER *)ptr;

			int count = 0;
			for (stdext::hash_map<int, handler_ptr>::iterator 
				iter = cons_.begin(); iter != cons_.end(); iter++)
			{
				if ((iter->second->id() < begin) || (iter->second->id() > end))
					continue;

				(*resp)[count].id = iter->second->id();
				(*resp)[count].address = iter->second->address();
				(*resp)[count].port = iter->second->port();
				++count;
			}

			cons_[conn->id()] = conn->shared_from_this();
			resp->serialize(errorcode::register_succeed, count, game_id_, group_id_, log_game_address_, log_game_port_);
			delete [] buffer;

			conn->start_async_write(ptr, resp->length);
			return true;
		}
	}
	else if (h->command == CMD_SERVER_NOTIFY_MANAGER_NOTIFY_SERVER)
	{
		SERVER_NOTIFY_MANAGER_NOTIFY_SERVER * req = (SERVER_NOTIFY_MANAGER_NOTIFY_SERVER *)h;
		int to = req->to;
		int from = req->from;

		if ((to < SERVER_BOUND_MAP_BEGIN) || (to > SERVER_BOUND_LIGHT_END))
		{
			SERVER_NOTIFY_RESP_SERVER * resp = (SERVER_NOTIFY_RESP_SERVER *)h;
			resp->serialize(errorcode::notify_error_to_id_overflow, from, to);
			conn->start_async_write(buffer, resp->length);
		}
		else if ((from < SERVER_BOUND_MAP_BEGIN) || (from > SERVER_BOUND_AGENT_END))
		{
			SERVER_NOTIFY_RESP_SERVER * resp = (SERVER_NOTIFY_RESP_SERVER *)h;
			resp->serialize(errorcode::notify_error_from_id_overflow, from, to);
			conn->start_async_write(buffer, resp->length);
		}
		else if (from == to)
		{
			SERVER_NOTIFY_RESP_SERVER * resp = (SERVER_NOTIFY_RESP_SERVER *)h;
			resp->serialize(errorcode::notify_error_to_id_from_id_equal, from, to);
			conn->start_async_write(buffer, resp->length);
		}
		else if (req->length <= sizeof(SERVER_NOTIFY_MANAGER_NOTIFY_SERVER))
		{
			SERVER_NOTIFY_RESP_SERVER * resp = (SERVER_NOTIFY_RESP_SERVER *)h;
			resp->serialize(errorcode::notify_error_protocol_length, from, to);
			conn->start_async_write(buffer, resp->length);
		}
		else
		{
			stdext::hash_map<int, handler_ptr>::iterator iter = cons_.find(to);
			if (iter != cons_.end())
			{
				iter->second->start_async_write(buffer, req->length);
			}
			else
			{
				SERVER_NOTIFY_RESP_SERVER * resp = (SERVER_NOTIFY_RESP_SERVER *)h;
				resp->serialize(errorcode::notify_error_to_id_nonexistent, from, to);
				conn->start_async_write(buffer, resp->length);
			}
		}

		return true;
	}

	boost::asio::ip::address_v4 addr(conn->address());
	logic_check(true, ("IP:%s, ID:%d, Unknown protocol", 
		addr.to_string().c_str(), conn->id()));

	return false;
}

void request_handler::handle_disconnect(connection * conn, const boost::system::error_code& err)
{
	if (!conn->id()) return ;

	cons_.erase(conn->id());
}

