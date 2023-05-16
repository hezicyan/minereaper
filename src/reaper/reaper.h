#ifndef REAPER_H
#define REAPER_H

#include "../game/game.h"

namespace reaper {

class Reaper {
 private:
  void Dfs(int x, int y);

  const game::Game* game_;
  int n_, m_, tot_situations_;
  std::vector<std::vector<int>> cnt_, tot_;
  std::vector<std::vector<bool>> vis_;
  std::vector<game::Coord> mines_;
  std::vector<game::Coord> blocks_;

 public:
  std::vector<std::vector<double>> GetPossibility(const game::Game* game);
};

}  // namespace reaper

#endif