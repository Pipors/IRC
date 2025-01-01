#pragma once

#include <iostream>
#include <cstring>
#include <sys/socket.h>
// #include <sys/type.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdint.h>
#include <vector>
#include <cerrno>
#include <cstdio>
#include <poll.h>
#include <sstream>
#include <unistd.h>
#include "Client.hpp"
#include "Command.hpp"
#include "Channel.hpp"
#include <fcntl.h>
#include "Channel.hpp"

#define RPL_NICKCHANGE(oldnickname, nickname) (":" + oldnickname + " NICK " + nickname + CRLF)

#define BUFFER_SIZE 1024 * 1024
#define ERRNOSUCHCHANNEL()

class Server
{
private	:
	int serverSock;							//socket file descriptor for the server 
	static bool running;
	std::vector<struct pollfd> monitor;
	std::vector<Client> clients;
	std::vector<Channel> channels;

    struct sockaddr_in serverAddr;
	std::string passwd;
	std::string serverName;
	Command command;

public :
	
	/* FUNCTIONS SETTING UP THE SERVER */
	void setServerSock(int port);
	void acceptNewConnection();
	void runningServer(int port, const char *av);

	/* FUCNTIONS HANDLING SENDING AND RECIEVING MSG */
	void recieveData(int clienSock);
	void parseCommand(int newsocket);
	

	/* UTILS */
	static void signalHandler(int sig);
	void throwError(const char* msg, int fd);
	void removeClientFromServer(int clientsock);
	void removeClientFromPollfd(int clientsock);
	void closeFd();
	
	/* SETTERS */
	void setServerPassWd(const char* av);
	
	/* GETTERS */
	int getServerFd() const;
	std::string getPasswd() const;
	uint16_t getMonitorSize() const;
	Client *getClientFromVectorByFd(int _clientSock);
	std::vector<struct pollfd> getMonitor() const;
	std::vector<std::string> getWords_(const std::string &str);
	std::string getRangeAsString(std::vector<std::string> vec, size_t start, size_t end, std::string delimiter);

	Server();
	~Server();
};






