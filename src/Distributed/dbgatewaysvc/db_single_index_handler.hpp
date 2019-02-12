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

	db_table table_;
	int only_id_;
	bool id_mgr_;
	db_allocator<db_type> alloc_;
};


template <typename db_type>
db_single_index_handler<db_type>::db_single_index_handler()
{
	id_mgr_ = false;
	only_id_ = 0;
}

template <typename db_type>
bool db_single_index_handler<db_type>::start(const char * con_str, bool fromfile)
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
bool db_single_index_handler<db_type>::stop()
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

		ext->getid(key);

		if (h->command == CMD_ACTION_APPEND)
		{ 
			::InterlockedIncrement(&db_write_handler::instance().insert_);

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
		
				LOG_ERROR(("表 %s，添加失败，关健字 %s 已存在", 
					db_type::getname(),key));
			}
		}
		else if (h->command == CMD_ACTION_UPDATE)
		{
			::InterlockedIncrement(&db_write_handler::instance().update_);

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
			::InterlockedIncrement(&db_write_handler::instance().delete_);

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
			::InterlockedIncrement(&db_write_handler::instance().select_);

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
			::InterlockedIncrement(&db_write_handler::instance().select_);

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
			::InterlockedIncrement(&db_write_handler::instance().update_);

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
		conn.reset();
	}
}

template <typename db_type>
void db_single_index_handler<db_type>::save_to_file()
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
	unsigned int len = table_.size() * ext_length + 5;
	char * buf = (char *)malloc(len + 1);
	int offset = 0;

	if (id_mgr_)
		buf[0] = 1;
	else
		buf[0] = 0;
	*(int *)(buf + 1) = only_id_;

	for (db_iterator iter = table_.begin (); 
		iter != table_.end (); iter++)
	{
		::memcpy(buf + 5 + (offset * ext_length), iter->second, ext_length);
		++offset;
	}

	file_write(handle, buf, len);
	file_close(handle);

	free(buf);
}

template <typename db_type>
bool db_single_index_handler<db_type>::load_from_file(const char * path)
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

	char key[255];

	for (int i = 0; i < count; i++)
	{
		db_type * r = alloc_.malloc();
		memcpy(r, (db_type *)&tstrct[i], ext_length);

		r->getid(key);

		db_iterator iter = table_.find(key);
		if (iter == table_.end())
		{
			table_.insert(std::pair<std::string, db_type *>(key, r));
		}
		else 
		{
			alloc_.free(r);
			LOG_ERROR(("表 %s，从文件初始化失败，出现重复记录，关健字 %s",
				db_type::getname(),key));

			free(buf);
			return clear_map(), false;
		}
	}

	free(buf);
	return true;
}

template <typename db_type>
bool db_single_index_handler<db_type>::load_from_db(const char * con_str)
{
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
	catch(otl_exception&)
	{ 
		db.logoff();
		return clear_map(), false;
	}

	db.logoff();
	return true;
}

template <typename db_type>
bool db_single_index_handler<db_type>::save_to_db_all()
{
	if (!table_.size()) return false;

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

	for (db_iterator iter = table_.begin(); 
		iter != table_.end (); iter++)
	{
		char * ptr = new char[1024];
		*(int*)ptr = iter->second->get_insert_str(ptr + 4);
		if (!db_write_handler::instance().push(ptr)) delete [] ptr;
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

#endif // __DB_SINGLE_INDEX_HANDLER_HPP__
