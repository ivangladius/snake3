
#include <iostream>
#include <unistd.h>
#include <ncurses.h>
 
#include "snake.hpp"

int main(int argc, char ** argv) {

  Snake snake(3, atoi(argv[1]), atoi(argv[2]));

  std::cout << "running game.." << std::endl;
  for (;;) {

    int key = getch();
    
    if (key == KEY_UP)
      snake.up();
    if (key == KEY_DOWN)
      snake.down();
    if (key == KEY_LEFT)
      snake.left();
    if (key == KEY_RIGHT)
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

  nodelay(stdscr, FALSE);
  getch();

  return 0;
}
