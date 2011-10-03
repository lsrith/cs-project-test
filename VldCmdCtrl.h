#ifndef VLDCMDCTRL_H
#define VLDCMDCTRL_H
#include <string>
#include <vector>
using namespace std;

class VldCmdCtrl {
public:
	enum command {	COSTOM, FORCE, EXACT, SIMILAR, EACH, COMMAND,						//extension
					TIME, DATE, FROM, TO, NAME, VENUE, NOTE, ALERT, REPEAT,	PRIORITY,	//marker
					HIGH, IMPT, NOMAL, DAY, WEEK, MONTH,								//object 
					ADD, EDIT, DELETE, TABLE, VIEW, REMINDER, NEXT, PREVIOUS,			//command
					FIRST, LAST, UNDO, REDO, HELP, SORT, SEARCH, CLEAR, RESET, EXIT, VOID};
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