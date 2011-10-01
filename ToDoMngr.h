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
	enum search_t {EXACT, SIMILAR, INDIVIDUAL};
	enum view_t {DAILY, WEEKLY, MONTHLY};

	ToDoMngr ();
	list<Task> get_active_list ();

//Ris:
	string view (int taskId);
	// return a string of the view of the specific task
	string view (view_t viewType, Time time);
	// return a string of the view by day, by week, or by month
	// have to consider how to display the clashed tasks
	// for each task shown, numering them from 1 to the end and save as a vector of address of template
	// if user enter view 5, we execute view (templateVect[4]); 
	string view (view_t viewType, TimePeriod period);
	string view (view_t viewType, string tableName);
	string reminder ();
	// return a string of the view of the reminder whenever ToDoMngr is executed;
	string alert ();
	// return a string to pop up whenever a task is set to be alert at any specified time
	
	void help (string command);
	// view help based on specific command, view full help if command == NULL;
	// for V0.1, suppose that the command is always NULL.
	
	//***Switch User*** function prototype to be completed

//Ben:
	Task addTask (Task task, bool forceAdd);
	// add a task to the dataStorage
	// if forceAdd is true, just add and return a fresh Template
	// else return the clashed task

	list<Task> addTask (Task task, RepeatType repeat, bool forceAdd);
	// add a task to the dataStorage based on repeat type

	list<Task> editTask (int taskId /*--*/)
	void eraseTask (int taskId);
	// functions overloading are considerable

	bool newTable (string name, TimePeriod period);
	// create a new timetable, save it with this name
	// return false if the name is already exist
	list<Task> addInTable (Task task);
	// add tasks periodically to the table
	// return a list of clashed tasks if any
	list<Task> editInTable (int taskId /*--*/);
	void eraseInTable (int taskId);
	// function overloading are considerable
	// edit or erase all task periodically
	void eraseTable (string name);
	// erase the whole Table with the name specified
	list<TimePeriod> eraseTable (TimePeriod period);
	// return a list of periods of a similar period if cannot find the table with the time period
	// return a clear list if be able to erase

	void undo ();
	void redo ();

private:
	static string _storageFile;
	static DataStorage _dataStorage;
	list<Task> _activeTaskList;
};
#endif