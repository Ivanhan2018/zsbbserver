#include "export_log_tool_dll.hpp"

//#include "distributed_protocol.hpp"
//#include <cstdlib>
//#include <cstring>
//#include <iostream>
//#include <boost/asio.hpp>
//
//using boost::asio::ip::tcp;
//
//std::string address = "192.168.1.18";
//u_short port = 512;
//
//void test_register_error_overflow()
//{
//	try
//	{
//		std::cout << "test_register_error_overflow......";
//
//		boost::asio::io_service io_service;
//		boost::asio::ip::tcp::endpoint endpoint(boost::asio::
//			ip::address::from_string(address.c_str()), port);
//		tcp::socket s(io_service);
//		s.connect(endpoint);
//		SERVER_REGISTER_MANAGER req;
//		req.serialize(1, 111);
//		boost::asio::write(s, boost::asio::buffer((char*)&req, req.length));
//
//		MANAGER_REGISTER_RESP_SERVER resp;
//		boost::asio::read(s, boost::asio::buffer((char*)&resp, sizeof(resp)));
//
//		if (resp.code == errorcode::register_error_overflow)
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
//void test_register_error_port_nonlicet()
//{
//	try
//	{
//		std::cout << "test_register_error_port_nonlicet......";
//		boost::asio::io_service io_service;
//		boost::asio::ip::tcp::endpoint endpoint(boost::asio::
//			ip::address::from_string(address.c_str()), port);
//		tcp::socket s(io_service);
//		s.connect(endpoint);
//
//		SERVER_REGISTER_MANAGER req;
//		req.serialize(2000, 0);
//		boost::asio::write(s, boost::asio::buffer((char*)&req, req.length));
//
//		MANAGER_REGISTER_RESP_SERVER resp;
//		boost::asio::read(s, boost::asio::buffer((char*)&resp, sizeof(resp)));
//
//		if (resp.code == errorcode::register_error_port_nonlicet)
//			std::cout << "SUCCEED\n";
//		else
//			std::cout << "FAILED\n";
//
//	}
//	catch (std::exception& e)
//	{
//		std::cerr << "Exception: " << e.what() << "\n";
//	}
//}
//
//void test_register_succeed_and_error_existent()
//{
//	try
//	{
//		std::cout << "test_register_succeed......";
//		boost::asio::io_service io_service;
//
//		boost::asio::ip::tcp::endpoint endpoint(boost::asio::
//			ip::address::from_string(address.c_str()), port);
//		tcp::socket s(io_service);
//		s.connect(endpoint);
//
//		SERVER_REGISTER_MANAGER req;
//		req.serialize(SERVER_BOUND_MAP_BEGIN, 9999);
//		boost::asio::write(s, boost::asio::buffer((char*)&req, req.length));
//
//		MANAGER_REGISTER_RESP_SERVER resp;
//		boost::asio::read(s, boost::asio::buffer((char*)&resp, sizeof(resp)));
//
//		if (resp.code == errorcode::register_succeed)
//			std::cout << "SUCCEED\n";
//		else
//			std::cout << "FAILED\n";
//
//		std::cout << "test_register_error_existent......";
//		tcp::socket s1(io_service);
//		s1.connect(endpoint);
//
//		SERVER_REGISTER_MANAGER req1;
//		req1.serialize(SERVER_BOUND_MAP_BEGIN, 9999);
//		boost::asio::write(s1, boost::asio::buffer((char*)&req1, req1.length));
//
//		MANAGER_REGISTER_RESP_SERVER resp1;
//		boost::asio::read(s1, boost::asio::buffer((char*)&resp1, sizeof(resp1)));
//
//		if (resp1.code == errorcode::register_error_existent)
//			std::cout << "SUCCEED\n";
//		else
//			std::cout << "FAILED\n";
//
//	}
//	catch (std::exception& e)
//	{
//		std::cerr << "Exception: " << e.what() << "\n";
//	}
//}
//
//void test_notify_error_to_id_overflow()
//{
//	try
//	{
//		std::cout << "test_notify_error_to_id_overflow......";
//		boost::asio::io_service io_service;
//
//		boost::asio::ip::tcp::endpoint endpoint(boost::asio::
//			ip::address::from_string(address.c_str()), port);
//		tcp::socket s(io_service);
//		s.connect(endpoint);
//
//		SERVER_NOTIFY_MANAGER_NOTIFY_SERVER req;
//		req.serialize(1000, 2, 0);
//		boost::asio::write(s, boost::asio::buffer((char*)&req, req.length));
//
//		SERVER_NOTIFY_RESP_SERVER resp;
//		boost::asio::read(s, boost::asio::buffer((char*)&resp, sizeof(resp)));
//
//		if (resp.code == errorcode::notify_error_to_id_overflow)
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
//void test_notify_error_from_id_overflow()
//{
//	try
//	{
//		std::cout << "test_notify_error_from_id_overflow......";
//		boost::asio::io_service io_service;
//
//		boost::asio::ip::tcp::endpoint endpoint(boost::asio::
//			ip::address::from_string(address.c_str()), port);
//		tcp::socket s(io_service);
//		s.connect(endpoint);
//
//		SERVER_NOTIFY_MANAGER_NOTIFY_SERVER req;
//		req.serialize(1, 2000, 0);
//		boost::asio::write(s, boost::asio::buffer((char*)&req, req.length));
//
//		SERVER_NOTIFY_RESP_SERVER resp;
//		boost::asio::read(s, boost::asio::buffer((char*)&resp, sizeof(resp)));
//
//		if (resp.code == errorcode::notify_error_from_id_overflow)
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
//void test_notify_error_to_id_from_id_equal()
//{
//	try
//	{
//		std::cout << "test_notify_error_to_id_from_id_equal......";
//		boost::asio::io_service io_service;
//
//		boost::asio::ip::tcp::endpoint endpoint(boost::asio::
//			ip::address::from_string(address.c_str()), port);
//		tcp::socket s(io_service);
//		s.connect(endpoint);
//
//		SERVER_NOTIFY_MANAGER_NOTIFY_SERVER req;
//		req.serialize(2000, 2000, 0);
//		boost::asio::write(s, boost::asio::buffer((char*)&req, req.length));
//
//		SERVER_NOTIFY_RESP_SERVER resp;
//		boost::asio::read(s, boost::asio::buffer((char*)&resp, sizeof(resp)));
//
//		if (resp.code == errorcode::notify_error_to_id_from_id_equal)
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
//void test_notify_error_protocol_length()
//{
//	try
//	{
//		std::cout << "test_notify_error_protocol_length......";
//		boost::asio::io_service io_service;
//
//		boost::asio::ip::tcp::endpoint endpoint(boost::asio::
//			ip::address::from_string(address.c_str()), port);
//		tcp::socket s(io_service);
//		s.connect(endpoint);
//
//		SERVER_NOTIFY_MANAGER_NOTIFY_SERVER req;
//		req.serialize(2000, 2001, 0);
//		boost::asio::write(s, boost::asio::buffer((char*)&req, req.length));
//
//		SERVER_NOTIFY_RESP_SERVER resp;
//		boost::asio::read(s, boost::asio::buffer((char*)&resp, sizeof(resp)));
//
//		if (resp.code == errorcode::notify_error_protocol_length)
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
//void test_notify_error_to_nonexistent()
//{
//	try
//	{
//		std::cout << "test_notify_error_to_nonexistent......";
//		boost::asio::io_service io_service;
//
//		boost::asio::ip::tcp::endpoint endpoint(boost::asio::
//			ip::address::from_string(address.c_str()), port);
//		tcp::socket s(io_service);
//		s.connect(endpoint);
//
//		SERVER_NOTIFY_MANAGER_NOTIFY_SERVER req;
//		req.serialize(2000, 2001, 10);
//		boost::asio::write(s, boost::asio::buffer((char*)&req, req.length));
//
//		SERVER_NOTIFY_RESP_SERVER resp;
//		boost::asio::read(s, boost::asio::buffer((char*)&resp, sizeof(resp)));
//
//		if (resp.code == errorcode::notify_error_to_id_nonexistent)
//			std::cout << "SUCCEED\n";
//		else
//			std::cout << "FAILED\n";
//	}
//	catch (std::exception& e)
//	{
//		std::cerr << "Exception: " << e.what() << "\n";
//	}
//}

