#ifndef BOARD_H
#define BOARD_H

#include <vector>

namespace game {

class Coord {
  const int x, y;
  Coord(const int& x, const int& y);
  ~Coord();
};

class Game {
 private:
  class Board;

  Board* board;

 public:
  Game();
  ~Game();
  void Setup(const int& n, const int& m);
  bool Reveal(const Coord& pos);
  bool CheckWin() const;
};

class Game::Board {
 private:
  class Cell;
  using Map = std::vector<std::vector<Cell>>;

  const int n, m;
  Map* mp;
  int step_count, cell_left;

  void Laymine();

 public:
  Board(const int& n, const int& m);
  ~Board();
  bool Reveal(const Coord& pos);
  bool CheckWin() const;
};

class Game::Board::Cell {
 private:
  const int number;
  bool revealed;

 public:
  Cell(const int& number);
  ~Cell();
};

}  // namespace game

#endif
