/*
Bryleigh Koci
Lab 6 
client
*/

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

void echo(int readByte, char *buffer, int socketNum, int receveByte){
    // echo mode
    while(1){
        // prompts input
        printf("Enter message to send to server:\n");

        // read input
        readByte = read(1, buffer, 1024);

        // send input
        send(socketNum, buffer, readByte, 0);

        // if "close" receved then quit loop and close socket
        if(strncmp(buffer, "close\n", 6) == 0){
            memset(buffer, 0, readByte);

            receveByte = recv(socketNum, buffer, 1024, 0);
            printf("receved from server: %s\n", buffer);
            break;
        }

        memset(buffer, 0, readByte);

        // receve from server
        receveByte = recv(socketNum, buffer, 1024, 0);

        // if anything else receved then print it
        printf("receved: %s\n", buffer);

        // clears buffer
        memset(buffer, 0, receveByte);
    }
}

void file(int readByte, char *buffer, int socketNum, int receveByte){
    // file mode
    int fileDes, sentByte;
    size_t fileSize;

    while(1){
        // opens file
        fileDes = open("clientRcvFile", O_RDWR|O_CREAT|O_TRUNC, S_IRWXU);

        // receves from server
        receveByte = recv(socketNum, buffer, 1024, 0);
        fileSize = atoi(buffer);
        memset(buffer, 0, receveByte);

        sprintf(buffer, "receved size");
        sentByte = send(socketNum, buffer, strlen(buffer),0);
        memset(buffer, 0, sentByte);

        int offset = 0;
        while(offset < fileSize){
            receveByte = recv(socketNum, buffer, 1024, 0);
            offset += receveByte;
            write(fileDes, buffer, receveByte);
            memset(buffer, 0, receveByte);
        }
        // close file and breaks
        close(fileDes);
        break;
    }
    printf("Done, checking if client file exists\n");
    // check if file exists
    if(access("clientRcvFile", F_OK) == 0){
        printf("client file exists\n");
    }
    else{
        printf("File dosn't exist");
    }

}

int main(int argc, char** argv){
    int socketNum;
    int inetStat;
    int connectStat;

    struct sockaddr_in address;

    // server address
    inetStat = address.sin_addr.s_addr = inet_pton(AF_INET6, "127.0.0.1", &address.sin_addr);

    address.sin_family = AF_INET;
    address.sin_port = htons(1234);

    // create socket
    socketNum = socket(AF_INET, SOCK_STREAM, 0);

    // connect
    connectStat = connect(socketNum, (struct sockaddr *)&address, sizeof(address));

    char buffer[1024] = {0};
    int receveByte;
    int readByte;

    // argv for file mode
    if(argc >= 2){
        // set mode
        if((strcmp(argv[1], "file") == 0) && (strcmp(argv[2], "transfer") == 0)){
            send(socketNum, "file transfer\n", 14, 0);
            file(readByte, buffer, socketNum, receveByte);
        }
        else if(strcmp(argv[1], "echo") == 0){
            send(socketNum, "echo\n", 5, 0);
            echo(readByte, buffer, socketNum, receveByte);
        }
    }
    else if (argc == 1) {
        // manually set mode
        printf("Enter what mode you want (file transfer or echo):\n");

        readByte = read(1, buffer, 1024);
        // send input
        if(strcmp(buffer, "file transfer\n") == 0){
            send(socketNum, "file transfer\n", 14, 0);
            memset(buffer, 0, sizeof(buffer));
            file(readByte, buffer, socketNum, receveByte);
        }
        else if(strcmp(buffer, "echo\n") == 0){
            send(socketNum, "echo\n", 5, 0);
            memset(buffer, 0, sizeof(buffer));
            echo(readByte, buffer, socketNum, receveByte);
        }
    }

    return 0;
}
