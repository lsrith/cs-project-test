#include "DataStorage.h"
#include "Task.h"
#include "Time.h"
#include <time.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;

DataStorage::DataStorage ()
{
	_largestIndex = 1;
	Task task;
	tasks.push_back (task);
	activeTasks.push_back (false);
	_taskFile = "dflt.txt";
	_tableIdxFile = "dflttt.txt";
	_taskIdxFile = "dflttk.txt";
	loadFromFile ();
}

void DataStorage::updateStorageName (string storageName) {
	arrangedTask.clear ();
	dates.clear ();
	tasks.clear ();
	activeTasks.clear ();
	_largestIndex = 1;
	Task task;
	tasks.push_back (task);
	activeTasks.push_back (false);

	_taskFile = storageName + ".txt";
	_tableIdxFile = storageName + "tt.txt";
	_taskIdxFile = storageName + "tk.txt";
	loadFromFile ();
}

void DataStorage::save (string tableName, TimePeriod period, list<int> taskIndex)
{
	int index = getTableIndex (tableName);
	list<int>::iterator iter;

	if (index == -1) {
		tableNames.push_back (tableName);
		tablePeriod.push_back (period);
		save (tableName, period, taskIndex);
	} else {
		if (tablePeriod[index] == period) {
			for (iter = taskIndex.begin (); iter != taskIndex.end (); iter++) {
				if (!isFound (tableTasks[index], *iter)) {
					tableTasks[index].push_back (*iter);
				}				
			}
		} else {
			tablePeriod[index] = period;
			tableTasks[index].clear ();
			save (tableName, period, taskIndex);
		}
	}

	writeTableIdxToFile ();
}

int DataStorage::getTableIndex (string tableName)
{
	int index, size = tableNames.size ();
	for (index = 0; index < size; index++)
		if (tableNames[index] == tableName)
			break;

	if (index == size)
		index = -1;
	return index;
}

list<Task> DataStorage::load (string tableName)
{
	list<Task> taskList;
	int index = getTableIndex (tableName);
	if (index != -1) {
		list<int> taskIndex;
		list<int>::iterator iter;
		for (iter = tableTasks[index].begin (); iter != tableTasks[index].end (); iter++) {
			if (!isFound (taskIndex, *iter)) {
				taskList.push_back (tasks[*iter]);
				taskIndex.push_back (*iter);
			}
		}
	}

	sort (&taskList);
	return taskList;
}

vector<string> DataStorage::load_table_name ()
{
	return tableNames;
}
	
void DataStorage::erase (list<int> taskIndex) {
	if (taskIndex.empty ()) return;
	
	list<int>::iterator iter;
	for (iter = taskIndex.begin (); iter != taskIndex.end (); iter++) {
		activeTasks[*iter] = false;
		for (int i = 0; i < arrangedTask.size (); i++)
			arrangedTask[i].remove (*iter);

		for (int i = 0; i < tableTasks.size (); i++)
			tableTasks[i].remove (*iter);
	}

	writeTableIdxToFile ();
	writeTaskIdxToFile ();
}

void DataStorage::clear () {
	arrangedTask.clear ();
	dates.clear ();
	tasks.clear ();
	_largestIndex = 1;
	Task task;
	tasks.push_back (task);

	ofstream taskFile (_taskFile);
	taskFile.close ();
	ofstream taskIdxFile (_taskIdxFile);
	taskFile.close ();
	ofstream tableIdxFile (_taskIdxFile);
	taskFile.close ();
}

void DataStorage::sort (list<Task>* taskList) {
	if (taskList->empty ()) return;

	list<Task>::iterator currIter, prevIter;
	list<Task>::iterator endIter = taskList->end ();
	Time prev, curr;
	Task temp;
	int i, size = taskList->size ();

	for (i = 0; i < size; i++) {
		bool is_sorted = true;
		for (currIter = ++(taskList->begin ()); currIter != endIter; currIter++) {
			prevIter = --currIter;
			currIter++;

			if (prevIter->timeTask)
				prev = prevIter->get_time ();
			else
				prev = prevIter->get_period ().get_start_time ();

			if (currIter->timeTask)
				curr = currIter->get_time ();
			else
				curr = currIter->get_period ().get_start_time ();

			if (prev > curr) {
				temp = *prevIter;
				*prevIter = *currIter;
				*currIter = temp;

				is_sorted = false;
			}
		}
		endIter--;
		if (is_sorted) return;
	}
}

