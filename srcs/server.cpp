#include "../includes/Server.hpp"

Server::Server()
{
}

Server::~Server()
{
}

/* Creating the server and making it ready to recieve the incoming connections */
void Server::setServerSock(int port)
{
	struct pollfd serverPoll;
	int optval = 1;

	this->serverSock = socket(AF_INET, SOCK_STREAM, 0);

	if(this->serverSock < 3)
		throwError("Socket Failed", 0);

	int sockopt = setsockopt(serverSock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
	if (sockopt < 0)
		std::cout << "chi l3ayba tema " << std::endl;
	
	// memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET; // for IP v4
    serverAddr.sin_addr.s_addr = INADDR_ANY; // any address
    serverAddr.sin_port = htons((uint16_t)port); //port to communicate from

	int _bind = bind(serverSock, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if (_bind == -1)
	{
		throwError("Bind failed => ", serverSock);
	}

	if (listen(serverSock, SOMAXCONN) == -1)
	{
        throwError("Listen failed", serverSock);
    }
	serverPoll.fd = serverSock;
	serverPoll.events = POLLIN;
	// serverPoll.revents = 0;

	this->monitor.push_back(serverPoll);
}


void Server::throwError(const char* msg, int fd)
{
	close(fd);
	perror(msg);
	exit(EXIT_FAILURE);
}

/* Accepting the incoming connection from the clients */
void Server::acceptNewConnection()
{
	Client client;
	int newSocket;
	struct sockaddr_in sockAddrClient;  // The information about the client (IPv4/v6, port) will be filled in this struct
	struct pollfd clientPoll;
	socklen_t len = sizeof(sockAddrClient);

 	newSocket = accept(serverSock, (struct sockaddr*)&sockAddrClient, &len);
	if (newSocket < 0) {
        throwError("Conenction failed", serverSock);
	}
	// else 
	// 	std::cout << "Connection is established successfuly" << std::endl;
	

	clientPoll.fd = newSocket;
	clientPoll.events = POLLIN; 

	client.setIp(inet_ntoa(sockAddrClient.sin_addr));
	client.setClientSock(newSocket);
	this->clients.push_back(client);
	this->monitor.push_back(clientPoll);
}

/* Reading msg from client */
/* newsocket is the one of the sending part "client" */
void Server::recieveData(int newsocket)
{
	char message[1024];
	int rbyte  = recv(newsocket, message, strlen(message), 0);
	message[rbyte] = '\0';
	if (rbyte < 0)
    	throwError("Error occured", newsocket);
	std::cout << message;
	command.setCommand(message);
	command.parseCommand(message);
}


/* Sending msg to client */
void Server::sendData(int newsocket, const char* msg)
{
	send(newsocket, msg, strlen(msg), 0);
    std::cout << "Message sent to client" << msg << std::endl;
}



void Server::closeFd()
{
	for(size_t i = 0; i < monitor.size(); i++)
		close(monitor[i].fd);
}



int Server::getServerFd() const
{
	return this->serverSock;
}



std::vector<struct pollfd> Server::getMonitor() const
{
	return this->monitor;
}


uint16_t Server::getMonitorSize() const
{
	return this->monitor.size();
}


void Server::runningServer(int port)
{
	this->setServerSock(port);

	while(true)
	{
		if(poll(&(monitor[0]), monitor.size(), 0) == -1)
			this->throwError("poll failed\n", 0);

		for(size_t i = 0; i < monitor.size(); i++)
		{
			if (monitor[i].revents & POLLIN)
			{
				
				if (monitor[i].fd == getServerFd())
					acceptNewConnection();
				else
					recieveData(monitor[i].fd);
			}
		}
	}
	this->closeFd();
}