#include <iostream>

bool EXPORT_LOG_TOOL_DLL_API data_receiver(const char * filename, const char * buffer, int length, float rate)
{
	std::cout << "filename : " << filename << " length : " << length << "  rate : " << rate << std::endl;
	return true;
}

//char buf[10240000];
int main(int argc, char* argv[])
{
	//FILE * fp = ::fopen("D:\\work\\ZSBB_Servers\\bin\\log_game\\201004\\12\\1.log", "r");
	//logic_check(!fp, ("fopen error\n"));
	//logic_check(!::fread(buf, file_size, 1, fp), ("fread error\n"));
	//fclose(fp);

	for (int i = 0; i < 100; i++)
	{
	if (log_download("192.168.1.243", 3000, 20100401, 20100931, data_receiver))
		std::cout << "\n²âÊÔ³É¹¦\n";
	}

//	test_register_error_overflow();
//#if defined(_WIN32)
//	Sleep(1000);
//#else
//	sleep(1);
//#endif
//	test_register_error_port_nonlicet();
//#if defined(_WIN32)
//	Sleep(1000);
//#else
//	sleep(1);
//#endif
//	test_register_succeed_and_error_existent();
//#if defined(_WIN32)
//	Sleep(1000);
//#else
//	sleep(1);
//#endif
//	test_notify_error_to_id_overflow();
//#if defined(_WIN32)
//	Sleep(1000);
//#else
//	sleep(1);
//#endif
//	test_notify_error_from_id_overflow();
//#if defined(_WIN32)
//	Sleep(1000);
//#else
//	sleep(1);
//#endif
//	test_notify_error_to_id_from_id_equal();
//#if defined(_WIN32)
//	Sleep(1000);
//#else
//	sleep(1);
//#endif
//	test_notify_error_protocol_length();
//#if defined(_WIN32)
//	Sleep(1000);
//#else
//	sleep(1);
//#endif
//	test_notify_error_to_nonexistent();
//
	std::cout << "Press ENTER key to exit\n";
	getchar();
	return 0;
}

