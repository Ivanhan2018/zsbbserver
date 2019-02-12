#ifndef __EXPORT_LOG_TOOL_DLL_HPP__
#define __EXPORT_LOG_TOOL_DLL_HPP__

#if defined (WIN32)
#	define EXPORT_LOG_TOOL_DLL_API __stdcall
#else
#	define EXPORT_LOG_TOOL_DLL_API
#endif

#if defined(c_plusplus) || defined(__cplusplus)
extern "C" {
#endif

	enum 
	{
		socket_error,
		connect_error,
		dir_nonexist,
		uncompress_error,
		succeed
	};

	// 返回true，续接文件，返回false，中止接文件
	typedef bool (EXPORT_LOG_TOOL_DLL_API * DATA_RECEIVER)(
		const char * filename, const char * buffer, int length, float rate);

	// 返回true，完成全部下载，返回false，可能是目录不存在或网络有问题
	int EXPORT_LOG_TOOL_DLL_API log_download(
		const char * address,
		unsigned short port, 
		int begin_date,
		int end_date,
		DATA_RECEIVER receiver_func);

	// 例子：返回true，续接文件，返回false，中止接文件
	//bool EXPORT_LOG_TOOL_DLL_API data_receiver(const char * filename, const char * buffer, int length)
	//{
	//	std::cout << "filename : " << filename << " length : " << length << std::endl;
	//	return true;
	//}

	//int main(int argc, char* argv[])
	//{
	//	if (log_download("192.168.1.243", 3000, 20100401, 20100931, data_receiver))
	//		std::cout << "\n测试成功\n";
	//}

#if defined(c_plusplus) || defined(__cplusplus)
}
#endif /*c_plusplus __cplusplus*/

#endif /* __EXPORT_LOG_TOOL_DLL_HPP__ */
