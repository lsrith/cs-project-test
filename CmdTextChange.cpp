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
	return mergeChar (&_chList);
}

void CmdTextChange::checkWord () {
	string str;
	str = getCurrWord ();
	command cmd = convertToCommand (str);
	if (cmd == CNULL) {
		if (prevWord () == NONE)
			setCurrWord (RED);
	} else {
		if (prevWord () == CMD) {
			setPrevWord (RED);
			setCurrWord (RED);
		} else if (prevWord () == DATA) {
			str = getPrevWord ();
			if (ifDataError (str))
				setPrevWord (RED);
			setCurrWord (BLUE);
		} else {
			setCurrWord (BLUE);
		}
	}
}

string CmdTextChange::getCurrWord () {
	string str, temp;
	list<chNode>::iterator iter = getNode (_cursor - 1);

	while (iter->_ch != ' ') {
		temp = iter->_ch;
		str.insert (0, temp);

		if (iter == _chList.begin ())
			break;
		else
			iter--;
	}

	return str;
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

	_cursor = 0;
	unsigned int i, size = str.size ();
	chNode node;
	for (i = 0; i < size; i++) {
		if (str[i] >= '!' && str[i] <= '~') {
			node._ch = str[i];
		} else {
			node._ch = ' ';
		}

		if (str[i] == ' ') {
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