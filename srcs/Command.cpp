#include "../includes/Command.hpp"


Command::Command()
{

}
Command::~Command()
{

}


std::string Command::getCommandLine() const
{
	return this->commandLine;
}

void Command::setCommandLine(char* command)
{
	this->commandLine = command;
}

void Command::setParameters(std::string param1, std::string param2)
{
	this->parameter1 = param1;
	this->parameter2 = param2;
}


std::string Command::getParameter1() const
{
	return this->parameter1;
}

std::string Command::getParameter2() const
{
	return this->parameter2;
}




std::vector<std::string> Command::getWords(const std::string& str)
{
    std::vector<std::string> words;
    std::istringstream stream(str);
    std::string word;

    // Extract words using the stream extraction operator default delimeter ('\t' '\n' ' ')
	// to use with std::getline to customize the delimeter std::getline(iss >> word >> 'delm')
    while (stream >> word)
	{
        words.push_back(word);
    }
    return words;
}



void Command::sendData(int newsocket, const std::string& msg)
{
	if (send(newsocket, msg.c_str(), msg.size(), 0) <= 0)
		std::cerr << ERR_FUNCSEND << std::endl;
	return ;
}


bool Command::channelExist(const std::string& name)
{
	size_t i = 0;
	while (i != channels.size())
	{
		if (channels[i].getChannelName() == name)
			return true;
		i++;
	}
	return false;
}


std::vector<Channel> Command::getChannelVector() const
{
	return this->channels;
}



void Command::passCommand(Client *client, const std::string& param, const std::string &passwd)
{
	if (param != passwd)
		sendData(client->getClientSock(), "Wrong Password... Try again\r\nUsage: /PASS \"passwd\"\r\n");
	else
	{
		client->setValid(true);
		if (!client->isEmptyName(client->getNickName(), client->getUserName()))
		{
			const std::string& msg = "Welcome " + client->getNickName() + " among us\r\n";
			send(client->getClientSock(), msg.c_str(), msg.size(), 0);
		}
		sendData(client->getClientSock(), "Correct password... Set the username, nickcname and realname\r\n");
		std::cout << client->getIpAddress() << "=> " << client->getNickName() << " is connected\r\n";
	}
}


std::string Command::standardMsg(std::string nick, std::string user, std::string ipaddress)
{
	std::string rep = ":" + nick + "!" + user + " @" + ipaddress;
	return rep;
}



void Command::modeCommand(Client *client, const std::string&target, const std::string& modestring, const std::string& arg) // param is the word after MODE : +/-i,t,l,o,k
{

	if (target.empty() || modestring.empty() || arg.empty())
		return;

	Channel *channel = getChannelByName(target);
	if (!channel)
		return;
	if (client->isModerator() == false)
	{
		const std::string &msg = ":IRC " + ERR_CHANOPRIVSNEEDED(client->getNickName(), channel->getChannelName());
		send(client->getClientSock(), msg.c_str(), msg.size(), 0);
		return;
	}
	const std::string& test = modestring.substr(1);
	std::string msg ;
	size_t i = 0;
	switch (modestring[0])
	{
		case '+':
		while (i != test.size())
		{
			switch (modestring[i])
			{
				case 'i':
				channel->setInviteMode(true);
				msg = standardMsg(client->getNickName(), client->getUserName(), client->getIpAddress()) + " MODE " + channel->getChannelName() + "+i\r\n";
				sendData(client->getClientSock(), msg.c_str());
				break;

				case 'k':
				if (channel->getPasswdRequired() == true)
				{
					msg = ":IRC " + ERR_KEYSET(client->getNickName(), channel->getChannelName());
					sendData(client->getClientSock(), msg.c_str());
				}
				if (channel->getPasswdRequired() == false)
				{
					channel->setPasswd(arg);
					msg = standardMsg(client->getNickName(), client->getUserName(), client->getIpAddress()) + " MODE " + channel->getChannelName() + "+k\r\n";	
					sendData(client->getClientSock(), msg.c_str());
				}
				break;

				case 'l': //redifine the limit number of client in a channel
				if (client->isModerator() == true && channel->getChannelName()[0] == '#')
					channel->resizeClientVector(atoi(arg.c_str()));
				break;

				case 'o':
				if (channel->getClientFromChannelByName(arg) == NULL)
				{
					msg =  ":IRC " + ERR_USERNOTINCHANNEL(client->getNickName(), arg, channel->getChannelName());
					sendData(client->getClientSock(), msg.c_str());
					return;
				}
				channel->getClientFromChannelByName(arg)->setModerator(true);
				break;

				// case 't':

				// break;
			}
			i++;
		}
		break;
		case '-':
		while (i != test.size())
		{
			switch (modestring[i])
			{
				case 'i':
				channel->setInviteMode(false);
				msg= standardMsg(client->getNickName(), client->getUserName(), client->getIpAddress()) + " MODE " + channel->getChannelName() + "-i\r\n";
				sendData(client->getClientSock(), msg.c_str());
				break;

				case 'k':
				channel->setPasswdRequired(false);
				break;

				case 'l': //redifine the limit number of client in a channel
				if (client->isModerator() == true && channel->getChannelName()[0] == '#')
					channel->resizeClientVector(atoi(arg.c_str()));
				break;

				case 'o':
				channel->getClientFromChannelByName(arg)->setModerator(false);
				break;

				// case 't':

				// break;
			}
			i++;
		}
		break;
	}
}




