#include "CmdControl.h"
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;

string CmdControl::LOST_FILE = "unable to find a file: ";
string CmdControl::INV_CMD = "unknown command: ";
string CmdControl::INV_DATA = "invalid data input: ";
string CmdControl::MSG_CLEAR = "All your tasks have been deleted!";
string CmdControl::MSG_CLASH = " is clashed with the following:\n";
string CmdControl::MSG_ERROR = "\n\tERROR!!\n";

CmdControl::CmdControl () {
	_validCmdFile = "vldCmd.txt";
	_flagError = NONE;
	_dayMonth = true;
	_force = false;
	_activeListAccessible = false;
	_search = ToDoMngr::search_t::SEACH;

	try {
		loadValidCmdList ();
	} catch (string message) {
		throw (message);
	}
}
/*
CmdControl::CmdControl (string validCmdFile) {
	_validCmdFile = validCmdFile;
	_flagError = NONE;
	_dayMonth = true;

	try {
		loadValidCmdList ();
	} catch (string message) {
		throw (message);
	}
}

CmdControl::CmdControl (vector<cmd_pair> validCmd) {
	_validCmd = validCmd;
	_flagError = NONE;
	_dayMonth = true;
}
*/
void CmdControl::loadValidCmdList ()
{
	ifstream inFile (_validCmdFile);

	if (inFile.is_open ()) {
		cmd_pair cmd;
		int cmdIndx;
		while (inFile >> cmdIndx && inFile >> cmd.str_cmd) {
			cmd.enum_cmd = convertToCommand (cmdIndx);
			_validCmd.push_back (cmd);
		}
	} else
		throw (LOST_FILE + _validCmdFile);
}

template <typename data_t>
void CmdControl::clear (queue<data_t>& Q) {
	while (!Q.empty ())
		Q.pop ();
}

void CmdControl::updateInput (string& input) {
	_input = input;
	clear ();
	try {
		splitInput ();
	} catch (string message) {
		throw (message);
	}
};

void CmdControl::addInput (string& input) {
	_input = input;
	try {
		splitInput ();
	} catch (string message) {
		throw (message);
	}
};

template <typename data_t>
queue<data_t>& operator+= (queue<data_t>& Q1, queue<data_t> Q2) {
	while (!Q2.empty ()) {
		Q1.push (Q2.front ());
		Q2.pop ();
	}
	return Q1;
}

CmdControl::input_t CmdControl::getErrorFlag () {
	return _flagError;
}

void CmdControl::splitInput () {
	string temp;
	unsigned int end_pos = 0;

	while (!_input.empty ()) {
		if (_input[0] == '.' || _input[0] == '-') {
			end_pos = _input.find_first_of (" .-", 1);
			temp = _input.substr (0, end_pos);
			command cmd = translateCmd (temp);
			if (_flagError != CMD) {
				push (CMD);
				if (end_pos == string::npos || _input[end_pos] != ' ')
					_input.erase (0, end_pos);
				else
					_input.erase (0, end_pos + 1);
			} else
				throw (INV_CMD);
		} else {
			end_pos = _input.find_first_of (" .\n");
			temp = _input.substr (0, end_pos);
			push (DATA);
			if (end_pos != string::npos)
				_input.erase (0, end_pos + 1);
			else
				_input.erase (0, end_pos);
		}
	}
}

void CmdControl::push (command cmd) {
	_cmdInput.push (cmd);
	_sequence.push (CMD);
}

void CmdControl::push (string data) {
	_dataInput.push (data);
	_sequence.push (DATA);
}

void CmdControl::pop () {
	switch (_sequence.front ()) {
	case DATA:
		_dataInput.pop ();
		break;
	case CMD:
		_cmdInput.pop ();
		break;
	default:
		break;
	}
}

void CmdControl::clear () {
	clear (_sequence);
	clear (_cmdInput);
	clear (_dataInput);
}

