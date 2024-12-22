#include "../includes/Command.hpp"


Command::Command()
{

}
Command::~Command()
{

}
std::string Command::getCommand() const
{
	return this->command;
}

void Command::init(char * message)
{

	std::string parse = strtok(message, "\r\n");

	while (!parse.empty())
	{
		command = parse.substr(0, parse.find(" "));
		parameter = parse.substr(parse.find(" ") + 1);
		parse = strtok(message, "\r\n");	
	}
}

uint16_t Command::getCommandLen(const std::string& command) const
{
	return (strlen(command.c_str()));
}

void Command::setCommand(std::string command)
{
	this->command = command;
}

std::string Command::getParameter()
{
	return this->parameter;
}


void Command::setParameter(std::string param)
{
	this->parameter = param;
}
