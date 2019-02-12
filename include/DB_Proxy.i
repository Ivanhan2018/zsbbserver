template < class T > int 
DB_Proxy::db_insert(T & t)
{
#ifndef NDEBUG
	test("db_insert", __LINE__);
#endif

	if (!fd_)
	{
#ifdef _DEBUG
//		printf("DB_Proxy::db_insert 套接字无效\n");
#endif
		return DB_FAILED;
	}

	static int tlen = sizeof(T);
	static int hlen = sizeof(DISTRIBUTED_HEADER);
	DISTRIBUTED_HEADER * h = (DISTRIBUTED_HEADER *)buffer_;
	memcpy(buffer_ + hlen, &t, tlen);
	h->serialize(hlen + tlen, 0, city_id_, t.gettype(), CMD_ACTION_APPEND);

	if (send_n(buffer_, h->length) != h->length)
	{
#ifdef _DEBUG
//		printf("DB_Proxy::db_insert 发送失败\n");
#endif
		return DB_FAILED;
	}
	
	if (recv_n(buffer_, hlen) != hlen)
	{
#ifdef _DEBUG
//		printf("DB_Proxy::db_insert 接收协议头失败\n");
#endif
		return DB_FAILED;
	}

	if ((h->length - hlen) == 0)
	{
#ifdef _DEBUG
//		printf("DB_Proxy::db_insert 数据已存在\n");
#endif
		return DB_EOF;
	}

	if (recv_n((char *)&t, tlen) != tlen)
	{
#ifdef _DEBUG
//		printf("DB_Proxy::db_insert 接收数据失败\n");
#endif
		return DB_FAILED;
	}

	return DB_SUCCEED;
}

template < class T > int
DB_Proxy::db_update(const T & src, T & desc)
{
#ifndef NDEBUG
	test("db_update", __LINE__);
#endif

	if (!fd_)
	{
#ifdef _DEBUG
//		printf("DB_Proxy::db_update 套接字无效\n");
#endif
		return DB_FAILED;
	}
	
	if (src.gettype() != desc.gettype())
	{
#ifdef _DEBUG
//		printf("DB_Proxy::db_update 两种数据结构不一致\n");
#endif
		return DB_FAILED;
	}
		

	static int tlen = sizeof(T);
	static int hlen = sizeof(DISTRIBUTED_HEADER);
	DISTRIBUTED_HEADER * h = (DISTRIBUTED_HEADER *)buffer_;
	memcpy(buffer_ + hlen, &src, tlen);
	memcpy(buffer_ + hlen + tlen, &desc, tlen);
	h->serialize(hlen + (tlen * 2), 0, city_id_, src.gettype(), CMD_ACTION_UPDATE);

	if (send_n(buffer_, h->length) != h->length)
	{
#ifdef _DEBUG
//		printf("DB_Proxy::db_update 发送失败\n");
#endif
		return DB_FAILED;
	}

	if (recv_n(buffer_, hlen) != hlen)
	{
		return DB_FAILED;
	}

	if ((h->length - hlen) == 0)
	{
		return DB_EOF;
	}

	if (recv_n((char *)&desc, tlen) != tlen)
	{
		return DB_FAILED;
	}

	return DB_SUCCEED;
}

template < class T > int 
DB_Proxy::db_increase(T & t)
{
#ifndef NDEBUG
	test("db_increase", __LINE__);
#endif

	if (!fd_)
	{
#ifdef _DEBUG
//		printf("DB_Proxy::db_increase 套接字无效\n");
#endif
		return DB_FAILED;
	}

	static int tlen = sizeof(T);
	static int hlen = sizeof(DISTRIBUTED_HEADER);
	DISTRIBUTED_HEADER * h = (DISTRIBUTED_HEADER *)buffer_;
	memcpy(buffer_ + hlen, &t, tlen);
	h->serialize(hlen + tlen, 0, city_id_, t.gettype(), CMD_ACTION_INCREASE);

	if (send_n(buffer_, h->length) != h->length)
	{
#ifdef _DEBUG
//		printf("DB_Proxy::db_increase 发送失败\n");
#endif
		return DB_FAILED;
	}
	
	if (recv_n(buffer_, hlen) != hlen)
	{
#ifdef _DEBUG
//		printf("DB_Proxy::db_increase 接收协议头失败\n");
#endif
		return DB_FAILED;
	}

	if ((h->length - hlen) == 0)
	{
#ifdef _DEBUG
//		printf("DB_Proxy::db_increase 数据不存在\n");
#endif
		return DB_EOF;
	}

	if (recv_n((char *)&t, tlen) != tlen)
	{
#ifdef _DEBUG
//		printf("DB_Proxy::db_increase 接收数据失败\n");
#endif
		return DB_FAILED;
	}

	return DB_SUCCEED;
}

template < class T > int 
DB_Proxy::db_delete(const T & t)
{
#ifndef NDEBUG
	test("db_delete", __LINE__);
#endif

	if (!fd_)
	{
#ifdef _DEBUG
//		printf("DB_Proxy::db_delete 套接字无效\n");
#endif
		return DB_FAILED;
	}

	static int tlen = sizeof(T);
	static int hlen = sizeof(DISTRIBUTED_HEADER);
	DISTRIBUTED_HEADER * h = (DISTRIBUTED_HEADER *)buffer_;
	memcpy(buffer_ + hlen, &t, tlen);
	h->serialize(hlen + tlen, 0, city_id_, t.gettype(), CMD_ACTION_DELETE);

	if (send_n(buffer_, h->length) != h->length)
	{
#ifdef _DEBUG
//		printf("DB_Proxy::db_delete 发送失败\n");
#endif
		return DB_FAILED;
	}

	if (recv_n(buffer_, hlen) != hlen)
	{
		return DB_FAILED;
	}

	if ((h->length - hlen) == 0)
	{
		return DB_EOF;
	}

	if (recv_n((char *)&t, tlen) != tlen)
	{
		return DB_FAILED;
	}

	return DB_SUCCEED;
}

