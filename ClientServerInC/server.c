
#ifdef _WIN32
#include <Windows.h>
#else 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>                               // file descriptors;
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#endif

#define true 1
#define false 0

#define PORT 64000
#define BACKLOG SOMAXCONN

int main(int argc, char* argv[])
{
    puts("----- SERVER -----\n");

    char ABORT[1024] = "exit";
    int serverSocket, bindStatus, listenStatus, clientSocket, sendStatus, receiveStatus;
    char buffer[1025] = "Welcome to Inet Server!";

    // create socket;
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0)
    {
        perror("[SOCKET] ERROR trying to create socket.\n");
        exit(EXIT_FAILURE);
    }
    printf("[SOCKET] Socket %i was SUCCESSFUL created.\n", serverSocket);

    // bind socket;
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(PORT);
    address.sin_addr.s_addr = INADDR_ANY;
    bindStatus = bind(serverSocket, (struct sockaddr*)&address, sizeof(address));
    if (bindStatus < 0)
    {
        perror("[BIND] ERROR trying to bind socket to an IP address.\n");
        exit(EXIT_FAILURE);
    }
    puts("[BIND] Socket was SUCCESSFUL binded to IP address.\n");

    // listen socket;
    listenStatus = listen(serverSocket, BACKLOG);
    if (listenStatus < 0)
    {
        perror("[LISTEN] ERROR trying to listen for connections.\n");
        exit(EXIT_FAILURE);
    }
    puts("[LISTEN] Server is SUCCESSFUL listen for connections...\n");

    // accept client connections;
    struct sockaddr_in clientAddress;
    clientAddress.sin_family = AF_INET;
    clientAddress.sin_port = htons(PORT);
    clientAddress.sin_addr.s_addr = INADDR_ANY;
    int clientAddrLen = sizeof(clientAddress);
    clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, (socklen_t*)&clientAddrLen);
    if (clientSocket < 0)
    {
        perror("[ACCEPT] Connection refused by the server.\n");
        exit(EXIT_FAILURE);
    }
    puts("[ACCEPT] Client was SUCCESSFUL connected.\n");

    // send welcome message to client;
    sendStatus = send(clientSocket, buffer, 1025, 0);
    if (sendStatus < 0)
    {
        perror("[SEND] ERROR trying to send message to client.\n");
        exit(EXIT_FAILURE);
    }
    puts("[SEND] Message was SUCCESSFUL sent to client.\n");

    // while loop;
    char clientBuffer[1024];
    do {
        sleep(3);
        // receive message from client;
        receiveStatus = recv(clientSocket, clientBuffer, sizeof(clientBuffer), 0);
        if (receiveStatus < 0)
        {
            perror("[RECV] ERROR trying to receive message from client.\n");
            exit(EXIT_FAILURE);
        }
        if (strcmp(clientBuffer, ABORT) == 0)
            {
                // after receive abort message, the server will shutdown itself;
                puts("Disconnecting client...\n");
                sleep(3);
                break;
            }

        printf("[CLIENT] > %s\n", clientBuffer);
        // printf("buffer size: %li", sizeof(clientBuffer));

        // echo message back to client;
        sendStatus = send(clientSocket, clientBuffer, sizeof(clientBuffer), 0);
        if (sendStatus < 0)
        {
            exit(EXIT_FAILURE);
        }

    } while(receiveStatus > 0);

    // shutdown server socket;
    close(serverSocket);

    return 0;
}
