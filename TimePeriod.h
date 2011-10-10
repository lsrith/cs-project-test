//TimePeriod.h
#ifndef TIMEPERIOD_H
#define TIMEPERIOD_H
#include "Time.h"
#include <string>
using namespace std;

class TimePeriod {
public:
	enum time_period_cmp {BEFORE, RIGHT_BEFORE, AFTER, RIGHT_AFTER, CLASH};

	TimePeriod ();
	TimePeriod (Time start_time, Time end_time);
	
	void modify_start_time (Time);
	void modify_end_time (Time);

	Time get_start_time ();
	Time get_end_time ();

	string string_time_period ();
	time_period_cmp compare (TimePeriod timePeriod);

	bool operator== (TimePeriod timePeriod);  //return true if the timings clash, else false
	bool operator== (Time time);
	bool operator!= (TimePeriod timePeriod);
	bool operator!= (Time time);
	bool operator< (TimePeriod timePeriod);   //return true if the timing is after *this, else false
	bool operator< (Time time);
	bool operator> (TimePeriod timePeriod);   //return true if the timing is before *this, else false
	bool operator> (Time time);
	bool operator<= (TimePeriod timePeriod);  //return true if the timing is just right after *this
	bool operator<= (Time time);
	bool operator>= (TimePeriod timePeriod);  //return true if the timing is just right before *this
	bool operator>= (Time time);
	void operator+ (TimePeriod timePeriod);
	//merge the two timing together, ignore the free duration or clash between
	int operator- (TimePeriod timePeriod);   
	//return the free duration (in mins) between the two timings (return -1 if clash)

private:
	Time _start;
	Time _end;

	static string MSG_START_AFTER_END;
};
#endif