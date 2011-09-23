/*
#include "Command.h"

//#include "TimePeriod.h"
//#include "ToDoMngr.h"
//#include "Template.h"

#include <fstream>
#include <sstream>
using namespace std;

string LOST_FILE = "unable to find a file: ";
string INV_CMD = "unknown command: ";

Command::Command () {
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

Command::Command (string cmdFile, string validCmdFile) {
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

Command::Command (vector<command> cmdList, vector<string> validCmdFile) {
	_cmdList = &cmdList;
	_validCmd = &validCmdFile;
	_flagError = NONE;
}

void Command::loadCmdList () {
	ifstream inFile (_cmdFile);

	if (inFile.is_open ()) {
		command temp;
		string tempStr;
		indx_t indx;
		stringstream str;

		while (getline (inFile, tempStr)){
			str << tempStr;
			str >> temp.cmd >> temp.indx;

			while (str >> indx)
				temp.exe_cmd.push_back (indx);
			
			str.clear ();
		}
	} else {
		throw (LOST_FILE + _cmdFile);
	}
}

void Command::loadValidCmdList ()
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
	while (!Q.empty)
		Q.pop ();
}

void Command::updateInput (string& input) {
	_input = input;
	clear (*_cmdInput);
	clear (*_dataInput);
	clear (*_sequence);
	splitInput ();
};

void Command::addInput (string& input) {
	_input = input;
	splitInput ();
};

void Command::updateInput (queue<command> cmdInput, queue <string> dataInput, queue<input_t> sequence) {
	_cmdInput = &cmdInput;
	_dataInput = &dataInput;
	_sequence = &sequence;
};

void Command::addInput (queue<command> cmdInput, queue <string> dataInput, queue<input_t> sequence) {
	*_cmdInput += cmdInput;
	*_dataInput += dataInput;
	*_sequence += sequence;
}

template <typename data_t>
operator+= (queue<data_t>& Q1, queue<data_t> Q2) {
	while (!Q2.empty ()) {
		Q1.push (Q2.front ());
		Q2.pop ();
	}
}

vector<Command::command> Command::getCmdList () {
	return *_cmdList;
}

Command::input_t Command::getErrorFlag () {
	return _flagError;
}

string Command::getLeftOverInput () {
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

void Command::splitInput () {
	string temp;
	unsigned int end_pos;

	while (!_input.empty ()) {
		if (_input[0] == '.') {
			end_pos = _input.find_first_of (" .");
			temp = _input.substr (0, end_pos);

			command cmd = translateCmd (temp);
			if (_flagError != CMD) {
				_sequence->push (CMD);
				_cmdInput->push (cmd);

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
*/