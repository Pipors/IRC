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

	
	bool channelExist(const std::string &);
	
	std::string standardMsg(std::string, std::string, std::string);

	void 	easyCheck(Client *, const std::string&); //processing the returned value of isEligible() function
	void 	joinCommand(const std::string &, const std::string&, Client *);
	void 	modeCommand(Client *,const std::string &, const std::string &, const std::string &);
	void 	passCommand(Client *, const std::string &, const std::string &);
	void 	privmsgCommandChannel(const std::string &, Client *, const std::string &);
	void 	privmsgCommandUser(Client *, const std::string &);
	void 	sendData(int, const std::string &);


	std::vector<Channel> 	 getChannelVector() const;
	std::vector<std::string> getWords(const std::string &);
	
	Channel 				 *getChannelByName(const std::string &);

private :
	std::string commandLine;
	std::string parameter1;
	std::string parameter2;
	std::vector<Channel> channels;
};


