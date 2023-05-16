#ifndef REAPER_H
#define REAPER_H

#include "../game/game.h"

namespace reaper {

class Reaper {
 private:
  game::Game* const game_;
  int n_, m_, tot_situations_;
  std::vector<std::vector<int>> cnt_, tot_;
  std::vector<std::vector<bool>> vis_;
  std::vector<game::Coord> borders_;
  std::vector<game::Coord> blocks_;
  std::vector<int> mines_;

  void GetBlock(int x, int y);
  void Dfs(int t);
  bool IsValidCoord(const game::Coord& pos);
  std::vector<std::vector<double>> GetPossibility();

 public:
  void Print();
  Reaper(game::Game* game);
  ~Reaper();
};

}  // namespace reaper

#endif