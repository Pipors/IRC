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

uint16_t Command::getCommandLen(const std::string& command) const
{
	return (strlen(command.c_str()));
}

void Command::setCommand(std::string command)
{
	this->command = command;
}


void Command::parseCommand(std::string _command)
{
	_command = this->getCommand();
	std::string line;

	if (command[0] == '/')
	{
		std::cout << "Enter your command:\nINVITE\nKICK\nMODE\nTOPIC\n";
		std::cin >> line;
		if (line == "INVITE")
			std::cout << "in";
		else if (line == "KICK")
			std::cout << "ki";
		else if (line == "MODE")
			std::cout << "mo";
		else if (line == "TOPIC")
			std::cout << "to";
		else
			std::cout << "Walo ben 3ami" << std::endl;
	}
}