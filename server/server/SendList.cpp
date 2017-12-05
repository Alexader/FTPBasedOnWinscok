#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <cstdint>
#include <string>
#include <vector>
//do i have to include head file in every cpp file

#include "header.h"

using std::vector;

void SendList(SOCKET connectSocket, vector<string> list) {
	//`htonl()` function to ensure basic data type be encodeed roght in tcp/ip
	long length = htonl(list.size());
	send(connectSocket, (char*)&length, sizeof(length), 0);
	for (size_t i = 2; i < list.size(); i++) {
		length = htonl(list[i].length());
		//inform the client the length of file name first;
		send(connectSocket, (char*)&length, sizeof(length), 0);
		send(connectSocket, list[i].c_str(), list[i].length(), 0);
	}
	printf("send file list to client success");
}