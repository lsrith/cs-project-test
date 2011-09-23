#ifndef TIME_H_
#define TIME_H_
#include "Template.h"
#include <time.h>
#include <string>
using namespace std;

typedef unsigned short int clk_t;		//format: HHMM 
typedef unsigned int date_t;            //format: DDMMYYYY
enum time_period_cmp {BEFORE, RIGHT_BEFORE, AFTER, RIGHT_AFTER, CLASH};

bool _valid_date (date_t date);
bool _valid_time (clk_t time);
bool LeapYear (int year);
bool isAfter (date_t start_date, date_t end_date);

class Time {                            //class name is to be modified
private:
	clk_t _time;
	date_t _date;
	friend class TimePeriod;
   
	int count_days ();					//count number of days since 01/01/1970
	int convert_to_mins ();				//count number of mins since 01/01/1970 at 00:00
	string display_day (int);
	string display_month (int);
	int get_day ();

public:
	Time ();
	Time (date_t date, clk_t time);

	void modify_date (date_t new_date);	//throw invalid date if entered date is invalid
	void modify_time (clk_t new_time);	//throw invalid time if entered time is invalid
	void current_time ();				//set _time and _date to current time and date
	
	date_t get_date ();
	clk_t get_time ();
	string display_time ();
	string display_date ();

	bool operator== (Time time);
	bool operator> (Time time);
	bool operator< (Time time);      //return true if the time is after *this, else false

	int operator- (Time time);		//return the difference between the two time in mins
/*
	Time operator+ (Time time);		//return the sum of two time
									//to be implemented -- be careful
	Time operator= (Time time);
*/
};

#endif