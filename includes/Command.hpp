#pragma once
#define CRLF "\r\n"
#
#define ERR_FUNCSEND "FATAL : send() Failed!"
#include <iostream>
#include <cstring>
#include <sys/socket.h>
// #include <sys/type.h>
#include <arpa/inet.h>
#include <netinet/in.h>
//#include <cstdint> 
#include <vector>
#include <poll.h>
#include <sstream>
#include <unistd.h>
#include <string>

#include "Channel.hpp"
#include "Client.hpp"
#include "Replies.hpp"


class Command
{
public :
	Command();
	~Command();

	const char* standardMsg(std::string hostname, std::string ipaddress, std::string channelname);
	
	bool channelExist(const std::string& name);
	
	std::string getCommandLine() const;
	std::string getParameter1() const; 
	std::string getParameter2() const; 
	// void checkPasswd(std::string passwd, Client client);
	// void init(char *message);
	void joinCommand(const std::string &param, Client *client);
	void parseHexChat(const std::string &param, std::string passwd, Client client);
	void passCommand(Client *client);
	void sendData(int newsocket, const std::string& msg);
	void setCommandLine(char*  command);
	void setParameters(std::string param1, std::string param2);
	
	std::vector<std::string> getWords(const std::string& str);
	std::vector<Channel> getChannelVector() const;
	
	Channel *getChannelByName(const std::string& name);

private :
	std::string commandLine;
	std::string parameter1;
	std::string parameter2;
	std::vector<Channel> channels;
	// uint16_t commandLen;
};


