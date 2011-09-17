#ifndef TEMPLATE_H_
#define TEMPLATE_H_
#include "TimePeriod.h"
#include <string>
using namespace std;

enum TemplateType {DEADLINE, MEETING, CLASS, DEFAULT};
enum PriorityLevel {HIGH, IMPORTANT, SIMPLE};

class Template {
private:
	TemplateType _type;

	string _name;
	string _venue;
	string _description;
	Time _time;
	TimePeriod _period;

	bool forceAdd;				//forceAdd need to be true if user want to add it altough it's clashed
	PriorityLevel _priority;	//indicate priority level of simultaneous task
								//first come, first serve basis for the tasks with the same priority level
public:
	Template (TemplateType, Time);
	Template (TemplateType, TimePeriod);
	Template (string);			//an inverse of string convert ()

	string convert ();			//convert the whole Template to string

	void modify_name ();
	void modify_time ();
	void modify_period ();
	void modify_venue ();
	void modify_description ();
	void modify_forceAdd ();
	void modify_priority ();
	
	string get_name ();
	Time get_time ();
	TimePeriod get_period ();
	string get_venue ();
	string get_description ();
	bool get_forceAdd ();
	PriorityLevel get_priority ();

	//tool for any sorting algorithm
	bool operator< (Template);
	bool operator> (Template);
	bool operator== (Template);
	bool operator<= (Template);
	bool operator>= (Template);
};

#endif