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
	// const std::string& _command = commandLine;

	// if (_command.find(" ") != std::string::npos)
	// 	setParameters(_command.substr(0, _command.find(" ")), _command.substr(_command.find(" ") + 1));
	// else
    // 	setParameters(_command, "");
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


// void Command::checkPasswd(std::string passwd, Client client)
// {
// 	if (this->getParameter1() == "PASS")
// 	{
// 		if (passwd != getParameter2().substr(0, strlen(getParameter2().c_str()) - 1))
// 		{
// 			close(client.getClientSock());
// 			std::cout << "Client {" << client.getClientSock() - 3 << "}" << " has been Disconnected." << std::endl;
// 		}
// 		else
// 		{
// 			std::cout << client.getRealName() << " has joined the server" << std::endl;
// 			client.setValid(true);
// 		}
// 	}
// 	return ;
// }





std::vector<std::string> Command::getWords(const std::string& str)
{
    std::vector<std::string> words;
    std::istringstream stream(str);
    std::string word;

    // Extract words using the stream extraction operator
    while (stream >> word)
	{
        words.push_back(word);
    }
    return words;
}


//void Command::parseHexChat(const std::string &hexMsg, std::string passwd, Client client)
//{
//	std::vector<std::string> vec(getWords(hexMsg));
//	
//	std::vector<std::string>::iterator it = vec.begin();
//	// for(it = vec.begin(); it != vec.end(); it++)
//	// 	std::cout << *it << std::endl;
//
//	while (it != vec.end())
//	{
//		
//		
//		if (*it == "JOIN")
//		{
//			
//		} 
//
//		it++;
//	}
//}


void Command::sendData(int newsocket, const char* msg)
{
	if (send(newsocket, msg, strlen(msg), 0) <= 0)
		std::cerr << ERR_SEND << std::endl;
}


bool Command::channelExist(const std::string& name)
{
	//std::unordered_map<std::string, Client> channelMap;

	//if (channelMap.find(name) != channelMap.end())                //which mean that the name "channel" exists in the map
	//	return true;
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

void Command::passCommand(Client client)
{
	std::string uName;
	std::string rName;
	std::string nName;
	sendData(client.getClientSock(), "Correct password... you can use other commands\r\n");
	std::cin >> uName;
	client.setNickName(uName);
	std::cin >> rName;
	client.setRealName(rName);
	std::cin >> nName;
	client.setRealName(nName);
	client.setValid(true);
	std::cout << client.getIpAddress() << "]" << " is connected\r\n";
}


const char* Command::msg(std::string hostname, std::string ipaddress, std::string channelname)
{
	std::string rep = ":" + hostname + "@" + ipaddress + " JOIN #" + channelname + "\r\n";
	return rep.c_str();
}

void Command::joinCommand(const std::string &param, Client client)
{
	const std::string& name = param.substr(1);
	if (param[0] == '#')
	{
		if (channelExist(name) == false)
		{
			const std::string& create = "Channel " + name + " has been created.\r\n";
			//send(client.getClientSock(), "msg\r\n", strlen("msg\r\n"), 0);
			sendData(client.getClientSock(), this->msg(client.getUserName(), client.getIpAddress(), param.c_str()));
			Channel newChannel(name);
			newChannel.setCreation(true);
			newChannel.fillChannelClients(client);
			client.isModerator(true);
			this->channels.push_back(newChannel);
		}
		else if (channelExist(name) == true)
		{
			const std::string& msg = client.getUserName() + " has joined the channel " + name + "\r\n";
			sendData(client.getClientSock(), msg.c_str());
		}
	}
	else
	{
		std::cout << "SHINRA TENSEI" << std::endl;
	}
}
