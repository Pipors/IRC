#include "../includes/Server.hpp"

Server::Server()
{
	this->passwd = "";
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
	int newsocket;
	struct sockaddr_in sockAddrClient;  // The information about the client (IPv4/v6, port) will be filled in this struct
	struct pollfd clientPoll;
	socklen_t len = sizeof(sockAddrClient);

 	newsocket = accept(serverSock, (struct sockaddr*)&sockAddrClient, &len);
	if (newsocket < 0) {
        throwError("Conenction failed", serverSock);
	}
	std::cout << "Client {" << newsocket - 3 << "}" << " has been Connected." << std::endl;
	// else 
	// 	std::cout << "Connection is established successfuly" << std::endl;
	

	clientPoll.fd = newsocket;
	clientPoll.events = POLLIN; 

	client.setIp(inet_ntoa(sockAddrClient.sin_addr));
	client.setClientSock(newsocket);
	this->clients.push_back(client);
	this->monitor.push_back(clientPoll);
}

/* Reading msg from client */
/* newsocket is the one of the sending part "client" */
void Server::recieveData(int newsocket)
{
	char message[1024];
	int rbyte  = recv(newsocket, message, sizeof(message) - 1, 0);
	if (rbyte <= 0 )
	{
    	close(newsocket);
		std::cout << "Client {" << newsocket - 3 << "}" << " has been Disconnected." << std::endl;

	}
	message[rbyte] = '\0';
	std::cout << message;
	char *parse = strtok(message, "\r\n");

	while (parse != NULL)
	{
		std::string _parse = parse;

		if (_parse.find(" ") != std::string::npos)
		{
			command.setCommand(_parse.substr(0, _parse.find(" ")));
			command.setParameter(_parse.substr(_parse.find(" ") + 1));
		}
		else
        {
            command.setCommand(_parse);
            command.setParameter("");
        }
		// parseCommand(newsocket)	;
		parse = strtok(message, "\r\n");	
	}
}



/* Sending msg to client */
void Server::sendData(int newsocket, const char* msg)
{
	send(newsocket, msg, sizeof(msg), 0);
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



void Server::runningServer(int port, const char *av)
{
	this->setServerSock(port);
	(void)av;
	this->setServerPassWd(av);


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

void Server::checkPasswd(int newsocket, Client client)
{
	std::cout << this->passwd;
	if (command.getParameter() != this->passwd)
	{
		send(newsocket, "Password ghalat", sizeof("Password ghalat"), 0);
		return ;
	}
	client.setValid(true);
}

void Server::parseCommand(int newsocket)
{
	(void)newsocket;
	Client client;
	client = this->getClientFromVectorByFd(newsocket);
	std::cout << "get command : " << command.getCommand() ;
	if (command.getCommand() == "PASS")
	{
		// checkPasswd(newsocket, client);

	}	
}



Client Server::getClientFromVectorByFd(int _clientSock) const
{
	for(size_t i = 0; i < this->clients.size(); i++)
	{
		if(clients[i].getClientSock() == _clientSock)
			return clients[i];
	}
	return Client();
}



void Server::setServerPassWd(const char* av)
{
	this->passwd = av;
}