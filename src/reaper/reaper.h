#ifndef REAPER_H
#define REAPER_H

#include "../game/game.h"

namespace reaper {

const double kEps = 1e-6;

int Sign(double x);

class Reaper {
 private:
  game::Game* const game_;
  int n_, m_, tot_situations_;
  std::vector<std::vector<int>> cnt_, tot_;
  std::vector<std::vector<bool>> vis_;
  std::vector<game::Coord> borders_;
  std::vector<game::Coord> blocks_;
  std::vector<int> mines_;

  bool IsValidCoord(const game::Coord& pos) const;
  bool OnCorner(const game::Coord& pos) const;
  bool OnBorder(const game::Coord& pos) const;

  void GetBlock(int x, int y);
  void Dfs(int t);
  std::vector<std::vector<double>> GetPossibility();

 public:
  Reaper(game::Game* game);
  ~Reaper();

  game::Coord GetNextStep();
  void DoNextStep();
};

}  // namespace reaper

#endif