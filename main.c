#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <fcntl.h>

//deinetion
#define DOMAIN_NAME 0
#define SOCK_TYPE 0
#define PROTOCL_TCP 0 
#define SIZE 70000

void Error (int exitcode, const char * message){
  perror (message);
  exit(exitcode);
}

void Maclloc (unsigned char **buffer){
  *buffer  = (unsigned char *) malloc (SIZE);
  if (*buffer == NULL)
    Error(2, "malloc");
}

int main (){
  int log_fd ; 
  int socketfd = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
  if (socketfd < 0)
      Error(1, "socket");
  log_fd = open ("log_file.txt", 1);
  while (1){
    
  }
  close (log_fd);
  return 0;
}
