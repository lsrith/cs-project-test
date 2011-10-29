#include "CmdControl.h"
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;

string CmdControl::INV_CMD = "unknown command: ";
string CmdControl::INV_DATA = "invalid data input: ";
string CmdControl::MSG_DELETED = " is deleted.";
string CmdControl::MSG_ADDED = " is added.";
string CmdControl::MSG_EDITED = " is edited.";
string CmdControl::MSG_CLEAR = "All your tasks have been deleted!";
string CmdControl::MSG_CLASH = " is clashed with the following:\n";
string CmdControl::MSG_ERROR = "\n\tERROR!!\n";
string CmdControl::MSG_NO_NEXT = "This is the last!";
string CmdControl::MSG_NO_PREV = "This is the first!";
string CmdControl::MSG_WRONG_DATE = "Invalid date entered!";
string CmdControl::MSG_WRONG_ID = "Invalid index entered!";
string CmdControl::MSG_WRONG_TABLE = "Invalid timetable name entered!";
string CmdControl::MSG_WRONG_PERIOD = "Invalid period entered!";

CmdControl::CmdControl (bool dotCmd) {
	if (dotCmd) {
		_validCmdFile = "vldCmd1.txt";
		_dfltCmdFile = "dfltCmd1.txt";
		_dotCmd = true;
	} else {
		_validCmdFile = "vldCmd2.txt";
		_dfltCmdFile = "dfltCmd2.txt";
		_dotCmd = false;
	}
	_validDayMnth = "vldDM.txt";

	_sequence = &_1stSeq;
	_cmdInput = &_1stCmd;
	_dataInput = &_1stData;
	_2ndQs = false;

	_flagError = NONE;
	_force = false;
	_activeListAccessible = false;
	clearTaskElement ();
//	_search = ToDoMngr::search_t::SEACH;
// these two following parts are to be modified
	string str = get_vldCmdList (_validCmd, standAloneCmdEndPos);
	_dayMonth = get_dayMonth ();
	log.log ("cmdControl");
/*
	if (_validCmd.empty ())
		cout << "empty" << endl;
*/
}

void CmdControl::clearTaskElement () {
	_taskElement._alert = false;
	_taskElement._note = false;
	_taskElement._period = false;
	_taskElement._repeat = false;
	_taskElement._time = false;
	_taskElement._venue = false;
	Task newTask;
	_task = newTask;
}

bool CmdControl::checkTaskElement (bool condition) {
	bool allCond;

	if (condition) {
		allCond = _taskElement._alert && _taskElement._note && _taskElement._period
			&& _taskElement._repeat && _taskElement._time && _taskElement._venue;
	} else {
		allCond = _taskElement._alert || _taskElement._note || _taskElement._period
			|| _taskElement._repeat || _taskElement._time || _taskElement._venue;
		allCond = !allCond;
	}

	return allCond;
}

void CmdControl::updateInput (string& input) {
	clear ();
	_input = input;
	_flagError = NONE;
	_flagPrompt = NOPROMPT;
	deactivate2ndQs ();
	try {
		splitInput ();
	} catch (string message) {
		throw (message);
	}
};

void CmdControl::addInput (string& input) {
	log.start ("addInput");
	_input = input;
	_flagError = NONE;
	_flagPrompt = NOPROMPT;
	if (_2ndQs && !_input.empty ()) {
		string temp;
		command cmd;
		unsigned int end_pos = 0;
		bool specialCmd = true;
		if (_dotCmd) {
			if (_input[0] == '.' || _input[0] == '-') {
				end_pos = _input.find_first_of (" .-", 1);
				temp = _input.substr (0, end_pos);
				cmd = translateCmd (temp);
			}
		} else {
			end_pos = _input.find_first_of (' ', 0);
			temp = _input.substr (0, end_pos);
			cmd = translateCmd (temp);
		}

		if (_flagError != CMD && (cmd == CDISCARD || cmd == CREPLACE)) {
			if (!_1stSeq.empty () && _1stSeq.front () == CMD) {
				_1stSeq.pop ();
				_1stCmd.pop ();
			} else if (!_1stSeq.empty () && _1stSeq.front () == DATA) {
				_1stSeq.pop ();
				_1stData.pop ();
			} else;
		} else if (_flagError != CMD && (cmd == CINSERT)) {
		} else {
			specialCmd = false;
		}

		if (specialCmd) {
			if (end_pos == string::npos || _input[end_pos] != ' ')
				_input.erase (0, end_pos);
			else
				_input.erase (0, end_pos + 1);
		}
//cout <<"addInput " << _input << endl;
	}

	try {
		splitInput ();
	} catch (string message) {
		throw (message);
	}
	log.end ();
};

void CmdControl::splitInput () {
	log.start ("splitInput");
	string temp;
	command cmd;
	unsigned int end_pos = 0;

	log.cond ("_dotCmd", _dotCmd);
	if (_dotCmd) {
		log.call ("checkIfStandAlone");
		checkIfStandAloneCmd ();
		log.end ();
		
		log.loop ("_input");
		while (!_input.empty ()) {
			if (_input[0] == '.' || _input[0] == '-') {
				end_pos = _input.find_first_of (" .-", 1);
				temp = _input.substr (0, end_pos);
				cmd = translateCmd (temp);
				if (_flagError != CMD) {
					push (cmd);
					if (end_pos == string::npos || _input[end_pos] != ' ')
						_input.erase (0, end_pos);
					else
						_input.erase (0, end_pos + 1);
					break;
				} else
					throw (INV_CMD);
			} else {
				end_pos = _input.find_first_of (" .\n");
				while (end_pos != string::npos && _input[end_pos] == '.' && _input[end_pos - 1] != ' ')
					end_pos = _input.find_first_of (" .\n", end_pos + 1);
				temp = _input.substr (0, end_pos);
				push (temp);
				if (end_pos != string::npos)
					_input.erase (0, end_pos + 1);
				else
					_input.erase (0, end_pos);
			}
		}
		log.end ();
	} else {
		log.loop ("_input");
		while (!_input.empty ()) {
			if (_input[0] == '\"') {
				end_pos = _input.find_first_of ('\"', 1);
				temp = _input.substr (1, end_pos - 1);
				push (temp);
				end_pos++;
			} else {
				end_pos = _input.find_first_of (' ', 0);
				temp = _input.substr (0, end_pos);
				cmd = translateCmd (temp);
				if (_flagError == CMD) {
					push (temp);
					_flagError = NONE;
				} else {
					push (cmd);
				}
			}

			if (end_pos != string::npos)
				_input.erase (0, end_pos + 1);
			else
				_input.erase (0, end_pos);
		}
		log.end ();
	}
	log.end ();
	log.end ();
}

