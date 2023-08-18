#include <iostream>
#include <string>

#include "lib.hpp"

int main()
{
  library lib = library {};
  auto const message = "Hello from " + lib.name + "!";
  std::cout << message << '\n';
  return 0;
}
