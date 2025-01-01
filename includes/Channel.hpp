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

	bool getCreation() const;
	bool getPasswd();

	void AddUser2Channel(Client* User);
	void setCreation(bool val);
	void setPasswd(bool &);

	std::string getChannelName() const;
	std::string getChannelClientByName();
	std::string getCurrentTimestamp();

	std::vector<Client> *getChannelClientsVector();

	Channel createChannel(std::string name);


private :
	bool isCreated;
	bool hasPasswd;
	std::string channelName;
	std::string channelPasswd;
	std::vector<Client> channelClients;
};

