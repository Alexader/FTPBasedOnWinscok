#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>

#include "header.h"

using std::string;

//receive control message
string ReceiveCmd(SOCKET ClientSocket) {
	int iResult;
	char recvbuf[DEFAULT_CMDLEN];
	//receive filename first and cmd no longer than `DEFAULT_CMDLEN`
	string cmd;
	long size = 0;
	//to know how long is this command
	recv(ClientSocket, (char*)&size, sizeof(size), 0);
	size = ntohl(size);

	while (size > 0) {
		iResult = recv(ClientSocket, recvbuf, min(DEFAULT_CMDLEN, size), 0);
		if (iResult > 0) {
			printf("cmd Bytes received: %d\n", iResult);
			cmd.append(recvbuf, iResult);
			size -= iResult;
		}
		// Receive until the peer shuts down the connection
		else if (iResult == 0) {
			printf("Connection closing...\n");
			break;
		}
		else {
			printf("recv failed with error: %d\n", WSAGetLastError());
			closesocket(ClientSocket);
			WSACleanup();
			exit(1);
		}
	}
	//not receiving anything
	if (cmd.length() == 0) exit(1);
	return cmd;
}