#include "HttpRequest.h"
#include<iostream>
#include<string>
#include<exception>
#include"StringReader.h"

string HttpRequest::DEFAULT_LANGUAGE = "en";

HttpRequest::HttpRequest(const string& request) :request(request)
{
	string request_copy = request;
	StringReader reader(request_copy);
	string firstLine = reader.readLine();
	size_t langPos = firstLine.find("?lang=");
	if (langPos != string::npos)
	{
		for (size_t i = langPos + 6; i<firstLine.size()&&firstLine[i]!=' '; i++ )
		{
			lang += firstLine[i];
		}
		auto begin = firstLine.begin() + langPos;
		firstLine.erase(begin, begin + 6 + lang.size());
	}
	StringReader readerFirstLine(firstLine);
	method = readerFirstLine.readWord();
	url = readerFirstLine.readWord();
	if (url != "")
		url.erase(url.begin());

	while (!reader.eof())
	{

		string headerLine = reader.readLine();
		if (headerLine == "")
			continue;
		StringReader headerReader(headerLine);
		Http::Header header = { headerReader.readWord(),headerReader.readRestOfString() };
		if (header.headerName[header.headerName.size() - 1] == ':')
		{
			header.headerName.erase(header.headerName.end() - 1, header.headerName.end());
		}
		if (header.headerName == "Content-Length")
		{

			int chars = std::stoi(header.value);
			
			for (auto itr = request_copy.end() - chars; itr != request_copy.end(); ++itr)
			{
				content += *itr;
			}
			request_copy.resize(request_copy.size() - chars);
		}
		headers.push_back(header);
	}


}

const Http::Header& HttpRequest::getHeader(const string& header) const
{
	for (const auto& h : headers)
		if (h.headerName == header || (h.headerName + ":") == header)
			return h;
	throw logic_error("header not exists");
}
