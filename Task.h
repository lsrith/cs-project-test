/*
*Author Rith
*/

#ifndef TEMPLATE_H_
#define TEMPLATE_H_
#include "Time.h"
#include "TimePeriod.h"
#include <string>
using namespace std;

enum PriorityLevel {HIGH, IMPORTANT, SIMPLE};
enum RepeatType {HOUR, DAY, WEEK, FORTNIGHT, MONTH, COSTOM, NOREPEAT};

class Task {
public:
	string name;
	string venue;
	string note;
	Time alert;					//if it need to be alerted
	RepeatType repeat_t;
	Time repeat;				//if repeat type is costom, _repeat is to indicate the space btw its repeat
	bool forceAdd;				//forceAdd need to be true if user want to add it altough it's clashed
	PriorityLevel priority;		//indicate priority level of simultaneous task
								//first come, first serve basis for the tasks with the same priority level
	bool timeTask;

	Task ();
	Task (Time);
	Task (TimePeriod);
	Task (string&);				//an inverse of string convert ()

	void invConvert (string&);
	string stringConvert ();	//convert the whole Template to string

	void modify_time (Time);
	void modify_period (TimePeriod);

	Time get_time ();
	TimePeriod get_period ();
	unsigned int get_index ();

private:
	Time _time;
	TimePeriod _period;
	unsigned int _index;		//each template is to be given a unique index
								//default index == 0

	friend class DataStorage;

	static string spaceMarker;

	RepeatType intToRepeat (int);
	PriorityLevel intToPriority (int);
	int repeatToInt (RepeatType);
	int priorityToInt (PriorityLevel);
	int findPosition (string, string, int);
};
#endif
