#if defined(_WIN32)

#include "server.hpp"
#include "request_handler.hpp"
#include "xmldoc.hpp"
#include "log.hpp"

void * operator new(std::size_t size)
{
	return ::malloc(((size >> 7) + !(!(size&127))) << 7);
}

void * operator new[](std::size_t size)
{
	return ::malloc(((size >> 7) + !(!(size&127))) << 7);
}

void read_xml_config(u_short & port, int & thread_pool, std::string & db_uid, 
					 std::string & db_pwd, std::string & db_dsn)
{
	xmlreader::xmldoc doc("config.xml");

	if (!doc.load_xml_doc()) throw std::runtime_error("load_xml_doc error");

	xmlreader::xmlnode * config_node = doc.get_root_node("config");
	if (!config_node) throw std::runtime_error("get_root_node(config) error");

	xmlreader::xmlnode * dbgatewaysvc_node = config_node->get_child("dbgatewaysvc");
	if (!dbgatewaysvc_node) throw std::runtime_error("get_child(dbgatewaysvc) error");

	port = ::atoi(dbgatewaysvc_node->property("listen_port").c_str());
	thread_pool = ::atoi(dbgatewaysvc_node->property("thread_pool").c_str());

	db_pwd = dbgatewaysvc_node->property("db_password").c_str();
	db_uid = dbgatewaysvc_node->property("db_username").c_str();
	db_dsn = dbgatewaysvc_node->property("db_dsn").c_str();
}

int main(int argc, char* argv[])
{
	exception_handler::init_exception_handler();

	try
	{
		u_short port;
		int thread_pool;
		std::string db_uid;
		std::string db_pwd;
		std::string db_dsn;

		bool flag = false;
		std::string path, type;

		while (true)
		{
			std::cout << "��ѡ���������(1.���ݿ� 2.�ļ�):";
			std::cin >> type;

			if (!strcmp(type.c_str(),"1"))
			{
				flag = false;
				break;
			}
			else if (!strcmp(type.c_str(),"2"))
			{
				flag = true;
				std::cout << "�������ļ�·��:";

				std::cin >> path;
				if (access(path.c_str(), 0))
				{
					std::cout << "�����·��\n";
				}
				else
					break;
			}
			else
			{
				std::cout << "�����ָ��\n";
			}
		}

		read_xml_config(port, thread_pool, db_uid, db_pwd, db_dsn);

		request_handler handler;
		server s(thread_pool, &handler, port);

		std::string con_str = "UID=";
		con_str += db_uid;
		con_str += ";PWD=";
		con_str += db_pwd;
		con_str += ";DSN=";
		con_str += db_dsn;

		if (handler.start(con_str.c_str(), path.c_str(), flag))
		{
			std::cout << "�����������ɹ�\n\n";

			boost::thread t(boost::bind(&server::io_service_run_event_loop, &s));

			while (true)
			{
				std::cout << "��ѡ���������(1.�˳� 2.�����ļ� 3.ȫ���������ݿ�):";
				std::cin >> type;

				if (!strcmp(type.c_str(),"1"))
				{
					std::cout << "��ȷ��Ҫ�˳���(Y/N):";
					std::cin >> type;

					if ((!strcmp(type.c_str(),"Y")) ||
						(!strcmp(type.c_str(),"y")))
						break;
				}
				else if (!strcmp(type.c_str(),"2"))
				{
					handler.save_to_file();
				}
				else if (!strcmp(type.c_str(),"3"))
				{
					std::cout << "��ȷ��Ҫȫ����Ϊ���ݿ���(Y/N):";
					std::cin >> type;

					if ((!strcmp(type.c_str(),"Y")) ||
						(!strcmp(type.c_str(),"y")))
					{
						handler.save_to_db_all();
					}
				}
				else
				{
					std::cout << "�����ָ��\n";
				}
			}
			s.io_service_end_event_loop();
			t.join();
		}

		handler.stop();
	}
	catch (std::exception& e)
	{
		MessageBox(0 , e.what(), "����", 16 + 0);
	}
	return 0;
}

#endif // defined(_WIN32)
