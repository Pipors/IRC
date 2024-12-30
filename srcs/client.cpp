#include "../includes/Client.hpp"

#define PORT 8080
#define BUFFER_SIZE 1024


Client::Client()
{
    this->valid = false;
    this->moderator = false;
    this->realName = "";
    this->nickName = "";
    this->userName = "";
}

Client::Client(const std::string& _userName, const std::string& _nickName) : userName(_userName), nickName(_nickName), valid(false), moderator(false)
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

int Client::getClientSock()
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

void Client::isModerator(bool val)
{
    this->moderator = val;
}


bool Client::getClientModeration() const
{
    return this->moderator;
}

bool Client::isEmptyName() const
{
    if (this->getNickName().empty() || this->getUserName().empty())
        return true;
    return false;
}