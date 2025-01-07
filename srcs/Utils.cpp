#include "../includes/Server.hpp"


void Server::processCommand(Client* client, const char* message)
{
	
	const std::string& msg = message;
	std::vector<std::string> vec(getWords_(msg));               // Splitting the message sent by client word by word
	std::vector<std::string>::iterator it = vec.begin();        // Iterating through the vector containing the message  
	std::cout << message;

	while ((it != vec.end()))
	{
		notCommand(*it);

		if (equalStrings(*it, "PASS"))
		{
			if (!getPasswd().empty() && ((it + 1) == vec.end() || !equalStrings(*(it + 1), getPasswd())))
			{
				const std::string& msg = ERR_PASSWDMISMATCH(*(it + 1));
				command.sendData(client->getClientSock(), msg);
			}
			else if (getPasswd().empty() || (equalStrings(*(it + 1), getPasswd())))
			{
				client->setValid(true);
				command.sendData(client->getClientSock(), RPL_WELCOME(client->getNickName(), "IRC"));
			}
		}

		if (equalStrings(*it, "USER") && client->isValid())

			client->setUserName(*(it + 1));
	

		if (equalStrings(*it, "NICK") && client->isValid())
			client->setNickName(*(it + 1));

		if (equalStrings(*it, "JOIN") && client->isEligible())
		{
			if (command.channelExist(*(it + 1)) && command.getChannelByName(*(it + 1))->getPasswdRequired() && (it + 2) == vec.end())
			{
				const std::string& msg = ":" + client->getNickName() + "!" + client->getUserName() + "@" + client->getIpAddress() + ".IP #JOIN " + ERR_CHANHASPASS(command.getChannelByName(*(it + 1))->getChannelName());
				send(client->getClientSock(), msg.c_str(), msg.size(), 0);
			}
			else
				command.joinCommand(client, *(it + 1), *(it + 2));
		}

		if (equalStrings(*it, "PRIVMSG") && client->isEligible())
		{
			const std::string& param = *(it + 1); 
			if (param[0] == '#') // channel's name
				command.privmsgCommandChannel(param, client, getRangeAsString(vec, 2, vec.size(), " "));
			else
			{
				Client *_client = getClientFromServer(param);               //client to whom the msg will be sent
				command.privmsgCommandUser(_client, getRangeAsString(vec, 2, vec.size(), " "));
			}
		}

		if (equalStrings(*it, "MODE") && client->isEligible())
		{
			// const std::string& target = ;
			// const std::string& modestring = ;
			// const std::string& arg = ;
			// if (((*(it + 1)).empty() || (*(it + 2)).empty() || (*(it + 3)).empty()))
			// {
			// 	const std::string& msg = ERR_NEEDMOREPARAMS(client->getNickName(), *it);
			// 	send(client->getClientSock(), msg.c_str(), msg.size(), 0);
			// 	return ;
			// }
			// std::cout << "HOOOOOOO " << std::endl;
			if ((it + 1) != vec.end() &&  (it + 2) != vec.end() &&  (it + 3) != vec.end())
			{
				command.modeCommand(client, *(it + 1), *(it + 2), *(it + 3));
			}
		}

		if (*it == "QUIT" && *(it + 1) == ":Leaving")
		{
			close(client->getClientSock());
			return;
		}
		it++;
	}
}