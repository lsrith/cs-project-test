#ifndef CMDCONTROL_H_
#define CMDCONTROL_H_
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
//	enum data_t {INT, STRING, TIME};

	struct command {
		string cmd;
		vector<indx_t> prev;	
		//store the indice of the prev to-be-executed commands
		vector<indx_t> next;	
		//store the indice of the next to-be-executed commands
	};

	CmdControl ();
	//Command will load CmdList from built-in file
	CmdControl (string cmdFile, string validCmdFile);
	//Command will load CmdList from specified file
	CmdControl (vector<command> cmdList, vector<string> validCmd);
	//Command will use the cmdList given
	
	void updateInput (string& input);
	void addInput (string& input);
	void updateInput (queue<command> cmdInput, queue<string> dataInput, queue<input_t> sequence);
	void addInput (queue<command> cmdInput, queue<string> dataInput, queue<input_t> sequence);
	vector<command> getCmdList ();
	
	string getLeftOverInput ();
	//return a string of input that could not be able to access
	string executeCmd ();
	input_t getErrorFlag ();

private:	
	vector<command>* _cmdList;
	string _input;
	queue<command*>* _cmdInput;
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
	indx_t translateCmd (string);
	//return an fresh command and flag the error if command is not valid
//	void* translateData (data_t, string);
	int convertToInt (string);
	Time convertToTime (string);
};
#endif