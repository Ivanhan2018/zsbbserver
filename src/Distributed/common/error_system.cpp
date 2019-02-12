#include "error_system.hpp"

const char * errorcode::error_message(int code)
{
	switch(code) 
	{
	case errorcode::register_error_overflow : 
		return "向管理服务器注册时，被告之我的注册ID超出合法范围";

	case errorcode::register_error_port_nonlicet :
		return "向管理服务器注册时，被告之我的注册端口必须在0到65000之间";

	case errorcode::register_error_existent : 
		return "向管理服务器注册时，被告之我的注册身份已经被人使用";

	case errorcode::notify_error_to_id_overflow : 
		return "向管理服务器转发数据时，被告之我指定的接收方(ID)身份超出合法范围";

	case errorcode::notify_error_from_id_overflow :
		return "向管理服务器转发数据时，被告之我指定的发送方(ID)身份超出合法范围";

	case errorcode::notify_error_to_id_from_id_equal :
		return "向管理服务器转发数据时，被告之我不能给自己转发数据";

	case errorcode::notify_error_protocol_length :
		return "向管理服务器转发数据时，被告之我给定的协议长度错误";

	case errorcode::notify_error_to_id_nonexistent : 
		return "向管理服务器转发数据时，被告之我指定的接收方不在线";

	case errorcode::handshake_error_to_id_overflow : 
		return "向另一台服务器握手时，被告之我指定的对方ID超出合法的范围";

	case errorcode::handshake_error_port_nonlicet : 
		return "向另一台服务器握手时，被告之我的注册端口必须在0到65000之间";

	case errorcode::client_request_error_from_id_less_100000 :
		return "客户端向服务器请求数据时，被告之客户端协议包中From至少为6位以上整数";

	case errorcode::client_request_error_to_id_overflow :
		return "客户端向服务器请求数据时，被告之客户端协议包中To超出合法范围";

	case errorcode::client_request_error_to_id_nonexistent :
		return "客户端向服务器请求数据时，被告之客户端请求的服务器已下线";

	case errorcode::client_request_error_command_null : 
		return "客户端向服务器请求数据时，被告之客户端协议包中Command不能为0";

	case errorcode::server_request_error_from_id :
		return "服务器向客户端请求数据时，被告之服务器协议包中From与服务器中记录的不附合";

	case errorcode::server_request_error_to_id_less_100000 :
		return "服务器向客户端请求数据时，被告之服务器协议包中To至少为6位以上整数";

	case errorcode::server_request_error_to_id_nonexistent : 
		return "服务器向客户端请求数据时，被告之服务器请求的客户端已下线";

	case errorcode::server_request_error_command_null : 
		return "服务器向客户端请求数据时，被告之服务器协议包中Command不能为0";

	case errorcode::client_login_error_username : 
		return "客户端登录失败，因为指定的用户名错误或不存在";

	case errorcode::client_login_error_password : 
		return "客户端登录失败，因为指定的密码错误";

	case errorcode::client_login_error_server_nonexistent : 
		return "客户端登录失败，因为没有可用的代理服务器";

	case errorcode::local_error_buffer_lenght : 
		return "本地逻辑错误，数据长度不合法";

	case errorcode::local_error_object_null : 
		return "本地逻辑错误，类对象不能为空";

	case errorcode::local_error_data_0 : 
		return "本地逻辑错误，数据不能为0";

	case errorcode::local_error_address_0 : 
		return "本地逻辑错误，IP地址不能为0";

	case errorcode::local_error_port_overflow : 
		return "本地逻辑错误，端口必须大于0";

	case errorcode::local_error_thread_num_overflow : 
		return "本地逻辑错误，线程数必须大于0并且小于100";

	case errorcode::local_error_id_overflow : 
		return "本地逻辑错误，指定的ID不能为0或不合法";

	case errorcode::local_error_server_nonexistent : 
		return "本地逻辑错误，指定的服务器不存在";

	case errorcode::local_error_param : 
		return "本地逻辑错误，有个参数输入错误，请检查";

	case errorcode::local_error_username_or_password_length : 
		return "本地逻辑错误，用户名或密码长度过长";

	case errorcode::local_error_address : 
		return "本地逻辑错误，服务器地址格式错误";

	case errorcode::local_error_connect : 
		return "本地逻辑错误，服务器连接错误";

	case errorcode::local_error_tcp_socket : 
		return "本地逻辑错误，通信错误";

	case errorcode::local_error_connect_proxy : 
		return "本地逻辑错误，连接HTTP代理失败";

	case errorcode::local_error_proxy_username_or_password : 
		return "本地逻辑错误，登录HTTP代理的用户名或密码错误";

	case errorcode::web_error_database :
		return "数据库网关出现致命错误";

	case web_login_error_username :
		return "登录用户名错误";

	case web_login_error_password :
		return "登录密码错误";

	case web_register_error_username_existent :
		return "注册用户名已存在";

	case client_login_error_version :
		return "当前客户版本太旧，请下载新版本";

	case client_login_error_repet :
		return "用户已登录，不能重复登录";

	default: return "无法识别的代码";

	}
}

