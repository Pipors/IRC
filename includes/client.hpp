#pragma once


#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstdint> 
#include <vector>
#include <poll.h>
#include <sstream>
#include <unistd.h>

class Client
{
public :
	Client();
	~Client();
	void createSocketClient();
	int connectionAttempting();
	void setUserName(const std::string& value);
	void setNickName(const std::string& value);
	void setRealName(const std::string& value);
	void setIp(const std::string& _ipAddres);
	void setClientSock(int fd);
	int getClientSock() const;


	
private :
	int clientSock;
	std::string ipAddress;
	std::string userName;
	std::string nickName;
	std::string realName;
	bool valid;
};