#include "TimePeriod.h"
#include <iostream>
#include <string>
using namespace std;

int main ()
{

	Time now;
	now.current_time ();
	cout << now.display_date () << " at " << now.display_time () << endl;
/*
	time_t rawTime;
	time (&rawTime);
	int sec = time (NULL);
	cout << sec << endl;
	cout << ctime (&rawTime) << endl;
*/

	Time time1 (25021992, 530);
	Time time2 (17092011, 849);
	Time time3;
	time3.modify_date (11092011);
	time3.modify_time (1523);
	Time time4 = time1;
	Time time5 (12092011, 2000);

	time4 = time1 - time2;

/*	
	TimePeriod period1 (time1, time2);
	TimePeriod period2 (time2, time1);
	TimePeriod period3 (time3, time1);
	TimePeriod period4 (time3, time5);
	TimePeriod period5 (time4, time4);

	cout << period1.display_time_period () << endl;
	cout << period2.display_time_period () << endl;
	cout << period3.display_time_period () << endl;
	cout << period4.display_time_period () << endl;
	cout << period5.display_time_period () << endl;

	cout << period1 - period2 << endl;
	cout << period2 - period3 << endl;
	cout << period4 - period1 << endl;
	cout << period4 - period3 << endl;
	cout << period5 - period2 << endl;
	cout << period4 - period5 << endl;
*/
	return 0;
}