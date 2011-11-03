#ifndef LOGGING_H
#define LOGGING_H
#include <stack>
using namespace std;

class Logging {
public:
	Logging ();
	void log (string);			//specify output file name
	void start (string);		//start function
	void end ();				//end last start
	void loop (string);			//start iterative, loop name
	void cond (string);			//start switch or if, name
	void cond (string, int);	//start switch, name, case
	void call (string);			//start calling function name
	void clear ();				//clear the log file

private:
	enum LOG {START = 0, LOOP, COND, CALL};

	bool _inactive;
	bool _start;
	bool _loop;
	string _file;
	unsigned int __tab;
	stack<unsigned int> __iter;
	stack<LOG> _history;
	stack<string> _func;

	string _tab ();
	void _clearHistory ();
	void _addLine (string);
	void _replaceLine (string);
	void _removeLine ();
};

#endif