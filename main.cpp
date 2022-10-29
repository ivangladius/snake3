
#include <iostream>
#include <unistd.h>
#include <ncurses.h>

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


 
#include "snake.hpp"

#define PAGE sysconf(_SC_PAGESIZE)

#define handle(str)                             \
  do {perror(str);exit(EXIT_FAILURE);}while(0)

int main(int argc, char ** argv) {


  int shm_id = shm_open("/snake", O_RDWR | O_CREAT, 0644);
  if (shm_id == -1)
    handle("shm_open");

  char * data = (char *)mmap(0, PAGE, PROT_READ, MAP_SHARED, shm_id, 0);
  if (!data)
    handle("mmap");

  Snake snake(3, atoi(argv[1]), atoi(argv[2]));

  std::cout << "running game.." << std::endl;
  for (;;) {

    int key = getch();

    if (strstr(data, "UP"))
      snake.up();
    else if (strstr(data, "DOWN"))
      snake.down();
    else if (strstr(data, "LEFT"))
      snake.left();
    else if (strstr(data, "RIGHT"))
      snake.right();

    
    if (key == KEY_UP)
      snake.up();
    else if (key == KEY_DOWN)
      snake.down();
    else if (key == KEY_LEFT)
      snake.left();
    else if (key == KEY_RIGHT)
      snake.right();
    

    if (snake.get_direction() == UP)
      snake.up();
    else if (snake.get_direction() == DOWN)
      snake.down();
    else if (snake.get_direction() == LEFT)
      snake.left();
    else if (snake.get_direction() == RIGHT)
      snake.right();
    else if (snake.get_direction() == LOST)
      break;


    snake.draw();
    usleep(100000);
  }

  if (munmap(data, PAGE) == -1)
    handle("munmap");

  close(shm_id);

  nodelay(stdscr, FALSE);
  getch();

  return 0;
}
