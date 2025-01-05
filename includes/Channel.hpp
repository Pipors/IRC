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

	void resizeClientVector(const size_t&);
	void AddUser2Channel(Client*);
	void setInviteMode(const bool &);
	void setPasswdRequired(const bool&);
	void setPasswd(const std::string&);


	bool channelIsFull();
	bool userExist(const std::string &, int);
	bool getInviteMode();
	bool channelInviteModeOnly();
	bool getPasswdRequired() const;


	std::string getChannelName() const;
	std::string getChannelClientByName();
	std::string getPasswd() const;
	std::string getCurrentTimestamp();

	size_t getChannelClientSize();
	std::vector<Client> *getChannelClientsVector();

	Client *getClientFromChannelByName(const std::string& name);

private :
	bool requirePasswd;
	bool inviteMode;
	size_t channelLimit;
	std::string channelName;
	std::string channelPasswd;
	std::vector<Client> channelClients;     // Storing the clients joining a channel
};

