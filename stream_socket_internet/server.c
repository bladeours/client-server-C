#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <netinet/ip.h>
#include <sys/socket.h>
#include <unistd.h>

int sfd,cfd;

void signalHandler(){
   printf("\nserver stop\n");
   close(sfd);
   close(cfd);
   exit(0);
}

int main(){
   signal(SIGINT, signalHandler);
   struct sockaddr_in server_addr, client_addr;

   sfd = socket(AF_INET, SOCK_STREAM, 0);
   if(sfd == -1){
      perror("socket");
      return -1;
   }
   server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
        server_addr.sin_port = htons(1234);
   
   if (bind(sfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) == -1) {
               perror("bind");
               return -1;
           }
   char fromClient[50];
   while(1){
      if(listen(sfd, 50) == -1){
         perror("listen");
         return -1;
      }
      socklen_t client_addr_size = sizeof(client_addr);

      cfd = accept(sfd, (struct sockaddr *) &client_addr, &client_addr_size);
      if(cfd == -1){
         perror("accept");
         return -1;
      }
      if(recv(cfd,fromClient, 50, 0) == -1){
         perror("recv");
         return -1;
      }
      printf("from client: %s\n", fromClient);
   }
}
