#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SERVER_PATH     "/tmp/server"
#define BUFFER_LENGTH    250

int main()
{
   unlink(SERVER_PATH); //czyscimy na wypadek nieprawidlowego zamkniecia serwera
   int    serverSocket, clientSocket;
   char   buffer[BUFFER_LENGTH]; //tutaj bedzie to co dostaniemy od klienta
   struct sockaddr_un serverAddressStruct; //potrzebna struktura


   serverSocket = socket(AF_UNIX, SOCK_STREAM, 0); 
   if (serverSocket == -1)
   {
      perror("socket() failed");
      return -1;
   }

   memset(&serverAddressStruct, 0, sizeof(serverAddressStruct)); // czyscimy strukture
   serverAddressStruct.sun_family = AF_UNIX; // man bind
   strcpy(serverAddressStruct.sun_path, SERVER_PATH); // man bind

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
   unlink(SERVER_PATH);
   return 0;
}