string CmdControl::executeCmd () {
	string str;
	if (_2ndQs)
		mergeAllQs ();

	_flagError = NONE;
	_flagPrompt = NOPROMPT;
	command cmd;
	while (_sequence->empty () == false) {
		if (_sequence->front () == CMD) {
			cmd  = _cmdInput->front ();
			pop ();
			str += executeCmd (cmd) + "\n";

			if (_flagError != NONE || _flagPrompt != NOPROMPT)
				break;
		} else {
			pop ();
		}
	}

	str += checkPrompt ();
	return str;
}

string CmdControl::executeCmd (command cmd) {
	string str;

	switch (cmd) {
	case CDISCARD:
	case CREPLACE:
	case CINSERT:
		break;
	default:
		deactivate2ndQs ();
		break;
	}

	switch (cmd) {
	case CFIRST:
		str = executeFIRST ();
		break;
	case CLAST:
		str = executeLAST ();
		break;
	case CNEXT:
		str = executeNEXT ();
		break;
	case CPREVIOUS:
		str = executePREV ();
		break;
	default:
		_taskId = 0;
		_time.modify_date (Time::DFLT_DATE);
		_period.modify_start_time (_time);
		_period.modify_end_time (_time);
		_first = false;
		_last = false;
		break;
	}

	switch (cmd) {
	case CTIME:
	case CFROM:
	case CNAME:
	case CVENUE:
	case CNOTE:
	case CALERT:
	case CREPEAT:
		if (!checkTaskElement (false)) {
			_toDoMngr.undo ();
			str = executeCmd (CADD);
		} else {
			pop ();
		}
		break;
	default:
		clearTaskElement ();
		break;
	}

	switch (cmd) {
	case CPROMPT:
		str = executePROMPT ();
		break;
	case CADD:
		str = executeADD ();
		break;
	case CEDIT:
		str = executeEDIT ();
		break;
	case CDELETE:
		str = executeDELETE ();
		break;
	case CTABLE:
		str = executeTABLE ();
		break;
	case CVIEW:
		str = executeVIEW ();
		break;
	case CREMINDER:
		str = _toDoMngr.reminder ();
		break;
	case CUNDO:
		_toDoMngr.undo ();
		break;
	case CREDO:
		_toDoMngr.redo ();
		break;
	case CSORT:
		executeSORT ();
		break;
	case CCLEAR:
		_toDoMngr.clear ();
		str = MSG_CLEAR;
		break;
	case CRESET:
		reset ();
		break;
	case CHELP:
		str = executeHELP ();
		break;
	case CSEARCH:
		str = executeSEARCH ();
		break;
	case CEXIT:
		if (_sequence->empty () == false && _sequence->front () == CMD && _cmdInput->front () == CTABLE) {
			pop ();
			_tableName.erase ();
		} else {
			update_dayMonth (_dayMonth);
			_toDoMngr.exit ();
			exit (true);
		}
		break;
	default:
		break;
	}

	if (cmd == CVIEW)
		_activeListAccessible = true;
	else
		_activeListAccessible = false;

	return str;
}

string CmdControl::checkPrompt () {
	string str;
	switch (_flagPrompt ) {
	case ADDCLASHED:
		clear (&_2ndData);
		clear (&_2ndSeq);
		clear (&_2ndCmd);
		activate2ndQs ();
		push (CPROMPT);
		push (CADD);
		str = "Do you still want to continue? (Y/N)\n";
		break;
	case EDITCLASHED:
		clear (&_2ndData);
		clear (&_2ndSeq);
		clear (&_2ndCmd);
		activate2ndQs ();
		push (CPROMPT);
		push (CEDIT);
		str = "Do you still want to continue? (Y/N)\n";
		break;
	case VLDINPUT:
	case VLDCMD:
		str = "TaskCal cannot translate this following instruction:\n";
		
		log.cond ("_sequence");
		if (_sequence->empty ()) {
			str += "More instruction is required.\n";
		} else if (_sequence->front () == CMD) {
			str += "This command [" + convertToString (_cmdInput->front ()) + "] may be invalid.\n";
		} else if (_sequence->front () == DATA) {
			str += "This data [" + _dataInput->front () + "] may be invalid.\n";
		} else;

		_sequence = new queue<input_t>;
		_dataInput = new queue<string>;
		_cmdInput = new queue<command>;

		*_sequence = _2ndSeq;
		*_dataInput = _2ndData;
		*_cmdInput = _2ndCmd;
		
		while (!_sequence->empty ()) {
			switch (_sequence->front ()) {
			case DATA:
				str += _dataInput->front ();
				_dataInput->pop ();
				_sequence->pop ();
				if (!_sequence->empty () && _sequence->front () == CMD)
					str += "\" ";
				else
					str += " ";
				break;
			case CMD:
				str += convertToString (_cmdInput->front ());
				_cmdInput->pop ();
				_sequence->pop ();
				if (!_sequence->empty () && _sequence->front () == DATA)
					str += " \"";
				else
					str += " ";
				break;
			default:
				break;
			}
		}

		if (_1stSeq.empty ()) {
			str += "|| [??]\nmore instruction:\n";
		} else if (_1stSeq.front () == CMD) {
			str += "|| " + convertToString (_1stCmd.front ()) + "\n";
			str += "Do you want to discard/replace/insert?\n";
		} else if (_1stSeq.front () == DATA) {
			str += "|| " + _1stData.front () + "\n";
			str += "Do you want to discard/replace/insert?\n";
		} else;

		activate2ndQs ();
		break;
	default:
		break;
	}

	return str;
}

