#ifndef __um_public_client_H__
#define __um_public_client_H__

#include "IceUtil.h"
#include "um_web.h"
#include <map>
#include <crtdbg.h>

#include "log.hpp"

struct tagDataNode :public IceUtil::Shared
{
	char * sbuf;		//发送缓冲指针
	long event;			//事件	
	bool rlt;			//结果
	int rlen;			//接收缓冲长度
	char * rbuf;		//接收缓冲指针
	
	tagDataNode():sbuf(NULL),event(NULL),rlt(false),rlen(0),rbuf(NULL){}
	~tagDataNode()
	{
		if (sbuf) delete [] sbuf;
		if (rbuf) delete [] rbuf;
		if (event) CloseHandle((HANDLE)event);
	}
};

typedef IceUtil::Handle<tagDataNode> HDN;


class um_public_client
{
private:
	um_public_client();
public:
	~um_public_client();
	static um_public_client& Instance();

	bool login(const char * ip,short port,int timeout,int reconntime); 
	bool relogin();
	bool LoginOut();

	int Recv(char * buf,int len);
	int Send(const char * buf,int len);
	int getsocket() const;
	int gettimeout() const;
	int getreconntime() const;
	int getindex();

	bool InsertData(HDN& dn);
	bool FindData(int idx,HDN& dn);
	
	bool DeleteData(int idx);
	bool UpdateData(int idx,int len,char * rbuf,bool bfailed=false);

	void SetActive(bool bactive);
	bool GetActive() const;

private: 
	IceUtil::Mutex idx_mux_;
	IceUtil::Mutex data_mux_;
	int socket_;
	char ip_[16];
	short port_; 
	int timeout_;
	int reconntime_;
	int index_;
	bool bactive_;
	std::map<int,HDN> mdata_;
};



#endif