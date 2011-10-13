#include <iostream>
#include "ToDoMngr.h"
#include "TimePeriod.h"
#include "Task.h"
#include "DataStorage.h"
#include <list>
#include <queue>
#include <string>
#include <sstream>
#include <fstream>
#include <ctime>
#include <iomanip>
using namespace std;

ToDoMngr::ToDoMngr () {	
}

list<Task> ToDoMngr::get_active_list () {
	return _activeTaskList;
}

 string ToDoMngr::view(Task& taskId){
	    Time _time;
		 std::ostringstream tsk;

		if(taskId.get_time()==_time)
		{
			tsk<<taskId.get_period().string_time_period()<<" "<<" "<<taskId.note<<" "<<taskId.venue;
		}
		else
		{
    
		tsk<<taskId.get_time().string_date()<<" "<<taskId.get_time().string_clock()<<" "<<taskId.note<<" "<<taskId.venue;
		}
        return tsk.str();
}

string ToDoMngr:: view(list<Task> tasklist){
        Time obj;
        std::ostringstream oss;
        list<Task> taskl=tasklist;
    list<Task>::iterator l;
	int index=1;
	if(tasklist.empty())
		oss<<" ";
	else{

        for(l =taskl.begin (); l !=taskl.end (); l++)
                {   
                        if(l->get_time()==obj){
                                oss<<l->get_period().string_time_period()<<"\n"<<index<<"."
                                <<" "<<l->note<<" "<<l->venue<<endl;
                        }
                        else
                        {
							oss<<l->get_time().string_date() << " at " <<l->get_time().string_clock()<<"\n"<<index<<"."
                                <<" "<<l->note<<" "<<l->venue<<endl;

                        }
						index++;
        }
	}
   return oss.str();
	
}

string ToDoMngr::view (int taskId){
        
            Time obj;
			ToDoMngr todo;
        std::ostringstream tsk;
        int i=1;
              // list<Task> _activeTasklist;
		list<Task>::iterator _taskpointer;

				for(_taskpointer=todo.get_active_list().begin();_taskpointer!=todo.get_active_list().end();_taskpointer++)
				{
					if(i<taskId){
						i++;
						continue;
					}
					else
						break;
					
                
                /*for(int i=1;i<taskId;i++){
					if(_taskpointer!=todo.get_active_list().end())
				     ++_taskpointer;*/
                }
                
                        if(_taskpointer->get_time()==obj){
                
        tsk<<_taskpointer->get_period().string_time_period ()<<
                                " "<<_taskpointer->note<<" "<<_taskpointer->venue<<endl;
                        }
                        else
                        {
							tsk<<_taskpointer->get_time().string_date()<<_taskpointer->get_time().string_clock()
                                <<" "<<_taskpointer->note<<" "<<_taskpointer->venue<<endl;
                        }

                         return tsk.str();
                
        }   // return a string of the view of the specific task

