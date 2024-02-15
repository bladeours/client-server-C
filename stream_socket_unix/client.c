#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

#define MY_SOCK_PATH "/tmp/serverTest5"

int main(int argc, char *argv[]){
   struct sockaddr_un server_addr;

   int sfd = socket(AF_UNIX, SOCK_STREAM, 0);
   if(sfd == -1){
      perror("socket");
      return -1;
   }
   memset(&server_addr, 0, sizeof(server_addr));
   server_addr.sun_family = AF_UNIX;
   strcpy(server_addr.sun_path, MY_SOCK_PATH);

   
   if(connect(sfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) == -1){
      perror("connect");
      return -1;
   }

   if(send(sfd, argv[1], 50, 0) == 1){
      perror("send");
      return -1;
   }
   return 0;
}
