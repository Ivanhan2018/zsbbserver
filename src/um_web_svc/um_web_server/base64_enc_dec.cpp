#include <iostream>
#include <stdio.h>
#include "base64.h"

#define OTL_STL
#define OTL_ODBC

#if defined (WIN32)
#define OTL_BIGINT __int64
#else
#define OTL_BIGINT long long
#endif
#include "otlv4.h"

otl_connect db_;

struct account
{
	int accountid;
	char pass[16];
	char encpass[50];
};

void select_record()
{
	std::string sql = "select count(1) from gameuser";
	otl_stream o((int)sql.size(), sql.c_str(), db_);
	int count = 0;
	o >> count;

	if (!count) return ;

	account * ac = (account *)malloc(sizeof(account) * count);

	sql = "select accountid, password from gameuser";
	otl_stream os((int)sql.size(), sql.c_str(), db_);

	int aaaa = 0;
	while (!os.eof())
	{
		os >> ac[aaaa].accountid >> ac[aaaa].pass;
		++aaaa;
	}

	os.close();

	for (int i = 0; i < count; i++)
	{
		base64_encode(ac[i].pass, strlen(ac[i].pass), ac[i].encpass);
	}

	char buf[1024];
	for (int i = 0; i < count; i++)
	{
		sprintf(buf, "update gameuser set password = \'%s\' where accountid = %d", ac[i].encpass, ac[i].accountid);
		otl_cursor::direct_exec(db_, buf);
		db_.commit();
	}

	free((void *)ac);
}



int main()
{
	otl_connect::otl_initialize();

	try
	{
		db_.rlogon("UID=sa;PWD=sa;DSN=localserver");
		select_record();
		std::cout << "用户表密码数据加密完成\n";
	}
	catch(otl_exception& p)
	{ 
		std::cerr << p.msg << std::endl;
		std::cerr << p.stm_text << std::endl;
		std::cerr << p.sqlstate << std::endl;
		std::cerr << p.var_info << std::endl;
	}

	db_.logoff();
	getchar();
	return 0;
}
