#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")


#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H
#define CLI_NAMESPACE_BEGIN namespace cli {
#define CLI_NAMESPACE_END }

CLI_NAMESPACE_BEGIN

	class clientSocket
	{
	public:
		clientSocket();
		void InitializeClient();
		void Create(int argc, char **argv);
		void Connect();
		void Send();
		~clientSocket();
	private:
		int iResult;
	};

CLI_NAMESPACE_END

#endif
