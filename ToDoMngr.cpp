#include "ToDoMngr.h"
#include <list>
#include <queue>
#include <string>
#include <sstream>
#include <fstream>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <windows.h>
using namespace std;

string ToDoMngr::NOTHING_TO_VIEW = "";

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
		int pos=0;
        int endpos=0;
        int end_word=40;
		int pos_venue=0;
        int endpos_venue=0;
        int end_word_venue=40;
	   
	   
		
        std::ostringstream oss,index_convert;
        list<Task> taskl=tasklist;
		
		if(taskl.size()==1)
			index_convert<<10;
		else
		    index_convert<<taskl.size();
       
		list<Task>::iterator l;
        int index=1;
        if(tasklist.empty())
                oss<<"\n"<<" No Tasks To View During The Particular Period Or Time "<<"\n";
        else{

        for(l =taskl.begin (); l !=taskl.end (); l++)
                { 
					


					/*index_convert.clear();//clear any bits set
                    index_convert.str(std::string());
					index_convert<<index;*/
		            
					


                        if(l->get_time()==obj){
							full_date=l->get_period().get_start_time().get_date();
					_date=full_date/1000000;
					
					
					month=l->get_time().display_month((full_date % 1000000) / 10000);
					year=full_date%10000;
				
						

									if(temp_date==_date&&temp_month==month&&temp_year==year){
                                  
							   oss<<setw(index_convert.str().length())<<setfill(' ')<<right<<index<<"."<<" ";
							   oss<<"Start :"<<" "<<l->get_period().get_start_time().string_clock();
							   oss<<"    "<<"Note :"<<" ";
							   while( pos<l->note.length())      

							   {   
	                            if(pos<end_word){
	
	pos=l->note.find_first_of(' ',endpos+1);
	oss<<l->note.substr(endpos,pos-endpos)<<" ";
	endpos=pos+1;
	}
	else{
	oss<<"\n"<<"                               ";
	  end_word+=40;
	  }
							   }
							   oss<<"\n";
							   //oss<<l->note<<endl;
							   oss<<setw(31)<<setfill(' ')<<right<<"Venue :"<<" ";//<<l->venue<<"\n";
							   while( pos_venue<l->venue.length())      

							   {   
	                            if(pos_venue<end_word_venue){
	                                pos_venue=l->venue.find_first_of(' ',endpos_venue+1);
                                    oss<<l->venue.substr(endpos_venue,pos_venue-endpos_venue)<<" ";
									endpos_venue=pos_venue+1;
	                               }
	                          else{
	                              oss<<"\n"<<"                                ";
	                              end_word_venue+=40;
	                              }
                                }
							   
                               oss<<"\n";
							   oss<<setw(29)<<setfill(' ')<<right<<"End :"<<" "<<l->get_time().display_day(l->get_period().get_end_time().get_day());
							   oss<<" "<<((l->get_period().get_end_time().get_date())/1000000)<<" ";
							   oss<<l->get_period().get_end_time().display_month((((l->get_period().get_end_time().get_date())% 1000000) / 10000))<<" ";
							   oss<<((l->get_period().get_end_time().get_date())%10000)<<" ";
							   oss<<l->get_period().get_end_time().string_clock()<<"\n";
							   oss<<setw(31)<<setfill(' ')<<right<<"Alert :"<<" ";
							   l->alert.display_day(l->alert.get_day());
							   oss<<" "<<((l->alert.get_date())/1000000)<<" ";
							   oss<<l->alert.display_month((((l->alert.get_date())% 1000000) / 10000))<<" ";
							   oss<<((l->alert.get_date())%10000)<<" ";
							   oss<<l->alert.string_clock()<<"\n";

							   oss<<"\n";
									
								/*oss<<setw(2)<<setfill(' ')<<right<<index<<"."<<" "<<"|"<<" ";
								oss<<l->get_period().get_start_time().string_clock()<<" "<<"-"<<" "<<l->get_period().get_end_time().string_clock();
								oss<<"("<<((l->get_period().get_end_time().get_date())/1000000)<<" "<<l->get_period().get_end_time().display_month((((l->get_period().get_end_time().get_date())% 1000000) / 10000))<<")";
								//if(l->venue.length()>10)
                          /* {
	                           std::ostringstream line_divider;
	                           line_divider<<" "<<"|"<<" "<<l->venue.substr(0,11)<<"|"<<"\n";
	                            line_divider<<setw(35)<<setfill(' ')<<"|"<<" "<<setw(10)<<setfill(' ')<<left<<l->venue.substr(11,l->venue.length());
                                string	modified_venue=line_divider.str();
	                           oss<<modified_venue;
                            }

								/*else
								oss<<" "<<"|"<<" "<<setw(10)<<setfill(' ')<<left<<l->venue;
								
								oss<<" "<<"|"<<" "<<setw(28)<<setfill(' ')<<left<<l->note<<endl;*/
									}
									else
									{
								oss<<"\n";
								oss<<setw(38)<<setfill(' ')<<"-------------------"<<"\n";
								oss<<setw(20)<<setfill(' ')<<"|"<<" "<<l->get_time().display_day(l->get_period().get_start_time().get_day())<<" "<<_date<<" ";
								
								if(temp_month==month){

									    oss<<temp_month<<" ";
							}
							else{
								//oss<<"\n";
                             oss<<month<<" ";
							 temp_month=month;
							
							 temp_date=0;

							}
							
							 if(temp_year==year){
							oss<<temp_year<<" "<<"|"<<"\n";
							 //oss<<"\n";
							 }
							 else{
						    oss<<year<<" "<<"|"<<"\n";
							 //oss<<"\n";
							  temp_year=year;
							 
							 }
							 oss<<setw(38)<<setfill(' ')<<"-------------------"<<"\n";
							 oss<<"\n";
							//oss<<setw(45)<<setfill(' ')<<right<<"--------"<<"\n";
							/*temp_month=month;
							temp_year=year;
							temp_date=0;*/
						
                              
							   oss<<setw(index_convert.str().length())<<setfill(' ')<<right<<index<<"."<<" ";
								   oss<<"Start :"<<" "<<l->get_period().get_start_time().string_clock();
							   oss<<"    "<<"Note :"<<" ";

							   while( pos<l->note.length())      

							   {   
	                            if(pos<end_word)
	
								{
	
									pos=l->note.find_first_of(' ',endpos+1);
	
									oss<<l->note.substr(endpos,pos-endpos)<<" ";
	
									endpos=pos+1;
	
								}
	
	
								else
	
								{
      
									oss<<"\n"<<"                               ";
									
	  
									end_word+=40;//l->note.length();
	  
	}
}
							   //oss<<l->note<<endl;
							   oss<<"\n";
							   oss<<setw(31)<<setfill(' ')<<right<<"Venue :"<<" ";
							    while( pos_venue<l->venue.length())      

							   {   
	                            if(pos_venue<end_word_venue)
	
								{
	
									pos_venue=l->venue.find_first_of(' ',endpos_venue+1);
	
									oss<<l->venue.substr(endpos_venue,pos_venue-endpos_venue)<<" ";
	
									endpos_venue=pos_venue+1;
	
								}
	
	
								else
	
								{
      
									oss<<"\n"<<"                                ";
	  
									end_word_venue+=40;
	  
	}
}
							  



							   
							   
							   oss<<"\n";
							   //;<<l->venue<<"\n";
							   oss<<setw(29)<<setfill(' ')<<right<<"End :"<<" "<<l->get_time().display_day(l->get_period().get_end_time().get_day());
							   oss<<" "<<((l->get_period().get_end_time().get_date())/1000000)<<" ";
							   oss<<l->get_period().get_end_time().display_month((((l->get_period().get_end_time().get_date())% 1000000) / 10000))<<" ";
							   oss<<((l->get_period().get_end_time().get_date())%10000)<<" ";
							   oss<<l->get_period().get_end_time().string_clock()<<"\n";
							   oss<<setw(31)<<setfill(' ')<<right<<"Alert :"<<" ";
							   oss<<l->alert.display_day(l->alert.get_day());
							   oss<<" "<<((l->alert.get_date())/1000000)<<" ";
							   oss<<l->alert.display_month((((l->alert.get_date())% 1000000) / 10000))<<" ";
							   oss<<((l->alert.get_date())%10000)<<" ";
							   oss<<l->alert.string_clock()<<"\n";
							   oss<<"\n";
							   	temp_date=_date;
								
							}
							
								
                        
						}
                        else
                        {
							full_date=l->get_time().get_date();
					_date=full_date/1000000;
					
					
					month=l->get_time().display_month((full_date % 1000000) / 10000);
					year=full_date%10000;
				
							if(temp_date==_date&&temp_month==month&&temp_year==year){
                                  
								oss<<setw(index_convert.str().length())<<setfill(' ')<<index<<"."<<" "<<"Start :"<<" "<<l->get_time().string_clock()<<" ";
							   oss<<"   "<<"Note :"<<" ";
							    while( pos<l->note.length())      

							   {   
	                            if(pos<end_word){
	
	pos=l->note.find_first_of(' ',endpos+1);
	oss<<l->note.substr(endpos,pos-endpos)<<" ";
	endpos=pos+1;
	}
	else{
	oss<<"\n"<<"                               ";
	  end_word+=40;
	  }

								}

							   
							   oss<<"\n";
							 
							   oss<<setw(31)<<setfill(' ')<<"Venue :"<<" ";
							    while( pos_venue<l->venue.length())      

							   {   
	                            if(pos_venue<end_word_venue){
	                                pos_venue=l->venue.find_first_of(' ',endpos_venue+1);
                                    oss<<l->venue.substr(endpos_venue,pos_venue-endpos_venue)<<" ";
									endpos_venue=pos_venue+1;
	                               }
	                          else{
	                              oss<<"\n"<<"                                ";
	                              end_word_venue+=40;
	                              }
                                }
								oss<<"\n";
							   //<<l->venue<<"\n";
							 
							   oss<<setw(31)<<setfill(' ')<<"Alert :"<<" ";
							   oss<<l->alert.display_day(l->alert.get_day());
							   oss<<" "<<((l->alert.get_date())/1000000)<<" ";
							   oss<<l->alert.display_month((((l->alert.get_date())% 1000000) / 10000))<<" ";
							   oss<<((l->alert.get_date())%10000)<<"\n";
							   oss<<"\n";
									
								/*oss<<setw(2)<<setfill(' ')<<right<<index<<"."<<" "<<"|"<<" ";
								oss<<l->get_period().get_start_time().string_clock()<<" "<<"-"<<" "<<l->get_period().get_end_time().string_clock();
								oss<<"("<<((l->get_period().get_end_time().get_date())/1000000)<<" "<<l->get_period().get_end_time().display_month((((l->get_period().get_end_time().get_date())% 1000000) / 10000))<<")";
								//if(l->venue.length()>10)
                          /* {
	                           std::ostringstream line_divider;
	                           line_divider<<" "<<"|"<<" "<<l->venue.substr(0,11)<<"|"<<"\n";
	                            line_divider<<setw(35)<<setfill(' ')<<"|"<<" "<<setw(10)<<setfill(' ')<<left<<l->venue.substr(11,l->venue.length());
                                string	modified_venue=line_divider.str();
	                           oss<<modified_venue;
                            }

								/*else
								oss<<" "<<"|"<<" "<<setw(10)<<setfill(' ')<<left<<l->venue;
								
								oss<<" "<<"|"<<" "<<setw(28)<<setfill(' ')<<left<<l->note<<endl;*/
									}
									else
									{
								oss<<"\n";
								oss<<setw(38)<<setfill(' ')<<"-------------------"<<"\n";
								oss<<setw(20)<<setfill(' ')<<"|"<<" "<<l->get_time().display_day(l->get_period().get_start_time().get_day())<<" "<<_date<<" ";
								if(temp_month==month){

									    oss<<temp_month<<" ";
							}
							else{
								//oss<<"\n";
                             oss<<month<<" ";
							 temp_month=month;
							
							 temp_date=0;

							}
							
							 if(temp_year==year){
							oss<<temp_year<<" "<<"|"<<"\n";
							
							 }
							 else{
						    oss<<year<<" "<<"|"<<"\n";
							
							  temp_year=year;
							 
							 }
							  oss<<setw(38)<<setfill(' ')<<"-------------------"<<"\n";
							 oss<<"\n";
							//oss<<setw(45)<<setfill(' ')<<right<<"--------"<<"\n";
							/*temp_month=month;
							temp_year=year;
							temp_date=0;*/
						
                              
							 oss<<setw(index_convert.str().length())<<setfill(' ')<<index<<"."<<" "<<"Start :"<<" "<<l->get_time().string_clock()<<" ";
							   oss<<"   "<<"Note :"<<" ";

							   while( pos<l->note.length())      

							   {   
	                            if(pos<end_word){
	
	pos=l->note.find_first_of(' ',endpos+1);
	oss<<l->note.substr(endpos,pos-endpos)<<" ";
	endpos=pos+1;
	}
	else{
	oss<<"\n"<<"                               ";
	  end_word+=40;
	  }

								}
							   oss<<"\n";
							   
							   oss<<setw(31)<<setfill(' ')<<"Venue :"<<" ";//<<l->venue<<"\n";

							    while( pos_venue<l->venue.length())      

							   {   
	                            if(pos_venue<end_word_venue){
	                                pos_venue=l->venue.find_first_of(' ',endpos_venue+1);
                                    oss<<l->venue.substr(endpos_venue,pos_venue-endpos_venue)<<" ";
									endpos_venue=pos_venue+1;
	                               }
	                          else{
	                              oss<<"\n"<<"                                ";
	                              end_word_venue+=40;
	                              }
                                }
								oss<<"\n";
							   
							   oss<<setw(31)<<setfill(' ')<<"Alert :"<<" ";
							   oss<<l->alert.display_day(l->alert.get_day());
							   oss<<" "<<((l->alert.get_date())/1000000)<<" ";
							   oss<<l->alert.display_month((((l->alert.get_date())% 1000000) / 10000))<<" ";
							   oss<<((l->alert.get_date())%10000)<<"\n";
							   oss<<"\n";
							   	temp_date=_date;
                                                       /* oss<<l->get_time().string_date() << " at " <<l->get_time().string_clock()<<"\n"<<index<<"."
                                <<" "<<l->note<<" "<<"at"<<" "<<l->venue<<endl;*/

                        }
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

//cout << ToDoMngr::view (_activeTaskList) << endl;
 
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
	   _format<<setw(42)<<setfill(' ')<<"--------------------------";
		_format<<"\n"<<setw(20)<<setfill(' ')<<"| "<<"AGENDA  FOR  TODAY"<<" |"<<"\n";
		_format<<setw(42)<<setfill(' ')<<"--------------------------"<<"\n";
		
	   _format<<"\n";
	   //_format<<"Your Tasks for the day are as follows -->"<<"\n";
	   _format<<view(DAILY,time);
	   
        return _format.str();//+view(DAILY,time);   //pass to view() to search for the tasks on that particular date and display it...year is not taken into account at present.
}

/*pos = str.find_first_of (" ", 0)
	str[pos]*/
string ToDoMngr::view (string tableName){

	_activeTaskList=_dataStorage.load(tableName);

	return view(_activeTaskList);
}

string ToDoMngr::viewTableNames () {
	return "viewTab";
}

string ToDoMngr::alert () {
	return "alert";
}

bool ToDoMngr::ifExistedTable (string tableName) {
	return false;
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
  list<Task> checkList;
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
    if(task.get_period() == taskPeriod)
    {
     clash = true;
     _clashList.push_back(*li);
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
  list<Task> taskList;
  _dataStorage.save(name, period, taskList);            
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
  list<Task> IdxList;
  _dataStorage.save(tableName,task.get_period(), IdxList);
 }
}


 Task ToDoMngr::edit(int taskId, TaskElement* taskElem, Task* task, bool forceEdit)
{
 list<Task> _blankList;
 // get the task that will be edited
 list<Task>iterator taskIterator = _activeTaskList.begin();
 for(int i=1;i==taskId; i++)
 {
  taskIterator++;
 }
 
 // check taskElem for what to edit
 if(taskElem->_time == true)
 {
  //edit time
  taskIterator->modifiy_time(task->get_time());
  return _blankList;
 }
 else if(taskElem->_period == true)   
 {
  //edit period when forcEdit == true
  if(forceEdit == true)
  {
   taskIterator->modify_period(task->get_period());
   return _blankList;
  }
  else
  {
   //check for clash
   bool clash false;
   list<Task> checkList;
   checkList = _dataStorage.load(taskIterator->get_period());
   if(checkList.empty() == true)
   {
    clash = false;
   }
   else
   {
    list<Task>::iterator li = checkList.begin();
    for(int i=1; i<= checkList.size();i++)
    {
     TimePeriod taskPeriod = checkList.begin()->get_period();
     if(taskIterator->get_period()== taskPeriod)
     {
      clash = true;
      _clashList.push_back(*li);
     }
    }
   }
   if(clash == true)
   {
    return _clashList;
   }  
   else
   {
    taskIterator->modify_period(task->get_period());
    return _blankList;
   }
  }
 }
 else if(taskElem->_note == true)
 {
  //edit note
  taskIterator->note = task->note;
  return _blankList;
 }
 else if(taskElem->_venue == true)
 {
  //edit venue
  taskIterator->venue = task->venue;
  return _blankList;
 }
 else if(taskElem->_alert== true)
 {
  taskIterator->alert = task->alert;
  return _blankList;
 }
 else if(taskElem->_repeat== true)    
 {
  taskIterator->repeat = task->repeat;
  return _blankList;
 }
}

void ToDoMngr::undo () {
}

void ToDoMngr::redo () {
}

//Rith
string ToDoMngr::search (search_t type, string phrase) {
	return ToDoMngr::view (_dataStorage.search (phrase, type));
}

void ToDoMngr::exit () {
	_dataStorage.exit ();
	_activeTaskList.clear ();
	_clashList.clear ();
}

void ToDoMngr::clear () {
	_dataStorage.clear ();
}

ToDoMngr::ToDoMngr () {
	_dataStorage.updateStorageName ("");
}

ToDoMngr::ToDoMngr (string storageName) {
	_dataStorage.updateStorageName (storageName);
}

void ToDoMngr::updateStorageName (string storageName) {
	_dataStorage.exit ();
	_activeTaskList.clear ();
	_clashList.clear ();
	_dataStorage.updateStorageName (storageName);
}
