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

	void printClt(int newsocket)
	{
		
		for(size_t i = 0; i < clients.size(); i++)
		{
			if (clients[i].getClientSock() == newsocket)
				clients[i].printInfo();
		}
	}
public :
	
	/* FUNCTIONS SETTING UP THE SERVER */
	void setServerSock(int port);
	void acceptNewConnection();
	void runningServer(int port, const char *av);

	/* FUCNTIONS HANDLING SENDING AND RECIEVING MSG */
	void recieveData(int clienSock);
	void parseCommand(int newsocket);
	// void checkPasswd(int newsocket, Client client);
	static void sendDataS(int newsocket, std::string msg)
	{
		if (send(newsocket, msg.c_str(), msg.size(), 0) <= 0)
			std::cerr << ERR_FUNCSEND << std::endl;
	}

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

	std::string getVectorWords(std::vector<std::string> vec, size_t i)
	{
		size_t pos = i;
		std::string message;
		while (pos != vec.size())
		{
			message += vec[pos];
			if (pos < vec.size() - 1)
				message = message + vec[pos] + " ";
			pos++;
		}
		return message;
	}
	

	/*std::string standardProtocol(Client client) const
	{
		return ();
	}*/

	std::vector<std::string> getWords_(const std::string &str);

	/* TEMPLATE */
	/*template<class T>
	void removeClient(std::vector<T> vec, int clientsock)
	{
		typename T::iterator it = vec.begin();
		for (it = vec.begin(); it != vec.end(); it++)
		{
			if (*it == clientsock)
			vec.erase(*it);
		}
	}*/


	std::vector<std::string> getRangeIterators(std::vector<std::string>& vec, size_t start, size_t end) {
		if (start > vec.size() || end > vec.size() || start > end) {
			throw std::out_of_range("Invalid range specified.");
		}

		return { vec.begin() + start, vec.begin() + end };
	}

	std::string getRangeAsString(const std::vector<std::string>& vec, size_t start, size_t end, std::string delimiter) {
		if (start > vec.size() || end > vec.size() || start > end) {
			throw std::out_of_range("Invalid range specified.");
		}

		// Extract range and concatenate strings
		std::string result;
		for (auto it = vec.begin() + start; it != vec.begin() + end; ++it) {
			if (!result.empty()) {
				result += delimiter; // Add a space between words
			}
			result += *it;
		}
		return result;
	}

	Server();
	~Server();
};






