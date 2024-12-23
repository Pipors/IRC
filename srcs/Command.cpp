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

void Command::setCommandLine(const char* command)
{
	this->commandLine = command;
	const std::string& _command = commandLine;

	if (_command.find(" ") != std::string::npos)
		setParameters(_command.substr(0, _command.find(" ")), _command.substr(_command.find(" ") + 1));
	else
    	setParameters(_command, "");
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


void Command::checkPasswd(std::string passwd, Client client)
{
	if (this->getParameter1() == "PASS")
	{
		if (passwd != getParameter2().substr(0, strlen(getParameter2().c_str()) - 1))
		{
			close(client.getClientSock());
			std::cout << "Client {" << client.getClientSock() - 3 << "}" << " has been Disconnected." << std::endl;
		}
		std::cout << client.getRealName() << " has joined the server" << std::endl;
		client.setValid(true);
	}
	return ;
}