//to be edited
string CmdControl::executeEditCmd (int indx) {
	cmd_pair pair;
	pair.enum_cmd = convertToCommand (indx);
	string str;
	str.erase ();

	if (_sequence->front () == DATA) {
		pair.str_cmd = _dataInput->front ();
		pop ();
		str = ">> " + pair.str_cmd + " indicate " + convertToString (pair.enum_cmd);
		
		vector<cmd_pair>::iterator iter;
		bool existed = search_vldCmd (pair.str_cmd, iter);
		if (!existed) {
			_validCmd.push_back (pair);
		} else if (pair.enum_cmd != iter->enum_cmd) {
				iter->enum_cmd = pair.enum_cmd;
		} else;
	}
	return str;
}

//to be edited
string CmdControl::checkAlert () {
	string str;
/*
	if (_toDoMngr.ifAlertAtive ()) {
		str = alert ();
		string message;
		if (promptToGetNewInput (message)) {
			int min = covertToInt (_dataInput->front ());
			if (min == -1)
				_toDoMngr.deactivateAlert ();
			else
				_toDoMngr.snoozeAlert (min);
		} else
			_toDoMngr.deactivateAlert ();
	}
*/
	return str;
}

string CmdControl::executeHELP () {
	string str;

	if (_sequence->empty () == false && _sequence->front () == CMD && _cmdInput->front () == CCOMMAND) {
		pop ();
		str = _toDoMngr.help ("cmd");
	} else if (_sequence->empty () == false && _sequence->front () == DATA) {
		str = _toDoMngr.help (mergeStringInput ());
		pop ();
	} else {
		str = _toDoMngr.help ("");
	}

	return str;
}

//to be edited
string CmdControl::executeTABLE () {
	string str;
	if (_sequence->empty () == false && _sequence->front () == DATA && !getTableName ()) {
		string tableName = mergeStringInput ();
		pop ();

		if (_sequence->empty () == false && _sequence->front () == CMD && (_cmdInput->front () == CFROM || _cmdInput->front () == CTO)) {
			TimePeriod period = get_period ();
			
			if (_flagError == NONE) {
				_toDoMngr.newTable (tableName, period);
				_tableName = tableName;
			}
		} else {
			_tableName = tableName;
		}
	} else {
		str = _toDoMngr.viewTableNames ();
	}

	return str;
}

//to be edited
string CmdControl::executeDELETE () {
	string str;
	Task task;
	TimePeriod period;
	int taskId;

	if (_sequence->empty () == false) {
		switch (_sequence->front ()) {
		case DATA:
			if (_activeListAccessible) {
				string data = _dataInput->front ();
				taskId = convertToInt (data);
//cout << "taskId = " << taskId << endl;
				task = _toDoMngr.erase (taskId);
//str = "_toDoMngr.erase (taskId)";
				if (task.get_index () != 0) {
					str = ToDoMngr::view (task) + MSG_DELETED;
//str = "ToDoMngr::view (task) + MSG_DELETED";
					pop ();
				}
			}
			break;
		case CMD:
			switch (_cmdInput->front ()) {
			case CFROM:
			case CTO:
				period = get_period ();
				if (_flagError == NONE)
					_toDoMngr.erase (period);
//str = "executeFunction (_toDoMngr.erase)";
				break;
			case CTABLE:
				pop ();
				_toDoMngr.erase (_tableName);
//str = "_toDoMngr.erase (_tableName)";
				break;
			default:
				break;
			}
		default:
			break;
		}
	}
	return str;
}

//to be edited
string CmdControl::executeVIEW () {
	string str;

	_viewType = DAILY;
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

	if (_sequence->empty ())
		return _toDoMngr.view (_period);

	switch (_sequence->front ()) {
	case DATA:
		if (_activeListAccessible) {
			string data = _dataInput->front ();
			_taskId = convertToInt (data);
			str = _toDoMngr.view (_taskId);
			if (!str.empty ())
				pop ();
		}
		break;
	case CMD:
		switch (_cmdInput->front ()) {
		case CFROM:
			_period = get_period ();
			
			if (_flagError != DATA) {
				str = _toDoMngr.view (_period);
			}
			break;
		case CTIME:
		case CDATE:
			pop ();
			_time.modify_date (get_date ());
			if (_flagError != DATA) {
				str = _toDoMngr.view (_viewType, _time);
			}
			break;
		case CTABLE:
			pop ();
			while (!getTableName ()) {
//				promptToGetValidInput (MSG_WRONG_TABLE);
			}	
//			str = _toDoMngr.view (viewType, _tableName);
//str = "_toDoMngr.view (viewType, _tableName)";
			break;
		default:
			break;
		}
		break;
	default:
		if (!_tableName.empty ())
//			str = _toDoMngr.view (viewType, _tableName);
//str = "_toDoMngr.view (viewType, _tableName)";
		break;
	}

	return str;
}

