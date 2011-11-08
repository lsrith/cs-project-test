#include "ToDoMngr.h"
#include "CmdTextChange.h"
#include <list>
#include <queue>
#include <string>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <assert.h>
#include <iostream>
using namespace std;

string ToDoMngr::NOTE =   "Note:   ";
string ToDoMngr::VENUE =  "Venue:  ";
string ToDoMngr::SPACE =  "        ";
string ToDoMngr::TIME =   "Time:   ";
string ToDoMngr::ALERT =  "Alert:  ";
string ToDoMngr::REPEAT = "Repeat: ";
string ToDoMngr::NOTHING_TO_VIEW = "There are no tasks to view during the particular period/time!\n";
string ToDoMngr::WRONG_ID = "The entered idex is wrong";
string ToDoMngr::MSG_REPEAT_ERR = "The repeatition could not be made!";

list<Task> ToDoMngr::get_active_list () {
	return _activeTaskList;
}

list<Task> ToDoMngr::get_alert_list () {
	return _alertTaskList;
}

list<string> ToDoMngr::wrapSentence (string str, short int len) {
	list<string> sentences;
	string sentence;
	unsigned int pos = 0;

	do {
		pos = str.find ("  ", 0);
		
		if (pos == string::npos)
			break;

		str.erase (pos, 1);
	} while (pos != string::npos);

	while (!str.empty ()) {
		if (str.size () <= (unsigned int) len) {
			sentence = str;
			str.erase ();
		} else if (str[len] == ' ') {
			sentence = str.substr (0, len - 1);
			str.erase (0, len - 1);
		} else {
			pos = str.find_last_of (' ', len - 1);
			if (pos == string::npos && str.size () > (unsigned int) len) {
				sentence = str.substr (0, len);
				str.erase (0, len);
			} else if (pos == string::npos) {
				sentence = str.substr (0, pos);
				str.erase (0, pos);
			} else {
				sentence = str.substr (0, pos);
				str.erase (0, pos + 1);
			}
		}
		sentences.push_back (sentence);
	}

	return sentences;
}

string ToDoMngr::intToString (int num, int max) {
	ostringstream s_str;
	if (num == 0)
		return s_str.str ();

	int digit = 0;
	while (max > 0) {
		max /= 10;
		digit++;
	}

	s_str << setw (digit) << num << ".  ";
	return s_str.str ();
}

string ToDoMngr::view (Task task) {
	return view (task, 0, 1);
}

string ToDoMngr::view (Task task, int id, int numTasks) {
	list<string> strList;
	list<string>::iterator iter;
	string __id = intToString (id, numTasks);
	string _space;
	for (unsigned int i = 0; i < __id.size (); i++)
		_space += ' ';

	string __space = _space + SPACE;
	int __len = CmdTextChange::MAX_WIDTH - __space.size ();
	__id = intToString (id, numTasks);
	string __note, __venue, __duration, __alert, __repeat;
	string _note = __id + NOTE;
	string _venue = _space + VENUE;

	if (!task.note.empty ()) {
		strList = wrapSentence (task.note, __len);
		for (iter = strList.begin (); iter != strList.end (); iter++)
			__note += __space + *iter + "\n";
		__note.replace (0, _note.size (), _note);
	} else {
		__note = _note + '\n';
	}

	if (!task.venue.empty ()) {
		strList = wrapSentence (task.venue, __len);
		for (iter = strList.begin (); iter != strList.end (); iter++)
			__venue += __space + *iter + "\n";
		__venue.replace (0, _venue.size (), _venue);
	} else {
		__venue = _venue + '\n';
	}

	__duration = _space + TIME;
	if (task.timeTask)
		__duration += task.get_time ().string_date () + ' ' + task.get_time ().string_clock () + '\n';
	else
		__duration += task.get_period ().string_time_period () + '\n';

	Time dfltTime;
	if (task.alert != dfltTime) {
		__alert = _space + ALERT + task.alert.string_date () + ' ' + task.get_time ().string_clock () + '\n';
	}

	//to be modified
	if (task.repeat != 0) {
		__repeat = _space + REPEAT + repeat (task.repeat) + task.r_period.string_time_period () + '\n';
	}

	return __note + __venue + __duration + __alert + __repeat + '\n';
}

