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


#define BUFFER_SIZE 1024 * 1024
#define ERRNOSUCHCHANNEL()

class Server
{
private	:
	int serverSock;							//socket file descriptor for the server 
    int addrlen;
    struct sockaddr_in serverAddr;
	std::vector<struct pollfd> monitor;
	std::vector<Client> clients;

public :

	void serverInit(int port);

	void acceptNewConnection();
	void recieveData();
	void sendData(const char* msg);
	void closeFd();
	void throwError(const char* msg);
	int getServerFd() const;

	Server();
	~Server();
};






