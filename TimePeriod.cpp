//TimePeriod.cpp
#include "TimePeriod.h"
#include <string>
#include <sstream>
#include <iomanip>
using namespace std;

string TimePeriod::MSG_START_AFTER_END = "The starting time must be before the ending time.";

TimePeriod::TimePeriod ()
{
}

TimePeriod::TimePeriod (Time start_time, Time end_time)
{
	if (start_time < end_time)
	{
		_start = start_time;
		_end = end_time;
	}
	else
	{
		_start = end_time;
		_end = start_time;
	}
}

void TimePeriod::modify_start_time (Time start)
{
	_start = start;
}

void TimePeriod::modify_end_time (Time end)
{
	_end = end;
}

Time TimePeriod::get_start_time ()
{
	return _start;
}

Time TimePeriod::get_end_time ()
{
	return _end;
}

time_period_cmp TimePeriod::compare (TimePeriod timePeriod)
{
	if (this->_end == timePeriod._start)
		return RIGHT_AFTER;
	else if (timePeriod._end == this->_start)
		return RIGHT_BEFORE;
	else if (this->_end < timePeriod._start)
		return AFTER;
	else if (timePeriod._end < this->_start)
		return BEFORE;
	else
		return CLASH;
}

bool TimePeriod::operator== (TimePeriod timePeriod)
{
	if (timePeriod._start < this->_end && this->_start < timePeriod._end)
		return true;
	else
		return false;
}

bool TimePeriod::operator< (TimePeriod timePeriod)
{
	if (this->_end < timePeriod._start)
		return true;
	else
		return false;
}

bool TimePeriod::operator> (TimePeriod timePeriod)
{
	if (timePeriod._end < this->_start)
		return true;
	else
		return false;
}

bool TimePeriod::operator<= (TimePeriod timePeriod)
{
	if (this->_end == timePeriod._start)
		return true;
	else
		return false;
}

bool TimePeriod::operator>= (TimePeriod timePeriod)
{
	if (this->_start == timePeriod._end)
		return true;
	else
		return false;
}

void TimePeriod::operator+ (TimePeriod timePeriod)
{
	if (_start > timePeriod._start)
		_start = timePeriod._start;

	if (_end < timePeriod._end)
		_end = timePeriod._end;
}

int TimePeriod::operator- (TimePeriod timePeriod)
{
	int duration;

	if (*this <= timePeriod || *this >= timePeriod)
		duration = 0;
	else if (*this == timePeriod)
		duration = -1;
	else if (*this < timePeriod)
		duration = timePeriod._start - this->_end;
	else if (*this > timePeriod)
		duration = this->_start - timePeriod._end;
	else
		duration = -2;			//this option occur unless there is an error

	return duration;
}

string TimePeriod::display_time_period ()
{
	ostringstream str;
	str << "From: " << _start.display_date () << " at " << _start.display_time ()
		<< "\nTo:   " << _end.display_date () << " at " << _end.display_time ();
	return str.str ();
}