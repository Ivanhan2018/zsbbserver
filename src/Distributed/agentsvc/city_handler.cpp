#include "city_handler.hpp"
#include "connection.hpp"
#include "distributed_protocol.hpp"
#include "zlib.h"
#include "log.hpp"

city_handler::city_handler() :
agent_id_(0)
{
}

bool city_handler::start(int agent_id, int id, connection * conn)
{
	if (is_run()) return false;

	agent_id_ = agent_id;
	conn->id() = id;
	city_ = conn->shared_from_this();
	return task_base_ex<connection, char *>::start();
}

bool city_handler::stop()
{
	if (!task_base_ex<connection, char *>::stop())
		return false;

	city_.reset();
	cnts_.clear();
	return true;
}

bool city_handler::zlib_compress(char *& desc, char * src)
{
	static int header_len = sizeof(DISTRIBUTED_HEADER);

	DISTRIBUTED_HEADER * header = (DISTRIBUTED_HEADER *)src;
	char * ptr = src + header_len;
	int ptr_len = header->length - header_len;

	uLong desc_len = compressBound(ptr_len) + header_len;
	desc = new char[desc_len];
	::memcpy(desc, src, header_len);

	if (compress((Bytef *)(desc + header_len),
		&desc_len, (Bytef *)ptr, ptr_len) != Z_OK)
	{
		delete [] desc;
		desc = 0;
		return false;
	}

	header = (DISTRIBUTED_HEADER *)desc;
	header->length = desc_len + header_len;
	header->src_len = ptr_len + header_len;
	header->compress = 1;

	LOG_ERROR(("%d,%d,%.4f", header->src_len, header->length, ((float)header->length / (float)header->src_len)*100));
	return true;
}

bool city_handler::zlib_uncompress(char *& desc, char * src)
{
	static int header_len = sizeof(DISTRIBUTED_HEADER);

	DISTRIBUTED_HEADER * header = (DISTRIBUTED_HEADER *)src;
	char * ptr = src + header_len;
	int ptr_len = header->length - header_len;

	uLong desc_len = header->src_len;
	desc = new char[desc_len];
	::memcpy(desc, src, header_len);

	if (uncompress((Bytef *)(desc + header_len),
		&desc_len, (Bytef *)ptr, ptr_len) != Z_OK)
	{
		delete [] desc;
		desc = 0;
		return false;
	}

	header = (DISTRIBUTED_HEADER *)desc;
	header->length = desc_len + header_len;
	header->src_len = 0;
	header->compress = 0;
	return true;
}

void city_handler::run()
{
	handler_ptr conn;
	char * buffer, * ptr, * ptr_1, * tmp;
	DISTRIBUTED_HEADER * h;
	AGENT_SERVER_RESPONSE * resp;
	clients::iterator it;
	int len;

	while (pop(conn, buffer))
	{
		h = (DISTRIBUTED_HEADER *)buffer;
		resp = (AGENT_SERVER_RESPONSE *)h;

		if (buffer == (char *)event_disconnect)
		{
			it = cnts_.find(conn->id());
			if (it != cnts_.end())
			{
				tmp = new char[sizeof(AGENT_CLIENT_LEAVE_SERVER)];
				AGENT_CLIENT_LEAVE_SERVER * resp = (AGENT_CLIENT_LEAVE_SERVER *)tmp;

				resp->serialize(agent_id_, city_->id(), conn->id());
				cnts_.erase(it);
				city_->start_async_write(tmp, resp->length);
			}
		}
		else if (h->command == CMD_SERVER_BROADCAST_CLIENT)
		{
			ptr = buffer + sizeof(SERVER_BROADCAST_CLIENT);
			len = h->length - sizeof(SERVER_BROADCAST_CLIENT);
			h = (DISTRIBUTED_HEADER *)ptr;
			h->agent = agent_id_;
			h->from = city_->id();

			if (len >= LARGE_BOUND)
			{
				logic_check(!zlib_compress(ptr_1, ptr), ("compress error"));
			}
			else
			{
				ptr_1 = ptr;
			}

			h = (DISTRIBUTED_HEADER *)ptr_1;

			__log_handler->add_bandwidth(h->length, 2, cnts_.size());

			for (it = cnts_.begin(); it != cnts_.end(); it++)
			{
				h->to = it->second->id();
				tmp = new char[h->length];
				memcpy(tmp, ptr_1, h->length);
				it->second->start_async_write(tmp, h->length);
			}
			delete [] buffer;
			if (len >= LARGE_BOUND) delete [] ptr_1;
		}
		else
		{
			if (h->from == city_->id())
			{
				if (!h->command)
				{
					resp->serialize(errorcode::server_request_error_command_null, agent_id_, h->from, h->to);

					__log_handler->add_bandwidth(resp->length);

					conn->start_async_write((char *)resp, resp->length);
				}
				else if (h->to < 100000)
				{
					resp->serialize(errorcode::server_request_error_to_id_less_100000, agent_id_, h->from, h->to);

					__log_handler->add_bandwidth(resp->length);

					conn->start_async_write((char *)resp, resp->length);
				}
				else
				{
					it = cnts_.find(h->to);
					if (it == cnts_.end())
					{
						resp->serialize(errorcode::server_request_error_to_id_nonexistent, agent_id_, h->from, h->to);

						__log_handler->add_bandwidth(resp->length);

						conn->start_async_write((char *)resp, resp->length);
					}
					else
					{
						h->agent = agent_id_;

						if (h->length >= LARGE_BOUND)
						{
							logic_check(!zlib_compress(ptr_1, buffer), ("compress error"));
							delete [] buffer;
						}
						else
						{
							ptr_1 = buffer;
						}

						h = (DISTRIBUTED_HEADER *)ptr_1;

						__log_handler->add_bandwidth(h->length);

						it->second->start_async_write(ptr_1, h->length);
					}
				}
			}
			else
			{
				if (h->from < 100000)
				{
					resp->serialize(errorcode::client_request_error_from_id_less_100000, agent_id_, h->from, h->to);
					conn->start_async_write((char *)resp, resp->length);
				}
				else if (h->to != city_->id())
				{
					resp->serialize(errorcode::client_request_error_to_id_overflow, agent_id_, h->from, h->to);
					conn->start_async_write((char *)resp, resp->length);
				}
				else if (!h->command)
				{
					resp->serialize(errorcode::client_request_error_command_null, agent_id_, h->from, h->to);
					conn->start_async_write((char *)resp, resp->length);
				}
				else
				{
					it = cnts_.find(h->from);
					if (it == cnts_.end())
					{
						conn->id() = h->from;
						cnts_.insert(std::pair<int, handler_ptr>(h->from, conn));
					}

					if (h->command == REQ_LEAVE_FIGHT_SYSTEM_INFO)
					{
						cnts_.erase(h->from);
					}

					h->agent = agent_id_;

					if (h->compress == 1)
					{
						logic_check(!zlib_uncompress(ptr_1, buffer), ("uncompress error"));
						delete [] buffer;
					}
					else
					{
						ptr_1 = buffer;
					}

					h = (DISTRIBUTED_HEADER *)ptr_1;
					city_->start_async_write(ptr_1, h->length);
				}
			}
		}
		conn.reset();
	}
}

void city_handler::exit(char * buffer)
{
	delete [] buffer;
}
