#pragma once

#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 8080
#define BUFFER_SIZE 1024

class Server
{
public :

	void serverInit();

	void acceptConnection();
	void readData();
	void sendData(const char* msg);
	void closeFd();
	void throwError(const char* msg);
	int getServerFd() const;

private	:
	int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};


};






