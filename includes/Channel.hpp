#pragma once


#include "Client.hpp"
#include <unordered_map>
// #include "Server.hpp"




class Channel
{
public :
	Channel();
	Channel(const std::string& _channelName);
	~Channel();
	void AddUser2Channel(const Client& User);
	void setCreation(bool val);
	bool getCreation() const;
	std::string getChannelName() const;

	Channel createChannel(std::string name);


private :
	std::string channelName;
	bool isCreated;
	std::vector<Client> channelClients;
	bool hasPasswd;
	std::string channelPasswd;
};

