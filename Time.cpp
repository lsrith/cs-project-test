#include "Time.h"
#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>
using namespace std;

const string INVALID_DATE = "The date is invalid!";
const string INVALID_TIME = "The time is invalid!";
const clk_t INF_TIME = 2359;				//infinite time
const date_t INF_DATE = 31129999;			//infinite date

Time::Time ()
{
	_time = INF_TIME;
	_date = INF_DATE;
}

Time::Time (date_t date, clk_t time)
{
	if (_valid_date (date))
		_date = date;
	else
		_date = INF_DATE;

	if (_valid_time (time))
		_time = time;
	else
		_time = INF_TIME;
}

bool _valid_date (date_t date)
{
	if (date == INF_DATE)
		return true;

	bool valid_date = true;
	date_t max_day, day, mth, year;
	day = date / 1000000;
	mth = (date % 1000000) / 10000;
	year = (date % 10000);

	if (day >= 1 && (mth >= 1 && mth <= 12) && (year >= 1950 && year <= 2100))
	{
		switch (mth)
		{
			case 1:
			case 3:
			case 5:
			case 7:
			case 8:
			case 10:
			case 12:
				max_day = 31;
				break;
			case 4:
			case 6:
			case 9:
			case 11:
				max_day = 30;
				break;
			case 2:
				if (LeapYear (year))
					max_day = 29;
				else
					max_day = 28;
				break;
			default:
				max_day = 0;
		}

		if (day <= max_day)
			valid_date = true;
		else
			valid_date = false;
	}
	else
		valid_date = false;
		
	return valid_date;
}

bool _valid_time (clk_t time)
{
	if (time == INF_TIME)
		return true;

	clk_t hour, min;
	hour = time / 100;
	min = time % 100;

	if ((hour >= 00 && hour <= 23) && (min >= 00 && min <= 59))
		return true;
	else
		return false;
}

void Time::modify_date (date_t new_date)
{
	if (_valid_date (new_date))
		_date = new_date;
	else
		throw (INVALID_DATE);
}

void Time::modify_time (clk_t new_time)
{
	if (_valid_time (new_time))
		_time = new_time;
	else
		throw (INVALID_TIME);
}

string Time::display_date ()
{
	ostringstream str;
	str << setw (2) << setfill ('0') << _date / 1000000 << "/" << setw (2) <<
		setfill ('0') << (_date % 1000000) / 10000 << "/" << setw(2) <<
		setfill ('0') <<_date % 10000;
	return str.str ();
}

string Time::display_time ()
{
	ostringstream str;
	str << setw(2) << setfill ('0') << _time / 100 << ":" << setw(2) <<
		setfill ('0') << _time % 100;
	return str.str ();
}

bool isAfter (date_t start_date, date_t end_date)
{
	bool _isAfter;
	
	if (start_date % 10000 < end_date % 10000)
		_isAfter = true;
	else if (start_date % 10000 > end_date % 10000)
		_isAfter = false;
	else
	{
		if (start_date % 1000000 < end_date % 1000000)
			_isAfter = true;
		else if (start_date % 1000000 > end_date % 1000000)
			_isAfter = false;
		else
		{
			if (start_date < end_date)
				_isAfter = true;
			else
				_isAfter = false;
		}
	}

	return _isAfter;
}

bool Time::operator> (Time time)
{
	bool isSmall;
	if (this->_date == time._date)
	{
		if (time._time < this->_time)
			isSmall = true;
		else
			isSmall = false;
	}
	else if (isAfter(time._date, this->_date))
		isSmall = true;
	else
		isSmall = false;

	return isSmall;	
}

bool Time::operator== (Time time)
{
	if (this->_date == time._date && this->_time == time._time)
		return true;
	else
		return false;
}

bool Time::operator< (Time time)
{
	bool isSmall;
	if (this->_date == time._date)
	{
		if (time._time > this->_time)
			isSmall = true;
		else
			isSmall = false;
	}
	else if (isAfter(this->_date, time._date))
		isSmall = true;
	else
		isSmall = false;

	return isSmall;
}

int Time::operator- (Time time)
{
	int duration;

	if (*this == time)
		duration = 0;
	else
		duration = this->convert_to_mins () - time.convert_to_mins ();

	if (duration < 0)
		duration = 0 - duration;

	return duration;
}

int Time::convert_to_mins ()
{
	short int hour = _time / 100;
	short int min = _time % 100;
	short int day = _date / 1000000;
	short int mnth = (_date % 1000000) / 10000;
	short int year = _date % 10000;
	unsigned int numMins, numDays;

	numDays = day;
	switch (mnth)
	{
		case 12:	numDays += 30;
		case 11:	numDays += 31;
		case 10:	numDays += 30;
		case 9:		numDays += 31;
		case 8:		numDays += 31;
		case 7:		numDays += 30;
		case 6:		numDays += 31;
		case 5:		numDays += 30;
		case 4:		numDays += 31;
		case 3:		if (LeapYear (year))
						numDays += 29;
					else
						numDays += 28;
		case 2:		numDays += 31;
		case 1:		break;
		default:	break;
	}

	for (int i = 0; i < year; i++)
		if (LeapYear (i))
			numDays += 366;
		else
			numDays += 365;

	numMins = ((numDays * 24) + hour) * 60 + min;

	return numMins;
}

bool LeapYear (int year)
{
	if (year % 400 == 0 || (year % 4 == 0 && year % 100 != 0))
		return true;
	else
		return false;
}
