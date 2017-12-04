#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include<fstream>

#include "header.h"

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

using std::ifstream;
using std::ofstream;
using std::string;

int __cdecl main(int argc, char **argv)
{
	SOCKET ConnectSocket = INVALID_SOCKET;
	int iResult;
	char recvbuf[DEFAULT_BUFLEN];
	int recvbuflen = DEFAULT_BUFLEN;

	// Validate the parameters
	if (argc != 3) {
		printf("usage: %s server-name\n", argv[0]);
		system("pause");
		return 1;
	}

	//connect with server
	ConnectSocket = ConnectServer(argv[1], DEFAULT_PORT);

	//send a file name first
	iResult = send(ConnectSocket, argv[2], strlen(argv[2]), 0);
	// file name is no longer than 512 bytes
	if (iResult >0) {
		printf("%d bytes was send", iResult);
	}
	else if (iResult == SOCKET_ERROR) {
		printf("send failed with error: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}

	//receive a file and store;
	string fileName(argv[2]);
	char* recvbuff = new char[DEFAULT_BUFLEN];
	ofstream fileOutput("e:\\saved\\"+fileName, ofstream::binary);
	do {
		
		iResult = recv(ConnectSocket, recvbuff, DEFAULT_BUFLEN, 0);
		if (iResult == SOCKET_ERROR) {
			printf("receive failed with error: %d\n", WSAGetLastError());
			closesocket(ConnectSocket);
			fileOutput.close();
			WSACleanup();
			return 1;
		}
		else if (iResult > 0) {
			fileOutput.write(recvbuff, iResult);
		}
	} while (iResult > 0);
	fileOutput.close();
	printf("trans complete.\n");
	printf("Bytes Sent: %ld\n", iResult);

	// shutdown the connection since no more data will be sent
	iResult = shutdown(ConnectSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		printf("shutdown failed with error: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}

	// Receive until the peer closes the connection
	do {

		iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
		if (iResult > 0)
			printf("Bytes received: %d\n", iResult);
		else if (iResult == 0)
			printf("Connection closed\n");
		else
			printf("recv failed with error: %d\n", WSAGetLastError());

	} while (iResult > 0);

	// cleanup
	closesocket(ConnectSocket);
	WSACleanup();
	system("pause");
	return 0;
}
