/*
#ifndef COMMAND_H_
#define COMMAND_H_
#include <string>
#include <vector>
#include <queue>
using namespace std;

class Command {
public:
	typedef unsigned short int indx_t;
	enum input_t {CMD, DATA, NONE};
	enum data_t {INT, STRING, TIME};
	enum command {ACC, ADD, ALRT, BACK, CLSH, CLR, CMD, CST, CP, CUT, DATA, 
				DATE, DAY, DEL, DES, EDIT, ENT, EXCT, EXIT, FRC, FROM, HELP, 
				HIGH, IMPT, INPT, IND, LOC, LOG, LAST, MOV, MTH, NAME, NX, PER, 
				PRE, PREV, PRI, PRNT, PST, PSW, REDO, REM, REP, SML, SORT, SRCH, 
				TAB, TEMP, TIME, TO, UNDO, VIEW, WK, YR, INT, STR};
/--
	struct command {
		string cmd;
		indx_t indx;
		vector<indx_t> exe_cmd;	
		//store the indice of the next to-be-executed commands
	};
--/
	Command ();
	//Command will load CmdList from built-in file
	Command (string cmdFile, string validCmdFile);
	//Command will load CmdList from specified file
	Command (vector<command> cmdList, vector<string> validCmd);
	//Command will use the cmdList given
	
	void updateInput (string& input);
	void addInput (string& input);
	void updateInput (queue<command> cmdInput, queue<string> dataInput, queue<input_t> sequence);
	void addInput (queue<command> cmdInput, queue<string> dataInput, queue<input_t> sequence);
	vector<command> getCmdList ();
	
	string getLeftOverInput ();
	//return a string of input that could not be able to access
	void executeCmd ();
	input_t getErrorFlag ();

private:	
	vector<command>* _cmdList;
	string _input;
	queue<command>* _cmdInput;
	queue<string>* _dataInput;
	queue<input_t>* _sequence;
	vector<string>* _validCmd;

	input_t _flagError;
	string _cmdFile;
	string _validCmdFile;

	const string LOST_FILE;
	const string INV_CMD;
	const string INV_DATA;

	bool executeCmd (command);
	void loadCmdList ();
	void loadValidCmdList ();
	void splitInput ();
	command translateCmd (string);
	//return an fresh command and flag the error if command is not valid
	void* translateData (data_t, string);
};

template <typename data_t>
operator+= (queue<data_t>& Q1, queue<data_t> Q2);

template <typename data_t>
void clear (queue<data_t>& Q);
#endif
*/