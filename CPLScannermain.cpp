/*****************************************
* Class: CS 4308 (Concept of Programming Language)/ Section 2 
* Term:  Summer Semester 2021 
* Group Names:  Haley Granger, Edward Hachem, Elisha Ngolemasango 
* Instructor:   Deepa Muralidhar 
* Project:  Deliverable 1 Scanner - C++.
* Title: CPLScannermain.cpp
* ****************************************/
#include <iostream>
#include <fstream>
#include <string>
#include <iterator>
#include <list>
#include "LanguageProcessor.h"
using namespace std;

//this is the function to read the string
list<string> inputf;
string Inputfiles(string fileName)
{
    ifstream fin;
    string temp;
    fin.open(fileName);
    while(!fin.eof())
    {
        getline(fin, temp);
        inputf.push_back(temp);
    }
    fin.close();
}

// this function is to print out the input file select
void printinput()
{
    for (list<string>::iterator iter = inputf.begin(); iter != inputf.end(); iter++)
    cout << *iter << endl;
    cout<<endl;
}

int main()
{	
	LanguageProcessor languageProcessor;
	//languageProcessor.processFile("BASIC_Input_File_2.bas");
	//this will ask the user to enter one of the two
    cout << "Enter the name of the data file\n Basic_Input_File_1 or\nBasic_Input_File_2";
    cin >> fileName;
    cout<<endl;
   Inputfiles(fileName);
   printinput();
	return 0;
}
