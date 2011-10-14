/*
* author Chew
*/
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

private: 
	string _storageFile;
	int _largestIndex;
	vector<list<int>> arrangedTask;
	vector<Time::date_t> dates;
	vector<Task> tasks;
	vector<string> tableNames;
	vector<list<int>> tableTasks;
	vector<TimePeriod> tablePeriod;
	
	void updateDates (list<Time::date_t>);
	int getDateIndex (Time::date_t);
	int getTableIndex (string);
	int getSimDateIndex (Time::date_t);
	void writeToFile(string storageFile);

/*	int reIndexingTemplates ();
	 when to do it is based on your decision
	 return the largest index
	list<Task> sort_by_date(list<Task> toDoList);
	void swap_element(list<Task> *ptr, list<Task> *ptr1);
	void write_to_file(list<Task> todolist);*/

public:
	DataStorage ();

	static void sort (list<Task>* taskList);

	void save (list<Task> taskList);
	void save (list<Task>* taskList);
	// save the templates to the storageFile by overwriting any overlapping index
	// give an idex to each templates if it's not given

	list<Task> load (TimePeriod period);
	// return a list of templates within a time period input :) get it from our storageFile
	// this load function takes care of the dates only.. no clock involved!!

	void erase (list<int> templateIndex);
	// erase any template with the specified index from the file
	void clear ();

	void save (string tableName, TimePeriod period, list<int> templateIndex);
	// save the timetable with the name and period so that you can load back all tasks
	// if the tableName is the existing one, check the period
	// if the period is the same, add more index inside; else, overwrite the old one
	list<Task> load (string tableName);
	// return a list of templates of the timetable
	vector<string> load_table_name ();
	// return a list of names of existing timetable

//	string search (string searchedWord, searchType type);
	// search based on any specified word.. return a string to be displayed
	// type to indicate exact match or similar match
//	string costomSearch (/*arguments to be modify*/);
	// search based on some criteria


};
#endif