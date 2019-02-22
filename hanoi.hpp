#pragma once

#include <array>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace hanoi {

class TowerInitFailure : public std::logic_error {
 public:
  using std::logic_error::logic_error;
};

class EmptyTower : public std::logic_error {
 public:
  using std::logic_error::logic_error;

  EmptyTower(const std::string& what_arg, int cnt_moves)
      : std::logic_error{what_arg}, cnt_moves_m{cnt_moves} {}
  EmptyTower(const char* what_arg, int cnt_moves)
      : std::logic_error{what_arg}, cnt_moves_m{cnt_moves} {}

  inline void set_cnt_moves(int new_cnt_moves) noexcept;
  int cnt_moves() const noexcept { return cnt_moves_m; }

 private:
  int cnt_moves_m{-1};
};

inline void EmptyTower::set_cnt_moves(int new_cnt_moves) noexcept {
  cnt_moves_m = new_cnt_moves;
}

// Represents a tower in the puzzle.
class Tower {
 public:
  using disc_size_t = int;

  Tower() = default;
  Tower(int num_discs);

  inline disc_size_t top_disc() const noexcept;

  bool empty() const noexcept { return discs_m.empty(); }
  int num_discs() const noexcept { return static_cast<int>(discs_m.size()); }

  void put_disc(disc_size_t size) { discs_m.push_back(size); }
  inline void take_disc();

 private:
  std::vector<disc_size_t> discs_m;
};

inline Tower::disc_size_t Tower::top_disc() const noexcept {
  if (empty()) {
    return 0;
  }
  return discs_m.back();
}

inline void Tower::take_disc() {
  if (discs_m.empty()) {
    throw EmptyTower{"Tried to take a disc from an empty tower"};
  }
  discs_m.pop_back();
}

// Represents a state of the puzzle.
class Hanoi {
 public:
  enum class TowerPos { left, center, right };
  enum class MoveResult { success, same_tower, empty_tower, larger_on_smaller };
  struct MoveDirection;

  Hanoi(int num_discs) : towers_m{num_discs, {}, {}}, num_discs_m{num_discs} {}

  inline bool is_completed() const noexcept;
  int num_discs() const noexcept { return num_discs_m; }
  std::size_t cnt_moves() const noexcept { return log_m.size(); }

  std::vector<MoveDirection> log() const& { return log_m; }
  std::vector<MoveDirection> log() const&& { return std::move(log_m); }

  MoveResult predict_move(MoveDirection dir) const noexcept;

  MoveResult move_disc(MoveDirection dir);

  const Tower& tower_view(TowerPos pos) const noexcept { return tower_at(pos); }

  Tower& tower_at(TowerPos pos) noexcept;
  const Tower& tower_at(TowerPos pos) const noexcept;

  std::array<Tower, 3> towers_m;
  int num_discs_m;
  std::vector<MoveDirection> log_m;
};

struct Hanoi::MoveDirection {
  TowerPos from;
  TowerPos dest;
};

inline bool Hanoi::is_completed() const noexcept {
  return tower_at(TowerPos::right).num_discs() == num_discs();
}

// Helper functions
std::string pos_to_str(Hanoi::TowerPos pos);

inline bool operator==(Hanoi::MoveDirection rhs, decltype(rhs) lhs) {
  return ((rhs.from == lhs.from) && (rhs.dest == lhs.dest));
}

inline bool operator!=(Hanoi::MoveDirection rhs, decltype(rhs) lhs) {
  return !(rhs == lhs);
}

}  // namespace hanoi
