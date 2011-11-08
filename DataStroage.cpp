#include "DataStorage.h"
#include "Task.h"
#include "Time.h"
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;

DataStorage::DataStorage ()
{
}

DataStorage::~DataStorage () {
	exit ();
}

void DataStorage::updateStorageName (string storageName) {
	if (!_taskFile.empty ()) {
		exit ();
		_indxTasks.clear ();
		_tables.clear ();
	}

	_largestIndex = 1;
	TaskNode* node = new TaskNode;
	_indxTasks.push_back (node);

	if (storageName.empty ())
		storageName = "dflt";

	_taskFile = storageName + "tk.txt";
	_tableFile = storageName + "tt.txt";
	loadFromFile ();
}

list<Task> DataStorage::load (TimePeriod period)
{
	list<Task> taskList;
	list<TaskNode*>::iterator iter;
	list<TaskNode*>::iterator endIter = _indxTasks.end ();
	if ((period.get_start_time ().get_date () == Time::DFLT_DATE && period.get_end_time ().get_date () == Time::DFLT_DATE) ||
		(period.get_start_time ().get_date () == Time::INF_DATE && period.get_end_time ().get_date () == Time::INF_DATE)) {
		for (iter = _indxTasks.begin (), iter++; iter != endIter; iter++) {
			if ((*iter)->_active)
				taskList.push_back ((*iter)->_task);
		}
	} else {
		bool clash = false;
		for (iter = _indxTasks.begin (); iter != endIter; iter++) {
			if ((*iter)->_active) {
				if ((*iter)->_task.timeTask)
					clash = checkTimeClash (*iter, &period);
				else
					clash = checkPeriodClash (*iter, &period);
			}

			if (clash == true) {
				taskList.push_back ((*iter)->_task);
				clash = false;
			}
		}
	}

	return taskList;
}

bool DataStorage::checkTimeClash (TaskNode* node, TimePeriod* period) {
	if (period->operator== (node->_task.get_time ()))
		return true;

	Time time = node->_task.get_time ();
	Time start, end;

	if (node->_task.r_period.get_end_time () < period->get_end_time ())
		end = node->_task.r_period.get_end_time ();
	else
		end = period->get_end_time ();

	if (node->_task.r_period.get_start_time () < period->get_start_time ())
		start = period->get_start_time ();
	else
		start = node->_task.r_period.get_start_time ();

	bool clash = false;

	switch (node->_task.repeat) {
	case 0:
		clash = period->operator== (node->_task.get_time ());
		break;
	case 1:
		while (time > start) {
			--time;
			if (period->operator== (time))
				break;
		}
		if (time > start || time == start )
			clash = true;

		while (time < end) {
			++time;
			if (period->operator== (time))
				break;
		}
		if (time < end || time == end)
			clash = true;

		break;
	case 2:
		while (time > start) {
			time.modify_date (time.get_date () - 1);
			if (period->operator== (time))
				break;
		}
		if (time > start || time == start )
			clash = true;

		while (time < end) {
			time.modify_date (time.get_date () + 1);
			if (period->operator== (time))
				break;
		}
		if (time < end || time == end)
			clash = true;

		break;
	default:
		while (time > start) {
			time = time - node->_task.repeat;
			if (period->operator== (time))
				break;
		}
		if (time > start || time == start )
			clash = true;

		while (time < end) {
			time = time + node->_task.repeat;
			if (period->operator== (time))
				break;
		}
		if (time < end || time == end)
			clash = true;

		break;
	}

	return clash;
}

