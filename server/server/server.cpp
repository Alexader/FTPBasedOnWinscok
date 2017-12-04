#undef UNICODE

//#define WIN32_LEAN_AND_MEAN

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>

#include "header.h"

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")

using std::string;

int __cdecl main(void)
{
	SOCKET ClientSocket = INVALID_SOCKET;
	int iResult;

	char recvbuf[DEFAULT_BUFLEN];
	int recvbuflen = DEFAULT_BUFLEN;

	//initialize socket
	ClientSocket = init();
	
	while (true) {
		string cmd;
		cmd = ReceiveCmd(ClientSocket);
		if (!cmd.compare("ls")) {

		}
		else if(!cmd.compare("get")) {
			string filename = ReceiveCmd(ClientSocket);
			SendFile(ClientSocket, filename);
		}
	}

	// shutdown the connection since we're done
	iResult = shutdown(ClientSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		printf("shutdown failed with error: %d\n", WSAGetLastError());
		closesocket(ClientSocket);
		WSACleanup();
		return 1;
	}

	// cleanup
	closesocket(ClientSocket);
	WSACleanup();

	system("pause");
	return 0;
}