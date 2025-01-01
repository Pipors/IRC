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

	std::string getCommandLine() const;
	void setCommandLine(char*  command);
	std::string getParameter1() const; 
	std::string getParameter2() const; 
	void setParameters(std::string param1, std::string param2);
	// void checkPasswd(std::string passwd, Client client);
	std::vector<std::string> getWords(const std::string& str);
	void parseHexChat(const std::string &param, std::string passwd, Client client);
	// void init(char *message);
	void sendData(int newsocket, const std::string& msg);
	bool channelExist(const std::string& name);
	std::vector<Channel> getChannelVector() const;
	const char* msg(std::string hostname, std::string ipaddress, std::string channelname);
	void passCommand(Client *client);
	void joinCommand(const std::string &param, Client *client);
	Channel *getChannelByName(const std::string& name)
	{
		for (size_t i = 0; i != channels.size(); i++)
		{
			if (channels[i].getChannelName() == name)
				return &(channels[i]);
		}
		return NULL;
	}
private :
	std::string commandLine;
	std::string parameter1;
	std::string parameter2;
	std::vector<Channel> channels;
	// uint16_t commandLen;
};