void Command::joinCommand(const std::string &param, const std::string& passwd, Client *client)
{
	if (param[0] == '#')
	{
		if (channelExist(param) == false)
		{
			Channel newChannel(param);
			newChannel.AddUser2Channel(client);
			client->setModerator(true);
			this->channels.push_back(newChannel);

			const std::string& msg = ":" + client->getNickName() + "!" + client->getUserName() + "@" + client->getIpAddress() + ".IP JOIN " + param + " * " + client->getRealName() + " :" + RPL_CREATIONTIME(client->getNickName(), newChannel.getChannelName(), newChannel.getCurrentTimestamp()) + "\r\n";
			send(client->getClientSock(), msg.c_str(), msg.size(), 0);  // The msg appears in the server interface on hexchat not the channel
		}
		else if (channelExist(param) == true)
		{
			Channel *channel = getChannelByName(param);

			if (channel->channelInviteModeOnly() == true)
			{
				const std::string& msg = ERR_INVITEONLYCHAN(client->getNickName(), channel->getChannelName());
				send(client->getClientSock(), msg.c_str(), msg.size(), 0);
				return;
			}

			if (channel->getPasswdRequired() == true)
			{
				if (strncmp(passwd.c_str(), channel->getPasswd().c_str(), channel->getPasswd().size()) != 0)
				{
					const std::string& msg = ERR_PASSWDMISMATCH(client->getNickName());
					send(client->getClientSock(), msg.c_str(), msg.size(), 0);
				}
				return;
			}

			if (channel->channelIsFull() == true)
			{
				const std::string& msg = ":IRC" + ERR_CHANNELISFULL(client->getNickName(), channel->getChannelName());
				send(client->getClientSock(), msg.c_str(), msg.size(), 0);
				return;
			}

			if (channel->userExist(client->getNickName(), client->getClientSock()) || 
				channel->userExist(client->getUserName(), client->getClientSock()) ||
				channel->userExist(client->getIpAddress(), client->getClientSock()))
			{
				const std::string& msg = ":IRC " + ERR_NICKNAMEINUSE(client->getNickName(), client->getNickName());
				send(client->getClientSock(), msg.c_str(), msg.size(), 0);
				return ;
			}

			if (channel->getInviteMode() == true)
			{
				const std::string& msg = ":IRC" + ERR_INVITEONLYCHAN(client->getNickName(), channel->getChannelName());
				send(client->getClientSock(), msg.c_str(), msg.size(), 0);
				return;
			}

			else
			{
				channel->AddUser2Channel(client);
				const std::string& msg = ":" + client->getNickName() + "!" + client->getUserName() + "@" + client->getIpAddress() + ".IP JOIN " + param + " * " + client->getRealName() + " :" + RPL_WELCOME(client->getNickName(), "IRC")	 + "Here is the list of users in the channel " + param.substr(1) + "\n" + channel->getChannelClientByName() + "\r\n";
				sendData(client->getClientSock(), msg.c_str());
				return ;
			}
		}
	}
	else
	{
		const std::string& msg = ":IRC" + ERR_NOSUCHCHANNEL(client->getNickName(), getChannelByName(param)->getChannelName());
		send(client->getClientSock(), msg.c_str(), msg.size(), 0);	
	}
}


Channel *Command::getChannelByName(const std::string& name)
{
	for (size_t i = 0; i != channels.size(); i++)
	{
		if (channels[i].getChannelName() == name)
			return &(channels[i]);
	}
	return NULL;
}

// The msg send in the channel
// forward the message to all clients added to this channel
void Command::privmsgCommandChannel(const std::string &param, Client *client, const std::string& tosend)
{
	if (param[0] == '#')
	{
		//Point to the channel in where the message were sent
		Channel *channel = getChannelByName(param);
		if (channel == NULL)
			return ;
		const std::string& msg = ":" + client->getNickName() + "!" + client->getUserName() + "@" + client->getIpAddress() + " PRIVMSG " + param + tosend + "\r\n";
		//Point to the channelClient vector in Channel
		std::vector<Client>* otherClients = channel->getChannelClientsVector();
		size_t i = 0;
		while (i < otherClients->size())
		{
			int fd = (*otherClients)[i].getClientSock();
			if (client->getClientSock() != fd)
				send(fd, msg.c_str(), msg.size(), 0);
			i++;
		}		
	}
}
// The message is send to a user
// look for the user by his nickname and send the msg to him
void Command::privmsgCommandUser(Client *client, const std::string& tosend)
{
	if (!client)
	{
		const std::string& msg = ":IRC" + ERR_NOSUCHNICK(client->getNickName(), client->getNickName());
		send(client->getClientSock(), msg.c_str(), msg.size(), 0);
		return ;
	}
	else 
	{
		const std::string& msg =  standardMsg(client->getNickName(), client->getUserName(), client->getIpAddress()) + " PRIVMSG " + client->getNickName() + tosend + "\r\n";
		sendData(client->getClientSock(),  msg.c_str());
		return ;
	}
}