string ToDoMngr::repeat (unsigned int __repeat) {
	string str;
	switch (__repeat) {
	case 0:
		break;
	case 1:
		str = "Every month ";
		break;
	case 2:
		str = "Every year  ";
		break;
	case 60:
		str = "Every hour  ";
		break;
	case 1440:
		str = "Every day   ";
		break;
	case 10080:
		str = "Every week  ";
		break;
	case 20160:
		str = "Every 2 weeks ";
		break;
	default:
		str = intToString (__repeat, __repeat) + "mins ";
		break;
	}

	return str;
}

string ToDoMngr::view (list<Task> taskList) {
	if (taskList.empty ())
		return NOTHING_TO_VIEW;

	string str;
	int i, size = taskList.size ();
	list<Task>::iterator iter;
	for (i = 1, iter = taskList.begin (); i <= size ; i++, iter++) {
		str += view (*iter, i, size);
	}
	return str;
}

string ToDoMngr::view (int taskId) {
	if (taskId < 1 || taskId > (int) _activeTaskList.size ())
		return WRONG_ID;

	list<Task>::iterator iter = _activeTaskList.begin ();
	for (int i = 1; i < taskId; i++, iter++);
	return view (*iter, taskId, _activeTaskList.size ());
}

string ToDoMngr::view (view_t viewType, Time time) {
	TimePeriod period;
	time.modify_clock (0);
	period.modify_start_time (time);
	time.modify_clock (2359);
	
	switch (viewType) {
	case DAILY:
		period.modify_end_time (time);
		break;
	case WEEKLY:
		time = time + 7 * Time::DAY;
		period.modify_end_time (time);
		break;
	case MONTHLY:
		++time;
		period.modify_end_time (time);
		break;
	default:
		break;
	}
	
	_activeTaskList= _dataStorage.load(period);
	_activeTaskList.sort(Task::compareByStartTime);
	return view(_activeTaskList);
}

//to be modified
string ToDoMngr::view (TimePeriod period){

	if (Table_Mode) {
		return view (_table.name);
	} else {
		_activeTaskList = _dataStorage.load(period);
		return view (_activeTaskList);
	}
}

string ToDoMngr::reminder(){
	std::ostringstream oss,_format;
	Time time;
	time.current_time ();

	_format << setw(42) << setfill(' ') <<"--------------------------";
	_format << "\n" << setw(20) << setfill(' ') <<"| "<<"AGENDA  FOR  TODAY"<<" |"<<"\n";
	_format << setw(42) << setfill(' ') <<"--------------------------"<<"\n";

	_format << "\n";
	_format << view(DAILY,time);

	return _format.str();
}

/*pos = str.find_first_of (" ", 0)
str[pos]*/
string ToDoMngr::view (string tableN){
	
	_activeTaskList=_dataStorage.load(tableN);


	if(_activeTaskList.empty())
		return view(_activeTaskList);
	
	else
	    return _activeTaskList.front().get_period().get_start_time().string_date()+" to "+_activeTaskList.back().get_period().get_end_time().string_date()+"\n"+view(_activeTaskList);
}

string ToDoMngr::viewTableNames () {
	
	vector<string> tablenames=_dataStorage.load_table_name();
	ostringstream table_view,index_count;
	vector<string>::iterator browse;
	int index=1;
	if(tablenames.size()>0&&tablenames.size()<=9)
		index_count<<10;
	else
		index_count<<tablenames.size();

	table_view<<"\nTable Names In The System\n";

	for(browse=tablenames.begin();browse!=tablenames.end();browse++){

		
		table_view<<setw(index_count.str().length())<<setfill(' ')<<right<<index<<"."<<" ";
		table_view<<*browse<<"\n";
		index++;
	}


	return table_view.str();
}
	
//modify the front task's alert += mins
//resort ur alertTaskList
//set the alertActive as false;
/*string ToDoMngr::alert () {
	

	list<Task>::iterator time_control;
	std::ostringstream alerter;
    
	Time _time;
	
	
	  for(time_control =_activeTaskList.begin (); time_control!=_activeTaskList.end (); time_control++)
	  {   _time.current_time();
		  if(time_control->alert==_time){
			  alerter<<"ALERT"<<"\a"<<"\n";
			  alerter<<"You have the following task/tasks now"<<"\n";
		      
			 return view(DAILY,time_control->alert);

		  }



		  }
	//return "alert";
}*/

bool ToDoMngr::ifExistedTable (string tableName) {
	return false;
}