CmdControl::command CmdControl::translateCmd (string str) {
	int size = _validCmd.size ();
	bool found = false;
	command cmd = VOID;
	for (int i = 0; !found && i < size; i++) {
		if (str == _validCmd[i].str_cmd) {
			found = true;
			cmd = _validCmd[i].enum_cmd;
		}
	}

	if (!found)
		_flagError = CMD;
	else {	
		//check extension cmd
		command prev = _cmdInput.back ();
		bool wrongExt = true;
		switch (cmd) {
		case COSTOM:
			if (prev == VIEW || prev == SEARCH)
				wrongExt = false;
			break;
		case FORCE:
			if (prev == ADD || prev == EDIT)
				wrongExt = false;
			break;
		case EXACT:
		case SIMILAR:
		case EACH:
			if (prev == SEARCH)
				wrongExt = false;
			break;
		case COMMAND:
			if (prev == HELP)
				wrongExt = false;
			break;
		default:
			wrongExt = false;
			break;
		}

		if (wrongExt)
			_flagError = CMD;
	}
	return cmd;
}

string CmdControl::convertToString (command cmd) {
	string str;
	switch (cmd) {
	case (COSTOM):		str = "COSTOM";		break;
	case (FORCE):		str = "FORCE";		break;
	case (EXACT):		str = "EXACT";		break;
	case (SIMILAR):		str = "SIMILAR";	break;
	case (EACH):		str = "EACH";		break;
	case (COMMAND):		str = "COMMAND";	break;
	case (TIME):		str = "TIME";		break;
	case (DATE):		str = "DATE";		break;
	case (FROM):		str = "FROM";		break;
	case (TO):			str = "TO";			break;
	case (NAME):		str = "NAME";		break;
	case (VENUE):		str = "VENUE";		break;
	case (NOTE):		str = "NOTE";		break;
	case (ALERT):		str = "ALERT";		break;
	case (REPEAT):		str = "REPEAT";		break;
	case (HIGH):		str = "HIGH";		break;
	case (IMPT):		str = "IMPT";		break;
	case (NOMAL):		str = "NOMAL";		break;
	case (DAY):			str = "DAY";		break;
	case (WEEK):		str = "WEEK";		break;
	case (MONTH):		str = "MONTH";		break;
	case (ADD):			str = "ADD";		break;
	case (EDIT):		str = "EDIT";		break;
	case (DELETE):		str = "DELETE";		break;
	case (TABLE):		str = "TABLE";		break;
	case (VIEW):		str = "VIEW";		break;
	case (REMINDER):	str = "REMINDER";	break;
	case (NEXT):		str = "NEXT";		break;
	case (PREVIOUS):	str = "PREVIOUS";	break;
	case (FIRST):		str = "FIRST";		break;
	case (LAST):		str = "LAST";		break;
	case (UNDO):		str = "UNDO";		break;
	case (REDO):		str = "REDO";		break;
	case (HELP):		str = "HELP";		break;
	case (SORT):		str = "SORT";		break;
	case (SEARCH):		str = "SEARCH";		break;
	case (CLEAR):		str = "CLEAR";		break;
	case (RESET):		str = "RESET";		break;
	case (EXIT):		str = "EXIT";		break;
	default:			str = "VOID";		break;
	}
	return str;
}

