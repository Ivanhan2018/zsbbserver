#include "um_write_thread.h"

um_write_thread::um_write_thread()
:m_bAlive(false)
{
}

um_write_thread::~um_write_thread()
{
}

um_write_thread& um_write_thread::Instance()
{
	static um_write_thread writethread;
	return writethread;
}

bool um_write_thread::startTask(size_t n_threads)
{
	if (m_bAlive)  return false;
	m_bAlive=IceUtil::Task<char *>::startTask(n_threads);
	return m_bAlive;
}

void um_write_thread::stopTask()
{
	m_bAlive=false;
	IceUtil::Task<char *>::stopTask();
}

void um_write_thread::run()
{
	char * dn;
	while (m_bAlive)
	{
		if ( getQueue(dn) )
		{
			if (um_public_client::Instance().Send((char*)dn,((header*)dn)->len)==SOCKET_ERROR)
			{
				um_public_client::Instance().UpdateData(((header*)dn)->index,0,0,true); 
			}
		}
	}
}
