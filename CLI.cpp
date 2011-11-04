#include "VldCmdCtrl.h"
#include "ExecuteCmd.h"
#include "AccCtrl.h"
#include "Logging.h"
#include "CmdTextChange.h"
#include <iostream>
#include <direct.h>
using namespace std;

inline VldCmdCtrl::command getFirstCmd (string*, VldCmdCtrl*);
inline void eraseFirstWord (string*);

int main () {
	Logging log;
	log.log ("main");
	log.start ("main");
	const string WELCOME = "\tTASKCAL\n\n";
	cout << WELCOME << endl;

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
		return 0;
	}
	log.end ();

	VldCmdCtrl cmdCtrl;
	bool prompt;
	VldCmdCtrl::command cmd = VldCmdCtrl::CUSER;

	CmdTextChange console;
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
		cout << ">> ";
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
					output += "\n<< " + exeCmd->get_input ();
					prompt = true;
				}
				output += "\n" + exeCmd->result ();
				input = exeCmd->get_input ();
			}
		}
		log.end ();

		cout << output << endl;
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