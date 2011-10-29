#include "Logging.h"
#include <sstream>
#include <fstream>
#include <stack>
#include <queue>
#include <iostream>
using namespace std;

Logging::Logging () {
	_inactive = true;
}

void Logging::log (string file) {
	_inactive = false;
	_file = "Log_" + file + ".txt";
	__tab = 0;
	_start = false;
	_loop = false;
	ofstream outFile (_file);
	outFile.close ();
}

void Logging::start (string function) {
	if (_inactive) return;
	_addLine ("starting: " + function + "()");
	_func.push (function);
	_history.push (START);
	_start = true;
	_loop = false;
}

void Logging::call (string function) {
	if (_inactive || !_start) return;
	if (_func.empty () || (_func.top () != function))
		_start = false;

	_addLine ("calling: " + function + "()...");
	_func.push (function);
	_history.push (CALL);
}

void Logging::loop (string name) {
	if (_inactive || !_start) return;
	_addLine ("looping: " + name + "...");	
	_loop = true;
	__iter.push (0);
	_history.push (LOOP);
}

void Logging::cond (string name) {
	if (_inactive || !_start) return;
	_addLine ("checking: " + name + "...");
	_history.push (COND);
}

void Logging::cond (string name, int cond) {
	if (_inactive || !_start) return;
	ostringstream strStream;
	strStream << "checking: " << name << " for " << cond << "...";
	_addLine (strStream.str ());
	_history.push (COND);
}

void Logging::end () {
	if (_inactive || !_start) return;
	if (_history.empty ()) return;
	
	__tab--;

	if (_loop == true && _history.top () != LOOP) {
		_history.pop ();
		_removeLine ();
	} else if (_loop == true && _history.top () == LOOP) {
		_history.pop ();
		_loop = false;
		__iter.pop ();
		_addLine ("looping finished!");
	} else if (_loop == false && _history.top () == LOOP) {
		_clearHistory ();
		_addLine ("Logging System Error!");
	} else {
		ostringstream strStream;
		switch (_history.top ()) {
		case START:
			_func.pop ();
			_addLine ("function exit!");
			break;
		case COND:
			_addLine ("condition checked!");
			break;
		case CALL:
			_start = true;
			_addLine ("calling successed!");
			break;
		default:
			_clearHistory ();
			_addLine ("Logging System Error!");
			_inactive = true;
			return;
		}

		_history.pop ();

		if (_history.empty ()) {
			_start = false;
		} else if (_history.top () == LOOP) {
			unsigned int iter = __iter.top () + 1;
			__iter.pop ();
			__iter.push (iter);
			_loop = true;
		} else {
			stack<LOG> temp;
			while (!_history.empty ()) {
				if (_history.top () == START) {
					break;
				} else if (_history.top () == LOOP) {
					_loop = true;
					break;
				} else {
					temp.push (_history.top ());
					_history.pop ();
				}
			}

			while (!temp.empty ()) {
				_history.push (temp.top ());
				temp.pop ();
			}
		}
	}
	__tab--;
}

void Logging::_addLine (string str) {
	if (_loop) {
		_replaceLine (str);
	} else {
		ofstream outFile (_file, fstream::app);
		outFile << _tab () << str << endl;
		outFile.close ();
		__tab++;
	}
}

void Logging::_replaceLine (string Str) {
	queue<string> lines;
	string str;
	ifstream inFile (_file);
	while (getline (inFile, str))
		lines.push (str);
	inFile.close ();
	
	ofstream outFile (_file);
	while (lines.size () > 1) {
		outFile << lines.back () << endl;
		lines.pop ();
	}
	outFile << _tab () << Str << endl;
	outFile.close ();
}

void Logging::_removeLine () {
	queue<string> lines;
	string str;

	ifstream inFile (_file);
	while (getline (inFile, str))
		lines.push (str);
	inFile.close ();
	
	ofstream outFile (_file);
	while (lines.size () > 1) {
		outFile << lines.back () << endl;
		lines.pop ();
	}
	outFile.close ();
}

string Logging::_tab () {
	string str;
	for (int i = 0; i < __tab; i++)
		str.append ("\t");
	return str;
}

void Logging::_clearHistory () {
	while (!_history.empty ()) {
		_history.pop ();
	}
}