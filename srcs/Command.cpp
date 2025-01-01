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



void Command::passCommand(Client *client)
{
	if (!client->isEmptyName())
	{
		const std::string& msg = "Welcome " + client->getNickName() + " among us\r\n";
		send(client->getClientSock(), msg.c_str(), msg.size(), 0);
	}
	sendData(client->getClientSock(), "Correct password... Set the username, nickcname and realname\r\n");
	std::cout << client->getIpAddress() << " is connected\r\n";
}


const char* Command::msg(std::string hostname, std::string ipaddress, std::string channelname)
{
	std::string rep = ":" + hostname + " @" + ipaddress + " JOIN #" + channelname + "\r\n\0";
	return rep.c_str();
}

void Command::joinCommand(const std::string &param, Client *client)
{
	if (param[0] == '#')
	{
		if (channelExist(param) == false)
		{
			Channel newChannel(param);
			newChannel.setCreation(true);
			newChannel.AddUser2Channel(client);
			client->isModerator(true);
			this->channels.push_back(newChannel);

			const std::string& create = RPL_CREATIONTIME(client->getNickName(), newChannel.getChannelName(), newChannel.getCurrentTimestamp());
			const std::string& msg = ":" + client->getNickName() + "!" + client->getUserName() + "@" + client->getIpAddress() + ".IP JOIN " + param + " * " + client->getRealName() + " :" + create + "\r\n";
			send(client->getClientSock(), msg.c_str(), msg.size(), 0);  // The msg appears in the server interface on hexchat not the channel
		}
		else if (channelExist(param) == true)
		{
			getChannelByName(param)->AddUser2Channel(client);
			const std::string& welcome = client->getUserName() + " has joined the channel " + param + "\r\n";
			const std::string& msg = ":" + client->getNickName() + "!" + client->getUserName() + "@" + client->getIpAddress() + ".IP JOIN " + param + " * " + client->getRealName() + " :" + welcome + "Here is the list of users in the channel " + param.substr(1) + "\n" + getChannelByName(param)->getChannelClientByName() + "\r\n";
			sendData(client->getClientSock(), msg.c_str());
			//const std::string &users =
		}
	}
	else
	{
		std::cout << "SHINRA TENSEI" << std::endl;
	}
}


