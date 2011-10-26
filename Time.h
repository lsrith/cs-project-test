/*
*Author Rith
*/

#ifndef TIME_H_
#define TIME_H_
#include <time.h>
#include <string>
using namespace std;

class Time {                            //class name is to be modified
public:
	typedef unsigned short int clk_t;		//format: HHMM 
	typedef unsigned int date_t;            //format: DDMMYYYY
	static int DAY;
	static clk_t INF_CLOCK;
	static date_t INF_DATE;

	Time ();
	Time (date_t date, clk_t clk);

	void modify_date (date_t new_date);	//throw invalid date if entered date is invalid
	void modify_clock (clk_t new_clk);	//throw invalid time if entered time is invalid
	void current_time ();				//set _time and _date to current time and date

	int get_day ();	
	date_t get_date ();
	clk_t get_clock ();
	string string_date ();
	string string_clock ();
	string string_clock_24 ();

	bool operator== (Time time);
	bool operator!= (Time time);
	bool operator> (Time time);
	bool operator< (Time time);      //return true if the time is after *this, else false

	int operator- (Time time);		//return the difference between the two time in mins
	Time& operator+ (int mins);		//return the sum
	Time& operator= (Time time);
	bool operator++ ();				//increment by one month, return true if it's possible
	bool operator-- ();				//decrement by one month, return true if it's possible

	static bool _valid_date (date_t);
	static bool _valid_clock (clk_t);
	static bool LeapYear (int year);
	static bool isAfter (date_t start_date, date_t end_date);

	static string display_day (int);
	static string display_month (int);

private:
	clk_t _clk;
	date_t _date;

	static string INVALID_DATE;
	static string INVALID_TIME;
	friend class TimePeriod;
   
	int count_days ();									//count number of days since 01/01/1970
	unsigned int convert_to_mins ();					//count number of mins since 01/01/1970 at 00:00
	void convert_from_mins (unsigned int);				//inverse of convert_to_mins ()
	int days_in_month (int, int);
};
#endif