#include "ToDoMngr.h"
#include "TimePeriod.h"
#include "TaskMngr.h"
#include "TableMngr.h"
#include "Template.h"
#include <list>
#include <queue>
#include <string>
using namespace std;

ToDoMngr::ToDoMngr ()
{
	_storageFile;
}