//ok
bool ToDoMngr::clashed(Task task){
// check if there is clashed of period tasks
	list<Task> checkList;
	checkList = _dataStorage.load(task.get_period());
	
	if(checkList.empty() == true){
		return false;;
	}
	else{
		list<Task>::iterator li = checkList.begin();
		bool clash = false; 
		for(int i=1; i<= checkList.size(); i++){
			TimePeriod taskPeriod = li->get_period();

			if(task.get_period() == taskPeriod){ //check if they clash
				_clashList.push_back(*li);
				clash = true;
				
				//check if there is duplication of the clashed task, delete duplication
				list<Task>::iterator li2 = checkList.begin();
				for(int i=1; i<checkList.size(); i++){
					if(task.compareByVenue(task,*li2) && task.compareByAlert(task, *li2) && task.compareByEndTime(task, *li2) && task.compareByStartTime(task, *li2)){
						_clashList.pop_back();
					}
				} // end of check for duplication of clashed task
			}
			li++;
		}// return back status of clash
		if(clash == true){
			return true;
		}
		else{
			return false;
		}
	}
} 

//ok
list<Task> ToDoMngr::add(Task task, bool forceAdd)                                                                      
{
//	cout<<tableName;
	if(Table_Mode){
		return add (_table.name, task, forceAdd);
	}

	else{
		list<Task> _addList;
		if(forceAdd == true || task.timeTask == true){ 
			if(task.repeat == 0 || (task.get_time().operator>(task.r_period.get_start_time()) 
				&& task.get_time().operator<(task.r_period.get_end_time()))){
				// forceAdd is true or is timetask 
				Task *ptr = &task;
				_addList.push_back(task);
				_dataStorage.save(_addList); 

				//add to undoStack;
				UserTask newAdd;
				newAdd._cmd = _addTask;
				newAdd._force = forceAdd;
				newAdd._task = task;
				int index = ptr->get_index();
				newAdd._index.push_back(index); 
				_undoStack.push(newAdd);

				// and return empty list
				_addList.clear();
				return _addList;
			}
			else {
			 throw("task is not in repeated period");
			}
		}
		else{
			bool clash = clashed(task);

			if(clash == true){
				return _clashList;
			}
			else{
				if(task.repeat == 0 || (task.get_period().get_start_time().operator>(task.r_period.get_start_time()) && 
					task.get_period().get_end_time().operator<(task.r_period.get_end_time()))){
					//ptr to address of task to get the index after adding to _dataStorage
					Task* ptr = &task;
					_addList.push_back(task);
					_dataStorage.save(&_addList); 
				
					//add to undoStack;
					UserTask newAdd;
					newAdd._cmd = _addTask;
					newAdd._force = forceAdd;
					newAdd._task = task;
					int index = ptr->get_index();
					newAdd._index.push_back(index); 
					_undoStack.push(newAdd);


					// and return empty list
					_addList.clear();
					return _addList;
				}
				else{
					throw("task is not in repeated period");
				}
			}
		}
	}
}

//ok
bool ToDoMngr::newTable(string name, TimePeriod period){  
	
		//check if the timetable period is valid
		if(period.get_start_time().get_date() == Time::INF_DATE || period.get_start_time().get_date() == Time::DFLT_DATE ||
			period.get_end_time().get_date() == Time::DFLT_DATE || period.get_end_time().get_date() == Time::DFLT_DATE) {
				return false;
		}

		else{
			if (activateTable (name)) {
				deactivateTable ();
				return true;
			} else {
				// no clash return true and save to dataStorage with empty taskIdxList
				list<Task> taskList;
				_table.name = name;
				_table.period = period;
				_dataStorage.save(_table, taskList);   
				Table_Mode=true;

				//add to undoStack
				UserTask newAdd;
				newAdd._cmd = _addTable1;
				newAdd._tableName = name;
				newAdd._period = period;
				_undoStack.push(newAdd);

				return true;
			}
		}
}

//ok
void ToDoMngr::erase(TimePeriod period){
	    list<Task> deleteList;
        deleteList = _dataStorage.load(period);
		list<int> deletedIdxList;
		list<Task>::iterator di = deleteList.begin();
		for(int i=0; i<deleteList.size(); i++){
			deletedIdxList.push_back(di->get_index());
			di++;
		}
		
		//add to undoStack
		UserTask newErase;
		newErase._cmd = _erasePeriod;
		newErase._period = period;
		newErase._index = deletedIdxList;
		_undoStack.push(newErase);

		_dataStorage.erase(deletedIdxList);
} 


