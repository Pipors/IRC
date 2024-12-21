#pragma once

#include <iostream>
#include <cstring>
#include <sys/socket.h>
// #include <sys/type.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <cstdint> 
#include <vector>
#include <poll.h>
#include <sstream>
#include <unistd.h>
#include "Client.hpp"
#include "Command.hpp"


#define BUFFER_SIZE 1024 * 1024
#define ERRNOSUCHCHANNEL()

class Server
{
private	:
	int serverSock;							//socket file descriptor for the server 
    int addrlen;
	std::vector<struct pollfd> monitor;
	std::vector<Client> clients;
    struct sockaddr_in serverAddr;
	Command command;

public :

	void setServerSock(int port);
	void runningServer(int port);

	void acceptNewConnection();
	void recieveData(int newsocket);
	void sendData(int newsocket, const char* msg);
	void closeFd();
	void throwError(const char* msg, int fd);
	int getServerFd() const;
	std::vector<struct pollfd> getMonitor() const;
	uint16_t getMonitorSize() const;

	Server();
	~Server();
};






