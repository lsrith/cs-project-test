#include "CmdControl.h"
#include <iostream>
using namespace std;

int main () {
	string userInput;
	CmdControl cmdControl;

	while (true) {
		getline (cin, userInput);
		cmdControl.updateInput (userInput);
		cout << cmdControl.executeCmd () << endl;
	}

	return 0;
}