void ToDoMngr::erase(string name){

	list<Task> deleteList;

	deleteList = _dataStorage.load(name);
	list<int> deleteIdxList;
	list<Task>::iterator di = deleteList.begin();
	for(int i=0; i<deleteList.size(); i++){
		deleteIdxList.push_back(di->get_index());
		di++;
	}

	//add to undoStack
	UserTask newErase;
	newErase._cmd = _eraseTable;
	newErase._index = deleteIdxList;
	newErase._tableName = name;
	_undoStack.push(newErase);

	_dataStorage.erase(deleteIdxList);		
}

//ok
Task ToDoMngr::erase(int taskId){
// delete from dataStorage given the taskId

	// locate the task in the _activeTaskList
	list<Task>::iterator di = _activeTaskList.begin();

	for(int i=1; i< taskId;i++)
	{
		di++;
	}     

	if(taskId <=_activeTaskList.size() && taskId > 0){

		//erase task for _activeTaskList
		Task deleteTask = *di;
		_activeTaskList.erase(di);     

		//delete from dataStorage
		list<int> deleteIdxList;
		deleteIdxList.push_back(deleteTask.get_index());

		//add to undoStack
		UserTask newErase;
		newErase._cmd = _eraseTask;
		newErase._task = deleteTask;
		newErase._taskId = taskId;
		newErase._index.push_back(deleteTask.get_index());
		_undoStack.push(newErase);

		_dataStorage.erase(deleteIdxList);
		return deleteTask; 
	}
	else 
	{
	 Task blanktask;
	 return blanktask;
	} 
}

list<Task> ToDoMngr::add(string tableName, Task task, bool forceAdd){

	TimePeriod activePeriod = _table.period;
	list<Task> taskList;
	task.repeat = 7 * Time::DAY;
	task.r_period = _table.period;

	//if is time task
	if(task.timeTask == true){
		// if timetask is within table period
		if(task.get_time().operator>(_table.period.get_start_time()) && task.get_time().operator<(_table.period.get_end_time())){
			taskList.push_back(task);
			_dataStorage.save(_table, taskList);
		}
		else{
			// if timetask is before table period
			if(task.get_time().operator<(_table.period.get_start_time()))
			{
				do{
					Time time = task.get_time () + 7*Time::DAY;
					task.modify_time(time);
				}while(task.get_time().operator<(_table.period.get_start_time()));
				taskList.push_back(task);
				_dataStorage.save(_table, taskList);
			}
			// if timetask is after table period
			else if(task.get_time().operator>(_table.period.get_end_time()))
			{
				while(task.get_time().operator>(_table.period.get_end_time()))
				{
					Time time = task.get_time () - 7*Time::DAY;
					task.modify_time(time);
				}
				taskList.push_back(task);
				_dataStorage.save(_table, taskList);
			}
		}
	}
	// if is period task
	else{
		if((task.get_period().get_end_time().operator-(task.get_period().get_start_time())) - ((_table.period.get_end_time().operator-(_table.period.get_start_time()) <0 )))
		{
			// if periodtask is within table period
			if(task.get_period().get_start_time().operator>(_table.period.get_start_time()) && task.get_period().get_end_time().operator<(_table.period.get_end_time()))
			{
        taskList.push_back(task);
				_dataStorage.save(_table, taskList);
			}
			//if task period is earlier than table period
			else if(task.get_period().get_start_time().operator<(_table.period.get_start_time()))
			{
				do
				{
					Time startTime = task.get_period().get_start_time()  + 7*Time::DAY;
					Time endTime = task.get_period().get_end_time()  + 7*Time::DAY;
					TimePeriod newPeriod(startTime, endTime);

					task.modify_period(newPeriod);
				}while(task.get_period().get_start_time().operator<(_table.period.get_start_time()));
				taskList.push_back(task);
				_dataStorage.save(_table, taskList);
			}
			// if task period is later than table period
			else if(task.get_period().get_start_time().operator>(_table.period.get_end_time()))
			{
				do
				{
					Time startTime = task.get_period().get_start_time()  - 7*Time::DAY;
					Time endTime = task.get_period().get_end_time()  - 7*Time::DAY;
					TimePeriod newPeriod(startTime, endTime);

					task.modify_period(newPeriod);
				}while(task.get_period().get_start_time().operator>(_table.period.get_end_time()));
				taskList.push_back(task);
				_dataStorage.save(_table, taskList);
			}
		}
	}
		
	
	taskList.clear();
	return taskList;
}

