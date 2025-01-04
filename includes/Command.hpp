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

	
	bool channelExist(const std::string& );
	
	std::string getCommandLine() const;
	std::string getParameter1() const; 
	std::string getParameter2() const; 
	std::string standardMsg(std::string, std::string, std::string );

	void passCommand(Client *, const std::string&, const std::string &);
	void joinCommand(const std::string &, const std::string&, Client *);
	void modeCommand(Client *,const std::string&, const std::string&, const std::string&);
	void sendData(int, const std::string& );
	void setCommandLine(char*  );
	void setParameters(std::string, std::string );
	void privmsgCommandChannel(const std::string &, Client *, const std::string& );
	void privmsgCommandUser(Client *, const std::string& );

	std::vector<std::string> getWords(const std::string& );
	std::vector<Channel> getChannelVector() const;
	
	Channel *getChannelByName(const std::string& );

private :
	std::string commandLine;
	std::string parameter1;
	std::string parameter2;
	std::vector<Channel> channels;
};


