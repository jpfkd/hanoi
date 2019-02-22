#include "hanoi.hpp"

#include <limits>
#include <numeric>

#include <cassert>

namespace hanoi {

Tower::Tower(int num_discs) {
  if (num_discs < 0) {
    throw TowerInitFailure{
        "Tried to create a tower that has minus number of discs."};
  }
  discs_m.resize(num_discs);
  std::iota(discs_m.rbegin(), discs_m.rend(), 1);
}

namespace {

std::size_t pos_to_idx(Hanoi::TowerPos pos) noexcept {
  assert(pos == Hanoi::TowerPos::left || pos == Hanoi::TowerPos::center ||
         pos == Hanoi::TowerPos::right);
  return static_cast<std::size_t>(pos);
}

}  // namespace

Tower& Hanoi::tower_at(TowerPos pos) noexcept {
  return towers_m[pos_to_idx(pos)];
}

const Tower& Hanoi::tower_at(TowerPos pos) const noexcept {
  return towers_m[pos_to_idx(pos)];
}

Hanoi::MoveResult Hanoi::predict_move(MoveDirection dir) const noexcept {
  if (dir.from == dir.dest) {
    return MoveResult::same_tower;
  }

  const auto& tower_from = tower_at(dir.from);
  const auto& tower_dest = tower_at(dir.dest);

  if (tower_from.empty()) {
    return MoveResult::empty_tower;
  }

  if (!tower_dest.empty() && (tower_from.top_disc() > tower_dest.top_disc())) {
    return MoveResult::larger_on_smaller;
  }

  return MoveResult::success;
}

Hanoi::MoveResult Hanoi::move_disc(MoveDirection dir) {
  if (MoveResult result; (result = predict_move(dir)) != MoveResult::success) {
    return result;
  }

  auto& tower_from = tower_at(dir.from);
  auto& tower_dest = tower_at(dir.dest);

  tower_dest.put_disc(tower_from.top_disc());
  tower_from.take_disc();

  log_m.push_back(dir);

  return MoveResult::success;
}

std::string pos_to_str(Hanoi::TowerPos pos) {
  switch (pos) {
    case Hanoi::TowerPos::left:
      return "left";
    case Hanoi::TowerPos::center:
      return "center";
    case Hanoi::TowerPos::right:
      return "right";
    default:
      assert(!"TowerPos value must be left, center or right.");
  }
}

}  // namespace hanoi
