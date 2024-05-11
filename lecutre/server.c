/*
C Socket Server
*/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>

int main(int argc, char** argv)
{
    int serverSocket;
    int clientSocket;
    int setSockOptResult;
    int bindResult;
    int listenResult;
    int yes = 1;
    FILE *file;
    char* fileBuffer;
    size_t fileSize;
    size_t fileReadSize;

    struct addrinfo hints, *res;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    getaddrinfo(NULL, "1234", &hints, &res);

    // struct sockaddr_in address;
    struct sockaddr_in clientAddress;
    int addrSize = sizeof(struct sockaddr_storage);
    // int serverSocket2;
    
    // address.sin_addr.s_addr = INADDR_ANY;
    // address.sin_family = AF_INET;
    // address.sin_port = htons(1234);

    // serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    serverSocket = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

    printf("The socket number is: %d\n", serverSocket);

    setSockOptResult = setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
    printf("setSockOptResult: %d\n", setSockOptResult);

    // bindResult = bind(serverSocket, (struct sockaddr *)&address, sizeof(address));
    bindResult = bind(serverSocket, res->ai_addr, res->ai_addrlen);
    printf("bindResult: %d\n", bindResult);

    listenResult = listen(serverSocket, 5);
    printf("listenResult: %d\n", listenResult);

    file = fopen("server.out", "rb");
    fseek(file, 0, SEEK_END);
    fileSize = ftell(file);
    rewind(file);

    printf("The file size is: %ld\n", fileSize);
    fileBuffer = (char *) malloc(fileSize * sizeof(char));
    fileReadSize = fread(fileBuffer, sizeof(char), fileSize, file);
    printf("Read in %ld bytes to fileBuffer\n", fileReadSize);
    // return 0;

    clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddress, (socklen_t *)&addrSize);
    printf("clientSocket: %d\n", clientSocket);

    char buffer[1024] = {0};
    while(1)
    {
        ssize_t sendSize;
        ssize_t recvSize;
        sprintf(buffer, "%ld", fileSize);
        sendSize = send(clientSocket, buffer, strlen(buffer), 0);

        memset(buffer, 0, sendSize);
        recvSize = recv(clientSocket, buffer, 1024, 0);
        memset(buffer, 0, recvSize);
        sendSize = send(clientSocket, fileBuffer, fileSize, 0);
        recvSize = recv(clientSocket, buffer, 1024, 0);
        printf("Received from client: %s\n", buffer);
        break;
        
        // printf("DEBUG: %s\n", buffer);
        // return 0;


        // ssize_t recvSize;
        // recvSize = recv(clientSocket, buffer, 1024, 0);
        // printf("Received size: %ld\n", recvSize);
        // printf("Received from client: %s\n", buffer);

        // //if buffer == "quit" then close connection and accept
        // if(strncmp(buffer, "quit", 4) == 0)
        // {
        //     int sendSize;
        //     memset(buffer, 0, recvSize);
        //     printf("Closing client connection\n");
        //     sendSize = sprintf(buffer, "quit");
        //     send(clientSocket, buffer, sendSize, 0);
        //     break;
        // }

        // ssize_t sendSize;
        // sendSize = send(clientSocket, buffer, recvSize, 0);

        // memset(buffer, 0, 1024);
    }
    



    // serverSocket2 = socket(AF_INET, SOCK_STREAM, 0);

    // printf("The socket number is: %d\n", serverSocket2);

    // while(1)
    // {
        
    // }
    fclose(file);
    close(serverSocket);
    
    return 0;
}