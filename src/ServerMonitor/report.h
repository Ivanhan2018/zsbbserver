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

#ifndef __REPORT_HPP__
#define __REPORT_HPP__

#if defined (WIN32)
#	define REPORT_API __stdcall
#else
#	define REPORT_API
#endif

#if defined(c_plusplus) || defined(__cplusplus)
extern "C" {
#endif

	int REPORT_API init(const char *, short);
	void REPORT_API fini();

	int REPORT_API log(const char *, int);

#if defined(c_plusplus) || defined(__cplusplus)
}
#endif /*c_plusplus __cplusplus*/

#endif /* __REPORT_HPP__ */
