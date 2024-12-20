#include "../includes/Server.hpp"

Server::Server()
{
	this->addrlen = sizeof(this->serverAddr);
	strcpy(this->buffer, "anas");
}

Server::~Server()
{
	close(newSocket);
    close(serverSock);
}

/* Creating the server and making it ready to recieve the incoming connections */
void Server::serverInit(int port)
{
	struct pollfd serverPoll;

	this->serverSock = socket(AF_INET, SOCK_STREAM, 0);
	if(this->serverSock < 3)
		throwError("Socket Failed");

	serverAddr.sin_family = AF_INET; // for IP v4
    serverAddr.sin_addr.s_addr = INADDR_ANY; // any address
    serverAddr.sin_port = htons((uint16_t)port); //port to communicate from

	int optval = 1;
	if (setsockopt(serverSock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
		std::cout << "chi l3ayba tema " << std::endl;
	
	if (bind(serverSock, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0)
	{
		close(serverSock);
		throwError("Bind failed");
	}

	if (listen(serverSock, SOMAXCONN) == -1)
	{
        close(serverSock);
        throwError("Listen failed");
    }
	serverPoll.fd = serverSock;
	serverPoll.events = POLLIN;
	serverPoll.revents = 0;

	this->monitor.push_back(serverPoll);
}


void Server::throwError(const char* msg)
{
	perror(msg);
	exit(EXIT_FAILURE);
}

/* Accepting the incoming connection from the clients */
void Server::acceptNewConnection()
{
	Client client;
	struct sockadd_in sockAddrClient;
	struct pollfd clientPoll;

 	newSocket = accept(serverSock, (struct sockaddr*)&sockAddrClient, (socketlen_t)sizeof(sockAddrClient));
	if (newSocket < 0) {
        close(serverSock);
        throwError("Conenction failed");
	}
	else 
		std::cout << "Connection is established successfuly" << std::endl;
	

	clientPoll.fd = newSocket;
	client.events = POLLIN; 

	client.setIp(inet_ntoa(sockAddrClient.sin_addr));
	client.setClientSock(newSocket);
	this->clients.push_back(client);
	this->monitor.push_back(clientPoll);
}

/* Reading msg from client */
void Server::recieveData(int newsocket)
{
	char message[1024];
	int rbyte  = recv(newSocket, message, sizeof(message));
    std::cout << "Message from client: " << message << std::endl;
}

/* Sending msg to client */
void Server::sendData(const char* msg)
{
	send(newSocket, msg, strlen(msg), 0);
    std::cout << "Message sent to client" << msg << std::endl;
}
void Server::closeFd()
{

}


int Server::getServerFd() const
{
	return this->serverSock;
}