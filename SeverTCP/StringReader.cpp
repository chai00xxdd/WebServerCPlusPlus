#include "StringReader.h"
#include<iostream>
string StringReader::readLine()
{
    string line = "";
    while (index != str.end() && (*index == LF || *index == CF))
        index++;
    while (index != str.end() && *index != LF && *index != CF)
        line += *index++;
    while (index != str.end() && (*index == LF || *index == CF))
        index++;
    return line;
}

string StringReader::readWord()
{
    string word = "";
    while (index != str.end() && (*index == SPACE || *index == LF || *index == CF))
        index++;
    while (index != str.end() && *index != SPACE && *index != LF && *index != CF)
    {
        word += *index++;
    }
    while (index != str.end() && (*index == SPACE || *index == LF || *index == CF))
        index++;
    return word;
}

string StringReader::readRestOfString()
{
    string rest = "";
    while (index != str.end() && (*index == SPACE || *index == LF || *index == CF))
        index++;
    while (index != str.end())
        rest += *index++;
    return rest;
}