bool ToDoMngr::tillEnd(int duration, list<Task> taskList, Task task, bool forceAdd, TimePeriod activePeriod){
	bool got_clash = false;
 	Time::date_t newStartDate;
	Time::date_t newEndDate;

	for(int i=0; i<duration; i++){

		//add 7 days to the start and end time of the task for every iterations

		//if adding 7 days to the task will not exceed the end date of the period
		if((task.get_period().get_end_time()+(Time::DAY*7)) < activePeriod.get_end_time()){
			newStartDate = task.get_period().get_start_time().get_date() + ((Time::DAY*7)*i);
			newEndDate = task.get_period().get_end_time().get_date() + ((Time::DAY*7)*i);
		}
		
		task.get_period().get_start_time().modify_date(newStartDate);
		task.get_period().get_end_time().modify_date(newEndDate);



		if(forceAdd == true || task.timeTask == true){ 
			// forceAdd is true or is timetask 
			taskList.push_back(task);
		}

		else{
			if(clashed(task) == true){
				got_clash = true;
			}
			else{
				taskList.push_back(task);
			}
		}
	}
	return got_clash;
}

bool ToDoMngr::tillStart(int duration, list<Task> taskList, Task task, bool forceAdd, TimePeriod activePeriod){	
	bool got_clash = false;
 	Time::date_t newStartDate;
	Time::date_t newEndDate;

	for(int i=0; i<duration; i++){

		//add 7 days to the start and end time of the task for every iterations

		//if minus 7 days to the task will not exceed the start date of the period
		if((task.get_period().get_start_time()+(Time::DAY*7)) < activePeriod.get_start_time()){
			newStartDate = task.get_period().get_start_time().get_date() - ((Time::DAY*7)*i);
			newEndDate = task.get_period().get_end_time().get_date() - ((Time::DAY*7)*i);
		}
		
		task.get_period().get_start_time().modify_date(newStartDate);
		task.get_period().get_end_time().modify_date(newEndDate);

		if(forceAdd == true || task.timeTask == true){ 
			// forceAdd is true or is timetask 
			taskList.push_back(task);
		}

		else{
			if(clashed(task) == true){
				got_clash = true;
			}
			else{
				taskList.push_back(task);
			}
		}
	}
	return got_clash;
}

//ok
list<Task> ToDoMngr::edit(int taskId, TaskElement* taskElem, Task* task, bool forceEdit){
        list<Task> _blankList;
        // get the task that will be edited
        list<Task>::iterator taskIterator = _activeTaskList.begin();
        for(int i=1;i<taskId; i++){
                taskIterator++;
        }

        // store information into the undoStack;
        UserTask newEdit; 
        newEdit._cmd = _editTask;
        newEdit._force = forceEdit;
        newEdit._eTask = task;
        newEdit._taskElem = taskElem;
        newEdit._task = *taskIterator;

        // check taskElem for what to edit
        if(taskElem->_time == true){
                //edit time
                taskIterator->modify_time(task->get_time());
                //add the updated task to the UserTask
                newEdit._updatedTask = *taskIterator;
                _undoStack.push(newEdit);
				_dataStorage.save(_activeTaskList);
                return _blankList;
        }
        else if(taskElem->_period == true){
                //edit period when forcEdit == true
                if(forceEdit == true){
                        taskIterator->modify_period(task->get_period());
                        //add the updated task to the UserTask
                        newEdit._updatedTask = *taskIterator;
                        _undoStack.push(newEdit);
						_dataStorage.save(_activeTaskList);
                        return _blankList;
                }
                else{
                        //check for clash
                        bool clash;
                        //modify period and check if there is clash
                        taskIterator->modify_period(task->get_period());
                        clash = clashed(*taskIterator);
                        
                        if(clash == false){
                                newEdit._updatedTask = *taskIterator;
                                _undoStack.push(newEdit);
								_dataStorage.save(_activeTaskList);
                                return _blankList;
                        }
                        else{
                                //return clashList, note there is no UserTask added to the undoStack
                                return _clashList;
                        }
                }
        }
        else if(taskElem->_note == true){
                //edit note
                taskIterator->note = task->note;
                //add the updated task to the UserTask
                newEdit._updatedTask = *taskIterator;
                _undoStack.push(newEdit);
				_dataStorage.save(_activeTaskList);
                return _blankList;
        }
        else if(taskElem->_venue == true){
                //edit venue
                taskIterator->venue = task->venue;
                //add the updated task to the UserTask
                newEdit._updatedTask = *taskIterator;
                _undoStack.push(newEdit);
				_dataStorage.save(_activeTaskList);
                return _blankList;
        }
        else if(taskElem->_alert== true){
                taskIterator->alert = task->alert;
                //add the updated task to the UserTask
                newEdit._updatedTask = *taskIterator;
                _undoStack.push(newEdit);
				_dataStorage.save(_activeTaskList);
                return _blankList;
        }
        else if(taskElem->_repeat== true){
                taskIterator->repeat = task->repeat;
                //add the updated task to the UserTask
                newEdit._updatedTask = *taskIterator;
                _undoStack.push(newEdit);
				_dataStorage.save(_activeTaskList);
                return _blankList;
        }
}

