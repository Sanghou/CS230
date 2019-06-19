#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <dirent.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define MAX 100
#define BUF 1000000

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
  dir = opendir("Client/");

/*
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
*/
  int file_len = strlen(filename) + 16;
  FILE *file;
  char full_name[file_len];

  char buf[BUF];

  strcpy(full_name, "./Client_files/");
  strcat(full_name,filename);

  printf("%s\n",full_name);

  file = fopen(full_name, "rb");

  if (file == NULL) {
	printf("no such file \n");
	close(dir);
        exit(-1);
  }
  //send file name

  send(socket, "upload", sizeof("upload"), 0);

  send(socket, filename, sizeof(filename), 0);
  printf("send name\n");
	
  int file_size = lseek(file,0, SEEK_END);
  //send file_size
  send(socket, &file_size, sizeof(file_size),0);
  printf("send size \n");
  lseek(file, 0, SEEK_SET);
  
  int read_size = 0;
  int total = 0;

  while ( total != file_size) {
    read_size = read(file, buf, BUF);
    total += read_size;
    buf[read_size] = '\0';
    send(socket, buf, read_size, 0);
    printf("sending \n");
  }
  close(file);
  closedir(dir);

  printf("end upload \n");
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
    cmd[strlen(cmd) - 1] = '\0';
   
 
    printf("input : %s \n", cmd);
	
    strtok_r(cmd," ", &arg);
	
    printf("**%s**\n", cmd);
    printf("**%s**\n", arg);

    if(strcmp(cmd,"ls") == 0) {
	printf("list!\n");
	list();
    } else if (strcmp(cmd,"upload") == 0 ) {
	printf("upload!\n");
	upload(socket_fd,arg);
	printf("end upload!\n");
    } else if (strcmp(cmd,"download") == 0) {
	printf("download! \n");
	download(socket_fd,arg);
    }
    printf("end cmd");
    free(cmd);

  }

  // check = send(socket_fd, msg, htonl(length), 0);

  close(socket_fd);
  freeaddrinfo(servinfo);
  return 0;
}
