#include "request_handler.hpp"
#include "writer_handler.hpp"
#include "connection.hpp"
#include "distributed_protocol.hpp"

bool request_handler::start()
{
	writer_ = new writer_handler;
	writer_->start();
	reader_.start();
	return true;
}

void request_handler::stop()
{
	writer_->stop();
	delete writer_;

	reader_.stop();
}

bool request_handler::handle_message(connection * conn, char * buf, std::size_t len)
{
	DISTRIBUTED_HEADER * h = (DISTRIBUTED_HEADER *)buf;

	if (h->command == CMD_LOG_GAME_WRITER)
	{
		return writer_->push(buf, len);
	}
	else if (h->command == CMD_LOG_GAME_READER)
	{
		return reader_.push(conn->shared_from_this(), buf) == reader_handler::SUCCEED;
	}
	else 
	{
		return false;
	}

	return true;
}