void ToDoMngr::undo (){
	// if undoStack is not empty, get the top of the stack
	if(_undoStack.empty () == false){
		UserTask undoTask = _undoStack.top();

		if(undoTask._cmd == _addTask){
			//prev add, delete task from dataStorage
			undoTask._index.push_back (undoTask._ptr->get_index());

			_dataStorage.erase(undoTask._index);
		}
		else if(undoTask._cmd == _eraseTask){
			//prev erase, locate the task in activeTaskList
			list<Task>::iterator li = _activeTaskList.begin();
			for(int i=1; i<undoTask._taskId; i++){
				li++;
			}
		
			//insert to dataStorage
			_activeTaskList.insert(li, undoTask._updatedTask);
			
			//add task to dataStorage
			_dataStorage.save(undoTask._index);
		}
		else if(undoTask._cmd == _erasePeriod){
			_dataStorage.save(undoTask._index);
			
		}
		else if(undoTask._cmd == _addTable1){
			//prev add timetable, delete timetable from dataStorage
			_dataStorage.erase(undoTask._tableName);
		}
		else if(undoTask._cmd == _addTable2){
			//prev add task into timetable, delete timetable from dataStorage
			_dataStorage.erase(undoTask._tableName);

			//add a blank timetable with the same name back to dataStorage
			list<Task> blankList;
			_dataStorage.save(undoTask._tableName, blankList);
		}
		else if(undoTask._cmd == _eraseTable){
			//add new table with no task
			list<Task>blankList;
			_dataStorage.save(undoTask._tableName, blankList);

			//add add task into the table back
			_dataStorage.save(undoTask._index);
			
		}
		else if(undoTask._cmd == _editTask){
			//prev function is edit, undo the change
			//find the task in the _activeTaskList;
			list<Task>::iterator li = _activeTaskList.begin();
			for(int i=1; i<undoTask._taskId; i++){
				li++;
			}
			//erase the edited task and add the original task
			list<Task> dummyList;
			list<int> dummyIntList;
			_activeTaskList.erase(li);
			_dataStorage.erase(dummyIntList);

			// add back original task
			dummyList.push_back(*li);
			_activeTaskList.insert(li, undoTask._task);
			_dataStorage.save(dummyList);
		}
		_undoStack.pop();
		_redoStack.push(undoTask);
	}
  else{
  }
}

void ToDoMngr::redo () {
	if(_redoStack.empty() == false){
		UserTask redoTask = _redoStack.top();

		if(redoTask._cmd == _addTask){
			list <Task> blankList;
			blankList = add(redoTask._updatedTask, redoTask._force);
		}
		else if(redoTask._cmd == _eraseTask){
			Task blankTask;
			blankTask = erase(redoTask._taskId);
		}
		else if(redoTask._cmd == _addTable1){
			bool blankBool;
			blankBool = newTable(redoTask._tableName, redoTask._period);
		}
		else if(redoTask._cmd == _addTable2){
			list <Task> blankList;
			blankList = add(redoTask._tableName, redoTask._period, redoTask._force);
		}
		else if(redoTask._cmd == _eraseTable){
			erase(redoTask._tableName);
		}
		else if(redoTask._cmd == _editTask){
			list<Task>blankList;
			blankList = edit(redoTask._taskId, redoTask._taskElem, redoTask._eTask, redoTask._force);
		}	
		else if(redoTask._cmd == _erasePeriod){
			erase(redoTask._period);
		}
		_redoStack.pop();
		_undoStack.push(redoTask);
	}
	else{
	}
}

//Rith
string ToDoMngr::search (search_t type, string phrase) {
	list<Task> taskList;
	if (type == SEXACT) {
		taskList = _dataStorage.search (phrase);
	} else if (type == SEACH) {
		list<string> keyWords;
		string str;
		int end_pos;

		while (!phrase.empty ()) {
			end_pos = phrase.find_first_of (' ', 0);
			str = phrase.substr (0, end_pos);
			keyWords.push_back (str);
			if (end_pos == string::npos)
				phrase.erase ();
			else
				phrase.erase (0, end_pos + 1);
		}

		list<string>::iterator iter;
		for (iter = keyWords.begin (); iter != keyWords.end (); iter++) {
			taskList.splice (taskList.end (), _dataStorage.search (*iter));
		}

		removeSameIdx (&taskList);
	} else;

	return view (taskList);
}

