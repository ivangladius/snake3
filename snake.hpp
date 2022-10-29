
#ifndef SNAKE_HEADER
#define SNAKE_HEADER

#include <deque>

typedef enum Direction {
  UP,
  DOWN,
  LEFT,
  RIGHT,
  LOST,
} direction_t ;

typedef struct Point {
  int x;
  int y;
} point_t;

class Snake {
private:
  int snake_size;
  int win_y, win_x;
  
  std::deque<point_t> snake;
  
  point_t snake_head;
  point_t snake_tail;
  point_t fruit_pos;

  direction_t direction;

public:

  Snake(int size, int, int);
  ~Snake();

  int get_snake_size() const;
  std::deque<point_t> get_snake() const;

  point_t get_snake_head() const;
  point_t get_snake_tail() const;
  
  point_t get_fruit_pos() const;

  direction_t get_direction() const;


  void set_snake_size(int);
  
  void set_snake_head(const point_t&);
  void set_snake_tail(const point_t&);
  
  void set_fruit_pos(const point_t&);
  void spawn_fruit();
  void grow();

  void set_direction(direction_t);

public:
  
  void up();
  void down();
  void left();
  void right();

public:
  
  void set_point(point_t&, const point_t&);
  int random_number(int min, int max);
  void draw();
  
};

#endif
