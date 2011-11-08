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

TimePeriod::time_period_cmp TimePeriod::compare (TimePeriod timePeriod)
{
	time_period_cmp result;
	if (this->_end == timePeriod._start)
		result = RIGHT_AFTER;
	else if (timePeriod._end == this->_start)
		result = RIGHT_BEFORE;
	else if (this->_end < timePeriod._start)
		result = AFTER;
	else if (timePeriod._end < this->_start)
		result = BEFORE;
	else
		result = CLASH;
	return result;
}

bool TimePeriod::operator== (TimePeriod timePeriod)
{
	time_period_cmp cmp = compare (timePeriod);
	if (cmp == CLASH)
		return true;
	else
		return false;
}

bool TimePeriod::operator== (Time time)
{
	if (!(_end < time) && !(time < _start))
		return true;
	else
		return false;
}

bool TimePeriod::operator!= (TimePeriod timePeriod) {
	if (this->operator== (timePeriod))
		return false;
	else
		return true;
}

bool TimePeriod::operator!= (Time time) {
	if (this->operator== (time))
		return false;
	else
		return true;
}

bool TimePeriod::operator< (TimePeriod timePeriod)
{
	if (this->_end < timePeriod._start)
		return true;
	else
		return false;
}

bool TimePeriod::operator< (Time time)
{
	if (_end < time)
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

bool TimePeriod::operator> (Time time)
{
	if (_start > time)
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

bool TimePeriod::operator<= (Time time)
{
	if (_end == time)
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

bool TimePeriod::operator>= (Time time)
{
	if (_start == time)
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

string TimePeriod::string_time_period ()
{
	string str;
	string __to = " to ";

	if (_start == _end) {
		str = _start.string_date () + ' ' + _start.string_clock ();
	} else if (_start.get_date () == _end.get_date ()) {
		str = _start.string_date ();

		if (_start.get_clock () != Time::DFLT_CLOCK && _start.get_clock () != Time::INF_CLOCK)
			str += ' ' + _start.string_clock ();

		if (_end.get_clock () != Time::DFLT_CLOCK && _end.get_clock () != Time::INF_CLOCK)
			str += __to + _end.string_clock ();
	} else {
		str = _start.string_date ();
		if (_start.get_clock () != Time::DFLT_CLOCK && _start.get_clock () != Time::INF_CLOCK)
			str += ' ' + _start.string_clock ();

		str += __to + _end.string_date ();
		if (_end.get_clock () != Time::DFLT_CLOCK && _end.get_clock () != Time::INF_CLOCK)
			str += ' ' + _end.string_clock ();
	}
/*
	ostringstream str;
	str << "From: " << _start.string_date () << " at " << _start.string_clock ()
		<< "\nTo:   " << _end.string_date () << " at " << _end.string_clock ();
	return str.str ();
*/
	return str;
}