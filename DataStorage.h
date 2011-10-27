#ifndef DATASTORAGE_H_
#define DATASTORAGE_H_
#include "TimePeriod.h"
#include "Time.h"
#include "Task.h"
#include <list>
#include <vector>
using namespace std;

template <typename data_t>
bool isFound (list<data_t> dataList, data_t data);

class DataStorage {
public:
	enum search_t {SEXACT, SSIMILAR, SEACH};
	
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

	void erase (list<int> taskIndex);
	// erase any template with the specified index from the file
	void clear ();
	void exit ();

	void save (string tableName, TimePeriod period, list<int> templateIndex);
	// save the timetable with the name and period so that you can load back all tasks
	// if the tableName is the existing one, check the period
	// if the period is the same, add more index inside; else, overwrite the old one
	list<Task> load (string tableName);
	// return a list of templates of the timetable
	vector<string> load_table_name ();
	// return a list of names of existing timetable

	list<Task> search (string searchedWord, search_t type);
	// search based on any specified word.. return a string to be displayed
	// type to indicate exact match or similar match
//	string costomSearch (/*arguments to be modify*/);
	// search based on some criteria

private: 
	string _taskFile;
	string _tableIdxFile;
	string _taskIdxFile;

	int _largestIndex;
	vector< list<int> > arrangedTask;
	vector<Time::date_t> dates;
	vector<Task> tasks;
	vector<bool> activeTasks;
	vector<string> tableNames;
	vector< list<int> > tableTasks;
	vector<TimePeriod> tablePeriod;
	
	void updateDates (list<Time::date_t>);
	int getDateIndex (Time::date_t);
	int getTableIndex (string);
	int getSimDateIndex (Time::date_t);
	void reIndexing ();

	inline void loadFromFile ();
	inline void writeTaskToFile ();
	inline void writeTaskIdxToFile ();
	inline void writeTableIdxToFile ();

	list<Task> exactSearch (string);
	list<Task> similarSearch (string);
	list<Task> eachSearch (string);
	void checkTaskList (list<Task>*);
};
#endif
