
#include <deque>
#include <iostream>
#include <random>

int main() {

  std::mt19937 mt{ std::random_device{}() };
  std::uniform_int_distribution die6{ 1, 10 };

  for (int x = 0; x < 10; x++)
    std::cout << die6(mt) << std::endl;
    
}
