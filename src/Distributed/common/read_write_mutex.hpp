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

#ifndef __READ_WRITE_MUTEX_HPP__
#define __READ_WRITE_MUTEX_HPP__

#include "header.hpp"

class read_write_mutex : private boost::noncopyable
{
public:
	read_write_mutex() : readers_(0), pending_writers_(0), current_writer_(false) 
	{}

	class scoped_read_lock : private boost::noncopyable
	{
	public:
		scoped_read_lock(read_write_mutex& rw_lock) :
		  rw_lock_(rw_lock)
		  {
			  rw_lock_.acquire_read_lock();
		  }

		  ~scoped_read_lock()
		  {
			  rw_lock_.release_read_lock();
		  }

	private:
		read_write_mutex & rw_lock_;
	};

	class scoped_write_lock : private boost::noncopyable
	{
	public:
		scoped_write_lock(read_write_mutex& rw_lock) :
		  rw_lock_(rw_lock)
		  {
			  rw_lock_.acquire_write_lock();
		  }

		  ~scoped_write_lock()
		  {
			  rw_lock_.release_write_lock();
		  }

	private:
		read_write_mutex& rw_lock_;
	};

private:
	void acquire_read_lock()
	{
		boost::asio::detail::mutex::scoped_lock lock(mutex_);

		while(pending_writers_ != 0 || current_writer_)
		{
			writer_finished_.wait(lock);
		}

		++readers_;
	}

	void release_read_lock()
	{
		boost::asio::detail::mutex::scoped_lock lock(mutex_);
		--readers_;

		if(readers_ == 0)
		{
			noreaders_.notify_all();
		}
	}

	void acquire_write_lock()
	{
		boost::asio::detail::mutex::scoped_lock lock(mutex_);
		++pending_writers_;

		while(readers_ > 0)
		{
			noreaders_.wait(lock);
		}

		while(current_writer_)
		{
			writer_finished_.wait(lock);
		}

		--pending_writers_;
		current_writer_ = true;
	}

	void release_write_lock()
	{        
		boost::asio::detail::mutex::scoped_lock lock(mutex_);
		current_writer_ = false;
		writer_finished_.notify_all();
	}

private:
	boost::asio::detail::mutex mutex_;

	unsigned int readers_;
	boost::condition noreaders_;

	unsigned int pending_writers_;
	bool current_writer_;
	boost::condition writer_finished_;

};

#endif // __READ_WRITE_MUTEX_HPP__

