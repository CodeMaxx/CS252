#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <thread>

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

void read_thread(char buffer[], int *newsockfd)
{
    int n;
    while(1)
    {
    bzero(buffer,256);
    n = read(*newsockfd,buffer,255); // Putting data from socket to buffer
    if (n < 0) error("ERROR reading from socket");
    if(buffer[0] == '/')
    {
        if(strcmp(buffer, "/help"))
        {
            printf("/help - To get help\n");
            printf("/register [username] - To start registration process\n");
            printf("/login [username] [password]\n");
            printf("/chat [Friend's Username] - To chat with a friend\n");
            printf("/showall - Show all registered users\n");
            printf("/showOnline - Show all online users\n");
            printf("/logout - To logout and quit\n");
            printf("/sendfile - To send file to friend\n");
        }
    }
    else
    {
        buffer[n-1] = '\0';
        printf("Client: %s %d\n",buffer, n);
    }
 }
}

void write_thread(char buffer[], int *newsockfd)
{
    int n;
    while(1)
    {
    bzero(buffer,256);
    printf("You: ");
    fgets(buffer,255,stdin);
    n = write(*newsockfd,buffer,strlen(buffer)); // Writing to socket
    if (n < 0) error("ERROR writing to socket");
 }
}


int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char read_buffer[256];
    char write_buffer[256];
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
    std::thread read_th(read_thread,read_buffer, &sockfd);
    std::thread write_th(write_thread,write_buffer, &sockfd);

    //read_th.join();
    while(1){;}
    close(sockfd); // Closing the socket.
    return 0;
}
