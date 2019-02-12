#ifndef __um_write_thread_H__
#define __um_write_thread_H__

#include "um_public_client.h"


class um_write_thread: public IceUtil::Task<char *>
{
private:
	um_write_thread();
public:
	~um_write_thread();
	static um_write_thread& Instance();
	bool startTask(size_t n_threads = 1);
	void stopTask();
	void run();
private:
	bool m_bAlive;
};


#endif
