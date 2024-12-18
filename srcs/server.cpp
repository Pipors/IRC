#include "../includes/Server.hpp"

Server::Server()
{
	this->addrlen = sizeof(this->address);
	strcpy(this->buffer, "anas");
}

Server::~Server()
{
	close(new_socket);
    close(serverFd);
	std::cout << new_socket << " " << serverFd << std::endl;
}

void Server::serverInit(int port)
{
	struct pollfd fds;
	if((this->serverFd = socket(AF_INET, SOCK_STREAM, 0)) < 3)
		throwError("Socket Failed");

	address.sin_family = AF_INET; // for IP v4
    address.sin_addr.s_addr = INADDR_ANY; // any address
    address.sin_port = htons((uint16_t)port); //port to communicate from
	int optval = 1;
	if (setsockopt(serverFd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
	{
		std::cout << "chi l3ayba tema " << std::endl;
	}

	if (bind(serverFd, (struct sockaddr*)&address, sizeof(address)) < 0)
	{
		close(serverFd);
		throwError("Bind failed");
	}
	if (listen(serverFd, 3) < 0)
	{
        close(serverFd);
        throwError("Listen failed");
    }
	fds.fd = serverFd;
	fds.events = POLLIN;
	fds.revents = 0;

	this->monitor.push_back(fds);
}

void Server::throwError(const char* msg)
{
	perror(msg);
	exit(EXIT_FAILURE);
}

void Server::acceptConnection()
{
	std::cout << "server FD : " << serverFd << std::endl;
 	new_socket = accept(serverFd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
	if (new_socket < 0) {
        close(serverFd);
        throwError("Accept failed");
	}
}

void Server::readData()
{
	read(new_socket, buffer, BUFFER_SIZE);
    std::cout << "Message from client: " << buffer << std::endl;
}
void Server::sendData(const char* msg)
{
	send(new_socket, msg, strlen(msg), 0);
    std::cout << "Hello message sent to client" << std::endl;
}
void Server::closeFd()
{

}


int Server::getServerFd() const
{
	return this->serverFd;
}