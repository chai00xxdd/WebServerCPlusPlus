#include "HttpServer.h"
#include<exception>
#include"Date.h"
void HttpServer::acceptConnection()
{
		SOCKET id = sockets.front().id;
		struct sockaddr_in from;		// Address of sending partner
		int fromLen = sizeof(from);

		SOCKET msgSocket = accept(id, (struct sockaddr*)&from, &fromLen);
		if (INVALID_SOCKET == msgSocket)
		{
			return;
		}

		string client_address = string(inet_ntoa(from.sin_addr)) + ":" + to_string(ntohs(from.sin_port));
		try
		{
			addSocket(msgSocket, RECEIVE, client_address);
			newClients.push_back(&sockets.back());
				
		}
		catch (...)
		{

		}
	
}
void HttpServer::clearLists()
{
	list<SocketState*>*lists[] = { &recivers,&senders,&newClients };
	for (auto list : lists)
		list->clear();
	disconnectedClients.clear();
}
void HttpServer::removeTimeOutSockets()
{
	
	auto end = chrono::high_resolution_clock::now();
	auto itr = ++sockets.begin();
	while (itr != sockets.end())
	{
		size_t timePassed = (size_t)std::chrono::duration_cast<chrono::seconds>(end - itr->last_msg_time).count();
		if (timePassed >= getTimeOut())
		{
			auto next = itr;
			next++;
			removeSocket(itr->id,string("disconnected because time out of ")+to_string(getTimeOut())+" seconds");
			itr = next;
		}
		else
			++itr;
	}
}
HttpServer::HttpServer(int port,size_t max_sockets):port(port),max_sockets(max_sockets)
{
	WSAData wsaData;

	if (NO_ERROR != WSAStartup(MAKEWORD(2, 2), &wsaData))
	{
		
		throw logic_error("failed to init server by WSAStartUp()");
	}
	SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (INVALID_SOCKET == listenSocket)
	{
		
		WSACleanup();
		throw logic_error("failed to init server");
	}


	sockaddr_in serverService;
	serverService.sin_family = AF_INET;
	serverService.sin_addr.s_addr = INADDR_ANY;
	serverService.sin_port = htons(port);

	if (SOCKET_ERROR == bind(listenSocket, (SOCKADDR*)&serverService, sizeof(serverService)))
	{
		
		closesocket(listenSocket);
		WSACleanup();
		throw logic_error("failed to bind server");
	}

	if (SOCKET_ERROR == listen(listenSocket, 5))
	{
		
		closesocket(listenSocket);
		WSACleanup();
		throw logic_error("failed to listen server");
	}
	
		addSocket(listenSocket, LISTEN);
	
	
	
	
}
int HttpServer::run()
{
	
	fd_set waitRecv, waitSend;
	FD_ZERO(&waitRecv);
	FD_ZERO(&waitSend);
	timeval timeout = { 1,0 };//timeout for select 
	clearLists();
	removeTimeOutSockets();
	for(SocketState& s:sockets)
	{
	
		if ((s.recv == LISTEN) || (s.recv == RECEIVE))
		{

			FD_SET(s.id, &waitRecv);
		}
		if (s.send == SEND)
			FD_SET(s.id, &waitSend);
	}
	

	int nfd;
	nfd = select(0, &waitRecv, &waitSend,NULL,&timeout);
	if (nfd == SOCKET_ERROR)
	{
		
		throw logic_error("error at select in run");
	}
	
	if (FD_ISSET(sockets.front().id, &waitRecv))
	{
		nfd--;
		acceptConnection();
	}
	for (auto itr=++sockets.begin(); itr!=sockets.end(); ++itr)
	{
		if (FD_ISSET(itr->id, &waitRecv))
		{
			itr->last_msg_time = chrono::high_resolution_clock::now();
			recivers.push_back(&*itr);
			
		}
		if (FD_ISSET(itr->id, &waitSend))
		{
			senders.push_back(&*itr);
		}
		
	}
	return nfd;
}

void HttpServer::removeSocket(SocketState& s,const string&reason)
{
	removeSocket(s.id,reason);
}

void HttpServer::addSocket(SOCKET socket,int what,const string&address)
{
	
	if (sockets.size()-1>=max_sockets && what!=LISTEN)//minus 1 because of listener
	{  
		closesocket(socket);
		disconnectedClients.push_back({ address,"declined because server is full capacity="+to_string(max_sockets) });
		throw logic_error("server is full cant add new socket");
		
	}
	
	unsigned long flag = 1;
	if (ioctlsocket(socket, FIONBIO, &flag) != 0)
	{
		closesocket(socket);
		disconnectedClients.push_back({ address,"declined could not set non-blocking socket error!!!" });
		throw logic_error("cant set socket as non blocking");
	}
	SocketState socket_state;
	socket_state.id = socket;
	socket_state.recv = what;
	socket_state.send = IDLE;
	socket_state.last_msg_time= chrono::high_resolution_clock::now();
	socket_state.address = address;
	sockets.push_back(socket_state);
	
	
}

void HttpServer::removeSocket(SOCKET socket,const string&reason)
{
	for (auto itr=++sockets.begin(); itr!=sockets.end(); ++itr)
	{
		if (itr->id == socket)
		{
			closesocket(itr->id);
			disconnectedClients.push_back({ itr->address,reason });
			sockets.erase(itr);
			break;
		}

	}
}





vector<Http::Header>& HttpServer::getHeaders()
{
	const HttpServer& thisconst = *this;
	return const_cast<vector<Http::Header>&>(thisconst.getHeaders());
}

const vector<Http::Header>& HttpServer::getHeaders() const
{
	auto itr= std::find(server_headers.begin(), server_headers.end(), Http::Header{ "Date" });
	if (itr != server_headers.end())
		itr->value = Date::getGMT_Time();
	return server_headers;
}

void HttpServer::recive(SocketState& s)
{

	SOCKET msgSocket = s.id;
	int max_length = 3000;
	char buffer[3000];
	int bytesRecv = recv(msgSocket, buffer, max_length, 0);
	if (bytesRecv == SOCKET_ERROR)
	{

		removeSocket(s.id, "recive error");
		throw logic_error("recive error at client");


	}
	if (bytesRecv == 0)//0 bytes means client closed the connection manually
	{
		
		string error = " client " + s.address+" disconnected";
		removeSocket(s.id,"disconnected");
		throw logic_error(error);
	}
	buffer[bytesRecv] = 0;
	s.requests_buffer.push(string(buffer));
	if (s.requests_buffer.size() > BUFFER_MAX_SIZE)
		s.requests_buffer.pop();
	s.send = HttpServer::SEND;
}

void HttpServer::sendString(SocketState& s, const string& response)
{
	int bytes = send(s.id, response.c_str(), (int)response.size(), 0);
	if(s.requests_buffer.size()==0)
	s.send = HttpServer::IDLE;
	s.recv = HttpServer::RECEIVE;
	if (bytes == SOCKET_ERROR)
	{
		
		removeSocket(s.id,"send error");
		string error = "client" + s.address + " disconnected cus sendString failed";
		throw logic_error(error);
	}
}



void HttpServer::close()
{
	for (size_t i = 0; i < sockets.size(); i++)
		closesocket(i);
	sockets.clear();
	
}
