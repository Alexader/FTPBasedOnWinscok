#define WIN32_LEAN_AND_MEAN

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <vector>

#include "header.h"

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

using std::ifstream;
using std::ofstream;
using std::string;
using std::getline;
using std::cin;
using std::cout;
using std::endl;
using std::vector;

void ReceiveList(SOCKET Conncect, vector<string> &list);

int __cdecl main(int argc, char **argv)
{
	SOCKET ConnectSocket = INVALID_SOCKET;
	int iResult;
	char recvbuf[DEFAULT_BUFLEN];
	int recvbuflen = DEFAULT_BUFLEN;

	//connect with server, argv[1] is the hostname or ip address
	ConnectSocket = ConnectServer(argv[1], DEFAULT_PORT);

	cout << "welcome to ftp client!!" << endl;
	cout << "you can enter command `dir` for source file list, `get [filename]` for file source" << endl;
	
	while (true) {
		//command no longer than 20 bytes
		string cmdline;
		vector<string> substrings;
		string delimiter = " ";
		//for record substring
		string token;
		cout << "enter your command in format: cmd + optional[space + filename]\nftp>";
		getline(cin, cmdline);
		size_t pos;
		//split cmdline and cmdline will be destroyed;
		while ((pos = cmdline.find(delimiter)) != string::npos) {
			token = cmdline.substr(0, pos);
			cmdline.erase(0, pos + delimiter.length());
			substrings.push_back(token);
		}
		substrings.push_back(cmdline);

		//deal with different commands
		int size = substrings.size();
		string cmd, param;
		if (size == 1) {
			cmd = substrings[0];
			if (!cmd.compare("ls")) {
				//Chinese character not supported
				SendCmd(ConnectSocket, cmd);
				cout << "send ls success" << endl;
				vector<string> fileNameToShow;
				ReceiveList(ConnectSocket, fileNameToShow);
				for (size_t i = 0; i < fileNameToShow.size(); i++) {
					cout << fileNameToShow[i] << endl;
				}
			}
			else if (!cmd.compare("exit")) {
				//exit ftp
				SendCmd(ConnectSocket, cmd);
				cout << "leave ftp, byte" << endl;
				break;
			}
		}
		else if (size == 2) {
			cmd = substrings[0];
			param = substrings[1];
			//different command with respectively handle
			if (!cmd.compare("get")) {
				SendCmd(ConnectSocket, cmd);
				SendCmd(ConnectSocket, param);
				ReceiveFile(ConnectSocket, param);
			}
			else if (!cmd.compare("put")) {
				SendCmd(ConnectSocket, cmd);
				SendCmd(ConnectSocket, param);
				SendFile(ConnectSocket, param);
			}
		}
	}


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

void ReceiveList(SOCKET Connect, vector<string> &list) {
	int iResult;
	//non-blocking mode for socket
	unsigned long imode = 1;
	iResult = ioctlsocket(Connect, FIONBIO, &imode);
	if (iResult != NO_ERROR)
	{
		printf("ioctlsocket failed with error: %ld\n", iResult);
	}
	long size = 0;
	//know how many strings will received
	recv(Connect, (char*)&size, sizeof(size), 0);
	size = ntohl(size);
	for (int i = 0; i < size; ++i) {
		std::string stringRead;
		long length = 0;
		//get length of each filename string
		recv(Connect, (char*)&length, sizeof(length), 0);
		length = ntohl(length);
		while (0 < length) {
			char buffer[CMD_MAX_LEN];
			int cread;
			cread = recv(Connect, buffer, min(sizeof(buffer), length), 0);
			stringRead.append(buffer, cread);
			length -= cread;
		}
		list.push_back(stringRead);
	}
}