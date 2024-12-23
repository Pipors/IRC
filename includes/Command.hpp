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


class Command
{
public :
	Command();
	~Command();

	std::string getCommandLine() const;
	void setCommandLine(const char*  command);
	std::string getParameter1() const; 
	std::string getParameter2() const; 
	void setParameters(std::string param1, std::string param2);
	void checkPasswd(std::string passwd, Client client);

	// void init(char *message);


private :
	std::string commandLine;
	std::string parameter1;
	std::string parameter2;
	// uint16_t commandLen;
};


