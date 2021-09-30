#pragma once
#include<iostream>
#include<string>
using namespace std;
class Date
{
public:
	//return a string with the GMT time
	static string getGMT_Time();
	//the function get localtime and convert it to GMT time string
	static string getGMT_Time(time_t* localtime);
};

