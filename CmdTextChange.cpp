#include "CmdTextChange.h"
#include <conio.h>
#include <ctype.h>
#include <string>
#include <iostream>
using namespace std;

CmdTextChange::CmdTextChange () {
	try {
		load_vldCmdList ();
	} catch (string message) {
		throw (message);
	}

	initPointers ();
	_insertActive = true;
	_cursor = 0;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
}

CmdTextChange::~CmdTextChange () {
	clear ();
}

string CmdTextChange::read () {
	return read ("");
}

string CmdTextChange::read (string __str) {
	splitString (__str);
	short int ch;
	do {
		buffer ();
		ch = _getch ();
		
		if (ch == '\r') {
			_putch ('\r');
			_putch ('\n');
			ch = '\n';
		} else if (ch == '\n') {
			_putch ('\r');
			_putch ('\n');
		} else if (ch == '\b') {
			bsChar ();
		} else if (ch == SPCMD) {
			spCmdCheck ();
		} else if (ch >= '!' && ch <= '~') {
			insChar (ch);
		} else if (ch == ' ' && (_chList.empty () || _chList.back ()._ch != ' ')) {
				checkWord ();
				insChar (' ');
		} else;
	} while (ch != '\n');
	
	SetConsoleTextAttribute(hConsole, GRAY);
	string str = mergeChar (&_chList);
	_cursor = str.size ();
	return str;
}

//to be modified
void CmdTextChange::checkWord () {
	string str;
	str = getCurrWord ();
	command cmd = convertToCommand (str);
	if (cmd == CNULL) {
		if (prevWord () == BLACK || prevWord () == RED)
			setCurrWord (RED);
		else
			setCurrWord (WHITE);
	} else {
		setCurrWord (SKY);
		if (prevWord () == WHITE) {
			getPrevWords (cmd, str);
			if (ifDataError (cmd, str))
				setPrevWords (RED);
		}
	}
}

bool CmdTextChange::ifDataError (command cmd, string str) {
	splitInput (str);
	switch (cmd) {
	case CFROM:
	case CTO:
	case CTIME:
	case CDATE:
	case CALERT:
		get_time ();
		if (_flagError != NONE)
			return true;
		else
			return false;
		break;
	default:
		return false;
		break;
	}
}

string CmdTextChange::getCurrWord () {
	string str, temp;
	list<chNode>::iterator iter = getNode (_cursor - 1);

	while (iter->_ch != ' ') {
		temp = (char) iter->_ch;
		str.insert (0, temp);

		if (iter == _chList.begin ())
			break;
		else
			iter--;
	}

	return str;
}

void CmdTextChange::getPrevWords (command& cmd, string& str) {
	cmd = CNULL;
	str.erase ();
	string temp, __temp;
	list<chNode>::iterator iter = getNode (_cursor - 1);
	Color currColor = iter->_cl;
	while (iter->_cl == currColor) {
			iter--;
	}

	currColor = iter->_cl;
	if (currColor != WHITE)
		return;

	iter--;
	while (iter->_cl == currColor) {
		temp = (char) iter->_ch;
		str.insert (0, temp);
		iter--;
	}

	currColor = iter->_cl;
	iter--;
	while (iter->_cl == currColor) {
		temp = (char) iter->_ch;
		__temp.insert (0, temp);

		if (iter == _chList.begin ())
			break;
		else
			iter--;
	}

	cmd = convertToCommand (__temp);
}

void CmdTextChange::setCurrWord (Color color) {
	unsigned short int i, numChar = 0;
	list<chNode>::iterator iter = getNode (_cursor - 1);
	while (iter->_ch != ' ') {
		iter->_cl = color;
		numChar++;

		if (iter == _chList.begin ())
			break;
		else
			iter--;
	}

	if (iter != _chList.begin ())
		iter++;

	for (i = 0; i < numChar; i++)
		_putch ('\b');

	for (i = 0; i < numChar; i++) {
		putChar (*iter);
		iter++;
	}
}

void CmdTextChange::setPrevWords (Color color) {
	unsigned short int i, numChar = 0;
	list<chNode>::iterator iter = getNode (_cursor - 1);
	Color currColor = iter->_cl;
	while (iter->_cl == currColor) {
			iter--;
			numChar++;
	}

	currColor = iter->_cl;
	iter--;
	while (iter->_cl == currColor) {
		iter->_cl = color;
		iter--;
		numChar++;
	}

	iter++;
	for (i = 0; i < numChar; i++)
		_putch ('\b');

	for (i = 0; i < numChar; i++) {
		putChar (*iter);
		iter++;
	}
}

