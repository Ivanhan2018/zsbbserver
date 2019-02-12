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

#ifndef __PROACTOR_POOL_HPP__
#define __PROACTOR_POOL_HPP__

#include "header.hpp"

class proactor_pool : private boost::noncopyable
{
public:
	explicit proactor_pool(std::size_t);

	void run_event_loop();
	void stop_event_loop();

	boost::asio::io_service& get_io_service();

private:
	typedef boost::shared_ptr<boost::
		asio::io_service> io_service_ptr;
	typedef boost::shared_ptr<boost::
		asio::io_service::work> work_ptr;

	std::vector<io_service_ptr> io_services_;
	std::vector<work_ptr> work_;
	std::size_t next_io_service_;
};

#endif // __PROACTOR_POOL_HPP__
