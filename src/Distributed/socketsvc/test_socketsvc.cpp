#include <iostream>

#include "header.hpp"
#include "socketsvc.hpp"
#include "Distributed_Header.h"

std::string address = "192.168.1.243";
u_short port = 512;
int myid = 2113;
int agentid = 4000;

int SOCKETSVC_API  event_notify_callback(int server_id, const char * buffer, int length)
{
	std::cout << "event_notify_callback server id:" << server_id <<std::endl;
	return 0;
}

int SOCKETSVC_API  data_receiver_callback(int agent_id, int player_id, char * buffer, int length)
{
	std::cout << "data_receiver_callback agent_id id:" << agent_id << " player id:" << player_id <<std::endl;
	return 0;
}

int SOCKETSVC_API  timer_notify_callback(const void * param_1, const void * param_2, long timer_id)
{
	long sss = GetTickCount() - (long)param_1;
	if (sss > 1000)
		printf("timer : %d\n", sss);

	return 0;
}

void SOCKETSVC_API  socket_disconn_callback(bool player_all, int agent_id, int server_id, int player_id)
{
	if (player_all)
	{
		std::cout << "agent disconnect :" << agent_id << "  myid: " << server_id <<std::endl;
	}
	else
	{
		std::cout << "client disconnect :" << player_id << "  myid: " << server_id <<std::endl;
	}
}

void SOCKETSVC_API  logic_exception_callback(int code, int agent_id, int from_id, int to_id)
{
	std::cout << "logic exception: \nrequest from " << from_id << " to " << to_id 
		<< "\nagent_id " << agent_id << "\nmessage:" << get_error_message(code) << std::endl;
}

//////////////////////////////////////////////////////////////////////////////////////////
void test_init()
{
	std::cout << "test_init......";

	if (init(address.c_str(), port, address.c_str(), 3000, myid, 5, data_receiver_callback, event_notify_callback, 
		timer_notify_callback, socket_disconn_callback, logic_exception_callback))
		std::cout << "SUCCEED\n";
	else
		std::cout << "FAILED\n";
}

void test_fini()
{
	std::cout << "test_fini......SUCCEED\n";
	fini();
}
//////////////////////////////////////////////////////////////////////////////////////////

void test_notify_error_to_id_from_id_equal()
{
	std::cout << "test_notify_error_to_id_from_id_equal......";

	if (event_notify(myid, "1234567890", strlen("1234567890")))
		std::cout << "SUCCEED\n";
	else
		std::cout << "FAILED\n";
}

void test_notify_error_to_id_nonexistent()
{
	std::cout << "test_notify_error_to_id_nonexistent......";

	if (event_notify(1000, "1234567890", strlen("1234567890")))
		std::cout << "SUCCEED\n";
	else
		std::cout << "FAILED\n";
}

void test_notify_error_to_id_overflow()
{
	std::cout << "test_notify_error_to_id_overflow......";

	if (event_notify(9999, "1234567890", strlen("1234567890")))
		std::cout << "SUCCEED\n";
	else
		std::cout << "FAILED\n";
}

void test_server_request_error_from_id()
{
	std::cout << "test_server_request_error_from_id......";

	char * tmp = new char[sizeof(DISTRIBUTED_HEADER)];
	DISTRIBUTED_HEADER * req = (DISTRIBUTED_HEADER *)tmp;
	req->serialize(sizeof(*req), agentid, 456, 456, 456);

	if (asynch_write(agentid, tmp, req->length))
		std::cout << "SUCCEED\n";
	else
		std::cout << "FAILED\n";
}

void test_server_request_error_to_id_less_100000()
{
	std::cout << "test_server_request_error_to_id_less_100000......";

	char * tmp = new char[sizeof(DISTRIBUTED_HEADER)];
	DISTRIBUTED_HEADER * req = (DISTRIBUTED_HEADER *)tmp;
	req->serialize(sizeof(*req), agentid, myid, 456, 456);

	if (asynch_write(agentid, tmp, req->length))
		std::cout << "SUCCEED\n";
	else
		std::cout << "FAILED\n";
}

void test_server_request_error_to_id_nonexistent()
{
	std::cout << "test_server_request_error_to_id_nonexistent......";

	char * tmp = new char[sizeof(DISTRIBUTED_HEADER)];
	DISTRIBUTED_HEADER * req = (DISTRIBUTED_HEADER *)tmp;
	req->serialize(sizeof(*req), agentid, myid, 100000, 456);

	if (asynch_write(agentid, tmp, req->length))
		std::cout << "SUCCEED\n";
	else
		std::cout << "FAILED\n";
}

void test_server_request_error_command_null()
{
	std::cout << "test_server_request_error_command_null......";

	char * tmp = new char[sizeof(DISTRIBUTED_HEADER)];
	DISTRIBUTED_HEADER * req = (DISTRIBUTED_HEADER *)tmp;
	req->serialize(sizeof(*req), agentid, myid, 100000, 0);

	if (asynch_write(agentid, tmp, req->length))
		std::cout << "SUCCEED\n";
	else
		std::cout << "FAILED\n";
}

//////////////////////////////////////////////////////////////////////////////////////////

void test_log(bool * run)
{
	while (*run)
	{
		for (int i = 0; i < 100; i++)
		{
			log_game_report("test_log\n");
		}
		Sleep(1);
	}

}

int main(int argc, char* argv[])
{
	test_init();

	bool run = true;

	std::vector<boost::shared_ptr<boost::thread> > threads;

	for (std::size_t i = 0; i < 1; ++i)
	{
		boost::shared_ptr<boost::thread> thread(new boost::thread(
			boost::bind(&test_log, &run)));
		threads.push_back(thread);
	}

	getchar();
	run = false;

	for (std::size_t i = 0; i < threads.size(); ++i)
		threads[i]->join();

//#if defined(_WIN32)
//	Sleep(1000);
//#else
//	sleep(1);
//#endif
//	
//	test_notify_error_to_id_from_id_equal();
//#if defined(_WIN32)
//	Sleep(1000);
//#else
//	sleep(1);
//#endif
//
//	test_notify_error_to_id_nonexistent();
//#if defined(_WIN32)
//	Sleep(1000);
//#else
//	sleep(1);
//#endif
//
//	test_notify_error_to_id_overflow();
//#if defined(_WIN32)
//	Sleep(1000);
//#else
//	sleep(1);
//#endif
//
//	test_server_request_error_from_id();
//#if defined(_WIN32)
//	Sleep(1000);
//#else
//	sleep(1);
//#endif
//
//	test_server_request_error_to_id_less_100000();
//#if defined(_WIN32)
//	Sleep(1000);
//#else
//	sleep(1);
//#endif
//
//	test_server_request_error_to_id_nonexistent();
//#if defined(_WIN32)
//	Sleep(1000);
//#else
//	sleep(1);
//#endif
//
//	test_server_request_error_command_null();
//#if defined(_WIN32)
//	Sleep(1000);
//#else
//	sleep(1);
//#endif

	getchar();

	test_fini();
	std::cout << "Press ENTER key to exit\n";
	getchar();

	return 0;
}
