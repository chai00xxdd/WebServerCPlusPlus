#pragma once
#include<list>
#include<string>
#include<vector>
#include"Http.h"
using namespace std;
class HttpResponse
{
private:
	list<Http::Header> headers;
	int parsecode=200;
	string parse="ok";
	string body = "";
public:
	HttpResponse() = default;
	HttpResponse(const string& parse, int parsecode, const string& body = "") :parse(parse), parsecode(parsecode) { setBody(body); }
	HttpResponse(const string& parse, int parsecode,const list<Http::Header>&headers):HttpResponse(parse,parsecode)
	{
		for (auto itr = headers.begin(); itr != headers.end(); ++itr)
			this->headers.push_back(*itr);
	}
	list<Http::Header>& getHeaders() { return headers; }
	const list<Http::Header>& getHeaders()const { return headers; }
	const string& getParse()const { return parse; }
	const string& getBody()const { return body; }
	int getParseCode()const { return parsecode; }
	void setBody(const string& body);
	void setParse(const string& parse) { this->parse = body; }
	void setParseCode(int parsecode) { this->parsecode = parsecode; }
	void addHeader(const Http::Header& header);
	void addHeader(const vector<Http::Header>& headers);
	void removeHeader(const string&header);
	

	operator string();

	static HttpResponse no_content, bad,created,page_not_found,options,request_timeout,ok,conflict;

	

};

