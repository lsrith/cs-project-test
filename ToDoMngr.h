/*
*Author Ris and Ben
*/

#ifndef TODOMNGR_H
#define TODOMNGR_H
#include "Task.h"
#include "DataStorage.h"
#include <list>
#include <queue>
#include <stack>
#include <string>
using namespace std;
        
enum view_t {DAILY, WEEKLY, MONTHLY};
enum cmdType {_addTask, _eraseTask, _erasePeriod, _addTable1, _addTable2, eraseTable, editTask };

class ToDoMngr {
public:
	static string NOTHING_TO_VIEW;

	struct TaskElement {
		bool _time;
		bool _period;
		bool _note;
		bool _venue;
		bool _alert;
		bool _repeat;
	};

	struct UserTask { 
		list<Task> _currentTaskList; 
		Task *_taskptr;
		cmdType _cmd;
		int _taskId;
		list<int> _indexList;
		TimePeriod _period;
		string _tableName;
		bool _forceAdd;
	};

	ToDoMngr ();
	ToDoMngr (string);
	list<Task> get_active_list ();

	void updateStorageName (string);
	void exit ();
	void clear ();
	// delete everything in the dataStorage
	string search (search_t, string);
	bool activateTable (string tableN);
	void deactivateTable ();
	bool getTableActivationStatus ();

//Ris:
	static string view (Task taskId);
	// return a string of the view of the specific task
	static string view (list<Task> taskList);

	string view (int taskId);
	// get the task from the _activeTaskList
	// return a null string if taskId is invalidD
	// return a string of the view of all these task
	string view (view_t viewType, Time time);
	// return a string of the view by day, by week, or by month
	// have to consider how to display the clashed tasks
	// for each task shown, numering them from 1 to the end and save as a vector of address of template
	// if user enter view 5, we execute view (templateVect[4]); 
	string view (TimePeriod period);
	string view (string tableN);
	string viewTableNames ();
	// view all the names of existing timetable
	string reminder ();
	// return a string of the view of the reminder whenever ToDoMngr is executed;
	string alert ();
	// return a string to pop up whenever a task is set to be alert at any specified time
        
	//***Switch User*** function prototype to be completed

	static list<string> tableNames ();
	// return all the names of the existing timetable

	bool ifExistedTable (string tableNames);

//Ben:
	list<Task> add (Task task, bool forceAdd);  
	// add a task to the dataStorage
	// if forceAdd is true, just add and return a fresh list
	// else return the clashed tasks

	list<Task> edit (int taskId, Task task, bool forceEdit);
	void setTaskElem (TaskElement*, Task*);
	list<Task> edit (int taskId, TaskElement* taskElem, Task* task, bool forceEdit);
	// edit elements in the tasks, for any of the taskElem->element = true, edit task->element
	// if forceEdit is true, just edit and return a fresh list
	// else return the clashed tasks, this happens only when taskElem->_period = true;
	// the poiter argument is restricted to use, the task should contain the full edited task after editing

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
	DataStorage _dataStorage;
	list<Task> _activeTaskList;
	list<Task> _clashList;
	bool Table_Mode;
	string tableName;

	stack<UserTask> _redoStack;
	stack<UserTask> _undoStack;

	bool clashed(Task task);
	
	bool tillStart(int, list<Task> taskList, Task task, bool forceAdd, TimePeriod activePeriod);
	bool tillEnd(int i, list<Task> taskList, Task task, bool forceAdd, TimePeriod activePeriod);


};
#endif
