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

void * operator new(std::size_t size)
{
	return ::malloc(((size >> 7) + !(!(size&127))) << 7);
}

void * operator new[](std::size_t size)
{
	return ::malloc(((size >> 7) + !(!(size&127))) << 7);
}

void read_xml_config(std::string & log_game_address, u_short & log_game_port, u_short & port, u_short & game_id, u_short & group_id)
{
	xmlreader::xmldoc doc("config.xml");

	if (!doc.load_xml_doc()) throw std::runtime_error("load_xml_doc error");

	xmlreader::xmlnode * config_node = doc.get_root_node("config");
	if (!config_node) throw std::runtime_error("get_root_node(config) error");

	xmlreader::xmlnode * managersvc_node = config_node->get_child("managersvc");
	if (!managersvc_node) throw std::runtime_error("get_child(managersvc) error");

	port = ::atoi(managersvc_node->property("listen_port").c_str());
	game_id = ::atoi(managersvc_node->property("game_id").c_str());
	group_id = ::atoi(managersvc_node->property("group_id").c_str());
	group_id = ::atoi(managersvc_node->property("group_id").c_str());
	log_game_address = managersvc_node->property("log_game_address");
	log_game_port = ::atoi(managersvc_node->property("log_game_port").c_str());
}

int main(int argc, char* argv[])
{
	exception_handler::init_exception_handler();

	try
	{
		std::string log_game_address;
		u_short port, game_id, group_id, log_game_port;
		read_xml_config(log_game_address, log_game_port, port, game_id, group_id);

		request_handler handler(game_id, group_id, log_game_address, log_game_port);
		server s(1, &handler, port);

		console_ctrl_function = boost::bind(&server::io_service_end_event_loop, &s);
		SetConsoleCtrlHandler(console_ctrl_handler, TRUE);

		std::cout << "ManagerServer v1.01\nPress CTRL + C key to exit\n";

		s.io_service_run_event_loop();

		handler.clear_all();
	}
	catch (std::exception& e)
	{
		MessageBox(0 , e.what(), "´íÎó", 16 + 0);
	}

	return 0;
}

#endif // defined(_WIN32)
