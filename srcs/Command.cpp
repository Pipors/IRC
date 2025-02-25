#include "../includes/Command.hpp"


Command::Command()
{

}
Command::~Command()
{

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
	// else
	// {
	// 	std::cout << "mouchkil fia" << std::endl;
	// 	return;
	// }
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
	{
		const std::string& msg = ERR_PASSWDMISMATCH(passwd);
		send(client->getClientSock(), msg.c_str(), msg.size(), 0);
		return ;
	}
	client->setValid(true);
	sendData(client->getClientSock(), RPL_WELCOME(client->getNickName(), "IRC"));
}


std::string Command::standardMsg(std::string nick, std::string user, std::string ipaddress)
{
	std::string rep = ":" + nick + "!" + user + "@" + ipaddress + ".IP";
	return rep;
}


bool Command::invalidNickName(const std::string& nick)
{
	size_t i = 0; 
	while (i <= nick.size())
	{
		if (nick[i] == ' ' || nick[i] == ',' || nick[i] == '@'
			|| nick[i] == '#' || nick[i] == '?' || nick[i] == '!'
			|| nick[i] == ':' || nick[i] == '.' || nick[i] == '*' || nick.size() == 1)
			return true;
		i++;
	}
	return false;
}


void Command::modeCommand(Client *client, const std::string&target, const std::string& modestring, const std::string& arg) // modestring is the word after the channel name "target" : +/-i,t,l,o,k
{
	Channel *channel = getChannelByName(target);

	if (!channel)
		return;

	if (channel->checkClientIsModerator(client->getClientSock()) == false)
	{
		const std::string &msg = ":IRC " + ERR_CHANOPRIVSNEEDED(client->getNickName(), channel->getChannelName());
		send(client->getClientSock(), msg.c_str(), msg.size(), 0);
		return;
	}
	

	if (!modestring.empty())
	{
		size_t i = 0;
		std::string msg = "";
		const std::string& test = modestring.substr(1);
		switch (modestring[0])
		{
			case '+':
			while (i != test.size())
			{
				switch (test[i])
				{
					case 'i':
					channel->setInviteMode(true);
					msg = standardMsg(client->getNickName(), client->getUserName(), client->getIpAddress()) + " MODE " + channel->getChannelName()  + " +i\r\n";
					sendData(client->getClientSock(), msg.c_str());
					break;

					case 'k':
					if (channel->getPasswdRequired())
					{
						msg = ":IRC " + ERR_KEYSET(client->getNickName(), channel->getChannelName());
						sendData(client->getClientSock(), msg.c_str());
					}
					if (!channel->getPasswdRequired())
					{
						channel->setPasswd(arg);
						channel->setPasswdRequired(true);
						msg = standardMsg(client->getNickName(), client->getUserName(), client->getIpAddress()) + " MODE " + channel->getChannelName() + " +k " + arg + "\r\n";	
						sendData(client->getClientSock(), msg.c_str());
					}
					break;

					case 'l': //redefine the limit number of client in a channel
					if (channel->getChannelName()[0] == '#')
					{
						channel->setHasLimit(true);
						channel->resizeClientLimit(atoi(arg.c_str()));
						msg = standardMsg(client->getNickName(), client->getUserName(), client->getIpAddress()) + " MODE " + channel->getChannelName() + " +l " + arg + "\r\n";	
						sendData(client->getClientSock(), msg.c_str());
					}
					else
					{
						const std::string& msg = standardMsg(client->getNickName(), client->getUserName(), client->getIpAddress()) + " JOIN " + ERR_NEEDMOREPARAMS(client->getNickName(), "JOIN") ;
						sendData(client->getClientSock(), msg);
					}
					break;

					case 'o':
					// channel->sendToAll(msg);
					if (channel->getClientFromChannelByName(arg) == NULL)
                    {
                        msg =  ":IRC " + ERR_USERNOTINCHANNEL(client->getNickName(), arg, channel->getChannelName());
                        // sendData(client->getClientSock(), msg.c_str());
                    }
                    else if (channel->checkClientIsModerator(client->getClientSock()))
                    {
                        msg =  ":IRC " + ERR_USERNOTINCHANNEL(client->getNickName(), arg, channel->getChannelName());
                        // sendData(client->getClientSock(), msg.c_str());
                    }
                    else
                    {
                        channel->getClientFromChannelByName(arg)->setModerator(true);
                        msg = standardMsg(client->getNickName(), client->getUserName(), client->getIpAddress()) + " MODE " + channel->getChannelName() + " +o " + arg + "\r\n";
                    }
                    channel->sendToAll(msg);
                    break;
					case 't':
					if (channel->getTopicMode() == false && channel->checkClientIsModerator(client->getClientSock()))
					{
						std::cout << "huwa\n";
						channel->setTopicMode(true);
						msg = standardMsg(client->getNickName(), client->getUserName(),client->getIpAddress()) + " MODE " + channel->getChannelName() + " +t\r\n";
						sendData(client->getClientSock(), msg);
					}
					break;
				}
				i++;
			}
			break;
			case '-':
			while (i != test.size())
			{
				switch (test[i])
				{
					case 'i':
					channel->setInviteMode(false);
					msg = standardMsg(client->getNickName(), client->getUserName(), client->getIpAddress()) + " MODE " + channel->getChannelName() + " -i\r\n";
					sendData(client->getClientSock(), msg.c_str());
					break;

					case 'k':
					channel->setPasswdRequired(false);
					break;

					case 'l': //redifine the limit number of client in a channel
					if (channel->getChannelName()[0] == '#')
					{
						channel->setHasLimit(false);
						channel->resizeClientLimit(100);
					}
					break;

					case 'o':
					channel->getClientFromChannelByName(arg)->setModerator(false);
					break;

					case 't':
					if (channel->getTopicMode())
					{
						channel->setTopicMode(false);
						msg = standardMsg(client->getNickName(), client->getUserName(),client->getIpAddress()) + " MODE " + channel->getChannelName() + " -t\r\n";
						sendData(client->getClientSock(), msg);
					}
					break;
				}
				i++;
			}
			break;
		}
	}
	// rpl_list(client, channel);
	return ;
}

