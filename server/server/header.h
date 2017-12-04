#pragma once
#ifndef HEADER_H
#define HEADER_H

#include <string>

#define DEFAULT_BUFLEN 512
#define DEFAULT_CMDLEN 20
#define DEFAULT_PORT "20"
#define DEFAULT_FILE_PATH "e:\\uri\\"

using std::string;

SOCKET init();
void SendFile(SOCKET ClientSocket, string filename);
string ReceiveCmd(SOCKET ClientSocket);
#endif