#include "Task.h"
#include <sstream>
using namespace std;

string Task::spaceMarker = " _' ";

Task::Task ()
{
	_index = 0;
	repeat = 0;
	timeTask = true;
}

Task::Task (Time time) {
	_time = time;
	timeTask = true;
	repeat = 0;
	_index = 0;
}

Task::Task (TimePeriod period) {
	if (period.get_start_time ().get_date () == Time::DFLT_DATE && period.get_end_time ().get_date () == Time::DFLT_DATE) {
		timeTask = true;
	} else if (period.get_start_time ().get_date () == Time::DFLT_DATE) {
		timeTask = true;
		_time = period.get_end_time ();
	} else if (period.get_end_time ().get_date () == Time::DFLT_DATE) {
		timeTask = true;
		_time = period.get_start_time ();
	} else if (period.get_start_time ().get_date () == Time::INF_DATE && period.get_end_time ().get_date () == Time::INF_DATE) {
		timeTask = true;
		_time.modify_date (Time::INF_DATE);
		_time.modify_clock (period.get_start_time ().get_clock ());
	} else {
		timeTask = false;
		_period = period;
	}
	repeat = 0;
	_index = 0;
}

Task::Task (string& str) {
	invConvert (str);
}

void Task::invConvert (string& str) {
	Time::date_t date;
	Time::clk_t clock;

	stringstream strStream;
	strStream << str;
	strStream >> _index >> timeTask;
	if (timeTask) {
		strStream >> date >> clock;
		_time.modify_date (date);
		_time.modify_clock (clock);
	} else {
		strStream >> date >> clock;
		_time.modify_date (date);
		_time.modify_clock (clock);
		_period.modify_start_time (_time);
		strStream >> date >> clock;
		_time.modify_date (date);
		_time.modify_clock (clock);
		_period.modify_end_time (_time);
		_time.modify_date (Time::DFLT_DATE);
		_time.modify_clock (Time::DFLT_CLOCK);
	}

	strStream >> date >> clock >> repeat;
	alert.modify_date (date);
	alert.modify_clock (clock);
	strStream >> date >> clock;
	_time.modify_date (date);
	_time.modify_clock (clock);
	r_period.modify_start_time (_time);
	strStream >> date >> clock;
	_time.modify_date (date);
	_time.modify_clock (clock);
	r_period.modify_end_time (_time);
	_time.modify_date (Time::DFLT_DATE);
	_time.modify_clock (Time::DFLT_CLOCK);

	note = strStream.str ();
	int pos = note.find (spaceMarker, 0);
	note.erase (0, pos + spaceMarker.size ());
	pos = note.find (spaceMarker, 0);
	venue = note.substr (0, pos);
	note.erase (0, pos + spaceMarker.size ());
}

string Task::stringConvert () {
	ostringstream str;
	
	str << _index << " " << timeTask;
	if (timeTask) {
		str << " " << _time.get_date () << " " << _time.get_clock ();
	} else {
		str << " " << _period.get_start_time ().get_date () << " " << _period.get_start_time ().get_clock ();
		str << " " << _period.get_end_time ().get_date () << " " << _period.get_end_time ().get_clock ();
	}
	str << " " << alert.get_date () << " " << alert.get_clock () << " " << repeat;
	str << " " << r_period.get_start_time ().get_date () << " " << r_period.get_start_time ().get_clock ();
	str << " " << r_period.get_end_time ().get_date () << " " << r_period.get_end_time ().get_clock ();
	str << spaceMarker << venue << spaceMarker << note;
	
	return str.str ();
}

void Task::modify_time (Time newTime) {
	Time dfltTime;
	if (newTime != dfltTime) {
		timeTask = true;
		_time = newTime;
		TimePeriod period;
		_period = period;
	}
}

void Task::modify_period (TimePeriod period) {
	if (period.get_start_time ().get_date () == Time::DFLT_DATE && period.get_end_time ().get_date () == Time::DFLT_DATE) {
		timeTask = true;
	} else if (period.get_start_time ().get_date () == Time::DFLT_DATE) {
		timeTask = true;
		_time = period.get_end_time ();
	} else if (period.get_end_time ().get_date () == Time::DFLT_DATE) {
		timeTask = true;
		_time = period.get_start_time ();
	} else if (period.get_start_time ().get_date () == Time::INF_DATE && period.get_end_time ().get_date () == Time::INF_DATE) {
		timeTask = true;
		_time.modify_date (Time::INF_DATE);
		_time.modify_clock (period.get_start_time ().get_clock ());
	} else {
		timeTask = false;
		_period = period;
	}
}

Time Task::get_time () const {
	return _time;
}

TimePeriod Task::get_period () const {
	return _period;
}

unsigned int Task::get_index () const {
	return _index;
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

bool Task::compareByStartTime (Task task1, Task task2) {
	if (task1.timeTask && task2.timeTask)
		return task1._time < task2._time;
	else if (task1.timeTask && !task2.timeTask)
		return task1._time < task2._period.get_start_time ();
	else if (!task1.timeTask && task2.timeTask)
		return task1._period.get_start_time () < task2._time;
	else
		return task1._period.get_start_time () < task2._period.get_start_time ();
}

bool Task::compareByEndTime (Task task1, Task task2) {
	if (task1.timeTask && task2.timeTask)
		return task1._time < task2._time;
	else if (task1.timeTask && !task2.timeTask)
		return false;
	else if (!task1.timeTask && task2.timeTask)
		return true;
	else
		return task1._period.get_end_time () < task2._period.get_end_time ();
}

bool Task::compareByAlert (Task task1, Task task2) {
	return task1.alert < task2.alert;
}

bool Task::compareByVenue (Task task1, Task task2) {
	string venue1 = task1.venue;
	string venue2 = task2.venue;

	if (venue1.size () != venue2.size ()) {
		return venue1.size () < venue2.size ();
	} else {
		for (int i = 0; i < venue1.size (); i++) {
			venue1[i] = tolower (venue1[i]);
			venue2[i] = tolower (venue2[i]);
		}
		return venue1 < venue2;
	}
}