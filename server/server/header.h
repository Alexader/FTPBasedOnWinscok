#pragma once
#ifndef HEADER_H
#define HEADER_H

#include <string>
#include <vector>

#define DEFAULT_BUFLEN 512
#define DEFAULT_CMDLEN 20
#define DEFAULT_PORT "20"
#define DEFAULT_FILE_PATH "e:\\uri\\"

using std::string;
using std::vector;

SOCKET init();
void SendFile(SOCKET ClientSocket, string filename);
string ReceiveCmd(SOCKET ClientSocket);
void response(SOCKET local, string info);
void SendList(SOCKET connect, vector<string> list);
void ReceiveFile(SOCKET ClientSocket, string filename);
bool deleteFile(string filename);
#endif