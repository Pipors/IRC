#pragma once


#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>



class Client
{
public :
	void createSocketClient();

	
private :
	int clientFd;
    struct sockaddr_in serv_addr;
	std::string ipAddress;
	std::string userName;
	std::string nickName;
	std::string realName;
	bool valid;
};