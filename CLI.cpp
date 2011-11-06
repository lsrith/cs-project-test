#include "VldCmdCtrl.h"
#include "ExecuteCmd.h"
#include "AccCtrl.h"
#include "Logging.h"
#include "CmdTextChange.h"
#include <iostream>
#include <Windows.h>
#include <direct.h>
using namespace std;

inline VldCmdCtrl::command getFirstCmd (string*, VldCmdCtrl*);
inline void eraseFirstWord (string*);
inline void setConsoleWindowConfig ();

int main () {
	Logging log;
	log.log ("main");
	log.start ("main");

	setConsoleWindowConfig ();

	string output, input, newInput;
	log.call ("ToDoMngr");
	ToDoMngr toDoMngr;
	log.end ();
	ToDoMngr* toDo = &toDoMngr;

	log.call ("VldCmdCtrl");
	try {
		VldCmdCtrl testLoading;
	} catch (string xcpt) {
		cout << xcpt << endl;
		cout << "press Enter to continue..." << endl;
		getline (cin, xcpt);
		return 0;
	}
	log.end ();

	CmdTextChange console;
	console.write ("                                            TASKCAL\n");
	console.write ("                                            WELCOME\n\n");

	VldCmdCtrl cmdCtrl;
	bool prompt;
	VldCmdCtrl::command cmd = VldCmdCtrl::CUSER;

	Merge merge (cmdCtrl.get_vldCmdList ());
	ExecuteCmd* exeCmd = NULL;
	AccCtrl userAcc (toDo);
	Add add (cmdCtrl.get_vldCmdList (), toDo);
	Edit edit (cmdCtrl.get_vldCmdList (), toDo);
	View view (cmdCtrl.get_vldCmdList (), toDo);
	Delete erase (cmdCtrl.get_vldCmdList (), toDo);
	Help help;

	input = VldCmdCtrl::convertToString (VldCmdCtrl::CUSER);

	log.loop ("main");
	do {
		output.erase (0, string::npos);
		console.write (">");
		console.write ("> ");
		newInput = console.read ();

		merge.updateInput (input, newInput);
		merge.execute ();
		input = merge.result ();
		prompt = false;

		log.loop ("sub");
		while (!input.empty () && !prompt) {
			cmd = getFirstCmd (&input, &cmdCtrl);
			
			switch (cmd) {
			case VldCmdCtrl::CVIEW:
			case VldCmdCtrl::CNEXT:
			case VldCmdCtrl::CPREVIOUS:
			case VldCmdCtrl::CFIRST:
			case VldCmdCtrl::CLAST:
				break;
			default:
				view.deactivateTraverse ();
				break;
			}

			prompt = true;
			switch (cmd) {
			case VldCmdCtrl::CCLEAR:
				toDo->clear ();
				eraseFirstWord (&input);
				break;
			case VldCmdCtrl::CUNDO:
				toDo->undo ();
				eraseFirstWord (&input);
				break;
			case VldCmdCtrl::CREDO:
				toDo->redo ();
				eraseFirstWord (&input);
				break;
			case VldCmdCtrl::CREMINDER:
				output += toDo->reminder ();
				eraseFirstWord (&input);
				break;
			default:
				prompt = false;
				break;
			}
			if (prompt) {
				prompt = false;
				continue;
			}

			exeCmd = NULL;
			switch (cmd) {
			case VldCmdCtrl::CADD:
				exeCmd = &add;
				break;
			case VldCmdCtrl::CEDIT:		
				exeCmd = &edit;		
				break;
			case VldCmdCtrl::CDELETE:	
				exeCmd = &erase;	
				break;
			case VldCmdCtrl::CVIEW:
			case VldCmdCtrl::CNEXT:
			case VldCmdCtrl::CPREVIOUS:
			case VldCmdCtrl::CFIRST:
			case VldCmdCtrl::CLAST:
				exeCmd = &view;
				break;
			case VldCmdCtrl::CHELP:
				exeCmd = &help;
				break;
			case VldCmdCtrl::CUSER:
				exeCmd = &userAcc;
				break;
			case VldCmdCtrl::CEXIT:
				prompt = true;
				break;
			default:
				eraseFirstWord (&input);
				exeCmd = NULL;
				break;
			}

			if (exeCmd != NULL) {
				exeCmd->updateInput (input);
				if (!exeCmd->execute ()) {
					if (cmd == VldCmdCtrl::CUSER)
						output.erase ();
					else
						output += "\n<< " + exeCmd->get_input ();
					prompt = true;
				}
				output += "\n" + exeCmd->result ();
				input = exeCmd->get_input ();
			}
		}
		log.end ();

		console.write (output + "\n");
	} while (cmd != VldCmdCtrl::CEXIT);
	log.end ();
	log.end ();
	return 0;
}

VldCmdCtrl::command getFirstCmd (string* input, VldCmdCtrl* cmdCtrl) {
	unsigned int pos = input->find_first_of (' ', 0);
	string firstWord = input->substr (0, pos);
	return cmdCtrl->convertToCommand (firstWord);
}

void eraseFirstWord (string* input) {
	unsigned int pos = input->find_first_of (' ', 0);
	if (pos == string::npos)
		input->erase (0, pos);
	else
		input->erase (0, pos + 1);
}

void setConsoleWindowConfig () {
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD NewSBSize = {CmdTextChange::MAX_WIDTH, CmdTextChange::MAX_HEIGHT};
	SMALL_RECT DisplayArea = {CmdTextChange::MAX_WIDTH, 50, 0, 0};
	SetConsoleTitle ("TaskCal");
    SetConsoleScreenBufferSize(hOut, NewSBSize);
    SetConsoleWindowInfo(hOut, TRUE, &DisplayArea);
	system ("cls");
}