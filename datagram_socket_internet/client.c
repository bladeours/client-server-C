#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <string.h>

int main(int argc, char * argv[]){
   
   struct sockaddr_in server_addr;

        int sfd = socket(AF_INET, SOCK_DGRAM, 0);
        if (sfd == -1){
      perror("socket");
      return -1;
    }
    memset(&server_addr, 0, sizeof(server_addr));

   server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
        server_addr.sin_port = htons(1234);
   
   if(sendto(sfd, argv[1], 50, 0, (struct sockaddr *) &server_addr,
            sizeof(server_addr)) == -1){
      perror("sendto");
      return -1;
   }

   return 0;
}
