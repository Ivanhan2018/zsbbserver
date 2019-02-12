#if defined(_WIN32)
#include <windows.h>
#endif

#include "socketsvc.hpp"
#include "Distributed_Header.h"

#include <iostream>
#include <string>

std::string address = "192.168.1.18";
u_short port = 512;
int myid = 2900;
int agentid = 4000;

int SOCKETSVC_API  event_notify_callback(int server_id, const char * buffer, int length)
{
//	std::cout << "event notify from server id:" << server_id <<std::endl;
	return 0;
}

int SOCKETSVC_API  data_receiver_callback(int agent_id, int player_id, char * buffer, int length)
{

//	std::cout << "data_receiver_callback agent_id id:" << agent_id << " player id:" << player_id <<std::endl;

	//DISTRIBUTED_HEADER * req = (DISTRIBUTED_HEADER *)buffer;
	//int to = req->to;
	//req->to = req->from;
	//req->from = to;

	//asynch_write(agent_id, buffer, length);
	return 0;
}

int SOCKETSVC_API  timer_notify_callback(const void * param_1, const void * param_2, long timer_id)
{
//	std::cout << "timer_notify_callback" <<std::endl;
	return 0;
}

void SOCKETSVC_API  socket_disconn_callback(bool player_all, int agent_id, int server_id, int player_id)
{
	//if (player_all)
	//{
	//	std::cout << "agent disconnect :" << agent_id << "  myid: " << server_id <<std::endl;
	//}
	//else
	//{
	//	std::cout << "client disconnect :" << player_id << "  myid: " << server_id <<std::endl;
	//}
}

void SOCKETSVC_API  logic_exception_callback(int code, int agent_id, int from_id, int to_id)
{
	std::cout << "logic exception: \nrequest from " << from_id << " to " << to_id 
		<< "\nagent_id " << agent_id << "\nmessage:" << get_error_message(code) << std::endl;
}

int main(int argc, char* argv[])
{
	try
	{
		if (!init("192.168.1.244", 512, "192.168.1.35", 3000, 1895, 1, data_receiver_callback, 
			event_notify_callback, timer_notify_callback, 
			socket_disconn_callback, logic_exception_callback))
		{
			abort();
		}

		std::cout << "Press ENTER key to test broadcast\n";
		getchar();

		char buf[4096];
		DISTRIBUTED_HEADER * h = (DISTRIBUTED_HEADER*)buf;
		h->serialize(4096, 4000, 1895, 0, 1);

		while (1)
		{
			broadcast(buf, h->length);
			Sleep(1000);
		}
	}
	catch (std::exception& e)
	{
		std::cout << "Exception: " << e.what() << "\n";
	}

	fini();

	return 0;
}
