
#include <iostream>
#include <string.h>

#ifdef _WIN32
#include <Windows.h> 
#include <Winsock2.h>
#include <Ws2tcpip.h>
#else 
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>              // getnameinfo()
#include <errno.h>
#endif

#define BUFFER_SIZE 1024 
#define PORT 45000 

int main(int argc, char *argv[])
{
    std::cout << "-----[SERVER]-----\n" << std::endl;

    // create socket;
    int serverSocket;
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0)
    {
        std::cout << "[SOCKET] ERROR trying to create socket." << std::endl;
        return 0;
    }
    else 
    {
        std::cout << "[SERVER] Server socket " << serverSocket << " was  SUCCESSFUL created." << std::endl;
    }

    // bind socket to an IP address;
    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT);
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    int bindStatus;
    bindStatus = bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
    if (bindStatus < 0)
    {
        std::cout << "[BIND] ERROR trying to bind socket to an IP address. Please make sure that the server address is a valid value." << std::endl;
        return 0;
    }
    else 
    {
        std::cout << "[BIND] Socket was SUCCESSFUL binded to IP address." << std::endl;
    }

    // listen for connections;
    int listenStatus;
    const int backlog = SOMAXCONN;
    listenStatus = listen(serverSocket, backlog);
    if (listenStatus < 0)
    {
        std::cout << "[LISTEN] ERROR trying to listen for connections. Make sure the server is open and the backlog is a valid value." << std::endl;
        return 0;
    } 
    else 
    {
        std::cout << "[LISTEN] Socket is SUCCESSFUL listen for connections." << std::endl;
    }

    // accept connections;
    int clientSocket;
    struct sockaddr_in clientAddress;
    clientAddress.sin_family = AF_INET;
    clientAddress.sin_port = htons(PORT);
    clientAddress.sin_addr.s_addr = INADDR_ANY;
    int clientAddrLen = sizeof(clientAddress);

    clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, (socklen_t*)&clientAddrLen);
    if (clientSocket < 0)
    {
        std::cout << "[ACCEPT] ERROR trying to accept client." << std::endl;
        return 0;
    }
    else 
    {
        char hostname[NI_MAXHOST];              // client remote name
        char serviceInfo[NI_MAXSERV];
        if (getnameinfo((sockaddr*)&clientAddress, sizeof(clientAddress), hostname, NI_MAXHOST, serviceInfo, NI_MAXSERV, 0) == 0)
        {
            // std::cout << host << " SUCCESSFUL connected on port " << service << "." << std::endl;
            std::cout << "HOST: " << hostname << std::endl;
            std::cout << "SERVICE: " << serviceInfo << "\n" << std::endl;
        }
        // std::cout << "[ACCEPT] SUCCESSFUL accepted client connection." << std::endl;
    }

    // send data to client;
    /* int sendStatus;
    char buffer[1024] = "";
    sendStatus = send(clientSocket, buffer, 1024, 0);
    if (sendStatus < 0)
    {
        std::cout << "[SEND] ERROR trying to send data to client." << std::endl;
        return 0; 
    }
    else 
    {
        std::cout << "[SEND] Data was SUCCESSFUL sent to client." << std::endl;
        std::cout << "[DATA LENGTH] :" << sizeof(buffer) << std::endl;
    }*/

    // recive data from client;
    int recvStatus;
    int sendStatus;

    char buffer[BUFFER_SIZE];
    do {
        // memset(buffer, '\0', 10);
        recvStatus = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (recvStatus < 0)
        {
            std::cerr << "[RECV] ERROR trying to receive data from client." << std::endl;
            return 0;
        }
        else if (recvStatus == 0)
        {
            std::cout << "\n";
            std::cout << "Client SUCCESSFUL disconnected." << std::endl;
        }
        else
        {
            std::cout << "CLIENT > " << buffer << std::endl;

            // send the data back to client;
            sendStatus = send(clientSocket, buffer, sizeof(buffer), 0);
            if (sendStatus < 0)
            {
                std::cerr << "[SEND] ERROR trying to send data back to client." << std::endl;
            }
            bzero(buffer, 256);
        }
    } while (recvStatus != 0 || buffer == "");

    // shutdown socket;
    if (close(serverSocket) < 0)
    {
        std::cerr << "[CLOSE] ERROR trying to close socket." << std::endl;
        return 0;
    }
    std::cout << "[CLOSE] Socket was SUCCESSFUL closed." << std::endl;
    std::cout << argv[0] << std::endl;

    return 0;
}