//to be edited
string CmdControl::executeSEARCH () {
	string str;
	if (_sequence->empty ())
		return str;

	search_t type = SEACH;
	if (_sequence->front () == CMD) {
		switch (_cmdInput->front ()) {
		case CEXACT:
			type = SEXACT;
			pop ();
			break;
		case CEACH:
			type = SEACH;
			pop ();
			break;
		case CSIMILAR:
			type = SSIMILAR;
			pop ();
			break;
		default:
			_flagError = CMD;
			break;
		}
	}

	if (_flagError == NONE && _sequence->front () == DATA) {
		str = _toDoMngr.search (type, mergeStringInput ());
	}

	return str;
}

string CmdControl::executeADD () {
	string str;
	_force = false;

	if (_sequence->front () == CMD && _cmdInput->front () == CFORCE) {
		pop ();
		_force = true;
	}

	update_task (&_task);
	
	switch (_flagError) {
	case NONE:
		_taskList = new list<Task>;
		*_taskList = _toDoMngr.add (_tableName, _task, _force);
		if (!_force && !_taskList->empty ()) {
			str = ToDoMngr::view (_task) + MSG_CLASH + ToDoMngr::view (*_taskList);
			_flagPrompt = ADDCLASHED;
		} else if (_force && !_taskList->empty ()) {
			str = MSG_ERROR;
		} else {
			str = ToDoMngr::view (_task) + MSG_ADDED;
			clearTaskElement ();
		}

		delete _taskList;
		break;
	case DATA:
		_flagPrompt = VLDINPUT;
		break;
	case CMD:
		_flagPrompt = VLDCMD;
		break;
	}
	return str;
}

string CmdControl::executeEDIT () {
	string str;

	if (_sequence->empty ())
		return str;
	else if (_sequence->front () == DATA) {
		_taskId = convertToInt (_dataInput->front ());
		if (_taskId < 1) {
			_flagError = DATA;
		}
	} else;

	if (_flagError == NONE && _sequence->front () == CMD) {
		update_task (&_task);

		list<Task> taskList = (_toDoMngr.edit (_taskId, &_taskElement, &_task, _force));
		_taskList = &taskList;

		if (!_force && !_taskList->empty ()) {
			str = ToDoMngr::view (_task) + MSG_CLASH + ToDoMngr::view (*_taskList);
			_flagPrompt = EDITCLASHED;
		} else if (_force && !_taskList->empty ()) {
			str = MSG_ERROR;
		} else {
			str = ToDoMngr::view (_task) + MSG_EDITED;
			clearTaskElement ();
		}
	
		if (!_taskList->empty ()) {
			_taskList->clear ();
		}
	}

	return str;
}

void CmdControl::executeSORT () {
cout << "exeSort" << endl;
}

string CmdControl::executeNEXT () {
	pop ();
	_first = false;
	string str;

	if (_taskId != 0) {
		_taskId++;
		str = _toDoMngr.view (_taskId);
		if (str.empty ())
			_last = true;
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
			str = _toDoMngr.view (_viewType, _time);
		}
	} else if (_period.get_start_time ().get_date () != Time::DFLT_DATE) {
		TimePeriod temp;
		temp.modify_start_time (_period.get_end_time () + Time::DAY);
		temp.modify_end_time (_period.get_end_time () + (_period.get_end_time () - _period.get_start_time ()) + Time::DAY);

		if (temp.get_end_time ().get_date () != Time::DFLT_DATE && temp.get_end_time ().get_date () != Time::INF_DATE) {
			_period = temp;
			str = _toDoMngr.view (_period);
		} else {
			_last = true;
		}
	} else;

	if (_last)
		str = MSG_NO_NEXT;

	return str;
}

string CmdControl::executePREV () {
	pop ();
	_last = false;
	string str;

	if (_taskId != 0) {
		_taskId--;
		str = _toDoMngr.view (_taskId);
		if (str.empty ())
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
			str = _toDoMngr.view (_viewType, _time);
		}
	} else if (_period.get_end_time ().get_date () != Time::DFLT_DATE) {
		TimePeriod temp;
		temp.modify_end_time (_period.get_start_time () - Time::DAY);
		temp.modify_start_time (_period.get_start_time () - (_period.get_end_time () - _period.get_start_time ()) - Time::DAY);

		if (temp.get_start_time ().get_date () != Time::DFLT_DATE && temp.get_start_time ().get_date () != Time::INF_DATE) {
			_period = temp;
			str = _toDoMngr.view (_period);
		} else {
			_first = true;
		}
	} else;

	if (_first)
		str = MSG_NO_PREV;

	return str;
}

string CmdControl::executeFIRST () {
	string str;
	string temp;
	
	while (!_first) {
		temp = executePREV ();
		if (!_first)
			str = temp;
	}

	return str;
}

string CmdControl::executeLAST () {
	string str;
	string temp;
	
	while (!_last) {
		temp = executeNEXT ();
		if (!_last)
			str = temp;
	}

	return str;
}

bool CmdControl::getTableName () {
	return false;
}

