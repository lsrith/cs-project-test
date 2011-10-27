#include "ToDoMngr.h"
#include <list>
#include <queue>
#include <string>
#include <sstream>
#include <fstream>
#include <ctime>
#include <iomanip>
#include <iostream>
using namespace std;


ToDoMngr::ToDoMngr () {	
}

list<Task> ToDoMngr::get_active_list () {
	return _activeTaskList;
}

string ToDoMngr::view(Task taskId){
     Time obj;
                       
						
		int full_date,_date,temp_date;
		temp_date=0;
		string month,temp_month;
		temp_month=" ";
		int year;
        std::ostringstream oss;
       
              // list<Task> _activeTasklist;
               
                
                        if(taskId.get_time()==obj){
                
                    full_date=taskId.get_period().get_start_time().get_date();
					_date=full_date/1000000;
				     month=taskId.get_time().display_month((full_date % 1000000) / 10000);
					year=full_date%10000;
				
								oss<<"\n";
                                oss<<month<<" "<<year<<"  ";
							    oss<<_date<<" "<<taskId.get_time().display_day(taskId.get_period().get_start_time().get_day())<<"\n";
								oss<<"\n";
							
								oss<<taskId.get_period().get_start_time().string_clock()<<" "<<"-"<<" "<<taskId.get_period().get_end_time().string_clock();
								oss<<" "<<taskId.note<<" "<<"at"<<" "<<taskId.venue;
								
							}
							
                        
                        else
                        {
							full_date=taskId.get_time().get_date();
					    _date=full_date/1000000;
					     month=taskId.get_time().display_month((full_date % 1000000) / 10000);
					     year=full_date%10000;
				
							
								oss<<"\n";
                                oss<<month<<" "<<year<<"  ";
							    oss<<_date<<" "<<taskId.get_time().display_day(taskId.get_time().get_day())<<"\n";
								oss<<"\n";
								
								oss<<taskId.get_time().string_clock();
								oss<<" "<<taskId.note<<" "<<"at"<<" "<<taskId.venue;
								
                               /* oss<<l->get_period().string_time_period()<<"\n"<<index<<"."
                                <<" "<<l->note<<" "<<"at"<<" "<<l->venue<<endl;*/
							}
                        

                         return oss.str();
	
	
	/*list<Task> _tempList;
	 _tempList.push_back(taskId);
	 return view(_tempList);*/
	
	/* Time _time;
                 std::ostringstream tsk;
				 int full_date,_date,temp_date;
		temp_date=0;
		string month,temp_month;
		temp_month=" ";
		int year;

                if(taskId.get_time()==_time)
                {
					     full_date=taskId.get_period().get_start_time().get_date();
					_date=full_date/1000000;
					
					
					month=taskId.get_time().display_month((full_date % 1000000) / 10000);
					year=full_date%10000;
				
							if(temp_month==month){
							}
							else{
								tsk<<"\n";
                             tsk<<month<<" "<<year<<"\n";
							tsk<<"--------"<<"\n";
							temp_month=month;
							temp_date=0;
						}

									if(temp_date==_date){
									
									//oss<<index<<"."<<" ";
								tsk<<taskId.get_period().get_start_time().string_clock()<<" "<<"-"<<" "<<taskId.get_period().get_end_time().string_clock();
								tsk<<" "<<taskId.note<<" "<<"at"<<" "<<taskId.venue<<endl;
									}
									else
									{
								tsk<<"\n";		
								tsk<<_date<<" "<<l->get_time().display_day(l->get_period().get_start_time().get_day())<<"\n";
								tsk<<"\n";
								//oss<<index<<"."<<" ";
								tsk<<taskId.get_period().get_start_time().string_clock()<<" "<<"-"<<" "<<taskId.get_period().get_end_time().string_clock();
							    tsk<<" "<<taskId.note<<" "<<"at"<<" "<<taskId.venue<<endl;
								temp_date=_date;
                            
							}
							
                        //tsk<<taskId.get_period().string_time_period()<<" "<<" "<<taskId.note<<" "<<taskId.venue;
                }
                else
                {
                      full_date=taskId.get_time().get_date();
					_date=full_date/1000000;
					
					
					month=taskId.get_time().display_month((full_date % 1000000) / 10000);
					year=full_date%10000;
				
							if(temp_month==month){
							}
							else{
								tsk<<"\n";
                            	tsk<<month<<" "<<year<<"\n";
								tsk<<"--------"<<"\n";
							temp_month=month;
							temp_date=0;
						}

									if(temp_date==_date){
									
									//oss<<index<<"."<<" ";
										tsk<<taskId.get_time().string_clock();
								    	tsk<<" "<<taskId.note<<" "<<"at"<<" "<<taskId.venue<<endl;
									}
									else
									{
									tsk<<"\n";		
									tsk<<_date<<" "<<taskId.get_time().display_day(taskId.get_time().get_day())<<"\n";
									tsk<<"\n";
								//oss<<index<<"."<<" ";
									tsk<<taskId.get_time().string_clock();
									tsk<<" "<<taskId.note<<" "<<"at"<<" "<<taskId.venue<<endl;
								temp_date=_date;
                            
							}
							
                //tsk<<taskId.get_time().string_date()<<" "<<taskId.get_time().string_clock()<<" "<<taskId.note<<" "<<taskId.venue;
                }
        return tsk.str();*/
}

