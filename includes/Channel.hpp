#pragma once


#include "Client.hpp"
// #include "Server.hpp"




class Channel
{
public :
	Channel();
	Channel(const std::string& _channelName);
	~Channel();
	std::vector<Client>	AddUser2Channel(Client User);
	bool setCreation();

	Channel createChannel(std::string name);
private :
	std::string channelName;
	bool isCreated;
	std::vector<Client> channelClients;

};

