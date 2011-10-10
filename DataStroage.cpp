#include "DataStorage.h"
#include "Task.h"
#include "Time.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <algorithm>

using namespace std;

DataStorage::DataStorage(string storageFile)
{
        _storageFile = storageFile;

        ifstream inputFile;
        inputFile.open (_storageFile);
/*
        stringstream strStream;
        string str;
        _largestIndex = 0;
        getline (inputFile, str);
        strStream << str;
        strStream >> _largestIndex;
*/
        _largestIndex = 0;
        inputFile >> _largestIndex;
}

void DataStorage::save(list<Task> toDoList)
{
        list<Task> templist;
        list<Task> todolist;
        Task obj;
        Time tm;
        TimePeriod tp;
        Time::date_t date;
        list<Task>::iterator iter = toDoList.begin();
        list<Task>::iterator ptr = toDoList.begin();
        list<Task>::iterator ptr1 = toDoList.begin();
        string task=" ";
        ofstream writefile (_storageFile);
        int ctr=0;

        while(iter != toDoList.end())
        {
                task = iter->stringConvert();
                todolist.push_back(task);
                iter++;
        }

        templist = sort_by_date(toDoList);
        
        
}       
                
//use bubble to sort the task by date and return a sorted list of task
list<Task> DataStorage::sort_by_date(list<Task> toDoList)
{
        Time tm;
        list<Task>::iterator iter = toDoList.begin();
        list<Task>::iterator ptr = toDoList.begin();
        list<Task> templist;
        Time::date_t date, date1;
        int i=-1;

        for(iter = toDoList.begin(); iter != toDoList.end(); iter++)
        {
                i++;
                for(ptr = toDoList.begin()+1+i; ptr != toDoList.end(); ptr++)
                {
                        //check whether the task is one-day type or have certain timeperiod
                        if(iter->get_time() == tm)
                        {
                                if(ptr->get_time() == tm)
                                {
                                        date = iter->get_period().get_start_time().get_date();
                                        date1 = ptr->get_period().get_start_time().get_date();
                                        if(date > date1)
                                        {
                                                swap_element(*iter, *ptr);
                                        }
                                }
                                else
                                {
                                        date= iter->get_period().get_start_time().get_date();
                                        date1 = ptr->get_time().get_date();
                                        if(date > date1)
                                        {
                                                swap_element(*iter, *ptr);
                                        }
                                        
                                }       
                        }
                        else if(ptr->get_time() == tm)
                        {
                                date = iter->get_time().get_date();
                                date1 = ptr->get_period().get_start_time().get_date();
                                if(date > date1)
                                {
                                        swap_element(*iter, *ptr);
                                }
                        }
                        else
                        {
                                date = iter->get_time().get_date();
                                date1 = ptr->get_time().get_date();
                                if(date > date1)
                                {
                                        swap_element(*iter, *ptr);
                                }
                        }
                }
        }
        return templist = toDoList;
}

//swapping algorithm
void DataStorage::swap_element(list<Task> *ptr, list<Task> *ptr1)
{
        list<Task> *temp;
        *temp = *ptr;
        *ptr = *ptr1;
        *ptr1 = *temp;
}

