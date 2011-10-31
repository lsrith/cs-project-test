#include "ExecuteCmd.h"
#include <sstream>
#include <vector>
#include <list>
#include <cassert>
#include <iostream>
using namespace std;

ExecuteCmd::ExecuteCmd () {
}

ExecuteCmd::~ExecuteCmd () {
}

void ExecuteCmd::updateInput (string& input) {
	_input = input;
};

string ExecuteCmd::get_input () {
	return _input;
}

string ExecuteCmd::result () {
	return _result;
}

bool ExecuteCmd::execute () {
	return true;
}

//-----------------------------------------------------------------

string Add::MSG_CLASH = " is clashed with the following:\n";
string Add::MSG_ADDED = " is added.";

Add::Add (vector<cmd_pair> validCmd, ToDoMngr* toDoMngr) {
	assert (toDoMngr != NULL);
	_validCmd = validCmd;
	_toDoMngr = toDoMngr;
	_sequence = NULL;
	_dataInput = NULL;
	_cmdInput = NULL;
	_splitedInput = NULL;
}

Add::~Add () {
	clear ();
}

bool Add::execute () {
	bool done;
	bool force = false;
	
	splitInput (_input);

	if (!_sequence->empty () && _sequence->front () == CMD && _cmdInput->front () == CADD) {
		pop ();
	} else {
		done = true;
	}

	if (_sequence->empty ()) {
		return false;
		_flagError = DATA;
	}

	if (_sequence->front () == CMD && _cmdInput->front () == CFORCE) {
		pop ();
		force = true;
	}
	Task task = get_task ();
	if (_flagError == NONE) {
		list<Task> taskList = _toDoMngr->add (task, force);
		if (!force && !taskList.empty ()) {
			_result = ToDoMngr::view (task) + MSG_CLASH + ToDoMngr::view (taskList);
			insertBreakPoint ();
			done = false;
		} else if (force && !taskList.empty ()) {
			assert (force == taskList.empty ());
		} else {
			_result = ToDoMngr::view (task) + MSG_ADDED;
			done = true;
		}
	} else {
		done = false;
	}

	_input = getLeftOverInput ();
	return done;
}

void Add::insertBreakPoint () {
	list<string>* temp = new list<string>;

	temp->push_back (_splitedInput->front ());
	_splitedInput->pop_front ();
	temp->push_back (BREAK);
	temp->splice (temp->end (), *_splitedInput);
	
	delete _splitedInput;
	_splitedInput = temp;
}

//-----------------------------------------------------------------

string Edit::MSG_EDITED = " is edited.";

Edit::Edit (vector<cmd_pair> validCmd, ToDoMngr* toDoMngr) {
	assert (toDoMngr != NULL);
	_validCmd = validCmd;
	_toDoMngr = toDoMngr;
	_sequence = NULL;
	_dataInput = NULL;
	_cmdInput = NULL;
	_splitedInput = NULL;
}

Edit::~Edit () {
	clear ();
}

bool Edit::execute () {
	bool done;
	bool force = false;

	splitInput (_input);

	if (!_sequence->empty () && _sequence->front () == CMD && _cmdInput->front () == CEDIT) {
		pop ();
	} else {
		done = true;
	}

	if (_sequence->empty ()) {
		return false;
		_flagError = DATA;
	}

	if (_sequence->front () == CMD && _cmdInput->front () == CFORCE) {
		pop ();
		force = true;
	}

	int taskId;
	if (!_sequence->empty () && _sequence->front () == DATA) {
		taskId = convertToInt (_dataInput->front ());
		if (taskId < 1) {
			_flagError = DATA;
		}
	} else {
		_flagError = DATA;
		return false;
	}

	Task task = get_task ();
	if (_flagError == NONE) {
		list<Task> taskList = _toDoMngr->edit (taskId, task, force);
		if (!force && !taskList.empty ()) {
			_result = ToDoMngr::view (task) + Add::MSG_CLASH + ToDoMngr::view (taskList);
			insertBreakPoint ();
			done = false;
		} else {
			_result = ToDoMngr::view (task) + MSG_EDITED;
			done = true;
		}
	} else {
		done = false;
	}

	_input = getLeftOverInput ();
	return done;
}

