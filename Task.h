/**
 * This class is define a Task type
 * 
 * @author LY SAMRITH
 */
#ifndef TEMPLATE_H_
#define TEMPLATE_H_
#include "Time.h"
#include "TimePeriod.h"
#include <string>
using namespace std;

class Task {
public:
	string venue;
	string note;
	Time alert;					//if it need to be alerted
	unsigned int repeat;		//num = 0 if no repeat, num = 1 if monthly repeat, num = 2 if yearly repeat
								//else represents the difference of mins between each repeat
	TimePeriod r_period;		//repeat period
	bool timeTask;

	Task ();
	Task (Time);
	Task (TimePeriod);
	Task (string&);				//an inverse of string convert ()

	void invConvert (string&);
	string stringConvert ();	//convert the whole Template to string

	void modify_time (Time);
	void modify_period (TimePeriod);

	Time get_time () const;
	TimePeriod get_period () const;
	unsigned int get_index () const;

	static bool compareByStartTime (Task task1, Task task2);
	static bool compareByEndTime (Task task1, Task task2);
	static bool compareByAlert (Task task1, Task task2);
	static bool compareByVenue (Task task1, Task task2);
	static bool compareByIndex (Task task1, Task task2);
private:
	Time _time;
	TimePeriod _period;
	unsigned int _index;		//each template is to be given a unique index
								//default index == 0
	friend class DataStorage;

	static string spaceMarker;

	int findPosition (string, string, int);
};
#endif
