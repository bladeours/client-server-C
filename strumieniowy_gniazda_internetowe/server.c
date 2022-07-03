#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netdb.h>

/*
   man recvmmsg - w przykladzie jest uzyta struktura sockaddr_in
   man 7 ip
   man bind
*/

#define BUFFER_LENGTH    250


int main()
{
   int    serverSocket, clientSocket;
   char   buffer[BUFFER_LENGTH]; //tutaj bedzie to co dostaniemy od klienta
   struct sockaddr_in serverAddressStruct; //potrzebna struktura


   serverSocket = socket(AF_INET, SOCK_STREAM, 0); 
   if (serverSocket == -1)
   {
      perror("socket() failed");
      return -1;
   }

   serverAddressStruct.sin_family = AF_INET; //man 7 ip
   serverAddressStruct.sin_port = 8080;
   serverAddressStruct.sin_addr.s_addr = htonl(INADDR_LOOPBACK); // man sendmmsg

   if(bind(serverSocket, (struct sockaddr *)&serverAddressStruct, sizeof(serverAddressStruct)) == -1)
   {
      perror("bind() failed");
      return -1;
   }

   while(1){

      if(listen(serverSocket, 10) == -1)
      {
         perror("listen() failed");
         break;
      }

      printf("Ready for client connect().\n");

      clientSocket = accept(serverSocket, NULL, NULL);
      if (clientSocket == -1)
      {
         perror("accept() failed");
         break;
      }

      if (recv(clientSocket, buffer, sizeof(buffer), 0) == -1)
      {
         perror("recv() failed");
         break;
      } 
      printf("%s\n",buffer);
   }
   

   close(serverSocket);
   close(clientSocket);
   return 0;
}