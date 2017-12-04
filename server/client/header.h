#pragma once
#ifndef HEADER_H
#define HEADER_H

#include <string>

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "20"
#define CMD_MAX_LEN 40

using std::string;

SOCKET ConnectServer(const char *hostname, const char *port);
void ReceiveFile(SOCKET ConnectSocket, string filename);
void SendCmd(SOCKET CmdSocket, string cmd);
#endif