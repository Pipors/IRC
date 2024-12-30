#include "../includes/Channel.hpp"

//#include <arpa/inet.h>
//#include <netinet/in.h>
////#include <cstdint> 
//#include <vector>
//#include <poll.h>
#include <sstream>
#include <unistd.h>


Channel::Channel() : channelName(""),  isCreated(0)
{

}


Channel::Channel(const std::string& _channelName) : channelName(_channelName), isCreated(0), channelClients(0)
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

void Channel::AddUser2Channel(const Client& client)
{
	this->channelClients.push_back(client);
}


std::string Channel::getChannelName() const
{
	return this->channelName;
}