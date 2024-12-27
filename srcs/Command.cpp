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


void Command::parseHexChat(const std::string &hexMsg, std::string passwd, Client client)
{
	std::vector<std::string> vec(getWords(hexMsg));
	
	std::vector<std::string>::iterator it = vec.begin();
	// for(it = vec.begin(); it != vec.end(); it++)
	// 	std::cout << *it << std::endl;

	while (it != vec.end())
	{
		if (*it == "PASS")
		{
			if (*(it + 1) == passwd)
			{
				client.setValid(true);
				if (sendData(client.getClientSock(), "Correct password... you can use other commands\r\n") <= 0)
					std::cout << "SENDING ERROR OCCURED";
				std::cout << client.getIpAddress() << "]" << " is connected\r\n";
			}
			else 
			{
				sendData(client.getClientSock(), "Wrong Password... Disconnecting\r\n");
				close(client.getClientSock());
				std::cout << "Client {" << client.getClientSock() - 3 << "}" << " has been Disconnected." << std::endl;
			}
		}
		if (*it == "NICK")
			client.setNickName(*(it + 1));

		if (*it == "USER")
			client.setUserName(*(it + 1));
		if (*it == "JOIN")
		{
			const std::string	&name = *(it + 1);
			//const std::string& sub = (it + 1)->substr(1);
			if (name[0] == '#')
			{
				Channel newChannel(name);
				this->channels.push_back(newChannel);
				if (newChannel.setCreation() == true)
				{

					//create channel
				//else dont create it !
				//add user to it !
				}
			}
			else 
				std::cout << "!!!!!!!!!!!!!!!!!" << std::endl;
		} 

		it++;
	}
}


int Command::sendData(int newsocket, const char* msg)
{
	int s = send(newsocket, msg, strlen(msg), 0);
	return s;
}