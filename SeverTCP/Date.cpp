#define _CRT_SECURE_NO_WARNINGS
#include "Date.h"
#include<ctime>
string Date::getGMT_Time()
{
    
    time_t now = time(0);
    tm* gmtm = gmtime(&now);
    char date[200];
    strftime(date,200, "%a, %d %b %Y %T GMT ", gmtm);
    return string(date);
}

string Date::getGMT_Time(time_t* localtime)
{
    tm* gmtm = gmtime(localtime);
    char date[200];
    strftime(date, 200, "%a, %d %b %Y %T GMT ", gmtm);
    return date;
}
