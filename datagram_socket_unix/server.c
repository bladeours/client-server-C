#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/un.h>

#define MY_SOCK_PATH "/tmp/serverDatagram1"
int sfd, cfd;

void signalHandler(int sig){
   printf("\nserver stop\n");
   close(sfd);
   close(cfd);
   unlink(MY_SOCK_PATH);
   exit(0);
}

int main(){
   signal(SIGINT, signalHandler);
   struct sockaddr_un server_addr, client_addr;
   sfd = socket(AF_UNIX, SOCK_DGRAM, 0);
   int client_addr_size = sizeof(client_addr);
   if(sfd == -1){
      perror("socket");
      return -1;
   }
   memset(&server_addr, 0, sizeof(server_addr));
   server_addr.sun_family = AF_UNIX;
   strcpy(server_addr.sun_path, MY_SOCK_PATH);

   if(bind(sfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) == -1){
      perror("bind");
      return -1;
   }
   char fromClient[50];
   while(1){
      if(recvfrom(sfd,fromClient, 50, 0, (struct sockaddr *)&client_addr,
                  &client_addr_size) == -1){
         perror("recvfrom");
         return -1;
      }
      printf("from client: %s\n",fromClient);
   
   }
}
