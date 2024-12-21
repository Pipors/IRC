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
    std::stringstream  s(av[1]);
    int nb;
    s >> nb;
    // Create socket, Bind to the port and start Listening for connections


    server.runningServer(nb);




    return 0;
}
