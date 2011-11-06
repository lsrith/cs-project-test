#ifndef CMDTXCH_H
#define CMDTXCH_H
#include "CmdTranslator.h"
#include <Windows.h>
#include <list>
using namespace std;

class CmdTextChange: public CmdTrans {
private:
	enum SpCmd {SPCMD = 224, RIGHT = 'M', LEFT = 'K', UP = 'H', DOWN = 'P', DEL = 'S', INS = 'R', UNDO = 26, REDO = 25};
	enum Color {BLACK = 0, GRAY = 7, GREY, BLUE, GREEN, SKY, RED, PINK, YELLOW, WHITE};

	struct chNode {
		short int _ch;
		Color _cl;
	};
public:
	CmdTextChange ();
	~CmdTextChange ();
	string read ();
	string read (string);
	string pswdRead ();
	void write (string);
	static unsigned int MAX_WIDTH;
	static unsigned int MAX_HEIGHT;

private:
	HANDLE hConsole;
	COORD __cursor;
	list<chNode> _chList;
	unsigned int _cursor;
	bool _insertActive;

	void checkWord ();
	void putChar (chNode);
	void putChar (short int);
	void spCmdCheck ();
	void delChar ();
	void bsChar ();
	void insChar (short int);
	void splitString (string&);
	string mergeChar (list<chNode>*);
	list<chNode>::iterator getNode (unsigned int);
	string getCurrWord ();
	void getPrevWords (command&, string&);
	bool ifDataError (command, string);
	void setCurrWord (Color);
	void setPrevWords (Color);
	Color prevWord ();
	Color getCmdColor (command);
	void clear ();
	void buffer ();
	void rewrite ();
	bool moveCursorForward ();
	bool moveCursorBackward ();
	bool moveCursorUp ();
	bool moveCursorDown ();
};

#endif