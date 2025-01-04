#include "../includes/Client.hpp"

#define PORT 8080
#define BUFFER_SIZE 1024


Client::Client() : valid(false), moderator(false), nickName("Guest"), userName("Guest")
{
    this->realName = "";
}

Client::Client(const std::string& _userName, const std::string& _nickName) : valid(false), moderator(false), nickName(_nickName), userName(_userName)
{

}

Client::~Client()
{

}


void Client::setClientSock(int fd)
{
    this->clientSock = fd;
}

void Client::setUserName(const std::string& value)
{
    this->userName = value;
}

void Client::setNickName(const std::string& value)
{
    this-> nickName = value;
} 

void Client::setRealName(const std::string& value)
{
    this-> realName = value;
}

void Client::setIpAddress(const std::string& _ipAddress)
{
    this->ipAddress = _ipAddress;
}

int Client::getClientSock() const
{
    return this->clientSock;
}

void Client::setValid(bool val)
{
    this->valid = val;
}


std::string Client::getNickName() const
{
    return this->nickName;
}

std::string Client::getUserName() const
{
    return this->userName;
}

std::string Client::getIpAddress() const
{
    return this->ipAddress;
}

void Client::setModerator(bool val)
{
    this->moderator = val;
}


bool Client::isModerator() const
{
    return this->moderator;
}

bool Client::isEmptyName(const std::string& nick, const std::string& user) const
{
    if (nick.empty() || user.empty())
        return true;
    return false;
}


std::string Client::getRealName() const
{
	return this->realName;
}


bool Client::isValid() const
{
	return this->valid;
}