string ToDoMngr::view(view_t viewType, Time time){
        
        string  DATE;
        std::ostringstream start_date,end_date,check;
        list<Task> _activeTaskList;
        list<Task> l;
        check<<time.get_date();
        DATE=check.str();
        string Feb="28";
        string MONTHS[12]={"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
        string month_nofDays[12]={"31",Feb,"31","30","31","30","31","31","30","31","30","31"};
        string days[7]={"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};
        string datecreator;
        string date,Month,Year;
        int _index;

        date=DATE.substr(0,2);
        Month=DATE.substr(2,2);
        Year=DATE.substr(4,DATE.length());
        int newStartDate,newEndDate;
        int newMonth;

        if(viewType==DAILY){
                
                
                Time start(atoi((DATE.substr(0,DATE.length())).c_str()),000);
                Time end(atoi((DATE.substr(0,DATE.length())).c_str()),2359);
                TimePeriod period(start,end);
                _activeTaskList= _dataStorage.load(period);
                
                return DATE+" "+":"+"\n"+view(_activeTaskList);
        }
                 
        else if(viewType=WEEKLY){
      Task _day;
         int day;
	for(int i=0;i<7;i++){
		day=_day.get_time().get_day();
		if(_day.get_time().display_day(day)==days[i]){
                         _index=i+1;
                        break;
                }
        }
        
    newStartDate=atoi(date.c_str()) -_index;
        newMonth=atoi(Month.c_str());
        
        if(atoi(Year.c_str())%400 ==0 || (atoi(Year.c_str())%100 != 0 && atoi(Year.c_str())%4 == 0)     ){ 
            if((Month=="02"&&newStartDate+6>29)||((Month=="01"||Month=="03"||Month=="05"||Month=="07"||Month=="08"||Month=="10"||Month=="12")
                        &&newStartDate+6>31)||((Month=="04"||Month=="06"||Month=="09"||Month=="11")&&newStartDate+6>30))
                        newMonth=atoi(Month.c_str())+1;
        }
                else {
                         if((Month=="02"&&newStartDate+6>28)||((Month=="01"||Month=="03"||Month=="05"||Month=="07"||Month=="08"||Month=="10"||Month=="12")
                        &&newStartDate+6>31)||((Month=="04"||Month=="06"||Month=="09"||Month=="11")&&newStartDate+6>30))
                        newMonth=atoi(Month.c_str())+1;
                }

                start_date<<std::setfill('0') << std::setw(2)<<newStartDate;
            start_date<<std::setfill('0') << std::setw(2)<<newMonth;
                start_date<<Year;
                Time start(atoi((start_date.str()).c_str()),000);
                        
                 newEndDate=newStartDate+6;
                 end_date<<std::setfill('0') << std::setw(2)<<newStartDate;
                 end_date<<std::setfill('0') << std::setw(2)<<newMonth;
             end_date<<Year;
             Time end(atoi((end_date.str()).c_str()),2359);
                 
                 TimePeriod period(start,end);
                 _activeTaskList= _dataStorage.load(period);
          return view(_activeTaskList);
        }

        else if(viewType==MONTHLY){
                
                if(DATE.at(2)+DATE.at(3)=='02')
                                        {
                                                if(atoi(Year.c_str())%400 ==0 || (atoi(Year.c_str())%100 != 0 && atoi(Year.c_str())%4 == 0)     ) 
                                                        Feb="29";
                                        }

                                        start_date<<"01"<<Month<<Year;
                                        Time start (atoi(start_date.str().c_str()), 000);
                                        end_date<<month_nofDays[(((DATE.at(2)+DATE.at(3))-'0')-1)]<<Month<<Year;
                                        Time end(atoi(end_date.str().c_str()), 2359);

                                        TimePeriod period(start,end);
                                         _activeTaskList= _dataStorage.load(period);
                      return view(_activeTaskList);

        }



} // return a string of the view by day, by week, or by month
       
string ToDoMngr::view (TimePeriod period){
list<Task> _activeTaskList;

_activeTaskList= _dataStorage.load(period);
 
return view(_activeTaskList);

}

string ToDoMngr::help (string command){

        ifstream myhelpfile;
        myhelpfile.open("HELP.txt");
        string HELP_LINE;
		ostringstream str;

        if(command.empty ()){
                while(getline(myhelpfile, HELP_LINE))
                {
                        str<<HELP_LINE<<endl;
                }
        }

		return str.str ();
}

string ToDoMngr::reminder(){

        string Date;
        string Month;
        string FINAL_FORMAT;
        std::ostringstream oss;

        time_t t = time(0);   //get the current time
        struct tm * now = localtime( & t );
        oss<<now->tm_mday<<(now->tm_mon + 1)<<(now->tm_year + 1900) ;

        Date=oss.str();        //Stores the current computer date

        Time time(atoi(Date.c_str()),t);
        return "Your Tasks for the day are as follows"+view(DAILY,time);   //pass to view() to search for the tasks on that particular date and display it...year is not taken into account at present.




}

list<Task> ToDoMngr::add(Task task, bool forceAdd)
{
 if(forceAdd == true)
 { 
  // forceAdd is true, save to dataStorage
  list<Task> addList;
  addList.push_back(task);
  _dataStorage.save(addList);   
  addList.clear();
  
  // return empty list
  return addList;
 }
  else
 {
  // check for clash
  list<Task> clashedTimeList;
  clashedTimeList = _dataStorage.load(task.get_period()) ;
  
  if(clashedTimeList.size() == 0)
  {
   list<Task> addList;
   addList.push_back(task);
   _dataStorage.save(addList);
   return addList;
  }
  else
  {
   _clashList.push_back(task);
   return _clashList;
  }
 }
}

/*
bool ToDoMngr::newTable (string name, TimePeriod period)
{
 bool clashed;
 list<string> existedTableName;  

 //load existing table name
 existedTableName = _dataStorage.load_table_name();
 
 //check for clashes of name with existing table names
 list<string>:: iterator tableNameIter = existedTableName.begin();
 for(int i=1; i<=existedTableName.size(); i++)
 {
  if(*tableNameIter == name)
  {
   clashed = true;
  }
  tableNameIter++;
 }
 
 // if there is not clash of name 
 // save to dataStorage with a empty list of taskId
 if(clashed == true)
 {
  return false;
 }
 else 
 {
  list<int> taskIdxList;
  _dataStorage.save(name, period, taskIdxList);
  return true;
 }
}

void ToDoMngr::erase(TimePeriod period)
{
 //get id of task in that period
 list<Task> deleteList;
 deleteList = _dataStorage.load(period);
 list<Task>::iterator di = deleteList.begin();
 list<int> deletedIdx;
 
 int size = deleteList.size();
 Task activeTask;
 
 for(int i=1; i<=size;i++)
 {
  deletedIdx.push_back(di->get_index());
  deletedIdx.push_back(activeTask.get_index());
  deleteList.pop_front();
 }
 
 //delete from dataStorage the tasks with the id in the list
 _dataStorage.erase(deletedIdx); 
} 

 void ToDoMngr::erase(string name)
{
 // load back the list of task in the timetable
 // get the idx of the tasks in the list
 
 list<Task> deleteList;
 
 deleteList = _dataStorage.load(name);
 list<Task>::iterator di = deleteList.begin();
 list<int> deletedIdx;
 
 int size = deleteList.size();
 for(int i=1; i<deleteList.size();i++)
 {
  deletedIdx.push_back(di->get_index());
  di++;
 }
 
 // delete from dataStorage
 _dataStorage.erase(deletedIdx); 
}


Task ToDoMngr::erase(int taskId)
{
 list<Task>::iterator di = _activeTaskList.begin();

 Task deletetask;
 
 // find the task with the taskId from _activeList and delete it
 for(int i = 1; i<_activeTaskList.size(); i++)
 {
  if( di->get_index() == taskId)
  if( _activeTaskList.get_index() == taskId)
  {
   deletetask = *di;
   _activeTaskList.erase(di);
   di++;
  }
 }
  
 // delete it from dataStorage
 list<int> deleteList; 
 deleteList.push_back(deletetask.get_index ());
 _dataStorage.erase(deleteList);
 
 // return deleted task
 return deletetask;  
}





list<Task> ToDoMngr::add(string tableName, Task task, bool forceAdd);
{
 //create a list and push task into list and add to dataStorage when tableName == NULL
 list<Task> addList;
 
 if(tableName.empty())
 {
  addList.push_back(task);
  add(addList,forceAdd);
 }

 else 
 {
  
  list<int> taskIdList;
  taskIdList.push_back(task.get_index());
  _dataStorage.save(tableName, task.get_period(), taskIdList);
 }
}
}
*/