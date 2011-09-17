#ifndef TODOMNGR_H
#define TODOMNGR_H
#include "TimePeriod.h"
#include "Template.h"
#include <list>
#include <queue>
#include <stack>
#include <string>
using namespace std;

enum searchType {EXACT, SIMILAR};
enum repeatType {};

class ToDoMngr {
private:
	string _storageFile;
	string _userInput;
	queue<string> _splitedCommand;
	stack<string> _undoHistory;
	stack<string> _redoHistory;
	string* _shortcut;
	string* _command;

	void splitCommand ();
	bool invalidCommand ();

	void getUserInput ();
	// getUserInput from CLI and stored in the userCommand;
	void accessCommand ();
	// access the command accordingly
	void undo ();
	void redo ();
	void CLdisplay ();

//Ris:
	string view (Template* task);
	// return a string of the view of the specific task
	string view (string viewType);
	// return a string of the view of the specific tasks, by day, by week or by table name
	// have to consider how to display the clashed tasks
	// for each task shown, numering them from 1 to the end and save as a vector of address of template
	// if user enter view 5, we execute view (templateVect[4]); 
	string reminder ();
	// return a string of the view of the reminder whenever ToDoMngr is executed;
	string alert ();
	// return a string to pop up whenever a task is set to be alert at any specified time
	
	void help (string command);
	// view help based on specific command, view full help if command == NULL;
	// for V0.1, suppose that the command is always NULL.
	
	//***Switch User*** function prototype to be completed

//Chew:
	void save (list<Template> toDoList);
	// save the templates to the storageFile by overwriting any overlapping TimePeriod
	list<Template> load (TimePeriod period);
	// return a list of templates within a time period input :) get it from our storageFile

	void save (string tableName, TimePeriod period, list<string> tasksName);
	// save the timetable with the name and period so that you can load back a tasks
	list<Template> load (string tableName);
	// return a list of templates of the timetable
	list<string> load_table_name ();
	// return a list of names of existing timetable

	void reIndexingTemplates ();
	// when to do it is based on your decision @chew
	string search (string searchedWord, searchType type);
	// search based on any specified word.. return a string to be displayed
	// type to indicate exact match or similar match
	string costomSearch (/*arguments to be modify*/);
	// search based on some criteria

//Ben:
	Template addTask (Template task, bool forceAdd);
	// add a task to the dataStorage
	// if forceAdd is true, just add and return a fresh Template
	// else return the clashed task

	list<Template> addTask (Template task, repeatType repeat, bool forceAdd);
	// add a task to the dataStorage based on repeat type

	void editTask (/*argument to be specified*/);
	Template editTask (/*argument to be specified*/);
	list<Template> editTask (/*argument to be specified*/);
	void eraseTask (/*argument to be specified*/);
	// functions overloading are considerable

	bool newTable (string name, TimePeriod period);
	// create a new timetable, save it with this name
	// return false if the name is already exist
	list<Template> addInTable (Template task);
	// add tasks periodically to the table
	// return a list of clashed tasks if any
	void editInTable (/*argument to be specified*/);
	void eraseInTable (/*argument to be specified*/);
	// function overloading are considerable
	// edit or erase all task periodically
	void eraseTable (string name);
	// erase the whole Table with the name specified
	list<TimePeriod> eraseTable (TimePeriod period);
	// return a list of periods of a similar period if cannot find the table with the time period
	// return a clear list if be able to erase

public:
	ToDoMngr ();
	void run ();
	//run the program
};
#endif