/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <thread>

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

void read_thread(char buffer[], int *newsockfd)
{
    int n;
    while(1)
    {
    bzero(buffer,256);
     n = read(*newsockfd,buffer,255); // Putting data from socket to buffer
     if (n < 0) error("ERROR reading from socket");
     buffer[n-1] = '\0';
     printf("Client: %s %d\n",buffer, n);
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
     int sockfd, newsockfd, portno; // Socket handles are also file descriptors. Port Number where the communication will happen.
     // File descriptors are used by the operating system to file information about the files.
     socklen_t clilen;
     char read_buffer[256]; // Read the socket and put characters into the buffer.
     char write_buffer[256];
     struct sockaddr_in serv_addr, cli_addr; // in stands for internet family addresses.
     int n;
     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
     sockfd = socket(AF_INET, SOCK_STREAM, 0); // First socket programming call! AF_INET = Address Family - Internet.(Will probably change for ipv6)
    // STREAM - TCP socket and not a UDP socket. TCP sockets are called Stream sockets. UDP sockets are called datagram sockets.
    // Third argument is a protocol. It might have a variety of protocols for a stream connection as well.
    // 0 here tells us, I don't know you pick the correct one.
     if (sockfd < 0)
        error("ERROR opening socket"); // If error print it.
     bzero((char *) &serv_addr, sizeof(serv_addr)); // Make the memory zero from a particular address and a given number of bytes from that
     portno = atoi(argv[1]); // First argument is the port number.
     // Now we populate the server address structure
     serv_addr.sin_family = AF_INET; // Always keep AF_INET
     serv_addr.sin_addr.s_addr = INADDR_ANY; // Pick up the localhost address
     serv_addr.sin_port = htons(portno); // Computer stores in a different way. So we convert it to how the networks mananges it.
     // Computer stores as little endian. Network takes big endian.
     if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) // We are binding the file descriptor and the server address.
              error("ERROR on binding");
     listen(sockfd,5); // We will now only refer to our socket by the descriptor. At max 5 socket connections in waiting.
     // Probably 1 is connected and 5 are waiting. Check this!
     // We have not started listening yet. We are just saying it is a listening type of port.
     // 5 waiting for handshakes. After newsockfd is created we can just use multiple threads.
     clilen = sizeof(cli_addr);
     newsockfd = accept(sockfd,
                 (struct sockaddr *) &cli_addr,
                 &clilen); // Now we start listening on the port. This method will also set the client address structure.
     // newsockfd is the real file descriptor of the whole connection.
     // sockfd is a descriptor for half socket. It has no information about the client addrss
     // sockets are also files. You read and write into them.
     // Note that clilen is also being sent by reference.
     // Note cli_addr is also being type casted here. Maybe for a different family of addresses which might be added later.

     if (newsockfd < 0)
          error("ERROR on accept");
     // "connect" request form client is being "accpeted" by the accept() function.
      std::thread read_th(read_thread,read_buffer, &newsockfd);
      std::thread write_th(write_thread,write_buffer, &newsockfd);

      // read_th.join();
      while(1){;}
     close(newsockfd);
     close(sockfd);
     return 0;
}
