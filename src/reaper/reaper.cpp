#include "reaper.h"

#include "../game/game.h"

using namespace reaper;

Reaper::Reaper(game::Game* game) : game_(game) {
  n_ = game_->row();
  m_ = game_->col();
}
int Sign(double x) {
  if (x > kEps) return 1;
  if (x < -kEps) return -1;
  return 0;
}

Reaper::Reaper(game::Game* game) : game_(game) {}

Reaper::~Reaper() {}

bool Reaper::IsValidCoord(const game::Coord& pos) {
  return pos.x >= 0 && pos.x < n_ && pos.y >= 0 && pos.y < n_;
}
int blocksize;
void Reaper::GetBlock(int x, int y) {
  if (vis_[x][y]) return;
  vis_[x][y] = true;
  auto now = game_->GetCell(game::Coord(x, y));
  if (!now.revealed) {
    borders_.push_back(game::Coord(x, y));
    return;
  }
  if (now.number != 0) blocksize++;
  for (auto p : game::kAdjDirs) {
    int xx = x + p.first;
    int yy = y + p.second;
    if (IsValidCoord(game::Coord(xx, yy))) {
      GetBlock(xx, yy);
    }
  }
}
int sum;
void Reaper::Dfs(int t) {
  if (t >= borders_.size()) {
    if (sum != blocksize) return;
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
      if (now.revealed and now.number != 9) {
        cnt_[xx][yy]++;
        if (cnt_[xx][yy] == now.number) sum++;
        if (cnt_[xx][yy] > now.number) valid = false;
      }
    }
  }
  if (valid) Dfs(t + 1);

  for (auto p : game::kAdjDirs) {
    int xx = x + p.first;
    int yy = y + p.second;
    auto pos = game::Coord(xx, yy);
    if (IsValidCoord(pos)) {
      auto now = game_->GetCell(pos);
      if (now.revealed and now.number != 9) {
        if (cnt_[xx][yy] == now.number) sum--;
        cnt_[xx][yy]--;
      }
    }
  }

  mines_.pop_back();
  Dfs(t + 1);
}

std::vector<std::vector<double>> Reaper::GetPossibility() {
  mines_.clear();
  vis_ = std::vector<std::vector<bool>>(n_, std::vector<bool>(m_, false));
  // std::cerr << "--------------------------------" << std::endl;
  std::vector<std::vector<double>> res(n_, std::vector<double>(m_, 0));
  for (int i = 0; i < n_; i++) {
    for (int j = 0; j < m_; j++) {
      if (vis_[i][j]) continue;
      auto now = game_->GetCell(game::Coord(i, j));
      if (now.revealed && now.number != 9) {
        // std::cout << i << " " << j << std::endl;
        tot_situations_ = 0;
        blocksize = 0;
        sum = 0;
        blocks_.clear();
        borders_.clear();
        cnt_ = std::vector<std::vector<int>>(n_, std::vector<int>(m_, 0));
        tot_ = std::vector<std::vector<int>>(n_, std::vector<int>(m_, 0));
        GetBlock(i, j);
        Dfs(0);
        std::cout << borders_.size() << std::endl;
        std::cout << tot_situations_ << std::endl;
        std::cout << blocksize << std::endl;
        for (auto u : borders_) {
          std::cout << tot_[u.x][u.y] << std::endl;
          res[u.x][u.y] = (double)tot_[u.x][u.y] / tot_situations_;
        }
      }
    }
  }
  return res;
}

void Reaper::Print() {
  auto p = GetPossibility();
  int n = game_->row();
  int m = game_->col();
  for (int i = 0; i < n; i++) {
  for (int j = 0; j < m; j++) {
    std::cerr << p[i][j] << " ";
  }
  std::cerr << std::endl;
  } 
    
  return;
}
