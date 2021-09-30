#include "Server.h"
#include"StringReader.h"
#include"Date.h"

void Server::run()
{
    HttpServer server;
    try
    {
        server = HttpServer(serverPort);
        server.setTimeOut(keepAlive);
        server.getHeaders() =
        {
            {"Server","WebServer"}
            ,{"Date","?"}//date value will generated automatic by server class
        };
        cout << string("server started at port ") + to_string(DEFAULT_SERVER_PORT) << endl;
        cout << "current GMT : " << Date::getGMT_Time() << endl;
        while (true)
        {


            server.run();

            list<HttpServer::SocketState*>& newClients = server.getNewClients();//list of all the new clients joined the server 
            list<HttpServer::SocketState*>& recivers = server.getRecivers();//list of sockets that can recive data
            list < HttpServer::SocketState*>& senders = server.getSenders();//list of sockets that can send data

            for (HttpServer::SocketState* client : newClients)
            {

                cout << "client " + client->address + " connected" << endl;

            }
            for (HttpServer::SocketState* sender : senders)
            {

                sendMessage(server, *sender);

            }
            for (HttpServer::SocketState* reciver : recivers)
            {
                try
                {
                    server.recive(*reciver);
                    cout << endl << "server recived " << StringReader(reciver->requests_buffer.back()).readLine() << " request" << endl;

                }
                catch (...)
                {
                }

            }
            list<std::pair<string, string>>& disconnectedClients = server.getDisconnectedClients();//list of clients disconeted from server
            for (auto& dis_client : disconnectedClients)
            {
                cout << string("client ") + dis_client.first + " " << dis_client.second << endl;
                //dis_client.first its the client ip  dis_client.second its the reason of the disconnection
            }


        }
    }
    catch (exception& e)// major error
    {
        cout << e.what() << endl;
    }
    server.close();
    WSACleanup();
}

Server::Server(int serverPort, size_t keepAlive)
{
    this->serverPort = serverPort;
    this->keepAlive = keepAlive;
}

void Server::sendMessage(HttpServer& server, HttpServer::SocketState& s)
{

    if (s.requests_buffer.size() == 0)
    {

        return;
    }
    string recvMsg = s.requests_buffer.front();
    s.requests_buffer.pop();
    HttpResponse http_response = CreateResponse(recvMsg);
    http_response.addHeader(server.getHeaders());
    string response = http_response;
    try
    {

        server.sendString(s, response);
        cout << "server sent to client " << StringReader(response).readLine() << endl;
    }
    catch (...)
    {

    }




}
HttpResponse Server::CreateResponse(const HttpRequest& req)
{
    string method = req.getMethod();
    HttpResponse response = HttpResponse::bad;
    string fullPath = req.getLanguage() + " " + req.getUrl();
    if (method == "GET" || method == "HEAD")
    {
        response = handleGetRequest(req);
    }
    else if (method == "POST")
    {
        bool exists = false;
        try
        {
            req.getHeader("Content-Length");
        }
        catch (...)
        {
            return HttpResponse::bad;
        }
        cout << "******posting******* " << endl << req.getBody() << endl << "********************" << endl;
        response = HttpResponse("OK", 200, "posted succesfuly");

    }
    else if (method == "DELETE")
    {
        file_manager.deleteFile(fullPath);
        response = HttpResponse::no_content;
    }
    else if (method == "TRACE")
    {
        response = HttpResponse("OK", 200, req.getReq());

    }
    else if (method == "OPTIONS")
    {

        if (req.getUrl() != "*" && !file_manager.isFileExists(fullPath))
        {
            response = HttpResponse("Not Found", 404);
        }
        else
        {
            response = HttpResponse::options;
        }

    }
    else if (method == "PUT")
    {

        try
        {

            try
            {
                req.getHeader("Content-Length");//throws exception if not exists
            }
            catch (...)
            {
                return HttpResponse::bad;
            }
            int action = file_manager.updateOrCreate({ fullPath,req.getBody() });
            if (action == file_manager.CREATE)
                response = HttpResponse::created;
            else
                response = HttpResponse::no_content;//on update file 

            string location = "/" + req.getUrl();
            if (req.getLanguage() != "")
                location += string("?lang=") + req.getLanguage();
            response.addHeader(Http::Header{ "Content-Location",location });
        }
        catch (...)
        {
            response = HttpResponse::conflict;//for example if file has ?/ chars which not allowed in files
        }
    }

    return response;
}
HttpResponse Server::handleGetRequest(const HttpRequest& req)
{
    HttpResponse response;
    string method = req.getMethod();
    string url = req.getLanguage() + " " + req.getUrl();
    try
    {

        if (!file_manager.isFileExists(url))
        {
            url = HttpRequest::DEFAULT_LANGUAGE + " " + req.getUrl();
        }

        response = HttpResponse("ok", 200, file_manager.getFileDetails(url).body);
        response.addHeader(Http::Header{ "Last-Modified",file_manager.getModificationDate(url) });
        if (method == "HEAD")
        {
            size_t body_size = response.getBody().size();
            response.setBody("");//head cant have a body
            response.addHeader({ "Content-Length",to_string(body_size) });

        }
        response.addHeader(Http::Header{ "Content-Type", "text/html ;charset=utf-8" });


    }
    catch (exception&)
    {

        response = HttpResponse::page_not_found;
        if (method == "HEAD")
            response.setBody("");//head cant have a body

    }
    return response;


}