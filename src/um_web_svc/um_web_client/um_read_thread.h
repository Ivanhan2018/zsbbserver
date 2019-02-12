#ifndef __um_read_thread_H__
#define __um_read_thread_H__


#include "um_public_client.h"


class um_read_thread : public IceUtil::Thread
{
private:
	um_read_thread();
public:
	~um_read_thread();
	static IceUtil::Handle<um_read_thread>& Instance();
	static void OnInstance();
	static void DownInstance();
	void run();
//private:
//	bool run_;
};

#endif 