string ToDoMngr:: view(list<Task> tasklist){
        Time obj;
		int full_date,_date,temp_date,temp_year;
		temp_date=0;
		string month,temp_month;
		temp_month=" ";
		temp_year=0;
		int year;
	
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
							full_date=l->get_period().get_start_time().get_date();
					_date=full_date/1000000;
					
					
					month=l->get_time().display_month((full_date % 1000000) / 10000);
					year=full_date%10000;
				
							if(temp_month==month){
							}
							else{
								oss<<"\n";
                             oss<<month<<" ";
							 
							 if(temp_year==year)
							oss<<temp_year<<"\n";
							 else
						    oss<<year<<"\n";
							
							 oss<<"--------"<<"\n";
							temp_month=month;
							temp_year=year;
							temp_date=0;
						}

									if(temp_date==_date){
									
									oss<<index<<"."<<" ";
								oss<<l->get_period().get_start_time().string_clock()<<" "<<"-"<<" "<<l->get_period().get_end_time().string_clock();
								oss<<" "<<l->note<<" "<<"at"<<" "<<l->venue<<endl;
									}
									else
									{
								oss<<"\n";		
								oss<<_date<<" "<<l->get_time().display_day(l->get_period().get_start_time().get_day())<<"\n";
								oss<<"\n";
								oss<<index<<"."<<" ";
								oss<<l->get_period().get_start_time().string_clock()<<" "<<"-"<<" "<<l->get_period().get_end_time().string_clock();
								oss<<" "<<l->note<<" "<<"at"<<" "<<l->venue<<endl;
								temp_date=_date;
                               /* oss<<l->get_period().string_time_period()<<"\n"<<index<<"."
                                <<" "<<l->note<<" "<<"at"<<" "<<l->venue<<endl;*/
							}
							
								
                        
						}
                        else
                        {
							full_date=l->get_time().get_date();
					_date=full_date/1000000;
					
					
					month=l->get_time().display_month((full_date % 1000000) / 10000);
					year=full_date%10000;
				
							if(temp_month==month){
							}
							else{
								oss<<"\n";
                             oss<<month<<" "<<year<<"\n";
							oss<<"--------"<<"\n";
							temp_month=month;
							temp_date=0;
						}

									if(temp_date==_date){
									
									oss<<index<<"."<<" ";
									oss<<l->get_time().string_clock();
								    oss<<" "<<l->note<<" "<<"at"<<" "<<l->venue<<endl;
									}
									else
									{
								oss<<"\n";		
								oss<<_date<<" "<<l->get_time().display_day(l->get_time().get_day())<<"\n";
								oss<<"\n";
								oss<<index<<"."<<" ";
								oss<<l->get_time().string_clock();
								oss<<" "<<l->note<<" "<<"at"<<" "<<l->venue<<endl;
								temp_date=_date;
                               /* oss<<l->get_period().string_time_period()<<"\n"<<index<<"."
                                <<" "<<l->note<<" "<<"at"<<" "<<l->venue<<endl;*/
							}
							
                                                       /* oss<<l->get_time().string_date() << " at " <<l->get_time().string_clock()<<"\n"<<index<<"."
                                <<" "<<l->note<<" "<<"at"<<" "<<l->venue<<endl;*/

                        }
                                                index++;
        }
        }
   return oss.str();
        
}

