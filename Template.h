#ifndef TEMPLATE_H_
#define TEMPLATE_H_
#include "Time.h"
#include "TimePeriod.h"
#include <string>
using namespace std;

enum PriorityLevel {HIGH, IMPORTANT, SIMPLE};
enum RepeatType {HOUR, DAY, WEEK, FORTNIGHT, MONTH, COSTOM, NONE};

class Template {
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

public:
	string _name;
	string _venue;
	string _note;
	Time _alert;				//if it need to be alerted
	RepeatType _repeat_t;
	Time _repeat;				//if repeat type is costom, _repeat is to indicate the space btw its repeat
	bool _forceAdd;				//forceAdd need to be true if user want to add it altough it's clashed
	PriorityLevel _priority;	//indicate priority level of simultaneous task
								//first come, first serve basis for the tasks with the same priority level

	Template ();
	Template (Time);
	Template (TimePeriod);
	Template (string&);			//an inverse of string convert ()

	void invConvert (string&);
	string stringConvert ();			//convert the whole Template to string

	Time get_time ();
	TimePeriod get_period ();
	unsigned int get_index ();
/*
	//tool for any sorting algorithm
	bool operator< (Template);
	bool operator> (Template);
	bool operator== (Template);
	bool operator<= (Template);
	bool operator>= (Template);
*/
};
#endif
