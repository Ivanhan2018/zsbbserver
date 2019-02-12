#include "distributed_protocol.hpp"

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

std::string address = "192.168.1.236";
u_short port = 80;


void test_client_request_error_from_id_less_100000()
{
	try
	{
		std::cout << "test_client_request_error_from_id_less_100000......";

		boost::asio::io_service io_service;
		boost::asio::ip::tcp::endpoint endpoint(boost::asio::
			ip::address::from_string(address.c_str()), port);
		tcp::socket s(io_service);
		s.connect(endpoint);

		DISTRIBUTED_HEADER req;
		req.serialize(sizeof(req), 456, 456, 2000, 456);
		boost::asio::write(s, boost::asio::buffer((char*)&req, req.length));

		AGENT_CLIENT_RESPONSE resp;
		boost::asio::read(s, boost::asio::buffer((char*)&resp, sizeof(resp)));

		if (resp.code == errorcode::client_request_error_from_id_less_100000)
			std::cout << "SUCCEED\n";
		else
			std::cout << "FAILED\n";
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}
}

void test_client_request_error_command_null()
{
	try
	{
		std::cout << "test_client_request_error_command_null......";

		boost::asio::io_service io_service;
		boost::asio::ip::tcp::endpoint endpoint(boost::asio::
			ip::address::from_string(address.c_str()), port);
		tcp::socket s(io_service);
		s.connect(endpoint);

		DISTRIBUTED_HEADER req;
		req.serialize(sizeof(req), 456, 100000, 2000, 0);
		boost::asio::write(s, boost::asio::buffer((char*)&req, req.length));

		AGENT_CLIENT_RESPONSE resp;
		boost::asio::read(s, boost::asio::buffer((char*)&resp, sizeof(resp)));

		if (resp.code == errorcode::client_request_error_command_null)
			std::cout << "SUCCEED\n";
		else
			std::cout << "FAILED\n";
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}
}

void test_client_request_error_to_id_nonexistent()
{
	try
	{
		std::cout << "test_client_request_error_to_id_nonexistent......";

		boost::asio::io_service io_service;
		boost::asio::ip::tcp::endpoint endpoint(boost::asio::
			ip::address::from_string(address.c_str()), port);
		tcp::socket s(io_service);
		s.connect(endpoint);

		DISTRIBUTED_HEADER req;
		req.serialize(sizeof(req), 456, 100000, 120, 1);
		boost::asio::write(s, boost::asio::buffer((char*)&req, req.length));

		AGENT_CLIENT_RESPONSE resp;
		boost::asio::read(s, boost::asio::buffer((char*)&resp, sizeof(resp)));

		if (resp.code == errorcode::client_request_error_to_id_nonexistent)
			std::cout << "SUCCEED\n";
		else
			std::cout << "FAILED\n";
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}
}

void test_server()
{
	try
	{
		std::cout << "test_server_handshake......";

		boost::asio::io_service io_service;
		boost::asio::ip::tcp::endpoint endpoint(boost::asio::
			ip::address::from_string(address.c_str()), port);
		tcp::socket s(io_service);
		s.connect(endpoint);

		SERVER_HANDSHAKE_SERVER req;
		req.serialize(2222);
		boost::asio::write(s, boost::asio::buffer((char*)&req, req.length));

		SERVER_HANDSHAKE_RESP_SERVER resp;
		boost::asio::read(s, boost::asio::buffer((char *)&resp, sizeof(resp)));

		if (resp.code != errorcode::handshake_succeed)
				std::cout << "FAILED\n";
		else
		{
			std::cout << "SUCCEED\n";
			std::cout << "test_server_request_error_from_id......";

			DISTRIBUTED_HEADER req;
			req.serialize(sizeof(req), 0, 0, 0, 0);
			boost::asio::write(s, boost::asio::buffer((char*)&req, req.length));

			AGENT_SERVER_RESPONSE resp;
			boost::asio::read(s, boost::asio::buffer((char*)&resp, sizeof(resp)));

			if (resp.code == errorcode::server_request_error_from_id)
				std::cout << "SUCCEED\n";
			else
				std::cout << "FAILED\n";

			std::cout << "test_server_request_error_to_id_less_100000......";

			req.serialize(sizeof(req), 0, 2222, 0, 0);
			boost::asio::write(s, boost::asio::buffer((char*)&req, req.length));
			boost::asio::read(s, boost::asio::buffer((char*)&resp, sizeof(resp)));

			if (resp.code == errorcode::server_request_error_to_id_less_100000)
				std::cout << "SUCCEED\n";
			else
				std::cout << "FAILED\n";

			std::cout << "test_server_request_error_command_null......";

			req.serialize(sizeof(req), 0, 2222, 100000, 0);
			boost::asio::write(s, boost::asio::buffer((char*)&req, req.length));
			boost::asio::read(s, boost::asio::buffer((char*)&resp, sizeof(resp)));

			if (resp.code == errorcode::server_request_error_command_null)
				std::cout << "SUCCEED\n";
			else
				std::cout << "FAILED\n";

			std::cout << "test_server_request_error_to_id_nonexistent......";

			req.serialize(sizeof(req), 0, 2222, 100000, 1);
			boost::asio::write(s, boost::asio::buffer((char*)&req, req.length));
			boost::asio::read(s, boost::asio::buffer((char*)&resp, sizeof(resp)));

			if (resp.code == errorcode::server_request_error_to_id_nonexistent)
				std::cout << "SUCCEED\n";
			else
				std::cout << "FAILED\n";
		}
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}
}

int main(int argc, char* argv[])
{
	test_client_request_error_from_id_less_100000();
#if defined(_WIN32)
	Sleep(1000);
#else
	sleep(1);
#endif
	test_client_request_error_command_null();
#if defined(_WIN32)
	Sleep(1000);
#else
	sleep(1);
#endif
	test_client_request_error_to_id_nonexistent();

	std::cout << "Press ENTER key to exit\n";
	getchar();
	return 0;
}
