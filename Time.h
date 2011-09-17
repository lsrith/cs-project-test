#ifndef TIME_H_
#define TIME_H_
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
   
   int convert_to_mins ();

public:
   Time ();
   Time (date_t date, clk_t time);

   void modify_date (date_t new_date);
   void modify_time (clk_t new_time);
   string display_time ();
   string display_date ();

   bool operator== (Time time);
   bool operator> (Time time);
   bool operator< (Time time);      //return true if the time is after *this, else false
   int operator- (Time time);		//return the difference between the two time in mins
};

#endif