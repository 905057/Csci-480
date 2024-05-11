/* 
CSCI 484 Computer Networks
Bryleigh Koci

finds ip address
credit to https://www.youtube.com/watch?v=yN6EGfv5Dew for further explaining program
*/

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main(int argc, char ** argv){
    // creates varibles
    struct addrinfo hints;
    struct addrinfo *info;

    if (argc != 2){
        printf("Usage: %s <hostname>\n", argv[0]);
        return 1;
    }

    const char *hostName = argv[1];

    memset(&hints, 0, sizeof hints); //empties hints struct
    hints.ai_family = AF_UNSPEC; // ipv4 and ipv6
    hints.ai_socktype = SOCK_STREAM; // only get stream socket

    int status = getaddrinfo(hostName, NULL, &hints, &info);

    if (status == 0){ // if getaddrinfo is sucsessful
        struct addrinfo *subInfo = info;
        printf("website: %s\n", hostName);
        printf("IP address info:\n");

        while (subInfo != NULL){ // can be a linked list for mutiple addreses

            void *addr;
            char ipstr[INET6_ADDRSTRLEN];
            char *ipver;

            if (subInfo->ai_family == AF_INET){
                // ipv4
                addr = &((struct sockaddr_in*)subInfo->ai_addr)->sin_addr;
                ipver = "IPv4";
            }
            else{
                // ipv6
                addr = &((struct sockaddr_in6*)subInfo->ai_addr)->sin6_addr;
                ipver = "IPv6";

            }
            inet_ntop(subInfo->ai_family, addr, ipstr, sizeof(ipstr));

            // print info
            // printf("website domain: %s\n", domain);
            printf("\t%s: %s\n", ipver, ipstr);
            subInfo = subInfo->ai_next;
        }
    }
    else{
        printf("getaddrinfo failed\n");
        return 1;
    }

    freeaddrinfo(info); //clears info

    return 0;
}
