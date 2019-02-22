#include "hanoi.hpp"
#include "solve.hpp"

#include <charconv>
#include <iostream>
#include <string_view>

#include <cstring>

namespace {

void msg_exit(std::string_view msg) {
  std::cerr << msg << '\n';
  exit(EXIT_FAILURE);
}

}  // namespace

int main(int argc, char** argv) {
  if (argc < 2) {
    msg_exit("Too few arguments.");
  }
  if (argc > 2) {
    msg_exit("Too many arguments.");
  }

  int hanoi_level;
  if (std::from_chars(argv[1], argv[1] + std::strlen(argv[1]), hanoi_level)
          .ec != std::errc{}) {
    msg_exit("Invalid argument");
  }

  hanoi::Hanoi game{hanoi_level};
  hanoi::solve_hanoi_iterative(game);

  for (const auto& move : game.log()) {
    std::cout << hanoi::pos_to_str(move.from) << " to "
              << hanoi::pos_to_str(move.dest) << '\n';
  }
  std::cout << "Total " << game.cnt_moves() << " movements.\n";
}
