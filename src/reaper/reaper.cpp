#include "reaper.h"

#include "../game/game.h"

using namespace reaper;

void Reaper::Dfs(int x, int y) {
  if (x >= n_) {
    ++tot_situations_;
    for (auto u : mines_) ++tot_[u.x][u.y];
    return;
  }
  if (vis_[x][y]) return;
  vis_[x][y] = true;
  auto goto_next = [x, y, this]() {
    if (y == m_ - 1) {
      Dfs(x + 1, 0);
    } else {
      Dfs(x, y + 1);
    }
  };
  auto now = game_->GetCell(game::Coord(x, y));
  if (now.revealed) goto_next();
  blocks_.push_back(game::Coord(x, y));
  bool flag = true;
  for (auto p : game::kAdjDirs) {
    int xx = x + p.first;
    int yy = y + p.second;
    if (xx >= 0 and xx < n_ and yy >= 0 and yy <= m_) {
      cnt_[xx][yy]++;
      auto nnow = game_->GetCell(game::Coord(xx, yy));
      if (cnt_[xx][yy] > nnow.number) flag = 0;
    }
  }
  mines_.push_back(game::Coord(x, y));
  if (flag) goto_next();
  mines_.pop_back();
  for (auto p : game::kAdjDirs) {
    int xx = x + p.first;
    int yy = y + p.second;
    if (xx >= 0 and xx < n_ and yy >= 0 and yy <= m_) {
      cnt_[xx][yy]--;
    }
  }
  if (flag) goto_next();
}

std::vector<std::vector<double>> Reaper::GetPossibility(
    const game::Game* game) {
  game_ = game;
  cnt_ = std::vector<std::vector<int>>(n_, std::vector<int>(m_, 0));
  tot_ = std::vector<std::vector<int>>(n_, std::vector<int>(m_, 0));
  vis_ = std::vector<std::vector<bool>>(n_, std::vector<bool>(m_, false));

  std::vector<std::vector<double>> res(n_, std::vector<double>(m_, 0));
  for (int i = 0; i < n_; i++) {
    for (int j = 0; j < m_; j++) {
      if (vis_[i][j]) continue;
      tot_situations_ = 0;
      blocks_.clear();
      mines_.clear();
      Dfs(i, j);
      for (auto u : blocks_) {
        res[u.x][u.y] = (double)tot_[u.x][u.y] / tot_situations_;
      }
    }
  }
  return res;
}
