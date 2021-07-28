
#include <iostream>
#include <string.h>
#include <string>

#ifdef _WIN32
#include <Winsock2.h>
#include <Ws2tcpip.h>
#else 
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <errno.h>
#endif

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif 

#define BUFFER_SIZE 1024
#define PORT 45000

int main(int argc, char *argv[])
{
    std::cout << "-----[CLIENT]-----\n" << std::endl;    

    // create a client socket;
    int clientSocket;
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0)
    {
        std::cerr << "[SOCKET] ERROR trying to create socket." << std::endl;
        return 0;
    }
    else 
    {
        std::cout << "[SOCKET] Client socket " << clientSocket <<" was SUCCESSFUL created." << std::endl;
    }

    // connect the client socket to an IP address;
    struct sockaddr_in clientAddress;
    clientAddress.sin_family = AF_INET;
    clientAddress.sin_port = htons(PORT);
    clientAddress.sin_addr.s_addr = INADDR_ANY;
    int connectStatus;
    connectStatus = connect(clientSocket, (struct sockaddr*)&clientAddress, sizeof(clientAddress));
    if (connectStatus < 0)
    {
        std::cerr << "[CONNECT] ERROR trying to connect client. Check if the server is open or review address information." << std::endl;
        return 0;
    }
    else 
    {
        std::cout << "[CONNECT] Client was SUCCESSFUL connected." << std::endl;
    }

    // send data to server;
    int sendStatus;
    int recvStatus;

    std::string clientInput;
    char serverOutput[256];
    do {
        // prompt the client;
        std::cout << "> ";
        std::getline(std::cin, clientInput);
        sendStatus = send(clientSocket, clientInput.c_str(), clientInput.size(), 0);
        if (sendStatus < 0)
        {
            std::cerr << "[SEND] ERROR trying to send data to server." << std::endl;
            return 0;
        }
        else
        {
            // receive data back from server;
            recvStatus = recv(clientSocket, serverOutput, sizeof(serverOutput), 0);
            if (recvStatus < 0)
            {
                std::cerr << "[RECV] ERROR trying to receive back data from the server." << std::endl;
            }
            else 
            {
                std::cout << "SERVER > " << serverOutput << std::endl;
                // fflush(stdout);
            }
            bzero(serverOutput, 256);
        }
    } while (clientInput != "0");

    // shutdown socket;
    if (close(clientSocket) < 0)
    {
        std::cerr << "[CLOSE] ERROR trying to close socket." << std::endl;
        return 0;
    }
    std::cout << "[CLOSE] Socket was SUCCESSFUL closed." << std::endl;

    return 0;
}
