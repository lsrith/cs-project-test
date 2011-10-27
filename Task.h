/*
*Author Rith
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

	int findPosition (string, string, int);
};
#endif
