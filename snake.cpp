
#include <cstdio>
#include <deque>
#include <ncurses.h>
#include <random>

#include "snake.hpp"

Snake::Snake(int snake_size, int winy, int winx)
    : snake_size{ snake_size }, win_y{ winy }, win_x{ winx } {
  
  this->set_direction(RIGHT);
  
  this->snake.assign({
      {10, this->win_y / 2 },
      {11, this->win_y / 2 },
      {12, this->win_y / 2 }
    });

  this->set_snake_head(this->snake.back());
  this->set_snake_tail(this->snake.front());

  this->spawn_fruit();

  initscr();
  cbreak();
  keypad(stdscr, TRUE);
  noecho();
  nodelay(stdscr, TRUE);
  curs_set(0);
}

Snake::~Snake() { endwin(); }

int Snake::get_snake_size() const { return this->snake_size; }

std::deque<point_t> Snake::get_snake() const { return snake; }
 
point_t Snake::get_snake_head() const { return this->snake_head; }
point_t Snake::get_snake_tail() const { return this->snake_tail; }
point_t Snake::get_fruit_pos() const { return this->fruit_pos; }

direction_t Snake::get_direction() const { return this->direction; }

void Snake::set_snake_size(int snake_size) {
  this->snake_size = snake_size;
}

void Snake::set_snake_head(const point_t& point) {
  set_point(this->snake_head, point);  
}

void Snake::set_snake_tail(const point_t& point) {
  set_point(this->snake_tail, point);
}

void Snake::set_fruit_pos(const point_t& point) {
  set_point(this->fruit_pos, point);  
}

void Snake::spawn_fruit() {
  int x = this->random_number(1, this->win_x);
  int y = this->random_number(1, this->win_y);
  this->set_fruit_pos({x, y});
}

void Snake::grow() {
  
  point_t new_tail = this->snake.front();
  
  if (this->get_direction() == UP) 
    new_tail.y += 1;
  else if (this->get_direction() == DOWN) 
    new_tail.y -= 1;
  else if (this->get_direction() == LEFT) 
    new_tail.x += 1;
  else if (this->get_direction() == UP) 
    new_tail.x -= 1;
  
  this->snake.push_front(new_tail);

  this->set_snake_size(get_snake_size() + 1);
    
}

void Snake::set_direction(direction_t direction) {
  this->direction = direction;
}

void Snake::set_point(point_t& dest, const point_t& src) {
  dest.x = src.x;
  dest.y = src.y;
}


int Snake::random_number(int min, int max) {
  
  std::mt19937 mt{ std::random_device{}() };
  std::uniform_int_distribution rand{ 1, max };

  return rand(mt);
  
}

void Snake::draw() {
  clear();

  mvprintw(0, 0, "Fruits collected: %d\n", this->get_snake_size() - 3);

  if (this->get_snake_head().x == this->get_fruit_pos().x &&
      this->get_snake_tail().y == this->get_fruit_pos().y) {
    grow();
    spawn_fruit();
    refresh();
  }
    

  for (const auto& s : this->snake) {
    if (this->get_snake_head().x >= this->win_x ||
        this->get_snake_head().y >= this->win_y ||
        this->get_snake_head().x <= 0 ||
        this->get_snake_head().x <= 0 ) {
      clear();
      mvprintw(win_y / 2, win_x / 2, "YOU LOST.......");
      
      this->set_direction(LOST);
      return; 
    }
    
          
    move(s.y, s.x);
    echochar('O');
    refresh();
  }

  move(this->fruit_pos.y, this->fruit_pos.x);
  echochar('X');
}

void Snake::up() {
  
  this->set_direction(UP);
  
  point_t temp_head = this->snake.back();
  temp_head.y -= 1;
  this->snake.push_back(temp_head);
  this->snake.pop_front();

  this->set_snake_head(this->snake.back());
  this->set_snake_tail(this->snake.front());
  

  
}

void Snake::down() {

  this->set_direction(DOWN);
  
  point_t temp_head = this->snake.back();
  temp_head.y += 1;
  this->snake.push_back(temp_head);
  this->snake.pop_front();

  this->set_snake_head(this->snake.back());
  this->set_snake_tail(this->snake.front());
}

void Snake::left() {
  
  this->set_direction(LEFT);
  
  point_t temp_head = this->snake.back();
  temp_head.x -= 1;
  this->snake.push_back(temp_head);
  this->snake.pop_front();

  this->set_snake_head(this->snake.back());
  this->set_snake_tail(this->snake.front());
  
}

void Snake::right() {
  
  this->set_direction(RIGHT);
  
  point_t temp_head = this->snake.back();
  temp_head.x += 1;
  this->snake.push_back(temp_head);
  this->snake.pop_front();

  this->set_snake_head(this->snake.back());
  this->set_snake_tail(this->snake.front());
}

