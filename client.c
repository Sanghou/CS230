#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

/*
 * DESCRIPTION: List files in "client_files" directory
 * 
 * PARAMETERS: None
 * RETURNS: None
 */
void list() {
	system('ls client_files/');
}

/*
 * DESCRIPTION: Upload the file with FILENAME to the server 
 * 
 * PARAMETERS: 
 *  - int socket: 
 *  - char* filename: name of the file to upload (in client_file directory)
 * 
 * RETURNS: None
 * 
 * NOTES: If no such file exists, then print "Error: no such file" in stdout
 */
void upload(int socket, char* filename) {
    
}

/*
 * DESCRIPTION: Download the file with FILENAME from the server 
 * 
 * PARAMETERS: 
 *  - int socket: 
 *  - char* filename: name of the file to download (in server_file directory)
 * 
 * RETURNS: None
 * 
 * NOTES: If no such file exists, then print "Error: no such file" in stdout
 */
void download(int socket, char* filename) {
    
}

int main(int argc, char* argv[]) {

  int c; 

  char *ipAddress;
  char *portNumber;
  int status;
  struct addrinfo hints;
  struct addrinfo *servinfo;

  while ((c = getopt (argc, argv, "h:p:o:s:")) != -1) {
    switch (c)
      {
      case 'h':
        ipAddress = optarg;
        break;
      case 'p':
        portNumber = optarg;
        break;
      default:
      	printf("other language %s \n", c);
        abort ();
      }
  }

  for (index = optind; index < argc; index++)
    printf ("Non-option argument %s\n", argv[index]);

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
  
  socket_fd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
  
  // bind(socket_fd, servinfo->ai_addr, servinfo->ai_addrlen);
  
  // printf("socket_fd : %d \n", socket_fd);

  check = connect(socket_fd, servinfo->ai_addr, servinfo->ai_addrlen);
  printf("connect : %d \n", check);
  
  while (1) {

  	char* str;

  	printf("[20150148]> ");
  	gets(str);

  	char cmd[] = str;

  	printf("%s \n", cmd);
  }

  // check = send(socket_fd, msg, htonl(length), 0);

  close(socket_fd);
  freeaddrinfo(servinfo);
  return 0;
}