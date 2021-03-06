#include "exception_handler.hpp"

#if defined(_WIN32)

#include <windows.h>
#include <dbghelp.h>
#include <stdio.h>

namespace exception_handler
{
	long __stdcall top_level_exception_filter(struct _EXCEPTION_POINTERS * except_info)
	{
		typedef int (__stdcall * DUMP_WRITE)(
			HANDLE, DWORD, HANDLE, MINIDUMP_TYPE, 
			const PMINIDUMP_EXCEPTION_INFORMATION, 
			const PMINIDUMP_USER_STREAM_INFORMATION, 
			const PMINIDUMP_CALLBACK_INFORMATION);

		HMODULE dll = ::LoadLibrary("DBGHELP.DLL");
		DUMP_WRITE write_func = (DUMP_WRITE)::GetProcAddress(dll, "MiniDumpWriteDump");

		char dump_path[500 + 1] = {0};
		long path_len = ::GetModuleFileName(0, dump_path, 500);

		SYSTEMTIME curTime;
		::GetLocalTime(&curTime);

		::sprintf(dump_path + path_len, "_%4.4d%2.2d%2.2d%2.2d%2.2d%2.2d.dmp",
			curTime.wYear, curTime.wMonth, curTime.wDay, 
			curTime.wHour, curTime.wMinute, curTime.wSecond);

		HANDLE file_handle = ::CreateFile(dump_path, GENERIC_WRITE,
			FILE_SHARE_WRITE, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

		if (file_handle != INVALID_HANDLE_VALUE)
		{
			_MINIDUMP_EXCEPTION_INFORMATION exinfo = {0};
			exinfo.ThreadId = ::GetCurrentThreadId();
			exinfo.ExceptionPointers = except_info;
			exinfo.ClientPointers = 0;

			if ((*write_func)(::GetCurrentProcess(), ::GetCurrentProcessId(), 
				file_handle, MiniDumpNormal, &exinfo, 0, 0))
				::CloseHandle(file_handle);
		}
		::FreeLibrary(dll);
		return EXCEPTION_EXECUTE_HANDLER;
	}

	bool init_exception_handler(int)
	{
		HMODULE dll = ::LoadLibrary("DBGHELP.DLL");
		if (!dll) return false;

		if (!::GetProcAddress(dll, "MiniDumpWriteDump"))
		{
			::FreeLibrary(dll);
			return false;
		}

		::SetUnhandledExceptionFilter(top_level_exception_filter);
		return true;
	}
}
#else

namespace exception_handler
{
	bool init_exception_handler(int)
	{
		return true;
	}
}

#endif // defined(_WIN32)