void DataStorage::save (list<Task>* taskList)
{
	if (taskList->empty ()) return;
	 
	Task* taskPtr;
	list<Task> timePeriodModifiedTasks;
	list<int> timePeriodModifiedIndex;
	list<Task>::iterator iter;
	list<Time::date_t> dateList;	
	for (iter = taskList->begin(); iter != taskList->end(); iter++)
	{
		if (iter->timeTask == true) {
			dateList.push_back (iter->get_time ().get_date ());
		} else {
			Time time;
			TimePeriod period = iter->get_period ();
			for (time = period.get_start_time (); time.get_date () != period.get_end_time ().get_date (); time = time + Time::DAY) {
				dateList.push_back (time.get_date ());
			}
			dateList.push_back (time.get_date ());
		}
	}
	updateDates (dateList);

//timePeriod related change
//for now, time and timePeriod can be modified
	int dateIndex;
	for (iter = taskList->begin(); iter != taskList->end(); iter++)
	{	
		if (iter->_index == 0)
		{
			iter->_index = _largestIndex;
			_largestIndex++;
			tasks.push_back (*iter);
			activeTasks.push_back (true);

			if (iter->timeTask == true) {
				dateIndex = getDateIndex (iter->get_time ().get_date ());
				arrangedTask[dateIndex].push_back (iter->_index);
			} else {
				TimePeriod period = iter->get_period ();
				int startIndex = getDateIndex (iter->get_period ().get_start_time ().get_date ());
				int endIndex = getDateIndex (iter->get_period ().get_end_time ().get_date ());
				for (dateIndex = startIndex; dateIndex <= endIndex; dateIndex++) {
					arrangedTask[dateIndex].push_back (iter->_index);
				}
			}
		}
		else
		{
			//check if time or period is modified
			taskPtr = &tasks[iter->_index];
			if (taskPtr->get_time () == iter->get_time () && taskPtr->get_period ().get_start_time () == iter->get_period ().get_start_time ()
				&& taskPtr->get_period ().get_end_time () == iter->get_period ().get_end_time ()) {
				*taskPtr = *iter;
			} else {
				iter->_index = 0;
				timePeriodModifiedTasks.push_back (*iter);
				timePeriodModifiedIndex.push_back (taskPtr->_index);
			}
  		}
	}
	
	//append new tasks to the _taskFile
	ofstream taskFile (_taskFile, ofstream::app);
	for (iter = taskList->begin(); iter != taskList->end(); iter++) {
		if (iter->_index != 0)
			taskFile << iter->stringConvert () << endl;
	}
	taskFile.close ();

	writeTaskIdxToFile ();

	erase (timePeriodModifiedIndex);
	save (&timePeriodModifiedTasks);
}

void DataStorage::save(list<Task> taskList)
{
	save (&taskList);
}

void DataStorage::updateDates (list<Time::date_t> dateList) 
{
	int dateIndex;
	list<Time::date_t>::iterator iter;
	vector<Time::date_t>::iterator dateIter;
	vector<list<int>>::iterator taskDateIter;
	list<int> temp;

	for (iter = dateList.begin (); iter != dateList.end (); iter++)
	{
		dateIndex = getDateIndex (*iter);
		if (dateIndex == -1)
		{
			for (dateIter = dates.begin (), taskDateIter = arrangedTask.begin (); dateIter != dates.end (); dateIter++, taskDateIter++)
			{
				if (Time::isAfter (*iter, *dateIter))
					break;
			}

			dates.insert (dateIter, *iter);
			arrangedTask.insert (taskDateIter, temp);
		}
	}
}

int DataStorage::getDateIndex (Time::date_t date)
{
	int index, size = dates.size ();
	for (index = 0; index < size; index++) {
		if (dates[index] == date)
			break;
	}

	if (index == size)
		index = -1;
	
	return index;
}

int DataStorage::getSimDateIndex (Time::date_t date) 
{
	int index, size = dates.size ();
	for (index = 0; index < size; index++) {
		if (Time::isAfter (date, dates[index]))
			break;
		else if (date == dates[index])
			break;
		else;
	}

	return index;
}

