#ifndef CMDCONTROL_H_
#define CMDCONTROL_H_
#include "ToDoMngr.h"
#include <string>
#include <vector>
#include <queue>
using namespace std;

template <typename data_t>
queue<data_t>& operator+= (queue<data_t>& Q1, queue<data_t> Q2);

template <typename data_t>
int binary_find (vector<data_t>&, data_t&);

template <typename data_t>
int binary_find (vector<data_t>&, data_t&, int, int);

class CmdControl {
public:

	template <typename data_t>
	void clear (queue<data_t>& Q);

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
/*
	CmdControl (string validCmdFile);
	//Command will load CmdList from specified file
	CmdControl (vector<cmd_pair> validCmd);
	//Command will use the cmdList given
*/	
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

	ToDoMngr _toDoMngr;
	string _tableName;
	list<Task>* _taskList;
	bool _force;
	bool _activeListAccessible;
	ToDoMngr::search_t _search;

	input_t _flagError;
	string _validCmdFile;
	bool _dayMonth;

	static string LOST_FILE;
	static string INV_CMD;
	static string INV_DATA;
	static string MSG_ERROR;
	static string MSG_CLEAR;
	static string MSG_CLASH;
	static string MSG_ADDED;
	static string MSG_DELETED;
	static string MSG_NO_NEXT;
	static string MSG_NO_PREV;
	static string MSG_WRONG_ID;
	static string MSG_WRONG_DATE;
	static string MSG_WRONG_TABLE;
	static string MSG_WRONG_PERIOD;

	string executeCmd (command);
	string executeADD ();
	string executeEDIT ();
	string executeDELETE ();
	string executeVIEW ();
	string executeTABLE ();
	bool executeNEXT ();
	bool executePREV ();
	string executeFunction (void* function (TimePeriod));
	
	inline void loadValidCmdList ();
	inline void splitInput ();
	inline void push (command);
	inline void push (string);
	inline void clear ();

	void pop ();
	command translateCmd (string);
	//return an fresh command and flag command error if command is not valid
	void reset ();

	int get_int ();
	Task get_task ();
	TimePeriod get_period ();
	Time get_time ();
	Time::date_t get_date ();
	//Format:: DDMMYYYY || DDMMYY || MMDDYYYY || MMDDYY || DDMMMYYYY || MMMDDYYYY || YYYYMMMDD
	//return INF_DATE and flag data error if unable to translate to date
	Time::clk_t get_clock ();
	//Format (24hrs):: HH.MM || HH:MM
	//Format (12hrs):: HH.MM AM || HH:MM AM || HAM || HHAM || HH.MMAM || HH:MMAM
	//return INF_CLOCK and flag data error if unable to translate to clock
	bool notMorning (string);
	bool notMorning ();
	bool getTableName ();		// return false if the tableName is invalid
	void get_date (int&, int&, int&);
	void convertToInt (string, int&, int&);
	int convertToInt (string);	// return -1 if the whole string is not an integer

	bool promptToContinue (string);
	bool promptToGetValidInput (string);
	command convertToCommand (int);
	string convertToString (command);
};
#endif