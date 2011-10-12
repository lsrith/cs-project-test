#include "DataStorage.h"
#include "Task.h"
#include "Time.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <algorithm>

using namespace std;

DataStorage::DataStorage ()
{
	_storageFile = "DataStorage";

	ifstream inputFile;
	inputFile.open (_storageFile);
/*
	stringstream strStream;
	string str;
	_largestIndex = 0;
	getline (inputFile, str);
	strStream << str;
	strStream >> _largestIndex;
*/
	_largestIndex = 0;
	inputFile >> _largestIndex;
}

void DataStorage::save(list<Task> toDoList)
{
	list<Task> templist;
	list<Task> todolist;
	Time::date_t date;
	list<Task>::iterator iter = toDoList.begin();
	string task;
	int ctr=0;
	unsigned int index = 1;

	while(iter != toDoList.end())
	{
		iter->_index = index;
		task = iter->stringConvert();
		todolist.push_back(task);
		iter++;
		index++;
	}

	templist = sort_by_date(toDoList);
	write_to_file(templist);
	
	
}	

bool isEmpty(std::ifstream& pFile)
{
	return pFile.peek() == std::ifstream::traits_type::eof();
}

		
//use bubble to sort the task by date and return a sorted list of task
list<Task> DataStorage::sort_by_date(list<Task> toDoList)
{
	Time tm;
	list<Task>::iterator iter;
	list<Task>::iterator ptr;
	list<Task> templist;
	Time::date_t date, date1;
	int i=-1;

	for(iter = toDoList.begin(); iter != toDoList.end(); iter++)
	{
		i++;
		for(ptr = toDoList.begin()+1+i; ptr != toDoList.end(); ptr++)
		{
			//check whether the task is one-day type or have certain timeperiod
			if(iter->get_time() == tm)
			{
				if(ptr->get_time() == tm)
				{
					date = iter->get_period().get_start_time().get_date();
					date1 = ptr->get_period().get_start_time().get_date();
					if(date > date1)
					{
						iter_swap(iter, ptr);
					}
				}
				else
				{
					date= iter->get_period().get_start_time().get_date();
					date1 = ptr->get_time().get_date();
					if(date > date1)
					{
						iter_swap(iter, ptr);
					}
					
				}	
			}
			else if(ptr->get_time() == tm)
			{
				date = iter->get_time().get_date();
				date1 = ptr->get_period().get_start_time().get_date();
				if(date > date1)
				{
					iter_swap(iter, ptr);
				}
			}
			else
			{
				date = iter->get_time().get_date();
				date1 = ptr->get_time().get_date();
				if(date > date1)
				{
					iter_swap(iter, ptr);
				}
			}
		}
	}
	return templist = toDoList;
}

//swapping algorithm
/*void DataStorage::swap_element(list<Task> *ptr, list<Task> *ptr1)
{
	list<Task> temp;
	temp = *ptr;
	*ptr = *ptr1;
	*ptr1 = temp;
}*/

//write the sorted list of task to a file, no idea how to carry on :( 
void DataStorage::write_to_file(list<Task> todolist)
{
	list<Task>::iterator iter = todolist.begin();
	list<Task>::iterator ptr = iter++;
	Time::date_t date, date1;
	ifstream readFile(_storageFile);
	ofstream writeToFile(_storageFile);
	string str;

	if(isEmpty(readFile))		//is this correct??
	{
	while(ptr != todolist.end())
	{
		date = iter->get_time().get_date();
		if(iter->timeTask == true)
		{
			if(ptr->timeTask == true)
			{
				if(ptr->get_time().get_date() == date)
					ptr++;
				else
				{
					writeToFile << date << endl;
					while(iter != ptr)
					{
						writeToFile << iter->stringConvert() << endl;
						iter++;
					}
				}
			}
		}
		else
		{
			if(ptr->get_period().get_start_time().get_date() == date)
				ptr++;
			else
			{
				writeToFile << date << endl;
				while(iter != ptr)
				{
					writeToFile << iter->stringConvert() << endl;
					iter++;
				}
			}
		}
	}
	}
	
}

list<Task> DataStorage::load (TimePeriod period)
{
	int largestIndex;
	string str;
	ifstream readFile(_storageFile);

	readFile >> largestIndex;
	while(getline(readFile, str))
	{
		if(period.get_start_time().get_date() == str)

	}
}
