#include "CmdControl.h"
//#include "ToDoMngr.h"
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;

string LOST_FILE = "unable to find a file: ";
string INV_CMD = "unknown command: ";

CmdControl::CmdControl () {
	_validCmdFile = "vldCmd.txt";
	_flagError = NONE;

	try {
		loadValidCmdList ();
	} catch (string message) {
		throw (message);
	}
}

CmdControl::CmdControl (string validCmdFile) {
	_validCmdFile = validCmdFile;
	_flagError = NONE;

	try {
		loadValidCmdList ();
	} catch (string message) {
		throw (message);
	}
}

CmdControl::CmdControl (vector<cmd_pair> validCmd) {
	_validCmd = validCmd;
	_flagError = NONE;
}

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
void clear (queue<data_t>& Q) {
	while (!Q.empty ())
		Q.pop ();
}

void CmdControl::updateInput (string& input) {
	_input = input;
	clear (_cmdInput);
	clear (_dataInput);
	clear (_sequence);
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
				_sequence.push (CMD);
				_cmdInput.push (cmd);

				if (end_pos == string::npos || _input[end_pos] != ' ')
					_input.erase (0, end_pos);
				else
					_input.erase (0, end_pos + 1);
			} else
				throw (INV_CMD);
		} else {
			end_pos = _input.find_first_of (" .\n");
			temp = _input.substr (0, end_pos);
			_sequence.push (DATA);
			_dataInput.push (temp);

			if (end_pos != string::npos)
				_input.erase (0, end_pos + 1);
			else
				_input.erase (0, end_pos);
		}
	}
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
	return str;
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