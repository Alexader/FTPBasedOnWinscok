#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include<fstream>

#include "header.h"

using std::string;
using std::ofstream;

void ReceiveFile(SOCKET ConnectSocket, string filename) {
	//receive a file and store;
	string fileName(filename);
	int iResult;
	char* recvbuff = new char[DEFAULT_BUFLEN];
	ofstream fileOutput(DEFAULT_FILE_PATH + fileName, ofstream::binary);
	do {
		iResult = recv(ConnectSocket, recvbuff, DEFAULT_BUFLEN, 0);
		if (iResult == SOCKET_ERROR) {
			printf("receive failed with error: %d\n", WSAGetLastError());
			closesocket(ConnectSocket);
			fileOutput.close();
			WSACleanup();
			exit(1);
		}
		else if (iResult > 0) {
			fileOutput.write(recvbuff, iResult);
		}
	} while (iResult==DEFAULT_BUFLEN);
	fileOutput.close();
	printf("transfer complete and file is stored at %s.\n", DEFAULT_FILE_PATH);
}

bool deleteFile(string filename) {
	return remove((DEFAULT_FILE_PATH + filename).c_str());
}
