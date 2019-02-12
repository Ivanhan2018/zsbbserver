#include "distributed_protocol.hpp"

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

std::string address = "192.168.1.35";
u_short port = 7771;


//void test_client_login_error_username()
//{
//	try
//	{
//		std::cout << "test_client_login_error_username......";
//
//		boost::asio::io_service io_service;
//		boost::asio::ip::tcp::endpoint endpoint(boost::asio::
//			ip::address::from_string(address.c_str()), port);
//		tcp::socket s(io_service);
//		s.connect(endpoint);
//
//		CLIENT_REGISTER_LOGIN req;
//		req.serialize("", "", 3);
//		boost::asio::write(s, boost::asio::buffer((char*)&req, req.length));
//
//		LOGIN_REGISTER_RESP_CLIENT resp;
//		boost::asio::read(s, boost::asio::buffer((char*)&resp, sizeof(resp)));
//
//		if (resp.code == errorcode::client_login_error_username)
//			std::cout << "SUCCEED\n";
//		else
//			std::cout << "FAILED\n";
//	}
//	catch (std::exception& e)
//	{
//		std::cerr << "Exception: " << e.what() << "\n";
//	}
//
//}
//
//void test_client_login_error_password()
//{
//	try
//	{
//		std::cout << "test_client_login_error_password......";
//
//		boost::asio::io_service io_service;
//		boost::asio::ip::tcp::endpoint endpoint(boost::asio::
//			ip::address::from_string(address.c_str()), port);
//		tcp::socket s(io_service);
//		s.connect(endpoint);
//
//		CLIENT_REGISTER_LOGIN req;
//		req.serialize("200000", "", 3);
//		boost::asio::write(s, boost::asio::buffer((char*)&req, req.length));
//
//		LOGIN_REGISTER_RESP_CLIENT resp;
//		boost::asio::read(s, boost::asio::buffer((char*)&resp, sizeof(resp)));
//
//		if (resp.code == errorcode::client_login_error_password)
//			std::cout << "SUCCEED\n";
//		else
//			std::cout << "FAILED\n";
//	}
//	catch (std::exception& e)
//	{
//		std::cerr << "Exception: " << e.what() << "\n";
//	}
//}
//
//void test_client_login_error_server_nonexistent()
//{
//	try
//	{
//		std::cout << "test_client_login_error_server_nonexistent......";
//
//		boost::asio::io_service io_service;
//		boost::asio::ip::tcp::endpoint endpoint(boost::asio::
//			ip::address::from_string(address.c_str()), port);
//		tcp::socket s(io_service);
//		s.connect(endpoint);
//
//		CLIENT_REGISTER_LOGIN req;
//		req.serialize("200000", "200000", 3);
//		boost::asio::write(s, boost::asio::buffer((char*)&req, req.length));
//
//		LOGIN_REGISTER_RESP_CLIENT resp;
//		boost::asio::read(s, boost::asio::buffer((char*)&resp, sizeof(resp)));
//
//		if (resp.code == errorcode::client_login_error_server_nonexistent)
//			std::cout << "SUCCEED\n";
//		else
//			std::cout << "FAILED\n";
//	}
//	catch (std::exception& e)
//	{
//		std::cerr << "Exception: " << e.what() << "\n";
//	}
//
//}
//
//void test_login(bool * run)
//{
//	while (*run)
//	{
//		try
//		{
//			boost::asio::io_service io_service;
//			boost::asio::ip::tcp::endpoint endpoint(boost::asio::
//				ip::address::from_string(address.c_str()), port);
//			tcp::socket s(io_service);
//
//			unsigned long begin = GetTickCount();
//
//			s.connect(endpoint);
//
//			CLIENT_REGISTER_LOGIN req;
//			req.serialize("123456789012345", "123456789012345", 3);
//			boost::asio::write(s, boost::asio::buffer((char*)&req, req.length));
//
//			LOGIN_REGISTER_RESP_CLIENT resp;
//			boost::asio::read(s, boost::asio::buffer((char*)&resp, sizeof(resp)));
//
//			unsigned long end = GetTickCount();
//
//			//			BOOST_ASSERT(resp.code == errorcode::client_login_succeed);
//			printf("%d\n", end - begin);
//			Sleep(1000);
//		}
//		catch (std::exception& e)
//		{
//			std::cerr << "Exception: " << e.what() << "\n";
//			return ;
//		}
//	}
//}

void test_client_login()
{
	try
	{
		std::cout << "test_client_login......";

		boost::asio::io_service io_service;
		boost::asio::ip::tcp::endpoint endpoint(boost::asio::
			ip::address::from_string(address.c_str()), port);
		tcp::socket s(io_service);
		s.connect(endpoint);

		CLIENT_REGISTER_LOGIN req;
		memset(&req, 0, sizeof(req));
		req.serialize("211841", "211841", "ÐÂ·ÉÑï_²âÊÔ_235", 20090911, 1);
		boost::asio::write(s, boost::asio::buffer((char*)&req, req.length));

		LOGIN_REGISTER_RESP_CLIENT resp;
		memset(&resp, 0, sizeof(resp));
		boost::asio::read(s, boost::asio::buffer((char*)&resp, sizeof(resp)));

		if (resp.code == errorcode::client_login_succeed)
		{
			printf("group name : %s\naddress : %s\nport:%d", req.group_name, resp.address, resp.port);
			std::cout << "SUCCEED\n";
		}
		else
			std::cout << "FAILED\n";
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}

}

int main(int argc, char* argv[])
{
	test_client_login();
	getchar();
#if defined(_WIN32)
	Sleep(1000);
#else
	sleep(1);
#endif

	test_client_login();
	getchar();
#if defined(_WIN32)
	Sleep(1000);
#else
	sleep(1);
#endif

	test_client_login();
	getchar();
#if defined(_WIN32)
	Sleep(1000);
#else
	sleep(1);
#endif

//	test_client_login_error_username();
//#if defined(_WIN32)
//	Sleep(1000);
//#else
//	sleep(1);
//#endif
//	test_client_login_error_password();
//#if defined(_WIN32)
//	Sleep(1000);
//#else
//	sleep(1);
//#endif
//	test_client_login_error_server_nonexistent();
//
//	std::cout << "Press ENTER key to exit\n";
	getchar();
	return 0;
}
