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

	bool start(const char *, bool);
	bool stop();

protected:
	void clear_map();
	void exit(char *);
	void run();

	void save_to_file();
	bool load_from_file(const char *);
	bool load_from_db(const char *);
	bool save_to_db_all();

	db_master table_;
	int record_count_;
	int only_id_;
	bool id_mgr_;
	db_allocator<db_type> alloc_;
};


template <typename db_type>
db_multi_index_handler<db_type>::db_multi_index_handler()
{
	id_mgr_ = false;
	only_id_ = 0;
	record_count_ = 0;
}

template <typename db_type>
bool db_multi_index_handler<db_type>::start(const char * con_str, bool fromfile)
{
	if (is_run()) return false;

	if (fromfile)
	{
		printf("Load from File(%s)......", db_type::getname());
		if (!load_from_file(con_str))
		{
			printf("FAILED\n");
			return false;
		}
		else
			printf("SUCCEED\n");
	}
	else
	{
		printf("Load from Database(%s)......", db_type::getname());
		if (!load_from_db(con_str))
		{
			printf("FAILED\n");
			return false;
		}
		else
			printf("SUCCEED\n");
	}

	task_base_ex<connection, char *>::start();
	return true;
}

template <typename db_type>
bool db_multi_index_handler<db_type>::stop()
{
	if (!task_base_ex<connection, char *>::stop())
		return false;

	printf("Uninitialize Database(%s)......", db_type::getname());

	save_to_file();
	clear_map();
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
		if (buffer == (char *)0xCCCC)
		{
			save_to_file();
			printf("Save to File(%s)......SUCCEED\n", db_type::getname());
			continue;
		}

		if (buffer == (char *)0xCBCB)
		{
			save_to_db_all();
			printf("Save to DB(%s)......SUCCEED\n", db_type::getname());
			continue;
		}

		h = (DISTRIBUTED_HEADER *)buffer;

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

		if ((h->command == CMD_ACTION_APPEND) && id_mgr_)
			ext->setkeyvalue(++only_id_);

		ext->getmasterid(master_key);
		ext->getdetailid(detail_key);

		if (h->command == CMD_ACTION_APPEND)
		{
			::InterlockedIncrement(&db_write_handler::instance().insert_);

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

				if (id_mgr_)
				{
					ptr = new char[256];
					*(int*)ptr = sprintf(ptr + 4, " UPDATE id_manager SET id_increase=%d WHERE tablename=\'\'%s\'\' ", 
						only_id_, db_type::getname());

					if (!db_write_handler::instance().push(ptr)) delete [] ptr;
				}
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
			::InterlockedIncrement(&db_write_handler::instance().update_);

			db_master_iterator iter;
			db_detail_iterator subiter;

			ext1 = (db_type *)(buffer + header_length + ext_length);
			ext1->getmasterid(master_key_new);
			ext1->getdetailid(detail_key_new);

			if ((db_type::gettype() == CMD_ARMY_TABLE) ||
				(db_type::gettype() == CMD_PLAYER_ITEM_STORAGE_TABLE))
			{
				bool flag = (strcmp(master_key, master_key_new) || strcmp(detail_key, detail_key_new));

				if (flag && (iter = table_.find(master_key_new)) != table_.end()) 
				{
					if (iter->second->find(detail_key_new) != iter->second->end())
					{
						h->length = header_length;
						conn->start_async_write((char *)h, h->length);
						conn.reset();

						LOG_ERROR(("表 %s 更新时出错，该新记录已存在, 旧键值(%s|%s),新键值(%s|%s)", 
							db_type::getname(), master_key, detail_key, master_key_new, detail_key_new));
						continue;
					}
				}

				if ((iter = table_.find(master_key)) == table_.end())
				{
					h->length = header_length;
					conn->start_async_write((char *)h, h->length);

					LOG_ERROR(("表 %s，更新失败，旧键值(%s|%s) 不存在", 
						db_type::getname(),master_key, detail_key));
				}
				else if ((subiter = iter->second->find(detail_key)) == iter->second->end())
				{
					h->length = header_length;
					conn->start_async_write((char *)h, h->length);

					LOG_ERROR(("表 %s，更新失败，旧键值(%s|%s) 不存在", 
						db_type::getname(),master_key, detail_key));
				}
				else
				{
					if (!flag)
					{
						ext1->setkeyvalue(subiter->second->getkeyvalue());
						::memcpy(subiter->second, ext1, ext_length);

						char * ptr = new char[1024];
						*(int*)ptr = ext1->get_update_str(ptr + 4, *ext);

						h->length = header_length + ext_length;
						::memcpy((char *)h + header_length, ext1, ext_length);
						conn->start_async_write((char *)h, h->length);

						if (!db_write_handler::instance().push(ptr)) delete [] ptr;
					}
					else
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
				else if ((iter = table_.find(master_key)) == table_.end()) 
				{
					h->length = header_length;
					conn->start_async_write((char *)h, h->length);

					LOG_ERROR(("表 %s，更新失败，旧键值(%s|%s) 不存在", 
						db_type::getname(),master_key, detail_key));
				}
				else if ((subiter = iter->second->find(detail_key)) == iter->second->end()) 
				{
					h->length = header_length;
					conn->start_async_write((char *)h, h->length);

					LOG_ERROR(("表 %s，更新失败，旧键值(%s|%s) 不存在", 
						db_type::getname(),master_key, detail_key));
				}
				else
				{
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
			::InterlockedIncrement(&db_write_handler::instance().delete_);

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
			::InterlockedIncrement(&db_write_handler::instance().select_);

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
			::InterlockedIncrement(&db_write_handler::instance().select_);

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
			::InterlockedIncrement(&db_write_handler::instance().select_);

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
			::InterlockedIncrement(&db_write_handler::instance().update_);

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
		conn.reset();
	}
}

template <typename db_type>
void db_multi_index_handler<db_type>::save_to_file()
{
	char path[MAX_PATH];
	long path_len = ::GetModuleFileName(0, path, MAX_PATH);
	while (path[path_len--] != '\\');
	path_len += 2;
	path[path_len] = 0;

	tm * fat = ::gmtime(&__filetime);

	sprintf(path + path_len, "%04d%02d%02d%02d%02d%02d", fat->tm_year + 1900,
		fat->tm_mon + 1, fat->tm_mday, (fat->tm_hour + 8) % 24, fat->tm_min, fat->tm_sec);

	if (access(path, 0)) mkdir(path);
	strcat(path, "\\");
	strcat(path, db_type::getname());
	strcat(path, ".dat");

	HANDLE handle = file_create(path);
	if (handle == INVALID_HANDLE_VALUE)
	{
		LOG_ERROR(("表 %s，创建文件失败", db_type::getname()));
		return ;
	}

	int ext_length = sizeof(db_type);
	unsigned int len = record_count_ * ext_length + 5;
	char * buf = (char *)malloc(len + 1);
	int offset = 0;

	if (id_mgr_)
		buf[0] = 1;
	else
		buf[0] = 0;
	*(int *)(buf + 1) = only_id_;

	db_type * asdfdsa = (db_type *)(buf + 5);
	for (db_master_iterator iter = table_.begin (); 
		iter != table_.end (); iter++)
	{
		db_detail_iterator subiter;
		for (subiter = iter->second->begin (); 
			subiter != iter->second->end (); subiter++)
		{
			::memcpy(buf + 5 + (offset * ext_length), subiter->second, ext_length);
			++offset;
		}
	}

	file_write(handle, buf, len);
	file_close(handle);

	free(buf);
}

template <typename db_type>
bool db_multi_index_handler<db_type>::load_from_file(const char * path)
{
	LOG_ERROR_EX_RETURN(path, ("表 %s，从文件加载失败，路径不合法", 
		db_type::getname()), false);

	char filename[255];
	strcpy(filename, path);
	strcat(filename, "\\");
	strcat(filename, db_type::getname());
	strcat(filename, ".dat");

	HANDLE handle = file_open(filename);
	if (handle == INVALID_HANDLE_VALUE)
	{
		LOG_ERROR(("表 %s，从文件加载失败，打开文件失败", db_type::getname()));
		return false;
	}

	int len = file_length(handle);

	int ext_length = sizeof(db_type);
	if (len < 5)
	{
		file_close(handle);
		LOG_ERROR(("表 %s，从文件加载失败，文件不合法",
			db_type::getname()));
		return false;
	}

	char * buf = (char *)malloc(len + 1);
	buf[len]='\0';
	file_read(handle, buf, len);
	file_close(handle);

	if (!buf[0])
		id_mgr_ = false;
	else
		id_mgr_ = true;

	only_id_ = *(int *)(buf + 1);

	db_type * tstrct = (db_type *)(buf + 5);
	int count = (len - 5) / ext_length;

	char master_key[255];
	char detail_key[255];

	record_count_ = 0;
	db_detail * detail = 0;

	for (int i = 0; i < count; i++)
	{
		db_type * r = alloc_.malloc();
		memcpy(r, (db_type *)&tstrct[i], ext_length);

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
			LOG_ERROR(("表 %s，从文件初始化失败，出现重复记录，新键值(%s|%s)",
				db_type::getname(),master_key, detail_key));

			free(buf);
			return clear_map(), false;
		}
	}

	free(buf);
	return true;
}

template <typename db_type>
bool db_multi_index_handler<db_type>::load_from_db(const char * con_str)
{
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
		{
			os >> only_id_;
			id_mgr_ = true;
		}

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
					db_type::getname(),master_key, detail_key));

				db.logoff();
				return clear_map(), false;
			}
		}
	}
	catch(otl_exception&)
	{ 
		db.logoff();
		return clear_map(), false;
	}

	db.logoff();
	return true;
}