bool DataStorage::checkPeriodClash (TaskNode* node, TimePeriod* period) {
	if (period->operator== (node->_task.get_period ()))
		return true;

	TimePeriod __period = node->_task.get_period ();
	Time start, end, time;

	if (node->_task.r_period.get_end_time () < period->get_end_time ())
		end = node->_task.r_period.get_end_time ();
	else
		end = period->get_end_time ();

	if (node->_task.r_period.get_start_time () < period->get_start_time ())
		start = period->get_start_time ();
	else
		start = node->_task.r_period.get_start_time ();

	bool clash = false;

	switch (node->_task.repeat) {
	case 0:
		clash = period->operator== (node->_task.get_period ());
		break;
	case 1:
		time = __period.get_start_time ();
		while (time > start) {
			--time;
			if (period->operator== (time))
				break;
		}
		if (time > start || time == start )
			clash = true;

		time = __period.get_end_time ();
		while (time < end) {
			++time;
			if (period->operator== (time))
				break;
		}
		if (time < end || time == end)
			clash = true;

		break;
	case 2:
		time = __period.get_start_time ();
		while (time > start) {
			time.modify_date (time.get_date () - 1);
			if (period->operator== (time))
				break;
		}
		if (time > start || time == start )
			clash = true;

		time = __period.get_end_time ();
		while (time < end) {
			time.modify_date (time.get_date () + 1);
			if (period->operator== (time))
				break;
		}
		if (time < end || time == end)
			clash = true;

		break;
	default:
		time = __period.get_start_time ();
		while (time > start) {
			time = time - node->_task.repeat;
			if (period->operator== (time))
				break;
		}
		if (time > start || time == start )
			clash = true;

		time = __period.get_end_time ();
		while (time < end) {
			time = time + node->_task.repeat;
			if (period->operator== (time))
				break;
		}
		if (time < end || time == end)
			clash = true;

		break;
	}

	return clash;}

/*
bool DataStorage::checkClash (TaskNode* node, TimePeriod* period) {
	if (node->_task.timeTask)
		return period->operator== (node->_task.get_time ());
	else
		return period->operator== (node->_task.get_period ());
}

bool DataStorage::checkRepeatClash (TaskNode* node, TimePeriod* period) {
	if (node->_task.r_period != *period)
		return false;

	bool clash;
	switch (node->_task.repeat) {
	case 0:
		clash = false;
		break;
	case 1:
		break;
	case 2:
		break;
	default:
		if (node->_task.timeTask) {
			Time time = node->_task.get_time ();
			if (period->operator== (time)) {
				clash = true;
			} else if (period->operator< (time)) {
				while (period->operator< (time)) {
					time = time - node->_task.repeat;
					if (period->operator== (time)) {
						clash = true;
						break;
					}
				}

				if (period->operator> (time))
					clash = false;
			} else {
				while (period->operator> (time)) {
					time = time - node->_task.repeat;
					if (period->operator== (time)) {
						clash = true;
						break;
					}
				}

				if (period->operator< (time))
					clash = false;
			}
		} else {
			TimePeriod _period = node->_task.get_period ();
			if (period->operator== (_period)) {
				clash = true;
			} else if (period->operator< (_period)) {
				while (period->operator< (_period)) {
					_period = time - node->_task.repeat;
					if (period->operator== (_period)) {
						clash = true;
						break;
					}
				}

				if (period->operator> (_period))
					clash = false;
			} else {
				while (period->operator> (_period)) {
					time = time - node->_task.repeat;
					if (period->operator== (time)) {
						clash = true;
						break;
					}
				}

				if (period->operator< (time))
					clash = false;
			}
		}
		break;
	}

	return clash;

}
*/
list<Task> DataStorage::load (string tableName) {
	list<Task> taskList;
	list<TableNode*>::iterator iter;
	for (iter = _tables.begin (); iter != _tables.end (); iter++)
		if ((*iter)->_name == tableName)
			break;

	if (!(*iter)->_active)
		return taskList;

	list<TaskNode*>::iterator taskIter;
	for (taskIter = (*iter)->_tasks.begin (); taskIter != (*iter)->_tasks.end (); taskIter++) {
		if ((*taskIter)->_active)
			taskList.push_back ((*taskIter)->_task);
	}

	return taskList;
}

void DataStorage::exit () {
	reIndexing ();
	writeToFile ();
}

list<Task> DataStorage::get_alertTasks () {
	list<Task> alertTasks;
	list<TaskNode*>::iterator iter;
	list<TaskNode*>::iterator endIter = _indxTasks.end ();
	Time currTime;
	currTime.current_time ();
	for (iter = _indxTasks.begin (); iter != endIter; iter++)
		if ((*iter)->_active && !((*iter)->_task.alert < currTime))
			alertTasks.push_back ((*iter)->_task);

	return alertTasks;
}

vector<string> DataStorage::load_table_name () {
	vector<string> tableNames;
	list<TableNode*>::iterator iter;
	for (iter = _tables.begin (); iter != _tables.end (); iter++)
		if ((*iter)->_active)
			tableNames.push_back ((*iter)->_name);
	
	return tableNames;
}

