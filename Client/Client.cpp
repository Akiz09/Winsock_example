#include "clientSocket.h"

CLI_NAMESPACE_BEGIN
using namespace std;

#define DEFAULT_ADDR "ip-address"
#define DEFAULT_PORT "port"

SOCKET ConnectSocket = INVALID_SOCKET;
struct addrinfo *result = NULL, *ptr = NULL, hints;

enum Packet
{
	P_ChatMessage
};
 
clientSocket::clientSocket() {

}

bool ProcessPacket(Packet packettype)
{
	switch (packettype)
	{
	case P_ChatMessage:
	{
		int bufferlenght;
		recv(ConnectSocket, (char*)&bufferlenght, sizeof(int), NULL);
		char* buffer = new char[bufferlenght + 1];
		buffer[bufferlenght] = '\0';
		recv(ConnectSocket, buffer, bufferlenght, NULL);
		cout << buffer << endl;
		delete[] buffer;
		break;
	}
	default:
		cout << "Unrecognized packet: " << packettype << endl;
		break;
	}
	return true;
}

void ClientThread()
{
	Packet packettype;
	while (true)
	{
		recv(ConnectSocket, (char*)&packettype, sizeof(Packet), NULL);

		if (!ProcessPacket(packettype))
		{
			break;
		}
	}
	closesocket(ConnectSocket);
}

void clientSocket::InitializeClient()
{
	WSADATA wsaData;
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		return;
	}

}

void clientSocket::Create(int argc, char **argv)
{
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	iResult = getaddrinfo(DEFAULT_ADDR, DEFAULT_PORT, &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return;
	}

	ptr = result;
	ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

	if (ConnectSocket == INVALID_SOCKET) {
		printf("Error at socket(): %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return;
	}
}

void clientSocket::Connect()
{
	iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);

	if (iResult == SOCKET_ERROR) {
		closesocket(ConnectSocket);
		ConnectSocket = INVALID_SOCKET;
	}

	freeaddrinfo(result);

	if (ConnectSocket == INVALID_SOCKET) {
		printf("Unable to connect to server!\n");
		WSACleanup();
		return;
	}
	cout << "Connected!" << endl;
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientThread, NULL, NULL, NULL);
}

void clientSocket::Send()
{
	string buffer;

	while (true)
	{
		getline(cin, buffer);
		int bufferlenght = buffer.size();
		Packet packettype = P_ChatMessage;
		send(ConnectSocket, (char*)&packettype, sizeof(Packet), NULL);
		send(ConnectSocket, (char*)&bufferlenght, sizeof(int), NULL);
		send(ConnectSocket, buffer.c_str(), bufferlenght, NULL);
		Sleep(10);
	}

}

clientSocket::~clientSocket() {

}

CLI_NAMESPACE_END