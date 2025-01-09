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
			std::cout << 	getRangeAsString(vec, 1, vec.size(), " ") << " - size : " << getRangeAsString(vec, 1, vec.size(), " ").size() << "\n";
		

			if ((!getPasswd().empty() && (it + 1) == vec.end()) || !equalStrings(getRangeAsString(vec, 1, vec.size(), " "), getPasswd()))
			{
				const std::string& msg = ERR_PASSWDMISMATCH(*(it + 1));
				command.sendData(client->getClientSock(), msg);
			}
			else if (getPasswd().empty() || (equalStrings(getRangeAsString(vec, 1, vec.size(), " "), getPasswd())))
			{
				client->setValid(true);
				command.sendData(client->getClientSock(), RPL_WELCOME(client->getNickName(), "IRC"));
			}
		}

		if (equalStrings(*it, "USER") && client->isValid())
		{
			if (emptyParam(vec, (it + 1), client->getClientSock(), ERR_NEEDMOREPARAMS(client->getNickName(), *it)))
				return;
			client->setUserName(*(it + 1));
		}
	

		if (equalStrings(*it, "NICK") && client->isValid())
		{
			if (emptyParam(vec, (it + 1), client->getClientSock(), ERR_NEEDMOREPARAMS(client->getNickName(), *it)))
				return;
			client->setNickName(*(it + 1));
		}

		if (equalStrings(*it, "JOIN") && client->isEligible())
		{
			Channel *chan = NULL;
			const std::string& name = *(it + 1);
			if (emptyParam(vec, (it + 1), client->getClientSock(), ERR_NEEDMOREPARAMS(client->getNickName(), *it)))
				return;
			if (name[0] != '#' || name.size() == 1)
			{
				command.sendData(client->getClientSock(), ERR_NEEDMOREPARAMS(client->getNickName(), "JOIN"));
				return;
			}
			std::cout << command.channelExist(name) << "\n"; 
			chan = command.getChannelByName(name);
			if (chan != NULL)
			{
				std::cout << chan->getPasswdRequired() << "\n";
				if (chan->getPasswdRequired())
				{
					std::cout << "sdasdas\n";
					if ((it + 2) == vec.end())
					{
						command.sendData(client->getClientSock(), ERR_CHANHASPASS(chan->getChannelName()));
						return;
					}
				}
			}
			command.joinCommand(client, name, *(it + 2));
		}

		if (equalStrings(*it, "PRIVMSG") && client->isEligible())
		{
			if (emptyParam(vec, (it + 1), client->getClientSock(), ERR_NEEDMOREPARAMS(client->getNickName(), *it)))
				return;
				
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