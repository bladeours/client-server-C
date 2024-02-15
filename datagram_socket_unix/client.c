#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <netinet/ip.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/un.h>


#define MY_SOCK_PATH "/tmp/serverDatagram1"

int main(int argc, char * argv[]){
   struct sockaddr_un server_addr;
   int sfd = socket(AF_UNIX, SOCK_DGRAM, 0);
   
   if(sfd == -1){
      perror("socket");
      return -1;
   }
   memset(&server_addr, 0, sizeof(server_addr));
   server_addr.sun_family = AF_UNIX;
   strcpy(server_addr.sun_path, MY_SOCK_PATH);

   if(sendto(sfd, argv[1], 50, 0, (struct sockaddr *) &server_addr,
                  sizeof(server_addr)) == -1){
      perror("sendto");
      return -1;
   }
   return 0;
}