void CmdControl::update_task (Task* taskPtr) {
	TimePeriod period;
	taskPtr->note = mergeStringInput ();
	bool reachExeCmd = false;
	while (!reachExeCmd && _flagError == NONE && !_sequence->empty ()) {
		if (_sequence->front () != CMD) {
			_flagError = DATA;
			break;
		}

		Time time;
		TimePeriod period;
		switch (_cmdInput->front ()) {
		case CTIME:
			pop ();
			time = get_time ();
			if (_flagError == NONE) {
				taskPtr->modify_time (time);
				_taskElement._time = true;
			}
			break;
		case CTO:
			_flagError = CMD;
			break;
		case CFROM:
			period = get_period ();
			if (_flagError == NONE) {
				taskPtr->modify_period (period);
				_taskElement._period = true;
			}
			break;
		case CVENUE:
			pop ();
			taskPtr->venue = mergeStringInput ();
			_taskElement._venue = true;
			break;
		case CNOTE:
			pop ();
			taskPtr->note = mergeStringInput ();
			_taskElement._note = true;
			break;
		case CALERT:
			pop ();
			time = get_time ();
			if (_flagError == NONE) {
				taskPtr->alert = time;
				_taskElement._alert = true;
			}
			break;
		case CREPEAT:
			pop ();
			if (_sequence->front () == CMD) {
				switch (_cmdInput->front ()) {
				case CHOUR:
					taskPtr->repeat = 60;
					pop ();
					break;
				case CFORTNIGHT:
					taskPtr->repeat = 14 * Time::DAY;
					pop ();
					break;
				case CYEAR:
					taskPtr->repeat = 2;
					pop ();
					break;
				case CDAY:
					taskPtr->repeat = Time::DAY;
					pop ();
					break;
				case CWEEK:
					taskPtr->repeat = 7 * Time::DAY;
					pop ();
					break;
				case CMONTH:
					taskPtr->repeat = 1;
					pop ();
					break;
				default:
					_flagError = CMD;
					break;
				}
			} else if (_sequence->front () == DATA) {
				int mins = convertToInt (_dataInput->front ());
				if (mins >= 0) {
					if (mins < 3)
						mins = 3;

					taskPtr->repeat = mins;
					pop ();
				} else {
					Time::clk_t clock = get_clock ();
					if (_flagError == NONE)
						taskPtr->repeat = clock / 100 * 60 + clock % 100;
				}
			} else;

			if (taskPtr->repeat != 0)
				_taskElement._repeat = true;
			break;
		default:
			reachExeCmd = true;
			break;
		}
	}
}

Task CmdControl::get_task () {
	Task task;
	clearTaskElement ();
	update_task (&task);
	return task;
}

string CmdControl::mergeStringInput () {
	string str;
	while (_sequence->empty () == false && _sequence->front () == DATA) {
		str += _dataInput->front () + " ";
		pop ();
	}
	str = str.substr (0, str.size () - 1);
	return str;
}

//Format1: FROM time1 TO time2         time1 <= time2 else _flagError = DATA
//Format2: FROM date1 clock1 TO clock2 if clock1 > clock2, date2 = date1 + 1day
//if time1 || time2 is not specified, _flagError = CMD
//if time1 || time2 is invalid, _flagError = DATA
TimePeriod CmdControl::get_period () {
	TimePeriod period;
	Time time;
	Time::clk_t clock;

	if (_sequence->front () == CMD && _cmdInput->front () == CFROM) {
		pop ();
		time = get_time ();
		if (_flagError == NONE)
			period.modify_start_time (time);
	} else {
		_flagError = CMD;
	}

	if (_flagError == NONE) {
		if (_sequence->front () == CMD && _cmdInput->front () == CTO) {
			pop ();
			clock = get_clock ();
			if (_flagError == NONE) {
				if (time.get_clock () > clock)
					time = time + Time::DAY;

				time.modify_clock (clock);
			} else {
				_flagError = NONE;

				time = get_time ();
				if (_flagError == NONE) {
					if (period.get_start_time () > time) {
						_flagError = DATA;
						_period = period;
					} else {
						period.modify_end_time (time);
					}
				}
			}
		}
	}

	return period;
}

//if date is not specified but clock is, current date will be used
//if date and clock are not specified, _flagError is from get_date ()
//if date is specified but no clock, clock store its default, no _flagError
Time CmdControl::get_time () {
	Time time;
	Time dfltTime;
	if (_flagError != NONE)
		_flagError = NONE;

	time.modify_clock (get_clock ());
	if (_flagError == NONE) {
		Time curr;
		curr.current_time ();

		if (time.get_clock () < curr.get_clock ())
			curr = curr + Time::DAY;

		time.modify_date (curr.get_date ());
	} else {
		_flagError = NONE;

		time.modify_date (get_date ());
		if (_flagError == NONE) {
			time.modify_clock (get_clock ());
			_flagError = NONE;
		}
	}
	return time;
}

