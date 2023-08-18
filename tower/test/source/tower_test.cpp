#include "lib.hpp"

auto main() -> int
{
  auto const lib = library {};

  return lib.name == "tower" ? 0 : 1;
}
