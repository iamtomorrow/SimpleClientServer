
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
#include <errno.h>
#endif

#define true 1
#define false 0

#define PORT 64000

int main(int argc, char* argv[])
{
    puts("----- CLIENT -----\n");

    char ABORT[1024] = "exit";
    int clientSocket, connectStatus, receiveStatus, sendStatus;
    char buffer[1025];

    // create socket;
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0)
    {
        perror("[SOCKET] ERROR trying to create socket.\n");
        exit(EXIT_FAILURE);
    }
    printf("[SOCKET] %i Socket was SUCCESSFUL created.\n", clientSocket);

    // connect client socket;
    struct sockaddr_in clientAddress;
    clientAddress.sin_family = AF_INET;
    clientAddress.sin_port = htons(PORT);
    clientAddress.sin_addr.s_addr = INADDR_ANY;
    int clientAddrLen = sizeof(clientAddress);
    connectStatus = connect(clientSocket, (struct sockaddr*)&clientAddress, clientAddrLen);
    if (connectStatus < 0)
    {
        perror("[CONNECT] ERROR trying to connect client socket to an IP address.\n");
        exit(EXIT_FAILURE);
    }
    puts("[CONNECT] CLient socket was SUCCESSFUL connected.\n");

    // receive message from server;
    receiveStatus = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (receiveStatus < 0)
    {
        perror("[RECV] ERROR trying to receive message from server.\n");
        exit(EXIT_FAILURE);
    }
    printf("[SERVER] > %s\n", buffer);

    // while loop;
    char clientInput[1024];
    while (true)
    {
        // prompt the client;
        printf("> ");
        scanf(" %100[^\n]", clientInput);
        if (strcmp(clientInput, ABORT) == 0)
        {
            puts("Disconnecting...\n");
            sleep(3); 

            // send abort message to server;
            sendStatus = send(clientSocket, clientInput, sizeof(clientInput), 0);
            break;
        }
        else 
        {
            sendStatus = send(clientSocket, clientInput, sizeof(clientInput), 0);
            if (sendStatus < 0)
            {
                perror("[SEND] ERROR trying to send data to client.\n");
                exit(EXIT_FAILURE);
            }
            else 
            {
                // receive data from server;
                receiveStatus = recv(clientSocket, buffer, sizeof(buffer), 0);
                puts(buffer);
            }
        }
    }

    return 0;
}
