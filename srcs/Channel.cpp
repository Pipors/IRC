#include "../includes/Channel.hpp"
#include <sstream>
#include <unistd.h>


Channel::Channel() : requirePasswd(0), inviteMode(0), channelLimit(10) ,channelName("")
{

}


Channel::Channel(const std::string& _channelName) : requirePasswd(0), inviteMode(0), channelLimit(10) ,channelName(_channelName), channelPasswd(""), channelClients(0)
{

}


Channel::~Channel()
{
}

void Channel::AddUser2Channel(Client* client)
{
	this->channelClients.push_back(*client);
}


std::string Channel::getChannelName() const
{
	return this->channelName;
}

void Channel::setInviteMode(const bool &mode)
{
	this->inviteMode = mode;
}


bool Channel::getInviteMode()
{
	return this->inviteMode;
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

std::string Channel::getPasswd() const
{
	return this->channelPasswd;
}


void Channel::setPasswdRequired(const bool& val)
{
	this->requirePasswd = val;
}

void Channel::setPasswd(const std::string& passwd)
{
	this->channelPasswd = passwd;
	if (!passwd.empty())
		setPasswdRequired(true);
	return;
}

bool Channel::getPasswdRequired() const
{
	return this->requirePasswd;
}


bool Channel::channelInviteModeOnly()
{
	return (this->inviteMode == true ? true : false);
}


size_t Channel::getChannelClientSize()
{
	return this->channelClients.size();
}

void Channel::resizeClientVector(const size_t& i)
{
	if (i < channelClients.size())
		this->channelLimit = i;
	channelClients.resize(channelLimit);
	return;
}

Client *Channel::getClientFromChannelByName(const std::string& name)
{
	size_t i = 0;
	while (i < channelClients.size())
	{
		if (channelClients[i].getNickName() == name)
			return &(channelClients[i]);
		i++;	
	}
	return NULL;
}