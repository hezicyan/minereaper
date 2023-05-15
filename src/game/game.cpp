#include "game.h"

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
  benchmark_value_ = 0;
  step_count_ = 0;
}
bool Game::CheckWin() const { return board_->CheckWin(); }
double Game::CalcIoe() const { return (double)benchmark_value_ / step_count_; }

bool Game::Reveal(const Coord& pos) {
  // TODO: calculate benchmark value
  ++step_count_;
  return board_->Reveal(pos);
}

Game::Cell Game::GetCell(const Coord& pos) const {
  return board_->GetCell(pos);
}
// end implementation of Game

// implementation of Game::Cell
Game::Cell::Cell(const int& number) : number(number), revealed(false) {}
Game::Cell::~Cell() {}
// end implementation of Game::Cell

// implementation of Game::Board
// TODO: implement Board
// end implementation of Game::Board
