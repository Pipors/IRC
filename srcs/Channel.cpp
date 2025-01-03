#include "../includes/Channel.hpp"
#include <sstream>
#include <unistd.h>


Channel::Channel() :isCreated(0), hasPasswd(0), inviteMode(0), channelLimit(10) ,channelName("")
{

}


Channel::Channel(const std::string& _channelName) : isCreated(0), hasPasswd(0), channelName(_channelName), channelClients(0)
{
}


Channel::~Channel()
{
}

Channel Channel::createChannel(std::string name)
{
	(void) name;
	return *this;
}

void Channel::setCreation(bool val)
{
	
	this->isCreated = val;
}


bool Channel::getCreation() const
{
	return this->isCreated;
}

void Channel::AddUser2Channel(Client* client)
{
	this->channelClients.push_back(*client);
}


std::string Channel::getChannelName() const
{
	return this->channelName;
}

void Channel::setInviteMode(bool &mode)
{
	this->inviteMode = mode;
}

void Channel::setPasswd(bool &val)
{
	this->hasPasswd = val;
}

bool Channel::getInviteMode()
{
	return this->inviteMode;
}

bool Channel::getPasswd()
{
	return this->hasPasswd;
}

std::vector<Client> *Channel::getChannelClientsVector()
{
	return &(channelClients);
}

std::string Channel::getChannelClientByName()
{
	std::string names;
	size_t i = 0;
	while (i < channelClients.size())
	{
		names += channelClients[i].getNickName();
		i++;
		if (i < channelClients.size())
			names += "\n";
	}
	return names;
}

std::string Channel::getCurrentTimestamp() 
{
    // Get the current time
    std::time_t now = std::time(nullptr);
    
    // Convert time_t to tm struct for local time
    std::tm* localTime = std::localtime(&now);
    
    // Create a buffer for the formatted timestamp
    char buffer[20];
    
    // Format the timestamp into the buffer
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localTime);
    
    return std::string(buffer);
}

void Channel::setChannelLimit(const int& i)
{
	this->channelLimit = i;
}

bool Channel::channelIsFull()
{
	return (this->channelLimit == channelClients.size() ? true : false);
}

bool Channel::userExist(const std::string &name, int nb)
{
	std::vector<Client>::iterator it = channelClients.begin();

	while (it != channelClients.end())
	{
		if (it->getClientSock() != nb)
		{
			if (it->getNickName() == name || it->getUserName() == name)
				return true;
		}
		it++;
	}
	return false;
}