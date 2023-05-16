#include "game.h"

#include <algorithm>
#include <random>
#include <stdexcept>

using namespace game;

// implementation of Coord
Coord::Coord(const int& x, const int& y) : x(x), y(y) {}
Coord::~Coord() {}
// end implementation of Coord

// implementation of Game
Game::Game() {}
Game::~Game() { delete board_; }

int Game::row() const { return board_->row(); }
int Game::col() const { return board_->col(); }

void Game::Setup(const int& n, const int& m, const int& mine) {
  delete board_;
  board_ = new Board(n, m, mine);
}
bool Game::CheckWin() const { return board_->CheckWin(); }
double Game::CalcIoe() const { return board_->CalcIoe(); }

bool Game::Reveal(const Coord& pos) { return board_->Reveal(pos); }
Game::Cell Game::GetCell(const Coord& pos) const {
  return board_->GetCell(pos);
}
// end implementation of Game

// implementation of Game::Cell
Game::Cell::Cell(const int& number) : number(number), revealed(false) {}
Game::Cell::~Cell() {}
// end implementation of Game::Cell

// implementation of Game::Board
Game::Board::Board(const int& n, const int& m, const int& mine)
    : n_(n), m_(m), mine_(mine) {
  cells_left_ = n * m;
  cur_3bv_ = 0;
  step_count_ = 0;
  LayMines();
  Calc3bv();
}
Game::Board::~Board() {}

int Game::Board::row() const { return n_; }
int Game::Board::col() const { return m_; }
int Game::Board::cur_3bv() const { return cur_3bv_; }
int Game::Board::tot_3bv() const { return tot_3bv_; }
int Game::Board::step_count() const { return step_count_; }

template <typename T>
void Game::Board::Shuffle(std::vector<T>& vec) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::shuffle(vec.begin(), vec.end(), gen);
}

void Game::Board::LayMines() {
  auto encode = [this](int x, int y) -> int { return x * m_ + y; };

  std::vector<bool> is_mine(n_ * m_, false);
  std::fill_n(is_mine.begin(), mine_, true);
  Shuffle(is_mine);

  map_.clear();
  map_.resize(n_);
  for (int i = 0; i < n_; ++i) {
    auto& row = map_[i];
    row.reserve(m_);
    for (int j = 0; j < m_; ++j) {
      int cur = encode(i, j);
      if (is_mine[cur]) {
        row.push_back(Cell(9));
      } else {
        int cnt = 0;
        for (int k = 0; k < kAdjDirCount; ++k) {
          int dx = i + kAdjDir[k][0];
          int dy = j + kAdjDir[k][1];
          int loc = encode(dx, dy);
          if (is_mine[loc]) ++cnt;
        }
        row.push_back(Cell(cnt));
      }
    }
  }
}

void Game::Board::Calc3bv() const {
  // TODO: implement Calc3bv function
}

bool Game::Board::HasAdjOp(const Coord& pos) const {
  CheckCoord(pos);
  auto adj_cells = GetAdjCells(pos);
  for (const auto& cell : adj_cells) {
    if (cell.number == 0) return false;
  }
  return false;
}

bool Game::Board::IsValidCoord(const Coord& pos) const {
  return pos.x >= 0 && pos.x < n_ && pos.y >= 0 && pos.y < m_;
}

void Game::Board::CheckCoord(const Coord& pos) const {
  if (!IsValidCoord(pos)) {
    throw std::out_of_range("Requested coordinate not on board.");
  }
}

bool Game::Board::CheckWin() const { return cells_left_ == mine_; }
double Game::Board::CalcIoe() const { return (double)cur_3bv_ / step_count_; }

bool Game::Board::Reveal(const Coord& pos) {
  // TODO: implement cell revealing, step and 3bv accumulating
  return false;
}

Game::Cell Game::Board::GetCell(const Coord& pos) const {
  CheckCoord(pos);
  return map_[pos.x][pos.y];
}

std::vector<Game::Cell> Game::Board::GetAdjCells(const Coord& pos) const {
  CheckCoord(pos);
  std::vector<Cell> res;
  res.push_back(GetCell(pos));
  for (int i = 0; i < kAdjDirCount; ++i) {
    auto dir_pos = Coord(pos.x + kAdjDir[i][0], pos.y + kAdjDir[i][1]);
    if (IsValidCoord(dir_pos)) {
      res.push_back(GetCell(dir_pos));
    }
  }
  return res;
}
// end implementation of Game::Board