string ToDoMngr::view (int taskId){
            Time obj;
                        ToDoMngr todo;
						
		int full_date,_date,temp_date;
		temp_date=0;
		string month,temp_month;
		temp_month=" ";
		int year;
        std::ostringstream oss;
        int i=1;
              // list<Task> _activeTasklist;
               list<Task>::iterator _taskpointer;
                   _taskpointer=_activeTaskList.begin();

                                  for(int i=1;i<taskId;i++){
                                        if(_taskpointer!=_activeTaskList.end())
                                     _taskpointer++;
                                  }
                
                        if(_taskpointer->get_time()==obj){
                
                    full_date=_taskpointer->get_period().get_start_time().get_date();
					_date=full_date/1000000;
				     month=_taskpointer->get_time().display_month((full_date % 1000000) / 10000);
					year=full_date%10000;
				
								oss<<"\n";
                                oss<<month<<" "<<year<<"\n";
							    oss<<"--------"<<"\n";
						        oss<<_date<<" "<<_taskpointer->get_time().display_day(_taskpointer->get_period().get_start_time().get_day())<<"\n";
								oss<<"\n";
								oss<<taskId<<"."<<" ";
								oss<<_taskpointer->get_period().get_start_time().string_clock()<<" "<<"-"<<" "<<_taskpointer->get_period().get_end_time().string_clock();
								oss<<" "<<_taskpointer->note<<" "<<"at"<<" "<<_taskpointer->venue<<endl;
								
							}
							
                        
                        else
                        {
							full_date=_taskpointer->get_time().get_date();
					    _date=full_date/1000000;
					     month=_taskpointer->get_time().display_month((full_date % 1000000) / 10000);
					     year=full_date%10000;
				
							
								oss<<"\n";
                                oss<<month<<" "<<year<<"\n";
							    oss<<"--------"<<"\n";
						        oss<<"\n";		
								oss<<_date<<" "<<_taskpointer->get_time().display_day(_taskpointer->get_time().get_day())<<"\n";
								oss<<"\n";
								oss<<taskId<<"."<<" ";
								oss<<_taskpointer->get_time().string_clock();
								oss<<" "<<_taskpointer->note<<" "<<"at"<<" "<<_taskpointer->venue<<endl;
								temp_date=_date;
                               /* oss<<l->get_period().string_time_period()<<"\n"<<index<<"."
                                <<" "<<l->note<<" "<<"at"<<" "<<l->venue<<endl;*/
							}
                        

                         return oss.str();
                
        }   // return a string of the view of the specific task

string ToDoMngr::view(view_t viewType, Time time){
        
        string  DATE;
        std::ostringstream start_date,end_date,check;
    
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
                
                return view(_activeTaskList);
        }
                 
        else if(viewType=WEEKLY){
      Task _day;
         int day;
        for(int i=0;i<7;i++){
                day=_day.get_time().get_day();
                if(_day.get_time().display_day(day)==days[i]){
                         _index=i;
                        break;
                }
        }
        
    newStartDate=atoi(date.c_str()) -_index;
        newMonth=atoi(Month.c_str());
        
        if(atoi(Year.c_str())%400 ==0 || (atoi(Year.c_str())%100 != 0 && atoi(Year.c_str())%4 == 0)     ){ 
            if((Month=="02"&&newStartDate+5>29)||((Month=="01"||Month=="03"||Month=="05"||Month=="07"||Month=="08"||Month=="10"||Month=="12")
                        &&newStartDate+5>31)||((Month=="04"||Month=="06"||Month=="09"||Month=="11")&&newStartDate+5>30))
                        newMonth=atoi(Month.c_str())+1;
        }
                else {
                         if((Month=="02"&&newStartDate+5>28)||((Month=="01"||Month=="03"||Month=="05"||Month=="07"||Month=="08"||Month=="10"||Month=="12")
                        &&newStartDate+5>31)||((Month=="04"||Month=="06"||Month=="09"||Month=="11")&&newStartDate+5>30))
                        newMonth=atoi(Month.c_str())+1;
                }

                start_date<<std::setfill('0') << std::setw(2)<<newStartDate;
            start_date<<std::setfill('0') << std::setw(2)<<newMonth;
                start_date<<Year;
                Time start(atoi((start_date.str()).c_str()),000);
                        
                 newEndDate=newStartDate+5;
                 end_date<<std::setfill('0') << std::setw(2)<<newEndDate;
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
        std::ostringstream oss,_format;

        time_t t = time(0);   //get the current time
        struct tm * now = localtime( & t );
        oss<<now->tm_mday<<(now->tm_mon + 1)<<(now->tm_year + 1900) ;

        Date=oss.str();        //Stores the current computer date

        Time time(atoi(Date.c_str()),t);
		string newline="\n";
	   
		_format<<"\n"<<setw(47)<<setfill(' ')<<"AGENDA  FOR  TODAY"<<"\n";
	   _format<<"\n";
	   _format<<"Your Tasks for the day are as follows -->"<<"\n";
	   _format<<view(DAILY,time);
        return _format.str();//+view(DAILY,time);   //pass to view() to search for the tasks on that particular date and display it...year is not taken into account at present.
}



