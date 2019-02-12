#include <iostream>

#include "socketsvc.hpp"
#include "distributed_protocol.hpp"

using boost::asio::ip::tcp;

std::string login_address = "192.168.1.18";
u_short login_port = 8888;

int agentid;
std::string agent_address;
u_short agent_port;

int myid;

std::string mya = "201122";
int sid = 2900;


bool client_login()
{
	try
	{
		std::cout << "client_login......";

		boost::asio::io_service io_service;

		//struct in_addr iaddr;
		//hostent * st = gethostbyname(login_address.c_str());
		//logic_check(!st, ("gethostbyname(gamesavor.uicp.net) failed"));

		//memcpy(&iaddr, st->h_addr_list[0], 4);
		//u_long login_address = ::ntohl(iaddr.s_addr);

		boost::asio::ip::tcp::endpoint endpoint(boost::asio::
			ip::address::from_string(login_address.c_str()), login_port);

		tcp::socket s(io_service);
		s.connect(endpoint);

		CLIENT_REGISTER_LOGIN req;
		req.serialize(mya.c_str(), mya.c_str(), "test", 20090101, 1000);
		boost::asio::write(s, boost::asio::buffer((char*)&req, req.length));

		LOGIN_REGISTER_RESP_CLIENT resp;
		boost::asio::read(s, boost::asio::buffer((char*)&resp, sizeof(resp)));

		if (resp.code != errorcode::client_login_succeed)
		{
			std::cout << errorcode::error_message(resp.code) << std::endl;
			return false;
		}

		myid = resp.playerid;
		agent_address = resp.address;
		agentid = resp.agentid;
		agent_port = resp.port;
		return true;

	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}
	return false;
}

void agent_receiver(tcp::socket * s, bool * run)
{
	char buffer[2048];
	int length;

	while ((*run))
	{
		try
		{
			boost::asio::read((*s), boost::asio::buffer(buffer, 11));

			length = *(int *)(buffer + 7);
			buffer[length] = '\0';

			boost::asio::read((*s), boost::asio::buffer(buffer + 11, length - 11));

			DISTRIBUTED_HEADER * h = (DISTRIBUTED_HEADER *)buffer;

			if (h->command == CMD_AGENT_CLIENT_RESPONSE)
			{
				AGENT_CLIENT_RESPONSE * resp = (AGENT_CLIENT_RESPONSE *)buffer;
				std::cout << errorcode::error_message(resp->code) << std::endl;
				std::cout << "logic exception: \nrequest from " << resp->from << " to " << resp->to
					<< "\nagent_id " << resp->agent << "\nmessage:" << errorcode::error_message(resp->code) << std::endl;
			}
			else
			{
				std::cout 
					<< "\nfrom: " << h->from 
					<< " to: " << h->to 
					<< " agent: " << h->agent 
					<< " context: " << buffer + sizeof(*h) 
					<< std::endl;
			}
		}
		catch (std::exception& e)
		{
			std::cerr << "Exception: " << e.what() << "\n";
			break;
		}
	}
}

int main(int argc, char* argv[])
{
	if (!client_login()) return 0;

	char buffer[2048];
	boost::asio::io_service io_service;
	tcp::socket s(io_service);

	s.connect(boost::asio::ip::tcp::endpoint(boost::asio::ip::address::
		from_string(agent_address.c_str()), agent_port));

	bool run = true;
	boost::thread t(boost::bind(&agent_receiver, &s, &run));

//	for (int i = 0; i < 2000; i++)
//	{
		DISTRIBUTED_HEADER * h = (DISTRIBUTED_HEADER *)buffer;
		h->serialize(sizeof(*h) + strlen("hello"), agentid, myid, sid, 1);
		memcpy(buffer + sizeof(*h), "hello", strlen("hello"));
		boost::asio::write(s, boost::asio::buffer(buffer, h->length));
//#if defined(_WIN32)
//		Sleep(1000);
//#else
//		sleep(1);
//#endif
//	}

	t.join();

	s.close();
	run = false;
	std::cout << "Press ENTER key to exit\n";
	getchar();

	return 0;
}
