#include "CmdControl.h"
//#include "DataStorage.h"
#include <iostream>
#include <string>
#include <ctime>
using namespace std;
/*
int main () {

	list<Task> tasks;
	Task temp;
	Time time2 (5022005, 100);
	Time time1 (5022005, 530);
	TimePeriod period;
	Time startTime (16022005, 0);
	Time endTime (27052005, 0);
//	startTime = time2;
//	endTime = startTime + 10 * Time::DAY;
	ToDoMngr _toDoMngr;
	bool force = true;
	for (int i = 0; i < 300; i++) {
		temp.note = i + 1;
		temp.venue = 20 + i;
		period.modify_start_time (time2);
		period.modify_end_time (time1);
		temp.modify_period (period);
		time2 = time2 + 800;
		time1 = time1 + 900;
		tasks.push_back (temp);
		_toDoMngr.add (temp, force);
	}
	string str;
	period.modify_start_time (startTime);
	period.modify_end_time (endTime);
	TimePeriod Period;
	view_t viewType = MONTHLY;
//	cout << Period.string_time_period () << endl;
	cout << "view the week of " << startTime.string_date () << "\n\n" << endl;
	str = _toDoMngr.view (WEEKLY, startTime);
	cout << str << endl;
	_toDoMngr.erase (period);
	cout << _toDoMngr.view (WEEKLY, startTime);
//	cout << _toDoMngr.view (_toDoMngr.get_active_list ());
//	cout << _toDoMngr.view (7) << endl;
//	cout << _toDoMngr.view (10) << endl;
//	cout << _toDoMngr.view (11) << endl;
/*
	DataStorage _dataStorage;
	int start, end;
	start = time (NULL);
	_dataStorage.save (tasks);
	end = time (NULL);
cout << "finished saving " << end - start << endl;
cout << startTime.string_date () << endl;
cout << endTime.string_date () << endl;
	TimePeriod Period (startTime, endTime);
	start = time (NULL);
	tasks = _dataStorage.load (Period);
	end = time (NULL);
cout << "finished loading " << end - start << endl;
	list<Task>::iterator iter;
	start = time (NULL);
	for (iter = tasks.begin (); iter != tasks.end (); iter++)
		cout << iter->stringConvert () << endl;
	end = time (NULL);
cout << "finished viewing " << end - start << endl;

	return 0;
}
*/
/*
int main () {
	CmdControl cmdControl;
	cmdControl.executeCmd ();
	return 0;
}
*/

int main () {
	string str;
	bool dotCmd;
	cout << "dotCmd = ";
	cin >> dotCmd;
	try {
		CmdControl cmdControl (dotCmd);
		while (str != "exit") {
			cout << "command: ";
			getline (cin, str);
//			cout << str << endl;
			try {
				cmdControl.updateInput (str);
				cout << cmdControl.executeCmd () << endl;

				switch (cmdControl.getPromptFlag () ) {
				case 0:
					while (true) {
						cout << "Do you still want to continue? (Y/N): ";
						getline (cin, str);
						if (str == "Y" || str == "y") {
							cmdControl.activatePrompt ();
							break;
						} else if (str == "N" || str == "n") {
							break;
						} else;
					}
				default:
					break;
				}
			} catch (string xcpt) {
				cout << "INV" << endl;
			}
		}
	} catch (string xcpt) {
		cout << "INV" + xcpt << endl;
	}
	return 0;
}

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