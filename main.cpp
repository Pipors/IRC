#include "includes/server.hpp"
#include "includes/client.hpp"


int main() {

    Server server;
    // Create socket
    // Bind to the port
    // Listen for connections
    server.serverInit();

    std::cout << "Server is listening on port " << PORT << std::endl;

    // Accept a connection
   
    server.acceptConnection();

    // Read data from client
   
    server.readData();
    // Send response to client
    server.sendData("samaykom");

    // Close sockets
    server.closeFd();


    return 0;
}
