#pragma once
#include<string>
#include<vector>
using namespace std;
class Http
{

public:
	struct Header
	{
		string headerName;
		string value;
		bool operator==(const Header& other) const
		{
			return headerName == other.headerName;
		}
		
	};
	static constexpr const char* HTTP_VER = "HTTP/1.1";
	static constexpr const char* CRLF = "\r\n";
};
