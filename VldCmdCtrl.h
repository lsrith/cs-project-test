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
					CFIRST, CLAST, CUNDO, CREDO, CHELP, CSORT, CSEARCH, CCLEAR, CUSER, CEXIT,
					CDISCARD, CREPLACE, CINSERT, CLEFT, CRIGHT, CVOID, CNULL};
	struct cmd_pair {
		string str_cmd;
		command enum_cmd;
	};

	VldCmdCtrl ();
	vector<cmd_pair> get_vldCmdList ();
	void load_vldCmdList ();
	void update_vldCmdList (vector<cmd_pair>&);
	void reset ();

	static string convertToString (command);
	static command convertToCommand (int);	
	command convertToCommand (string);
	
	bool search_vldCmd (string, command&);

protected:
	bool _dotCmd;
	string _validCmdFile;
	string _dfltCmdFile;
	vector<cmd_pair> _validCmd;

	static string LOST_FILE;
	static string LOAD_DFLT_CMD;

	vector<cmd_pair> load_vldCmdList (string);
};
#endif
