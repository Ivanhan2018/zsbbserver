#ifndef __INCLUDE_DB_PROXY_H__
#define __INCLUDE_DB_PROXY_H__

#include "Distributed_Header.h"

class DB_Proxy
{
	enum { MAX_BUFFER_SIZE = 2048};
public:
	enum { DB_FAILED, DB_SUCCEED, DB_EOF };
public:
	DB_Proxy ();

	// 初始化，主要功能是连接DBG
	// const char * DBG地址
	// short DBG端口
	// true则成功，否则失败
	bool init(const char *, short, int);
	bool init_proxy(const char *, short, const char *, short, int);

	// 反初始化，主要功能是断开DBG
	// true则成功，否则失败
	bool fini();

	// 在数据库为T的表中添加数据
	// T & 要添加的记录，以及DBG返回添加记录的结果
	// true则成功，false则失败
	template < class T > int db_insert(T &);

	// 在数据库为T的表中修改数据
	// T & 要修改的记录
	// true则成功，false则失败
	template < class T > int db_update(const T &, T &);

	// 在数据库为T的表中加数据
	// T & 要加的记录
	// true则成功，false则失败
	template < class T > int db_increase(T &);

	// 在数据库为T的表中删除数据
	// T & 要删除的记录
	// true则成功，false则失败
	template < class T > int db_delete(const T &);

	// 在数据库为T的表中查询一条数据
	// T & 要查询的记录，以及DBG返回添加记录的结果
	// true则成功，false则失败
	template < class T > int db_select(T &);

	// 在数据库为T的表中查询所有数据
	// T & 要查询的表
	// char *& 返回的结果集,注意该内存需使用者来释放
	// int & 返回char*&的总长度
	// true则成功，false则失败
	template < class T > int db_select_all(const T &, char *&, int &);

	// 在数据库为T的表中查询多条数据
	// T & 要查询的表
	// char *& 返回的结果集,注意该内存需使用者来释放
	// int & 返回char*&的总长度
	// true则成功，false则失败
	template < class T > int db_multi_select(const T &, char *&, int &);

protected:
	DB_Proxy (const DB_Proxy &);            
	DB_Proxy & operator = (const DB_Proxy &);

	int send_n(const char *, int);
	int recv_n(char *, int);
	int recv(char *, int);

	int fd_;
	char buffer_[MAX_BUFFER_SIZE];
	int city_id_;

#ifndef NDEBUG

	void test(const char *, int);

	long tid_;
#endif
};

#include "DB_Proxy.i"

#endif // __INCLUDE_DB_PROXY_H__
