#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <netinet/ip.h>
#include <sys/socket.h>
#include <unistd.h>

int main(int argc, char * argv[]){
   struct sockaddr_in server_addr;
   int sfd = socket(AF_INET, SOCK_STREAM, 0);
   if(sfd == -1){
      perror("socket");
      return -1;
   }
   server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
        server_addr.sin_port = htons(1234);
   
   if(connect(sfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) == -1){
      perror("connect");
      return -1;
   }
   if(send(sfd, argv[1], 50, 0) == -1){
      perror("send");
      return -1;
   }

   close(sfd);
   return 0;
}
