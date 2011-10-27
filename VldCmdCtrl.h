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
	enum command {	CCOSTOM, CFORCE, CEXACT, CSIMILAR, CEACH, CCOMMAND,						//extension
					CTIME, CDATE, CFROM, CTO, CNAME, CVENUE, CNOTE, CALERT, CREPEAT, CPRIORITY,	//marker
					CHIGH, CIMPT, CNOMAL, CDAY, CWEEK, CMONTH, CNOW, CTODAY, CTMR,				//object 
					CADD, CEDIT, CDELETE, CTABLE, CVIEW, CREMINDER, CNEXT, CPREVIOUS,			//command
					CFIRST, CLAST, CUNDO, CREDO, CHELP, CSORT, CSEARCH, CCLEAR, CRESET, CEXIT, CVOID};
	struct cmd_pair {
		string str_cmd;
		command enum_cmd;
	};

	string get_vldCmdList (vector<cmd_pair>&, int&);
	bool get_dayMonth ();
	void update_vldCmdList (vector<cmd_pair>&);
	void update_dayMonth (bool&);
	void resetCmd ();
	void resetDateType ();
	void reset ();

	string convertToString (command);
	bool search_vldCmd (string&, vector<cmd_pair>::iterator);
	bool search_vldCmd (string, command&);
	bool search_standAloneCmd (string, command&);

protected:
	bool _dotCmd;
	string _validCmdFile;
	string _dfltCmdFile;
	string _validDayMnth;
	vector<cmd_pair> _validCmd;
	int standAloneCmdEndPos;

	static string LOST_FILE;
	static string LOAD_DFLT_CMD;

	vector<cmd_pair> get_vldCmdList (string, int&);
	command convertToCommand (int);
	bool checkIfVldExtension (command, command);
};
#endif
