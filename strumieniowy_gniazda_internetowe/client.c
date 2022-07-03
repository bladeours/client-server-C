
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/ip.h>
/*
   man 7 ip - pokazana struktura sockaddr_in
   man sendmmsg pokazany htonl(INADDR_LOOPBACK)
   man bind
*/
int main(int argc, char *argv[])
{

   int    serverSocket;
   struct sockaddr_in serverAddress;

   serverSocket = socket(AF_INET, SOCK_DGRAM, 0);
   if (serverSocket == -1)
   {
      perror("socket() failed");
      return -1;
   }

   serverAddress.sin_family = AF_INET; 
   serverAddress.sin_port = 8080;
   serverAddress.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

   if(connect(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == -1)
   {
      perror("connect() failed");
      return -1;
   }

   if(send(serverSocket, argv[1], 100, 0) == -1)
   {
      perror("send() failed");
      return -1;
   }

   close(serverSocket);
   return 0;
}