CmdControl::command CmdControl::convertToCommand (int indx) {
	command cmd;
	switch (indx) {
	case 0:		cmd = COSTOM;	break;
	case 1:		cmd = FORCE;	break;
	case 2:		cmd = EXACT;	break;
	case 3:		cmd = SIMILAR;	break;
	case 4:		cmd = EACH;		break;
	case 5:		cmd = COMMAND;	break;
	case 6:		cmd = TIME;		break;
	case 7:		cmd = DATE;		break;
	case 8:		cmd = FROM;		break;
	case 9:		cmd = TO;		break;
	case 10:	cmd = NAME;		break;
	case 11:	cmd = VENUE;	break;
	case 12:	cmd = NOTE;		break;
	case 13:	cmd = ALERT;	break;
	case 14:	cmd = REPEAT;	break;
	case 15:	cmd = HIGH;		break;
	case 16:	cmd = IMPT;		break;
	case 17:	cmd = NOMAL;	break;
	case 18:	cmd = DAY;		break;
	case 19:	cmd = WEEK;		break;
	case 20:	cmd = MONTH;	break;
	case 21:	cmd = ADD;		break;
	case 22:	cmd = EDIT;		break;
	case 23:	cmd = DELETE;	break;
	case 24:	cmd = TABLE;	break;
	case 25:	cmd = VIEW;		break;
	case 26:	cmd = REMINDER;	break;
	case 27:	cmd = NEXT;		break;
	case 28:	cmd = PREVIOUS;	break;
	case 29:	cmd = FIRST;	break;
	case 30:	cmd = LAST;		break;
	case 31:	cmd = UNDO;		break;
	case 32:	cmd = REDO;		break;
	case 33:	cmd = HELP;		break;
	case 34:	cmd = SORT;		break;
	case 35:	cmd = SEARCH;	break;
	case 36:	cmd = CLEAR;	break;
	case 37:	cmd = RESET;	break;
	case 38:	cmd = EXIT;		break;
	default:	cmd = VOID;		break;
	}

	return cmd;
}

string CmdControl::executeCmd () {
	string str;
/*
	while (_sequence.empty () == false) {
		if (_sequence.front () == CMD) {
			str += convertToString (_cmdInput.front ());
			str += " ";
			_cmdInput.pop ();
		} else if (_sequence.front () == DATA) {
			str += _dataInput.front ();
			str += " ";
			_dataInput.pop ();
		} else
			str += "\nError\n";
		_sequence.pop ();
	}
*//*
	do {
		cin >> str;
		_sequence.push (DATA);
		_dataInput.push (str);
//		cout << _sequence.size () << endl;
	} while (str != "end");

//	Time::clk_t clock = get_clock ();
//	cout << clock << endl;
	Time time;
	time = get_time ();
	cout << time.string_date () << " " << time.string_clock () << endl;
	cout << _sequence.size () << endl;
*/
	return str;
}

string CmdControl::executeCmd (command cmd) {
	string str;

	switch (cmd) {
	case ADD:
		str = executeADD ();
		break;
	case EDIT:
		str = executeEDIT ();
		break;
	case DELETE:
		str = executeDELETE ();
		break;
	case TABLE:
		str = executeTABLE ();
		break;
	case VIEW:
		str = executeVIEW ();
		break;
	case REMINDER:
		str = _toDoMngr.reminder ();
	case FIRST:
		while (executePREV ());
		break;
	case LAST:
		while (executeNEXT ());
		break;
	case NEXT:
		if (!executeNEXT ())
			str = MSG_NO_NEXT;
		break;
	case PREVIOUS:
		if (!executePREV ())
			str = MSG_NO_PREV;
		break;
	case UNDO:
		_toDoMngr.undo ();
		break;
	case REDO:
		_toDoMngr.redo ();
		break;
	case SORT:
		break;
	case CLEAR:
		_toDoMngr.clear ();
		str = MSG_CLEAR;
		break;
	case RESET:
		reset ();
		break;
	case HELP:
		if (_sequence.front () == CMD && _cmdInput.front () == COMMAND) {
			pop ();
			str = _toDoMngr.help ("cmd");
		} else if (_sequence.front () == DATA) {
			str = _toDoMngr.help (_dataInput.front ());
			pop ();
		} else {
			str = _toDoMngr.help ("");
		}
		break;
	case EXIT:
		if (_sequence.front () == CMD && _cmdInput.front () == TABLE) {
			pop ();
			_tableName.erase ();
		} else
			_toDoMngr.exit ();
		break;
	default:
		break;
	}

	if (cmd == VIEW)
		_activeListAccessible = true;
	else
		_activeListAccessible = false;

	return str;
}

