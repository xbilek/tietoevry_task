#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

#define PORT 10200
#define BUFFER_SIZE 20
#define ADDR "127.0.0.1"

int main(int argc, char **argv) {
    int s = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in servAddr;
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(PORT);
    servAddr.sin_addr.s_addr = inet_addr(ADDR);

    int connectStatus = connect(s, (struct sockaddr *)&servAddr, sizeof(servAddr));
    if (connectStatus == -1) {
        perror("Connection error");
        return 1;
    }

    char strData[BUFFER_SIZE];

    while (1) { //client running loop
        int bytesReceived = recv(s, strData, sizeof(strData), 0);
        if (bytesReceived <= 0) {
            perror("Connection closed or error while reading data");
            break;
        }

        printf("Received data: ");
        for (int i = 0; i < bytesReceived; i++) {//printing every byte hexadecimaly
            printf("%02X ", (unsigned char)strData[i]);
        }
        printf("\n");
    }

    close(s);
    return 0;
}
