//TimePeriod.h
#ifndef TIMEPERIOD_H
#define TIMEPERIOD_H
#include "Time.h"
#include <string>
using namespace std;

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