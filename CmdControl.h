#ifndef CMDCONTROL_H_
#define CMDCONTROL_H_
#include "VldCmdCtrl.h"
#include "Task.h"
//#include "ToDoMngr.h"
#include <string>
#include <vector>
#include <queue>
#include <list>
using namespace std;

template <typename data_t>
queue<data_t>& operator+= (queue<data_t>& Q1, queue<data_t> Q2);

class CmdControl: public VldCmdCtrl {
public:
	template <typename data_t>
	void clear (queue<data_t>& Q);

	typedef unsigned short int indx_t;
	enum input_t {CMD, DATA, NONE};

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
	queue<command> _1stCmd;
	queue<command> _2ndCmd;
	queue<string> _1stData;
	queue<command> _2ndData;
	queue<input_t> _1stSeq;
	queue<input_t> _2ndSeq;
	queue<command>* _cmdInput;
	queue<string>* _dataInput;
	queue<input_t>* _sequence;
/*
	ToDoMngr _toDoMngr;
	ToDoMngr::search_t _search;
*/
	list<Task>* _taskList;
	string _tableName;
	bool _force;
	bool _activeListAccessible;

	input_t _flagError;
	bool _dayMonth;

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
/*
	string executeCmd (command);
	string executeADD ();
	string executeEDIT ();
	string executeDELETE ();
	string executeVIEW ();
	string executeTABLE ();
	string executeMODIFY ();
	bool executeNEXT ();
	bool executePREV ();
	string executeFunction (void* function (TimePeriod));
*/
	string executeEditCmd (int);
	bool promptToContinue (string);
	bool promptToGetValidInput (string);
	
	void splitInput ();
	command translateCmd (string);
	//return an fresh command and flag command error if command is not valid
	void checkIfStandAloneCmd ();

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

	void activate2ndQs ();
	void deactivate2ndQs ();
	void push (command);
	void push (string);
	void clear ();
	bool pop ();
};
#endif