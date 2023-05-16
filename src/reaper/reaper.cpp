#include "reaper.h"

#include "../game/game.h"

using namespace reaper;

Reaper::Reaper(game::Game* game) : game_(game) {}

Reaper::~Reaper() {}

bool Reaper::IsValidCoord(const game::Coord& pos) {
  return pos.x >= 0 && pos.x < n_ && pos.y >= 0 && pos.y < n_;
}

void Reaper::GetBlock(int x, int y) {
  if (vis_[x][y]) return;
  vis_[x][y] = true;
  auto now = game_->GetCell(game::Coord(x, y));
  if (!now.revealed) {
    borders_.push_back(game::Coord(x, y));
    return;
  }
  for (auto p : game::kAdjDirs) {
    int xx = x + p.first;
    int yy = y + p.second;
    if (IsValidCoord(game::Coord(xx, yy))) {
      GetBlock(xx, yy);
    }
  }
}

void Reaper::Dfs(int t) {
  if (t >= borders_.size()) {
    for (int u : mines_) {
      tot_[borders_[u].x][borders_[u].y]++;
    }
    tot_situations_++;
    return;
  }
  mines_.push_back(t);
  int x = borders_[t].x, y = borders_[t].y;

  bool valid = true;
  for (auto p : game::kAdjDirs) {
    int xx = x + p.first;
    int yy = y + p.second;
    auto pos = game::Coord(xx, yy);
    if (IsValidCoord(pos)) {
      auto now = game_->GetCell(pos);
      if (now.revealed and now.number != 9) cnt_[xx][yy]++;
      if (cnt_[xx][yy] > now.number) valid = false;
    }
  }
  if (valid) Dfs(t + 1);

  for (auto p : game::kAdjDirs) {
    int xx = x + p.first;
    int yy = y + p.second;
    auto pos = game::Coord(xx, yy);
    if (IsValidCoord(pos)) {
      auto now = game_->GetCell(pos);
      if (now.revealed and now.number != 9) cnt_[xx][yy]--;
    }
  }

  mines_.pop_back();
  Dfs(t + 1);
}

std::vector<std::vector<double>> Reaper::GetPossibility() {
  vis_ = std::vector<std::vector<bool>>(n_, std::vector<bool>(m_, false));
  mines_.clear();

  std::vector<std::vector<double>> res(n_, std::vector<double>(m_, 0));
  for (int i = 0; i < n_; i++) {
    for (int j = 0; j < m_; j++) {
      if (vis_[i][j]) continue;
      auto now = game_->GetCell(game::Coord(i, j));
      if (now.revealed && now.number != 9) {
        tot_situations_ = 0;
        blocks_.clear();
        borders_.clear();
        cnt_ = std::vector<std::vector<int>>(n_, std::vector<int>(m_, 0));
        tot_ = std::vector<std::vector<int>>(n_, std::vector<int>(m_, 0));
        GetBlock(i, j);
        Dfs(0);
        for (auto u : blocks_) {
          res[u.x][u.y] = (double)tot_[u.x][u.y] / tot_situations_;
        }
      }
    }
  }
  return res;
}
