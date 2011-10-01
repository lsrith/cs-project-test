#include "CmdControl.h"
#include <iostream>
#include <string>
using namespace std;

int main () {
	CmdControl cmdControl;
	cmdControl.executeCmd ();
	return 0;
}

/*
int main () {
	string str;
	try {
		CmdControl cmdControl;
		while (str != ".exit") {
			cout << "command: ";
			getline (cin, str);
			cout << str << endl;
			try {
				cmdControl.addInput (str);
				cout << cmdControl.executeCmd () << endl;
			} catch (string xcpt) {
				cout << "INV" << endl;
			}
		}
	} catch (string xcpt) {
		cout << xcpt << endl;
	}
	return 0;
}
*/
/*
int main ()
{
	Time start (23092011, 1600);
	Time end (24092011, 1000);
	TimePeriod period (start, end);
//cout << "-1-" << endl;
	Task B (start);
	B.note = "GOOGLE";
	B.forceAdd = true;
	B.priority = HIGH;
	B.repeat_t = COSTOM;
	B.repeat = end;
//cout << "-2-" << endl;
	Task A (period);
	A.name = "BABE";
	A.venue = "NUS";
	A.note = "I LOVE YOU";
//cout << "\n-3-" << endl;
	string a, b, c, d;
a = A.stringConvert ();
cout << a << endl;
	Task C;
	C.invConvert (A.stringConvert ());
//cout << "\n-4-" << endl;
b = B.stringConvert ();
cout << b << endl;
	Task D (B.stringConvert ());
//cout << "\n-5-" << endl;
	c = C.stringConvert ();
	cout << c << endl;
//cout << "\n-6-" << endl;
	d = D.stringConvert ();
	cout << d << endl;

	if (a == c)
		cout << "true" << endl;
	else
		cout << "false" << endl;
	if (b == d)
		cout << "true" << endl;
	else
		cout << "false" << endl;

	cout << B.priority << " " << D.priority << endl;

	Time now;
	now.current_time ();
	cout << now.string_date () << " at " << now.string_clock () << endl;

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
	Time time4;
	Time time5 (12092011, 2000);

	time4 = time1 + 40;
	
	TimePeriod period1 (time1, time2);
	TimePeriod period2 (time2, time1);
	TimePeriod period3 (time3, time1);
	TimePeriod period4 (time3, time5);
	TimePeriod period5 (time1 + 40, time4);

	cout << period1.string_time_period () << endl;
	cout << period2.string_time_period () << endl;
	cout << period3.string_time_period () << endl;
	cout << period4.string_time_period () << endl;
	cout << period5.string_time_period () << endl;

	cout << period1 - period2 << endl;
	cout << period2 - period3 << endl;
	cout << period4 - period1 << endl;
	cout << period4 - period3 << endl;
	cout << period5 - period2 << endl;
	cout << period4 - period5 << endl;

	return 0;
}
*/