list<DataStorage::Table> DataStorage::load_tables () {
	list<Table> tables;
	Table table;
	list<TableNode*>::iterator iter;
	for (iter = _tables.begin (); iter != _tables.end (); iter++)
		if ((*iter)->_active) {
			table.name = (*iter)->_name;
			table.period = (*iter)->_period;
			tables.push_back (table);
		}
	
	return tables;
	
}

void DataStorage::erase (list<int> taskIndex) {
	if (taskIndex.empty ()) return;
	
	taskIndex.sort ();
	list<int>::iterator iter = taskIndex.begin ();
	list<int>::iterator endIter = taskIndex.end ();
	list<TaskNode*>::iterator nodeIter = _indxTasks.begin ();
	list<TaskNode*>::iterator endNodeIter = _indxTasks.end ();
	list<TaskNode*> erasedTasks;
	int index = 0;
	while (iter != endIter && nodeIter != endNodeIter) {
		if (*iter == index) {
			(*nodeIter)->_active = false;
			erasedTasks.push_back (*nodeIter);
			iter++;
		}
		nodeIter++;
		index++;
	}

	appendToFile (&erasedTasks);
}

void DataStorage::erase (string tableName) {
	list<Task> taskList;
	list<TableNode*>::iterator iter;
	for (iter = _tables.begin (); iter != _tables.end (); iter++)
		if ((*iter)->_name == tableName)
			break;

	(*iter)->_active = false;
	setInactive (&((*iter)->_tasks));
	(*iter)->_tasks.clear ();
	appendToFile (*iter);
}

void DataStorage::setInactive (list<TaskNode*>* tasks) {
	list<TaskNode*>::iterator iter;
	for (iter = tasks->begin (); iter != tasks->end (); iter++) {
		(*iter)->_active = false;
	}
}

void DataStorage::clear () {
	_indxTasks.clear ();
	_tables.clear ();
	TaskNode* node = new TaskNode;
	_indxTasks.push_back (node);
	_largestIndex = 1;
	ofstream taskFile (_taskFile);
	taskFile.close ();
	ofstream tableFile (_tableFile);
	tableFile.close ();
}

void DataStorage::save (list<int> taskIndx) {
	list<TaskNode*> newTasks;
	list<TaskNode*>::iterator nodeIter = _indxTasks.begin ();
	list<int>::iterator iter;
	taskIndx.sort ();
	unsigned int i = 0;

	for (iter = taskIndx.begin (); iter != taskIndx.end (); iter++) {
		for (; i < *iter; i++, nodeIter++);
		(*nodeIter)->_active = true;
		newTasks.push_back (*nodeIter);
	}

	appendToFile (&newTasks);
}

void DataStorage::save (list<Task>* taskList) {
	if (taskList->empty ()) return;

	list<Task>::iterator iter;
	list<TaskNode*>::iterator nodeIter;
	list<TaskNode*> newTasks;
	for (iter = taskList->begin(); iter != taskList->end(); iter++) {	
		if (iter->_index == 0) {
			iter->_index = _largestIndex;
			_largestIndex++;
			TaskNode* node = new TaskNode;
			node->_active = true;
			node->_task = *iter;
			_indxTasks.push_back (node);
			newTasks.push_back (node);
		} else {
			unsigned int i;
			for (i = 0, nodeIter = _indxTasks.begin (); i < iter->_index; i++, nodeIter++);
			(*nodeIter)->_task = *iter;
			(*nodeIter)->_active = true;
			newTasks.push_back (*nodeIter);
		}
	}
	
	appendToFile (&newTasks);
}

void DataStorage::save (list<Task> taskList) {
	save (&taskList);
}

void DataStorage::save (TableNode* table, list<Task>* taskList) {
	if (taskList->empty ()) return;

	list<Task>::iterator iter;
	list<TaskNode*>::iterator nodeIter;
	list<TaskNode*> newTasks;
	for (iter = taskList->begin(); iter != taskList->end(); iter++) {	
		if (iter->_index == 0) {
			iter->_index = _largestIndex;
			_largestIndex++;
			TaskNode* node = new TaskNode;
			node->_active = true;
			node->_table = table->_name;
			node->_task = *iter;
			_indxTasks.push_back (node);
			table->_tasks.push_back (node);
			newTasks.push_back (node);
		} else {
			unsigned int i;
			for (i = 0, nodeIter = _indxTasks.begin (); i < iter->_index; i++, nodeIter++);
			(*nodeIter)->_task = *iter;
			(*nodeIter)->_table = table->_name;
			(*nodeIter)->_active = true;
			newTasks.push_back (*nodeIter);
		}
	}
	
	appendToFile (&newTasks);
}