void Edit::insertBreakPoint () {
	list<string>* temp = new list<string>;

	temp->push_back (_splitedInput->front ());
	_splitedInput->pop_front ();
	temp->push_back (BREAK);
	temp->splice (temp->end (), *_splitedInput);
	
	delete _splitedInput;
	_splitedInput = temp;
}

//----------------------------------------------------------------

string View::MSG_NO_NEXT = "This is the last!";
string View::MSG_NO_PREV = "This is the first!";
string View::MSG_WRONG_ID = "Invalid index entered!";

View::View (vector<cmd_pair> validCmd, ToDoMngr* toDoMngr) {
	assert (toDoMngr != NULL);
	_validCmd = validCmd;
	_toDoMngr = toDoMngr;
	_taskId = 0;
	_first = false;
	_last = false;
	_viewType = DAILY;
	_traverse = true;
	_activeListAccessible = false;
	_sequence = NULL;
	_dataInput = NULL;
	_cmdInput = NULL;
	_splitedInput = NULL;
}

View::~View () {
	clear ();
}

bool View::execute () {
	bool done;
	
	splitInput (_input);

	if (!_sequence->empty () && _sequence->front () == CMD && _cmdInput->front () == CVIEW) {
		pop ();
	} else {
		done = true;
	}

	if (!_sequence->empty () && _sequence->front () == CMD) {
		switch (_cmdInput->front ()) {
		case CDAY:
			_viewType = DAILY;
			pop ();
			break;
		case CWEEK:
			_viewType = WEEKLY;	
			pop ();
			break;
		case CMONTH:
			_viewType = MONTHLY;
			pop ();
			break;
		default:
			break;
		}
	}

	if (_sequence->empty ()) {
		TimePeriod period;
		_activeListAccessible = true;
		_result = _toDoMngr->view (period);
		return true;
	}

	done = true;

	switch (_sequence->front ()) {
	case DATA:
		_taskId = convertToInt (_dataInput->front ());
		if (_activeListAccessible && _taskId > 0) {
			_result = _toDoMngr->view (_taskId);
			pop ();
		} else {
			done = false;
			_flagError = DATA;
		}
		break;
	case CMD:
		switch (_cmdInput->front ()) {
		case CFROM:
			_period = get_period ();
			
			if (_flagError == NONE) {
				_activeListAccessible = true;
				_result = _toDoMngr->view (_period);
			}
			break;
		case CTIME:
		case CDATE:
			pop ();
		case CTODAY:
		case CTMR:
			_time = get_time ();
			if (_flagError == NONE) {
				_activeListAccessible = true;
				_result = _toDoMngr->view (_viewType, _time);
			}
			break;
		case CTABLE:
			pop ();
			if (!_sequence->empty () && _sequence->front () == DATA) {
				_activeListAccessible = true;
				_result = _toDoMngr->view (mergeStringInput ());
			} else {
				_flagError = DATA;
			}
			break;
		default:
			break;
		}
		break;
	default:
		done = false;
		_flagError = CMD;
		break;
	}

	_input = getLeftOverInput ();
	return done;
}

string View::next () {
	if (!_traverse)
		return _result;

	_first = false;

	if (_taskId != 0) {
		_taskId++;
		_result = _toDoMngr->view (_taskId);
		if (_result == ToDoMngr::NOTHING_TO_VIEW) {
			_last = true;
		}
	} else if (_time.get_date () != Time::DFLT_DATE) {
		Time temp;
		switch (_viewType) {
		case DAILY:
			temp = _time + Time::DAY;
			break;
		case WEEKLY:
			temp = _time + 7 * Time::DAY;
			break;
		case MONTHLY:
			if (++_time)
				temp = _time;
			else
				temp.modify_date (Time::INF_DATE);
			break;
		}

		if (_time.get_date () == Time::INF_DATE) {
 			_last = true;
		} else {
			_time = temp;
			_result = _toDoMngr->view (_viewType, _time);
		}
	} else if (_period.get_start_time ().get_date () != Time::DFLT_DATE) {
		TimePeriod temp;
		temp.modify_start_time (_period.get_end_time () + Time::DAY);
		temp.modify_end_time (_period.get_end_time () + (_period.get_end_time () - _period.get_start_time ()) + Time::DAY);

		if (temp.get_end_time ().get_date () != Time::DFLT_DATE && temp.get_end_time ().get_date () != Time::INF_DATE) {
			_period = temp;
			_result = _toDoMngr->view (_period);
		} else {
			_last = true;
		}
	} else;

	if (_last)
		_result = MSG_NO_NEXT;

	return _result;
}

