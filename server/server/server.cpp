#undef UNICODE

//#define WIN32_LEAN_AND_MEAN

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <vector>
#include "dirent.h"
#include "header.h"

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")

using std::string;
using std::vector;

//declare a function for list out file entries
vector<string> listFile(string filePath);

int __cdecl main(void)
{
	SOCKET ClientSocket = INVALID_SOCKET;
	int iResult;

	//char recvbuf[DEFAULT_BUFLEN];
	int recvbuflen = DEFAULT_BUFLEN;

	//initialize socket
	ClientSocket = init();
	printf("ftp server is on\n");
	
	while (true) {
		string cmd;
		cmd = ReceiveCmd(ClientSocket);
		if (!cmd.compare("ls")) {
			vector<string> fileList;
			fileList = listFile(DEFAULT_FILE_PATH);
			//the file list have two unuseful `.` and `..` string
			SendList(ClientSocket, fileList);
		}
		else if(!cmd.compare("get")) {
			string filename = ReceiveCmd(ClientSocket);
			SendFile(ClientSocket, filename);
		}
		else if (!cmd.compare("exit")) {
			break;
		}
		else if (!cmd.compare("put")) {
			string filename = ReceiveCmd(ClientSocket);
			ReceiveFile(ClientSocket, filename);
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

//for list all file in a directory
vector<string> listFile(string filePath) {
	vector<string> fileList;
	DIR *dir = NULL;
	struct dirent *pent = NULL;
	dir = opendir(filePath.c_str());
	if (dir == NULL) {
		printf("can\'t open directory %s", filePath.c_str());
		exit(1);
	}
	while (pent = readdir(dir)) {
		if (pent == NULL) {
			printf("\n pent can\'t be initialized correctly.");
			exit(1);
		}
		fileList.push_back(pent->d_name);
	}
	closedir(dir);
	return fileList;
}
