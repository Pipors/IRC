#include "includes/Server.hpp"
#include "includes/Client.hpp"


int main(int ac, char **av)
{
    if (ac != 2)
    {
        std::cerr << "Error!\nUsage : ./exe PORT PASSWD" << std::endl;
        return 0;
    }

    Server server;
    std::string  s(av[1]);
    std::stringstream  ss(s);
    int nb;
    ss >> nb;
    // Create socket
    // Bind to the port
    // Listen for connections
    server.serverInit(nb);
    // std::cout << "Server is listening on port " << PORT << std::endl;
    while (true)
    {
        // Accept a connection
        server.acceptNewConnection();
        // Read data from client);
        server.readData();
        // Send response to client);
        server.sendData("samaykom");
    }
    server.closeFd();
    return 0;
}
