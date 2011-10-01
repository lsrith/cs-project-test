#ifndef CMDCONTROL_H_
#define CMDCONTROL_H_
#include "Task.h"
#include <string>
#include <vector>
#include <queue>
using namespace std;

template <typename data_t>
queue<data_t>& operator+= (queue<data_t>& Q1, queue<data_t> Q2);

template <typename data_t>
void clear (queue<data_t>& Q);

template <typename data_t>
int binary_find (vector<data_t>&, data_t&);

template <typename data_t>
int binary_find (vector<data_t>&, data_t&, int, int);

class CmdControl {
public:
	typedef unsigned short int indx_t;
	enum input_t {CMD, DATA, NONE};
	enum command {	COSTOM, FORCE, EXACT, SIMILAR, EACH, COMMAND,					//extension
					TIME, DATE, FROM, TO, NAME, VENUE, NOTE, ALERT, REPEAT,			//marker
					HIGH, IMPT, NOMAL, DAY, WEEK, MONTH,							//object 
					ADD, EDIT, DELETE, TABLE, VIEW, REMINDER, NEXT, PREVIOUS,		//command
					FIRST, LAST, UNDO, REDO, HELP, SORT, SEARCH, CLEAR, RESET, EXIT, VOID};
	struct cmd_pair {
		string str_cmd;
		command enum_cmd;
	};

	CmdControl ();
	//Command will load CmdList from built-in file
	CmdControl (string validCmdFile);
	//Command will load CmdList from specified file
	CmdControl (vector<cmd_pair> validCmd);
	//Command will use the cmdList given
	
	void updateInput (string& input);
	void addInput (string& input);

//	string getLeftOverInput ();
	//return a string of input that could not be able to access
	string executeCmd ();
	input_t getErrorFlag ();

private:	
	string _input;
	queue<command> _cmdInput;
	queue<string> _dataInput;
	queue<input_t> _sequence;
	vector<cmd_pair> _validCmd;

	input_t _flagError;
	string _validCmdFile;
	bool _dateBeforeMonth;

	const string LOST_FILE;
	const string INV_CMD;
	const string INV_DATA;

//	bool executeCmd (command);
	void loadValidCmdList ();
	void splitInput ();
	command translateCmd (string);
	//return an fresh command and flag the error if command is not valid
	
	int get_int ();
	Time get_time ();
	Time::date_t get_date ();
	Time::clk_t get_clock ();
	//Format (24hrs):: H (0H:00) || HH (HH:00) || HH.MM || HH:MM
	//Format (12hrs):: H AM || HH AM || HH.MM AM || HH:MM AM || HAM || HHAM || HH.MMAM || HH:MMAM
	bool notMorning (string);
	bool notMorning ();
	void convertToInt (string, int&, int&);
	
	command convertToCommand (int);
	string convertToString (command);
};
#endif