string View::prev () {
	if (!_traverse)
		return _result;

	_last = false;

	if (_taskId != 0) {
		_taskId--;
		_result = _toDoMngr->view (_taskId);
		if (_result == ToDoMngr::NOTHING_TO_VIEW)
			_first = true;
	} else if (_time.get_date () != Time::DFLT_DATE) {
		Time temp;
		switch (_viewType) {
		case DAILY:
			temp = _time - Time::DAY;
			break;
		case WEEKLY:
			temp = _time - 7 * Time::DAY;
			break;
		case MONTHLY:
			if (--_time)
				temp = _time;
			else
				temp.modify_date (Time::INF_DATE);
			break;
		}

		if (_time.get_date () == Time::INF_DATE) {
 			_first = true;
		} else {
			_time = temp;
			_result = _toDoMngr->view (_viewType, _time);
		}
	} else if (_period.get_end_time ().get_date () != Time::DFLT_DATE) {
		TimePeriod temp;
		temp.modify_end_time (_period.get_start_time () - Time::DAY);
		temp.modify_start_time (_period.get_start_time () - (_period.get_end_time () - _period.get_start_time ()) - Time::DAY);

		if (temp.get_start_time ().get_date () != Time::DFLT_DATE && temp.get_start_time ().get_date () != Time::INF_DATE) {
			_period = temp;
			_result = _toDoMngr->view (_period);
		} else {
			_first = true;
		}
	} else;

	if (_first)
		_result = MSG_NO_PREV;

	return _result;
}

string View::first () {
	if (!_traverse)
		return _result;

	string temp;
	
	while (!_first) {
		prev ();
		if (!_first)
			temp = _result;
	}
	return _result = temp;
}

string View::last () {
	if (!_traverse)
		return _result;
	
	string str;
	string temp;
	
	while (!_last) {
		next ();
		if (!_last)
			temp = _result;
	}
	return _result = temp;
}

void View::deactivateTraverse () {
	_traverse = false;
	if (_result.size ())
		_result.erase (0, string::npos);
}

//---------------------------------------------------------------

string Delete::MSG_DELETED = " is deleted.";

Delete::Delete (vector<cmd_pair> validCmd, ToDoMngr* toDoMngr) {
	assert (toDoMngr != NULL);
	_validCmd = validCmd;
	_toDoMngr = toDoMngr;
	_sequence = NULL;
	_dataInput = NULL;
	_cmdInput = NULL;
	_splitedInput = NULL;
}

Delete::~Delete () {
	clear ();
}

bool Delete::execute () {
	Task task;
	TimePeriod period;
	int taskId;

	splitInput (_input);


	if (_sequence->empty () == false) {
		switch (_sequence->front ()) {
		case DATA:
			taskId = convertToInt (_dataInput->front ());
			task = _toDoMngr->erase (taskId);
			if (task.get_index () != 0) {
				_result = ToDoMngr::view (task) + MSG_DELETED;
				pop ();
			}
			break;
		case CMD:
			switch (_cmdInput->front ()) {
			case CFROM:
			case CTO:
				period = get_period ();
				if (_flagError == NONE)
					_toDoMngr->erase (period);
				break;
			case CTABLE:
				pop ();
				_toDoMngr->erase(mergeSimStringInput ());
				break;
			default:
				break;
			}
		default:
			break;
		}
	}

	_input = getLeftOverInput ();

	if (_flagError == NONE)
		return true;
	else
		return false;
}

//--------------------------------------------------------------

AccCtrl::AccCtrl (ToDoMngr* toDoMngr) {
	assert (toDoMngr != NULL);
	_toDoMngr = toDoMngr;
}

AccCtrl::~AccCtrl () {
}

bool AccCtrl::execute () {
	return true;
}