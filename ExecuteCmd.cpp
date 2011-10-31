#include "ExecuteCmd.h"
#include <sstream>
#include <vector>
#include <list>
#include <iostream>
using namespace std;

template <typename data_t>
void append_back (list<data_t>* main, list<data_t> sub) {
	while (!sub.empty ()) {
		main->push_back (sub.front ());
		sub.pop_front ();
	}
}

string ExecuteCmd::BREAK = "[break]";
string ExecuteCmd::MORE = "[more]";

ExecuteCmd::ExecuteCmd () {
	_sequence = NULL;
	_dataInput = NULL;
	_cmdInput = NULL;
	_splitedInput = NULL;
}

ExecuteCmd::ExecuteCmd (bool dotCmd) {
	_validCmdFile = "vldCmd2.txt";
	_dfltCmdFile = "dfltCmd2.txt";

	_flagError = NONE;
	try {
		load_vldCmdList ();
	} catch (string message) {
		throw (message);
	}

	_sequence = new queue<input_t>;
	_dataInput = new queue<string>;
	_cmdInput = new queue<command>;
	_splitedInput = new list<string>;
}

ExecuteCmd::~ExecuteCmd () {
	clear ();
}

void ExecuteCmd::clear () {
	if (_sequence != NULL) {
cout << "-CA";
cout << _sequence << endl;
		delete _sequence;
		_sequence = NULL;
	} else {
cout << "-CB";
	}

	if (_dataInput != NULL) {
		delete _dataInput;
		_dataInput = NULL;
	}

	if (_cmdInput != NULL) {
		delete _cmdInput;
		_cmdInput = NULL;
	}

	if (_splitedInput != NULL) {
		delete _splitedInput;
		_splitedInput = NULL;
	}
}

void ExecuteCmd::updateInput (string& input) {
	clear ();
	_sequence = new queue<input_t>;
	_dataInput = new queue<string>;
	_cmdInput = new queue<command>;
	_splitedInput = new list<string>;
	_input = input;
	_flagError = NONE;
	splitInput ();
};

string ExecuteCmd::get_input () {
	string input;
	int size = _sequence->size ();
	if (_flagError == NONE) {
		while (_splitedInput->size () > size) {
			_splitedInput->pop_front ();
		}

		while (!_splitedInput->empty ()) {
			input += _splitedInput->front () + " ";
			_splitedInput->pop_front ();
		}
	} else {
		while (_splitedInput->size () > size) {
			input += _splitedInput->front () + " ";
			_splitedInput->pop_front ();
		}

		if (size > 0)
			input += BREAK + " ";
		else
			input += MORE + " ";

		while (!_splitedInput->empty ()) {
			input += _splitedInput->front () + " ";
			_splitedInput->pop_front ();
		}		
	}
	return input.substr (0, input.size () - 1);
}

string ExecuteCmd::result () {
	return _result;
}

bool ExecuteCmd::execute () {
	return false;
}

void ExecuteCmd::splitInput () {
	string temp;
	command cmd;
	unsigned int end_pos = 0;

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

		_splitedInput->push_back (temp);
	}
}

//some elements are unexpectedly modified :(
Task ExecuteCmd::get_task () {
	Task task;
	if (!_sequence->empty () && _sequence->front () == DATA)
		task.note = mergeStringInput ();
	
	bool reachExeCmd = false;
	while (!reachExeCmd && _flagError == NONE && !_sequence->empty ()) {
		if (_sequence->front () != CMD) {
			_flagError = DATA;
			break;
		}

		Time time;
		TimePeriod period;
		switch (_cmdInput->front ()) {
		case CDATE:
		case CTIME:
			pop ();
		case CTODAY:
		case CTMR:
		case CNOW:
			time = get_time ();
			if (_flagError == NONE)
				task.modify_time (time);
			break;
		case CTO:
			_flagError = CMD;
			break;
		case CFROM:
			period = get_period ();
			if (_flagError == NONE)
				task.modify_period (period);
			break;
		case CVENUE:
			pop ();
			task.venue = mergeSimStringInput ();
			break;
		case CNOTE:
			pop ();
			task.note = mergeSimStringInput ();
			break;
		case CALERT:
			pop ();
			time = get_time ();
			if (_flagError == NONE)
				task.alert = time;
			break;
		case CREPEAT:
			pop ();
			if (_sequence->front () == CMD) {
				switch (_cmdInput->front ()) {
				case CHOUR:
					task.repeat = 60;
					pop ();
					break;
				case CFORTNIGHT:
					task.repeat = 14 * Time::DAY;
					pop ();
					break;
				case CYEAR:
					task.repeat = 2;
					pop ();
					break;
				case CDAY:
					task.repeat = Time::DAY;
					pop ();
					break;
				case CWEEK:
					task.repeat = 7 * Time::DAY;
					pop ();
					break;
				case CMONTH:
					task.repeat = 1;
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

					task.repeat = mins;
					pop ();
				} else {
					Time::clk_t clock = get_clock ();
					if (_flagError == NONE)
						task.repeat = clock / 100 * 60 + clock % 100;
				}
			} else;
			break;
		default:
			reachExeCmd = true;
			break;
		}
	}

	return task;
}

string ExecuteCmd::mergeStringInput () {
	string str;
	while (_sequence->empty () == false && _sequence->front () == DATA) {
		str += _dataInput->front () + " ";
		pop ();
	}
	str = str.substr (0, str.size () - 1);
	return str;
}

