#if !defined(_WIN32)

#include "server.hpp"
#include "request_handler.hpp"
#include "xmldoc.hpp"

#include <pthread.h>
#include <signal.h>

void read_xml_config(u_short & port)
{
	xmlreader::xmldoc doc("config.xml");

	if (!doc.load_xml_doc()) throw std::runtime_error("load_xml_doc error");

	xmlreader::xmlnode * config_node = doc.get_root_node("config");
	if (!config_node) throw std::runtime_error("get_root_node(config) error");

	xmlreader::xmlnode * managersvc_node = config_node->get_child("managersvc");
	if (!managersvc_node) throw std::runtime_error("get_child(managersvc) error");

	port = ::atoi(managersvc_node->property("listen_port").c_str());
}

int main(int argc, char* argv[])
{
	try
	{
		u_short port;
		read_xml_config(port);


		sigset_t new_mask;
		sigfillset(&new_mask);
		sigset_t old_mask;
		pthread_sigmask(SIG_BLOCK, &new_mask, &old_mask);

		request_handler handler;
		server s(1, &handler, port);
		boost::thread t(boost::bind(&server::io_service_run_event_loop, &s));

		std::cout << "ManagerServer v1.01\nPress CTRL + C key to exit\n";

		pthread_sigmask(SIG_SETMASK, &old_mask, 0);

		sigset_t wait_mask;
		sigemptyset(&wait_mask);
		sigaddset(&wait_mask, SIGINT);
		sigaddset(&wait_mask, SIGQUIT);
		sigaddset(&wait_mask, SIGTERM);
		pthread_sigmask(SIG_BLOCK, &wait_mask, 0);
		int sig = 0;
		sigwait(&wait_mask, &sig);

		s.io_service_end_event_loop();
		handler.clear_all();
		t.join();
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}

	return 0;
}

#endif // !defined(_WIN32)
