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
	
	void 			createSocketClient();
	int 			connectionAttempting();

	void 			setClientSock(int fd);
	void 			setNickName(const std::string& value);
	void 			setUserName(const std::string& );
	void 			setRealName(const std::string& value);
	void 			setIpAddress(const std::string& _ipAddres);
	void 			setValid(bool );
	void 			setModerator(bool val);

	bool 			isEmptyName(const std::string& nick, const std::string& user) const;
	bool 			isValid() const;
	bool 			isModerator() const;

	int 			getClientSock() const;
	std::string 	getIpAddress() const; 
	std::string 	getUserName() const;
	std::string 	getNickName() const;
	std::string 	getRealName() const;

	
private :
	bool valid;
	bool moderator;
	int clientSock;
	std::string nickName;
	std::string userName;
	std::string ipAddress;
	std::string realName;
};
