/*
* author Chew
*/
/*
#ifndef DATASTORAGE_H_
#define DATASTORAGE_H_
#include "TimePeriod.h"
#include "Time.h"
#include "Template.h"
#include <list>
using namespace std;

class DataStorage {
private:
	string _storageFile;
	int _largestIndex;
	
	int reIndexingTemplates ();
	// when to do it is based on your decision
	// return the largest index
public:
	DataStorage (string storageFile);
	void save (list<Template> toDoList);
	// save the templates to the storageFile by overwriting any overlapping TimePeriod
	// unless forceAdd == true for any particular template
	// give an idex to each templates if it's not given
	list<Template> load (TimePeriod period);
	// return a list of templates within a time period input :) get it from our storageFile
	void erase (list<int> templateIndex);
	// erase any template with the specified index from the file

	void save (string tableName, TimePeriod period, list<int> templateIndex);
	// save the timetable with the name and period so that you can load back all tasks
	list<Template> load (string tableName);
	// return a list of templates of the timetable
	list<string> load_table_name ();
	// return a list of names of existing timetable

	string search (string searchedWord, searchType type);
	// search based on any specified word.. return a string to be displayed
	// type to indicate exact match or similar match
	string costomSearch (/*arguments to be modify*/);
	// search based on some criteria
};
#endif
*/