CmdTextChange::Color CmdTextChange::prevWord () {
	list<chNode>::iterator iter = getNode (_cursor - 1);
	Color currColor = iter->_cl;
	while (iter->_cl == currColor) {
		if (iter == _chList.begin ())
			break;
		else
			iter--;
	}

	if (iter == _chList.begin ())
		return BLACK;
	else
		return iter->_cl;
}

void CmdTextChange::putChar (chNode node) {
	SetConsoleTextAttribute(hConsole, node._cl);
	_putch (node._ch);
}

void CmdTextChange::spCmdCheck () {
	short int ch = _getch ();
	switch (ch) {
	case RIGHT:
		if (_cursor < _chList.size ()) {
			list<chNode>::iterator iter = getNode (_cursor);
			
			if (iter->_ch == ' ')
				checkWord ();

			putChar (*iter);
			_cursor++;
		}
		break;
	case LEFT:
		if (_cursor > 0) {
			_putch ('\b');
			_cursor--;
		}
		break;
	case DEL:
		delChar ();
		break;
	case INS:
		_insertActive = !_insertActive;
		break;
	default:
		break;
	}
}

void CmdTextChange::delChar () {
	if (_cursor == _chList.size ())
		return;

	list<chNode>::iterator iter = getNode (_cursor);
	iter = _chList.erase (iter);
	while (iter != _chList.end ()) {
		putChar (*iter);
		iter++;
	}
	_putch (' ');

	for (unsigned int i = _chList.size (); i >= _cursor; i--)
		_putch ('\b');
}

void CmdTextChange::bsChar () {
	if (_cursor == 0) {
	} else if (_cursor == _chList.size ()) {
		_putch ('\b');
		_putch (' ');
		_putch ('\b');
		_chList.pop_back ();
		_cursor--;
	} else {
		_putch ('\b');
		_cursor--;
		list<chNode>::iterator iter = getNode (_cursor);
		iter = _chList.erase (iter);
		while (iter != _chList.end ()) {
			putChar (*iter);
			iter++;
		}

		chNode node = {' ', WHITE};
		putChar (node);
	
		for (unsigned int i = _chList.size (); i >= _cursor; i--)
			_putch ('\b');
	}
}

void CmdTextChange::insChar (short int ch) {
	chNode node = {ch, WHITE};

	if (_cursor == _chList.size ()) {
		putChar (node);
		_chList.push_back (node);
		_cursor++;
		return;
	}

	list<chNode>::iterator iter = getNode (_cursor);
	if (_insertActive) {
		iter = _chList.insert (iter, node);
		while (iter != _chList.end ()) {
			putChar (*iter);
			iter++;
		}
		_cursor++;
		for (unsigned int i = _chList.size (); i > _cursor; i--)
			_putch ('\b');
	} else {
		*iter = node;
		putChar (node);
	}
}

void CmdTextChange::splitString (string& str) {
	SetConsoleTextAttribute(hConsole, WHITE);
	clear ();
	unsigned int i, size = str.size ();
	chNode node;
	for (i = 0; i < size; i++) {
		if (str[i] >= '!' && str[i] <= '~') {
			node._ch = str[i];
		} else {
			node._ch = ' ';
		}

		if (node._ch == ' ') {
			if (_chList.empty () || _chList.back ()._ch == ' ') {
				continue;
			} else {
				checkWord ();
			}
		}

		node._cl = WHITE;
		_chList.push_back (node);
		_putch (node._ch);
		_cursor++;
	}
}

string CmdTextChange::mergeChar (list<chNode>* __chList) {
	string str;
	unsigned int i, size = __chList->size ();

	if (size == 0)
		return str;

	list<chNode>::iterator iter = __chList->begin ();
	for (i = 0; i < size - 1; i++) {
		str += (char) iter->_ch;
		iter++;
	}

	if (iter->_ch != ' ') {
		str += (char) iter->_ch;
	}

	return str;
}

list<CmdTextChange::chNode>::iterator CmdTextChange::getNode (unsigned int pos) {
	list<chNode>::iterator iter = _chList.begin ();
	for (unsigned int i = 0; i < pos; i++, iter++);
	return iter;
}

void CmdTextChange::clear () {
	_cursor = 0;
	_insertActive = true;
	_chList.clear ();
}

void CmdTextChange::buffer () {
	unsigned long int currTick, prevTick = GetTickCount ();
	do {
		currTick = GetTickCount ();
	} while (currTick - prevTick < 150);
}

CmdTextChange::Color CmdTextChange::getCmdColor (command cmd) {
	Color color;
	switch (cmd) {
	case CCOSTOM:
	case CFORCE:
	case CEXACT:
	case CSIMILAR:
	case CEACH:
	case CCOMMAND:
		color = 
	}
	return 0;
}