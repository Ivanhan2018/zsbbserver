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

#ifndef __DB_HANDLER_HPP__
#define __DB_HANDLER_HPP__

#include "task_base_ex.hpp"

class connection;

time_t __filetime = 0;

class db_handler : public task_base_ex<connection, char *>
{
public:
	virtual bool start(const char *, bool){ return true; }
	virtual ~db_handler(){};
};

int file_exists(const char * file)
{
	HANDLE h;
	WIN32_FIND_DATA f;

	if (!file) return -1;

	h = FindFirstFile(file, &f);
	if (h != INVALID_HANDLE_VALUE)
	{
		FindClose(h);
		return 0;
	}

	return -1;
}

HANDLE file_open(const char * filename)
{
	HANDLE fd;
	SECURITY_ATTRIBUTES sa;

	if (!filename) return INVALID_HANDLE_VALUE;

	if (file_exists(filename) == -1)
		return INVALID_HANDLE_VALUE;

	sa.nLength = sizeof(sa);
	sa.lpSecurityDescriptor = 0;
	sa.bInheritHandle = (GENERIC_READ | GENERIC_WRITE & 0x0080) == 0;

	fd = CreateFile(filename, 
		GENERIC_READ, 0, &sa,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	return fd;
}

HANDLE file_create(const char * filename)
{
	HANDLE fd;
	SECURITY_ATTRIBUTES sa;

	if (!filename) return INVALID_HANDLE_VALUE;

	if (file_exists(filename) != -1)
	{
		DeleteFile(filename);
		Sleep(1);
	}

	sa.nLength = sizeof(sa);
	sa.lpSecurityDescriptor = 0;
	sa.bInheritHandle = (GENERIC_READ | GENERIC_WRITE & 0x0080) == 0;

	fd = CreateFile(filename, 
		GENERIC_READ | GENERIC_WRITE, 0, &sa,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

	return fd;
}

void file_close(HANDLE fd)
{
	if (fd == INVALID_HANDLE_VALUE) return;
	CloseHandle(fd);
}

int file_write(HANDLE fd, char * buf, int len)
{
	unsigned long t;

	if (!buf || !len) return -1;
	if (fd == INVALID_HANDLE_VALUE) return -1;

	if (WriteFile((HANDLE)fd, buf, len, &t, 0))
		return t;

	return -1;
}

int file_read(HANDLE fd, char * buf, int len)
{
	unsigned long t;

	if (!buf || !len) return -1;
	if (fd == INVALID_HANDLE_VALUE) return -1;

	if (ReadFile((HANDLE)fd, buf, len, &t, 0))
		return t;

	return -1;
}

unsigned long file_length(HANDLE fd)
{
	unsigned long b, e;

	if (fd == INVALID_HANDLE_VALUE) return 0;

	b = SetFilePointer((HANDLE)fd, 0, 0, FILE_BEGIN);
	e = SetFilePointer((HANDLE)fd, 0, 0, FILE_END);
	SetFilePointer((HANDLE)fd, 0, 0, FILE_BEGIN);
	return e - b;
}

#endif // __DB_HANDLER_HPP__
