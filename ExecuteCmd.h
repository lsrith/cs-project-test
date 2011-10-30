#ifndef EXECUTECMD_H
#define EXECUTECMD_H
#include "VldCmdCtrl.h"
#include "ToDoMngr.h"
using namespace std;

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

	template <typename data_t>
	void clear (queue<data_t>* Q);

	template <typename data_t>
	void append (queue<data_t>* Q1, queue<data_t> Q2);

protected:
	string _input;
	string _result;
	queue<command>* _cmdInput;
	queue<string>* _dataInput;
	queue<input_t>* _sequence;
	queue<string>* _splitedInput;
	input_t _flagError;
	bool _dayMonth;

	void splitInput ();
	command translateCmd (string);
	//return an fresh command and flag command error if command is not valid
	void checkIfStandAloneCmd ();

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
#endif