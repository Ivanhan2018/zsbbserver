#if defined(_WIN32)

#include "server.hpp"
#include "request_handler.hpp"
#include "xmldoc.hpp"

boost::function0<void> console_ctrl_function;

BOOL WINAPI console_ctrl_handler(DWORD ctrl_type)
{
	switch (ctrl_type)
	{
	case CTRL_C_EVENT:
	case CTRL_BREAK_EVENT:
	case CTRL_CLOSE_EVENT:
	case CTRL_SHUTDOWN_EVENT:
		console_ctrl_function();
		return TRUE;
	default:
		return FALSE;
	}
}

void read_xml_config(int & thread_pool, u_short & listen_port, long & version,
					 std::string & db_uid, std::string & db_pwd, std::string & db_dsn)
{
	xmlreader::xmldoc doc("config.xml");

	if (!doc.load_xml_doc()) throw std::runtime_error("load_xml_doc error");

	xmlreader::xmlnode * config_node = doc.get_root_node("config");
	if (!config_node) throw std::runtime_error("get_root_node(config) error");

	xmlreader::xmlnode * loginsvc_node = config_node->get_child("loginsvc");
	if (!loginsvc_node) throw std::runtime_error("get_child(loginsvc) error");

	thread_pool = ::atoi(loginsvc_node->property("thread_pool").c_str());
	listen_port = ::atoi(loginsvc_node->property("listen_port").c_str());
	version = ::atoi(loginsvc_node->property("version").c_str());

	db_pwd = loginsvc_node->property("db_password").c_str();
	db_uid = loginsvc_node->property("db_username").c_str();
	db_dsn = loginsvc_node->property("db_dsn").c_str();
}

void * operator new(std::size_t size)
{
	return ::malloc(((size >> 7) + !(!(size&127))) << 7);
}

void * operator new[](std::size_t size)
{
	return ::malloc(((size >> 7) + !(!(size&127))) << 7);
}

int main(int argc, char* argv[])
{
	exception_handler::init_exception_handler();

	try
	{
		u_short listen_port;
		int thread_pool;
		long version;
		std::string db_uid;
		std::string db_pwd;
		std::string db_dsn;

		read_xml_config(thread_pool, listen_port, version, db_uid, db_pwd, db_dsn);

		request_handler handler;
		server s(thread_pool, &handler, listen_port);

		handler.start(thread_pool, version, db_uid.c_str(), db_pwd.c_str(), db_dsn.c_str());

		console_ctrl_function = boost::bind(&server::io_service_end_event_loop, &s);
		SetConsoleCtrlHandler(console_ctrl_handler, TRUE);

		std::cout << "LoginServer v1.01\nPress CTRL + C key to exit\n";

		s.io_service_run_event_loop();
		handler.stop();
	}
	catch (std::exception& e)
	{
		MessageBox(0 , e.what(), "´íÎó", 16 + 0);
	}

	return 0;
}

#endif // defined(_WIN32)