void Command::rpl_list(Client *client, Channel *channel)
{

    std::string reply_message;
    reply_message += ":" + client->getIpAddress() + " 353 " + client->getNickName() + " = ";
    reply_message += channel->getChannelName() + " :";
    std::vector<Client>* Clients = channel->getChannelClientsVector();
    std::vector<Client>::iterator it = Clients->begin();
    // for( it =  Clients->begin() ; it != Clients->end(); it++)
	while(it != Clients->end())
    {
        if(channel->checkClientIsModerator(client->getClientSock()))
      		reply_message += "@";
        reply_message += it->getNickName() + " ";
		it++;
    }
	reply_message += "\r\n";
    sendData(client->getClientSock(),reply_message);
    reply_message = ":" + client->getIpAddress() + RPL_ENDOFNAMES(client->getNickName(), channel->getChannelName());
    sendData(client->getClientSock(),reply_message);
}

void Command::joinCommand(Client *client, const std::string &param, const std::string& passwd)
{
	if (channelExist(param) == false)
	{
		Channel newChannel(param);
		newChannel.addModerator(*client);
		client->setModerator(true);
		newChannel.AddUser2Channel(client);
		this->channels.push_back(newChannel);

		const std::string& msg = standardMsg(client->getNickName(), client->getUserName(), client->getIpAddress()) + " JOIN " + param + " * :" + client->getRealName() + "\r\n";
		send(client->getClientSock(), msg.c_str(), msg.size(), 0);
	}
	else if (channelExist(param))
	{
		Channel *channel = getChannelByName(param);

		if (channel == NULL)
			return; //!!

		if (channel->userExistInChannelByName(client->getNickName(), client->getClientSock()))
		{
			const std::string& msg = ":IRC " + ERR_NICKNAMEINUSE(client->getNickName(), client->getNickName());
			sendData(client->getClientSock(), msg);
			return ; //!!
		}

		if (channel->userExistInChannelBySock(client->getClientSock()))
		{
			const std::string& msg = "IRC :You are already a member of " + channel->getChannelName() + "\r\n";
			sendData(client->getClientSock(), msg);
			return ; //!!
		}

		if (channel->channelInviteModeOnly())
		{
			sendData(client->getClientSock(),  ERR_INVITEONLYCHAN(client->getNickName(), channel->getChannelName()));
			return; //!!
		}

		if (channel->getPasswdRequired())
		{
			if (strncmp(passwd.c_str(), channel->getPasswd().c_str(), channel->getPasswd().size()) != 0)
			{
				sendData(client->getClientSock(), ERR_PASSWDMISMATCH(client->getNickName()));
				return; //!!
			}
		}
	
		if (channel->channelIsFull())
		{
			sendData(client->getClientSock(), ERR_CHANNELISFULL(client->getNickName(), channel->getChannelName()));
			return; //!!
		}
		
		if (channel->getInviteMode())
		{
			const std::string& msg = ":IRC" + ERR_INVITEONLYCHAN(client->getNickName(), channel->getChannelName());
			send(client->getClientSock(), msg.c_str(), msg.size(), 0);
			return; //!!
		}
		
			client->setModerator(false);
			channel->AddUser2Channel(client);
			const std::string& msg = standardMsg(client->getNickName(), client->getUserName(), client->getIpAddress()) + " JOIN " + param + " * " + client->getRealName() + "\r\n" ;
			sendData(client->getClientSock(), msg.c_str());
			return ; //!!
		// return;
	}	

}


