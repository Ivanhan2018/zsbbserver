#ifndef __ERROR_SYSTEM_HPP__
#define __ERROR_SYSTEM_HPP__


namespace errorcode
{
	const int register_succeed = 0xCCCC;
	const int register_error_overflow = 0x0001;
	const int register_error_port_nonlicet = 0x0002;
	const int register_error_existent = 0x0003;

	const int notify_error_to_id_overflow = 0x0004;
	const int notify_error_from_id_overflow = 0x0005;
	const int notify_error_to_id_from_id_equal = 0x0006;
	const int notify_error_protocol_length = 0x0007;
	const int notify_error_to_id_nonexistent = 0x0008;
	
	const int handshake_succeed = 0xCCCC;
	const int handshake_error_to_id_overflow = 0x0009;
	const int handshake_error_port_nonlicet = 0x000A;

	const int client_request_error_from_id_less_100000 = 0x000B;
	const int client_request_error_to_id_overflow = 0x000C;
	const int client_request_error_to_id_nonexistent = 0x000D;
	const int client_request_error_command_null = 0x00E;

	const int server_request_error_from_id = 0x000F;
	const int server_request_error_to_id_less_100000 = 0x0010;
	const int server_request_error_to_id_nonexistent = 0x0011;
	const int server_request_error_command_null = 0x0012;

	const int client_login_succeed = 0xCCCC;
	const int client_login_error_username = 0x0013;
	const int client_login_error_password = 0x0014;
	const int client_login_error_server_nonexistent = 0x0015;
	const int client_login_error_version = 0x002A;
	const int client_login_error_repet = 0x002B;


	const int local_error_buffer_lenght = 0x0016;
	const int local_error_object_null = 0x0017;
	const int local_error_data_0 = 0x0018;
	const int local_error_address_0 = 0x0019;
	const int local_error_port_overflow = 0x001A;
	const int local_error_thread_num_overflow = 0x001B;
	const int local_error_id_overflow = 0x001C;
	const int local_error_server_nonexistent = 0x001D;

	const int local_error_param = 0x001E;
	const int local_error_username_or_password_length = 0x001F;
	const int local_error_address = 0x0020;
	const int local_error_connect = 0x0021;
	const int local_error_tcp_socket = 0x0022;
	const int local_error_connect_proxy = 0x0023;
	const int local_error_proxy_username_or_password = 0x0024;

	const int web_error_database = 0x0025;
	const int web_login_error_username = 0x0026;
	const int web_login_error_password = 0x0027;
	const int web_login_succeed = 0xCCCC;
	const int web_register_error_username_existent = 0x0028;
	const int web_register_succeed = 0x0029;

	const  char * error_message(int code);
};


#endif // __ERROR_SYSTEM_HPP__
