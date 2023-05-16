#include "reaper.h"

#include <random>
#include <stdexcept>

#include "../game/game.h"

using namespace reaper;

int Sign(double x) {
  if (x > kEps) return 1;
  if (x < -kEps) return -1;
  return 0;
}

Reaper::Reaper(game::Game* game) : game_(game) {}

Reaper::~Reaper() {}

bool Reaper::IsValidCoord(const game::Coord& pos) const {
  return pos.x >= 0 && pos.x < n_ && pos.y >= 0 && pos.y < n_;
}

bool Reaper::OnCorner(const game::Coord& pos) const {
  bool dx = pos.x == 0 || pos.x == n_ - 1;
  bool dy = pos.y == 0 || pos.y == m_ - 1;
  return dx && dy;
}

bool Reaper::OnBorder(const game::Coord& pos) const {
  bool dx = pos.x == 0 || pos.x == n_ - 1;
  bool dy = pos.y == 0 || pos.y == m_ - 1;
  return dx || dy;
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

game::Coord Reaper::GetNextStep() {
  auto p = GetPossibility();
  std::vector<game::Coord> cand;
  double mn = 1;
  for (int i = 0; i < n_; ++i) {
    for (int j = 0; j < m_; ++j) {
      if (Sign(p[i][j]) == 0) return game::Coord(i, j);
      int v = Sign(p[i][j] - mn);
      if (v == 1) continue;
      if (v == -1) {
        mn = p[i][j];
        cand.clear();
      }
      cand.push_back(game::Coord(i, j));
    }
  }

  for (auto p : cand) {
    if (OnCorner(p)) return p;
  }
  for (auto p : cand) {
    if (OnBorder(p)) return p;
  }
  std::random_device rd;
  std::mt19937 gen(rd());
  int idx = gen() % cand.size();
  return cand[idx];
}

void Reaper::DoNextStep() {
  auto pos = GetNextStep();
  bool ret = game_->Reveal(pos);
  if (!ret) {
    throw std::logic_error("Trying to reveal a mine cell.");
  }
}
