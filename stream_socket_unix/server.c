#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#define MY_SOCK_PATH "/tmp/serverTest5"

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
   socklen_t client_addr_size;

   sfd = socket(AF_UNIX, SOCK_STREAM, 0);
   if(sfd == -1){
      perror("socket");
      return 1;
   }
   memset(&server_addr, 0, sizeof(server_addr));
   server_addr.sun_family = AF_UNIX;
   strcpy(server_addr.sun_path, MY_SOCK_PATH);

   if(bind(sfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) == -1){
      perror("bind");
      return 1;
   }
   char fromClient[50];
   while(1){
      if(listen(sfd, 50) == -1){
         perror("listen");
         return 1;
      }

      client_addr_size = sizeof(client_addr);
      cfd = accept(sfd, (struct sockaddr *) &client_addr, &client_addr_size);
      if(cfd == -1){
         perror("accept");
         return 1;
      }

      if(recv(cfd, fromClient, 50, 0) == -1){
         perror("recv");
         return 1;
      }
      printf("from client: %s\n", fromClient);
   }
}