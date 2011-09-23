/*
#include "Template.h"
#include <sstream>
#include <iomanip>
using namespace std;

Template::Template (Time time) {
	_time = time;
	_repeat_t = NONE;
	_forceAdd = false;
	_priority = SIMPLE;
}

Template::Template (TimePeriod period) {
	_period = period;
	_repeat_t = NONE;
	_forceAdd = false;
	_priority = SIMPLE;
}

Template::Template (string& str) {
	unsigned int i, start_pos, end_pos;
	string spMarker, tempStr;
	int tempInt;
	Time tempTime;
	stringstream strStream;

	i = 0;
	start_pos = 0;
	end_pos = 0;

	while (end_pos != string::npos) {
		start_pos = end_pos + spMarker.size ();
		spMarker = spaceMarker (i);
		end_pos = str.find (spMarker);
		strStream << str.substr (start_pos, end_pos - start_pos);

		switch (i) {
		case 0:
			strStream >> _index;
			break;
		case 1:
			strStream >> tempInt;
			_time.modify_date (tempInt);
			break;
		case 2:
			strStream >> tempInt;
			_time.modify_time (tempInt);
			break;
		case 3:
			strStream >> tempInt;
			tempTime.modify_date (tempInt);
			break;
		case 4:
			strStream >> tempInt;
			tempTime.modify_time (tempInt);
			_period.modify_start_time (tempTime);
			break;
		case 5:
			strStream >> tempInt;
			tempTime.modify_date (tempInt);
			break;
		case 6:
			strStream >> tempInt;
			tempTime.modify_time (tempInt);
			_period.modify_end_time (tempTime);
			break;
		case 7:
			strStream >> _name;
			break;
		case 8:
			strStream >> _venue;
			break;
		case 9:
			strStream >> _note;
			break;
		case 10:
			strStream >> tempInt;
			_alert.modify_date (tempInt);
			break;
		case 11:
			strStream >> tempInt;
			_alert.modify_time (tempInt);
		case 12:
			strStream >> tempInt;
			_repeat_t = intToRepeat (tempInt);
			break;
		case 13:
			strStream >> tempInt;
			_repeat.modify_date (tempInt);
			break;
		case 14:
			strStream >> tempInt;
			_repeat.modify_time (tempInt);
			break;
		case 15:
			strStream >> _forceAdd;
			break;
		case 16:
			strStream >> tempInt;
			_priority = intToPriority (tempInt);
			break;
		default:
			break;
		}
	}
}

string Template::convert () {
	ostringstream str;
	str << _index << spaceMarker (0) << _time.get_date () << spaceMarker (1) << _time.get_time ();
	str << spaceMarker (2) << _period.get_start_time ().get_date () << spaceMarker (3) << _period.get_end_time ().get_time ();
	str << spaceMarker (4) << _period.get_end_time ().get_date () << spaceMarker (5) << _period.get_end_time ().get_time ();
	str << spaceMarker (6) << _name << spaceMarker (7) << _venue << spaceMarker (8) << _note;
	str << spaceMarker (9) << _alert.get_date () << spaceMarker (10) << _alert.get_time ();
	str << spaceMarker (11) << repeatToInt (_repeat_t) << spaceMarker (12) << _repeat.get_date () << spaceMarker (13) << _repeat.get_time ();
	str << spaceMarker (14) << _forceAdd << spaceMarker (15) << priorityToInt (_priority);
	return str.str ();
}

Time Template::get_time () {
	return _time;
}

TimePeriod Template::get_period () {
	return _period;
}

unsigned int Template::get_index () {
	return _index;
}

string Template::spaceMarker (int num)
{
	string str;
	switch (num) {
	case 0:
		str = " _td' ";
		break;
	case 1:
		str = " _tc' ";
		break;
	case 2:
		str = " _psd' ";
		break;
	case 3:
		str = " _pst' ";
		break;
	case 4:
		str = " _ped' ";
		break;
	case 5:
		str = " _pet' ";
		break;
	case 6:
		str = " _n' ";
		break;
	case 7:
		str = " _v' ";
		break;
	case 8:
		str = " _n' ";
		break;
	case 9:
		str = " _ad' ";
		break;
	case 10:
		str = " _at' ";
		break;
	case 11:
		str = " _r' ";
		break;
	case 12:
		str = " _rd' ";
		break;
	case 13:
		str = " _rt' ";
		break;
	case 14:
		str = " _p' ";
		break;
	default:
		str = " _' ";
		break;
	}

	return str;
}

RepeatType Template::intToRepeat (int num) {
	switch (num) {
	case 0:		return HOUR;
	case 1:		return DAY;
	case 2:		return WEEK;
	case 3:		return FORTNIGHT;
	case 4:		return MONTH;
	case 5:		return COSTOM;
	case 6:		return NONE;
	}
}

PriorityLevel Template::intToPriority (int num) {
	switch (num) {
	case 0:		return HIGH;
	case 1:		return IMPORTANT;
	case 2:		return SIMPLE;
	}
}

int Template::repeatToInt (RepeatType repeat) {
	switch (repeat) {
	case HOUR:		return 0;
	case DAY:		return 1;
	case WEEK:		return 2;
	case FORTNIGHT:	return 3;
	case MONTH:		return 4;
	case COSTOM:	return 5;
	case NONE:		return 6;
	}
}

int Template::priorityToInt (PriorityLevel priority) {
	switch (priority) {
	case HIGH:		return 0;
	case IMPORTANT:	return 1;
	case SIMPLE:	return 2;
	}
}
*/