void ToDoMngr::exit () {
	_dataStorage.exit ();
	_activeTaskList.clear ();
	_clashList.clear ();
	
	while(_redoStack.empty() == false){
		_redoStack.pop();
	}

	while(_undoStack.empty() == false){
		_undoStack.pop();
	}

}

void ToDoMngr::clear () {
	_dataStorage.clear ();
}

ToDoMngr::ToDoMngr () {
	Table_Mode=false;
	_alertActive=false;
	
	_dataStorage.updateStorageName ("");
	_alertTaskList=_dataStorage.get_alertTasks();
}

ToDoMngr::ToDoMngr (string storageName) {
	Table_Mode=false;
	_alertActive=false;
	
	_dataStorage.updateStorageName (storageName);
	_alertTaskList=_dataStorage.get_alertTasks();
}

void ToDoMngr::updateStorageName (string storageName) {
	_dataStorage.exit ();
	_activeTaskList.clear ();
	_clashList.clear ();
	_alertTaskList.clear();
	_dataStorage.updateStorageName (storageName);
	_alertTaskList=_dataStorage.get_alertTasks();
}

list<Task> ToDoMngr::edit (int taskId, Task* task, bool forceEdit) {
	TaskElement taskElem;
	setTaskElem (&taskElem, task);
	return edit (taskId, &taskElem, task, forceEdit);
}

void ToDoMngr::setTaskElem (ToDoMngr::TaskElement* taskElem, Task* task) {
	Time dfltTime;
	if (task->get_time () != dfltTime)
		taskElem->_time = true;
	else
		taskElem->_time = false;

	if (task->get_period ().get_start_time () != dfltTime || task->get_period ().get_end_time () != dfltTime)
		taskElem->_period = true;
	else
		taskElem->_period = false;

	if (!task->note.empty ())
		taskElem->_note = true;
	else
		taskElem->_note = false;

	if (!task->venue.empty ())
		taskElem->_venue = true;
	else
		taskElem->_venue = false;

	if (task->alert != dfltTime)
		taskElem->_alert = true;
	else
		taskElem->_alert = false;

	if (task->repeat != 0)
		taskElem->_repeat = true;
	else
		taskElem->_repeat = false;
}

bool ToDoMngr::activateTable (string tableN) {
	list<DataStorage::Table> tables = _dataStorage.load_tables();

	list<DataStorage::Table>::iterator browse;
	bool found = false;;
	for(browse = tables.begin() ; browse != tables.end(); browse++){
		if(tableN == browse->name){
			Table_Mode = true;
			_table = *browse;
            found= true;
			break;
		}		
	}
	return found;
}

void ToDoMngr::deactivateTable () {
	Table_Mode=false;
}

bool ToDoMngr::ifTableMode () {
	return Table_Mode;
}

bool ToDoMngr::ifAlertActive (){
	_alertTaskList.sort(Task::compareByAlert);
	Time _current;
	_current.current_time();
	if(!(_current > _alertTaskList.front().alert) ){
		_alertActive=true;
		return true;
	}
	else
		return false;

}
//compare the currTime with the front of the alertTaskList, do not use currTime == alertTime, but !(currTime > alertTime) (safer)
//set a boolean private variable alertActive to true
string ToDoMngr:: alert (){

	return view(DAILY,_alertTaskList.front().alert);

}

//view the front of the alertTaskList;
//do not pop it, keep the alertActive as true
void ToDoMngr::deactivateAlert (){

	_alertTaskList.pop_front();
	_alertActive=false;
}

//pop the front of the alertTaskList;
//set the alertActive as false;
void ToDoMngr:: snoozeAlert (int mins){
	_alertTaskList.front().alert=_alertTaskList.front().alert+mins;
	_alertTaskList.sort(Task::compareByAlert);
	_alertActive=false;
  }

