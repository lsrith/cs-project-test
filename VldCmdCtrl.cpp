#include "VldCmdCtrl.h"
#include <string>
#include <vector>
#include <fstream>
using namespace std;

string VldCmdCtrl::LOST_FILE = "unable to find a file: ";
string VldCmdCtrl::LOAD_DFLT_CMD = "Loaded default command!";

VldCmdCtrl::VldCmdCtrl () {
	_validCmdFile = "vldCmd2.txt";
	_dfltCmdFile = "dfltCmd2.txt";

	try {
		load_vldCmdList ();
	} catch (string message) {
		throw message;
	}
}

void VldCmdCtrl::load_vldCmdList () {
	try {
		_validCmd = load_vldCmdList (_validCmdFile);
		if (_validCmd.empty ()) {
			try {
				reset ();
			} catch (string Message) {
				throw (Message);
			}
			
			get_vldCmdList ();
		}
	} catch (string message) {
		try {
			reset ();
		} catch (string Message) {
			throw (Message);
		}
			
		get_vldCmdList ();
	}
}

vector<VldCmdCtrl::cmd_pair> VldCmdCtrl::get_vldCmdList () {
	return _validCmd;
}

vector<VldCmdCtrl::cmd_pair> VldCmdCtrl::load_vldCmdList (string cmdFile) {
	vector<cmd_pair> validCmd;
	ifstream inFile (cmdFile);
	if (inFile.is_open ()) {
		cmd_pair cmd;
		int cmdIndx;
		while (inFile >> cmdIndx && inFile >> cmd.str_cmd) {
			cmd.enum_cmd = convertToCommand (cmdIndx);
			validCmd.push_back (cmd);
		}
	} else
		throw (LOST_FILE + cmdFile);
	
	return validCmd;
}

void VldCmdCtrl::update_vldCmdList (vector<cmd_pair>& validCmd) {
	ofstream outFile (_validCmdFile);
	int size = validCmd.size ();
	for (int i = 0; i < size; i++)
		outFile << validCmd[i].enum_cmd << " " << validCmd[i].str_cmd << endl;
	outFile.close ();
}

