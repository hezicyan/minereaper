#include "reaper.h"

#include "../game/game.h"

using namespace reaper;

void Reaper::GetBlock(int x, int y) {
  if (vis_[x][y]) return;
  vis_[x][y] = true;
  auto now = game_->GetCell(game::Coord(x, y));
  if (!now.revealed) {
    mines.push_back(std::make_pair(x, y));
    return;
  }
  for (auto p : game::kAdjDirs) {
    int xx = x + p.first;
    int yy = y + p.second;
    if (xx >= 0 and xx < n_ and yy >= 0 and yy <= m_) {
      GetBlock(xx, yy);
    }
  }
}
std::vector<int> ismine;
void Reaper::Dfs(int t) {
  if (t >= mines.size()) {
    for (int u : ismine) {
      tot_[mines[u].first][mines[u].second]++;
    }
    tot_situations_++;
    return;
  }
  ismine.push_back(t);
  int x = mines[t].first, y = mines[t].second;
  bool flag = 1;
  for (auto p : game::kAdjDirs) {
    int xx = x + p.first;
    int yy = y + p.second;
    if (xx >= 0 and xx < n_ and yy >= 0 and yy <= m_) {
      auto now = game_->GetCell(game::Coord(xx, yy));
      if (now.revealed and now.number != 9) cnt[xx][yy]++;
      if (cnt[xx][yy] > now.number) flag = 0;
    }
  }
  if (flag) dfs(t + 1);
  for (auto p : game::kAdjDirs) {
    int xx = x + p.first;
    int yy = y + p.second;
    if (xx >= 0 and xx < n_ and yy >= 0 and yy <= m_) {
      auto now = game_->GetCell(game::Coord(xx, yy));
      if (now.revealed and now.number != 9) cnt[xx][yy]--;
    }
  }
  ismine.pop_back();
  dfs(t + 1);
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
      auto now = game->GetCell(game::Coord(i, j));
      if (now.revealed and now.number != 9) {
        tot_situations_ = 0;
        blocks_.clear();
        mines_.clear();
        memset(tot_, 0, sizeof(tot_));
        memset(cnt, 0, sizeof(cnt));
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
