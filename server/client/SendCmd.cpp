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