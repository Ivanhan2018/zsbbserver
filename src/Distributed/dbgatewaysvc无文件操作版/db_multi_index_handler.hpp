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

#ifndef __DB_MULTI_INDEX_HANDLER_HPP__
#define __DB_MULTI_INDEX_HANDLER_HPP__

template <typename db_type>
class db_multi_index_handler : public db_handler
{
	typedef stdext::hash_map< std::string, db_type * > db_detail;
	typedef typename db_detail::iterator db_detail_iterator;

	typedef stdext::hash_map< std::string, db_detail * > db_master;
	typedef typename db_master::iterator db_master_iterator;

public:
	explicit db_multi_index_handler();
	virtual ~db_multi_index_handler(){};

	bool start(const char *);
	bool stop();

protected:
	void clear_map();
	void exit(char *);
	void run();

	db_master table_;
	int record_count_;
	int only_id_;
	db_allocator<db_type> alloc_;
};


template <typename db_type>
db_multi_index_handler<db_type>::db_multi_index_handler()
{
	only_id_ = 0;
	record_count_ = 0;
}

template <typename db_type>
bool db_multi_index_handler<db_type>::start(const char * con_str)
{
	if (is_run()) return false;

	printf("Initialize Database(%s)......", db_type::getname());

	otl_connect db;
	try 
	{ 
		db.rlogon(con_str);

		db_detail * detail = 0;
		char master_key[255];
		char detail_key[255];

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

			r->getmasterid(master_key);
			r->getdetailid(detail_key);

			db_master_iterator iter = table_.find(master_key);
			if (iter == table_.end())
			{
				detail = new db_detail;
				table_.insert(std::pair<std::string, db_detail *>(master_key, detail));
			}
			else
			{
				detail = iter->second;
			}

			if (detail->find(detail_key) == detail->end())
			{
				detail->insert(std::pair<std::string, db_type *>(detail_key, r));
				++record_count_;
			}
			else 
			{
				alloc_.free(r);
				LOG_ERROR(("表 %s，初始化失败，出现重复记录，新键值(%s|%s)",
					db_type::getname(),master_key, detail));

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
bool db_multi_index_handler<db_type>::stop()
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
void db_multi_index_handler<db_type>::clear_map()
{
	for (db_master_iterator iter = table_.begin (); 
		iter != table_.end (); iter++)
	{
		db_detail_iterator subiter;
		for (subiter = iter->second->begin (); 
			subiter != iter->second->end (); subiter++)
		{
			alloc_.free(subiter->second);
		}
		delete iter->second;
	}

	table_.clear();
}

template <typename db_type>
void db_multi_index_handler<db_type>::exit(char * buffer)
{
	delete [] buffer;
}

template <typename db_type>
void db_multi_index_handler<db_type>::run()
{
	char master_key[255];
	char detail_key[255];

	char master_key_new[255];
	char detail_key_new[255];

	db_detail * detail = 0;
	DISTRIBUTED_HEADER * h = 0;

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

		ext->getmasterid(master_key);
		ext->getdetailid(detail_key);

		if (h->command == CMD_ACTION_APPEND)
		{
			db_master_iterator iter = table_.find(master_key);
			if (iter == table_.end())
			{
				detail = new db_detail;
				table_.insert(std::pair<std::string, db_detail *>(master_key, detail));
			}
			else
			{
				detail = iter->second;
			}

			if (detail->find(detail_key) == detail->end())
			{
				new_ext = alloc_.malloc();
				::memcpy(new_ext, ext, ext_length);

				detail->insert(std::pair<std::string, db_type *>(detail_key, new_ext));
				++record_count_;

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

				LOG_ERROR(("表 %s，添加失败，键值(%s|%s) 已存在",
					db_type::getname(),master_key, detail_key));
			}
		}
		else if (h->command == CMD_ACTION_UPDATE)
		{
			db_master_iterator iter;
			db_detail_iterator subiter;

			ext1 = (db_type *)(buffer + header_length + ext_length);
			ext1->getmasterid(master_key_new);
			ext1->getdetailid(detail_key_new);

			if (db_type::gettype() == CMD_ARMY_TABLE)
			{
				// 先应负一下，注意风险，要更正
				bool flag = (strcmp(master_key, master_key_new) || strcmp(detail_key, detail_key_new));

				if (flag && (iter = table_.find(master_key_new)) != table_.end()) // 新主键是否存在？
				{
					if (iter->second->find(detail_key_new) != iter->second->end()) // 新外键是否存在？
					{
						// 已存在，返回失败，不能添加
						// 先应负一下，注意风险，要更正
						h->length = header_length;
						conn->start_async_write((char *)h, h->length);
						conn.reset();

						LOG_ERROR(("表 %s 更新时出错，该新记录已存在, 旧键值(%s|%s),新键值(%s|%s)", 
							db_type::getname(), master_key, detail_key, master_key_new, detail_key_new));
						continue;
					}
				}

				if ((iter = table_.find(master_key)) == table_.end()) // 主键是否存在？
				{
					// 没有这个主键
					h->length = header_length;
					conn->start_async_write((char *)h, h->length);

					LOG_ERROR(("表 %s，更新失败，旧键值(%s|%s) 不存在", 
						db_type::getname(),master_key, detail_key));
				}
				else if ((subiter = iter->second->find(detail_key)) == iter->second->end()) // 外键是否已存在？
				{
					// 没有这个外键
					h->length = header_length;
					conn->start_async_write((char *)h, h->length);

					LOG_ERROR(("表 %s，更新失败，旧键值(%s|%s) 不存在", 
						db_type::getname(),master_key, detail_key));
				}
				else
				{
					if (!flag) // 不用修改关健字
					{
						// 相同的主键和外键，不修改主外键，主修改其它字段
						ext1->setkeyvalue(subiter->second->getkeyvalue());
						::memcpy(subiter->second, ext1, ext_length);

						char * ptr = new char[1024];
						*(int*)ptr = ext1->get_update_str(ptr + 4, *ext);

						h->length = header_length + ext_length;
						::memcpy((char *)h + header_length, ext1, ext_length);
						conn->start_async_write((char *)h, h->length);

						if (!db_write_handler::instance().push(ptr)) delete [] ptr;
					}
					else // 需要修改关健字
					{
						new_ext = subiter->second;
						ext1->setkeyvalue(subiter->second->getkeyvalue());
						iter->second->erase(subiter);

						if ((iter = table_.find(master_key_new)) == table_.end())
						{
							detail = new db_detail;
							table_.insert(std::pair<std::string, db_detail *>(master_key_new, detail));
						}
						else
						{
							detail = iter->second;
						}

						::memcpy(new_ext, ext1, ext_length);
						detail->insert(std::pair<std::string, db_type *>(detail_key_new, new_ext));
						h->length = header_length + ext_length;

						char * ptr = new char[1024];
						*(int*)ptr = ext1->get_update_str(ptr + 4, *ext);

						::memcpy((char *)h + header_length, new_ext, ext_length);
						conn->start_async_write((char *)h, h->length);

						if (!db_write_handler::instance().push(ptr)) delete [] ptr;
					}
				}
			}
			else
			{
				if (strcmp(master_key, master_key_new) != 0)
				{
					h->length = header_length;
					conn->start_async_write((char *)h, h->length);

					LOG_ERROR(("表 %s 更新时主健不一致, 旧键值(%s|%s),新键值(%s|%s)", 
						db_type::getname(), master_key, detail_key, master_key_new, detail_key_new));
				}
				else if (strcmp(detail_key, detail_key_new) != 0)
				{
					h->length = header_length;
					conn->start_async_write((char *)h, h->length);

					LOG_ERROR(("表 %s 更新时外健不一致, 旧键值(%s|%s),新键值(%s|%s)", 
						db_type::getname(), master_key, detail_key, master_key_new, detail_key_new));
				}
				else if ((iter = table_.find(master_key)) == table_.end()) // 主键是否存在？
				{
					// 没有这个主键
					h->length = header_length;
					conn->start_async_write((char *)h, h->length);

					LOG_ERROR(("表 %s，更新失败，旧键值(%s|%s) 不存在", 
						db_type::getname(),master_key, detail_key));
				}
				else if ((subiter = iter->second->find(detail_key)) == iter->second->end()) // 外键是否已存在？
				{
					// 没有这个外键
					h->length = header_length;
					conn->start_async_write((char *)h, h->length);

					LOG_ERROR(("表 %s，更新失败，旧键值(%s|%s) 不存在", 
						db_type::getname(),master_key, detail_key));
				}
				else
				{
					// 相同的主键和外键，不修改主外键，主修改其它字段
					ext1->setkeyvalue(subiter->second->getkeyvalue());
					::memcpy(subiter->second, ext1, ext_length);

					char * ptr = new char[1024];
					*(int*)ptr = ext1->get_update_str(ptr + 4, *ext);

					h->length = header_length + ext_length;
					::memcpy((char *)h + header_length, ext1, ext_length);
					conn->start_async_write((char *)h, h->length);

					if (!db_write_handler::instance().push(ptr)) delete [] ptr;
				}
			}
		}
		else if (h->command == CMD_ACTION_DELETE)
		{
			db_master_iterator iter = table_.find(master_key);
			if (iter == table_.end())
			{
				h->length = header_length;
				conn->start_async_write((char *)h, h->length);

				LOG_ERROR(("表 %s，删除失败，键值(%s|%s) 不存在", 
						db_type::getname(),master_key, detail_key));
			}
			else
			{
				db_detail_iterator subiter = iter->second->find(detail_key);
				if (subiter != iter->second->end())
				{
					char * ptr = new char[1024];
					*(int*)ptr = subiter->second->get_delete_str(ptr + 4);

					alloc_.free(subiter->second);
					iter->second->erase(subiter);
					--record_count_;

					conn->start_async_write((char *)h, h->length);
					if (!db_write_handler::instance().push(ptr)) delete [] ptr;
				}
				else
				{
					h->length = header_length;
					conn->start_async_write((char *)h, h->length);

					LOG_ERROR(("表 %s，删除失败，键值(%s|%s) 不存在", 
							db_type::getname(),master_key, detail_key));
				}
			}
		}
		else if (h->command == CMD_ACTION_SELECT)
		{
			db_master_iterator iter = table_.find(master_key);
			if (iter == table_.end())
			{
				h->length = header_length;

				LOG_ERROR(("表 %s，查询失败，键值(%s|%s) 不存在", 
						db_type::getname(),master_key, detail_key));
			}
			else
			{
				db_detail_iterator subiter = iter->second->find(detail_key);
				if (subiter != iter->second->end())
				{
					::memcpy((char *)h + header_length, 
						subiter->second, ext_length);
				}
				else
				{
					h->length = header_length;

					LOG_ERROR(("表 %s，查询失败，键值(%s|%s) 不存在", 
						db_type::getname(),master_key, detail_key));
				}
			}

			conn->start_async_write((char *)h, h->length);
		}
		else if (h->command == CMD_ACTION_MULTI_SELECT)
		{
			db_master_iterator iter = table_.find(master_key);
			if (iter == table_.end())
			{
				h->length = header_length;
				conn->start_async_write((char *)h, h->length);
			}
			else
			{
				h->length = header_length + (int)iter->second->size() * ext_length;
				char * t = new char[h->length];
				::memcpy(t, h, header_length);
				int offset = 0;

				db_detail_iterator subiter;
				for (subiter = iter->second->begin (); 
					subiter != iter->second->end (); subiter++)
				{
					::memcpy(t + header_length + (offset * ext_length), 
						subiter->second, ext_length);
					++offset;
				}

				conn->start_async_write(t, h->length);
				delete [] buffer;
			}
		}
		else if (h->command == CMD_ACTION_SELECT_ALL)
		{
			h->length = header_length + record_count_ * ext_length;
			char * t = new char[h->length];
			::memcpy(t, h, header_length);
			int offset = 0;

			for (db_master_iterator iter = table_.begin (); 
				iter != table_.end (); iter++)
			{
				db_detail_iterator subiter;
				for (subiter = iter->second->begin (); 
					subiter != iter->second->end (); subiter++)
				{
					::memcpy(t + header_length + (offset * ext_length), 
						subiter->second, ext_length);
					++offset;
				}
			}

			conn->start_async_write(t, h->length);
			delete [] buffer;
		}
		else if (h->command == CMD_ACTION_INCREASE)
		{
			db_master_iterator iter = table_.find(master_key);
			if (iter == table_.end())
			{
				h->length = header_length;
				conn->start_async_write((char *)h, h->length);
			}
			else
			{
				db_detail_iterator subiter = iter->second->find(detail_key);
				if (subiter != iter->second->end())
				{
					(*subiter->second) += (*ext);

					char * ptr = new char[1024];
					*(int*)ptr = subiter->second->get_update_str(ptr + 4, *ext);

					::memcpy((char *)h + header_length, subiter->second, ext_length);
					conn->start_async_write((char *)h, h->length);

					if (!db_write_handler::instance().push(ptr)) delete [] ptr;
				}
				else
				{
					h->length = header_length;
					conn->start_async_write((char *)h, h->length);
				}
			}
		}
		else
		{
			delete [] buffer;
		}

		conn.reset();
	}
}


#endif // __DB_MULTI_INDEX_HANDLER_HPP__

