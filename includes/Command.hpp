#pragma once
#define CRLF "\r\n"
#
#define ERR_SEND "FATAL : send() Failed!"
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
	void sendData(int newsocket, const char* msg);
	bool channelExist(const std::string& name);
	std::vector<Channel> getChannelVector() const;
	const char* msg(std::string hostname, std::string ipaddress, std::string channelname);
	void passCommand(Client client);
	void joinCommand(const std::string &param, Client client);

private :
	std::string commandLine;
	std::string parameter1;
	std::string parameter2;
	std::vector<Channel> channels;
	std::unordered_map<std::string, Channel> map;
	// uint16_t commandLen;
};


