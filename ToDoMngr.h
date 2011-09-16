#ifndef TODOMNGR_H
#define TODOMNGR_H
#include "TimePeriod.h"
#include "TaskMngr.h"
#include "TableMngr.h"
#include "Template.h"
#include <list>
#include <queue>
#include <string>
using namespace std;

class ToDoMngr {
private:
	string _storageFile;
	string _userInput;
	queue<string> _splitedCommand;
	string* _shortcut;
	string* _command;

	void splitCommand ();
	bool invalidCommand ();

public:
	ToDoMngr ();
	void getUserInput ();
	void accessCommand ();
};

#endif