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
	serverAddr.sin_family = AF_INET; // for IP v4
    serverAddr.sin_addr.s_addr = INADDR_ANY; // any address
    serverAddr.sin_port = htons((uint16_t)port); //port to communicate from

	this->serverSock = socket(AF_INET, SOCK_STREAM, 0);

	if(this->serverSock < 3)
		throwError("Socket Failed", 0);

	int sockopt = setsockopt(serverSock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
	if (sockopt < 0)
		std::cout << "chi l3ayba tema " << std::endl;
	if (fcntl(sockopt, F_SETFL, O_NONBLOCK) == -1) //-> set the socket option (O_NONBLOCK) for non-blocking socket
		throw(std::runtime_error("faild to set option (O_NONBLOCK) on socket"));

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
	send(newsocket, "Connection is established successfuly with the server\r\n", strlen("Connection is established successfuly with the server\r\n"), 0);


	clientPoll.fd = newsocket;
	clientPoll.events = POLLIN; 

	client.setIpAddress(inet_ntoa(sockAddrClient.sin_addr));
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
	//Client &client;
	char message[1024];
	int rbyte = recv(newsocket, message, sizeof(message) - 1, 0);
	if (rbyte < 0)
	{
		close(newsocket);
		std::cout << "Client {" << newsocket - 3 << "}" << " has been Disconnected." << std::endl;
		return;
	}
	message[rbyte] = '\0';
	Client &client = *this->getClientFromVectorByFd(newsocket);
	std::cout << message;

	std::vector<std::string> vec(getWords_(message));
	std::vector<std::string>::iterator it = vec.begin();

	while (it != vec.end())
	{
		if (*it == "PASS")
		{
			if (*(it + 1) != this->getPasswd())
			{
				command.sendData(client.getClientSock(), "Wrong Password... Disconnecting\r\n");
			}
			else
			{
				//client.setValid(true);
				//for (size_t i = 0; i < clients.size(); i++)
				//{
					//if (clients[i].getClientSock() == newsocket)
				client.setValid(true);
				//}
				command.passCommand(client);
				
				//char msg[] =;
				//close(client.getClientSock());
				//std::cout << "Client {" << client.getClientSock() - 3 << "}" << " has been Disconnected." << std::endl;
			}
		}
		if (*it == "NICK" && client.isValid() == true)
		{
			for (size_t i = 0; i < clients.size(); i++)
			{
				if (clients[i].getClientSock() == newsocket)
					clients[i].setNickName(*(it + 1));
			}
		}
		if (*it == "USER" && client.isValid() == true)
		{
			for (size_t i = 0; i < clients.size(); i++)
			{
				if (clients[i].getClientSock() == newsocket)
					clients[i].setUserName(*(it + 1));
			}
		}
		if (*it == "JOIN" && client.isEmptyName() == false)
		{
			const std::string& param = *(it + 1);
			command.joinCommand(param, client);
		}
		if (*it == "QUIT" && *(it + 1) == ":Leaving")
			close(client.getClientSock());
		it++;
	}
}

/* Sending msg to client */

/*********/
/*       */
/* UTILS */
/*       */
/*********/

void Server::throwError(const char* msg, int fd)
{
	close(fd);
	perror(msg);
	_exit(0);
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



Client* Server::getClientFromVectorByFd(int _clientSock)
{
	Client* p = NULL;
	for (std::vector<Client>::iterator it = clients.begin(); it != clients.end(); it++)
	{
		if (it->getClientSock() == _clientSock)
		{
			p = &(*it);
			return p;
		}
	}
	return NULL;
}


std::string Server::getPasswd() const
{
	return this->passwd;
}


std::vector<std::string> Server::getWords_( std::string str)
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







