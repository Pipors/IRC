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

	void AddUser2Channel(Client*);
	void removeClientFromChannel(const Client& client);
	void resizeClientLimit(const size_t&);  //set Channel limit
	void setChannelLimit(const size_t&);
	void setHasLimit(const size_t&);
	void setInviteMode(const bool &);
	void setPasswdRequired(const bool&);
	void setPasswd(const std::string&);
	void setTopicMode(const bool&);
	

	bool userExistInChannelBySock(const int&);
	bool channelIsFull();
	bool userExistInChannelByName(const std::string &, const int&);
	bool getInviteMode() const;
	bool channelInviteModeOnly();
	bool getPasswdRequired() const;
				// command.getChannelByName(*(it + 1))->addClientToVector(invitedClient);
	bool getTopicMode() const;
	bool getNumberOfModerator() const;
	bool getHasLimit() const;

	std::string getChannelName() const;
	std::string getChannelClientByName();
	std::string getPasswd() const;
	std::string getCurrentTimestamp();
	std::string getChannelMode() const;

	size_t getChannelLimit() const;
	size_t getChannelClientSize();
	
	std::vector<Client> *getChannelClientsVector();



	Client *getClientFromChannelByName(const std::string& name);

private :
	bool requirePasswd;                    // for joining a channel
	bool inviteMode;
	bool topicMode;
	bool hasLimit;
	size_t channelLimit;
	std::string channelName;
	std::string channelPasswd;
	std::vector<Client> channelClients;     // Storing the clients joining a channel
};

