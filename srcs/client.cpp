#include "../includes/Client.hpp"

#define PORT 8080
#define BUFFER_SIZE 1024


Client::Client()
{
    this->valid = false;
    this->realName = "anas";
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

void Client::setIp(const std::string& _ipAddress)
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