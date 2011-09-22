#ifndef TEMPLATE_H_
#define TEMPLATE_H_
#include "TimePeriod.h"
#include <string>
using namespace std;

enum TemplateType {DEADLINE, MEETING, CLASS, DEFAULT};
enum PriorityLevel {HIGH, IMPORTANT, SIMPLE};
enum RepeatType {};

//which one shoud we use??

struct Template {
	TemplateType _type;

	string _name;
	string _venue;
	string _description;
	Time _time;
	TimePeriod _period;
	Time _alert;				//if it need to be alerted
	RepeatType _repeat_t;		
	int _index;					//each template is to be given a unique index for a faster search performance

	bool _forceAdd;				//forceAdd need to be true if user want to add it altough it's clashed
	PriorityLevel _priority;	//indicate priority level of simultaneous task
								//first come, first serve basis for the tasks with the same priority level
};

class Template {
private:
	TemplateType _type;

	string _name;
	string _venue;
	string _description;
	Time _time;
	TimePeriod _period;
	Time _alert;				//if it need to be alerted
	RepeatType _repeat_t;		
	int _index;					//each template is to be given a unique index for a faster search performance

	bool _forceAdd;				//forceAdd need to be true if user want to add it altough it's clashed
	PriorityLevel _priority;	//indicate priority level of simultaneous task
								//first come, first serve basis for the tasks with the same priority level
	friend class ToDoMngr;		//ToDoMngr could access these private attribute freely

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
	void modify_index ();

	string get_name ();
	Time get_time ();
	TimePeriod get_period ();
	string get_venue ();
	string get_description ();
	bool get_forceAdd ();
	PriorityLevel get_priority ();
	int get_index ();

	//tool for any sorting algorithm
	bool operator< (Template);
	bool operator> (Template);
	bool operator== (Template);
	bool operator<= (Template);
	bool operator>= (Template);
};

void sort (list<Template>&, string sortType);
// sort the list based on specified sorting type -- sorting type is to be determined
#endif