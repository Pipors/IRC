#include "../includes/server.hpp"


void Server::serverInit()
{
	if(this->server_fd = socket(AF_INET, SOCK_STREAM, 0) < 3)
		throwError("Socket Failed");

	address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
	if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0)
	{
		close(server_fd);
		throwError("Bind failed");
	}
	if (listen(server_fd, 3) < 0)
	{
        close(server_fd);
        throwError("Listen failed");
    }
}

void Server::throwError(const char* msg)
{
	perror(msg);
	exit(EXIT_FAILURE);
}

void Server::acceptConnection()
{
	if ((new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0) {
        close(server_fd);
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
	close(new_socket);
    close(server_fd);
}


int Server::getServerFd() const
{
	return this->server_fd;
}