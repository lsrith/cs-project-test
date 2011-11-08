/**
 * This class is the main logic layer of TaskCal
 * It add, edit, delete and view the task inside its calendar
 * 
 * @author Ly Samrith: view
 * @author Rishov Prabhah Barman: alert
 * @author Yeo Lock Wei Ben: add, edit, delete, undo and redo
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
enum cmdType {_addTask, _eraseTask, _erasePeriod, _addTable1, _addTable2, _eraseTable, _editTask};

class ToDoMngr {
public:
	static string NOTHING_TO_VIEW;
	static string WRONG_ID;
	static string MSG_REPEAT_ERR;

	struct TaskElement {
		bool _time;
		bool _period;
		bool _note;
		bool _venue;
		bool _alert;
		bool _repeat;
	};

	struct UserTask { 
		cmdType _cmd;	
		Task* _eTask;
		Task _task, _updatedTask;
		int _taskId;
		list<Task*> _deletePeriodList;
		list<int> _index;
		TimePeriod _period;
		string _tableName;
		bool _force;
		TaskElement* _taskElem;
	};

	ToDoMngr ();
	ToDoMngr (string);
	list<Task> get_active_list ();
	list<Task> get_alert_list ();

	void updateStorageName (string);
	void exit ();
	void clear ();
	// delete everything in the dataStorage
	string search (search_t, string);
	void removeSameIdx (list<Task>*);
	bool activateTable (string tableN);
	bool ifTableMode ();
	void deactivateTable ();

	static list<Task>* listTask (Task task);

//Ris:
	static list<string> wrapSentence (string, short int);
	static string intToString (int, int);
	static string view (Task task);
	static string view (Task task, int, int);
	// return a string of the view of the specific task
	static string view (list<Task> taskList);
	static string repeat (unsigned int);

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
    void deactivateAlert ();
	bool ifAlertActive ();
	void snoozeAlert (int mins);

	void  push_alertTask (Task);
//push inside the task if its alert is to be in the future, push it a way that u got it in order
    void pop_alertTask (Task);
//delete that Task from ur alertTaskList
    
	//***Switch User*** function prototype to be completed

	static list<string> tableNames ();
	// return all the names of the existing timetable

	bool ifExistedTable (string tableNames);

//Ben:
	list<Task> add (Task task, bool forceAdd);  
	// add a task to the dataStorage
	// if forceAdd is true, just add and return a fresh list
	// else return the clashed tasks

	list<Task> edit (int taskId, Task* task, bool forceEdit);
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
	list<Task> _erasetasklist;
	list<Task> _alertTaskList;
	bool _alertActive;
	bool Table_Mode;
	DataStorage::Table _table;

	stack<UserTask> _redoStack;
	stack<UserTask> _undoStack;

	bool clashed(Task task);
	bool tillStart(int, list<Task> taskList, Task task, bool forceAdd, TimePeriod activePeriod);
	bool tillEnd(int i, list<Task> taskList, Task task, bool forceAdd, TimePeriod activePeriod);

	static string NOTE;
	static string VENUE;
	static string TIME;
	static string SPACE;
	static string ALERT;
	static string REPEAT;
};
#endif