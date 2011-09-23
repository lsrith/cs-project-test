#include "Template.h"
#include <sstream>
#include <iostream>
using namespace std;

string Template::spaceMarker = " _' ";

Template::Template ()
{
}

Template::Template (Time time) {
	_time = time;
	_repeat_t = NONE;
	_forceAdd = false;
	_priority = SIMPLE;
	_index = 0;
}

Template::Template (TimePeriod period) {
	_period = period;
	_repeat_t = NONE;
	_forceAdd = false;
	_priority = SIMPLE;
	_index = 0;
}

Template::Template (string& str) {
	invConvert (str);
}

void Template::invConvert (string& str) {
	unsigned int i, size, start_pos, end_pos;
	int tempInt;
	Time tempTime;

	i = 0;
	start_pos = 0;
	end_pos = 0;
	size = spaceMarker.size ();

	while (i <= 16 && end_pos != string::npos) {
		stringstream strStream;
		end_pos = findPosition (str, spaceMarker, start_pos);
		if (end_pos == string::npos)
			strStream << str.substr (start_pos);
		else
			strStream << str.substr (start_pos, end_pos - start_pos);

		switch (i) {
		case 0:
			strStream >> tempInt;
			_index = tempInt;
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

		i++;
		start_pos = end_pos + size - 1;
	}
}

string Template::stringConvert () {
	ostringstream str;
	str << _index << spaceMarker << _time.get_date () << spaceMarker << _time.get_time ();
	str << spaceMarker << _period.get_start_time ().get_date () << spaceMarker << _period.get_end_time ().get_time ();
	str << spaceMarker << _period.get_end_time ().get_date () << spaceMarker << _period.get_end_time ().get_time ();
	str << spaceMarker << _name << spaceMarker << _venue << spaceMarker << _note;
	str << spaceMarker << _alert.get_date () << spaceMarker << _alert.get_time ();
	str << spaceMarker << repeatToInt (_repeat_t) << spaceMarker << _repeat.get_date () << spaceMarker << _repeat.get_time ();
	str << spaceMarker << _forceAdd << spaceMarker << priorityToInt (_priority);
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
/*
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
*/
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
	PriorityLevel priority;
	switch (num) {
	case 0:
		priority = HIGH;
		break;
	case 1:
		priority = IMPORTANT;
		break;
	default:
		priority = SIMPLE;
		break;
	}

	return priority;
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
	int num;
	switch (priority) {
	case HIGH:
		num = 0;
		break;
	case IMPORTANT:	
		num = 1;
		break;
	default:
		num = 2;
		break;
	}
	return num;
}

int Template::findPosition (string str, string subStr, int init_pos) {
	const int size1 = str.size ();
	const int size2 = subStr.size ();
	int i, j;
	bool found = false;
	for (i = init_pos; !found && (i < size1 - size2); i++) {
		found = true;
		for (j = 0; found && (j < size2); j++) {
			if (str[i + j] != subStr[j])
				found = false;
		}
	}

	if (found)
		return i;
	else
		return size1;
}