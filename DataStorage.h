#ifndef DATASTORAGE_H_
#define DATASTORAGE_H_
#include "TimePeriod.h"
#include "Time.h"
#include "Task.h"
#include <list>
#include <vector>
using namespace std;

template <typename data_t>
data_t* find_pos (list<data_t>*, int);
enum search_t {SEXACT, SSIMILAR, SEACH};

class DataStorage {
private:
	struct TaskNode {
		bool _active;
		string _table;
		Task _task;
	};

	struct TableNode {
		bool _active;
		string _name;
		TimePeriod _period;
		list<TaskNode*> _tasks;
	};

public:	
	DataStorage ();

	void updateStorageName (string storageName);
	
	static void sort (list<Task>* taskList);

	void save (list<Task> taskList);
	void save (list<Task>* taskList);
	// save the templates to the storageFile by overwriting any overlapping index
	// give an idex to each templates if it's not given
	void save (list<int> taskIdex);

	list<Task> load (TimePeriod period);
	// return a list of templates within a time period input :) get it from our storageFile
	// this load function takes care of the dates only.. no clock involved!!

	void erase (string tableName);
	void erase (list<int> taskIndex);
	// erase any template with the specified index from the file
	void clear ();
	void exit ();

	void save (string tableName, TimePeriod period, list<Task> tasks);
	// save the timetable with the name and period so that you can load back all tasks
	// if the tableName is the existing one, check the period
	// if the period is the same, add more index inside; else, overwrite the old one
	list<Task> load (string tableName);
	// return a list of tasks of the timetable
	vector<string> load_table_name ();
	// return a list of names of existing timetable

	list<Task> search (string searchedWord, search_t type);
	// search based on any specified word.. return a string to be displayed
	// type to indicate exact match or similar match

	list<Task> get_alertTasks ();

private:
	string _taskFile;
	string _tableFile;
	int _largestIndex;
	list<TaskNode*> _indxTasks;
	list<TableNode*> _tables;
	
	void save (TableNode* table, list<Task>* taskList);
	void setInactive (list<TaskNode*>* tasks);

	inline void removeInactiveTasks (list<TaskNode*>*);
	inline void reIndexing ();
	inline void loadFromFile ();
	inline void writeToFile ();
	inline void appendToFile (list<TaskNode*>*);
	inline void appendToFile (TableNode*);

	list<Task> exactSearch (string);
	list<Task> similarSearch (string);
	list<Task> eachSearch (string);
};
#endif