string ExecuteCmd::mergeSimStringInput () {
	string str;
	bool finish = false;
	while (_sequence->empty () == false && !finish) {
		if (_sequence->front () == DATA) {
			str += _dataInput->front () + " ";
			pop ();
		} else if (_sequence->front () == CMD) {
			switch (_cmdInput->front ()) {
			case CFORCE:
			case CCOSTOM:
			case CEXACT:
			case CSIMILAR:
			case CEACH:
			case CCOMMAND:
			case CHOUR:
			case CFORTNIGHT:
			case CYEAR:
			case CDAY:
			case CWEEK:
			case CMONTH:
			case CDISCARD:
			case CREPLACE:
			case CINSERT:
				str += convertToString (_cmdInput->front ());
				pop ();
				break;
			default:
				finish = true;
				break;
			}
		}
	}

	return str;
}

//Format1: FROM time1 TO time2         time1 <= time2 else _flagError = DATA
//Format2: FROM date1 clock1 TO clock2 if clock1 > clock2, date2 = date1 + 1day
//if time1 || time2 is not specified, _flagError = CMD
//if time1 || time2 is invalid, _flagError = DATA
TimePeriod ExecuteCmd::get_period () {
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
Time ExecuteCmd::get_time () {
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
Time::date_t ExecuteCmd::get_date () {
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
Time::clk_t ExecuteCmd::get_clock () {
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

void ExecuteCmd::get_date (int& day, int& mnth, int& year) {
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
				day = date / 1000000;
				mnth = date / 10000 - day * 100;
				year = date % 10000;					
			}
		}
		break;
	case 6:
		date = convertToInt (strDate);
		if (date <= Time::INF_DATE) {
			day = date / 10000;
			mnth = date / 100 - day * 100;			
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
}

int ExecuteCmd::convertToInt (string str) {
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

void ExecuteCmd::convertToInt (string str, int& day, int& mnth) {
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

bool ExecuteCmd::notMorning (string str) {
	if (str.size () == 2 && (str[0] == 'p' || str[0] == 'P') && (str[1] == 'm' || str[1] == 'M'))
		return true;
	else if (str.size () == 2 && (str[0] == 'a' || str[0] == 'A') && (str[1] == 'm' || str[1] == 'M'))
		return false;
	else {
		_flagError = DATA;
		return false;
	}
}

bool ExecuteCmd::notMorning () {
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

VldCmdCtrl::command ExecuteCmd::translateCmd (string str) {
	command cmd;
	bool found = search_vldCmd (str, cmd);
	
	if (!found)
		_flagError = CMD;
	else {
		command prev = CVOID;
		if (!_cmdInput->empty ())
			prev = _cmdInput->back ();
	}

	return cmd;
}

void ExecuteCmd::push (command cmd) {
	_cmdInput->push (cmd);
	_sequence->push (CMD);
}

void ExecuteCmd::push (string data) {
	_dataInput->push (data);
	_sequence->push (DATA);
}

bool ExecuteCmd::pop () {
	if (!_sequence->empty ()) {

		switch (_sequence->front ()) {
		case DATA:
			_dataInput->pop ();
			break;
		case CMD:
			_cmdInput->pop ();
			break;
		default:
			break;
		}

		_sequence->pop ();
		return true;
	} else
		return false;
}

//-----------------------------------------------------------------

string Add::MSG_CLASH = " is clashed with the following:\n";
string Add::MSG_ADDED = " is added.";

Add::Add (vector<cmd_pair> validCmd, ToDoMngr* toDoMngr) {
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
		} else {
			_result = ToDoMngr::view (task) + MSG_ADDED;
			done = true;
		}
	} else {
		done = false;
	}

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
	_validCmd = validCmd;
	_toDoMngr = toDoMngr;
	_taskId = 0;
	_first = false;
	_last = false;
	_viewType = DAILY;
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
				_result = _toDoMngr->view (_viewType, _time);
			}
			break;
		case CTABLE:
			pop ();
			if (!_sequence->empty () && _sequence->front () == DATA) {
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

	return done;
}

void View::next () {
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
	string temp;
	
	while (!_first) {
		prev ();
		if (!_first)
			temp = _result;
	}
	_result = temp;
}

void View::last () {
	string str;
	string temp;
	
	while (!_last) {
		next ();
		if (!_last)
			temp = _result;
	}
	_result = temp;
}

//---------------------------------------------------------------

Reminder::Reminder (ToDoMngr* toDoMngr) {
	_toDoMngr = toDoMngr;
}

bool Reminder::execute () {
	_result = _toDoMngr->reminder ();
	return true;
}

//---------------------------------------------------------------

History::History (ToDoMngr* toDoMngr) {
	_toDoMngr = toDoMngr;
	_undo = true;
}

void History::undo (bool __undo) {
	_undo = __undo;
}

bool History::execute () {
	if (_undo) {
		_toDoMngr->undo ();
	} else {
		_toDoMngr->redo ();
	}
	return true;
}

//---------------------------------------------------------------

string Delete::MSG_DELETED = " is deleted.";

Delete::Delete (vector<cmd_pair> validCmd, ToDoMngr* toDoMngr) {
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

	if (_flagError == NONE)
		return true;
	else
		return false;
}