template <typename db_type>
bool db_multi_index_handler<db_type>::save_to_db_all()
{
	if (!record_count_) return false;

	otl_connect db;
	try 
	{ 
		db.rlogon(db_write_handler::instance().db_con_str_.c_str());
		char buf[128];
		sprintf(buf, "TRUNCATE table %s ", db_type::getname());
		otl_cursor::direct_exec(db, buf, otl_exception::disabled);
	}
	catch(otl_exception&)
	{ 
		db.logoff();
		return false;
	}

	db.logoff();

	for (db_master_iterator iter = table_.begin (); 
		iter != table_.end (); iter++)
	{
		db_detail_iterator subiter;
		for (subiter = iter->second->begin (); 
			subiter != iter->second->end (); subiter++)
		{
			char * ptr = new char[1024];
			*(int*)ptr = subiter->second->get_insert_str(ptr + 4);
			if (!db_write_handler::instance().push(ptr)) delete [] ptr;
		}
	}

	if (id_mgr_)
	{
		char * ptr = new char[256];
		*(int*)ptr = sprintf(ptr + 4, " UPDATE id_manager SET id_increase=%d WHERE tablename=\'\'%s\'\' ", 
			only_id_, db_type::getname());

		if (!db_write_handler::instance().push(ptr)) delete [] ptr;
	}

	return true;
}

#endif // __DB_MULTI_INDEX_HANDLER_HPP__

