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
	void setInviteMode(bool &);
	void setPasswd(bool &);
	void setChannelLimit(const int& i);

	bool channelIsFull();
	bool userExist(const std::string &name);
	bool getInviteMode();
	bool channelInviteModeOnly(const std::string &name);

	std::string getChannelName() const;
	std::string getChannelClientByName();
	std::string getCurrentTimestamp();

	std::vector<Client> *getChannelClientsVector();

	Channel createChannel(std::string name);


private :
	bool isCreated;
	bool hasPasswd;
	bool inviteMode;
	size_t channelLimit;
	std::string channelName;
	std::string channelPasswd;
	std::vector<Client> channelClients;
};

