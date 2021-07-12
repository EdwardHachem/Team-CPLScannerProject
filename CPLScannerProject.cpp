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
	getline(cin, fname);
	ifstream ifile;
	ifile.open(fname);
	if(ifile){
		ifile.close();
		languageProcessor.processFile(fname);
	}
	else
	{
		cout<<"file does not exist, using default"<<endl;
		ifile.close();
		languageProcessor.processFile(defaultname);
	}
	
	return 0;
}


