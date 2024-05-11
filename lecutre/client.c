#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/stat.h>

int main(int argc, char** argv)
{
    int socketNum;
    int inetStatus;
    int connectStatus;
    struct sockaddr_in address;

    int fileDes;
    size_t fileSize;


    //server ip address
    // address.sin_addr.s_addr = "127.0.0.1";
    inetStatus = inet_pton(AF_INET, "127.0.0.1", &address.sin_addr);
    printf("inetStatus: %d\n", inetStatus);

    address.sin_family = AF_INET;
    address.sin_port = htons(1234);

    socketNum = socket(AF_INET, SOCK_STREAM, 0);

    connectStatus = connect(socketNum, (struct sockaddr *)&address, sizeof(address));
    printf("connectStatus: %d\n", connectStatus);

    char buffer[10240] = {0};
    int recvBytes;
    int readBytes;
    int sentBytes;
    while(1)
    {
        fileDes = open("clientRecvFile", O_RDWR|O_CREAT|O_TRUNC, S_IRWXU);
        recvBytes = recv(socketNum, buffer, 10240, 0);
        fileSize = atoi(buffer);
        memset(buffer, 0, recvBytes);
        sprintf(buffer, "Received Size");
        printf("DEBUG: buffer: %s\n", buffer);
        sentBytes = send(socketNum, buffer, strlen(buffer), 0);
        printf("DEBUG: sentBytes after received size: %d\n", sentBytes);
        memset(buffer, 0, sentBytes);

        int offset = 0;
        while(offset < fileSize)
        {
            recvBytes = recv(socketNum, buffer, 10240, 0);
            offset += recvBytes;
            write(fileDes, buffer, recvBytes);
            memset(buffer, 0, recvBytes);
            printf("DEBUG: offset: %d\n", offset);
        }
        close(fileDes);
        break;

        // printf("What do you want to send to the server?\n");
        // readBytes = read(1, buffer, 1024);
        // printf("sending: %s\n", buffer);
        // send(socketNum, buffer, readBytes, 0);
        // memset(buffer, 0, readBytes);

        // recvBytes = recv(socketNum, buffer, 1024, 0);
        // printf("recevied from server: %s\n", buffer);
        // if(strncmp(buffer, "quit", 4) == 0)
        // {
        //     // int sendSize;
        //     memset(buffer, 0, 1024);
        //     printf("Closing connection\n");
        //     // sendSize = sprintf(buffer, "quit");
        //     // send(clientSocket, buffer, sendSize, 0);
        //     break;
        // }

        // memset(buffer, 0, recvBytes);
    }

    return 0;
}