//Format: DDMMYYYY || DDMMYY || MMDDYYYY || MMDDYY || DDMMMYYYY || MMMDDYYYY || YYYYMMMDD
//if date is not specified, _flagError = CMD, date = DFLT_DATE
//if date is invalid, _flagError = DATA, date = INF_DATE
Time::date_t CmdControl::get_date () {
	if (_sequence->empty ()) {
		_flagError = CMD;
		return Time::DFLT_DATE;
	}

	Time currTime;
	currTime.current_time ();

	if (_sequence->front () == CMD && _cmdInput->front () == CTODAY) {
		pop ();
		return currTime.get_date ();
	} else if (_sequence->front () == CMD && _cmdInput->front () == CTMR) {
		pop ();
		currTime = currTime + Time::DAY;
		return currTime.get_date ();
	} else if (_sequence->front () == CMD) {
		_flagError = CMD;
		return Time::DFLT_DATE;
	} else;

	string strDate = _dataInput->front ();
	int size = strDate.size ();
	int Day = 7;
	int tempDay, tempMnth, tempYear;
	int day = -1;
	int mnth = -1;
	int year = -1;

	if (_flagError == DATA)
		_flagError = NONE;

	if (size == 3) {
		convertToInt (strDate, Day, mnth);
		if (Day < 7)
			pop ();
	} else if (size > 3) {
		convertToInt (strDate.substr (0, 3), Day, mnth);
		if (Day < 7)
			_dataInput->front ().erase (0, 3);
	} else;
	mnth = -1;

	while (_sequence->empty () == false && _sequence->front () == DATA && (day == -1 || mnth == -1 || year == -1)) {
		get_date (tempDay, tempMnth, tempYear);
		if (tempDay != -1 || tempMnth != -1 || tempYear != -1)
			pop ();
		else
			break;

		if (day == -1)
			day = tempDay;
		else {
			if (tempDay != -1 && mnth == -1)
				mnth = tempDay;
			
			if (mnth != -1 && mnth > 12 && day != -1 && day <= 12)
				_dayMonth = false;

			if (!_dayMonth) {
				tempDay = day;
				day = mnth;
				mnth = tempDay;
			}
		}

		if (mnth == -1 && tempMnth != -1)
			mnth = tempMnth;
		if (year == -1)
			year = tempYear;
	}

	if (Day < 7) {
		if (day == -1 && mnth == -1 && year == -1) {
			int currDay = currTime.get_day ();
			currTime = currTime + ((Day + 7 - currDay) % 7) * Time::DAY;
		} else if (day != -1 && mnth == -1 && year == -1) {
			int currDate = currTime.get_date ();
			currDate += (day - currDate / 1000000) * 1000000;				//substitute with user's day
			try {
				currTime.modify_date (currDate);

				while (currTime.get_day () != Day) {
					if (!(++currTime)) {
						_flagError = DATA;
						break;
					}
				}
			} catch (string msg) {
				_flagError = DATA;
			}
		} else if (day != -1 && mnth != -1 && year == -1) {
			int currDate = currTime.get_date ();
			currDate = day * 1000000 + mnth * 10000 + currDate % 10000;	//substitute with user's day and mnth
			try {
				currTime.modify_date (currDate);
				while (currTime.get_day () != Day) {
					if (currTime.get_date () % 10000 == 0) {
						_flagError = DATA;
						break;
					} else {
						currDate++;
						currTime.modify_date (currDate);						//increment by one year
					}
				}
			} catch (string msg) {
				_flagError = DATA;
			}
		} else if (day != -1 && mnth != -1 && year != -1) {
			try {
				currTime.modify_date (day * 1000000 + mnth * 10000 + year);
				
				if (currTime.get_day () != Day)
					_flagError = DATA;
			} catch (string msg) {
				_flagError = DATA;
			}
		} else
			_flagError = DATA;
	} else {
		if (day == -1 && mnth == -1 && year == -1) {
			_flagError = DATA;
		} else if (day != -1 && mnth == -1 && year == -1) {
			int currDate = currTime.get_date ();
			currDate += (day - currDate / 1000000) * 1000000;				//substitute with user's day
			try {
				currTime.modify_date (currDate);
			} catch (string msg) {
				_flagError = DATA;
			}
		} else if (day != -1 && mnth != -1 && year == -1) {
			int currDate = currTime.get_date ();
			currDate = day * 1000000 + mnth * 10000 + currDate % 10000;	//substitute with user's day and mnth
			try {
				currTime.modify_date (currDate);
			} catch (string msg) {
				_flagError = DATA;
			}
		} else if (day != -1 && mnth != -1 && year != -1) {
			try {
				currTime.modify_date (day * 1000000 + mnth * 10000 + year);
			} catch (string msg) {
				_flagError = DATA;
			}
		} else
			_flagError = DATA;
	}

	if (_flagError == DATA)
		return Time::INF_DATE;
	else
		return currTime.get_date ();
}

//Format (24hrs): HH.MM || HH:MM
//Format (12hrs): HH.MM AM || HH:MM AM || HAM || HHAM || HH.MMAM || HH:MMAM
//if clock is not specified, _flagError = CMD, clock = DFLT_CLOCK
//if clock is invalid, _flagError = DATA, clock = INF_CLOCK
Time::clk_t CmdControl::get_clock () {
	Time::clk_t clock = 0;
	if (_sequence->empty ()) {
		_flagError = CMD;
		return	clock = Time::DFLT_CLOCK;
	} else if (_sequence->front () == CMD && _cmdInput->front () == CNOW) {
		pop ();
		Time currTime;
		currTime.current_time ();
		return currTime.get_clock ();
	} else if (_sequence->front () == CMD) {
		_flagError = CMD;
		return clock = Time::DFLT_CLOCK;
	} else;

	string strClk = _dataInput->front ();
	int size = strClk.size ();

	switch (size) {
	case 7:
	case 5:
		if ((strClk[2] == '.' || strClk[2] == ':') &&
			(strClk.substr (3, 2) >= "00" && strClk.substr (3, 2) <= "59"))
				clock += (strClk[3] - '0') * 10 + strClk[4] - '0';
		else
			_flagError = DATA;
	case 4:
		if (strClk.substr (0, 2) >= "00" && strClk.substr (0, 2) <= "23")
			clock += (strClk[0] - '0') * 1000 + (strClk[1] - '0') * 100;
		else
			_flagError = DATA;
		break;
	case 3:
		if (strClk[0] >= '0' && strClk[0] <= '9')
			clock += (strClk[0] - '0') * 100;
		else
			_flagError = DATA;
		break;
	default:
		_flagError = DATA;
		break;
	}

	if (_flagError == NONE) {
		switch (size) {
		case 5:
			pop ();
			if (clock <= 1259 && notMorning ()) {
				if (clock != 1200)
					clock += 1200;
			}
			else if (clock <= 1259 && clock == 1200)
					clock += 1200;
			else;
			break;
		case 7:
		case 4:
		case 3:
			if (clock >= 1259)
				_flagError = DATA;
			else if (notMorning (strClk.substr (size - 2))) {
				if (clock != 1200)
					clock += 1200;
			}
			else
				if (clock == 1200)
					clock += 1200;

			if (_flagError == NONE)
				pop ();
			break;
		}
	}

	if (clock > 2359)
		clock -= 2400;

	if (!Time::_valid_clock (clock))
		_flagError = DATA;

	if (_flagError != NONE) {
		clock = Time::INF_CLOCK;
	}

	return clock;
}

