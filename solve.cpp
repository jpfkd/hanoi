#include "solve.hpp"
#include "hanoi.hpp"

#include <array>
#include <stack>
#include <utility>

#include <cassert>

namespace hanoi {

namespace {

// Still can fail when both towers are empty.
Hanoi::MoveResult move_either_direction(Hanoi& game, Hanoi::MoveDirection dir) {
  if (game.predict_move(dir) != Hanoi::MoveResult::success) {
    std::swap(dir.from, dir.dest);
  }

  return game.move_disc(dir);
}

void move_either_direction_or_die(Hanoi& game, Hanoi::MoveDirection dir) {
  auto move_result = move_either_direction(game, dir);
  assert(move_result == Hanoi::MoveResult::success);
}

}  // namespace

void solve_hanoi_iterative(Hanoi& game) {
  game = Hanoi{game.num_discs()};

  std::array<Hanoi::MoveDirection, 3> procedure;
  if ((game.num_discs() % 2) == 0) {
    procedure[0] = {Hanoi::TowerPos::left, Hanoi::TowerPos::center};
    procedure[1] = {Hanoi::TowerPos::left, Hanoi::TowerPos::right};
    procedure[2] = {Hanoi::TowerPos::center, Hanoi::TowerPos::right};
  } else {
    procedure[0] = {Hanoi::TowerPos::left, Hanoi::TowerPos::right};
    procedure[1] = {Hanoi::TowerPos::left, Hanoi::TowerPos::center};
    procedure[2] = {Hanoi::TowerPos::center, Hanoi::TowerPos::right};
  }

  for (auto it = procedure.begin(); !game.is_completed();) {
    move_either_direction_or_die(game, *it);

    ++it;
    if (it == procedure.end()) {
      it = procedure.begin();
    }
  }
}

void solve_hanoi_recursive(Hanoi& game) {
  game = Hanoi{game.num_discs()};

  struct StackEntry {
    int n;
    Hanoi::TowerPos source;
    Hanoi::TowerPos target;
    Hanoi::TowerPos auxilary;
    bool pushed1{false};
    bool pushed2{false};
  };

  std::stack<StackEntry> callstack;

  callstack.push({game.num_discs(), Hanoi::TowerPos::left,
                  Hanoi::TowerPos::right, Hanoi::TowerPos::center});

  while (!callstack.empty()) {
    auto [n, source, target, auxilary, pushed1, pushed2] = callstack.top();
    if (!pushed2) {
      if (!pushed1 && n > 1) {
        callstack.top().pushed1 = true;
        callstack.push({n - 1, source, auxilary, target});
        continue;
      }
      game.move_disc({source, target});
      if (n > 1) {
        callstack.top().pushed2 = true;
        callstack.push({n - 1, auxilary, target, source});
        continue;
      }
    }
    callstack.pop();
  }
}

}  // namespace hanoi
