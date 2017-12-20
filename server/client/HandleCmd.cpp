#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include<fstream>

#include "header.h"

using std::string;

void SendCmd(SOCKET CmdSocket, string cmd) {
	int iResult;
	long size = htonl(cmd.length());
	send(CmdSocket, (char*)&size, sizeof(size), 0);
	
	iResult = send(CmdSocket, cmd.c_str(), cmd.length(), 0);
	// file name is no longer than 20 bytes
	if (iResult >0) {
		printf("%d bytes command was send.\n", iResult);
	}
	else if (iResult == SOCKET_ERROR) {
		printf("send failed with error: %d\n", WSAGetLastError());
		closesocket(CmdSocket);
		WSACleanup();
		exit(1);
	}
}

//receive response message
string ReceiveRes(SOCKET LocalSocket) {
	int iResult;
	char recvbuf[DEFAULT_BUFLEN];
	//receive filename first and cmd no longer than `DEFAULT_CMDLEN`
	string res;
	long size = 0;
	//to know how long is this command
	recv(LocalSocket, (char*)&size, sizeof(size), 0);
	size = ntohl(size);

	while (size > 0) {
		iResult = recv(LocalSocket, recvbuf, min(DEFAULT_BUFLEN, size), 0);
		if (iResult > 0) {
			printf("res Bytes received: %d\n", iResult);
			res.append(recvbuf, iResult);
			size -= iResult;
		}
		// Receive until the peer shuts down the connection
		else if (iResult == 0) {
			printf("Connection closing...\n");
			break;
		}
		else {
			printf("recv failed with error: %d\n", WSAGetLastError());
			closesocket(LocalSocket);
			WSACleanup();
			exit(1);
		}
	}
	//not receiving anything
	if (res.length() == 0) exit(1);
	return res;
}