#include "VldCmdCtrl.h"
#include <string>
#include <vector>
#include <fstream>
using namespace std;

string VldCmdCtrl::LOST_FILE = "unable to find a file: ";
string VldCmdCtrl::LOAD_DFLT_CMD = "Loaded default command!";

string VldCmdCtrl::get_vldCmdList (vector<cmd_pair>& validCmd, int& endPos) {
	string str;

	try {
		validCmd = get_vldCmdList (_validCmdFile, endPos);
		if (validCmd.empty ()) {
			try {
				resetCmd ();
				str = get_vldCmdList (validCmd, endPos);
			} catch (string Message) {
				str = Message;
			}
		}
	} catch (string message) {
		try {
			resetCmd ();
			str = message + "\n" + LOAD_DFLT_CMD +"\n";
			str += get_vldCmdList (validCmd, endPos);
		} catch (string Message) {
			str = message + "\n" + Message;
		}
	}

	return str;
}

vector<VldCmdCtrl::cmd_pair> VldCmdCtrl::get_vldCmdList (string cmdFile, int& endPos){
	vector<cmd_pair> validCmd;
	endPos = 0;
	ifstream inFile (cmdFile);
	if (inFile.is_open ()) {
		cmd_pair cmd;
		int cmdIndx;
		while (inFile >> cmdIndx && inFile >> cmd.str_cmd) {
			cmd.enum_cmd = convertToCommand (cmdIndx);
			validCmd.push_back (cmd);
			if (cmd.str_cmd[0] != '-' && cmd.str_cmd[0] != '.')
				endPos++;
		}
	} else
		throw (LOST_FILE + cmdFile);
	
	return validCmd;
}

bool VldCmdCtrl::get_dayMonth () {
	bool dayMonth;
	ifstream inFile (_validDayMnth);
	if (inFile.is_open ())
		inFile >> dayMonth;
	else
		dayMonth = true;
	
	return dayMonth;
}

void VldCmdCtrl::update_vldCmdList (vector<cmd_pair>& validCmd) {
	ofstream outFile (_validCmdFile);
	int size = validCmd.size ();
	for (int i = 0; i < size; i++)
		outFile << validCmd[i].enum_cmd << " " << validCmd[i].str_cmd << endl;
	outFile.close ();
}

void VldCmdCtrl::update_dayMonth (bool& dayMnth) {
	ofstream outFile (_validDayMnth);
	outFile << dayMnth;
	outFile.close ();
}

void VldCmdCtrl::reset () {
	resetCmd ();
	resetDateType ();
}

void VldCmdCtrl::resetCmd () {
	ifstream inFile (_dfltCmdFile);
	ofstream outFile (_validCmdFile);

	if (inFile.is_open ()) {
		string str;
		while (getline (inFile, str))
			outFile << str << "\n";
	} else
		throw (LOST_FILE + _dfltCmdFile);

	inFile.close ();
	outFile.close ();
}

void VldCmdCtrl::resetDateType () {
	ofstream outFile (_validDayMnth);
	outFile << 1;
	outFile.close ();
}

VldCmdCtrl::command VldCmdCtrl::convertToCommand (int indx) {
	command cmd;
	switch (indx) {
	case  0:	cmd = COSTOM;	break;
	case  1:	cmd = FORCE;	break;
	case  2:	cmd = EXACT;	break;
	case  3:	cmd = SIMILAR;	break;
	case  4:	cmd = EACH;		break;
	case  5:	cmd = COMMAND;	break;
	case  6:	cmd = TIME;		break;
	case  7:	cmd = DATE;		break;
	case  8:	cmd = FROM;		break;
	case  9:	cmd = TO;		break;
	case 10:	cmd = NAME;		break;
	case 11:	cmd = VENUE;	break;
	case 12:	cmd = NOTE;		break;
	case 13:	cmd = ALERT;	break;
	case 14:	cmd = REPEAT;	break;
	case 15:	cmd = PRIORITY;	break;
	case 16:	cmd = HIGH;		break;
	case 17:	cmd = IMPT;		break;
	case 18:	cmd = NOMAL;	break;
	case 19:	cmd = DAY;		break;
	case 20:	cmd = WEEK;		break;
	case 21:	cmd = MONTH;	break;
	case 22:	cmd = ADD;		break;
	case 23:	cmd = EDIT;		break;
	case 24:	cmd = DELETE;	break;
	case 25:	cmd = TABLE;	break;
	case 26:	cmd = VIEW;		break;
	case 27:	cmd = REMINDER;	break;
	case 28:	cmd = NEXT;		break;
	case 29:	cmd = PREVIOUS;	break;
	case 30:	cmd = FIRST;	break;
	case 31:	cmd = LAST;		break;
	case 32:	cmd = UNDO;		break;
	case 33:	cmd = REDO;		break;
	case 34:	cmd = HELP;		break;
	case 35:	cmd = SORT;		break;
	case 36:	cmd = SEARCH;	break;
	case 37:	cmd = CLEAR;	break;
	case 38:	cmd = RESET;	break;
	case 39:	cmd = EXIT;		break;
	default:	cmd = VOID;		break;
	}

	return cmd;
}


