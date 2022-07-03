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
   
   int    serverSocket;
   char   buffer[BUFFER_LENGTH]; //tutaj bedzie to co dostaniemy od klienta
   struct sockaddr_un serverAddressStruct, clientAddressStruct; //potrzebna struktura


   serverSocket = socket(AF_UNIX, SOCK_DGRAM, 0); 
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

       socklen_t len = sizeof clientAddressStruct;
      if (recvfrom(serverSocket, buffer, sizeof(buffer), 0, (struct sockaddr*)  &clientAddressStruct, &len ) == -1)
      {
         perror("recvfrom() failed");
         break;
      } 
      printf("%s\n",buffer);
   }
   

   close(serverSocket);
   unlink(SERVER_PATH);
   return 0;
}