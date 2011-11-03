#include "ExecuteCmd.h"
#include <iostream>
#include <iomanip>
using namespace std;

int main () {
	try {
		VldCmdCtrl cmdCtrl;
		Merge merge (cmdCtrl.get_vldCmdList ());
		string input, newInput, testOutput, expOutput;
		int compare;
	
		input = "hello how are you? I am fine.";
		newInput = "I am fine as well";
		expOutput = "hello how are you? I am fine. I am fine as well";
		merge.updateInput (input, newInput);
		merge.execute ();
		testOutput = merge.result ();
		compare = expOutput.compare (testOutput);
		cout << compare << endl;
		if (compare) {
			cout << "exp:: " << expOutput << endl;
			cout << "try:: " << testOutput << endl;
		}
		cout << endl;

		input = "hello how are you? I am fine. " + CmdTrans::BREAK;
		newInput = "I am fine as well";
		expOutput = "hello how are you? I am fine. I am fine as well";
		merge.updateInput (input, newInput);
		merge.execute ();
		testOutput = merge.result ();
		compare = expOutput.compare (testOutput);
		cout << compare << endl;
		if (compare) {
			cout << "exp:: " << expOutput << endl;
			cout << "try:: " << testOutput << endl;
		}
		cout << endl;

		input = "hello how are you? " + CmdTrans::BREAK + " I am fine.";
		newInput = "I am fine as well";
		expOutput = "hello how are you? I am fine as well I am fine.";
		merge.updateInput (input, newInput);
		merge.execute ();
		testOutput = merge.result ();
		compare = expOutput.compare (testOutput);
		cout << compare << endl;
		if (compare) {
			cout << "exp:: " << expOutput << endl;
			cout << "try:: " << testOutput << endl;
		}
		cout << endl;

		input = CmdTrans::BREAK + " hello how are you? I am fine.";
		newInput = "I am fine as well";
		expOutput = "I am fine as well hello how are you? I am fine.";
		merge.updateInput (input, newInput);
		merge.execute ();
		testOutput = merge.result ();
		compare = expOutput.compare (testOutput);
		cout << compare << endl;
		if (compare) {
			cout << "exp:: " << expOutput << endl;
			cout << "try:: " << testOutput << endl;
		}
		cout << endl;
	
		input = "hello how are you? I am fine. " + CmdTrans::BREAK;
		newInput = "discard -l I am fine as well";
		expOutput = "hello how are you? I am";
		merge.updateInput (input, newInput);
		merge.execute ();
		testOutput = merge.result ();
		compare = expOutput.compare (testOutput);
		cout << compare << endl;
		if (compare) {
			cout << "exp:: " << expOutput << endl;
			cout << "try:: " << testOutput << endl;
		}
		cout << endl;

		input = "hello how are you? I am fine. " + CmdTrans::BREAK;
		newInput = "discard -r";
		expOutput = "hello how are you? I am fine.";
		merge.updateInput (input, newInput);
		merge.execute ();
		testOutput = merge.result ();
		compare = expOutput.compare (testOutput);
		cout << compare << endl;
		if (compare) {
			cout << "exp:: " << expOutput << endl;
			cout << "try:: " << testOutput << endl;
		}
		cout << endl;

		input = "hello how are you? " + CmdTrans::BREAK + " I am fine.";
		newInput = "discard -r I am fine as well";
		expOutput = "hello how are you? am fine.";
		merge.updateInput (input, newInput);
		merge.execute ();
		testOutput = merge.result ();
		compare = expOutput.compare (testOutput);
		cout << compare << endl;
		if (compare) {
			cout << "exp:: " << expOutput << endl;
			cout << "try:: " << testOutput << endl;
		}
		cout << endl;

		input = "hello how are you? " + CmdTrans::BREAK + " I am fine.";
		newInput = "discard -l";
		expOutput = "hello how are I am fine.";
		merge.updateInput (input, newInput);
		merge.execute ();
		testOutput = merge.result ();
		compare = expOutput.compare (testOutput);
		cout << compare << endl;
		if (compare) {
			cout << "exp:: " << expOutput << endl;
			cout << "try:: " << testOutput << endl;
		}
		cout << endl;

		input = CmdTrans::BREAK + " hello how are you? I am fine.";
		newInput = "discard -l";
		expOutput = "hello how are you? I am fine.";
		merge.updateInput (input, newInput);
		merge.execute ();
		testOutput = merge.result ();
		compare = expOutput.compare (testOutput);
		cout << compare << endl;
		if (compare) {
			cout << "exp:: " << expOutput << endl;
			cout << "try:: " << testOutput << endl;
		}
		cout << endl;
	
		input = "hello how are you? I am fine. " + CmdTrans::BREAK;
		newInput = "replace -l I am fine as well";
		expOutput = "hello how are you? I am I am fine as well";
		merge.updateInput (input, newInput);
		merge.execute ();
		testOutput = merge.result ();
		compare = expOutput.compare (testOutput);
		cout << compare << endl;
		if (compare) {
			cout << "exp:: " << expOutput << endl;
			cout << "try:: " << testOutput << endl;
		}
		cout << endl;

		input = "hello how are you? I am fine. " + CmdTrans::BREAK;
		newInput = "replace -r I am fine as well";
		expOutput = "hello how are you? I am fine. I am fine as well";
		merge.updateInput (input, newInput);
		merge.execute ();
		testOutput = merge.result ();
		compare = expOutput.compare (testOutput);
		cout << compare << endl;
		if (compare) {
			cout << "exp:: " << expOutput << endl;
			cout << "try:: " << testOutput << endl;
		}
		cout << endl;

		input = "hello how are you? " + CmdTrans::BREAK + " I am fine.";
		newInput = "replace -r I am fine as well";
		expOutput = "hello how are you? I am fine as well am fine.";
		merge.updateInput (input, newInput);
		merge.execute ();
		testOutput = merge.result ();
		compare = expOutput.compare (testOutput);
		cout << compare << endl;
		if (compare) {
			cout << "exp:: " << expOutput << endl;
			cout << "try:: " << testOutput << endl;
		}
		cout << endl;

		input = "hello how are you? " + CmdTrans::BREAK + " I am fine.";
		newInput = "replace -l I am fine as well";
		expOutput = "hello how are I am fine as well I am fine.";
		merge.updateInput (input, newInput);
		merge.execute ();
		testOutput = merge.result ();
		compare = expOutput.compare (testOutput);
		cout << compare << endl;
		if (compare) {
			cout << "exp:: " << expOutput << endl;
			cout << "try:: " << testOutput << endl;
		}
		cout << endl;

		input = CmdTrans::BREAK + " hello how are you? I am fine.";
		newInput = "replace -l I am fine as well";
		expOutput = "I am fine as well hello how are you? I am fine.";
		merge.updateInput (input, newInput);
		merge.execute ();
		testOutput = merge.result ();
		compare = expOutput.compare (testOutput);
		cout << compare << endl;
		if (compare) {
			cout << "exp:: " << expOutput << endl;
			cout << "try:: " << testOutput << endl;
		}
		cout << endl;

		input = "hello how are you? " + CmdTrans::BREAK + " I am fine.";
		newInput = "replace I am fine as well";
		expOutput = "hello how are you? I am fine as well I am fine.";
		merge.updateInput (input, newInput);
		merge.execute ();
		testOutput = merge.result ();
		compare = expOutput.compare (testOutput);
		cout << compare << endl;
		if (compare) {
			cout << "exp:: " << expOutput << endl;
			cout << "try:: " << testOutput << endl;
		}
		cout << endl;

		input = "hello how are you? " + CmdTrans::BREAK + " I am fine.";
		newInput = "discard I am fine as well";
		expOutput = "hello how are you? I am fine.";
		merge.updateInput (input, newInput);
		merge.execute ();
		testOutput = merge.result ();
		compare = expOutput.compare (testOutput);
		cout << compare << endl;
		if (compare) {
			cout << "exp:: " << expOutput << endl;
			cout << "try:: " << testOutput << endl;
		}
		cout << endl;

	} catch (string xcpt) {
		cout << xcpt << endl;
	}

	return 0;
};