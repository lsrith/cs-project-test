#include "CmdControl.h"
#include "ToDoMngr.h"
#include <fstream>
#include <sstream>
using namespace std;

string LOST_FILE = "unable to find a file: ";
string INV_CMD = "unknown command: ";

CmdControl::CmdControl () {
	_cmdFile = "cmdIdx.txt";
	_validCmdFile = "vldCmd.txt";
	_flagError = NONE;

	try {
		loadCmdList ();
		loadValidCmdList ();
	} catch (string message) {
		throw (message);
	}
}

CmdControl::CmdControl (string cmdFile, string validCmdFile) {
	_cmdFile = cmdFile;
	_validCmdFile = validCmdFile;
	_flagError = NONE;

	try {
		loadCmdList ();
		loadValidCmdList ();
	} catch (string message) {
		throw (message);
	}
}

CmdControl::CmdControl (vector<command> cmdList, vector<string> validCmdFile) {
	_cmdList = &cmdList;
	_validCmd = &validCmdFile;
	_flagError = NONE;
}

void CmdControl::loadCmdList () {
	ifstream inFile (_cmdFile);

	if (inFile.is_open ()) {
		string tempStr;
		indx_t indx;
		int counter;
		int MAX_PREV_EXE_CMD;

		inFile >> MAX_PREV_EXE_CMD;

		while (getline (inFile, tempStr)){
			command temp;
			stringstream str;
			str << tempStr;
			str >> temp.cmd;

			counter = 0;
			while (str >> indx) {
				if (counter < MAX_PREV_EXE_CMD) {
					if (indx != 0)
						temp.prev.push_back (indx);
				} else {
					temp.next.push_back (indx);
				}
				
				counter++;
			}

			_cmdList->push_back (temp);
		}
	} else {
		throw (LOST_FILE + _cmdFile);
	}
}

