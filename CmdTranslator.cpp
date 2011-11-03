#include "CmdTranslator.h"
#include <cassert>
using namespace std;

string CmdTrans::BREAK = "[break]";

CmdTrans::CmdTrans () {
	try {
		load_vldCmdList ();
	} catch (string message) {
		throw (message);
	}

	_sequence = NULL;
	_dataInput = NULL;
	_cmdInput = NULL;
	_splitedInput = NULL;
}

CmdTrans::~CmdTrans () {
	clear ();
}

void CmdTrans::splitInput (string& _input) {
	clear ();
	_sequence = new (nothrow) queue<input_t>;
	_dataInput = new (nothrow) queue<string>;
	_cmdInput = new (nothrow) queue<command>;
	_splitedInput = new (nothrow) list<string>;

	assert (_sequence != NULL && _dataInput != NULL && _cmdInput != NULL && _splitedInput != NULL);

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

string CmdTrans::getLeftOverInput () {
	string input;
	unsigned int size = _sequence->size ();
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

		if (_splitedInput->empty ())
			input += BREAK;
		else
			input += BREAK + " ";

		while (!_splitedInput->empty ()) {
			input += _splitedInput->front () + " ";
			_splitedInput->pop_front ();
		}		
	}
	return input.substr (0, input.size () - 1);
}

//some elements are unexpectedly modified :(
Task CmdTrans::get_task () {
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

string CmdTrans::mergeStringInput () {
	string str;
	while (_sequence->empty () == false && _sequence->front () == DATA) {
		str += _dataInput->front () + " ";
		pop ();
	}
	str = str.substr (0, str.size () - 1);
	return str;
}

string CmdTrans::mergeSimStringInput () {
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
TimePeriod CmdTrans::get_period () {
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
Time CmdTrans::get_time () {
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
Time::date_t CmdTrans::get_date () {
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
Time::clk_t CmdTrans::get_clock () {
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

void CmdTrans::get_date (int& day, int& mnth, int& year) {
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

int CmdTrans::convertToInt (string str) {
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

void CmdTrans::convertToInt (string str, int& day, int& mnth) {
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

bool CmdTrans::notMorning (string str) {
	if (str.size () == 2 && (str[0] == 'p' || str[0] == 'P') && (str[1] == 'm' || str[1] == 'M'))
		return true;
	else if (str.size () == 2 && (str[0] == 'a' || str[0] == 'A') && (str[1] == 'm' || str[1] == 'M'))
		return false;
	else {
		_flagError = DATA;
		return false;
	}
}

bool CmdTrans::notMorning () {
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

VldCmdCtrl::command CmdTrans::translateCmd (string str) {
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

void CmdTrans::push (command cmd) {
	_cmdInput->push (cmd);
	_sequence->push (CMD);
}

void CmdTrans::push (string data) {
	_dataInput->push (data);
	_sequence->push (DATA);
}

bool CmdTrans::pop () {
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

void CmdTrans::clear () {
	if (_sequence != NULL) {
		delete _sequence;
		_sequence = NULL;
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