void ToDoMngr:: push_alertTask (Task tsk){
	list<Task>::iterator eraser;
	Time _current;
	_current.current_time();

	      if(tsk.alert>_current)
			_alertTaskList.push_back(tsk);
		
		  _alertTaskList.sort(Task::compareByAlert);
	
}
//push inside the task if its alert is to be in the future, push it a way that u got it in order
void ToDoMngr::pop_alertTask (Task tsk){
	list<Task>::iterator eraser;
	

	for(eraser=_alertTaskList.begin();eraser!=_alertTaskList.end();eraser++)
	{ 
		if(eraser->alert==tsk.alert){
			_alertTaskList.erase(eraser);
		
		}

			 }
}
//delete that Task from ur alertTaskList

list<Task>* ToDoMngr::listTask (Task _task) {
	list<Task>* taskList = new list<Task>;
	taskList->push_back (_task);
	if (_task.repeat == 0) {
		return taskList;
	}

	TimePeriod* r_period = &_task.r_period;
	int __dur = r_period->get_end_time () - r_period->get_start_time ();

	if (_task.repeat == 1) {
		if (__dur / (30 * Time::DAY) > 5000)
			throw (MSG_REPEAT_ERR);

		if (_task.timeTask) {
			Time* time = &(_task.get_time ());

			if (_task.r_period != *time)
				throw (MSG_REPEAT_ERR);

			while (time->operator< (r_period->get_end_time ())) {
				if (time->operator++ ())
					break;

				_task.modify_time (*time);
				taskList->push_back (_task);
			}

			*time = _task.get_time ();
			while (time->operator> (r_period->get_start_time ())) {
				if (time->operator-- ())
					break;

				_task.modify_time (*time);
				taskList->push_back (_task);
			}
		} else {
			TimePeriod* period = &(_task.get_period ());
			if ((_task.r_period < *period && _task.r_period > *period) ||
				((_task.r_period.get_end_time () - _task.r_period.get_start_time ())
				< (period->get_end_time () - period->get_start_time ())))
				throw (MSG_REPEAT_ERR);
			
			Time start = period->get_start_time ();
			Time end = period->get_end_time ();
			while (period->operator< (r_period->get_end_time ())) {
				if (++start || ++end)
					break;

				period->modify_end_time (end);
				period->modify_start_time (start);
				_task.modify_period (*period);
				taskList->push_back (_task);
			}

			period = &(_task.get_period ());
			start = period->get_start_time ();
			end = period->get_end_time ();
			while (period->operator> (r_period->get_start_time ())) {
				if (--start || --end)
					break;

				period->modify_start_time (start);
				period->modify_end_time (end);
				_task.modify_period (*period);
				taskList->push_back (_task);
			}
		}
	} else if (_task.repeat == 2) {
	} else if (_task.repeat > 3) {
		if (__dur / _task.repeat > 5000)
			throw (MSG_REPEAT_ERR);

		if (_task.timeTask) {
			Time* time = &(_task.get_time ());

			if (_task.r_period != *time)
				throw (MSG_REPEAT_ERR);

			while (time->operator< (r_period->get_end_time ())) {
				*time = (time->operator+ (_task.repeat));
				_task.modify_time (*time);
				taskList->push_back (_task);
			}

			*time = _task.get_time ();
			while (time->operator> (r_period->get_start_time ())) {
				*time = (time->operator- (_task.repeat));
				_task.modify_time (*time);
				taskList->push_front (_task);
			}
		} else {
			TimePeriod* period = &(_task.get_period ());
			if ((_task.r_period < *period && _task.r_period > *period) ||
				((_task.r_period.get_end_time () - _task.r_period.get_start_time ())
				< (period->get_end_time () - period->get_start_time ())))
				throw (MSG_REPEAT_ERR);

			while (period->operator< (r_period->get_end_time ())) {
				period->modify_end_time (period->get_end_time () + _task.repeat);
				period->modify_start_time (period->get_start_time () + _task.repeat);
				_task.modify_period (*period);
				taskList->push_back (_task);
			}

			*period = _task.get_period ();
			while (period->operator> (r_period->get_start_time ())) {
				period->modify_start_time (period->get_start_time () - _task.repeat);
				period->modify_end_time (period->get_end_time () - _task.repeat);
				_task.modify_period (*period);
				taskList->push_back (_task);
			}
		}
	} else;

	return taskList;
}

void ToDoMngr::removeSameIdx (list<Task>* taskList) {
	taskList->sort (Task::compareByIndex);
	if (taskList->size () < 2)
		return;

	list<Task>::iterator prev, curr = taskList->begin ();
	do {
		prev = curr;
		curr++;
		
		if (curr != taskList->end () && curr->get_index () == prev->get_index ())
			curr = taskList->erase (prev);

	} while (curr != taskList->end ());
}