void VldCmdCtrl::reset () {
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

VldCmdCtrl::command VldCmdCtrl::convertToCommand (int indx) {
	command cmd;
	switch (indx) {
		case 0:		cmd = CCOSTOM;		break;
		case 1:		cmd = CFORCE;		break;
		case 2:		cmd = CEXACT;		break;
		case 3:		cmd = CSIMILAR;		break;
		case 4:		cmd = CEACH;		break;
		case 5:		cmd = CCOMMAND;		break;
		case 6:		cmd = CTIME;		break;
		case 7:		cmd = CDATE;		break;
		case 8:		cmd = CFROM;		break;
		case 9:		cmd = CTO;			break;
		case 10:	cmd = CNAME;		break;
		case 11:	cmd = CVENUE;		break;
		case 12:	cmd = CNOTE;		break;
		case 13:	cmd = CALERT;		break;
		case 14:	cmd = CREPEAT;		break;
		case 16:	cmd = CHOUR;		break;
		case 17:	cmd = CFORTNIGHT;	break;
		case 18:	cmd = CYEAR;		break;
		case 19:	cmd = CDAY;			break;
		case 20:	cmd = CWEEK;		break;
		case 21:	cmd = CMONTH;		break;
		case 22:	cmd = CNOW;			break;
		case 23:	cmd = CTODAY;		break;
		case 24:	cmd = CTMR;			break;
		case 25:	cmd = CADD;			break;
		case 26:	cmd = CEDIT;		break;
		case 27:	cmd = CDELETE;		break;
		case 28:	cmd = CTABLE;		break;
		case 29:	cmd = CVIEW;		break;
		case 30:	cmd = CREMINDER;	break;
		case 31:	cmd = CNEXT;		break;
		case 32:	cmd = CPREVIOUS;	break;
		case 33:	cmd = CFIRST;		break;
		case 34:	cmd = CLAST;		break;
		case 35:	cmd = CUNDO;		break;
		case 36:	cmd = CREDO;		break;
		case 37:	cmd = CHELP;		break;
		case 38:	cmd = CSORT;		break;
		case 39:	cmd = CSEARCH;		break;
		case 40:	cmd = CCLEAR;		break;
		case 41:	cmd = CUSER;		break;
		case 42:	cmd = CEXIT;		break;
		case 43:	cmd = CDISCARD;		break;
		case 44:	cmd = CREPLACE;		break;
		case 45:	cmd = CINSERT;		break;
		case 46:	cmd = CLEFT;		break;
		case 47:	cmd = CRIGHT;		break;
		default:	cmd = CVOID;		break;
	}

	return cmd;
}

VldCmdCtrl::command VldCmdCtrl::convertToCommand (string str) {
	command cmd;
	bool found = search_vldCmd (str, cmd);
	return cmd;
}

bool VldCmdCtrl::search_vldCmd (string str, command& cmd) {
	int size = _validCmd.size ();
	bool found = false;
	cmd = CVOID;
	for (int i = 0; !found && i < size; i++) {
		if (str == _validCmd[i].str_cmd) {
			found = true;
			cmd = _validCmd[i].enum_cmd;
		}
	}
	return found;
}

string VldCmdCtrl::convertToString (command cmd) {
	string str;
	switch (cmd) {
	case (CCOSTOM):		str = "costom";		break;
	case (CFORCE):		str = "force";		break;
	case (CEXACT):		str = "exact";		break;
	case (CSIMILAR):	str = "similar";	break;
	case (CEACH):		str = "each";		break;
	case (CCOMMAND):	str = "command";	break;
	case (CTIME):		str = "time";		break;
	case (CDATE):		str = "date";		break;
	case (CFROM):		str = "from";		break;
	case (CTO):			str = "to";			break;
	case (CNAME):		str = "name";		break;
	case (CVENUE):		str = "venue";		break;
	case (CNOTE):		str = "note";		break;
	case (CALERT):		str = "alert";		break;
	case (CREPEAT):		str = "repeat";		break;
	case (CPROMPT):		str = "prompt";		break;
	case (CHOUR):		str = "hour";		break;
	case (CFORTNIGHT):	str = "fortnight";	break;
	case (CYEAR):		str = "year";		break;
	case (CDAY):		str = "day";		break;
	case (CWEEK):		str = "week";		break;
	case (CMONTH):		str = "month";		break;
	case (CNOW):		str = "now";		break;
	case (CTODAY):		str = "today";		break;
	case (CTMR):		str = "tomorrow";	break;
	case (CADD):		str = "add";		break;
	case (CEDIT):		str = "edit";		break;
	case (CDELETE):		str = "delete";		break;
	case (CTABLE):		str = "table";		break;
	case (CVIEW):		str = "view";		break;
	case (CREMINDER):	str = "reminder";	break;
	case (CNEXT):		str = "next";		break;
	case (CPREVIOUS):	str = "previous";	break;
	case (CFIRST):		str = "first";		break;
	case (CLAST):		str = "last";		break;
	case (CUNDO):		str = "undo";		break;
	case (CREDO):		str = "redo";		break;
	case (CHELP):		str = "help";		break;
	case (CSORT):		str = "sort";		break;
	case (CSEARCH):		str = "search";		break;
	case (CCLEAR):		str = "clear";		break;
	case (CUSER):		str = "user";		break;
	case (CEXIT):		str = "exit";		break;
	case (CDISCARD):	str = "discard";	break;
	case (CREPLACE):	str = "replace";	break;
	case (CINSERT):		str = "insert";		break;
	case (CLEFT):		str = "left";		break;
	case (CRIGHT):		str = "right";		break;
	default:			str = "void";		break;
	}
	return str;
}