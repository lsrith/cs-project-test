#ifndef EXECUTECMD_H
#define EXECUTECMD_H
#include "VldCmdCtrl.h"
#include "CmdTranslator.h"
#include "ToDoMngr.h"
#include "Logging.h"
using namespace std;

template <typename data_t>
void append_back (list<data_t>* main, list<data_t> sub);

/**
 * This class is used as an interface to othe execute command key
 *
 * @author LY SAMRITH
 */
class ExecuteCmd {
public:
	ExecuteCmd ();
	~ExecuteCmd ();

	void updateInput (string& input);
	string get_input ();
	//return a string of input that could not be able to access
	//return a string with a break point if execution cannot be done
	virtual bool execute ();
	string result ();

protected:
	string _input;
	string _result;
	ToDoMngr* _toDoMngr;
	void eraseFirstWord ();
};

/**
 * This class is used to execute add command
 *
 * @author LY SAMRITH
 */
class Add: public CmdTrans, public ExecuteCmd {
public:
	Add (vector<cmd_pair>, ToDoMngr*);
	~Add ();
	bool execute ();
	static string MSG_CLASH;

private:
	static string MSG_ADDED;
	void insertBreakPoint ();
};

/**
 * This class is used to execute edit command
 *
 * @author LY SAMRITH
 */
class Edit: public CmdTrans, public ExecuteCmd {
public:
	Edit (vector<cmd_pair>, ToDoMngr*);
	~Edit ();
	bool execute ();

private:
	static string MSG_EDITED;
	void insertBreakPoint ();
};

/**
 * This class is used to execute view command
 *
 * @author LY Samrith
 */
class View: public CmdTrans, public ExecuteCmd {
public:
	View (vector<cmd_pair>, ToDoMngr*);
	~View ();
	bool execute ();
	void deactivateTraverse ();
	void next ();
	void prev ();
	void last ();
	void first ();

private:
	static string MSG_NO_NEXT;
	static string MSG_NO_PREV;
	static string MSG_WRONG_ID;

	int _taskId;
	bool _first;
	bool _last;
	TimePeriod _period;
	Time _time;
	view_t _viewType;
	bool _traverse;
	bool _activeListAccessible;
};

/**
 * This class is used to execute delete command
 *
 * @author LY SAMRITH
 */
class Delete: public CmdTrans, public ExecuteCmd {
public:
	Delete (vector<cmd_pair>, ToDoMngr*);
	~Delete ();
	bool execute ();

private:
	static string MSG_DELETED;
};

/**
 * This class is used to execute table command
 *
 * @author LY SAMRITH
 */
class Table:public CmdTrans, public ExecuteCmd {
public:
	Table (vector<cmd_pair>, ToDoMngr*);
	~Table ();
	bool execute ();

private:
	static string MSG_TABLE_ACTIVATED;
	static string MSG_TABLE_CREATED;
	static string MSG_TABLE_NOT_CREATED;
};

/**
 * This class is used to execute search command
 *
 * @author LY SAMRITH
 */
class Search:public CmdTrans, public ExecuteCmd {
public:
	Search (vector<cmd_pair>, ToDoMngr*);
	~Search ();
	bool execute ();
};

/**
 * This class is used to merge two input consisted of BREAK point
 *
 * @author LY SAMRITH
 */
class Merge: public CmdTrans, public ExecuteCmd {
public:
	Merge (vector<cmd_pair>);
	~Merge ();
	void updateInput (string&, string&);
	bool execute ();

private:
	Logging log;
	string _newInput;
	string _part1;
	string _part2;
	string appendStrings (string&, string&, string&);
	void seperateInput ();
	void discard ();
};

/**
 * This class is used to retrieve help information
 *
 * @author Chew Sin Chen
 */
class Help : public ExecuteCmd {
public:
	bool execute ();

};

/**
 * This class is used to control user account
 *
 * @author Chew Sin Chen
 */
class AccCtrl : public ExecuteCmd {
public:
	AccCtrl (ToDoMngr*);
	~AccCtrl ();
	bool execute ();
	bool ifPassword ();
	bool ifAccCtrl ();

private:
	enum UserStatus {UNEW, UOLD, UNONE};
	UserStatus _userStatus;
	string _username;
	string _password;
	string _retypedPassword;
	bool _accCtrl;
	string convert(string);
};
#endif