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

	// ��ʼ������Ҫ����������DBG
	// const char * DBG��ַ
	// short DBG�˿�
	// true��ɹ�������ʧ��
	bool init(const char *, short, int);
	bool init_proxy(const char *, short, const char *, short, int);

	// ����ʼ������Ҫ�����ǶϿ�DBG
	// true��ɹ�������ʧ��
	bool fini();

	// �����ݿ�ΪT�ı����������
	// T & Ҫ��ӵļ�¼���Լ�DBG������Ӽ�¼�Ľ��
	// true��ɹ���false��ʧ��
	template < class T > int db_insert(T &);

	// �����ݿ�ΪT�ı����޸�����
	// T & Ҫ�޸ĵļ�¼
	// true��ɹ���false��ʧ��
	template < class T > int db_update(const T &, T &);

	// �����ݿ�ΪT�ı��м�����
	// T & Ҫ�ӵļ�¼
	// true��ɹ���false��ʧ��
	template < class T > int db_increase(T &);

	// �����ݿ�ΪT�ı���ɾ������
	// T & Ҫɾ���ļ�¼
	// true��ɹ���false��ʧ��
	template < class T > int db_delete(const T &);

	// �����ݿ�ΪT�ı��в�ѯһ������
	// T & Ҫ��ѯ�ļ�¼���Լ�DBG������Ӽ�¼�Ľ��
	// true��ɹ���false��ʧ��
	template < class T > int db_select(T &);

	// �����ݿ�ΪT�ı��в�ѯ��������
	// T & Ҫ��ѯ�ı�
	// char *& ���صĽ����,ע����ڴ���ʹ�������ͷ�
	// int & ����char*&���ܳ���
	// true��ɹ���false��ʧ��
	template < class T > int db_select_all(const T &, char *&, int &);

	// �����ݿ�ΪT�ı��в�ѯ��������
	// T & Ҫ��ѯ�ı�
	// char *& ���صĽ����,ע����ڴ���ʹ�������ͷ�
	// int & ����char*&���ܳ���
	// true��ɹ���false��ʧ��
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
