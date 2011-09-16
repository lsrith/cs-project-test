//TimePeriod.h
#ifndef TIMEPERIOD_H
#define TIMEPERIOD_H
#include <string>
using namespace std;

typedef unsigned short int clk_t;		//format: HHMM 
typedef unsigned int date_t;            //format: DDMMYYYY
enum time_period_cmp {BEFORE, RIGHT_BEFORE, AFTER, RIGHT_AFTER, CLASH};

class Time {                            //class name is to be modified
private:
   clk_t _time;
   date_t _date;
   friend class TimePeriod;
   
   static string INVALID_DATE;
   static string INVALID_TIME;
   static date_t INF_DATE;				//infinite date
   static clk_t INF_TIME;				//infinite time
   bool _valid_date (date_t date);
   bool _valid_time (clk_t time);
   bool LeapYear (int year);
   int convert_to_mins ();

public:
   Time ();
   Time (date_t date, clk_t time);

   void modify_date (date_t new_date);
   void modify_time (clk_t new_time);
   string display_time ();
   string display_date ();

   bool isAfter (date_t start_date, date_t end_date);
   bool operator== (Time time);
   bool operator> (Time time);
   bool operator< (Time time);      //return true if the time is after *this, else false
   int operator- (Time time);		//return the difference between the two time in mins
};

class TimePeriod {
private:
   Time _start;
   Time _end;

   static string MSG_START_AFTER_END;

public:
   TimePeriod (Time start_time, Time end_time);
   
   string display_time_period ();
   time_period_cmp compare (TimePeriod timePeriod);

   bool operator== (TimePeriod timePeriod);  //return true if the timings clash, else false
   bool operator< (TimePeriod timePeriod);   //return true if the timing is after *this, else false
   bool operator> (TimePeriod timePeriod);   //return true if the timing is before *this, else false
   bool operator<= (TimePeriod timePeriod);  //return true if the timing is just right after *this
   bool operator>= (TimePeriod timePeriod);  //return true if the timing is just right before *this
   void operator+ (TimePeriod timePeriod);
   //merge the two timing together, ignore the free duration or clash between
   int operator- (TimePeriod timePeriod);   
   //return the free duration (in mins) between the two timings (return -1 if clash)
};

#endif