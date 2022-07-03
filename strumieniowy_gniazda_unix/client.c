
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SERVER_PATH "/tmp/server"

int main(int argc, char *argv[])
{

   int    serverSocket;
   struct sockaddr_un serverAddress;

   serverSocket = socket(AF_UNIX, SOCK_STREAM, 0);
   if (serverSocket == -1)
   {
      perror("socket() failed");
      return -1;
   }

   memset(&serverAddress, 0, sizeof(serverAddress)); //czyscimy strukture
   serverAddress.sun_family = AF_UNIX; //man bind
   strcpy(serverAddress.sun_path, SERVER_PATH); //man bind

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