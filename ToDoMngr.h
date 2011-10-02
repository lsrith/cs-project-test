#ifndef TODOMNGR_H
#define TODOMNGR_H
#include "Task.h"
#include "DataStorage.h"
#include <list>
#include <queue>
#include <stack>
#include <string>
using namespace std;

class ToDoMngr {
public:
	enum search_t {SEXACT, SSIMILAR, SEACH};
	enum view_t {DAILY, WEEKLY, MONTHLY};

	ToDoMngr ();
	list<Task> get_active_list ();

//Ris:
	string view (int taskId);
	// get the task from the _activeTaskList
	// return a null string if taskId is invalidD
	static string view (Task& taskId);
	// return a string of the view of the specific task
	static string view (list<Task>* taskList);
	// return a string of the view of all these task
	string view (view_t viewType, Time time);
	// return a string of the view by day, by week, or by month
	// have to consider how to display the clashed tasks
	// for each task shown, numering them from 1 to the end and save as a vector of address of template
	// if user enter view 5, we execute view (templateVect[4]); 
	string view (TimePeriod period);
	string view (view_t viewType, string tableName);
	string viewTableNames ();
	// view all the names of existing timetable
	string reminder ();
	// return a string of the view of the reminder whenever ToDoMngr is executed;
	string alert ();
	// return a string to pop up whenever a task is set to be alert at any specified time
	
	string help (string command);
	// view help based on specific command, view full help if command == NULL;
	// for V0.1, suppose that the command is only NULL or command;
	//***Switch User*** function prototype to be completed

	static list<string> tableNames ();
	// return all the names of the existing timetable

	void exit ();
	void clear ();
	//delete everything in the dataStorage
//Ben:
	list<Task> add (Task task, bool forceAdd);
	// add a task to the dataStorage
	// if forceAdd is true, just add and return a fresh list
	// else return the clashed tasks

	list<Task> edit (int taskId /*--*/);
	Task erase (int taskId);
	// return an erased task
	// if taskId is incorrect, return a default Task
	void erase (TimePeriod period);
	// do not erase any Tasks in any timetable

	bool newTable (string name, TimePeriod period);
	// create a new timetable, save it with this name
	// return false if the name is already exist
	list<Task> add (string tableName, Task task, bool forceAdd);
	// if tableName is NULL, simply add the task
	// add tasks periodically to the table
	// return a list of clashed tasks if any
	void erase (string name);
	// erase the timetable with the name specified

	void undo ();
	void redo ();

private:
	static string _storageFile;
	static DataStorage _dataStorage;
	list<Task> _activeTaskList;
};
#endif