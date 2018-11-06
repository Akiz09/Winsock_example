#include "Socket.h"
#define DEFAULT_PORT "port"

SOC_NAMESPACE_BEGIN
using namespace std;

struct addrinfo *result = NULL, *ptr = NULL, hints;
SOCKET ListenSocket = INVALID_SOCKET;
SOCKET ClientSocket;

SOCKET Connections[100];
int ConnectionCounter = 0;

enum Packet
{
	P_ChatMessage
};

bool ProcessPacket(int index, Packet packettype)
{
	switch (packettype)
	{
	case P_ChatMessage:
	{
		int bufferlenght;
		recv(Connections[index], (char*)&bufferlenght, sizeof(int), NULL);
		char * buffer = new char[bufferlenght];
		recv(Connections[index], buffer, sizeof(buffer), NULL);

		for (int i = 0; i < ConnectionCounter; i++)
		{
			if (i == index)
				continue;
			Packet chatmessagepacket = P_ChatMessage;
			send(Connections[i], (char*)&chatmessagepacket, sizeof(Packet), NULL);
			send(Connections[i], (char*)&bufferlenght, sizeof(int), NULL);
			send(Connections[i], buffer, bufferlenght, NULL);
		}
		delete[] buffer;
		break;
	}
	default:
		cout << "Unrecognized packet: " << packettype << endl;
		break;
	}
	return true;
}

Socket::Socket()
{
	/*Constructor*/
}

void ClientThreadHandler(int index)
{
	Packet packettype;
	while (true)
	{
		recv(Connections[index], (char*)&packettype, sizeof(Packet), NULL);

		if (!ProcessPacket(index, packettype))
		{
			break;
		}
	}
	closesocket(Connections[index]);
}

void Socket::InitializeSock()
{
	WSADATA wsa;
	iResult = WSAStartup(MAKEWORD(2, 2), &wsa);

	Socket::LoadingAnimation();

	if (iResult != 0)
	{
		printf("Failed. Error Code : %d", WSAGetLastError());
		cout << "Press Enter to Continue";
		cin.ignore();
		return;
	}
	printf("\nInitialised\n");

}

void Socket::Create() 
{

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed: %d\n", iResult);
		WSACleanup();
		return;
	}

	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

	if (ListenSocket == INVALID_SOCKET) {
		printf("Error at socket(): %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return;
	}

}

void Socket::Bind() 
{
	iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		printf("bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
		return;
	}
}

void Socket::Listen() 
{   
	iResult = listen(ListenSocket, SOMAXCONN);
	if (iResult == SOCKET_ERROR) {
		printf("Listen failed with error: %ld\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return;
	}
}

void Socket::Accept() 
{
	// Accept a client socket
	ClientSocket = accept(ListenSocket, NULL, NULL);
	if (ClientSocket == INVALID_SOCKET) {
		printf("accept failed: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return;
	}
}

void Socket::Recive() 
{
	for (int i = 0; i < 100; i++)
	{
		Socket::Accept();

		if (ClientSocket == 0)
		{
			cout << "Faiiled to accept connection." << endl;
		}
		else
		{

			cout << "Client #" << i << " Connected." << endl;
			Connections[i] = ClientSocket;
			ConnectionCounter += 1;
			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientThreadHandler, (LPVOID)(i), NULL, NULL);
			Packet chatmessagepacket = P_ChatMessage;
			send(Connections[i], (char*)&chatmessagepacket, sizeof(Packet), NULL);
			string MOTD = "Welcome!";
			int sizeofmotd = MOTD.size();
			send(Connections[i], (char*)&sizeofmotd, sizeof(int), NULL);
			send(Connections[i], MOTD.c_str(), sizeofmotd, NULL);

		}
	}
	system("pause");
}

void Socket::LoadingAnimation()
{
	for (int j = 0; j < 3; j++) {
		cout << "\rLoading   \rLoading";
		for (int i = 0; i < 3; i++) {
			cout << ".";
			Sleep(300);
		}
	}
}

Socket::~Socket()
{
	/*Destructors*/
}

SOC_NAMESPACE_END