void DataStorage::save (string tableName, list<Task> tasks) {
	if (_tables.empty ())
		return;

	Table __table;
	__table.name = tableName;
	list<TableNode*>::iterator tableIter;
	for (tableIter = _tables.begin (); tableIter != _tables.end (); tableIter++) {
		if ((*tableIter)->_name == tableName) {
			__table.period = (*tableIter)->_period;
			break;
		}
	}

	if (tableIter == _tables.end ())
		return;
	else
		save (__table, tasks);
}

void DataStorage::save (Table __table, list<Task> tasks) {
	bool existingTable = false;
	TableNode* table;
	list<TableNode*>::iterator tableIter;
	
	if (!_tables.empty ()) {
		for (tableIter = _tables.begin (); tableIter != _tables.end (); tableIter++) {
			if ((*tableIter)->_name == __table.name) {
				if ((*tableIter)->_active &&
					(*tableIter)->_period.get_start_time () == __table.period.get_start_time () &&
					(*tableIter)->_period.get_end_time () == __table.period.get_end_time ()) {
					save (*tableIter, &tasks);
				} else {
					setInactive (&((*tableIter)->_tasks));
					(*tableIter)->_period = __table.period;
					(*tableIter)->_active = true;
					(*tableIter)->_tasks.clear ();
					save (*tableIter, &tasks);
				}

				existingTable = true;
				table = *tableIter;
				break;
			}
		}
	}

	if (!existingTable) {
		TableNode* node = new TableNode;
		node->_active = true;
		node->_name = __table.name;
		node->_period = __table.period;
		_tables.push_back (node);
		save (node, &tasks);
		table = node;
	}

	appendToFile (table);
}

void DataStorage::reIndexing () {
	list<TableNode*>::iterator tableIter;
	for (tableIter = _tables.begin (); tableIter != _tables.end (); tableIter++) {
		if ((*tableIter)->_active) {
			removeInactiveTasks (&((*tableIter)->_tasks));
		} else {
			setInactive (&((*tableIter)->_tasks));
			tableIter = _tables.erase (tableIter);
			tableIter--;
		}
	}

	removeInactiveTasks (&_indxTasks);

	int newIndx = 0;
	list<TaskNode*>::iterator taskIter;
	for (taskIter = _indxTasks.begin (); taskIter != _indxTasks.end (); taskIter++) {
		(*taskIter)->_task._index = newIndx;
		newIndx++;
	}
	_largestIndex = newIndx;
}

//to be modified
void DataStorage::removeInactiveTasks (list<TaskNode*>* tasks) {
	list<TaskNode*>::iterator taskIter = tasks->begin ();
//	taskIter++;
	while (taskIter != tasks->end ())
		if (!(*taskIter)->_active) {
			taskIter = _indxTasks.erase (taskIter);
		} else {
			taskIter++;
		}
}

void DataStorage::writeToFile () {
	ofstream taskFile (_taskFile);
	list<TaskNode*>::iterator taskIter = _indxTasks.begin ();
	taskIter++;
	list<TaskNode*>::iterator endIter = _indxTasks.end ();
	for (; taskIter != endIter; taskIter++) {
		taskFile << (*taskIter)->_active << " ." << (*taskIter)->_table << endl;
		taskFile << (*taskIter)->_task.stringConvert () << endl;
	}
	taskFile.close ();

	ofstream tableFile (_tableFile);
	list<TableNode*>::iterator tabIter;
	for (tabIter = _tables.begin (); tabIter != _tables.end (); tabIter++) {
		tableFile << (*tabIter)->_active << " " << (*tabIter)->_name << " ";
		tableFile << (*tabIter)->_period.get_start_time ().get_date () << " ";
		tableFile << (*tabIter)->_period.get_start_time ().get_clock () << " ";
		tableFile << (*tabIter)->_period.get_end_time ().get_date () << " ";
		tableFile << (*tabIter)->_period.get_end_time ().get_clock () << " " << endl;
	}
	tableFile.close ();
}

