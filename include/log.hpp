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

#ifndef __LOG_HPP__
#define __LOG_HPP__

#include "socketsvc.hpp"
#include <io.h>
#include <direct.h>
#include <stdarg.h>
#include <time.h>
#include <fstream>
#pragma warning(push)
#pragma warning(disable: 4996)
class log_helper
{
public:
	explicit log_helper(const char * file, int line)
	{
		std::size_t pos = strlen(file);
		while (file[pos] != '\\' && pos){ --pos; }
		file_ = (const char *)(file + pos + 1);
		line_ = line;
		time_ = (unsigned long)time(0);
	}

	void db_format(const char * fmt, ...)
	{
		int len = sprintf(buffer_, "file(%s) line(%d) ", file_, line_);
	
		va_list ap;
		va_start(ap,fmt);
		vsprintf(buffer_ + len, fmt, ap);
		va_end(ap);
	}

	void log_format(const char * fmt, ...)
	{
		unsigned long dc = time_ % 86400;
		int len = sprintf(buffer_, "time(%02d:%02d:%02d) file(%s) line(%d) ", 
			(dc / 3600 + 8) % 24, (dc % 3600) / 60, dc % 60, file_, line_);
	
		va_list ap;
		va_start(ap,fmt);
		vsprintf(buffer_ + len, fmt, ap);
		va_end(ap);
		strcat(buffer_, "\n");
	}

	const char * message() { return buffer_; }

private:
	const char * file_;
	int line_;
	unsigned long time_;
	char buffer_[300];
};

class logger
{
public:
	static logger& instance() { static logger l; return l; }
	void write(const char * buf)
	{ 
		fout_ << buf; 
	}

private:
	explicit logger()
	{
		char path[MAX_PATH];
		long path_len = ::GetModuleFileName(0, path, MAX_PATH);
		while (path[path_len--] != '\\');
		path[path_len + 2] = 0;

		strcat(path, "log\\");
		if (access(path, 0)) mkdir(path);

		char name[MAX_PATH];
		long name_len = ::GetConsoleTitle(name, MAX_PATH);
		while (name_len && name[name_len--] != '\\');
		name_len = (name_len < 1 ? 0 : name_len + 2);
		strcat(path, name + name_len);

		time_t t = ::time(0);
		tm * fat = ::gmtime(&t);

		char tmc[20];
		sprintf(tmc, " %4.4d-%2.2d-%2.2d.log", fat->tm_year + 1900,
			fat->tm_mon + 1, fat->tm_mday);

		strcat(path, tmc);

		fout_.open(path, std::ios::out | std::ios::app);
	}

	virtual ~logger() { fout_.close(); }

	std::ofstream fout_;
};


#define LOG_TRACE(X) \
	{ \
		log_helper lh(__FILE__, __LINE__); \
		lh.db_format X; \
		report(lh.message()); \
	} 


#define LOG_TRACE_EX(W, X) \
	if (!(W)) \
	{ \
		log_helper lh(__FILE__, __LINE__); \
		lh.db_format X; \
		report(lh.message()); \
	}

#define THROW_EXECPTION(X) if (!(X)) throw throw std::runtime_error("")


#ifdef NDEBUG
#	define LOG_ERROR(X) \
	{ \
		log_helper lh(__FILE__, __LINE__); \
		lh.log_format X; \
		logger::instance().write(lh.message()); \
	}

#	define LOG_ERROR_EX(W, X) \
	if (!(W)) \
	{ \
		log_helper lh(__FILE__, __LINE__); \
		lh.log_format X; \
		logger::instance().write(lh.message()); \
	}

#	define LOG_ERROR_EX_RETURN(W, X, R) \
	if (!(W)) \
	{ \
		log_helper lh(__FILE__, __LINE__); \
		lh.log_format X; \
		logger::instance().write(lh.message()); \
		return R; \
	}
#else
#	define LOG_ERROR(X) \
	{ \
		log_helper lh(__FILE__, __LINE__); \
		lh.log_format X; \
		logger::instance().write(lh.message()); \
		printf(lh.message()); \
	}

#	define LOG_ERROR_EX(W, X) \
	if (!(W)) \
	{ \
		log_helper lh(__FILE__, __LINE__); \
		lh.log_format X; \
		logger::instance().write(lh.message()); \
		printf(lh.message()); \
	}

#	define LOG_ERROR_EX_RETURN(W, X, R) \
	if (!(W)) \
	{ \
		log_helper lh(__FILE__, __LINE__); \
		lh.log_format X; \
		logger::instance().write(lh.message()); \
		printf(lh.message()); \
		return R; \
	}

#endif /*NDEBUG*/
#pragma warning(pop)
#endif // __LOG_HPP__
