#ifndef CMDTRANS_H
#define CMDTRANS_H
#include "Task.h"
#include "VldCmdCtrl.h"
#include <queue>
#include <list>
using namespace std;

class CmdTrans: public VldCmdCtrl {
protected:
	typedef unsigned short int indx_t;
	enum input_t {CMD, DATA, NONE};

public:
	CmdTrans ();
	~CmdTrans ();
	void splitInput (string&);
	string getLeftOverInput ();
	int get_int ();
	Task get_task ();
	TimePeriod get_period ();
	Time get_time ();
	Time::date_t get_date ();
	Time::clk_t get_clock ();

	static string BREAK;

protected:
	queue<command>* _cmdInput;
	queue<string>* _dataInput;
	queue<input_t>* _sequence;
	list<string>* _splitedInput;
	input_t _flagError;

	command translateCmd (string);
	//return an fresh command and flag command error if command is not valid

	bool notMorning (string);
	bool notMorning ();
	void get_date (int&, int&, int&);
	void convertToInt (string, int&, int&);
	int convertToInt (string);	// return -1 if the whole string is not an integer
	string mergeStringInput ();
	string mergeSimStringInput ();

	void initPointers ();
	void push (command);
	void push (string);
	void clear ();
	bool pop ();
};
#endif