void CmdControl::get_date (int& day, int& mnth, int& year) {
	string strDate = _dataInput->front ();
	int size = strDate.size ();
	Time::date_t date;
	day = -1;
	mnth = -1;
	year = -1;

	switch (size) {
	case 10:
		if ((strDate[2] == '.' || strDate[2] == '/' || strDate[2] == '-') &&
			(strDate[5] == '.' || strDate[5] == '/' || strDate[5] == '-')) {
				_dataInput->front ().erase (5, 1);
				_dataInput->front ().erase (2, 1);
				get_date (day, mnth, year);
		}
		break;
	case 8:
		if ((strDate[2] == '.' || strDate[2] == '/' || strDate[2] == '-') && 
			(strDate[5] == '.' || strDate[5] == '/' || strDate[5] == '-')) {
				_dataInput->front ().erase (5, 1);
				_dataInput->front ().erase (2, 1);
				get_date (day, mnth, year);
		} else {
			date = convertToInt (strDate);
			if (date <= Time::INF_DATE) {
				if (_dayMonth) {
					day = date / 1000000;
					mnth = date / 10000 - day * 100;
					year = date % 10000;					
				} else {
					mnth = date / 1000000;
					day = date / 10000 - mnth * 100;
					year = date % 10000;
				}
			}
		}
		break;
	case 6:
		date = convertToInt (strDate);
		if (date <= Time::INF_DATE) {
			if (_dayMonth) {
				day = date / 10000;
				mnth = date / 100 - day * 100;
			} else {
				mnth = date / 10000;
				day = date / 100 - mnth * 100;
			}
			
			year = date % 100;
			if (year < 70)
				year += 2000;
			else
				year += 1900;
		}
		break;
	case 9:
		convertToInt (strDate.substr (0, 3), day, mnth);
		if (mnth != 0) {
			date = convertToInt (strDate.substr (3));
			day = date / 10000;
			year = date % 10000;
		} else {
			convertToInt (strDate.substr (2, 3), day, mnth);
			if (mnth != 0) {
				day = convertToInt (strDate.substr (0, 2));
				year = convertToInt (strDate.substr (size - 4));
			} else {
				convertToInt (strDate.substr (4, 3), day, mnth);
				if (mnth != 0) {
					day = convertToInt (strDate.substr (size - 2));
					year = convertToInt (strDate.substr (0, 4));
				} else {
					day = -1;
					mnth = -1;
					year = -1;
				}
			}
		}
		break;
	case 5:
		convertToInt (strDate.substr (0, 3), day, mnth);
		if (mnth != 0) {
			day = convertToInt (strDate.substr (3));
		} else {
			convertToInt (strDate.substr (2, 3), day, mnth);
			if (mnth != 0) {
				day = convertToInt (strDate.substr (0, 2));
			} else {
				day = -1;
				mnth = -1;
				year = -1;
			}
		}
		break;
	case 4:
		year = convertToInt (strDate);
		break;
	case 3:
		convertToInt (strDate, day, mnth);
		day = -1;
		if (mnth == 0)
			mnth = -1;
		break;
	case 2:
		day = convertToInt (strDate);
		break;
	default:
		break;
	}

	switch (size) {
	case 10:
	case 8:
	case 6:
		if (_flagError != DATA && mnth > 12 && day <= 12) {
			int temp = mnth;
			mnth = day;
			day = temp;
			_dayMonth = !_dayMonth;
		}
	}
}

int CmdControl::convertToInt (string str) {
	int num = 0;
	int size = str.size ();

	for (int i = 0; i < size; i++) {
		if (str[i] < '0' || str[i] > '9') {
			num = -1;
			break;
		}
		num *= 10;
		num += str[i] - '0';
	}

	return num;
}

void CmdControl::convertToInt (string str, int& day, int& mnth) {
	day = 7;
	mnth = 0;
	if (str == "Sun" || str == "sun")
		day = 0;
	else if (str == "Mon" || str == "mon")
		day = 1;
	else if (str == "Tue" || str == "tue")
		day = 2;
	else if (str == "Wed" || str == "wed")
		day = 3;
	else if (str == "Thu" || str == "thu")
		day = 4;
	else if (str == "Fri" || str == "fri")
		day = 5;
	else if (str == "Sat" || str == "sat")
		day = 6;
	else if (str == "Jan" || str == "jan")
		mnth = 1;
	else if (str == "Feb" || str == "feb")
		mnth = 2;
	else if (str == "Mar" || str == "mar")
		mnth = 3;
	else if (str == "Apr" || str == "apr")
		mnth = 4;
	else if (str == "May" || str == "may")
		mnth = 5;
	else if (str == "Jun" || str == "jun")
		mnth = 6;
	else if (str == "Jul" || str == "jul")
		mnth = 7;
	else if (str == "Aug" || str == "aug")
		mnth = 8;
	else if (str == "Sep" || str == "sep")
		mnth = 9;
	else if (str == "Oct" || str == "oct")
		mnth = 10;
	else if (str == "Nov" || str == "nov")
		mnth = 11;
	else if (str == "Dec" || str == "dec")
		mnth = 12;
	else;
}

bool CmdControl::notMorning (string str) {
	if (str.size () == 2 && (str[0] == 'p' || str[0] == 'P') && (str[1] == 'm' || str[1] == 'M'))
		return true;
	else if (str.size () == 2 && (str[0] == 'a' || str[0] == 'A') && (str[1] == 'm' || str[1] == 'M'))
		return false;
	else {
		_flagError = DATA;
		return false;
	}
}

