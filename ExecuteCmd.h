#ifndef EXECUTECMD_H
#define EXECUTECMD_H
#include "VldCmdCtrl.h"
#include "ToDoMngr.h"
using namespace std;

template <typename data_t>
void append_back (list<data_t>* main, list<data_t> sub);

class ExecuteCmd: public VldCmdCtrl {
protected:
	typedef unsigned short int indx_t;
	enum input_t {CMD, DATA, NONE};

public:
	ExecuteCmd ();
	ExecuteCmd (bool);
	~ExecuteCmd ();
	//Command will load CmdList from built-in file
	//true if the user wants the '.' command
	//false if the user wants the literal command

	void updateInput (string& input);
	string get_input ();
	//return a string of input that could not be able to access
	//return a string with a break point if execution cannot be done
	virtual bool execute ();
	string result ();

	static string BREAK;
	static string MORE;

protected:
	string _input;
	string _result;
	queue<command>* _cmdInput;
	queue<string>* _dataInput;
	queue<input_t>* _sequence;
	list<string>* _splitedInput;
	input_t _flagError;
	ToDoMngr* _toDoMngr;

	void splitInput ();
	command translateCmd (string);
	//return an fresh command and flag command error if command is not valid

	int get_int ();
	Task get_task ();
	TimePeriod get_period ();
	Time get_time ();
	Time::date_t get_date ();
	Time::clk_t get_clock ();
	bool notMorning (string);
	bool notMorning ();
	void get_date (int&, int&, int&);
	void convertToInt (string, int&, int&);
	int convertToInt (string);	// return -1 if the whole string is not an integer
	string mergeStringInput ();
	string mergeSimStringInput ();

	void push (command);
	void push (string);
	void clear ();
	bool pop ();
};

class Add: public ExecuteCmd {
public:
	Add (vector<cmd_pair>, ToDoMngr*);
	~Add ();
	bool execute ();
	static string MSG_CLASH;

private:
	static string MSG_ADDED;
	void insertBreakPoint ();
};

//think of wrong id??
class Edit: public ExecuteCmd {
public:
	Edit (vector<cmd_pair>, ToDoMngr*);
	~Edit ();
	bool execute ();

private:
	static string MSG_EDITED;
	void insertBreakPoint ();
};

class View: public ExecuteCmd {
public:
	View (vector<cmd_pair>, ToDoMngr*);
	~View ();
	bool execute ();
	void next ();
	void prev ();
	void last ();
	void first ();

private:
	static string MSG_NO_NEXT;
	static string MSG_NO_PREV;
	static string MSG_WRONG_ID;

	int _taskId;
	bool _first;
	bool _last;
	TimePeriod _period;
	Time _time;
	view_t _viewType;
	bool _activeListAccessible;
};

class Reminder: public ExecuteCmd {
public:
	Reminder (ToDoMngr*);
	bool execute ();
};

class History: public ExecuteCmd {
public:
	History (ToDoMngr*);
	void undo (bool);
	bool execute ();

private:
	bool _undo;
};

class Delete: public ExecuteCmd {
public:
	Delete (vector<cmd_pair>, ToDoMngr*);
	~Delete ();
	bool execute ();

private:
	static string MSG_DELETED;
};
#endif