list<Task> DataStorage::load (TimePeriod period)
{
	list<Task> taskList;
	int startIndex, endIndex;
	if ((period.get_start_time ().get_date () == Time::DFLT_DATE && period.get_end_time ().get_date () == Time::DFLT_DATE) ||
		(period.get_start_time ().get_date () == Time::INF_DATE && period.get_end_time ().get_date () == Time::INF_DATE)) {
		startIndex = 0;
		endIndex = dates.size () - 1;
	} else {
		startIndex = getSimDateIndex (period.get_start_time ().get_date ());
		if (Time::isAfter (period.get_end_time ().get_date (), dates[startIndex]))
			return taskList;

		endIndex = getSimDateIndex (period.get_end_time ().get_date ());
		if (startIndex == dates.size ())
			return taskList;
		else if (endIndex == dates.size ())
			endIndex--;
		else;
	}
	
	list<int> taskIndex;
	list<int>::iterator iter;
	for (int i = startIndex; i <= endIndex; i++) 
	{
		for (iter = arrangedTask[i].begin (); iter != arrangedTask[i].end (); iter++)
		{
			if (!isFound (taskIndex, *iter)) {
				taskList.push_back (tasks[*iter]);
				taskIndex.push_back (*iter);
			}
		}
	}

	sort (&taskList);
	return taskList;
}

void DataStorage::exit () {
	reIndexing ();
	writeTaskToFile ();
	writeTaskIdxToFile ();
	writeTableIdxToFile ();
}

template <typename data_t>
bool isFound (list<data_t> dataList, data_t data) {
	list<data_t>::iterator iter;
	
	for (iter = dataList.begin (); iter != dataList.end (); iter++)
		if (*iter == data)
			return true;

	return false;
}

void DataStorage::reIndexing () {
	//merge all active task index
	list<int> taskIdex;
	int size = arrangedTask.size ();
	for (int i = 0; i < size; i++)
		taskIdex.splice (taskIdex.end (), arrangedTask[i]);
	taskIdex.sort ();

	//remove duplicated index
	int num;
	list<int>::iterator iter;
	for (iter = taskIdex.begin (); iter != taskIdex.end (); iter++) {
		num = *iter;
		taskIdex.remove (num);
	}
	
	//erase inactive task index from the task and table storage
	list<int> erasedTaskIdx;
	list<Task> newTaskList;
	int newIdex = 1;
	for (iter = taskIdex.begin (); iter != taskIdex.end (); iter++) {
		if (*iter > newIdex) {
			tasks[*iter]._index = newIdex;
			erasedTaskIdx.push_back (*iter);
			newTaskList.push_back (tasks[*iter]);
			newIdex++;
		} else;
	}
	erase (erasedTaskIdx);

	//erase inactive task from the task vector
	for (int i = 0; i < _largestIndex; i++)
		tasks[tasks[i]._index] = tasks[i];
	for (; _largestIndex > newIdex; _largestIndex--)
		tasks.pop_back ();
	save (newTaskList);
}

void DataStorage::loadFromFile () {
	string str;

	ifstream taskFile (_taskFile);
	if (taskFile.is_open ()) {
		while (getline (taskFile, str)) {
			Task task (str);
			if (task._index == _largestIndex) {
				tasks.push_back (task);
				activeTasks.push_back (true);
				_largestIndex++;
			} else if (task._index < _largestIndex) {
				tasks[task._index] = task;
			} else;		//this case occurs only when there is logic error
		}
		taskFile.close ();
	} else {
		ofstream newFile (_taskFile);
		newFile.close ();
	}

	int index, i = 0;
	Time::date_t date;
	list<int> intList;
	ifstream taskIdxFile (_taskIdxFile);
	if (taskIdxFile.is_open ()) {
		while (getline (taskIdxFile, str)) {
			stringstream strStream;
			strStream << str;
			strStream >> date;
			dates.push_back (date);
			arrangedTask.push_back (intList);
			while (strStream >> index)
				arrangedTask[i].push_back (index);
			i++;
		}
		taskIdxFile.close ();
	} else {
		ofstream newFile (_taskIdxFile);
		newFile.close ();
	}
	
	Time time;
	TimePeriod period;
	i = 0;
	ifstream tableIdxFile (_tableIdxFile);
	if (tableIdxFile.is_open ()) {
		while (getline (tableIdxFile, str)) {
			stringstream strStream;
			strStream << str;
			strStream >> str;
			tableNames.push_back (str);
			strStream >> date;
			time.modify_date (date);
			time.modify_clock (0);
			period.modify_start_time (time);
			strStream >> date;
			time.modify_date (date);
			time.modify_clock (2359);
			period.modify_end_time (time);
			tablePeriod.push_back (period);
			tableTasks.push_back (intList);
			while (strStream >> index)
				tableTasks[i].push_back (index);
			i++;
		}
		tableIdxFile.close ();
	} else {
		ofstream newFile (_tableIdxFile);
		newFile.close ();
	}
}

