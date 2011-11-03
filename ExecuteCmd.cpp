#include "ExecuteCmd.h"
#include <sstream>
#include <fstream>
#include <vector>
#include <list>
#include <cassert>
#include <iomanip>
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

	if (task.get_time ().get_date () == Time::DFLT_DATE || task.get_period ().get_start_time ().get_date () == Time::DFLT_DATE)
		_flagError = DATA;

	if (_flagError == NONE) {
		list<Task> taskList = _toDoMngr->add (task, force);
		if (!force && !taskList.empty ()) {
			_result = ToDoMngr::view (task) + MSG_CLASH + ToDoMngr::view (taskList);
			_result += "\nEnter -f to continue adding.";
			insertBreakPoint ();
			done = false;
		} else if (force && !taskList.empty ()) {
			assert (force == taskList.empty ());
		} else {
			_input = getLeftOverInput ();
			_result = ToDoMngr::view (task) + MSG_ADDED;
			done = true;
		}
	} else {
		done = false;
		_input = getLeftOverInput ();
	}

	return done;
}

void Add::insertBreakPoint () {
	_input = _splitedInput->front () + " ";
	_splitedInput->pop_front ();

	if (_splitedInput->empty ())
		_input += BREAK;
	else
		_input += BREAK + " ";

	while (!_splitedInput->empty ()) {
		_input += _splitedInput->front () + " ";
		_splitedInput->pop_front ();
	}		
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

	if (!_sequence->empty () && _sequence->front () == CMD) {
		switch (_cmdInput->front ()) {
		case CVIEW:
			pop ();
			break;
		case CNEXT:
			next ();
			pop ();
			_input = getLeftOverInput ();
			return true;
			break;
		case CPREVIOUS:
			prev ();
			pop ();
			_input = getLeftOverInput ();
			return true;
			break;
		case CLAST:
			last ();
			pop ();
			_input = getLeftOverInput ();
			return true;
			break;
		case CFIRST:
			first ();
			pop ();
			_input = getLeftOverInput ();
			return true;
			break;
		default:
			break;
		}
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
		case CNEXT:
			next ();
			pop ();
			_input = getLeftOverInput ();
			return true;
			break;
		case CPREVIOUS:
			prev ();
			pop ();
			_input = getLeftOverInput ();
			return true;
			break;
		case CLAST:
			last ();
			pop ();
			_input = getLeftOverInput ();
			return true;
			break;
		case CFIRST:
			first ();
			pop ();
			_input = getLeftOverInput ();
			return true;
			break;
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

void View::next () {
	if (!_traverse)
		return;

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
}

void View::prev () {
	if (!_traverse)
		return;

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
}

void View::first () {
	if (!_traverse)
		return;

	string temp;
	
	while (!_first) {
		prev ();
		if (!_first)
			temp = _result;
	}
	_result = temp;
}

void View::last () {
	if (!_traverse)
		return;
	
	string str;
	string temp;
	
	while (!_last) {
		next ();
		if (!_last)
			temp = _result;
	}
	_result = temp;
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

//---------------------------------------------------------------

Merge::Merge (vector<cmd_pair> validCmd) {
	_validCmd = validCmd;
	_sequence = NULL;
	_dataInput = NULL;
	_cmdInput = NULL;
	_splitedInput = NULL;
}

Merge::~Merge () {
	clear ();
}

void Merge::updateInput (string& input, string& newInput) {
	_input = input;
	_newInput = newInput;
}

bool Merge::execute () {
	seperateInput ();
	splitInput (_newInput);

	if (!_sequence->empty () && _sequence->front () == CMD) {
		switch (_cmdInput->front ()) {
		case CVOID:
			_input.erase ();
			_result.erase ();
			break;
		case CDISCARD:
			pop ();
			discard ();
			_input.erase ();
			_result = appendStrings (_part1, _part2, _input);
			break;
		case CREPLACE:
			pop ();
			discard ();
			_input = getLeftOverInput ();
			_result = appendStrings (_part1, _input, _part2);
			break;
		case CINSERT:
			pop ();
		default:
			_input = getLeftOverInput ();
			_result = appendStrings (_part1, _input, _part2);
		}
	} else {
		_input = getLeftOverInput ();
		_result = appendStrings (_part1, _input, _part2);
	}

	return true;
}

void Merge::discard () {
	unsigned int pos;

	if (!_sequence->empty () && _sequence->front () == CMD) {
		if (_cmdInput->front () == CLEFT) {
			pop ();
			pos = _part1.find_last_of (' ');
			_part1 = _part1.substr (0, pos);
		} else if (_cmdInput->front () == CRIGHT) {
			pop ();
			pos = _part2.find_first_of (' ', 0);
			if (pos + 1 < _part2.size ()) {
				_part2 = _part2.substr (pos + 1, string::npos);
			}
		} else;
	}
}

void Merge::seperateInput () {
	unsigned int pos, next;
	pos = _input.find (BREAK, 0);

	int num_break = 0;
	next = pos + BREAK.size ();
	while (next != string::npos && next > _input.size ()) {
		next = _input.find (BREAK, next);
		next += BREAK.size ();
		num_break++;
	}
	assert (num_break <= 1);

	_part1.erase ();
	_part2.erase ();

	if (pos == string::npos) {
		_part1 = _input;
	} else if (pos == 0) {
		_part2 = _input.substr (BREAK.size () + 1, string::npos);
	} else {
		_part1 = _input.substr (0, pos - 1);

		if (pos + 1 + BREAK.size () < _input.size ()) {
			_part2 = _input.substr (pos + 1 + BREAK.size (), string::npos);
		}
	}
}

string Merge::appendStrings (string& str1, string& str2, string& str3) {
	string str;

	if (str1.empty () && str2.empty ()) {
		str = str3;
	} else if (str1.empty () && str3.empty ()) {
		str = str2;
	} else if (str2.empty () && str3.empty ()) {
		str = str1;
	} else if (str1.empty ()) {
		str = str2 + ' ' + str3;
	} else if (str2.empty ()) {
		str = str1 + ' ' + str3;
	} else if (str3.empty ()) {
		str = str1 + ' ' + str2;
	} else {
		str = str1 + ' ' + str2 + ' ' + str3;
	}

	return str;
}

//---------------------------------------------------------------

Table::Table (vector<cmd_pair> validCmd, ToDoMngr* toDoMngr) {
	assert (toDoMngr != NULL);
	_validCmd = validCmd;
	_toDoMngr = toDoMngr;
	_sequence = NULL;
	_dataInput = NULL;
	_cmdInput = NULL;
	_splitedInput = NULL;
}

Table::~Table () {
	clear ();
}

bool Table::execute () {
	bool done = true;
	return done;
}

//---------------------------------------------------------------

bool Help::execute () {
	ifstream myhelpfile;
	myhelpfile.open("HELP.txt");
	string HELP_LINE;
	ostringstream oss;

	eraseFirstWord ();
//from here, you can change command variable to _input so that you dont need this line
	string command = _input;

	for(int i=0; i<command.size(); i++)
		command[i] = tolower(command[i]);
	
	if(command == "" || command == "command")
	{
		int i=0, k=0;
		getline(myhelpfile, HELP_LINE);
		oss << endl << setw(54) << setfill(' ') << HELP_LINE << endl;
		oss << endl << "  ----------------------------------------------------------------------------  ";
		while(getline(myhelpfile, HELP_LINE) && !myhelpfile.eof())
		{
			if(HELP_LINE.empty())
				oss << endl;
			else
			{
				if(k==0)
				{
					oss << "  " << "Command: " << HELP_LINE << endl;
					k++;
				}
				else
				{
					oss << endl << "  " << HELP_LINE << endl; 
					k=0;
				}
				if(HELP_LINE.empty()) 
					oss << "  ----------------------------------------------------------------------------  "<< endl ;
			}
			if(i != 0 && HELP_LINE.empty())
				oss << "  ----------------------------------------------------------------------------  "<< endl ;
			i++;
		}
		oss << endl << "  ----------------------------------------------------------------------------  ";
	}

	while(getline(myhelpfile, HELP_LINE))
	{
		if(command == HELP_LINE)
		{	
			oss << endl << setw(54) << setfill(' ') << "<--- TASKCAL Help Viewer --->"  << endl;
			oss << endl << "  ----------------------------------------------------------------------------  ";
			oss << endl << "  " << "Command: " << HELP_LINE << endl;
			while(getline(myhelpfile, HELP_LINE) && !HELP_LINE.empty())
				oss << endl << "  " << HELP_LINE << endl;
			oss << endl << "  ----------------------------------------------------------------------------  ";
			myhelpfile.close();
			break;
		}
	}
	
	//this is to make sure that you dont return any leftOverInput
	_input.erase ();

	return true;
}

void Help::eraseFirstWord () {
	unsigned int pos = _input.find_first_of (' ', 0);
	if (pos == string::npos)
		_input.erase (0, pos);
	else
		_input.erase (0, pos + 1);
}

//---------------------------------------------------------------

AccCtrl::AccCtrl (ToDoMngr* toDoMngr) {
	assert (toDoMngr != NULL);
	_toDoMngr = toDoMngr;
}

AccCtrl::~AccCtrl () {
}

bool AccCtrl::execute () {
	return true;
}