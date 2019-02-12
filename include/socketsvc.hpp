#ifndef __SOCKETSVC_HPP__
#define __SOCKETSVC_HPP__

#if defined (WIN32)
#	define SOCKETSVC_API __stdcall
#else
#	define SOCKETSVC_API
#endif

#if defined(c_plusplus) || defined(__cplusplus)
extern "C" {
#endif

	/***************************************************************************
	// int 服务器ID
	// const char * 接收的数据
	// unsigned int 接收数据的长度
	// 返回0
	***************************************************************************/
	typedef int (SOCKETSVC_API * EVENT_NOTIFY)(int server_id, 
		const char * buffer, int length);

	/***************************************************************************
	// int 代理服务器ID
	// int 玩家ID
	// const char * 接收的数据
	// unsigned int 接收数据的长度
	// 返回0
	***************************************************************************/
	typedef int (SOCKETSVC_API * DATA_RECEIVER)(int agent_id, 
		int player_id, char * buffer, int length);

	/***************************************************************************
	// const void * 设置定时器时指定的回传参数
	// 返回0
	***************************************************************************/
	typedef int (SOCKETSVC_API * TIMER_NOTIFY)(const void * param_1, const void * param_2, long timer_id);

	/***************************************************************************
	// int 玩家ID
	// 返回0
	***************************************************************************/
	typedef void (SOCKETSVC_API * SOCKET_DISCONNECT)(bool player_all,
		int agent_id, int server_id, int player_id);

	/***************************************************************************
	// const void * 设置定时器时指定的回传参数
	// 返回0
	***************************************************************************/
	typedef void (SOCKETSVC_API * LOGIC_EXCEPTION)(int code, 
		int agent_id, int from_id, int to_id);

	/***************************************************************************
	// unsigned int 服务器线程数
	// receiver_func 数据接收的回调函数
	// notify_func 事件通知接收的回调函数
	// timer_notify 定时器通知回调函数
	// 返回0为错误，返回1为正确
	// 注意：该函数在程序中只能被调用一次并且在程序开始时调用
	***************************************************************************/
	int SOCKETSVC_API init(const char * manager_address,
		unsigned short manager_port,
		const char * log_address,
		unsigned short log_port,
		int local_server_id,
		unsigned int num_threads, 
		DATA_RECEIVER receiver_func, 
		EVENT_NOTIFY notify_func,
		TIMER_NOTIFY timer_func,
		SOCKET_DISCONNECT disconn_func,
		LOGIC_EXCEPTION exception_func);

	/***************************************************************************
	// 注意：该函数在程序中只能被调用一次并且在程序退出时调用
	***************************************************************************/
	void SOCKETSVC_API fini();

	/***************************************************************************
	// int 代理器句柄，该参数会在回调接收函数中传入
	// char * 要发送的数据
	// unsigned int 要发送的数据长度
	// 返回0为错误，返回1为正确
	***************************************************************************/
	int SOCKETSVC_API asynch_write(int agent_id, char * buffer, int length);

	/***************************************************************************
	// int 事件接收服务器的句柄，该参数会在回调接收函数中传入
	// char * 要发送的数据
	// unsigned int 要发送的数据长度
	// 返回0为错误，返回1为正确
	***************************************************************************/
	int SOCKETSVC_API event_notify(int server_id, char * buffer, int length);

	/***************************************************************************
	// char * 要发送的数据
	// unsigned int 要发送的数据长度
	// 返回0为错误，返回1为正确
	***************************************************************************/
	int SOCKETSVC_API broadcast(char * buffer, int length);

	/***************************************************************************
	// int 以指定的服务器为身份来发广播，如聊天向1025郡发广播聊天消息
	// char * 要发送的数据
	// unsigned int 要发送的数据长度
	// 返回0为错误，返回1为正确
	***************************************************************************/
	int SOCKETSVC_API broadcast_ex(int id, char * buffer, int length);

	/***************************************************************************
	// unsigned long 定时器间隔,单位：秒
	// const void * 需要在定时器处发时，通知客户端的附加参数
	// 成功返回定时器ID，失败则返回0
	***************************************************************************/
	long SOCKETSVC_API set_timer(unsigned long interval, const void * param_1, const void * param_2 = 0);

	/***************************************************************************
	// unsigned long 定时器间隔,单位：秒
	// const void * 需要在定时器处发时，通知客户端的附加参数
	// 成功返回定时器ID，失败则返回0
	***************************************************************************/
	long SOCKETSVC_API set_timer_ms(unsigned long interval, const void * param_1, const void * param_2 = 0);

	/***************************************************************************
	// long 需要取消的定时器ID
	// 返回0为错误，返回1为正确
	***************************************************************************/
	int SOCKETSVC_API cancel_timer(long id);

	/***************************************************************************
	// int 错误代码
	// 返回错误信息
	***************************************************************************/
	const char * SOCKETSVC_API get_error_message(int code);

	/***************************************************************************
	// int 错误代码
	// 返回错误信息
	// 日志报告
	***************************************************************************/
	int SOCKETSVC_API report(const char * msg);

	/***************************************************************************
	// int 错误代码
	// 返回错误信息
	// 在线人数报告
	***************************************************************************/
	int SOCKETSVC_API online(int num);

	/***************************************************************************
	// int 错误代码
	// 返回错误信息
	// 游戏日志报告
	// 例：log_game_report("show me the money : %d\n", 1000);
	***************************************************************************/
	int SOCKETSVC_API log_game_report(const char *, ...);


#if defined(c_plusplus) || defined(__cplusplus)
}
#endif /*c_plusplus __cplusplus*/

#endif /* __SOCKETSVC_HPP__ */
