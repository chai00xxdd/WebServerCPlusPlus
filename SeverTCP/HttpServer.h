#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#pragma comment(lib, "Ws2_32.lib")
#include <winsock2.h>
#include<list>
#include<vector>
#include<queue>
#include"Http.h"
#include"HttpResponse.h"
#include<chrono>
using namespace std;
class HttpServer
{
public:
	static constexpr int LISTEN = 1;
	static constexpr int RECEIVE = 2;
	static constexpr int IDLE = 3;
	static constexpr int SEND = 4;
	
	struct SocketState
	{
	    private:
		std::chrono::steady_clock::time_point last_msg_time; //last msg recived at this time
		SOCKET id;			// Socket handle
		int	recv;			// Receiving?
		int	send;			// Sending?
	   public:
		queue<string> requests_buffer;
		string address;
		 friend class HttpServer;
		
	};
	
private:
	static constexpr int BUFFER_MAX_SIZE = 10;
	int port;
	size_t max_sockets=60;//how many clients can be in the server
	size_t keepAliveTimeOut = 1000;//in seconds
	list<SocketState> sockets;//sockets[0] = LISTEN socket
	list<SocketState*> recivers;
	list<SocketState*> senders;
	list<SocketState*> newClients;
	list<std::pair<string, string>> disconnectedClients;
	mutable vector<Http::Header> server_headers;
	//the function add a new socket to the server if possible(number of sockets<max_sockets)
	void addSocket(SOCKET socket, int what = RECEIVE, const string& address = "");
	//the function remove all the clients that didnt send any request to the server
	//in the last this->KeepAliveTimeOut seconds
	void removeTimeOutSockets();
	//the function get a new client to the server
	void acceptConnection();
	//the function clear the sockets lists senders,recivers,newClients,disconnectedClients
	void clearLists();
	//
	void removeSocket(SOCKET socket, const string& reason = "");
public:
	HttpServer(int port,size_t max_sockets=60);
	HttpServer() = default;
	int run();
	void removeSocket(SocketState& s,const string&reason="");
	//getters setters
	list<SocketState>& getSockets() { return sockets; }
	list<SocketState*>& getSenders() { return senders; }
	list<SocketState*>& getRecivers() { return recivers; }
	list<SocketState*>& getNewClients() { return newClients; }
    list <std::pair<string, string>>& getDisconnectedClients() { return disconnectedClients; }
	vector<Http::Header>& getHeaders();
	size_t getTimeOut()const { return keepAliveTimeOut; }
	void setTimeOut(size_t time) { keepAliveTimeOut = time; }
	const vector<Http::Header>& getHeaders()const;
	size_t getServerCapacity()const { return max_sockets; }
	void setServerCapacity(size_t capacity) { max_sockets = capacity; }

	// the functoin get a socket s and recive a string to it
	void recive(SocketState& s);
	//the function get the respone string to the socket 
	void sendString(SocketState& s, const string& response);
	//the function close the server
	void close();

	

};

