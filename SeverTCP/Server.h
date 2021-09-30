#pragma once
#include"HttpResponse.h"
#include"HttpRequest.h"
#include"FileManager.h"
#include "HttpServer.h"
class Server
{
private:
	static constexpr size_t DEFAULT_KEEP_ALIVE = 10;//in seconds
	static constexpr int DEFAULT_SERVER_PORT = 8080;
	FileManager file_manager;
	//the functoin get the server and a client s and send a reply to the client
//as answer to the request in the client requests buffer
	void sendMessage(HttpServer& server, HttpServer::SocketState& s);
	//the functoin get a req of method get and create a response to it
	HttpResponse handleGetRequest(const HttpRequest& req);
	//the function create a respone to a http req
	HttpResponse CreateResponse(const HttpRequest& req);
	int serverPort = DEFAULT_SERVER_PORT;
	size_t keepAlive = DEFAULT_KEEP_ALIVE;

public:
	void setKeepAliveTimeOut(size_t newKeepAlive) { this->keepAlive = newKeepAlive; }
	void run();
	Server(int serverPort = DEFAULT_SERVER_PORT, size_t keepAlive = DEFAULT_KEEP_ALIVE);
	

};

