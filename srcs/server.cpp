#include "../includes/Server.hpp"

Server::Server()
{
	this->passwd = "";
	this->serverSock = 0;
	this->monitor.clear();
	this->clients.clear();
	std::string serverName = "";
}

Server::~Server()
{
}
/***********************************/
/*                                 */
/* FUNCTIONS SETTING UP THE SERVER */
/*                                 */
/***********************************/

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
	
	serverAddr.sin_family = AF_INET; // for IP v4
    serverAddr.sin_addr.s_addr = INADDR_ANY; // any address
    serverAddr.sin_port = htons((uint16_t)port); //port to communicate from

	int _bind = bind(serverSock, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if (_bind == -1)
		throwError("Bind failed => ", serverSock);

	if (listen(serverSock, SOMAXCONN) == -1)
        throwError("Listen failed", serverSock);

	serverPoll.fd = serverSock;
	serverPoll.events = POLLIN;
	this->monitor.push_back(serverPoll);
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
	std::cout << "Connection is established successfuly with client N" << newsocket - 3 << std::endl;
	

	clientPoll.fd = newsocket;
	clientPoll.events = POLLIN; 

	client.setIp(inet_ntoa(sockAddrClient.sin_addr));
	client.setClientSock(newsocket);
	this->clients.push_back(client);
	this->monitor.push_back(clientPoll);
}



void Server::runningServer(int port, const char *av)
{
	this->setServerSock(port);
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
	std::cout << "-----CLIENTS-----" << std::endl;
	this->printClt();
	this->closeFd();
}



/************************************************/
/*          									*/
/* FUCNTIONS HANDLING SENDING AND RECIEVING MSG */
/*                          			        */
/************************************************/


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
		return ;
	}
	message[rbyte] = '\0';
	// std::string parse = strtok(message, "\r\n");
	std::cout << message;
	this->command.setCommandLine(message);
	this->parseCommand(newsocket);

}



void Server::parseCommand(int newsocket)
{
	Client client;
	client = this->getClientFromVectorByFd(newsocket);
	this->command.checkPasswd(this->getPasswd(), client);
}

// void Server::checkPasswd(int newsocket, Client client)
// {
// 	if (command.getParameter2() != this->passwd)
// 	{
// 		send(newsocket, "Password ghalat", sizeof("Password ghalat"), 0);
// 		return ;
// 	}
// 	client.setValid(true);
// }

/* Sending msg to client */
void Server::sendData(int newsocket, const char* msg)
{
	send(newsocket, msg, sizeof(msg), 0);
    std::cout << "Message sent to client" << msg << std::endl;
}


/*********/
/*       */
/* UTILS */
/*       */
/*********/

void Server::throwError(const char* msg, int fd)
{
	close(fd);
	perror(msg);
	exit(EXIT_FAILURE);
}

void Server::closeFd()
{
	for(size_t i = 0; i < monitor.size(); i++)
		close(monitor[i].fd);
}


bool Server::running = true;

void Server::signalHandler(int sig)
{
	(void)sig;
    std::cout << std::endl << "Received SIGUSR1 signal!" << std::endl;
    Server::running = false;
}


void Server::removeClientFromServer(int clientsock)
{
	std::vector<Client>::iterator it = clients.begin();
	for (it = clients.begin(); it < clients.end(); it++)
	{
		const int &i = it->getClientSock();
		if (i == clientsock)
		{
			close(clients[i].getClientSock());
			clients.erase(it);
		}
	}
	return;
}

void Server::removeClientFromPollfd(int clientsock)
{
	std::vector<struct pollfd>::iterator it = monitor.begin();
	for (it = monitor.begin(); it < monitor.end(); it++)
	{
		const int &i = it->fd;
		if (i == clientsock)
		{
			close(it->fd);
			monitor.erase(it);
		}
	}
	return;
}

/***********/
/*         */
/* SETTERS */
/*         */
/***********/

void Server::setServerPassWd(const char* av)
{
	this->passwd = av;
}


int Server::getServerFd() const
{
	return this->serverSock;
}


/***********/
/*         */
/* GETTERS */
/*         */
/***********/

std::vector<struct pollfd> Server::getMonitor() const
{
	return this->monitor;
}


uint16_t Server::getMonitorSize() const
{
	return this->monitor.size();
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


std::string Server::getPasswd() const
{
	return this->passwd;
}













