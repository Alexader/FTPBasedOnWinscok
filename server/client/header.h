#pragma once
#ifndef HEADER_H
#define HEADER_H

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"

SOCKET ConnectServer(const char *hostname, const char *port);
#endif