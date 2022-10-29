
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PAGE sysconf(_SC_PAGESIZE)

#define handle(str)                             \
  do {perror(str);exit(EXIT_FAILURE);}while(0)

int main(int argc, char ** argv) {

  struct sockaddr_in sockaddr;
  int serverfd, clientfd;

  memset(&sockaddr, 0, sizeof(sockaddr));
  sockaddr.sin_family = AF_INET;
  sockaddr.sin_addr.s_addr = INADDR_ANY;
  sockaddr.sin_port = htons(atoi(argv[1]));

  if ((serverfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    handle("socket");

  if (bind(serverfd, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) == -1)
    handle("bind");

  if (listen(serverfd, 5) == -1)
    handle("listen");

  int shm_id = shm_open("/snake",  O_RDWR | O_CREAT, 0644);
  if (shm_id == -1)
    handle("shm_open");

  if (ftruncate(shm_id, PAGE) == -1)
    handle("ftruncate");

  char * data = (char *)mmap(0, PAGE, PROT_WRITE, MAP_SHARED, shm_id, 0);
  if (!data)
    handle("mmap");

  char http_header[256] = "GET HTTP/1.1 200 OK\r\n\n";
  char http_buffer[4096];
  char full_http[16000];
  
  FILE * htmlFile = fopen("index.html", "r");
  if (!htmlFile)
    handle("fopen");


  char line[100];
  while (fgets(line, 100, htmlFile) != 0)
    strcat(http_buffer, line);
  
  fclose(htmlFile);

  strncat(full_http, http_header, sizeof(http_header));
  strncat(full_http, http_buffer, sizeof(http_buffer));

  char buffer[4096];
  char * mode;

  for( ;;) {
    
    if ((clientfd = accept(serverfd, NULL, NULL)) == -1)
      handle("accept");
    
    if (send(clientfd, full_http, strlen(full_http), 0) == -1)
      handle("write");

    if (read(clientfd, buffer, sizeof(buffer)) == -1)
      handle("read");

    buffer[strlen(buffer)] = '\0';

    mode = strstr(buffer, "mode=");
    if (mode) {
      memcpy(data, mode+5, strlen(mode+5));
      printf("strlen(mode+5): %s : %ld\n", mode+5, strlen(mode+5));
    }
    
      
    memset(buffer, 0, sizeof(buffer));

  }

  if (munmap(data, PAGE) == -1)
    handle("munmap");

  close(serverfd);

  close(shm_id);

  shm_unlink(argv[1]);

  return 0;
}

