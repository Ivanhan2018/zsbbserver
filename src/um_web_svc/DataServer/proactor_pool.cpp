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

#include "proactor_pool.hpp"

proactor_pool::proactor_pool(std::size_t pool_size) : 
next_io_service_(0)
{
	logic_check(pool_size, ("pool_size=%d", pool_size));

	for (std::size_t i = 0; i < pool_size; ++i)
	{
		io_service_ptr io_service(new boost::asio::io_service);
		io_services_.push_back(io_service);

		work_ptr work(new boost::asio::io_service::work(*io_service));
		work_.push_back(work);
	}
}

void proactor_pool::run_event_loop()
{
	std::vector<boost::shared_ptr<boost::thread> > threads;
	for (std::size_t i = 0; i < io_services_.size(); ++i)
	{
		boost::shared_ptr<boost::thread> thread(new boost::thread(
			boost::bind(&boost::asio::io_service::run, io_services_[i])));

		threads.push_back(thread);
	}

	for (std::size_t i = 0; i < threads.size(); ++i)
		threads[i]->join();
}

void proactor_pool::stop_event_loop()
{
	for (std::size_t i = 0; i < io_services_.size(); ++i)
		io_services_[i]->stop();
}

boost::asio::io_service& proactor_pool::get_io_service()
{
	boost::asio::io_service& io_service = *io_services_[next_io_service_];
	++next_io_service_;

	if (next_io_service_ == io_services_.size())
		next_io_service_ = 0;

	return io_service;
}