bool CmdControl::notMorning () {
	if (!_sequence->empty () && _sequence->front () == DATA) {
		string str = _dataInput->front ();
		if (str.size () == 2 && (str[0] == 'p' || str[0] == 'P') && (str[1] == 'm' || str[1] == 'M')) {
			pop ();
			return true;
		} else if (str.size () == 2 && (str[0] == 'a' || str[0] == 'A') && (str[1] == 'm' || str[1] == 'M')) {
			pop ();
			return false;
		} else;
	}
	return false;
}

string CmdControl::executePROMPT () {
	string str;
	if (!_sequence->empty () && _sequence->front () == CMD) {
		switch (_cmdInput->front ()) {
		case CADD:
			pop ();
			if ((!_sequence->empty () && _sequence->front () == DATA) &&
				(_dataInput->front () == "Y" || _dataInput->front () == "y")) {
				_toDoMngr.add (_tableName, _task, true);
				str = ToDoMngr::view (_task) + MSG_ADDED;
				clearTaskElement ();
			} else;
			pop ();
			break;
		case CEDIT:
			pop ();
			if ((!_sequence->empty () && _sequence->front () == DATA) &&
				(_dataInput->front () == "Y" || _dataInput->front () == "y")) {
				_toDoMngr.edit (_taskId, &_taskElement, &_task, true);
				str = ToDoMngr::view (_task) + MSG_EDITED;
				clearTaskElement ();
			}
			pop ();
			break;
		default:
			break;
		}	
	} else;

	deactivate2ndQs ();
	return str;
}

void CmdControl::checkIfStandAloneCmd () {
	command cmd;
	bool found = search_standAloneCmd (_input, cmd);

	if (found && cmd != CCOMMAND) {
		push (cmd);
		_input.erase ();
	} else if (cmd == CCOMMAND) {
		push (CHELP);
		push (CCOMMAND);
		_input.erase ();
	} else;
}

VldCmdCtrl::command CmdControl::translateCmd (string str) {
	command cmd;
	bool found = search_vldCmd (str, cmd);
	
	if (!found)
		_flagError = CMD;
	else {
		command prev = CVOID;
		if (!_cmdInput->empty ())
			prev = _cmdInput->back ();

		bool vldExt = checkIfVldExtension (cmd, prev);

		if (!vldExt)
			_flagError = CMD;
	}

	return cmd;
}

void CmdControl::mergeAllQs () {
	if (!_2ndQs) return;

	queue<string> dataQ = _1stData;
	queue<input_t> seqQ = _1stSeq;
	queue<command> cmdQ = _1stCmd;

	_sequence = &_1stSeq;
	_cmdInput = &_1stCmd;
	_dataInput = &_1stData;

	*_sequence = _2ndSeq;
	*_cmdInput = _2ndCmd;
	*_dataInput = _2ndData;

	append (_sequence, seqQ);
	append (_cmdInput, cmdQ);
	append (_dataInput, dataQ);

//cout << "size of _1st " << _1stSeq.size () << " " << _1stCmd.size () << " " << _1stData.size () << endl;

	clear (&_2ndSeq);
	clear (&_2ndCmd);
	clear (&_2ndData);

//cout << "size of _2nd " << _2ndSeq.size () << " " << _2ndCmd.size () << " " << _2ndData.size () << endl;

	_2ndQs = false;
}

void CmdControl::activate2ndQs () {
log.start ("activate2ndQs");
	_2ndQs = true;
	_sequence = &_2ndSeq;
	_cmdInput = &_2ndCmd;
	_dataInput = &_2ndData;
log.end ();
}

void CmdControl::deactivate2ndQs () {
	if (!_2ndQs) return;
	_2ndQs = false;
	clear ();
	_sequence = &_1stSeq;
	_cmdInput = &_1stCmd;
	_dataInput = &_1stData;
}

void CmdControl::push (command cmd) {
	_cmdInput->push (cmd);
	_sequence->push (CMD);
}

void CmdControl::push (string data) {
	_dataInput->push (data);
	_sequence->push (DATA);
}

bool CmdControl::pop () {
	if (!_sequence->empty ()) {

//cout << _2ndQs << " " << _sequence->front () << " ";

		switch (_sequence->front ()) {
		case DATA:
//cout << _dataInput->front () << endl;
			if (!_2ndQs)
				_2ndData.push (_dataInput->front ());
			_dataInput->pop ();
			break;
		case CMD:
//cout << convertToString (_cmdInput->front ()) << endl;
			if (!_2ndQs)
				_2ndCmd.push (_cmdInput->front ());
			_cmdInput->pop ();
			break;
		default:
			break;
		}

		if (!_2ndQs)
			_2ndSeq.push (_sequence->front ());
		_sequence->pop ();

		if (_sequence->empty () && _2ndQs)
			deactivate2ndQs ();

		return true;
	} else
		return false;
}

void CmdControl::clear () {
	clear (_sequence);
	clear (_cmdInput);
	clear (_dataInput);
}

template <typename data_t>
void CmdControl::clear (queue<data_t>* Q) {
	while (!Q->empty ())
		Q->pop ();
}

template <typename data_t>
void CmdControl::append (queue<data_t>* Q1, queue<data_t> Q2) {
	while (!Q2.empty ()) {
		Q1->push (Q2.front ());
		Q2.pop ();
	}
}

template <typename data_t>
queue<data_t>& operator+= (queue<data_t>& Q1, queue<data_t> Q2) {
	while (!Q2.empty ()) {
		Q1.push (Q2.front ());
		Q2.pop ();
	}
	return Q1;
}
