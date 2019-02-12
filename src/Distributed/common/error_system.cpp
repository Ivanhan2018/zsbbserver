#include "error_system.hpp"

const char * errorcode::error_message(int code)
{
	switch(code) 
	{
	case errorcode::register_error_overflow : 
		return "����������ע��ʱ������֮�ҵ�ע��ID�����Ϸ���Χ";

	case errorcode::register_error_port_nonlicet :
		return "����������ע��ʱ������֮�ҵ�ע��˿ڱ�����0��65000֮��";

	case errorcode::register_error_existent : 
		return "����������ע��ʱ������֮�ҵ�ע������Ѿ�����ʹ��";

	case errorcode::notify_error_to_id_overflow : 
		return "����������ת������ʱ������֮��ָ���Ľ��շ�(ID)��ݳ����Ϸ���Χ";

	case errorcode::notify_error_from_id_overflow :
		return "����������ת������ʱ������֮��ָ���ķ��ͷ�(ID)��ݳ����Ϸ���Χ";

	case errorcode::notify_error_to_id_from_id_equal :
		return "����������ת������ʱ������֮�Ҳ��ܸ��Լ�ת������";

	case errorcode::notify_error_protocol_length :
		return "����������ת������ʱ������֮�Ҹ�����Э�鳤�ȴ���";

	case errorcode::notify_error_to_id_nonexistent : 
		return "����������ת������ʱ������֮��ָ���Ľ��շ�������";

	case errorcode::handshake_error_to_id_overflow : 
		return "����һ̨����������ʱ������֮��ָ���ĶԷ�ID�����Ϸ��ķ�Χ";

	case errorcode::handshake_error_port_nonlicet : 
		return "����һ̨����������ʱ������֮�ҵ�ע��˿ڱ�����0��65000֮��";

	case errorcode::client_request_error_from_id_less_100000 :
		return "�ͻ������������������ʱ������֮�ͻ���Э�����From����Ϊ6λ��������";

	case errorcode::client_request_error_to_id_overflow :
		return "�ͻ������������������ʱ������֮�ͻ���Э�����To�����Ϸ���Χ";

	case errorcode::client_request_error_to_id_nonexistent :
		return "�ͻ������������������ʱ������֮�ͻ�������ķ�����������";

	case errorcode::client_request_error_command_null : 
		return "�ͻ������������������ʱ������֮�ͻ���Э�����Command����Ϊ0";

	case errorcode::server_request_error_from_id :
		return "��������ͻ�����������ʱ������֮������Э�����From��������м�¼�Ĳ�����";

	case errorcode::server_request_error_to_id_less_100000 :
		return "��������ͻ�����������ʱ������֮������Э�����To����Ϊ6λ��������";

	case errorcode::server_request_error_to_id_nonexistent : 
		return "��������ͻ�����������ʱ������֮����������Ŀͻ���������";

	case errorcode::server_request_error_command_null : 
		return "��������ͻ�����������ʱ������֮������Э�����Command����Ϊ0";

	case errorcode::client_login_error_username : 
		return "�ͻ��˵�¼ʧ�ܣ���Ϊָ�����û�������򲻴���";

	case errorcode::client_login_error_password : 
		return "�ͻ��˵�¼ʧ�ܣ���Ϊָ�����������";

	case errorcode::client_login_error_server_nonexistent : 
		return "�ͻ��˵�¼ʧ�ܣ���Ϊû�п��õĴ��������";

	case errorcode::local_error_buffer_lenght : 
		return "�����߼��������ݳ��Ȳ��Ϸ�";

	case errorcode::local_error_object_null : 
		return "�����߼������������Ϊ��";

	case errorcode::local_error_data_0 : 
		return "�����߼��������ݲ���Ϊ0";

	case errorcode::local_error_address_0 : 
		return "�����߼�����IP��ַ����Ϊ0";

	case errorcode::local_error_port_overflow : 
		return "�����߼����󣬶˿ڱ������0";

	case errorcode::local_error_thread_num_overflow : 
		return "�����߼������߳����������0����С��100";

	case errorcode::local_error_id_overflow : 
		return "�����߼�����ָ����ID����Ϊ0�򲻺Ϸ�";

	case errorcode::local_error_server_nonexistent : 
		return "�����߼�����ָ���ķ�����������";

	case errorcode::local_error_param : 
		return "�����߼������и����������������";

	case errorcode::local_error_username_or_password_length : 
		return "�����߼������û��������볤�ȹ���";

	case errorcode::local_error_address : 
		return "�����߼����󣬷�������ַ��ʽ����";

	case errorcode::local_error_connect : 
		return "�����߼����󣬷��������Ӵ���";

	case errorcode::local_error_tcp_socket : 
		return "�����߼�����ͨ�Ŵ���";

	case errorcode::local_error_connect_proxy : 
		return "�����߼���������HTTP����ʧ��";

	case errorcode::local_error_proxy_username_or_password : 
		return "�����߼����󣬵�¼HTTP������û������������";

	case errorcode::web_error_database :
		return "���ݿ����س�����������";

	case web_login_error_username :
		return "��¼�û�������";

	case web_login_error_password :
		return "��¼�������";

	case web_register_error_username_existent :
		return "ע���û����Ѵ���";

	case client_login_error_version :
		return "��ǰ�ͻ��汾̫�ɣ��������°汾";

	case client_login_error_repet :
		return "�û��ѵ�¼�������ظ���¼";

	default: return "�޷�ʶ��Ĵ���";

	}
}

