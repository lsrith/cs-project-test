#include "ToDoMngr.h"
#include "TimePeriod.h"
#include "Template.h"
#include <list>
#include <queue>
#include <string>
using namespace std;

ToDoMngr::ToDoMngr ()
{
	_storageFile = "Data Storage.txt";
}

//Haven't refactored the code yet :) Ris
bool IS_DAY(string COMMAND)
{ if( COMMAND.substr(0,3)=="Mon"|| COMMAND.substr(0,3)=="Tue"|| COMMAND.substr(0,3)=="Wed"
|| COMMAND.substr(0,3)=="Thu"|| COMMAND.substr(0,3)=="Fri"|| COMMAND.substr(0,3)=="Sat"|| COMMAND.substr(0,3)=="Sun")
return 1;
else 
	return 0;
}

bool IS_MONTH(string COMMAND)
{ if(COMMAND.substr(0,3)=="Jan"||COMMAND.substr(0,3)=="Feb"||COMMAND.substr(0,3)=="Mar"||COMMAND.substr(0,3)=="Apr"||
COMMAND.substr(0,3)=="May"||COMMAND.substr(0,3)=="Jun"||COMMAND.substr(0,3)=="Jul"||COMMAND.substr(0,3)=="Aug"||
COMMAND.substr(0,3)=="Sep"||COMMAND.substr(0,3)=="Oct"||COMMAND.substr(0,3)=="Nov"||COMMAND.substr(0,3)=="Dec")
return 1;
else 
	return 0;
}

bool isNULL(string command){
	if(command=="0")
		return 1;
	else
		return 0;

}

void view (string viewType){

	ifstream myreadfile;
	myreadfile.open(_storageFile);

	string TASK;
	string DATE;
	int marker=0;

	if(IS_DAY(viewType)){

		while(getline(myreadfile,TASK)&&(marker==0)){
			if(IS_MONTH(TASK))
				cout<<TASK<<endl;
			if(TASK.substr(0,3)=="Dec")
				marker=1;

			DATE=TASK.substr(4,TASK.length());

			if(TASK.substr(0,3)==viewType.substr(0,3)){
				cout<<viewType<<" "<<DATE<<endl;
				while(getline(myreadfile,TASK)){
					if(TASK.substr(0,4)=="Task")
						cout<<TASK<<endl;
					else
						break;
				}

			}

		}


	}

	else if((IS_MONTH(viewType))&&((viewType.substr(3,viewType.length())).empty())){

		while(getline(myreadfile,TASK)){
			if(TASK.substr(0,3)==viewType.substr(0,3)){
				while(getline(myreadfile,TASK)){
					if(IS_DAY(TASK)||TASK.substr(0,4)=="Task")
						cout<<TASK<<endl;
					else
						break;
				}
			}

		}
	}

	else if(isdigit(viewType.at(4)))
	{   string date;
	int marker=0;
	string datechecker;
	while(getline(myreadfile,TASK)&&(marker==0)){
		if(TASK.substr(0,3)==viewType.substr(0,3)){
			if(isdigit(viewType.at(5)))
				date=viewType.at(4)+viewType.at(5);
			else
				date=viewType.at(4);


			while(getline(myreadfile,TASK)&&(marker==0)){


				if(isdigit(TASK.at(5)))
					datechecker=TASK.at(4)+TASK.at(5);
				else
					datechecker=TASK.at(4);

				if(date==datechecker){
					while(getline(myreadfile,TASK)){

						if(IS_DAY(TASK)){
							marker=1;
							break;
						}
						else{
							if(IS_MONTH(TASK)){
								marker=1;
								break;
							}
							cout<<TASK<<endl;
							marker=1;
						}

					}
				}

			}
		}


	}
	}


	else if(viewType.substr(4,4)=="week"){
		char week_number=viewType.at(8);
		int j;
		int marker=0;

		while(getline(myreadfile,TASK)&&(marker==0)){
			if(TASK.substr(0,3)==viewType.substr(0,3)){

				for(j=0;j<((week_number-'0')*7)-7;){
					if(IS_DAY(TASK)){
						while(getline(myreadfile,TASK)){

							if(IS_DAY(TASK)||TASK.substr(0,4)=="Task"){
								j++;
								break;
							}

						}
					}
					else
						getline(myreadfile,TASK);
				}
				for(int i=0;i<7;){
					if(j==0){
						getline(myreadfile,TASK);
						j++;
					}
					if(IS_DAY(TASK)||TASK.substr(0,4)=="Task"){
						cout<<TASK<<endl;
						while(getline(myreadfile,TASK)){
							if(IS_DAY(TASK)){
								i++;
								break;
							}
							else{
								if(!(IS_MONTH(TASK))){
									cout<<TASK<<endl;
									marker=1;
								}
							}
						}
					}
				}
			}

		} } }

void help (string command){

	ifstream myhelpfile;
	myhelpfile.open("HELP.txt");
	string HELP_LINE;

	if(isNULL(command)){
		while(getline(myhelpfile, HELP_LINE))
		{
			cout<<HELP_LINE<<endl;
		}


	}


}

void reminder(){
	string MONTHS[12]={"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
	string Date;
	string Month;
	string FINAL_FORMAT;
	std::ostringstream oss;

	time_t t = time(0);   //get the current time
	struct tm * now = localtime( & t );
	oss << (now->tm_year + 1900) << '-' 
		<< (now->tm_mon + 1) << '-'
		<<  now->tm_mday;
	Date=oss.str();        //Stores the current computer date

	Month=MONTHS[(Date.at(5)-'0')-1];

	FINAL_FORMAT=Month+" "+Date.substr(7,Date.length());  //created a format to look like this -> Sep 22nd

	cout<<"Your tasks for today are as follows:"<<endl;
	view(FINAL_FORMAT);   //pass to view() to search for the tasks on that particular date and display it...year is not taken into account at present.




}