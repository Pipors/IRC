#pragma once

#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstdint> 
#include <vector>
#include <poll.h>
#include <sstream>
#include <unistd.h>
#include "Client.hpp"

// #define PORT 8080
#define BUFFER_SIZE 1024 * 1024
#define ERRNOSUCHCHANNEL()

class Server
{
private	:
	int serverFd;
	int new_socket;
    struct sockaddr_in address;
    int addrlen;
    char buffer[BUFFER_SIZE];
	std::vector<pollfd> monitor;
	std::vector<Client> clients;

public :

	void serverInit(int port);

	void acceptConnection();
	void readData();
	void sendData(const char* msg);
	void closeFd();
	void throwError(const char* msg);
	int getServerFd() const;

	Server();
	~Server();
};






