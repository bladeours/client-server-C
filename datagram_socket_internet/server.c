#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <string.h>

int sfd, cfd;

void signalHandler(int sig){
   printf("\nserver stop\n");
   close(sfd);
   close(cfd);
   exit(0);
}

int main(){
   signal(SIGINT, signalHandler);
   struct sockaddr_in server_addr, client_addr;
        socklen_t client_addr_size = sizeof(client_addr);

        sfd = socket(AF_INET, SOCK_DGRAM, 0);
        if (sfd == -1){
      perror("socket");
      return -1;
    }
    memset(&server_addr, 0, sizeof(client_addr));

   server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
        server_addr.sin_port = htons(1234);

   if(bind(sfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) == -1){
      perror("bind");
      return -1;
   }
   char fromClient[50];
   while(1){
      if(recvfrom(sfd, fromClient, 50, 0, (struct sockaddr *) &client_addr,
            &client_addr_size) == -1){
         perror("recvfrom");
         return -1;
      }

      printf("from client: %s\n");
   }



}
