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

#ifndef __DB_SINGLE_INDEX_HANDLER_HPP__
#define __DB_SINGLE_INDEX_HANDLER_HPP__

template <typename db_type>
class db_single_index_handler : public db_handler
{
	typedef stdext::hash_map<std::string, db_type *> db_table;
	typedef typename db_table::iterator db_iterator;

public:
	explicit db_single_index_handler();
	virtual ~db_single_index_handler(){};

	bool start(const char *);
	bool stop();

protected:
	void clear_map();
	void exit(char *);
	void run();

	db_table table_;
	int only_id_;
	db_allocator<db_type> alloc_;
};


template <typename db_type>
db_single_index_handler<db_type>::db_single_index_handler()
{
	only_id_ = 0;
}

template <typename db_type>
bool db_single_index_handler<db_type>::start(const char * con_str)
{
	if (is_run()) return false;

	printf("Initialize Database(%s)......", db_type::getname());

	otl_connect db;
	try 
	{ 
		char key[255];
		db.rlogon(con_str);

		std::string qry = "SELECT id_increase FROM id_manager WHERE tablename=:f<char[50]>";

		otl_stream os;
		os.open((int)qry.size(), qry.c_str(), db);
		os << db_type::getname();

		if (!os.eof())
			os >> only_id_;
		else
			only_id_ = 0;

		os.close();

		int len = (int)strlen(db_type::get_select_str());
		os.open(len, db_type::get_select_str(), db);

		while (!os.eof())
		{
			db_type * r = alloc_.malloc();
			os >> (*r);

			r->getid(key);

			db_iterator iter = table_.find(key);
			if (iter == table_.end())
			{
				table_.insert(std::pair<std::string, db_type *>(key, r));
			}
			else 
			{
				alloc_.free(r);
				LOG_ERROR(("表 %s，初始化失败，出现重复记录，关健字 %s",
					db_type::getname(),key));
				
				db.logoff();
				return clear_map(), false;
			}
		}
	}
	catch(otl_exception& p)
	{ 
		printf("FAILED\n%s\n", p.msg);
		db.logoff();
		return clear_map(), false;
	}

	task_base_ex<connection, char *>::start();

	printf("SUCCEED\n");
	return true;
}

template <typename db_type>
bool db_single_index_handler<db_type>::stop()
{
	if (!task_base_ex<connection, char *>::stop())
		return false;

	printf("Uninitialize Database(%s)......", db_type::getname());

	otl_connect db;

	try 
	{
		db.rlogon(db_write_handler::instance().db_con_str_.c_str());

		std::string qry = "SELECT id_increase FROM id_manager WHERE tablename=:f<char[50]>";
		otl_stream os;
		os.open((int)qry.size(), qry.c_str(), db);
		os << db_type::getname();

		if (!os.eof())
		{
			os.close();
			qry = "UPDATE id_manager SET id_increase=:f<int> WHERE tablename=:g<char[50]>";
			os.open((int)qry.size(), qry.c_str(), db);
			os << only_id_ << db_type::getname();
		}
		os.close();
	}
	catch(otl_exception& p)
	{ 
		LOG_ERROR(("表 %s，反初始化失败，\n%s", db_type::getname(),p.msg));
		printf("FAILED\n");
		clear_map();
		db.logoff();
		return false;
	}

	clear_map();
	db.logoff();
	printf("SUCCEED\n");

	return true;
}

template <typename db_type>
void db_single_index_handler<db_type>::clear_map()
{
	for (db_iterator iter = table_.begin(); 
		iter != table_.end (); iter++)
		alloc_.free(iter->second);

	table_.clear();
}

template <typename db_type>
void db_single_index_handler<db_type>::exit(char * buffer)
{
	delete [] buffer;
}

