#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

#define PORT 10200
#define BUFFER_SIZE 21
#define ADDR "127.0.0.1"

unsigned char calculate_checksum(const char *data, int length) {
    unsigned int sum = 0;
    for (int i = 0; i < length; i++) {
        sum += (unsigned char)data[i];
    }
    return (unsigned char)(sum % 256);
}


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

    //main loop
    while (1) {
    int bytesReceived = recv(s, strData, sizeof(strData), 0);
    if (bytesReceived <= 0) {
        perror("Connection closed or error while reading data");
        break;
    }

    unsigned char receivedChecksum = (unsigned char)strData[bytesReceived - 1];
    unsigned char calculatedChecksum = calculate_checksum(strData, bytesReceived - 1);


    if (receivedChecksum == calculatedChecksum) {
        printf("Checksum OK. Received data: ");
        for (int i = 0; i < bytesReceived - 1; i++) {
            printf("%02X ", (unsigned char)strData[i]);
        }
        printf("\n");
    } else {
        printf("Checksum mismatch. Data might be corrupted.\n");
    }
}

    close(s);
    return 0;
}
