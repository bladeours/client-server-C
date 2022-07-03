#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netdb.h>

/*
man recvmmsg - w przykladzie jest uzyta struktura sockaddr_in
man recvfrom
*/

#define BUFFER_LENGTH    250


int main()
{
   int    serverSocket;
   char   buffer[BUFFER_LENGTH]; //tutaj bedzie to co dostaniemy od klienta
   struct sockaddr_in serverAddressStruct; //potrzebna struktura


   serverSocket = socket(AF_INET, SOCK_DGRAM, 0); 
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
      socklen_t len = sizeof serverAddressStruct;
      if (recvfrom(serverSocket, buffer, sizeof(buffer), 0,(struct sockaddr*)&serverAddressStruct, &len) == -1)
      {
         perror("recv() failed");
         break;
      } 
      printf("%s\n",buffer);
   }
   

   close(serverSocket);
   return 0;
}