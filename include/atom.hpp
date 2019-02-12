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

#ifndef __ATOM_HPP__
#define __ATOM_HPP__

class global_atom
{
public:
	global_atom(int serverid)
	{
		long path_len = ::GetModuleFileName(0, proc_name_, 128);
		while (proc_name_[path_len--] != '\\');
		::sprintf(proc_name_, "%s_%d", proc_name_ + path_len + 2, serverid);
		::strupr(proc_name_);

		::GlobalDeleteAtom(::GlobalFindAtom(proc_name_));
		::GlobalAddAtom(proc_name_);
	}

	virtual ~global_atom()
	{
		::GlobalDeleteAtom(::GlobalFindAtom(proc_name_));
	}
private:
	char proc_name_[128];
};

#endif // __ATOM_HPP__
