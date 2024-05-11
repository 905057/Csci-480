/*
Bryleigh Koci
Lab 6 
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
    int yes = 1;

    struct addrinfo hints, *res;

    int clientSocket;
    struct sockaddr_in clientAddr;
    socklen_t addrSize = sizeof(struct sockaddr_in);

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if(getaddrinfo(NULL, "1234", &hints, &res) != 0){
        return 1;
    }

    // create socket
    serverSocket = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if(serverSocket == -1){
        freeaddrinfo(res);
        return 1;
    }

    // sets socket options
    if(setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1){
            close(serverSocket);
            freeaddrinfo(res);
            return 1;
    }

    // binds socket
    if(bind(serverSocket, res->ai_addr, res->ai_addrlen) == -1){
            close(serverSocket);
            freeaddrinfo(res);
        return 1;
    }

    // listens for connections
    if(listen(serverSocket, 5) == -1){
        close(serverSocket);
        freeaddrinfo(res);
        return 1;
    }

    // fills client struct w/ info
    clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, (socklen_t *)&addrSize);
    if(clientSocket == -1){
            close(serverSocket);
            freeaddrinfo(res);
            return 1;
    }

    char buffer[1024] = {0};
    ssize_t sendSize, recvSize;

    // gets message for mode
    recvSize = recv(clientSocket, buffer, sizeof(buffer), 0);
    if(recvSize == -1){
        close(serverSocket);
        freeaddrinfo(res);
        return 1;
    }

    if(strncmp(buffer, "echo\n", 5) == 0){
        // loop for echo
        while(1){
            // clears buffer
            memset(buffer, 0, sizeof(buffer));
            // receves and stores message and size
            recvSize = recv(clientSocket, buffer, sizeof(buffer), 0);
            if(recvSize == -1){
                close(serverSocket);
                freeaddrinfo(res);
                return 1;
            }
            else if(recvSize == 0){
                // for client disconnection
                close(clientSocket);
                break;
            }

            printf("receved size: %ld\n", recvSize);
            printf("message: %s\n", buffer);

            // if "close" receved then quit loop
            if(strncmp(buffer, "close\n", 6) == 0){
                printf("Closing connection\n");
                sendSize = send(clientSocket, "goodbye\n", 8, 0);
                if(sendSize == -1){
                    close(clientSocket);
                    close(serverSocket);
                    freeaddrinfo(res);
                    return 1;
                }
                close(clientSocket);
                break;
            }

            // send message back to clent
            sendSize = send(clientSocket, buffer, recvSize, 0);
            if(sendSize == -1){
                close(clientSocket);
                close(serverSocket);
                freeaddrinfo(res);
                return 1;
            }
        }
    }
    else if(strncmp(buffer, "file transfer\n", 14) == 0){
        printf("in file mode\n");
        FILE *file;
        char *fileBuff;
        size_t fileSize, fileRedSize;

        file = fopen("fileText.txt", "rb");
        fseek(file, 0, SEEK_END);
        fileSize = ftell(file);
        rewind(file);

        fileBuff = (char *) malloc(fileSize * sizeof(char));
        fileRedSize = fread(fileBuff, sizeof(char), fileSize, file);

        while(1){
            sprintf(buffer, "%ld", fileSize);
            sendSize = send(clientSocket, buffer, strlen(buffer), 0);

            memset(buffer, 0, sendSize);
    
            recvSize = recv(clientSocket, buffer, 1024, 0);

            memset(buffer, 0, recvSize);
            sendSize = send(clientSocket, fileBuff, fileSize, 0);
            recvSize = recv(clientSocket, buffer, 1024, 0);

            fclose(file);
            break;
        }
    }

    // closes socket
    close(serverSocket);
    freeaddrinfo(res);
    return 0;
}
