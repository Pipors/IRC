#include "../includes/Channel.hpp"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <cstdint> 
#include <vector>
#include <poll.h>
#include <sstream>
#include <unistd.h>


Channel::Channel() : isCreated(0)
{

}


Channel::Channel(const std::string& _channelName) : channelName(_channelName), isCreated(0)
{
	this->channelClients.clear();



}


Channel::~Channel()
{
	std::cout << "all channels are removed" << std::endl;
}

Channel Channel::createChannel(std::string name)
{
	(void) name;
	return *this;
}
std::vector<Client>	Channel::AddUser2Channel(Client User){
	channelClients.push_back(User);

	return channelClients;
}

bool Channel::setCreation()
{
	
	this->isCreated = true;

	return isCreated;
}