
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SERVER_PATH "/tmp/server"
#define CLIENT_PATH "/tmp/client"

int main(int argc, char *argv[])
{
    unlink(CLIENT_PATH);
   int    serverSocket, clientSocket;
   struct sockaddr_un serverAddress, clientAddress;

   clientSocket = socket(AF_UNIX, SOCK_DGRAM, 0);
   if (clientSocket == -1)
   {
      perror("socket() failed");
      return -1;
   }

   memset(&serverAddress, 0, sizeof(serverAddress)); //czyscimy strukture, jest w man bind
   memset(&clientAddress, 0, sizeof(clientAddress)); //czyscimy strukture, jest w man bind

   serverAddress.sun_family = AF_UNIX; //man bind
   strcpy(serverAddress.sun_path, SERVER_PATH); //man bind

   clientAddress.sun_family = AF_UNIX; // man bind
   strcpy(clientAddress.sun_path, CLIENT_PATH); // man bind

    if(bind(clientSocket, (struct sockaddr *)&clientAddress, sizeof(clientAddress)) == -1)
   {
      perror("bind() failed");
      return -1;
   }

   if(sendto(clientSocket, argv[1], 100, 0, (struct sockaddr *)&serverAddress,sizeof(serverAddress)) == -1)
   {
      perror("sendto() failed");
      return -1;
   }

   close(serverSocket);
   return 0;
}