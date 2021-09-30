#pragma once
#include<string>
#include<vector>
#include"Http.h"
using namespace std;
class HttpRequest
{
private:
	vector<Http::Header> headers;
	string method="";
	string url="";
	string lang="";
	string content = "";
	string request="";
public:
	//the constructor get a string request and extract all the parmeters of the request
	HttpRequest(const string& request);
	//getters setters
	const string& getMethod()const { return method; }
	const string& getUrl()const { return url; }
	const string& getReq()const { return request; }
	const string& getBody()const { return content; }
	const string& getLanguage()const { return lang==""?DEFAULT_LANGUAGE:lang; }
	const vector<Http::Header>& getHeaders()const { return headers; }
	const Http::Header& getHeader(const string& header)const;
	static void setDefaultLanguage(const string& lang) { DEFAULT_LANGUAGE = lang; }
	static string DEFAULT_LANGUAGE;

};

