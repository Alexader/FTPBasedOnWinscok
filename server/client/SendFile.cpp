#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <fstream>
#include <sstream>

#include "header.h"

using std::ifstream;
using std::string;

void SendFile(SOCKET ClientSocket, string fileName) {
	int iResult;
	//setup a fstream for writing files
	ifstream inputFile(DEFAULT_FILE_PATH + fileName);
	//get the length of a file.
	inputFile.seekg(0, inputFile.end);
	int fileLength = inputFile.tellg();
	//reset cursor
	inputFile.seekg(0, inputFile.beg);

	char* fileBuffer = new char[DEFAULT_BUFLEN];
	int actualLength;
	do {
		actualLength = DEFAULT_BUFLEN > fileLength ? fileLength : DEFAULT_BUFLEN;
		inputFile.read(fileBuffer, actualLength);
		//record the output data length
		fileLength -= actualLength;
		iResult = send(ClientSocket, fileBuffer, actualLength, 0);
		if (iResult == SOCKET_ERROR) {
			printf("send failed with error: %d\n", WSAGetLastError());
			closesocket(ClientSocket);
			inputFile.close();
			WSACleanup();
			exit(1);
		}
	} while (fileLength != 0);
	inputFile.close();
	printf("transfer complete.\n");
}