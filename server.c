#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define BUF 1000000

void save(int client_fd) {
  char file_name[100];
  //char file_size[100];


  int read_len = recv(client_fd, file_name, sizeof(file_name), MSG_WAITALL);
  printf("read_len : %d\n", read_len);
  //read_len = recv(client_fd, file_size, sizeof(file_size), 0);
  //printf("read_len : %d\n", read_len);
 
  printf("name %s\n", file_name);
  //printf("size %s\n", file_size); 


}


int main(int argc, char* argv[]) {

  int c;

  char* ipAddress = "127.0.0.1";
  char* portNumber;

  int server_socket;
  int client_socket;
	
  int status; 

  struct addrinfo hints;
  struct addrinfo *servinfo;
  struct addrinfo *clientinfo;

  while((c = getopt (argc, argv, "p:")) != -1) {
    switch(c)
      {
      case 'p':
	portNumber = optarg;
	break;
      default:
	printf("other language %s \n",c);
	break;
      }
  }

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  if ((status = getaddrinfo(ipAddress, portNumber, &hints, &servinfo)) != 0) {
    fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
    exit(1);
  }

  int socket_fd;
  int check;
  int client_fd;
  int client_len = sizeof(clientinfo);

  socket_fd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);

  bind(socket_fd, servinfo->ai_addr, servinfo->ai_addrlen);

  listen(socket_fd, 5);

  while(1)
  {
    char buf[BUF];
     
    printf("listen ready \n");

    client_fd = accept(socket_fd, (struct addrinfo*)&clientinfo, &client_len);
    printf("Client Conneted : %d\n", client_fd);

    int read_len = read(client_fd, buf, BUF);

    printf("%s , %d\n", buf, read_len);

    if (strcmp(buf, "upload") == 0) {
      printf("upload function\n");
      save(client_fd);
    
    } else if (strcmp(buf, "download") ==0) {
      printf("download function \n");
    }

  }

  printf("socket_fd %d\n", socket_fd);
  return 0;
}
