#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>

#include "header.h"

using std::string;

string ReceiveCmd(SOCKET ClientSocket) {
	int iResult;
	char recvbuf[DEFAULT_CMDLEN];
	int recvbuflen = DEFAULT_CMDLEN;
	//receive filename first
	string cmd;
	do {
		iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
		if (iResult > 0) {
			printf("cmd Bytes received: %d\n", iResult);
			cmd.append(recvbuf, iResult);
		}
		// Receive until the peer shuts down the connection
		else if (iResult == 0)
			printf("Connection closing...\n");
		else {
			printf("recv failed with error: %d\n", WSAGetLastError());
			closesocket(ClientSocket);
			WSACleanup();
			exit(1);
		}
	} while (iResult == recvbuflen);
	//not receiving anything
	if (cmd.length() == 0) exit(1);
	return cmd;
}