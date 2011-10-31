/*
*Author Rith
*/

#ifndef VLDCMDCTRL_H
#define VLDCMDCTRL_H
#include <string>
#include <vector>
using namespace std;

class VldCmdCtrl {
public:
	enum command {	CCOSTOM = 0, CFORCE, CEXACT, CSIMILAR, CEACH, CCOMMAND,						//extension
					CTIME, CDATE, CFROM, CTO, CNAME, CVENUE, CNOTE, CALERT, CREPEAT, CPROMPT,	//marker
					CHOUR, CFORTNIGHT, CYEAR, CDAY, CWEEK, CMONTH, CNOW, CTODAY, CTMR,			//object 
					CADD, CEDIT, CDELETE, CTABLE, CVIEW, CREMINDER, CNEXT, CPREVIOUS,			//command
					CFIRST, CLAST, CUNDO, CREDO, CHELP, CSORT, CSEARCH, CCLEAR, CRESET, CEXIT,
					CDISCARD, CREPLACE, CINSERT, CVOID};
	struct cmd_pair {
		string str_cmd;
		command enum_cmd;
	};

	VldCmdCtrl ();
	VldCmdCtrl (bool);
	vector<cmd_pair> get_vldCmdList ();
	void load_vldCmdList ();
	void update_vldCmdList (vector<cmd_pair>&);
	void reset ();

	static string convertToString (command);
	static command convertToCommand (int);	
	command convertToCommand (string);
	
	bool search_vldCmd (string&, vector<cmd_pair>::iterator);
	bool search_vldCmd (string, command&);
	bool search_standAloneCmd (string, command&);

protected:
	bool _dotCmd;
	string _validCmdFile;
	string _dfltCmdFile;
	vector<cmd_pair> _validCmd;
	int standAloneCmdEndPos;

	static string LOST_FILE;
	static string LOAD_DFLT_CMD;

	vector<cmd_pair> get_vldCmdList (string, int&);
	bool checkIfVldExtension (command, command);
};
#endif
