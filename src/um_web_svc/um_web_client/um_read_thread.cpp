#include "um_read_thread.h"

um_read_thread::um_read_thread()
{
}

um_read_thread::~um_read_thread()
{

}

IceUtil::Handle<um_read_thread>& um_read_thread::Instance()
{
	static IceUtil::Handle<um_read_thread> readthread;
	return readthread;
}

void um_read_thread::OnInstance()
{
	um_read_thread::Instance() = new um_read_thread;
}

void um_read_thread::DownInstance()
{
	um_read_thread::Instance() = 0;
}

void um_read_thread::run()
{
	char buf[defsize];
	int length;
	while (true)
	{
		if (um_public_client::Instance().Recv(buf,defsize) ==SOCKET_ERROR)
		{
			if (!um_public_client::Instance().GetActive()) break;
			um_public_client::Instance().LoginOut();
			
			if (um_public_client::Instance().getreconntime()>0)
			{
				Sleep(um_public_client::Instance().getreconntime());
				if (!um_public_client::Instance().GetActive()) break;
				um_public_client::Instance().relogin();
				continue;
			}
		}
		length =*(int*)(buf+offset);

		char * buf_(new char[length]);
		if (um_public_client::Instance().Recv(buf_+defsize,length-defsize) == SOCKET_ERROR)
		{
			delete [] buf_;
			if (!um_public_client::Instance().GetActive()) break;
			um_public_client::Instance().LoginOut();

			if (um_public_client::Instance().getreconntime()>0)
			{
				Sleep(um_public_client::Instance().getreconntime());
				if (!um_public_client::Instance().GetActive()) break;
				um_public_client::Instance().relogin();
				continue;
			}
		}
		memcpy(buf_,buf,defsize);
		um_public_client::Instance().UpdateData(((header*)buf_)->index,length,buf_,false); 
	}
}

