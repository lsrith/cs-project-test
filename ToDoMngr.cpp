#include "ToDoMngr.h"
#include <iostream>
#include <list>
#include <queue>
#include <string>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <windows.h>
using namespace std;

string ToDoMngr::NOTHING_TO_VIEW = "\nThere are no tasks to view during the particular period/time!\n";

list<Task> ToDoMngr::get_active_list () {
	return _activeTaskList;
}

string ToDoMngr::view(Task taskId){
	Time obj;
	int  pos=0;
	int endpos=0;
	int end_word=65;
	int pos_venue=0;
	int endpos_venue=0;
	int end_word_venue=65;

	Time _t;       

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
		month=taskId.get_period().get_start_time().display_month((full_date % 1000000) / 10000);
		year=full_date%10000;

		oss<<"\n";
		if(taskId.get_period().get_start_time().get_date()!=Time::DFLT_DATE){
			oss<<setw(24)<<setfill(' ')<<month<<" "<<year<<"  "<<_date<<" "<<taskId.get_period().get_start_time().display_day(taskId.get_period().get_start_time().get_day())<<"\n";

			oss<<"\n";
		}
		else{
			oss<<"\n\n";

		}

		if(taskId.get_period().get_start_time().get_clock()!=Time::DFLT_CLOCK)
			oss<<taskId.get_period().get_start_time().string_clock()<<" "<<"-"<<" "<<taskId.get_period().get_end_time().string_clock()<<"\n";





		oss<<"Note :"<<" ";  
		while( pos<taskId.note.length())      

		{   
			if(pos<end_word){

			   pos=taskId.note.find_first_of(' ',endpos+1);
			    oss<<taskId.note.substr(endpos,pos-endpos)<<" ";
                endpos=pos+1;

			}


			else

			{

				oss<<"\n";


				end_word+=65;//l->note.length();

			}
		}
		pos=0;
		endpos=0;
		end_word=40;


		oss<<"\n";
		oss<<"Venue :"<<" ";//<<taskId.venue;

		while( pos_venue<taskId.venue.length())      

		{   
			if(pos_venue<end_word_venue){
				pos_venue=taskId.venue.find_first_of(' ',endpos_venue+1);
				oss<<taskId.venue.substr(endpos_venue,pos_venue-endpos_venue)<<" ";
				endpos_venue=pos_venue+1;
			}
			else{
				oss<<"\n"<<"                                ";
				end_word_venue+=65;
			}
		}
		pos_venue=0;
		endpos_venue=0;
		end_word_venue=40;
		oss<<"\n\n";

	}


	else
	{
		full_date=taskId.get_time().get_date();
		_date=full_date/1000000;
		month=taskId.get_time().display_month((full_date % 1000000) / 10000);
		year=full_date%10000;


		oss<<"\n";
		if(taskId.get_time().get_date()!=Time::DFLT_DATE){
			oss<<setw(24)<<setfill(' ')<<month<<" "<<year<<"  "<<_date<<" "<<taskId.get_time().display_day(taskId.get_time().get_day())<<"\n";

			oss<<"\n";
		}
		else
		{
			oss<<"\n";
		}

		if(taskId.get_time().get_clock()!=Time::DFLT_CLOCK)
			oss<<taskId.get_time().string_clock();




		oss<<"Note :"<<" ";  
		while( pos<taskId.note.length())      

		{   
			if(pos<end_word)

			{

				pos=taskId.note.find_first_of(' ',endpos+1);

				oss<<taskId.note.substr(endpos,pos-endpos)<<" ";

				endpos=pos+1;

			}


			else

			{

				oss<<"\n";


				end_word+=65;//l->note.length();

			}
		}
		pos=0;
		endpos=0;
		end_word=40;

		oss<<"\n";

		oss<<"Venue :"<<" ";//<<taskId.venue;

		while( pos_venue<taskId.venue.length())      

		{   
			if(pos_venue<end_word_venue){
				pos_venue=taskId.venue.find_first_of(' ',endpos_venue+1);
				oss<<taskId.venue.substr(endpos_venue,pos_venue-endpos_venue)<<" ";
				endpos_venue=pos_venue+1;
			}
			else{
				oss<<"\n"<<"                                ";
				end_word_venue+=65;
			}
		}
		pos_venue=0;
		endpos_venue=0;
		end_word_venue=40;
		oss<<"\n\n";		
		/* oss<<l->get_period().string_time_period()<<"\n"<<index<<"."
		<<" "<<l->note<<" "<<"at"<<" "<<l->venue<<endl;*/
	}

	//cout<<oss.str();
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
	Time _tm;



	std::ostringstream oss,index_convert;
	list<Task> taskl=tasklist;
	taskl.sort(Task::compareByStartTime);

	if(taskl.size()>0&&taskl.size()<=9)
		index_convert<<10;
	else
		index_convert<<taskl.size();

	list<Task>::iterator l;
	int index=1;

	

	if(tasklist.empty())
		oss<<"\n"<<NOTHING_TO_VIEW<<"\n";
	else{

		for(l =taskl.begin (); l !=taskl.end (); l++)
		{ 



			/*index_convert.clear();//clear any bits set
			index_convert.str(std::string());
			index_convert<<index;*/




			if(l->get_time()==obj){
				full_date=l->get_period().get_start_time().get_date();
				_date=full_date/1000000;


				month=l->get_period().get_start_time().display_month((full_date % 1000000) / 10000);
				year=full_date%10000;



				if(temp_date==_date&&temp_month==month&&temp_year==year){

					oss<<setw(index_convert.str().length())<<setfill(' ')<<right<<index<<"."<<" ";
					oss<<"Start :"<<" ";

					if(l->get_period().get_start_time().get_clock()!=-1)
						oss<<l->get_period().get_start_time().string_clock();
					else
						oss<<"        ";

					oss<<setw(10-(index_convert.str().length()-2))<<setfill(' ')<<right<<"Note :"<<" ";
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
					pos=0;
					endpos=0;
					end_word=40;
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
					pos_venue=0;
					endpos_venue=0;
					end_word_venue=40;

					oss<<"\n";
					oss<<setw(29)<<setfill(' ')<<right<<"End :"<<" ";

					if(l->get_period().get_end_time().get_date()!=Time::DFLT_DATE){
						oss<<l->get_period().get_end_time().display_day(l->get_period().get_end_time().get_day());
					oss<<" "<<((l->get_period().get_end_time().get_date())/1000000)<<" ";
					oss<<l->get_period().get_end_time().display_month((((l->get_period().get_end_time().get_date())% 1000000) / 10000))<<" ";
					oss<<((l->get_period().get_end_time().get_date())%10000)<<" ";
					}

					if(l->get_period().get_end_time().get_clock()!=Time::DFLT_CLOCK)
						oss<<l->get_period().get_end_time().string_clock()<<"\n";

					else
						oss<<"Time Unspecified"<<"\n";


					oss<<setw(31)<<setfill(' ')<<right<<"Alert :"<<" ";
					if(l->alert!=_tm) {
						oss<<l->alert.display_day(l->alert.get_day());
						oss<<" "<<((l->alert.get_date())/1000000)<<" ";
						oss<<l->alert.display_month((((l->alert.get_date())% 1000000) / 10000))<<" ";
						oss<<((l->alert.get_date())%10000)<<" ";
						oss<<l->alert.string_clock()<<"\n";
					}
					else
						oss<<"Unspecified"<<"\n";

					oss<<"\n";

					/*oss<<setw(2)<<setfill(' ')<<right<<index<<"."<<" "<<"|"<<" ";
					oss<<l->get_period().get_start_time().string_clock()<<" "<<"-"<<" "<<l->get_period().get_end_time().string_clock();
					oss<<"("<<((l->get_period().get_end_time().get_date())/1000000)<<" "<<l->get_period().get_end_time().display_month((((l->get_period().get_end_time().get_date())% 1000000) / 10000))<<")";
					//if(l->venue.length()>10)
					/* {
					std::ostringstream line_divider;
					line_divider<<" "<<"|"<<" "<<l->venue.substr(0,11)<<"|"<<"\n";
					line_divider<<setw(35)<<setfill(' ')<<"|"<<" "<<setw(10)<<setfill(' ')<<left<<l->venue.substr(11,l->venue.length());
					string  modified_venue=line_divider.str();
					oss<<modified_venue;
					}

					/*else
					oss<<" "<<"|"<<" "<<setw(10)<<setfill(' ')<<left<<l->venue;

					oss<<" "<<"|"<<" "<<setw(28)<<setfill(' ')<<left<<l->note<<endl;*/
				}
				else
				{ //if(l->get_period().get_start_time().get_date()!=Time::DFLT_DATE)
					oss<<"\n";
					oss<<setw(38)<<setfill(' ')<<"-------------------"<<"\n";
					oss<<setw(20)<<setfill(' ')<<"|"<<" "<<l->get_period().get_start_time().display_day(l->get_period().get_start_time().get_day())<<" "<<setw(2)<<setfill(' ')<<_date<<" ";

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
					oss<<"Start :"<<" ";

					if(l->get_period().get_start_time().get_clock()!=-1)
						oss<<l->get_period().get_start_time().string_clock();
					else
						oss<<"        ";

					oss<<setw(10-(index_convert.str().length()-2))<<setfill(' ')<<right/*"    "*/<<"Note :"<<" ";

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
					pos=0;
					endpos=0;
					end_word=40;

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


					pos_venue=0;
					endpos_venue=0;
					end_word_venue=40;



					oss<<"\n";
					//;<<l->venue<<"\n";
					oss<<setw(29)<<setfill(' ')<<right<<"End :"<<" ";

					if(l->get_period().get_end_time().get_date()!=Time::DFLT_DATE){
						oss<<l->get_period().get_end_time().display_day(l->get_period().get_end_time().get_day());
					oss<<" "<<((l->get_period().get_end_time().get_date())/1000000)<<" ";
					oss<<l->get_period().get_end_time().display_month((((l->get_period().get_end_time().get_date())% 1000000) / 10000))<<" ";
					oss<<((l->get_period().get_end_time().get_date())%10000)<<" ";
					}
					
					if(l->get_period().get_end_time().get_clock()!=-1)
						oss<<l->get_period().get_end_time().string_clock()<<"\n";

					else
						oss<<"Time Unspecified"<<"\n";


					oss<<setw(31)<<setfill(' ')<<right<<"Alert :"<<" ";
					if(l->alert!=_tm) {
						oss<<l->alert.display_day(l->alert.get_day());
						oss<<" "<<((l->alert.get_date())/1000000)<<" ";
						oss<<l->alert.display_month((((l->alert.get_date())% 1000000) / 10000))<<" ";
						oss<<((l->alert.get_date())%10000)<<" ";
						oss<<l->alert.string_clock()<<"\n";
						oss<<"\n";
					}
					else{
						oss<<"Unspecified"<<"\n";
						oss<<"\n";
					}

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

					oss<<setw(index_convert.str().length())<<setfill(' ')<<index<<"."<<" ";
					oss<<"Start :"<<" ";

					if(l->get_time().get_clock()!=Time::DFLT_CLOCK)
						oss<<l->get_time().string_clock();
					else
						oss<<"        ";

					oss<<setw(10-(index_convert.str().length()-2))<<setfill(' ')<<right<<"Note :"<<" ";
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
					pos=0;
					endpos=0;
					end_word=40;


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
					pos_venue=0;
					endpos_venue=0;
					end_word_venue=40;
					oss<<"\n";
					//<<l->venue<<"\n";

					oss<<setw(31)<<setfill(' ')<<"Alert :"<<" ";
					if(l->alert!=_tm) {
						oss<<l->alert.display_day(l->alert.get_day());
						oss<<" "<<((l->alert.get_date())/1000000)<<" ";
						oss<<l->alert.display_month((((l->alert.get_date())% 1000000) / 10000))<<" ";
						oss<<((l->alert.get_date())%10000)<<"\n";
						oss<<"\n";
					}
					else{
						oss<<"Unspecified"<<"\n";
						oss<<"\n";
					}
					/*oss<<setw(2)<<setfill(' ')<<right<<index<<"."<<" "<<"|"<<" ";
					oss<<l->get_period().get_start_time().string_clock()<<" "<<"-"<<" "<<l->get_period().get_end_time().string_clock();
					oss<<"("<<((l->get_period().get_end_time().get_date())/1000000)<<" "<<l->get_period().get_end_time().display_month((((l->get_period().get_end_time().get_date())% 1000000) / 10000))<<")";
					//if(l->venue.length()>10)
					/* {
					std::ostringstream line_divider;
					line_divider<<" "<<"|"<<" "<<l->venue.substr(0,11)<<"|"<<"\n";
					line_divider<<setw(35)<<setfill(' ')<<"|"<<" "<<setw(10)<<setfill(' ')<<left<<l->venue.substr(11,l->venue.length());
					string  modified_venue=line_divider.str();
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
					oss<<setw(20)<<setfill(' ')<<"|"<<" "<<l->get_time().display_day(l->get_time().get_day())<<" "<<setw(2)<<setfill(' ')<<_date<<" ";
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


					oss<<setw(index_convert.str().length())<<setfill(' ')<<index<<"."<<" ";
					oss<<"Start :"<<" ";

					if(l->get_time().get_clock()!=Time::DFLT_CLOCK)
						oss<<l->get_time().string_clock();
					else
						oss<<"        ";

					oss<<setw(10-(index_convert.str().length()-2))<<setfill(' ')<<right<<"Note :"<<" ";

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


					pos=0;
					endpos=0;
					end_word=40;

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
					pos_venue=0;
					endpos_venue=0;
					end_word_venue=40;
					oss<<"\n";

					oss<<setw(31)<<setfill(' ')<<"Alert :"<<" ";

					if(l->alert!=_tm) {
						oss<<l->alert.display_day(l->alert.get_day());
						oss<<" "<<((l->alert.get_date())/1000000)<<" ";
						oss<<l->alert.display_month((((l->alert.get_date())% 1000000) / 10000))<<" ";
						oss<<((l->alert.get_date())%10000)<<"\n";
						oss<<"\n";
					}
					else{
						oss<<"Unspecified"<<"\n";
						oss<<"\n";
					}

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
	Time _time;
	int pos=0;
	int endpos=0;
	int end_word=65;
	int pos_venue=0;
	int endpos_venue=0;
	int end_word_venue=65;
	std::ostringstream oss;
	int i=1;
	// list<Task> _activeTasklist;
	list<Task>::iterator _taskpointer;
	_taskpointer=_activeTaskList.begin();

	if(taskId>_activeTaskList.size())
		oss<<ToDoMngr::NOTHING_TO_VIEW;
	else{
		for(int i=1;i<taskId;i++){
			if(_taskpointer!=_activeTaskList.end())
				_taskpointer++;
		}


		if(_taskpointer->get_time()==obj){

			full_date=_taskpointer->get_period().get_start_time().get_date();
			_date=full_date/1000000;
			month=_taskpointer->get_period().get_start_time().display_month((full_date % 1000000) / 10000);
			year=full_date%10000;

			oss<<"\n";
			if(_taskpointer->get_period().get_start_time().get_date()!=Time::DFLT_DATE){
				oss<<setw(28)<<setfill(' ')<<month<<" "<<year<<"\n";
				oss<<setw(33)<<setfill(' ')<<"--------"<<"\n";
				oss<<setw(28)<<setfill(' ')<<_date<<" "<<_taskpointer->get_period().get_start_time().display_day(_taskpointer->get_period().get_start_time().get_day())<<"\n";
			}
			oss<<"\n";

			oss<<taskId<<"."<<" ";

			if(_taskpointer->get_period().get_start_time().get_clock()!=Time::DFLT_CLOCK)
				oss<<"Start time : "<<_taskpointer->get_period().get_start_time().string_clock()<<" "<<"-"<<" ";

			if(_taskpointer->get_period().get_end_time().get_clock()!=Time::DFLT_CLOCK)
				oss<<"End time : "<<_taskpointer->get_period().get_end_time().string_clock();
			


			if(_taskpointer->get_period().get_end_time().get_date()!=Time::DFLT_DATE){
				oss<<"\n   End date :"<<" "<<"("<<(_taskpointer->get_period().get_end_time().get_date()/1000000)<<" "<<_taskpointer->get_period().get_end_time().display_month((_taskpointer->get_period().get_end_time().get_date()%1000000)/10000);
				oss<<" "<<_taskpointer->get_period().get_end_time().get_date()%10000<<")";
			}
			oss<<"\n   Note : ";
			while( pos<_taskpointer->note.length())      

			{   
				if(pos<end_word){

					pos=_taskpointer->note.find_first_of(' ',endpos+1);
					oss<<_taskpointer->note.substr(endpos,pos-endpos)<<" ";
					endpos=pos+1;
				}
				else{
					oss<<"\n"<<"   ";
					end_word+=65;
				}

			}
			pos=0;
			endpos=0;
			end_word=40;


			oss<<"\n";

			oss<<"   Venue :"<<" ";
			while( pos_venue<_taskpointer->venue.length())      

			{   
				if(pos_venue<end_word_venue){
					pos_venue=_taskpointer->venue.find_first_of(' ',endpos_venue+1);
					oss<<_taskpointer->venue.substr(endpos_venue,pos_venue-endpos_venue)<<" ";
					endpos_venue=pos_venue+1;
				}
				else{
					oss<<"\n"<<"   ";
					end_word_venue+=65;
				}
			}
			pos_venue=0;
			endpos_venue=0;
			end_word_venue=40;
			oss<<"\n";
			//oss<<" "<<_taskpointer->note<<" "<<"at"<<" "<<_taskpointer->venue<<"\n\n";

		}


		else
		{
			full_date=_taskpointer->get_time().get_date();
			_date=full_date/1000000;
			month=_taskpointer->get_time().display_month((full_date % 1000000) / 10000);
			year=full_date%10000;


			oss<<"\n";
			if(_taskpointer->get_time().get_date()!=Time::DFLT_DATE){

				oss<<month<<" "<<year<<"\n";
				oss<<"--------"<<"\n";
				oss<<"\n";              
				oss<<_date<<" "<<_taskpointer->get_time().display_day(_taskpointer->get_time().get_day())<<"\n";
			}
			oss<<"\n";
			oss<<taskId<<"."<<" ";

			if(_taskpointer->get_time().get_clock()!=Time::DFLT_CLOCK)
				oss<<_taskpointer->get_time().string_clock();

			oss<<"\n   Note : ";
			while( pos<_taskpointer->note.length())      

			{   
				if(pos<end_word){

					pos=_taskpointer->note.find_first_of(' ',endpos+1);
					oss<<_taskpointer->note.substr(endpos,pos-endpos)<<" ";
					endpos=pos+1;
				}
				else{
					oss<<"\n"<<"   ";
					end_word+=65;
				}

			}
			pos=0;
			endpos=0;
			end_word=40;


			oss<<"\n";

			oss<<"   Venue :"<<" ";
			while( pos_venue<_taskpointer->venue.length())      

			{   
				if(pos_venue<end_word_venue){
					pos_venue=_taskpointer->venue.find_first_of(' ',endpos_venue+1);
					oss<<_taskpointer->venue.substr(endpos_venue,pos_venue-endpos_venue)<<" ";
					endpos_venue=pos_venue+1;
				}
				else{
					oss<<"\n"<<"   ";
					end_word_venue+=65;
				}
			}
			pos_venue=0;
			endpos_venue=0;
			end_word_venue=40;
			oss<<"\n";										//oss<<" "<<_taskpointer->note<<" "<<"at"<<" "<<_taskpointer->venue<<"\n\n";
			temp_date=_date;
			/* oss<<l->get_period().string_time_period()<<"\n"<<index<<"."
			<<" "<<l->note<<" "<<"at"<<" "<<l->venue<<endl;*/
		}

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
		_activeTaskList.sort(Task::compareByStartTime);
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
		_activeTaskList.sort(Task::compareByStartTime);
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
		_activeTaskList.sort(Task::compareByStartTime);
		return view(_activeTaskList);

	}



} // return a string of the view by day, by week, or by month

string ToDoMngr::view (TimePeriod period){

	if(Table_Mode)
       return view (tableName);

	else{
	_activeTaskList= _dataStorage.load(period);

	//cout << ToDoMngr::view (_activeTaskList) << endl;

	return view(_activeTaskList);
	}
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

string ToDoMngr::alert () {
	

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
}

bool ToDoMngr::ifExistedTable (string tableName) {
	return false;
}


bool ToDoMngr::clashed(Task task){
	list<Task> checkList;
	checkList = _dataStorage.load(task.get_period());

	if(checkList.empty() == true){
		return false;;
	}
	else{
		list<Task>::iterator li = checkList.begin();
		bool clash = false; 

		for(int i=1; i<= checkList.size(); i++){
			TimePeriod taskPeriod = checkList.begin()->get_period();
		
			if(task.get_period() == taskPeriod){
				_clashList.push_back(*li);
				clash = true;
				
				//check if there is duplication of the clashed task
				list<Task>::iterator li2 = checkList.begin();
				for(int i=1; i<checkList.size(); i++)
				{
					if(task.compareByVenue(task,*li2) && task.compareByAlert(task, *li2) && task.compareByEndTime(task, *li2) && task.compareByStartTime(task, *li2))
					{
					 _clashList.pop_back();
					}
				}
				}
				return clash;
			}

			li++;
		}
}


list<Task> ToDoMngr::add(Task task, bool forceAdd)                                                                      
{

	cout<<tableName;
	if(Table_Mode)
     return add(tableName,task,forceAdd);

	else{
	
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
}

bool ToDoMngr::newTable(string name, TimePeriod period){  
	
	//check if the period is valid

	if(period.get_start_time().get_date() == Time::INF_DATE || period.get_start_time().get_date() == Time::DFLT_DATE ||
		period.get_end_time().get_date() == Time::DFLT_DATE || period.get_end_time().get_date() == Time::DFLT_DATE) {
	 cout<<"Error in Timetable Period."<<endl;
	}

	else{
		bool clashName=false;
		vector<string> existedTableName;  

		//load existing table name
		existedTableName = _dataStorage.load_table_name();                                                                    

		//check for clashes of name with existing table names
		for(int i=0; i<existedTableName.size(); i++) {
			if(existedTableName[i] == name)	{
				clashName= true;
			}
		}

		if(clashName == true)	{ 
			// got clash return false 
			return false;
		}
		else	{ 
			// no clash return true and save to dataStorage with empty taskIdxList
			list<Task> taskList;
			_dataStorage.save(name, period, taskList);   
			Table_Mode=true;
			tableName=name;
			return true;
		}
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
      di++
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
  {
	list<Task>::iterator di = _activeTaskList.begin();

	for(int i=1; i< taskId;i++)
	{
		di++;
	}     
	if(di!=_activeTaskList.end()){
	Task deleteTask = *di;
	_activeTaskList.erase(di);     

	int deleteTaskIdx;
	deleteTaskIdx = deleteTask.get_index(); 

	list<int> deleteIdx;
	deleteIdx.push_back(deleteTaskIdx);

	_dataStorage.erase(deleteIdx);

	return deleteTask;  
	}
	else
	{   
		Task task;
		return task;

	}
}



list<Task> ToDoMngr::add(string tableName, Task task, bool forceAdd){
	if(tableName.size() == 0){
		list<Task> blankList;
		blankList = add(task, forceAdd);
		return blankList;
	}

	else{   	
	// get timetable period
		list<DataStorage::Table> tableList;
		list<DataStorage::Table>::iterator tableIter = tableList.begin(); 		
		TimePeriod activePeriod;

		for(int i=0;i<tableList.size();i++){
			if(tableIter->name == tableName){
				activePeriod = tableIter->period;
			}
		}
		
	// check if it is before, during or after the timetable period 
		enum checkType {before, during, after};
		checkType type;

		if(task.get_period().get_start_time().operator<(activePeriod.get_start_time())){
			type = before;
		}
		else if(task.get_period().get_start_time().operator>(activePeriod.get_end_time())){
			type = after;
		}
		else {
			type = during;
		}

	// check if task is >7 days, return a error message when task is more than 7 dats

		int duration;

		duration = task.get_period().get_end_time() - task.get_period().get_start_time();

		if(duration > 10080){
			cout<<"This is a weekly Timetable. We can't have tasks  that last more than 7 days. "<<endl;
		}

	// add the same task to previous week or next week to timetable and check clashes with the calender
	// repeat for the entire timetable period

		bool got_clash;
		list<Task> taskList;

		// if task is within the timetable period
		if(type == during){
			// duration of timetable period in term of weeks from the task to the start and end of timetable
			int duration_endtable = (activePeriod.get_end_time() - task.get_period().get_start_time()) / 10080;
			int duration_starttable = (task.get_period().get_start_time() - activePeriod.get_start_time()) / 100800;	
			
			
			if(tillEnd(duration_endtable, taskList, task, forceAdd, activePeriod) == true){
				got_clash = true;
			}
			if(tillStart(duration_starttable, taskList, task, forceAdd, activePeriod) == true){
				got_clash = true;
			}
		}

		// if task is before the timetable period
		// push back the task by a week in each iteration until the task is after the starting time of the timetable
		if(type == before){
			while((task.get_period().get_start_time() - activePeriod.get_start_time()) < 0 ){
				Time::date_t newStartDate = task.get_period().get_start_time().get_date() + ((Time::DAY*7));
				Time::date_t newEndDate = task.get_period().get_end_time().get_date() + ((Time::DAY*7));
			}

			// add task into timetable

			int duration = (activePeriod.get_end_time() - activePeriod.get_start_time());
			if(tillEnd(duration, taskList, task, forceAdd, activePeriod) == true){
				got_clash = true;
			}
		}

		if(type == after)
		{
			while((task.get_period().get_end_time() - activePeriod.get_end_time()) < 0 ){
				Time::date_t newStartDate = task.get_period().get_start_time().get_date() - ((Time::DAY*7));
				Time::date_t newEndDate = task.get_period().get_end_time().get_date() - ((Time::DAY*7));
			}

			// add task into timetable

			int duration = (activePeriod.get_end_time() - activePeriod.get_start_time());
			if(tillEnd(duration, taskList, task, forceAdd, activePeriod) == true){
				got_clash = true;
			}
		}
	
		// save taskList to dataStorage
		_dataStorage.save(taskList);


		//return the clash if there is clashes
		if(got_clash == true) {
		 return _clashList;
		}
		else{
			list<Task> blankList;
			return blankList;
		}
	}	
}



bool ToDoMngr::tillEnd(int duration, list<Task> taskList, Task task, bool forceAdd, TimePeriod activePeriod)
{
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

bool ToDoMngr::tillStart(int duration, list<Task> taskList, Task task, bool forceAdd, TimePeriod activePeriod)
{	
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

list<Task> ToDoMngr::edit(int taskId, TaskElement* taskElem, Task* task, bool forceEdit)
{
	list<Task> _blankList;
	// get the task that will be edited
	list<Task>::iterator taskIterator = _activeTaskList.begin();
	for(int i=1;i<=taskId; i++)
	{
		taskIterator++;
	}

	// check taskElem for what to edit
	if(taskElem->_time == true)
	{
		//edit time
		taskIterator->modify_time(task->get_time());
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
			bool clash = false;
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
	Table_Mode=false;
	_dataStorage.updateStorageName ("");
}

ToDoMngr::ToDoMngr (string storageName) {
	Table_Mode=false;
	_dataStorage.updateStorageName (storageName);
}

void ToDoMngr::updateStorageName (string storageName) {
	_dataStorage.exit ();
	_activeTaskList.clear ();
	_clashList.clear ();
	_dataStorage.updateStorageName (storageName);
}

list<Task> ToDoMngr::edit (int taskId, Task task, bool forceEdit) {
	TaskElement taskElem;
	setTaskElem (&taskElem, &task);
	return edit (taskId, &taskElem, &task, forceEdit);
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

	//cout<<tableName;
	vector<string> table_names=_dataStorage.load_table_name();

	vector<string>::iterator browse;
	bool found=false;;
	for(browse=table_names.begin();browse!=table_names.end();browse++){

		if(tableN==*browse){
			Table_Mode=true;
			tableName=tableN;
            found= true;
			break;
		}		
	}
	return found;
}

void ToDoMngr::deactivateTable () {
	Table_Mode=false;
}

bool ToDoMngr::getTableActivationStatus () {

	if(Table_Mode==true)
		return true;
	else
	    return false;
}

