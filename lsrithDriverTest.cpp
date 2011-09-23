#include "Template.h"
#include <iostream>
#include <string>
using namespace std;

int main ()
{
	Time start (23092011, 1600);
	Time end (24092011, 1000);
	TimePeriod period (start, end);
//cout << "-1-" << endl;
	Template B (start);
	B._note = "GOOGLE";
	B._forceAdd = true;
	B._priority = HIGH;
	B._repeat_t = COSTOM;
	B._repeat = end;
//cout << "-2-" << endl;
	Template A (period);
	A._name = "BABE";
	A._venue = "NUS";
	A._note = "I LOVE YOU";
//cout << "\n-3-" << endl;
//cout << A.convert () << endl;
	Template C;
	C.invConvert (A.stringConvert ());
//cout << "\n-4-" << endl;
//cout << B.convert () << endl;
	Template D (B.stringConvert ());
//cout << "\n-5-" << endl;
	cout << C.stringConvert () << endl;
//cout << "\n-6-" << endl;
	cout << D.stringConvert () << endl;

	return 0;
}

/*
int main ()
{
/*
	Time now;
	now.current_time ();
	cout << now.display_date () << " at " << now.display_time () << endl;

	time_t rawTime;
	time (&rawTime);
	int sec = time (NULL);
	cout << sec << endl;
	cout << ctime (&rawTime) << endl;

	Time time1 (25021992, 530);
	Time time2 (17092011, 849);
	Time time3;
	time3.modify_date (11092011);
	time3.modify_time (1523);
	Time time4 = time1;
	Time time5 (12092011, 2000);

//	time4 = time1 - time2;


//----------------------	
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

	return 0;
}
*/
