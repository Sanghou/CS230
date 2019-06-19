#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define MAX 100
/*
 * DESCRIPTION: List files in "client_files" directory
 * 
 * PARAMETERS: None
 * RETURNS: None
 */
void list() {
	system("ls Client_files/");
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

  DIR *dir;
  struct dirent *dent;

  int proper = 0;

	filename[strlen(filename)] = '\0';

  dir = opendir('Client/');

  if (dir != NULL) {
    while((dent=readdir(dir)) != NULL){
      if (strcmp(dent->d_name,filename)==0) {
        printf("exists want file!!");
        proper = 1;
        break;
      }
      printf(dent->d_name);
    }
  }

  if (!proper) {
    printf("no such file \n");
    exit(-1);
  }

  FILE *file;
  char* full_name = '/Client/';
  strcat(full_name,filename);

  printf("%s\n",full_name);

  file = fopen(full_name, "rb");

  send(socket, file, strlen(filename),0);



  close(dir);

	printf("filename : %s", filename); 



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
    printf("filename : %s",filename);
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

  for (int index = 0; index < argc; index++)
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
 
	
  char str[MAX];
 
  while (1) {

    char* arg;
    char* cmd;

    cmd = (char *) malloc(MAX);

    printf("[20150148]> ");

    fgets(str, MAX, stdin);

    memcpy(cmd, str, MAX);	
    cmd[strlen(str) - 1] = '\0';
    
    printf("input : %s \n", cmd);
	
    strtok_r(cmd," ", &arg);
	
    printf("**%s**\n", cmd);
    printf("**%s**\n", arg);


    if(strncmp(cmd,"ls",2) == 0) {
	list();
    } else if (strcmp(cmd,"upload") == 0 ) {
	upload(socket_fd,arg);
    } else if (strcmp(cmd,"download") == 0) {
	download(socket_fd,arg);
    }

    free(cmd);

  }

  // check = send(socket_fd, msg, htonl(length), 0);

  close(socket_fd);
  freeaddrinfo(servinfo);
  return 0;
}
