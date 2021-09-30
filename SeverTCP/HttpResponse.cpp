#include "HttpResponse.h"

HttpResponse HttpResponse::no_content("No Content", 204);
HttpResponse HttpResponse::created("Created", 201);
HttpResponse HttpResponse::bad("Bad Request", 400);
HttpResponse HttpResponse::page_not_found("Not Found", 404, " <h1> PAGE NOT FOUND  </h1>");
HttpResponse HttpResponse::options("OK", 200, { {"Allow","GET, HEAD, POST, DELETE, PUT, TRACE"} });
HttpResponse HttpResponse::request_timeout ("Request Timeout", 408);
HttpResponse HttpResponse::ok("OK", 200);
HttpResponse HttpResponse::conflict("Conflict", 409);

void HttpResponse::setBody(const string& body)
{
	this->body = body;
	addHeader(Http::Header{ "Content-Type","text/html" });
	addHeader(Http::Header{ "Content-Length",to_string(body.size()) });
}
void HttpResponse::removeHeader(const string&header)
{
	headers.remove({ header });
}
void HttpResponse::addHeader(const Http::Header& header)
{
	headers.remove(header);// to ensure no duplications
	headers.push_back(header);
}
void HttpResponse::addHeader(const vector<Http::Header>& headers)
{
	for (const Http::Header& h : headers)
	{
		addHeader(h);
	}
}

HttpResponse::operator string()
{
	string http = string(Http::HTTP_VER) + " " + to_string(parsecode) + " " + parse + Http::CRLF;
	for (const Http::Header& header : headers)
	{
		http += header.headerName + ": " + header.value + Http::CRLF;
	}
	
	http += Http::CRLF;
	http += body;
	
	return http;
}