//write the sorted list of task to a file, no idea how to carry on :( 
void DataStorage::write_to_file(list<Task> todolist)
{
        list<Task>::iterator iter = todolist.begin();
        list<Task>::iterator ptr = todolist.begin();
        typedef unsigned int date_t; 
        Time tm;
        ofstream writeFile(_storageFile);

        for(iter = todolist.begin(); iter != todolist.end(); iter++)
        {
                for(ptr = todolist.begin()+1; ptr != todolist.end(); ptr++)
                {
                        if(iter->get_time() == tm)
                        {
                                if(ptr->get_time() == tm)
                                {
                                        date_t date = iter->get_period().get_start_time().get_date();
                                        date_t date1 = ptr->get_period().get_start_time().get_date();
                                        if(date != date1)
                                        {
                                                writeFile << date << endl;
                                                writeFile << iter->stringConvert() << endl;
                                                writeFile << date1 << endl;
                                                writeFile << ptr->stringConvert() << endl;
                                        }
                                        else
                                        {
                                                writeFile << date << endl;
                                                writeFile << iter->stringConvert() << endl;
                                                writeFile << ptr->stringConvert() << endl;
                                        }
                                                
                                                
                                }
                                else
                                {
                                        date_t date2 = iter->get_period().get_start_time().get_date();
                                        date_t date3 = ptr->get_time().get_date();
                                        if(date2 != date3)
                                        {
                                                writeFile << date2 << endl;
                                                writeFile << iter->stringConvert() << endl;
                                                writeFile << date3 << endl;
                                                writeFile << ptr->stringConvert() << endl;
                                        }
                                        else
                                        {
                                                writeFile << date2 << endl;
                                                writeFile << iter->stringConvert() << endl;
                                                writeFile << ptr->stringConvert() << endl;
                                        }
                                        
                                }       
                        }
                        else if(ptr->get_time() == tm)
                        {
                                date_t date4 = iter->get_time().get_date();
                                date_t date5 = ptr->get_period().get_start_time().get_date();
                                if(date4 != date5)
                                {
                                        writeFile << date4 << endl;
                                        writeFile << iter->stringConvert() << endl;
                                        writeFile << date5 << endl;
                                        writeFile << ptr->stringConvert() << endl;
                                }
                                else
                                {       
                                        writeFile << date4 << endl;
                                        writeFile << iter->stringConvert() << endl;
                                        writeFile << ptr->stringConvert() << endl;
                                }
                        }
                        else
                        {
                                date_t date6 = iter->get_time().get_date();
                                date_t date7 = ptr->get_time().get_date();
                                if(date6 != date7)
                                {
                                        writeFile << date6 << endl;
                                        writeFile << iter->stringConvert() << endl;
                                        writeFile << date7 << endl;
                                        writeFile << ptr->stringConvert() << endl;
                                }
                                else
                                {
                                        writeFile << date6 << endl;
                                        writeFile << iter->stringConvert() << endl;
                                        writeFile << ptr->stringConvert() << endl;
                                }
                        }
                
                }
        }
 
}

/*      list<Template> sortedList;
        Template obj;
        Time tm;
        TimePeriod tp;
        typedef unsigned int date_t; 
        date_t date;
        list<Template>::iterator iter;
        list<Template>:: iterator ptr;
        iter=toDoList.begin();
        string task;
        ifstream readFile(_storageFile);
        ofstream writeFile(_storageFile);

        ptr=iter;
        for(iter = toDoList.begin(); iter != toDoList.end(); iter++)
        {
                task = iter -> stringConvert();
                if(iter->get_time() == tm)
                {
                        date = iter->get_period().get_start_time().get_date();
                        ptr++;
                        if(ptr->get_time() == tm)
                        {
                                if(date > ptr->get_period().get_start_time().get_date())
                                {

                        }
                }
                else
                        iter++;
                
                if(obj._forceAdd==true)
                {
                        
                }
                writeFile << task;
                obj._index++;  
        */
        
list<Task> DataStorage::load (TimePeriod period)
{
        int largestIndex;
        string str;
        ifstream readFile(_storageFile);

        readFile >> largestIndex;
        while(getline(readFile, str))
        {
                if(period.get_start_time().get_date() == str)

        }
}

/*list<Template> DataStorage::sort_by_date(list<Template> todoList)
{
        typedef unsigned int date_t; 
        map<date_t, list<Template>> sortedList;
        list<Template>::iterator iter;
        iter=todoList.begin();

        for(iter = todoList.begin(); iter != todoList.end(); iter++)
        {
                sortedList[iter->get_time().get_date()] = todoList;
                iter++;
        }


}*/

/*void mergeSort(list<Task> todolist, list<Task> *low_index, list<Task> *high_index, int largest_index)
{
        list<Task>::iterator iter = todolist.begin();
        
        Task *mid = NULL;

        low_index = todolist.front();

        for(iter = todolist.begin(); iter != todolist.end(); iter++)
        {
                if(low_index->get < largest_index)
                {
                        for(int i=0; i < todolist.size(); i++)
                        {
                                mid++;
                        }
                        mergeSort(todolist, 
                        mergeSort(todolist, mid+1,  
                }
        }
}*/

/*while(iter != toDoList.end())
        {
                task = iter -> stringConvert();
        }

        for(iter = toDoList.begin(); iter != toDoList.end(); iter++)
        {
                ptr = iter++;
        
                task = iter -> stringConvert();
                if(iter->get_time() == tm)
                        templist.push_back(iter->get_period().get_start_time().get_date());
                else
                        templist.push_back(iter->get_date());
        }

        templist.sort();

        for(cur = templist.begin(); cur != templist.end(); cur++)
        {
                for(ptr = templist.begin()++; ptr != templist.end(); ptr++)
                {
                
                }
        }*/
