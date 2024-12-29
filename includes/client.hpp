#pragma once
#include <ostream>

#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdint.h>
#include <vector>
#include <poll.h>
#include <sstream>
#include <unistd.h>

class Client
{
public :
	Client();
	Client(const std::string &_userName, const std::string &_nickName);
	~Client();
	void createSocketClient();
	int connectionAttempting();
	void setUserName(const std::string& );
	void setNickName(const std::string& value);
	void setRealName(const std::string& value);
	// void fillClientInfo();
	void setIpAddress(const std::string& _ipAddres);
	std::string getIpAddress() const; 
	void setClientSock(int fd);
	int getClientSock() const;
	std::string getUserName() const;
	std::string getNickName() const;
	void setValid(bool );
	void isModerator(bool val);
	bool isEmptyName() const;

	bool getClientModeration() const;
	bool isValid() const
	{
		return this->valid;
	}
	std::string getRealName() const
	{
		return this->realName;
	}

	void printInfo()
	{
		std::cout << "User Name : " << getUserName() << std::endl;
		std::cout << "Nick Name : " << getNickName() << std::endl;
	}

	
private :
	int clientSock;
	std::string userName;
	std::string nickName;
	std::string ipAddress;
	std::string realName;
	bool valid;
	bool moderator;
};
