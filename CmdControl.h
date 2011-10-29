/*
*Author Rith
*/

#ifndef CMDCONTROL_H_
#define CMDCONTROL_H_
#include "Logging.h"
#include "VldCmdCtrl.h"
#include "Task.h"
#include "ToDoMngr.h"
#include <string>
#include <vector>
#include <queue>
#include <list>
using namespace std;

template <typename data_t>
queue<data_t>& operator+= (queue<data_t>& Q1, queue<data_t> Q2);

class CmdControl: public VldCmdCtrl {
private:
	typedef unsigned short int indx_t;
	enum input_t {CMD, DATA, NONE};
	enum prompt_t {ADDCLASHED, EDITCLASHED, VLDINPUT, VLDCMD, NOPROMPT};

public:
	template <typename data_t>
	void clear (queue<data_t>* Q);
	template <typename data_t>
	void append (queue<data_t>* Q1, queue<data_t> Q2);
	CmdControl (bool);
	//Command will load CmdList from built-in file
	//true if the user wants the '.' command
	//false if the user wants the literal command
	void updateInput (string& input);
	void addInput (string& input);

//	string getLeftOverInput ();
	//return a string of input that could not be able to access
	string executeCmd ();
private:
	Logging log;
	string _input;
	queue<command> _1stCmd;
	queue<command> _2ndCmd;
	queue<string> _1stData;
	queue<string> _2ndData;
	queue<input_t> _1stSeq;
	queue<input_t> _2ndSeq;
	queue<command>* _cmdInput;
	queue<string>* _dataInput;
	queue<input_t>* _sequence;
	bool _2ndQs;

	ToDoMngr _toDoMngr;
//	ToDoMngr::search_t _search;

	ToDoMngr::TaskElement _taskElement;
	Task _task;
	list<Task>* _taskList;
	string _tableName;
	bool _force;
	bool _activeListAccessible;

	input_t _flagError;
	prompt_t _flagPrompt;
	bool _dayMonth;

	//view variable
	int _taskId;
	bool _first;
	bool _last;
	TimePeriod _period;
	Time _time;
	view_t _viewType;

	static string INV_CMD;
	static string INV_DATA;
	static string MSG_ERROR;
	static string MSG_CLEAR;
	static string MSG_CLASH;
	static string MSG_ADDED;
	static string MSG_EDITED;
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
	string executeSEARCH ();
	string executeTABLE ();
	string executeHELP ();
	string executeNEXT ();
	string executePREV ();
	string executeFIRST ();
	string executeLAST ();
	string checkAlert ();
	string checkPrompt ();
	string executeEditCmd (int);
	void executeSORT ();
	string executePROMPT ();

	void splitInput ();
	command translateCmd (string);
	//return an fresh command and flag command error if command is not valid
	void checkIfStandAloneCmd ();

	int get_int ();
	Task get_task ();
	void update_task (Task*);
	TimePeriod get_period ();
	Time get_time ();
	Time::date_t get_date ();
	Time::clk_t get_clock ();
	bool notMorning (string);
	bool notMorning ();
	bool getTableName ();		// return false if the tableName is invalid
	void get_date (int&, int&, int&);
	void convertToInt (string, int&, int&);
	int convertToInt (string);	// return -1 if the whole string is not an integer
	string mergeStringInput ();
	void clearTaskElement ();
	bool checkTaskElement (bool);
	void check2ndQsCommand ();

	void mergeAllQs ();
	void activate2ndQs ();
	void deactivate2ndQs ();
	void push (command);
	void push (string);
	void clear ();
	bool pop ();
};
#endif
