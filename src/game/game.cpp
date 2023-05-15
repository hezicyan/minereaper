#include "game.h"

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
Game::Board::~Board() { delete map_; }

int Game::Board::row() const { return n_; }
int Game::Board::col() const { return m_; }
int Game::Board::cur_3bv() const { return cur_3bv_; }
int Game::Board::tot_3bv() const { return tot_3bv_; }
int Game::Board::step_count() const { return step_count_; }

void Game::Board::LayMines() {
  // TODO: implement LayMines function
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
  return (*map_)[pos.x][pos.y];
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
