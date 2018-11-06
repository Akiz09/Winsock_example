#include <iostream>
#include <conio.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#pragma comment(lib,"ws2_32.lib")

#ifndef SOCKET_H
#define SOCKET_H
#define SOC_NAMESPACE_BEGIN namespace soc {
#define SOC_NAMESPACE_END }

SOC_NAMESPACE_BEGIN

	class Socket
	{
	public:
		Socket();
		void InitializeSock();
		void Create();
		void Bind();
		void Listen();
		void Accept();
		void Recive();
		~Socket();
	private:
		void LoadingAnimation();
		int iResult;
	};

SOC_NAMESPACE_END
#endif