string CmdControl::executeTABLE () {
	string str;
	pop ();

	if (_sequence.front () == DATA && !getTableName ()) {
		string tableName = _dataInput.front ();
		pop ();

		if (_sequence.front () == CMD && (_cmdInput.front () == FROM || _cmdInput.front () == TO)) {
			tabPeriod:
				TimePeriod period = get_period ();
			
			if (_flagError != DATA) {
				_toDoMngr.newTable (tableName, period);
				_tableName = tableName;
			} else {
				if (promptToGetValidInput (MSG_WRONG_PERIOD))
					goto tabPeriod;
			}
		}
	} else {
		str = _toDoMngr.viewTableNames ();
	}
	return str;
}

string CmdControl::executeDELETE () {
	string str;
	switch (_sequence.front ()) {
	case DATA:
		if (_activeListAccessible) {
			delID:
				string data = _dataInput.front ();
				int taskId = convertToInt (data);

			Task task = _toDoMngr.erase (taskId);
			if (task.get_index () == 0) {
				if (promptToGetValidInput (MSG_WRONG_ID))
					goto delID;
			} else {
				str = ToDoMngr::view (task) + MSG_DELETED;
				pop ();
			}
		}
		break;
	case CMD:
		switch (_cmdInput.front ()) {
		case FROM:
		case TO:
			str = executeFunction (_toDoMngr.erase);
			break;
		case TABLE:
			pop ();
			while (!getTableName ())
				promptToGetValidInput (MSG_WRONG_TABLE);
			
			_toDoMngr.erase (_tableName);
			break;
		default:
			break;
		}
	default:
		break;
	}
	return str;
}

string CmdControl::executeVIEW () {
	string str;

	ToDoMngr::view_t viewType = ToDoMngr::view_t::DAILY;
	if (_sequence.front () == CMD) {
		switch (_cmdInput.front ()) {
		case DAY:
			viewType = ToDoMngr::view_t::DAILY;
			pop ();
			break;
		case WEEK:
			viewType = ToDoMngr::view_t::WEEKLY;	
			pop ();
			break;
		case MONTH:
			viewType = ToDoMngr::view_t::MONTHLY;
			pop ();
			break;
		default:
			break;
		}
	}

	switch (_sequence.front ()) {
	case DATA:
		if (_activeListAccessible) {
			viewID:
				string data = _dataInput.front ();
				int taskId = convertToInt (data);

			str = _toDoMngr.view (taskId);
			if (str.empty ()) {
				if (promptToGetValidInput (MSG_WRONG_ID))
					goto viewID;
			} else {
				_dataInput.pop ();
				_sequence.pop ();
			}
		}
		break;
	case CMD:
		switch (_cmdInput.front ()) {
		case FROM:
		case TO:
			str = executeFunction (_toDoMngr.view);
			break;
		case TIME:
		case DATE:
			pop ();
			viewTime:
				Time time;
				time.modify_date (get_date ());
			
			if (_flagError != DATA) {
				str = _toDoMngr.view (viewType, time);		
			} else {
				if (promptToGetValidInput (MSG_WRONG_DATE))
					goto viewTime;  
			}
			break;
		case TABLE:
			pop ();
			while (!getTableName ()) {
				promptToGetValidInput (MSG_WRONG_TABLE);
			}
				
			str = _toDoMngr.view (viewType, _tableName);
			break;
		default:
			break;
		}
		break;
	default:
		if (!_tableName.empty ())
			str = _toDoMngr.view (viewType, _tableName);
		break;
	}

	return str;
}

