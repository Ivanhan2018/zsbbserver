template < class T > int 
DB_Proxy::db_insert(T & t)
{
#ifndef NDEBUG
	test("db_insert", __LINE__);
#endif

	if (!fd_)
	{
#ifdef _DEBUG
//		printf("DB_Proxy::db_insert �׽�����Ч\n");
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
//		printf("DB_Proxy::db_insert ����ʧ��\n");
#endif
		return DB_FAILED;
	}
	
	if (recv_n(buffer_, hlen) != hlen)
	{
#ifdef _DEBUG
//		printf("DB_Proxy::db_insert ����Э��ͷʧ��\n");
#endif
		return DB_FAILED;
	}

	if ((h->length - hlen) == 0)
	{
#ifdef _DEBUG
//		printf("DB_Proxy::db_insert �����Ѵ���\n");
#endif
		return DB_EOF;
	}

	if (recv_n((char *)&t, tlen) != tlen)
	{
#ifdef _DEBUG
//		printf("DB_Proxy::db_insert ��������ʧ��\n");
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
//		printf("DB_Proxy::db_update �׽�����Ч\n");
#endif
		return DB_FAILED;
	}
	
	if (src.gettype() != desc.gettype())
	{
#ifdef _DEBUG
//		printf("DB_Proxy::db_update �������ݽṹ��һ��\n");
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
//		printf("DB_Proxy::db_update ����ʧ��\n");
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
//		printf("DB_Proxy::db_increase �׽�����Ч\n");
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
//		printf("DB_Proxy::db_increase ����ʧ��\n");
#endif
		return DB_FAILED;
	}
	
	if (recv_n(buffer_, hlen) != hlen)
	{
#ifdef _DEBUG
//		printf("DB_Proxy::db_increase ����Э��ͷʧ��\n");
#endif
		return DB_FAILED;
	}

	if ((h->length - hlen) == 0)
	{
#ifdef _DEBUG
//		printf("DB_Proxy::db_increase ���ݲ�����\n");
#endif
		return DB_EOF;
	}

	if (recv_n((char *)&t, tlen) != tlen)
	{
#ifdef _DEBUG
//		printf("DB_Proxy::db_increase ��������ʧ��\n");
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
//		printf("DB_Proxy::db_delete �׽�����Ч\n");
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
//		printf("DB_Proxy::db_delete ����ʧ��\n");
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
//		printf("DB_Proxy::db_select �׽�����Ч\n");
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
//		printf("DB_Proxy::db_select ����ʧ��\n");
#endif
		return DB_FAILED;
	}

	if (recv_n(buffer_, hlen) != hlen)
	{
#ifdef _DEBUG
//		printf("DB_Proxy::db_select ����Э��ͷʧ��\n");
#endif
		return DB_FAILED;
	}

	if ((h->length - hlen) == 0)
	{
#ifdef _DEBUG
//		printf("DB_Proxy::db_select ���ݿ�û������\n");
#endif
		return DB_EOF;
	}

	if (recv_n((char *)&t, tlen) != tlen)
	{
#ifdef _DEBUG
//		printf("DB_Proxy::db_select ��������ʧ��\n");
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
//		printf("DB_Proxy::db_select_all �׽�����Ч\n");
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
//		printf("DB_Proxy::db_select_all ����ʧ��\n");
#endif
		return DB_FAILED;
	}

	if (recv_n(buffer_, hlen) != hlen)
	{
#ifdef _DEBUG
//		printf("DB_Proxy::db_select_all ����Э��ͷʧ��\n");
#endif
		return DB_FAILED;
	}

	l = h->length - hlen;
	if (l == 0)
	{
#ifdef _DEBUG
//		printf("DB_Proxy::db_select_all ���ݿ�û������\n");
#endif
		return DB_EOF;
	}

	b = new char[l];
	if (recv_n(b, l) != l)
	{
		delete [] b;
#ifdef _DEBUG
//		printf("DB_Proxy::db_select_all ��������ʧ��\n");
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
//		printf("DB_Proxy::db_multi_select �׽�����Ч\n");
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
//		printf("DB_Proxy::db_multi_select ����ʧ��\n");
#endif
		return DB_FAILED;
	}

	if (recv_n(buffer_, hlen) != hlen)
	{
#ifdef _DEBUG
//		printf("DB_Proxy::db_multi_select ����Э��ͷʧ��\n");
#endif
		return DB_FAILED;
	}

	l = h->length - hlen;
	if (l == 0)
	{
#ifdef _DEBUG
//		printf("DB_Proxy::db_multi_select ���ݿ�û������\n");
#endif
		return DB_EOF;
	}

	b = new char[l];
	if (recv_n(b, l) != l)
	{
		delete [] b;
#ifdef _DEBUG
//		printf("DB_Proxy::db_multi_select ��������ʧ��\n");
#endif
		return DB_FAILED;
	}

	return DB_SUCCEED;
}

