#include "CmdTextChange.h"
#include <conio.h>
#include <ctype.h>
#include <string>
#include <assert.h>
using namespace std;

unsigned int CmdTextChange::MAX_HEIGHT = 1000;
unsigned int CmdTextChange::MAX_WIDTH = 80;

CmdTextChange::CmdTextChange () {
	try {
		load_vldCmdList ();
	} catch (string message) {
		throw (message);
	}

	initPointers ();
	_insertActive = true;
	_cursor = 0;
	__cursor.X = 0;
	__cursor.Y = 0;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hConsole, __cursor);
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
		
		if (ch == '\r' || ch == '\n') {
			ch = '\n';
			putChar ('\n');
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

void CmdTextChange::write (string str) {
	SetConsoleTextAttribute(hConsole, WHITE);
	clear ();
	unsigned int i, size = str.size ();
	for (i = 0; i < size; i++) {
		if (str[i] >= '!' && str[i] <= '~') {
			putChar (str[i]);
		} else if (str[i] == '\r' || str[i] == '\n') {
			putChar ('\n');
		} else {
			putChar (' ');
		}
		_cursor++;
	}
}

void CmdTextChange::checkWord () {
	string str;
	str = getCurrWord ();
	command cmd = convertToCommand (str);
	setCurrWord (getCmdColor (cmd));
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
		assert (moveCursorBackward ());

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
		assert (moveCursorBackward ());

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
	if (node._ch == '\r' || node._ch == '\n') {
		if (__cursor.Y == MAX_HEIGHT - 1)
			rewrite ();

		_putch (node._ch);
		__cursor.X = 0;
		__cursor.Y++;
	} else {
		if (__cursor.X == MAX_WIDTH - 1 && __cursor.Y == MAX_HEIGHT - 1)
			rewrite ();

		_putch (node._ch);
		moveCursorForward ();
	}
}

void CmdTextChange::putChar (short int ch) {
	if (ch == '\r' || ch == '\n') {
		if (__cursor.Y == MAX_HEIGHT - 1)
			rewrite ();

		_putch (ch);
		__cursor.X = 0;
		__cursor.Y++;
	} else {
		if (__cursor.X == MAX_WIDTH - 1 && __cursor.Y == MAX_HEIGHT - 1)
			rewrite ();

		_putch (ch);
		moveCursorForward ();
	}
}

void CmdTextChange::rewrite () {
	system ("cls");
	if (_chList.size () < MAX_HEIGHT * MAX_WIDTH) {
			system ("cls");
			list<chNode>::iterator iter;
			for (iter = _chList.begin (); iter != _chList.end (); iter++)
				putChar (*iter);

			for (unsigned int i = _chList.size (); i > _cursor; i--)
				assert (moveCursorBackward ());
	}
}

void CmdTextChange::spCmdCheck () {
	short int ch = _getch ();
	switch (ch) {
	case RIGHT:
		if (_cursor < _chList.size ()) {
/*
			list<chNode>::iterator iter = getNode (_cursor);
			
			if (iter->_ch == ' ')
				checkWord ();

			putChar (*iter);
*/
			assert (moveCursorForward ());
			_cursor++;
		}
		break;
	case LEFT:
		if (_cursor > 0) {
			assert (moveCursorBackward ());
			_cursor--;
		}
		break;
	case UP:
		if (_cursor > MAX_WIDTH) {
			assert (moveCursorUp ());
			_cursor -= MAX_WIDTH;
		}
		break;
	case DOWN:
		if (_cursor + MAX_WIDTH <= _chList.size ()) {
			assert (moveCursorDown ());
			_cursor += MAX_WIDTH;
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
	putChar (' ');

	for (unsigned int i = _chList.size (); i >= _cursor; i--)
		assert (moveCursorBackward ());
}

void CmdTextChange::bsChar () {
	if (_cursor == 0) {
	} else if (_cursor == _chList.size ()) {
		assert (moveCursorBackward ());
		putChar (' ');
		assert (moveCursorBackward ());
		_chList.pop_back ();
		_cursor--;
	} else {
		assert (moveCursorBackward ());
		list<chNode>::iterator iter = getNode (_cursor);
		iter = _chList.erase (iter);
		while (iter != _chList.end ()) {
			putChar (*iter);
			iter++;
		}

		chNode node = {' ', WHITE};
		putChar (node);
	
		for (unsigned int i = _chList.size (); i >= _cursor; i--)
			assert (moveCursorBackward ());
		_cursor--;
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
			assert (moveCursorBackward ());
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
		putChar (node);
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
	} while (currTick - prevTick < 10);
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
	case CLEFT:
	case CRIGHT:
		color = GRAY;
		break;
	case CTIME:
	case CDATE:
	case CFROM:
	case CTO:
	case CNAME:
	case CVENUE:
	case CNOTE:
	case CALERT:
	case CREPEAT:
		color = GREEN;
		break;
	case CHOUR:
	case CFORTNIGHT:
	case CYEAR:
	case CDAY:
	case CWEEK:
	case CMONTH:
	case CNOW:
	case CTODAY:
	case CTMR:
		color = PINK;
		break;
	case CADD:
	case CEDIT:
	case CDELETE:
	case CTABLE:
	case CVIEW:
	case CREMINDER:
	case CNEXT:
	case CPREVIOUS:
	case CFIRST:
	case CLAST:
	case CUNDO:
	case CREDO:
	case CHELP:
	case CSORT:
	case CSEARCH:
	case CCLEAR:
	case CUSER:
	case CEXIT:
	case CDISCARD:
	case CREPLACE:
	case CINSERT:
	case CVOID:
		color = SKY;
		break;
	default:
		color = WHITE;
		break;
	}
	return color;
}

bool CmdTextChange::moveCursorForward () {
	if (__cursor.X == MAX_WIDTH - 1 && __cursor.Y == MAX_HEIGHT) {
		return false;
	}
	
	if (__cursor.X < MAX_WIDTH - 1) {
		__cursor.X++;
	} else {
		__cursor.X = 0;
		__cursor.Y++;
	}

	SetConsoleCursorPosition(hConsole, __cursor);
	return true;
}

bool CmdTextChange::moveCursorBackward () {
	if (__cursor.X == 0 && __cursor.Y == 0)
		return false;
	
	if (__cursor.X > 0) {
		__cursor.X--;
	} else {
		__cursor.X = MAX_WIDTH - 1;
		__cursor.Y--;
	}

	SetConsoleCursorPosition(hConsole, __cursor);
	return true;
}

bool CmdTextChange::moveCursorDown () {
	if (__cursor.Y == MAX_WIDTH - 1)
		return false;

	__cursor.Y++;
	SetConsoleCursorPosition(hConsole, __cursor);
	return true;
}

bool CmdTextChange::moveCursorUp () {
	if (__cursor.Y == 0)
		return false;
	
	__cursor.Y--;
	SetConsoleCursorPosition(hConsole, __cursor);
	return true;
}