string CmdControl::executeADD () {
	string str;

	if (_sequence.front () == CMD && _cmdInput.front () == FORCE) {
		pop ();
		_force = true;
	}
	Task task = get_task ();
		
	add:
		*_taskList = _toDoMngr.add (_tableName, task, _force);

	if (!_force && !_taskList->empty ()) {
		string message = ToDoMngr::view (task) + MSG_CLASH + ToDoMngr::view (taskList);
		if (promptToContinue (message)) {
			_force = true;
			goto add;
		}
	} else if (_force && !_taskList->empty ()) {
		str = MSG_ERROR;
	} else {
		str = ToDoMngr::view (task) + MSG_ADDED;
	}
		
	_taskList->clear ();
	return str;
}

string CmdControl::executeFunction (void* function (TimePeriod)) {
	string str;
	getPeriod:
		TimePeriod period = get_period ();
			
	if (_flagError != DATA) {
		str = _toDoMngr.view (period);		
	} else {
		if (promptToGetValidInput (MSG_WRONG_PERIOD))
			goto getPeriod;  
	}
}

Time CmdControl::get_time () {
	Time time;
	if (_flagError == DATA)
		_flagError = NONE;

	time.modify_date (get_date ());
	time.modify_clock (get_clock ());

	if (_flagError != DATA && time.get_date () == Time::INF_DATE && time.get_clock () != Time::INF_CLOCK) {
		_flagError = NONE;
		Time curr;
		curr.current_time ();

		if (time.get_clock () < curr.get_clock ())
			curr.modify_date (curr.get_date () + 1000000);

		time.modify_date (curr.get_date ());
	}

	return time;
}

Time::date_t CmdControl::get_date () {
	string strDate = _dataInput.front ();
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
			_dataInput.front ().erase (0, 3);
	} else;
	mnth = -1;

	while (_sequence.front () == DATA && (day == -1 || mnth == -1 || year == -1)) {
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
	
	Time currTime;
	currTime.current_time ();
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
			currDate = day * 1000000 + mnth * 100000 + currDate % 10000;	//substitute with user's day and mnth
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

void CmdControl::get_date (int& day, int& mnth, int& year) {
	string strDate = _dataInput.front ();
	int size = strDate.size ();
	Time::date_t date;
	day = -1;
	mnth = -1;
	year = -1;

	switch (size) {
	case 10:
		if ((strDate[2] == '.' || strDate[2] == '/' || strDate[2] == '-') &&
			(strDate[5] == '.' || strDate[5] == '/' || strDate[5] == '-')) {
				_dataInput.front ().erase (5, 1);
				_dataInput.front ().erase (2, 1);
				get_date (day, mnth, year);
		}
		break;
	case 8:
		if ((strDate[2] == '.' || strDate[2] == '/' || strDate[2] == '-') && 
			(strDate[5] == '.' || strDate[5] == '/' || strDate[5] == '-')) {
				_dataInput.front ().erase (5, 1);
				_dataInput.front ().erase (2, 1);
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

Time::clk_t CmdControl::get_clock () {
	string strClk = _dataInput.front ();
	int size = strClk.size ();
	int clock = 0;
	if (_flagError == DATA)
		_flagError = NONE;

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

	if (_flagError != DATA) {
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

			if (_flagError != DATA)
				pop ();
			break;
		}
	}

	if (clock > 2359)
		clock -= 2400;

	if (!Time::_valid_clock (clock))
		_flagError = DATA;

	if (_flagError == DATA)
		clock = Time::INF_CLOCK;

	return clock;
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
	if (_sequence.front () == DATA) {
		string str = _dataInput.front ();
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

template <typename data_t>
int binary_find (vector<data_t>& vect, data_t& data) {
	return find (vect, data, 0, vect.size () - 1);
}

template <typename data_t>
int binary_find (vector<data_t>& vect, data_t& data, int min, int max) {
	int mid = (min + max) / 2;
	if (vect[mid] == data)
		return mid;
	else if (vect[mid] > data)
		return find (vect, data, min, mid - 1);
	else if (vect[mid] < data)
		return find (vect, data, mid + 1, max);
}