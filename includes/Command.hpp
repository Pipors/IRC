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




class Command
{
public :
	Command();
	~Command();

	std::string getCommand() const;
	uint16_t getCommandLen(const std::string& command) const;
	void setCommand(std::string command);
	void setParameter(std::string param);
	std::string getParameter(); 

	void init(char *message);

private :
	std::string command;
	std::string parameter;
	// uint16_t commandLen;
};


