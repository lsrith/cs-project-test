/*
* author Chew
*/
#ifndef DATASTORAGE_H_
#define DATASTORAGE_H_
#include "TimePeriod.h"
#include "Time.h"
#include "Task.h"
#include <list>
using namespace std;

class DataStorage {

private: 
	string _storageFile;
	int _largestIndex;
	
	int reIndexingTemplates ();
	// when to do it is based on your decision
	// return the largest index
	list<Task> sort_by_date(list<Task> toDoList);
	void swap_element(list<Task> *ptr, list<Task> *ptr1);
	void write_to_file(list<Task> todolist);

public:
	DataStorage ();
	void save (list<Task> toDoList);
	// save the templates to the storageFile by overwriting any overlapping index
	// give an idex to each templates if it's not given
	list<Task> load (TimePeriod period);
	// return a list of templates within a time period input :) get it from our storageFile
	void erase (list<int> templateIndex);
	// erase any template with the specified index from the file

	void save (string tableName, TimePeriod period, list<int> templateIndex);
	// save the timetable with the name and period so that you can load back all tasks
	list<Task> load (string tableName);
	// return a list of templates of the timetable
	list<string> load_table_name ();
	// return a list of names of existing timetable

//	string search (string searchedWord, searchType type);
	// search based on any specified word.. return a string to be displayed
	// type to indicate exact match or similar match
//	string costomSearch (/*arguments to be modify*/);
	// search based on some criteria


};
#endif