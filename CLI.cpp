#include "VldCmdCtrl.h"
#include "ExecuteCmd.h"
#include "AccCtrl.h"
#include "Logging.h"
#include <iostream>
using namespace std;

inline VldCmdCtrl::command getFirstCmd (string*, VldCmdCtrl*);
inline void eraseFirstWord (string*);

int main () {
	const string WELCOME = "\tTASKCAL\n\n";
	cout << WELCOME << endl;

	string output, input, newInput;
	ToDoMngr toDoMngr;
	ToDoMngr* toDo = &toDoMngr;

	//logging in the program
	input = VldCmdCtrl::convertToString (VldCmdCtrl::CUSER);
	AccCtrl userAcc (toDo);
	while (!userAcc.execute ()) {
		input = userAcc.get_input ();
		output = userAcc.result ();
		cout << output;
		getline (cin, newInput);
		input += newInput;
	}

	VldCmdCtrl cmdCtrl;
	input.erase (0, string::npos);
	output.erase (0, string::npos);
	bool prompt;
	VldCmdCtrl::command cmd;
//	Merge merge (cmdCtrl.get_vldCmdList ());
	ExecuteCmd* exeCmd = NULL;
	Add add (cmdCtrl.get_vldCmdList (), toDo);
	Edit edit (cmdCtrl.get_vldCmdList (), toDo);
	View view (cmdCtrl.get_vldCmdList (), toDo);
	Delete erase (cmdCtrl.get_vldCmdList (), toDo);
	do {
		cout << ">> ";
		getline (cin, newInput);

//		merge.update (input, newInput);
//		input = merge.execute ();
		input = newInput;
		prompt = false;
		while (!input.empty () && !prompt) {
			cmd = getFirstCmd (&input, &cmdCtrl);
			
			prompt = true;
			switch (cmd) {
			case VldCmdCtrl::CNEXT:
				output += view.next ();
				eraseFirstWord (&input);
				break;
			case VldCmdCtrl::CPREVIOUS:
				output += view.prev ();
				eraseFirstWord (&input);
				break;
			case VldCmdCtrl::CFIRST:
				output += view.first ();
				eraseFirstWord (&input);
				break;
			case VldCmdCtrl::CLAST:
				output += view.last ();
				eraseFirstWord (&input);
				break;
			default:
				view.deactivateTraverse ();
				prompt = false;
				break;
			}
			if (prompt) {
				prompt = false;
				continue;
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
				exeCmd = &view;
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
				if (exeCmd->execute ())
					output += exeCmd->result ();
				else
					prompt = true;

				input = exeCmd->get_input ();
			}
		}
		cout << output << endl;
		output.erase (0, string::npos);
	} while (cmd != VldCmdCtrl::CEXIT);
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