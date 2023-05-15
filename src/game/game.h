#ifndef BOARD_H
#define BOARD_H

#include <vector>

namespace game {

struct Coord {
 public:
  const int x, y;
  Coord(const int& x, const int& y);
  ~Coord();
};

class Game {
 private:
  class Board;
  struct Cell;

  Board* board_;
  int benchmark_value_, step_count_;

 public:
  Game();
  ~Game();
  void Setup(const int& n, const int& m, const int& mine);
  bool Reveal(const Coord& pos);
  bool CheckWin() const;

  int row();
  int col();

  int CalcIoe();
  Cell GetCell(const Coord& pos);
};

struct Game::Cell {
 public:
  const int number;
  bool revealed;

  Cell(const int& number);
  ~Cell();
};

class Game::Board {
 private:
  using Map = std::vector<std::vector<Cell>>;

  const int n_, m_, mine_;
  Map* mp_;
  int cell_left_;

  void Laymine();

 public:
  Board(const int& n, const int& m, const int& mine);
  ~Board();
  bool Reveal(const Coord& pos);
  bool CheckWin() const;
};

}  // namespace game

#endif
