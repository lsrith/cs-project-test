#include "DataStorage.h"
#include "Task.h"
#include "Time.h"
#include <vector>

using namespace std;

DataStorage::DataStorage ()
{
	_largestIndex = 1;
	Task task;
	tasks.push_back (task);
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

void DataStorage::save(list<Task> taskList)
{
	list<Task>::iterator iter;
	list<Time::date_t> dateList;	
	for(iter = taskList.begin(); iter != taskList.end(); iter++)
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
//for now, assume time and timePeriod wont be modified
	int dateIndex;
	for(iter = taskList.begin(); iter != taskList.end(); iter++)
	{	
		if (iter->_index == 0)
		{
			iter->_index = _largestIndex;
			_largestIndex++;
			tasks.push_back (*iter);

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
			tasks[iter->_index] = *iter;
		}
	}
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

	if (index == size)
		index = size - 1;

	return index;
}

list<Task> DataStorage::load (TimePeriod period)
{
	int startIndex = getSimDateIndex (period.get_start_time ().get_date ());
	int endIndex = getSimDateIndex (period.get_end_time ().get_date ());
	
	list<Task> taskList;
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

template <typename data_t>
bool isFound (list<data_t> dataList, data_t data) {
	list<data_t>::iterator iter;
	
	for (iter = dataList.begin (); iter != dataList.end (); iter++)
		if (*iter == data)
			return true;

	return false;
}