#pragma once
#include<string>
using namespace std;
class StringReader
{
private:
	const string& str;
	string::const_iterator index;
	static constexpr char LF = '\n', SPACE = ' ', CF = '\r';
public:
	StringReader(const string& str) :str(str), index(str.begin()) {}
	//the function read the firstline in the string and return it
	string readLine();
	//the function read the first word in the string and return it
	string readWord();
	//the function return the reamining string with out backspaces in the begining
	string readRestOfString();
	bool eof()const { return index == str.end(); }

};