list<Task> ToDoMngr::add(Task task, bool forceAdd)                                                                      
{
 list<Task> _addList;
 if(forceAdd == true || task.timeTask == true)
 { 
  // forceAdd is true or is timetask 
  _addList.push_back(task);
  _dataStorage.save(_addList); 
  
  // and return empty list
  _addList.clear();
  return _addList;
 }
 else
 {
  bool clash = false;
  list<task> checkList;
  checkList = _dataStorage.load(task.get_period());
  
  if(checkList.empty() == true)
  {
   clash = false;
  }
  else
  {
   list<Task>::iterator li = checkList.begin();
   for(int i=1; i<= checkList.size(); i++)
   {
    TimePeriod taskPeriod = checkList.begin()->get_period();
    if(task.get_period() = taskPeriod)
    {
     clash = true;
     _clashList.push_back(li);
    }
    li++;
   }     
  }
  
  if(clash == true)
    {
   return _clashList;
  }
  else
  {
  _addList.push_back(task);
  _dataStorage.save(_addList); 
  
  // and return empty list
  _addList.clear();
  return _addList;
  }
 }
}





bool ToDoMngr::newTable(string name, TimePeriod period)
{
 bool clashName;
 vector<string> existedTableName;  

 //load existing table name
 existedTableName = _dataStorage.load_table_name();                                                                    
 
 //check for clashes of name with existing table names
 for(int i=1; i<=existedTableName.size(); i++)
 {
  if(existedTableName[i] == name)
  {
   clashName= true;
  }
 }
 
 if(clashName == true) // got clash return false
 {
  return false;
 }
 else // no clash return true and save to dataStorage with empty taskIdxList
 {
  list<int> taskIdxList;
  _dataStorage.save(name, period, taskIdxList);            
  return true;
 }
}

void ToDoMngr::erase(TimePeriod period)
{
 //get the id of tasks in that period
 list<Task> deleteList;
 deleteList = _dataStorage.load(period);
 list<int> deletedIdx;
 list<Task>::iterator di = deleteList.begin();
 
 for(int i=1; i<=deleteList.size(); i++)
 {
  deletedIdx.push_back(di->get_index());
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
 
 for(int i=1; i<deleteList.size();i++)
 {
  deletedIdx.push_back(di->get_index());
  di++;
 }
 
 // delete from dataStorage
 _dataStorage.erase(deletedIdx); 
}


// delete from dataStorage given the taskId
Task ToDoMngr::erase(int taskId)
{
 list<Task>::iterator di = _activeTaskList.begin();
 
 for(int i=1; i< taskId;i++)
 {
  di++;
 }     
 
 Task deleteTask = *di;
 _activeTaskList.erase(di);     
 
 int deleteTaskIdx;
 deleteTaskIdx = deleteTask.get_index(); 
 
 list<int> deleteIdx;
 deleteIdx.push_back(deleteTaskIdx);
 
 _dataStorage.erase(deleteIdx);
 
 return deleteTask;  
}



list<Task> ToDoMngr::add(string tableName, Task task, bool forceAdd)
{
 //create a list and push task into list and add to dataStorage when tableName == NULL
 if(tableName.size() == 0)
 {
  list<Task> blankList;
  blankList = add(task, forceAdd);
  return blankList;
 }
 
 else
 {
  list<int> IdxList;
  _dataStorage.save(tableName,task.get_period(), IdxList);
 }
}