void CmdControl::loadValidCmdList ()
{
	ifstream inFile (_validCmdFile);

	if (inFile.is_open ()) {
		string str;
		while (inFile >> str)
			_validCmd->push_back (str);
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
	clear (*_cmdInput);
	clear (*_dataInput);
	clear (*_sequence);
	splitInput ();
};

void CmdControl::addInput (string& input) {
	_input = input;
	splitInput ();
};

void CmdControl::updateInput (queue<command> cmdInput, queue <string> dataInput, queue<input_t> sequence) {
	_cmdInput = &cmdInput;
	_dataInput = &dataInput;
	_sequence = &sequence;
};

void CmdControl::addInput (queue<command> cmdInput, queue <string> dataInput, queue<input_t> sequence) {
	*_cmdInput += cmdInput;
	*_dataInput += dataInput;
	*_sequence += sequence;
}

template <typename data_t>
queue<data_t>& operator+= (queue<data_t>& Q1, queue<data_t> Q2) {
	while (!Q2.empty ()) {
		Q1.push (Q2.front ());
		Q2.pop ();
	}
	return Q1;
}

vector<CmdControl::command> CmdControl::getCmdList () {
	return *_cmdList;
}

CmdControl::input_t CmdControl::getErrorFlag () {
	return _flagError;
}

string CmdControl::getLeftOverInput () {
	queue<command> cmdInput = *_cmdInput;
	queue<string> dataInput = *_dataInput;
	queue<input_t> sequence = *_sequence;
	ostringstream str;

	while (!sequence.empty ()) {
		switch (sequence.front ()) {
		case CMD:
			str << " ." + cmdInput.front ().cmd;
			cmdInput.pop ();
			break;
		case DATA:
			str << " " + dataInput.front ();
			dataInput.pop ();
			break;
		case NONE:
			break;
		}
		sequence.pop ();
	}
	return str.str ();
}

void CmdControl::splitInput () {
	string temp;
	unsigned int end_pos;

	while (!_input.empty ()) {
		if (_input[0] == '.') {
			end_pos = _input.find_first_of (" .");
			temp = _input.substr (0, end_pos);

			indx_t indx = translateCmd (temp);
			if (_flagError != CMD) {
				_sequence->push (CMD);
				_cmdInput->push (&(_cmdList->at(indx)));

				if (_input[end_pos] == ' ')
					_input.erase (0, end_pos + 1);
				else
					_input.erase (0, end_pos);
			} else
				throw (INV_CMD + temp);
		} else {
			end_pos = _input.find (" .");
			temp = _input.substr (0, end_pos);
			_sequence->push (DATA);
			_dataInput->push (temp);
			_input.erase (0, end_pos + 1);
		}
	}
}

CmdControl::indx_t CmdControl::translateCmd (string str) {
	int	pos = binary_find (*_validCmd, str);
	command cmd;
	indx_t indx = 0;

	if (pos < _validCmd->size ())
		_flagError != CMD;
	else {
		switch (pos) {
		case 0:		/*.a*/
		case 2:		/*.add*/
			indx = 2;
			break;
		case 1:		/*.acc*/
			indx = 1;
			break;
		case 3:		/*.al*/
		case 4:		/*.alert*/
			indx = 3;
			break;
		case 5:		/*.b*/
		case 7:		/*.by*/
		case 55:	/*.on*/
			indx = 49;
			break;
		case 6:		/*.back*/
			indx = 4;
			break;
		case 8:		/*.c*/
			if (_sequence->empty () || _sequence->back () != CMD)
				indx = 9;
			else {
				command* temp = _cmdInput->back ();
				if (temp == &(_cmdList->at (2)) /*add*/|| temp == &(_cmdList->at (16)) /*edit*/) 
					indx = 5;
				else if (temp == &(_cmdList->at (46)) /*srch*/|| temp == &(_cmdList->at (52)) /*view*/)
					indx = 8;
				else
					_flagError = CMD;
			}
			break;
		case 9:		/*.cl*/
		case 12:	/*.clear*/
			indx = 6;
			break;
		case 10:	/*.cla*/
		case 11:	/*.clash*/
		case 13:	/*.clsh*/
			indx = 5;
			break;
		case 14:	/*.cmd*/
		case 15:	/*.command*/
			indx = 7;
			break;
		case 16:	/*.copy*/
		case 18:	/*.cp*/
			indx = 9;
			break;
		case 17:	/*.costom*/
			indx = 8;
			break;
		case 19:	/*.cut*/
			indx = 10;
			break;
		case 20:	/*.d*/
		case 22:	/*.del*/
		case 23:	/*.delete*/
			indx = 14;
			break;
		case 21:	/*.day*/
			indx = 13;
			break;
		case 24:	/*.des*/
		case 25:	/*.description*/
		case 53:	/*.note*/
			indx = 15;
			break;
		case 26:	/*.e*/
		case 27:	/*.edit*/
			indx = 16;
			break;
		case 28:	/*.en*/
		case 29:	/*.enter*/
		case 49:	/*.n*/
		case 51:	/*.new*/
			indx = 17;
			break;
		case 30:	/*.exit*/
			indx = 19;
			break;
		case 31:	/*.f*/
			if (_sequence->empty () || _sequence->back () != CMD)
				indx = 21;
			else {
				command* temp = _cmdInput->back ();
				if (temp == &(_cmdList->at (2)) /*add*/|| temp == &(_cmdList->at (16)) /*edit*/) 
					indx = 20;
				else
					_flagError = CMD;
			}
			break;
		case 32:	/*.force*/
			indx = 20;
			break;
		case 33:	/*.from*/
			indx = 21;
			break;
		case 34:	/*.h*/
		case 35:	/*.help*/
			indx = 22;
			break;
		case 36:	/*.high*/
			indx = 23;
			break;
		case 37:	/*.impt*/
		case 38:	/*.importance*/
			indx = 24;
			break;
		case 39:	/*.l*/	
		case 40:	/*.last*/
			indx = 27;
			break;
		case 41:	/*.loc*/
			indx = 28;
			break;
		case 42:	/*.log*/
			indx = 29;
			break;
		case 43:	/*.m*/
		case 46:	/*.mov*/
		case 47:	/*.move*/
		case 48:	/*.mv*/
			indx = 30;
			break;
		case 44:	/*.mon*/
		case 45:	/*.month*/
			indx = 31;
			break;
		case 50:	/*.name*/
			indx = 32;
			break;
		case 52:	/*.next*/
		case 54:	/*.nx*/
			indx = 33;
			break;
		case 56:	/*.p*/
		case 57:	/*.paste*/
			indx = 39;
			break;
		case 58:	/*.per*/
		case 59:	/*.period*/
			indx = 34;
			break;
		case 60:	/*.pre*/
		default:
			break;
		}
	}

	return indx;
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