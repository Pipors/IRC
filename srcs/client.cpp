#include "../includes/Client.hpp"

#define PORT 8080
#define BUFFER_SIZE 1024


Client::Client()
{

}

Client::~Client()
{

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


int Client::connectionAttempting()
{
    // Create socket file descriptor for client
    if ((clientSock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Socket creation error");
        return -1;
    }

    clientAddr.sin_family = AF_INET;
    clientAddr.sin_port = htons((uint16_t)8080);

    // Convert IPv4 address from text to binary
    if (inet_pton(clientAddr.sin_family, (const char *)INADDR_ANY, &clientAddr.sin_addr) <= 0)
    {
        perror("Invalid address or address not supported");
        return -1;
    }

    // Connect to server
    if (connect(clientSock, (struct sockaddr*)&clientAddr, sizeof(clientAddr)) < 0)
    {
        perror("Connection failed");
        return -1;
    }
}

void Client::setIP(const std::string& _ipAddress)
{
    this->ipAddress = _ipAddress;
}