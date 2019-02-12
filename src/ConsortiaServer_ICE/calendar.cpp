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

#include "calendar.h"
#include <stdexcept>

#define DAY_SCALE 8
#define DAY_1 365
#define DAY_4 1461
#define DAY_100 36524
#define DAY_400 146097
#define SEC_DAY 86400
#define SEC_19000101 62135683200


namespace game_calendar
{
	static short month_days[2][12] = 
	{{31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}, 
	{31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}};
};

using namespace game_calendar;


calendar::calendar(__int64 second) :second_(second)
{
	if (!second) throw std::runtime_error("calendar::calendar second is 0");

	if (!QueryPerformanceFrequency(&frequency_))
		throw std::runtime_error("calendar::calendar QueryPerformanceFrequency is 0");

	if (!QueryPerformanceCounter(&tick_begin_))
		throw std::runtime_error("calendar::calendar QueryPerformanceCounter is 0");
}

bool calendar::get_game_calendar(int & year, int & month, int & day, int & hours, int & mins, int & secs)
{
	LARGE_INTEGER tick_curr;

	if (!QueryPerformanceCounter(&tick_curr))
		throw std::runtime_error("calendar::get_game_calendar QueryPerformanceCounter is 0");

	__int64 sec = second_ + ((tick_curr.QuadPart - tick_begin_.QuadPart) * 1000 / frequency_.QuadPart / 1000);

	unsigned int days = (unsigned int)((sec/* + SEC_19000101*/) * DAY_SCALE / SEC_DAY);
	unsigned long dayclock = (unsigned long)sec % SEC_DAY;

	dayclock += (8 * 3600);
	secs = dayclock % 60;
    mins = (dayclock % 3600) / 60;
    hours = dayclock / 3600;

	return days_to_date(days, year, month, day);
}

bool calendar::date_to_days(int year, int month, int day, unsigned int & days)
{
	days = 0;
	int leap_year = is_leap_year(year);

	if ((year >= 1) && (year <= 9999) && (month >= 1) && (month <= 12) && 
		(day >= 1) && (day <= month_days[leap_year][month]))
	{
		for (int i = 0; i < (month - 1); i++)
			day += month_days[leap_year][i];

		int j = year - 1;
		days = j * 365 + j / 4 - j / 100 + j / 400 + day;
		return true;
	}

	return false;
}

bool calendar::days_to_date(unsigned int days, int & year, int & month, int & day)
{
	int y, m, d, i, t, leap_year;

	if (days <= 0) return false;

	t = days;
	y = 1;
	m = d = i = leap_year = 0;

	--t;
	while (t >= DAY_400)
	{
		t -= DAY_400;
		y += 400;
	}

	i = t / DAY_100;
	d = t % DAY_100;

	if (i == 4)
	{
		--i;
		d -= DAY_100;
	}

	y += (i * 100);

	i = d / DAY_4;
	d %= DAY_4;

	y += (i * 4);

	i = d / DAY_1;
	d %= DAY_1;

	if (i == 4)
	{
		--i;
		d += DAY_1;
	}

	y += i;

	leap_year = is_leap_year(y);
	m = 1;

	while (1)
	{
		i = month_days[leap_year][m - 1];
		if (d < i) break;
		d -= i;
		++m;
	}

	year = y;
	month = m;
	day = d + 1;

	return true;
}

int calendar::is_leap_year(int year)
{
	if ((year % 4 == 0) && ((year % 100 != 0) || (year % 400 == 0)))
		return 1;
	return 0;
}




