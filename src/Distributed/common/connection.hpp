#ifndef __CONNECTION_HPP__
#define __CONNECTION_HPP__

#include "header.hpp"

class request_handler;

class connection : private boost::noncopyable, 
	public boost::enable_shared_from_this<connection>
{
public:
	enum { http_header_size = 0x0007 };
	enum { max_header_size = 0x000B };
	enum { max_buffer_size = 10485760 };
	enum { default_buffer_size = 0x0100 };
	enum { http_proxy_header_size = 59};

public:
	explicit connection(boost::asio::io_service&, request_handler*);

	boost::asio::ip::tcp::socket& socket();

	void start_async_read(bool flag = false);

	void start_async_write(char *, int);

	int & id() {return conn_id_;}
	int & address() {return address_;}
	u_short & port() {return port_;}
	int & online() {return online_;}
	std::string & url() {return url_;}
	std::string & group_name() {return group_name_;}
	bool connected() {return flag_;}

private:
	void handle_read_header(const boost::system::error_code&);
	void handle_read_http(const boost::system::error_code&, char *);
	void handle_read_body(const boost::system::error_code&, char *);
	void handle_write(const boost::system::error_code& e, char *);

	boost::asio::ip::tcp::socket socket_;

	bool flag_;
	int conn_id_;
	int address_;
	std::string url_;
	std::string group_name_;
	u_short port_;
	int * length_;
	int online_;
	char header_[max_header_size];
	request_handler * handler_;
};

#endif // __CONNECTION_HPP__
