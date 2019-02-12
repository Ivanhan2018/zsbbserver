/***************************************************************************
*   Copyright (C) 2007 by wangshenwang                                    *
*   wangshenwang1@gmail.com                                               *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
*   This program is distributed in the hope that it will be useful,       *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
*   GNU General Public License for more details.                          *
*                                                                         *
*   You should have received a copy of the GNU General Public License     *
*   along with this program; if not, write to the                         *
*   Free Software Foundation, Inc.,                                       *
*   59 Temple Place - Suite 330, Boston, MA  02111-1307, CHN.             *
***************************************************************************/

#ifndef __HEADER_HPP__
#define __HEADER_HPP__

#if defined(_WIN32)
#	define _WIN32_WINNT 0x0502
#endif

#pragma warning( disable : 4996 4267)


#include <queue>
#include <vector>
#include <hash_map>
#include <iostream>

#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <boost/thread/condition.hpp>
#include <boost/enable_shared_from_this.hpp>
#include "boost/filesystem/operations.hpp"
#include "boost/filesystem/path.hpp"

#if defined(_WIN32)
#	include <mstcpip.h>
#endif

#include "exception_handler.hpp"
#include "read_write_mutex.hpp"
#include "error_system.hpp"

class check_helper
{
public:
	explicit check_helper(
		const char * file, 
		int line, 
		const char * func, 
		const char * tm)
	{
		std::size_t pos = strlen(file);
		while (file[pos] != '\\' && pos){ --pos; }
		file_ = (const char *)(file + pos + 1);
		function_ = func;
		line_ = line;
		time_ = tm;
	}

	void check_assert(char * fmt, ...)
	{
		va_list ap;
		char message[512]; 
		memset(message,0,sizeof(message));
		va_start(ap,fmt);
		vsprintf(message,fmt,ap);
		va_end(ap);

		std::cout << std::endl 
			<< "assert message :" 
			<< std::endl 
			<< "  file : "		<< file_ 
			<< std::endl 
			<< "  function : "	<< function_ 
			<< std::endl 
			<< "  line : "		<< line_ 
			<< std::endl 
			<< "  time : "		<< time_ 
			<< std::endl
			<< "  message : "	<< message
			<< std::endl;

		abort();
	}

	void check_throw(char * fmt, ...)
	{
		va_list ap;
		char message[512]; 
		memset(message,0,sizeof(message));
		va_start(ap,fmt);
		vsprintf(message,fmt,ap);
		va_end(ap);

		std::stringstream os;
		os  <<  std::endl 
			<< "  file : "		<< file_ 
			<< std::endl 
			<< "  function : "	<< function_ 
			<< std::endl 
			<< "  line : "		<< line_ 
			<< std::endl 
			<< "  time : "		<< time_ 
			<< std::endl
			<< "  message : "	<< message
			<< std::endl;

		throw std::runtime_error(os.str().c_str());
	}

private:
	const char * file_;
	const char * function_;
	const char * time_;
	int line_;
};


#ifndef NDEBUG

#define logic_check(W, X) \
	if ((W)) \
	{ \
		check_helper ch(__FILE__, __LINE__, __FUNCTION__, __TIME__); \
		ch.check_assert X; \
	}

#else

#define logic_check(W, X) \
	if ((W)) \
	{ \
		check_helper ch(__FILE__, __LINE__, __FUNCTION__, __TIME__); \
		ch.check_throw X; \
	}

#endif

#endif // __HEADER_HPP__
