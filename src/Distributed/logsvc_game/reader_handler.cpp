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

#include "reader_handler.hpp"
#include "connection.hpp"
#include "distributed_protocol.hpp"
#include "zlib.h"

bool reader_handler::handle_message(connection * conn, char * buffer, std::size_t length)
{
	if (!is_run()) return false;
	logic_check(!push(conn->shared_from_this(), buffer), ("logsvc_game service push error"));
	return true;
}

bool reader_handler::query_file(int bdate, int edate, std::vector<std::string> & filelist)
{
	fs::path root_path("./log_game");

	if (!fs::exists(root_path)) return false;

	std::string tmp;

	fs::directory_iterator ym_end_iter, day_end_iter, file_end_iter;

	for (fs::directory_iterator ym_itr(root_path); ym_itr != ym_end_iter; ++ym_itr)
	{
		if (!fs::is_directory(ym_itr->status())) continue;

		for (fs::directory_iterator day_itr(ym_itr->path()); day_itr != day_end_iter; ++day_itr)
		{
			if (!fs::is_directory(day_itr->status())) continue;

			tmp = ym_itr->path().string() + day_itr->path().string().c_str();
			int date = atoi(tmp.c_str());

			if (date < bdate || date > edate) continue;

			for (fs::directory_iterator file_itr(day_itr->path()); file_itr != file_end_iter; ++file_itr)
			{
				if (!fs::is_regular(file_itr->status())) continue;
				filelist.push_back(file_itr->path().directory_string());
			}
		}
	}

	return !filelist.empty();
}

bool reader_handler::zlib_compress(char *& desc, char * src)
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

	return true;
}

void reader_handler::run()
{
	char * buf;
	handler_ptr conn;

	unsigned int file_size;
	DISTRIBUTED_HEADER * header;
	LOG_GAME_READER * reader;
	LOG_GAME_READER_RSP * reader_rsp;
	std::vector<std::string> filelist;

	FILE * fp = 0;
	char * large_buffer = (char *)::malloc(12582912);
	unsigned int large_buf_len = 12582912;

	while (pop(conn, buf))
	{
		header = (DISTRIBUTED_HEADER *)buf;
		if (header->command == CMD_LOG_GAME_READER)
		{
			reader = (LOG_GAME_READER *)buf;

			if (!query_file(reader->bdate, reader->edate, filelist))
			{
				reader_rsp = (LOG_GAME_READER_RSP *)buf;
				reader_rsp->serialize(-1, 0, 0, " ");
				conn->start_async_write(buf, reader_rsp->length);
			}
			else
			{
				int curpos;
				for (unsigned int i = 0; i < filelist.size(); i++)
				{
					if (!conn->connected()) break;

					fp = ::fopen(filelist[i].c_str(), "rt");
					logic_check(!fp, ("fopen error\n"));

					curpos = ::ftell(fp);
					::fseek(fp, 0L, SEEK_END);
					file_size = ::ftell(fp);
					::fseek(fp, curpos, SEEK_SET);

					if ((file_size + sizeof(*reader_rsp)) > large_buf_len)
					{
						::free(large_buffer);
						large_buf_len = file_size + sizeof(*reader_rsp);
						large_buffer = (char *)::malloc(large_buf_len);
					}

					reader_rsp = (LOG_GAME_READER_RSP *)large_buffer;
					file_size = ::fread(reader_rsp->buffer(), 1, file_size, fp);
					fclose(fp);

					reader_rsp->serialize(i + 1, filelist.size(), file_size, filelist[i].c_str());

					char * ptr = 0;
					logic_check(!zlib_compress(ptr, large_buffer), ("compress error"));
					reader_rsp = (LOG_GAME_READER_RSP *)ptr;
					conn->start_async_write(ptr, reader_rsp->length);
					Sleep(10);
				}

				filelist.clear();
				delete [] buf;
			}
		}
		conn.reset();
	}

	::free(large_buffer);
}
