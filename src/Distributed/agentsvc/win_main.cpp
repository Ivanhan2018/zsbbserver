#if defined(_WIN32)

#include "server.hpp"
#include "request_handler.hpp"
#include "xmldoc.hpp"
#include "log_handler.hpp"

log_handler * __log_handler;

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

void read_xml_config(u_short & listen_port, int & thread_pool, std::string & group_name, int & role_id, 
					 std::string & manager_address, u_short & manager_port,
					 std::string & log_address, u_short & log_port,
					 std::string & login_addr, u_short & login_port, std::string & url)
{
	xmlreader::xmldoc doc("config.xml");

	if (!doc.load_xml_doc()) throw std::runtime_error("load_xml_doc error");

	xmlreader::xmlnode * config_node = doc.get_root_node("config");
	if (!config_node) throw std::runtime_error("get_root_node(config) error");

	xmlreader::xmlnode * agentsvc_node = config_node->get_child("agentsvc");
	if (!agentsvc_node) throw std::runtime_error("get_child(agentsvc) error");

	listen_port = ::atoi(agentsvc_node->property("listen_port").c_str());
	thread_pool = ::atoi(agentsvc_node->property("thread_pool").c_str());
	group_name = agentsvc_node->property("group_name").c_str();
	role_id = ::atoi(agentsvc_node->property("role_id").c_str());
	manager_address = agentsvc_node->property("manager_address").c_str();
	manager_port = ::atoi(agentsvc_node->property("manager_port").c_str());
	log_address = agentsvc_node->property("log_address").c_str();
	log_port = ::atoi(agentsvc_node->property("log_port").c_str());
	login_addr = agentsvc_node->property("login_address").c_str();
	login_port = ::atoi(agentsvc_node->property("login_port").c_str());
	url = agentsvc_node->property("agent_url").c_str();
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
		std::string group_name;
		int role_id, thread_pool;
		std::string manager_address, login_addr, url, log_address;
		u_short manager_port, login_port, log_port;

		read_xml_config(listen_port, thread_pool, group_name, role_id, manager_address, 
			manager_port, log_address, log_port, login_addr, login_port, url);

		request_handler handler;
		server s(thread_pool, &handler, listen_port);

		log_handler log;
		__log_handler = &log;
		log.start(role_id, log_address.c_str(), log_port);

		handler.start(manager_address.c_str(), manager_port, group_name.c_str(), role_id,
			s.get_listen_port(), login_addr.c_str(), login_port, url.c_str(), s.get_io_service());

		console_ctrl_function = boost::bind(&server::io_service_end_event_loop, &s);
		SetConsoleCtrlHandler(console_ctrl_handler, TRUE);

		std::cout << "AgentServer v1.01\nPress CTRL + C key to exit\n";

		s.io_service_run_event_loop();

		log.stop();
		handler.stop();
	}
	catch (std::exception& e)
	{
		MessageBox(0 , e.what(), "´íÎó", 16 + 0);
	}

	return 0;
}

#endif // defined(_WIN32)