bool VldCmdCtrl::search_vldCmd (string& str, vector<cmd_pair>::iterator iter) {
	bool found = false;
	vector<cmd_pair>::iterator endIter = _validCmd.end ();
	for (iter = _validCmd.begin (); !found && iter != endIter; iter++)
		if (str == iter->str_cmd)
			found = true;
	return found;
}

bool VldCmdCtrl::search_vldCmd (string str, command& cmd) {
	int size = _validCmd.size ();
	bool found = false;
	cmd = VOID;
	for (int i = standAloneCmdEndPos; !found && i < size; i++) {
		if (str == _validCmd[i].str_cmd) {
			found = true;
			str = _validCmd[i].enum_cmd;
		}
	}
	return found;
}

bool VldCmdCtrl::search_standAloneCmd (string str, command& cmd) {
	bool found = false;
	cmd = VOID;
	for (int i = 0; !found && i < standAloneCmdEndPos; i++) {
		if (str == _validCmd[i].str_cmd) {
			found = true;
			str = _validCmd[i].enum_cmd;
		}
	}
	return found;
}

bool VldCmdCtrl::checkIfVldExtension (command ext, command cmd) {
	bool vldExt = false;

	switch (ext) {
	case COSTOM:
		if (cmd == VIEW || cmd == SEARCH)
			vldExt = true;
		break;
	case FORCE:
		if (cmd == ADD || cmd == EDIT)
			vldExt = true;
		break;
	case EXACT:
	case SIMILAR:
	case EACH:
		if (cmd == SEARCH)
			vldExt = true;
		break;
	case COMMAND:
		if (cmd == HELP || cmd == RESET)
			vldExt = true;
		break;
	default:
		vldExt = true;
		break;
	}

	return vldExt;
}

string VldCmdCtrl::convertToString (command cmd) {
	string str;
	switch (cmd) {
	case (COSTOM):		str = "COSTOM";		break;
	case (FORCE):		str = "FORCE";		break;
	case (EXACT):		str = "EXACT";		break;
	case (SIMILAR):		str = "SIMILAR";	break;
	case (EACH):		str = "EACH";		break;
	case (COMMAND):		str = "COMMAND";	break;
	case (TIME):		str = "TIME";		break;
	case (DATE):		str = "DATE";		break;
	case (FROM):		str = "FROM";		break;
	case (TO):			str = "TO";			break;
	case (NAME):		str = "NAME";		break;
	case (VENUE):		str = "VENUE";		break;
	case (NOTE):		str = "NOTE";		break;
	case (ALERT):		str = "ALERT";		break;
	case (REPEAT):		str = "REPEAT";		break;
	case (PRIORITY):	str = "PRIORITY";	break;
	case (HIGH):		str = "HIGH";		break;
	case (IMPT):		str = "IMPT";		break;
	case (NOMAL):		str = "NOMAL";		break;
	case (DAY):			str = "DAY";		break;
	case (WEEK):		str = "WEEK";		break;
	case (MONTH):		str = "MONTH";		break;
	case (ADD):			str = "ADD";		break;
	case (EDIT):		str = "EDIT";		break;
	case (DELETE):		str = "DELETE";		break;
	case (TABLE):		str = "TABLE";		break;
	case (VIEW):		str = "VIEW";		break;
	case (REMINDER):	str = "REMINDER";	break;
	case (NEXT):		str = "NEXT";		break;
	case (PREVIOUS):	str = "PREVIOUS";	break;
	case (FIRST):		str = "FIRST";		break;
	case (LAST):		str = "LAST";		break;
	case (UNDO):		str = "UNDO";		break;
	case (REDO):		str = "REDO";		break;
	case (HELP):		str = "HELP";		break;
	case (SORT):		str = "SORT";		break;
	case (SEARCH):		str = "SEARCH";		break;
	case (CLEAR):		str = "CLEAR";		break;
	case (RESET):		str = "RESET";		break;
	case (EXIT):		str = "EXIT";		break;
	default:			str = "VOID";		break;
	}
	return str;
}