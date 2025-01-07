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
#include "Replies.hpp"

#define RPL_NICKCHANGE(oldnickname, nickname) (":" + oldnickname + " NICK " + nickname + CRLF)

#define BUFFER_SIZE 1024 * 1024
#define ERRNOSUCHCHANNEL()


class Server
{
public :
	
	/* FUNCTIONS SETTING UP THE SERVER */
	void setServerSock(int port);
	void acceptNewConnection();
	void runningServer(int port, const char *av);
	void processCommand(Client *, const char* message);

	bool equalStrings(const std::string& it, const std::string& compare);
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
	std::string getName() const;
	uint16_t getMonitorSize() const;
	Client *getClientFromVectorByFd(int _clientSock);
	Client *getClientFromServer(const std::string& nickname);
	std::vector<struct pollfd> getMonitor() const;
	std::vector<std::string> getWords_(const std::string &str);
	Client *getServerClient(const std::string &str);
	std::string getRangeAsString(std::vector<std::string> vec, size_t start, size_t end, std::string delimiter);
	void notCommand(const std::string&);

	Server();
	~Server();

private	:
	int serverSock ;							//socket file descriptor for the server 
	std::string passwd;                     // Passwd jo join the server
	std::string serverName;
    struct sockaddr_in serverAddr;
	static bool running;
	std::vector<struct pollfd> monitor;
	std::vector<Client> clients;            // Storing the clients who successfully establishde a connection wiht the server
	Command command;

};






