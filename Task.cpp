#include "Task.h"
#include <sstream>
#include <iostream>
using namespace std;

string Task::spaceMarker = " _' ";

Task::Task ()
{
	_index = 0;
}

Task::Task (Time time) {
	_time = time;
	timeTask = true;
	repeat_t = NOREPEAT;
	forceAdd = false;
	priority = SIMPLE;
	_index = 0;
}

Task::Task (TimePeriod period) {
	_period = period;
	timeTask = false;
	repeat_t = NOREPEAT;
	forceAdd = false;
	priority = SIMPLE;
	_index = 0;
}

Task::Task (string& str) {
	invConvert (str);
}

void Task::invConvert (string& str) {
	int tempInt;
	string tempStr;
	Time tempTime;
	bool counterUpdated = false;
	unsigned int counter = 0;
	unsigned int start_pos = 0;
	unsigned int end_pos = 0;
	unsigned int size = spaceMarker.size ();

	while (counter <= 15 && end_pos != string::npos) {
		stringstream strStream;
		end_pos = findPosition (str, spaceMarker, start_pos);
		if (end_pos == string::npos)
			tempStr = str.substr (start_pos);
		else
			tempStr = str.substr (start_pos, end_pos - start_pos - 1);
		strStream << tempStr;

		if (timeTask && !counterUpdated && counter == 4)
			counter += 2;

		switch (counter) {
		case 0:
			strStream >> tempInt;
			_index = tempInt;
			break;
		case 1:
			strStream >> timeTask;
			break;
		case 6:
			name = tempStr;
			break;
		case 7:
			venue = tempStr;
			break;
		case 8:
			note = tempStr;
			break;
		case 9:
			strStream >> tempInt;
			alert.modify_date (tempInt);
			break;
		case 10:
			strStream >> tempInt;
			alert.modify_clock (tempInt);
		case 11:
			strStream >> tempInt;
			repeat_t = intToRepeat (tempInt);
			break;
		case 12:
			strStream >> tempInt;
			repeat.modify_date (tempInt);
			break;
		case 13:
			strStream >> tempInt;
			repeat.modify_clock (tempInt);
			break;
		case 14:
			strStream >> forceAdd;
			break;
		case 15:
			strStream >> tempInt;
			priority = intToPriority (tempInt);
			break;
		default:
			break;
		}

		if (timeTask) {
			switch (counter) {
			case 2:
				strStream >> tempInt;
				_time.modify_date (tempInt);
				break;
			case 3:
				strStream >> tempInt;
				_time.modify_clock (tempInt);
				break;
			default:
				break;
			}
		} else {
			switch (counter) {
			case 2:
				strStream >> tempInt;
				tempTime.modify_date (tempInt);
				break;
			case 3:
				strStream >> tempInt;
				tempTime.modify_clock (tempInt);
				_period.modify_start_time (tempTime);
				break;
			case 4:
				strStream >> tempInt;
				tempTime.modify_date (tempInt);
				break;
			case 5:
				strStream >> tempInt;
				tempTime.modify_clock (tempInt);
				_period.modify_end_time (tempTime);
				break;
			default:
				break;
			}
		}

		counter++;
		start_pos = end_pos + size - 1;
	}
}

string Task::stringConvert () {
	ostringstream str;
	
	str << _index << spaceMarker << timeTask;
	if (timeTask) {
		str << spaceMarker << _time.get_date () << spaceMarker << _time.get_clock ();
	} else {
		str << spaceMarker << _period.get_start_time ().get_date () << spaceMarker << _period.get_start_time ().get_clock ();
		str << spaceMarker << _period.get_end_time ().get_date () << spaceMarker << _period.get_end_time ().get_clock ();
	}
	str << spaceMarker << name << spaceMarker << venue << spaceMarker << note;
	str << spaceMarker << alert.get_date () << spaceMarker << alert.get_clock ();
	str << spaceMarker << repeat_t << spaceMarker << repeat.get_date () << spaceMarker << repeat.get_clock ();
	str << spaceMarker << forceAdd << spaceMarker << priority;
	
	return str.str ();
}

Time Task::get_time () {
	return _time;
}

TimePeriod Task::get_period () {
	return _period;
}

unsigned int Task::get_index () {
	return _index;
}

RepeatType Task::intToRepeat (int num) {
	RepeatType repeat;
	switch (num) {
	case 0:
		repeat = HOUR;
		break;
	case 1:
		repeat = DAY;
		break;
	case 2:
		repeat = WEEK;
		break;
	case 3:
		repeat = FORTNIGHT;
		break;
	case 4:
		repeat = MONTH;
		break;
	case 5:
		repeat = COSTOM;
		break;
	default:
		repeat = NOREPEAT;
		break;
	}

	return repeat;
}

PriorityLevel Task::intToPriority (int num) {
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

int Task::repeatToInt (RepeatType repeat) {
	switch (repeat) {
	case HOUR:		return 0;
	case DAY:		return 1;
	case WEEK:		return 2;
	case FORTNIGHT:	return 3;
	case MONTH:		return 4;
	case COSTOM:	return 5;
	default:		return 6;
	}
}

int Task::priorityToInt (PriorityLevel priority) {
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

int Task::findPosition (string str, string subStr, int init_pos) {
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