template < class T > int 
DB_Proxy::db_select(T & t)
{
#ifndef NDEBUG
	test("db_select", __LINE__);
#endif

	if (!fd_)
	{
#ifdef _DEBUG
//		printf("DB_Proxy::db_select 套接字无效\n");
#endif
		return DB_FAILED;
	}

	static int tlen = sizeof(T);
	static int hlen = sizeof(DISTRIBUTED_HEADER);
	DISTRIBUTED_HEADER * h = (DISTRIBUTED_HEADER *)buffer_;
	memcpy(buffer_ + hlen, &t, tlen);
	h->serialize(hlen + tlen, 0, city_id_, t.gettype(), CMD_ACTION_SELECT);

	if (send_n(buffer_, h->length) != h->length)
	{
#ifdef _DEBUG
//		printf("DB_Proxy::db_select 发送失败\n");
#endif
		return DB_FAILED;
	}

	if (recv_n(buffer_, hlen) != hlen)
	{
#ifdef _DEBUG
//		printf("DB_Proxy::db_select 接收协议头失败\n");
#endif
		return DB_FAILED;
	}

	if ((h->length - hlen) == 0)
	{
#ifdef _DEBUG
//		printf("DB_Proxy::db_select 数据库没有数据\n");
#endif
		return DB_EOF;
	}

	if (recv_n((char *)&t, tlen) != tlen)
	{
#ifdef _DEBUG
//		printf("DB_Proxy::db_select 接收数据失败\n");
#endif
		return DB_FAILED;
	}

	return DB_SUCCEED;
}

template < class T > int 
DB_Proxy::db_select_all(const T & t, char *& b, int & l)
{
#ifndef NDEBUG
	test("db_select_all", __LINE__);
#endif

	if (!fd_)
	{
#ifdef _DEBUG
//		printf("DB_Proxy::db_select_all 套接字无效\n");
#endif
		return DB_FAILED;
	}

	static int tlen = sizeof(T);
	static int hlen = sizeof(DISTRIBUTED_HEADER);
	DISTRIBUTED_HEADER * h = (DISTRIBUTED_HEADER *)buffer_;
	memcpy(buffer_ + hlen, &t, tlen);
	h->serialize(hlen + tlen, 0, city_id_, t.gettype(), CMD_ACTION_SELECT_ALL);

	if (send_n(buffer_, h->length) != h->length)
	{
#ifdef _DEBUG
//		printf("DB_Proxy::db_select_all 发送失败\n");
#endif
		return DB_FAILED;
	}

	if (recv_n(buffer_, hlen) != hlen)
	{
#ifdef _DEBUG
//		printf("DB_Proxy::db_select_all 接收协议头失败\n");
#endif
		return DB_FAILED;
	}

	l = h->length - hlen;
	if (l == 0)
	{
#ifdef _DEBUG
//		printf("DB_Proxy::db_select_all 数据库没有数据\n");
#endif
		return DB_EOF;
	}

	b = new char[l];
	if (recv_n(b, l) != l)
	{
		delete [] b;
#ifdef _DEBUG
//		printf("DB_Proxy::db_select_all 接收数据失败\n");
#endif
		return DB_FAILED;
	}

	return DB_SUCCEED;
}

template < class T > int 
DB_Proxy::db_multi_select(const T & t, char *& b, int & l)
{
#ifndef NDEBUG
	test("db_multi_select", __LINE__);
#endif

	if (!fd_)
	{
#ifdef _DEBUG
//		printf("DB_Proxy::db_multi_select 套接字无效\n");
#endif
		return DB_FAILED;
	}

	static int tlen = sizeof(T);
	static int hlen = sizeof(DISTRIBUTED_HEADER);
	DISTRIBUTED_HEADER * h = (DISTRIBUTED_HEADER *)buffer_;
	memcpy(buffer_ + hlen, &t, tlen);
	h->serialize(hlen + tlen, 0, city_id_, t.gettype(), CMD_ACTION_MULTI_SELECT);

	if (send_n(buffer_, h->length) != h->length)
	{
#ifdef _DEBUG
//		printf("DB_Proxy::db_multi_select 发送失败\n");
#endif
		return DB_FAILED;
	}

	if (recv_n(buffer_, hlen) != hlen)
	{
#ifdef _DEBUG
//		printf("DB_Proxy::db_multi_select 接收协议头失败\n");
#endif
		return DB_FAILED;
	}

	l = h->length - hlen;
	if (l == 0)
	{
#ifdef _DEBUG
//		printf("DB_Proxy::db_multi_select 数据库没有数据\n");
#endif
		return DB_EOF;
	}

	b = new char[l];
	if (recv_n(b, l) != l)
	{
		delete [] b;
#ifdef _DEBUG
//		printf("DB_Proxy::db_multi_select 接收数据失败\n");
#endif
		return DB_FAILED;
	}

	return DB_SUCCEED;
}

