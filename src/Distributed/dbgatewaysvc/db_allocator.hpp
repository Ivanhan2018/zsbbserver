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

#ifndef __DB_ALLOCATOR_HPP__
#define __DB_ALLOCATOR_HPP__

#include "header.hpp"

template <class type> 
class db_allocator : private boost::noncopyable
{
	enum {MAX_BLOCK_NUM = 4096};
	struct node	{ node * next_; };
	struct free_list { node * head_; std::size_t offset_; };
		
public:
	explicit db_allocator();
	virtual ~db_allocator();
		
	type * malloc();
	void free(type *);
		
protected:
	node * head_;
	free_list list_;
};

template <class type> 
db_allocator<type>::db_allocator()
{
	head_ = 0;
	list_.head_ = 0;
	list_.offset_ = 0;
}

template <class type> 
db_allocator<type>::~db_allocator()
{
	head_ = 0;
	list_.offset_ = 0;
	void * p = (void *)list_.head_;
	
	while (p)
	{
		list_.head_ = list_.head_->next_;
		::free(p);
		p = list_.head_;
	}
}

template <class type> type *
db_allocator<type>::malloc()
{
	void * p;
	
	if (head_)
	{
		p = (void *)head_;
		head_ = head_->next_;
		return (type *)p;
	}
	
	if (!(list_.offset_ % MAX_BLOCK_NUM))
	{
		node * nd = (node *)::malloc((sizeof(node) +
			sizeof(type) * MAX_BLOCK_NUM));
		
		nd->next_ = list_.head_;
		list_.head_ = nd;
		list_.offset_ = 0;
	}

	p = (((char *)list_.head_) + (sizeof(node) + 
		sizeof(type) * (list_.offset_)));
	++list_.offset_;
	return (type *)p;
}

template <class type> void
db_allocator<type>::free(type * ptr)
{
	node * p = (node *)ptr;
	p->next_ = head_;
	head_ = p;
}

#endif // __DB_ALLOCATOR_HPP__
