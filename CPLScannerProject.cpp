#include <iostream>
#include <fstream>
#include <string>
#include "LanguageProcessor.h"
using namespace std;

int main()
{	
	LanguageProcessor languageProcessor;
	cout<< "Enter a file name: ,default is BASIC_Input_File_1.bas"<<endl;
	string defaultname = "BASIC_Input_File_1.bas";
	string fname;
	cin>>fname;
	languageProcessor.processFile("BASIC_Input_File_1.bas");
	return 0;
}


