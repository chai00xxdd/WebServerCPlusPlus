#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#pragma comment(lib, "Ws2_32.lib")
#include <winsock2.h>
using namespace std;
#include <time.h>
#include<string>
#include"HttpServer.h"
#include"HttpRequest.h"
#include"HttpResponse.h"
#include<fstream>
#include"FileManager.h"
#include"StringReader.h"
#include"Date.h"
#include"Server.h"
#include<chrono>

int main()
{
   Server().run();
}