template <typename db_type>
void db_single_index_handler<db_type>::run()
{
	char key[255];
	char key_new[255];
	DISTRIBUTED_HEADER * h;
	static int header_length = sizeof(*h);
	int ext_length = sizeof(db_type);

	db_type * new_ext, * ext, * ext1;
	handler_ptr conn;
	char * buffer;

	while (pop(conn, buffer) == SUCCEED)
	{
		h = (DISTRIBUTED_HEADER *)buffer;

		// 安全检查，应付一下，没办法
		if (h->command == CMD_ACTION_UPDATE)
		{
			if ((ext_length * 2 + header_length) != h->length)
			{
				delete [] buffer;
				conn->socket().close();
				conn.reset();
				continue;
			}
		}
		else if ((ext_length + header_length) != h->length)
		{
			delete [] buffer;
			conn->socket().close();
			conn.reset();
			continue;
		}

		ext = (db_type *)(buffer + header_length);

		if (h->command == CMD_ACTION_APPEND)
			ext->setkeyvalue(++only_id_);

		ext->getid(key);

		if (h->command == CMD_ACTION_APPEND)
		{
			db_iterator iter = table_.find(key);
			if (iter == table_.end())
			{
				new_ext = alloc_.malloc();
				::memcpy(new_ext, ext, ext_length);
				table_.insert(std::pair<std::string, db_type *>(key, new_ext));

				conn->start_async_write((char *)h, h->length);

				char * ptr = new char[1024];
				*(int*)ptr = new_ext->get_insert_str(ptr + 4);
				if (!db_write_handler::instance().push(ptr)) delete [] ptr;
			}
			else 
			{
				--only_id_;
				h->length = header_length;
				conn->start_async_write((char *)h, h->length);
		
				LOG_ERROR(("表 %s，添加失败，关健字 %s 已存在", 
					db_type::getname(),key));
			}
		}
		else if (h->command == CMD_ACTION_UPDATE)
		{
			db_iterator iter = table_.find(key);
			if (iter != table_.end())
			{
				ext1 = (db_type *)(buffer + header_length + ext_length);
				ext1->getid(key_new);

				if (strcmp(key, key_new) != 0)
				{
					h->length = header_length;
					conn->start_async_write((char *)h, h->length);

					LOG_ERROR(("表 %s 更新时主健不一致, 旧主键(%s),新主键(%s)", 
						db_type::getname(), key, key_new));
				}
				else
				{
					ext1->setkeyvalue(iter->second->getkeyvalue());
					::memcpy(iter->second, ext1, ext_length);

					char * ptr = new char[1024];
					*(int*)ptr = ext1->get_update_str(ptr + 4, *ext);

					h->length = header_length + ext_length;
					::memcpy((char *)h + header_length, ext1, ext_length);

					conn->start_async_write((char *)h, h->length);

					if (!db_write_handler::instance().push(ptr)) delete [] ptr;
				}
			}
			else
			{
				h->length = header_length;
				conn->start_async_write((char *)h, h->length);

				LOG_ERROR(("表 %s，更新失败，关健字 %s 不存在",
					db_type::getname(), key));
			}
		}
		else if (h->command == CMD_ACTION_DELETE)
		{
			db_iterator iter = table_.find(key);
			if (iter != table_.end())
			{
				char * ptr = new char[1024];
				*(int*)ptr = iter->second->get_delete_str(ptr + 4);

				alloc_.free(iter->second);
				table_.erase(iter);

				conn->start_async_write((char *)h, h->length);

				if (!db_write_handler::instance().push(ptr)) delete [] ptr;
			}
			else
			{
				h->length = header_length;
				conn->start_async_write((char *)h, h->length);

				LOG_ERROR(("表 %s，删除失败，关健字 %s 不存在", 
					db_type::getname(), key));
			}
		}
		else if (h->command == CMD_ACTION_SELECT)
		{
			db_iterator iter = table_.find(key);
			if (iter != table_.end())
			{
				::memcpy((char *)h + header_length, 
					iter->second, ext_length);
			}
			else
			{
				h->length = header_length;
				LOG_ERROR(("表 %s，查询失败，关健字 %s 不存在",
					db_type::getname(), key));
			}

			conn->start_async_write((char *)h, h->length);
		}
		else if (h->command == CMD_ACTION_SELECT_ALL)
		{
			h->length = header_length + (int)table_.size() * ext_length;
			char * t = new char[h->length];
			::memcpy(t, h, header_length);
			int offset = 0;

			for (db_iterator iter = table_.begin (); 
				iter != table_.end (); iter++)
			{
				::memcpy(t + header_length + (offset * 
					ext_length), iter->second, ext_length);
				++offset;
			}

			conn->start_async_write(t, h->length);
			delete [] buffer;
		}
		else if (h->command == CMD_ACTION_INCREASE)
		{
			db_iterator iter = table_.find(key);
			if (iter != table_.end())
			{
				(*iter->second) += (*ext);

				char * ptr = new char[1024];
				*(int*)ptr = iter->second->get_update_str(ptr + 4, *ext);

				::memcpy((char *)h + header_length, iter->second, ext_length);

				conn->start_async_write((char *)h, h->length);

				if (!db_write_handler::instance().push(ptr)) delete [] ptr;
			}
			else
			{
				h->length = header_length;
				conn->start_async_write((char *)h, h->length);
			}
		}
		else
		{
			delete [] buffer;
		}
		conn.reset();
	}
}


#endif // __DB_SINGLE_INDEX_HANDLER_HPP__