void DataStorage::writeTaskToFile () {
	ofstream taskFile (_taskFile);
	vector<Task>::iterator iter;
	for (iter = tasks.begin(); iter != tasks.end(); iter++) {
		if (iter->_index != 0)
			taskFile << iter->stringConvert () << endl;
	}
	taskFile.close ();
}

void DataStorage::writeTaskIdxToFile () {
	list<int>::iterator intIter;
	int size = dates.size ();
	ofstream taskIdxFile (_taskIdxFile);

	for (int i = 0; i < size; i++) {
		taskIdxFile << dates[i] << " ";
		for (intIter = arrangedTask[i].begin (); intIter != arrangedTask[i].end (); intIter++)
			taskIdxFile << *intIter << " ";
		taskIdxFile << endl;
	}

	taskIdxFile.close ();
}

void DataStorage::writeTableIdxToFile () {
	list<int>::iterator intIter;
	int size = tableNames.size ();
	ofstream tableIdxFile (_tableIdxFile);

	for (int i = 0; i < size; i++) {
		tableIdxFile << tableNames[i] << " ";
		tableIdxFile << tablePeriod[i].get_start_time ().get_date () << " " << tablePeriod[i].get_end_time ().get_date () << " ";
		for (intIter = arrangedTask[i].begin (); intIter != arrangedTask[i].end (); intIter++)
			tableIdxFile << *intIter << " ";
		tableIdxFile << endl;
	}

	tableIdxFile.close ();
}

list<Task> DataStorage::search (string searchedWord, search_t type) {
	list<Task> taskList;

	switch (type) {
	case SEACH:
		taskList = eachSearch (searchedWord);
		break;
	case SEXACT:
		taskList = exactSearch (searchedWord);
		break;
	case SSIMILAR:
		taskList = similarSearch (searchedWord);
		break;
	default:
		break;
	}

	checkTaskList (&taskList);
	return taskList;
}

list<Task> DataStorage::exactSearch (string searchedWord) {
	list<Task> taskList;

	string str;
	ifstream taskFile (_taskFile);
	int pos;
	if (taskFile.is_open ()) {
		while (getline (taskFile, str)) {
			pos = str.find (searchedWord);
			if (pos != string::npos) {
				Task task (str);
				taskList.push_back (task);
			}
		}

		taskFile.close ();
	}

	return taskList;
}

list<Task> DataStorage::similarSearch (string searchedWord) {
	list<Task> taskList;
	return taskList;
}

list<Task> DataStorage::eachSearch (string searchedWord) {
	list<Task> taskList;
	list<string> keyWords;
	string str;
	int end_pos;

	while (!searchedWord.empty ()) {
		end_pos = searchedWord.find_first_of (" ", 0);
		str = searchedWord.substr (0, end_pos - 1);
		keyWords.push_back (str);
		searchedWord.erase (0, end_pos);
cout << str << endl;
	}

	while (!keyWords.empty ()) {
		taskList.splice (taskList.end (), exactSearch (keyWords.front ()));
		keyWords.pop_front ();
	}

	return taskList;
}

void DataStorage::checkTaskList (list<Task>* taskList) {
	list<Task>::iterator iter;
	sort (taskList);
	for (iter = taskList->begin (); iter != taskList->end (); iter++) {
		if (!activeTasks[iter->_index])
			taskList->erase (iter);
	}
}