Channel *Command::getChannelByName(const std::string& name)
{
	for (size_t i = 0; i != channels.size(); i++)
	{
		if (channels[i].getChannelName() == name)
		{
			return &(channels[i]);
		}
	}
	return NULL;
}

// The msg send in the channel
// forward the message to all clients added to this channel
void Command::privmsgCommandChannel(const std::string &channelname, Client *client, const std::string& tosend)
{
	// eligibiltyErr(client, "");

	//Point to the channel in where the message were sent
	Channel *channel = getChannelByName(channelname);
	if (channel == NULL || channel->getClientFromChannelByName(client->getNickName()) == NULL)
	{
		const std::string& msg =  ":" + client->getNickName() + "!" + client->getUserName() + "@" + client->getIpAddress() + " PRIVMSG " + channelname + " " + ERR_NOSUCHCHANNEL(client->getNickName(), channelname);
		sendData(client->getClientSock(), msg);
		return ;
	}
	// if ()
	// {
	// 	const std::string& msg =  ":" + client->getNickName() + "!" + client->getUserName() + "@" + client->getIpAddress() + " PRIVMSG " + channelname + " " + ERR_USERNOTINCHANNEL(client->getNickName(), channelname);
	// 	sendData(client->getClientSock(), msg);
	// 	return ;
	// }
	
	//Point to the client whom sent the message
	Client *_client = channel->getClientFromChannelByName(client->getNickName());
	if ( _client == NULL)
	{
		const std::string& msg =  ":" + _client->getNickName() + "!" + _client->getUserName() + "@" + _client->getIpAddress() + " PRIVMSG " + channelname + " " + ERR_USERNOTINCHANNEL(_client->getNickName(), _client->getNickName(), channel->getChannelName());
		sendData(_client->getClientSock(), msg);
		return ;
	}
	const std::string& msg =  ":" + client->getNickName() + "!" + client->getUserName() + "@" + client->getIpAddress() + " PRIVMSG " + channelname + " " + tosend + "\r\n";
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
// The message is send to a user
// look for the user by his nickname and send the msg to him
void Command::privmsgCommandUser(Client *sender, Client *client, const std::string& tosend)
{
	// eligibiltyErr(client, "");
	// if (!client)
	// {
	// 	const std::string& msg = ":IRC" + ERR_NOSUCHNICK(client->getNickName(), client->getNickName());
	// 	send(client->getClientSock(), msg.c_str(), msg.size(), 0);
	// 	return ;
	// }
	const std::string& msg =  standardMsg(sender->getNickName(), sender->getUserName(), sender->getIpAddress()) + " PRIVMSG " + client->getNickName() + " " + tosend + "\r\n";
	sendData(client->getClientSock(),  msg.c_str());
	return ;
}



void Command::eligibiltyErr(Client *client, const std::string& msg)
{
	if (!client->isEligible())
		send(client->getClientSock(), msg.c_str(), msg.size(), 0);
	return;
}

void Command::removeClientFromAllChannels(const int& toremove)
{
	std::vector<Channel>::iterator it = channels.begin();

	while (it != channels.end())
	{
		std::vector<Client>::iterator c_it = it->getChannelClientsVector()->begin();
		while (c_it != it->getChannelClientsVector()->end())
		{
			if (c_it->getClientSock() == toremove)
				it->getChannelClientsVector()->erase(c_it);
			c_it++;
		}
		it++;
	}
}

void Command::kickCommand(Client *client,  const std::string& channelName, const std::string& clientName, Client *clientToKick, std::string m)
{
	Channel *channel = getChannelByName(channelName);
	if(!channel)
	{
		const std::string &msg = ":IRC " + ERR_NOSUCHCHANNEL(client->getNickName(), channelName);
		sendData(client->getClientSock(), msg.c_str());
		return ;
	}
	if(clientinthechannel(channelName, clientToKick->getNickName())!= 0)
	{
		const std::string  &msg = ":IRC " + ERR_USERNOTINCHANNEL(client->getNickName(), clientName, channelName)  ;
		sendData(client->getClientSock(), msg);
		return ;
	}
	if(!clientToKick)
	{
		const std::string  &msg = ":IRC " + ERR_USERNOTINCHANNEL(client->getNickName(), clientName, channelName)  ;
		sendData(client->getClientSock(), msg);
		return ;
	}

	if(channel->checkClientIsModerator(client->getClientSock()) == false)
	{
		const std::string &msg = ":IRC " + ERR_CHANOPRIVSNEEDED(client->getNickName(), channelName);
		sendData(client->getClientSock(), msg);
		return ;
	}
	int j = clientinthechannel(channelName, clientName);

	if(j == 3)
	{
			const std::string  &msg = standardMsg(client->getNickName(), client->getUserName(), client->getIpAddress()) + ERR_USERNOTINCHANNEL(client->getNickName(), clientName, channelName);
			sendData(client->getClientSock(), msg);
			return ;
	}

	int h = userinthechannel(client, channelName, clientName);
	if(h == 3)
	{
		const std::string  &msg = ":IRC " + ERR_USERNOTINCHANNEL(client->getNickName(), clientName, channelName);
		sendData(client->getClientSock(), msg);
		return ;
	}
	if(h == 2)
	{
		const std::string &msg = ": IRC : Client Can't kick himself \n";
		sendData(client->getClientSock(), msg);
		return;	
	}
	int k = kickClientFromChannel(channelName,clientToKick);
	if (k != 0)
	{
		const std::string &msg = ":IRC " + ERR_USERNOTINCHANNEL(client->getNickName(), clientName, channelName);
		sendData(client->getClientSock(), msg);
		return ;
	}
		const std ::string &msg =  standardMsg(client->getNickName(), client->getUserName(), client->getIpAddress()) + " : IRC : Client kicked successfully \r\n";
		sendData(client->getClientSock(), msg);

		const std::string &sg = standardMsg(clientToKick->getNickName(), clientToKick->getUserName(), clientToKick->getIpAddress()) + " KICK " + channelName + " * " + clientToKick->getRealName() + " :Kicked by " + client->getNickName() + " REASON :"+ m +"\r\n";
		// sendData(clientToKick->getClientSock(), sg);
		sendData(clientToKick->getClientSock(), sg);
		channel->sendToAll(sg);
		return;
}

void Command::partCommand(Client *client,const std::string &channelName, const std::string &message)
{
	(void)message;
		Channel *channel = getChannelByName(channelName);
	std::cout << "44\n";
	if(!channel)
	{
		const std::string &msg = ":IRC " + ERR_NOSUCHCHANNEL(client->getNickName(), channelName);
		sendData(client->getClientSock(), msg.c_str());
		return ;
	}
			std::cout << "55\n";
	if(clientinthechannel(channelName, client->getNickName()) != 0)
	{
			const std::string  &msg = standardMsg(client->getNickName(), client->getUserName(), client->getIpAddress()) + " " + ERR_USERNOTINCHANNEL(client->getNickName(), client->getNickName(), channelName);
			sendData(client->getClientSock(), msg);
			return ;
	}
			std::cout << "33\n";

	std::vector<Client>* vec = channel->getChannelClientsVector();
	std::vector<Client>::iterator it = vec->begin();
	if(channel->checkClientIsModerator(client->getClientSock()))
	{
			std::cout << "232\n";

		int k = 0;
		while(it != vec->end())
		{
			if(channel->checkClientIsModerator(client->getClientSock()))
				k++;
			it++;

		}
		if(k == 1)
		{
			it = vec->begin();
			while(it != vec->end())
			{
				if( it->getNickName() == client->getNickName())
				{
					if((it+1) != vec->end())
						(it + 1)->setModerator(true);
					// else if (it - 1 != vec->end())
					// {
					// 		(it - 1)->setModerator(true);
					// // }
					// else 
					// 	return;
				}
				it++;
			}
		}
	}	
			std::cout << "122\n";
	const std::string &sg = standardMsg(client->getNickName(), client->getUserName(), client->getIpAddress()) + " PART  " + channelName + " * " + client->getRealName() +" : " + client->getNickName() + " HAS PARTED " + " REASON :"+ message +"\r\n";
	// std::string msg = ":" + client->getNickName() + "!" + client->getUserName() + "@" + client->getIpAddress() + " PART " + channelName + "\r\n";
	sendData(client->getClientSock(), sg);
	while(it != vec->end())
	{
		sendData(it->getClientSock(), sg);
		it++;
	}
	channel->removeClientFromChannel(*client);
}



int Command::kickClientFromChannel(const std::string &chaine, Client *client)
{
	Channel *channel = getChannelByName(chaine);
	if(!channel)
		return 1;
	channel->removeClientFromChannel(*client);
		return 0;
}
int Command::userinthechannel(Client* client, std::string const &name, std::string const &usname)
{
	// int i = 0;
	(void)client;
	if(client->getNickName() == usname)
		return 2;
	Channel *channel = getChannelByName(name);
	if(!channel)
		return 1;
	std::vector<Client>* vec = channel->getChannelClientsVector();

	for (std::vector<Client>::iterator ite = vec->begin(); ite != vec->end(); ++ite) 
    {
        if (usname == ite->getNickName())
            return 0;
    }
	return 3;
	// command.getChannelByName(name)->userInChannel(client->getclinetByName())
}
int Command::clientinthechannel(std::string const &chaine, std::string const &name)
{
	Channel *channel = getChannelByName(chaine);
	if(!channel)
		return 1;
	std::vector<Client>* vec = channel->getChannelClientsVector();
	for (std::vector<Client>::iterator it = vec->begin(); it != vec->end(); ++it) 
    {
        if (name == it->getNickName())
            return 0;
    }
	return 3;
}
int Command::inviteclientcheck(Client* client, std::string const &name)
{
	Channel *channel = getChannelByName(name);
	if(!channel)
		return 1;
	// if(channel->getInviteMode() == false)
	// 	return 2;
	std::vector<Client>* vec = channel->getChannelClientsVector();

	for (std::vector<Client>::iterator it = vec->begin(); it != vec->end(); ++it) 
    {
        if (client->getNickName() == it->getNickName())
            return 3;
    }
	return 0;

}
void Command::inputClient(Client *client, std:: string name)
{
	Channel *channel = getChannelByName(name);
	std::vector<Client>* vec = channel->getChannelClientsVector();
	vec->push_back(*client);
}
void Command::sendToAll(Client *client,const std::string msg, std::string name)
{
	Channel *channel = getChannelByName(name);
	std::vector<Client>* otherClients = channel->getChannelClientsVector();
	std::cout << "1\n";
	if(!otherClients)
		return;
	std::cout << "2\n";
	size_t i = 0;
	while (i < otherClients->size())
	{
		int fd = (*otherClients)[i].getClientSock();
		if (client->getClientSock() != fd)
			send(fd, msg.c_str(), msg.size(), 0);
		i++;
	}
	// std::cout << "l7wa" << std::endl;
}