void DataStorage::appendToFile (list<TaskNode*>* tasks) {
	ofstream taskFile (_taskFile, fstream::app);
	list<TaskNode*>::iterator taskIter;
	list<TaskNode*>::iterator endIter = tasks->end ();
	for (taskIter = tasks->begin (); taskIter != endIter; taskIter++) {
		taskFile << (*taskIter)->_active << " ." << (*taskIter)->_table << endl;
		taskFile << (*taskIter)->_task.stringConvert () << endl;
	}
	taskFile.close ();
}

void DataStorage::appendToFile (TableNode* node) {
	ofstream tableFile (_tableFile, fstream::app);
	tableFile << node->_active << " " << node->_name << " ";
	tableFile << node->_period.get_start_time ().get_date () << " ";
	tableFile << node->_period.get_start_time ().get_clock () << " ";
	tableFile << node->_period.get_end_time ().get_date () << " ";
	tableFile << node->_period.get_end_time ().get_clock () << " " << endl;
	tableFile.close ();
}

void DataStorage::loadFromFile () {
	string str;
	ifstream tableFile (_tableFile);
	list<TableNode*>::iterator tabIter;
	if (tableFile.is_open ()) {
		Time::date_t date;
		Time::clk_t clock;
		while (getline (tableFile, str)) {
			TableNode* node = new TableNode;
			stringstream strStream;
			strStream << str;
			strStream >> node->_active >> node->_name;
			strStream >> date >> clock;
			Time start (date, clock);
			node->_period.modify_start_time (start);
			strStream >> date >> clock;
			Time end (date, clock);
			node->_period.modify_end_time (end);

			for (tabIter = _tables.begin (); tabIter != _tables.end (); tabIter++) {
				if ((*tabIter)->_name == node->_name) {
					**tabIter = *node;
					delete node;
					break;
				}
			}

			if (tabIter == _tables.end ())
				_tables.push_back (node);
		}
		tableFile.close ();
	} else {
		ofstream outFile (_tableFile);
		outFile.close ();
	}

	ifstream taskFile (_taskFile);
	list<TaskNode*>::iterator taskIter;
	if (taskFile.is_open ()) {
		while (getline (taskFile, str)) {
			TaskNode *node = new TaskNode;
			stringstream strStream;
			strStream << str;
			strStream >> node->_active >> str;
			node->_table = str.substr (1);

			if (getline (taskFile, str)) {
				Task task (str);
				node->_task = task;
			}

			if (node->_task._index == _largestIndex) {
				_indxTasks.push_back (node);
				_largestIndex++;

				if (node->_table.empty () == false) {
					for (tabIter = _tables.begin (); tabIter != _tables.end (); tabIter++) {
						if ((*tabIter)->_name == node->_table) {
							(*tabIter)->_tasks.push_back (node);
							break;
						}
					}
				}
			} else if (node->_task._index < _largestIndex) {
				TaskNode* *ptr;
				ptr = find_pos (&_indxTasks, node->_task._index);
				**ptr = *node;
				delete node;
			} else;
		}
		taskFile.close ();
	} else {
		ofstream outFile (_taskFile);
		outFile.close ();
	}
}

list<Task> DataStorage::search (string keyword) {
	unsigned int pos;
	list<Task> taskList;
	list<TaskNode*>::iterator iter;
	for (iter = _indxTasks.begin (); iter != _indxTasks.end (); iter++) {
		if ((*iter)->_active) {
			pos = (*iter)->_task.note.find (keyword, 0);
			if (pos != string::npos) {
				taskList.push_back ((*iter)->_task);
				continue;
			}

			pos = (*iter)->_task.venue.find (keyword, 0);
			if (pos != string::npos) {
				taskList.push_back ((*iter)->_task);
				continue;
			}
		}
	}
	return taskList;
}

template <typename data_t>
data_t* find_pos (list<data_t>* dataList, int pos) {
	list<data_t>::iterator iter;
	int i;
	for (i = 0, iter = dataList->begin (); i < pos && iter != dataList->end (); iter++, i++);

	if (iter == dataList->end ())
		return NULL;
	else
		return &(*iter);
}