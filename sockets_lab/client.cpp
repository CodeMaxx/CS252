#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");

    server = gethostbyname(argv[1]); // System call that does the DNS query. Will return a structure which has the IP address of the server

    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr)); // Zeroing the instance out.
    // Populating server address structure
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length); // Now IP we got from gethostbyname is added to this structure

    serv_addr.sin_port = htons(portno); // Note there is ntohs() also

    // Note connect is not returning any new file descriptor.
    /* Not sure about this comment-> At server we need a listening socket since multiple clients can go there. sockfd keeps the
    server side info and for any new connection from a new client a newsockfd is created. newsockfd is unique for every client */
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
        error("ERROR connecting");
    while(1)
    {
    printf("Please enter the message: ");
    bzero(buffer,256);

    fgets(buffer,255,stdin); // Get 255 bytes of input from stdin(i.e. terminal) - Standard input
    n = write(sockfd,buffer,strlen(buffer)); // What you read, write to socket
    if (n < 0)
         error("ERROR writing to socket");
    bzero(buffer,256); // Clear the buffer since we'll now read to the same buffer.
    n = read(sockfd,buffer,255); // Waits for server to send a message.
    // n gives the length of the message.
    if (n < 0)
         error("ERROR reading from socket");
    printf("%s %d\n",buffer, n);
    }
    close(sockfd); // Closing the socket.
    return 0;
}
