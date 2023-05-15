#ifndef BOARD_H
#define BOARD_H

#include <utility>
#include <vector>

namespace game {

struct Coord {
 public:
  const int x, y;
  Coord(const int& x, const int& y);
  ~Coord();
};

class Game {
  // TODO: add info print functions

 private:
  class Board;
  Board* board_;

 public:
  struct Cell;

  Game();
  ~Game();

  int row() const;
  int col() const;

  void Setup(const int& n, const int& m, const int& mine);
  bool CheckWin() const;
  double CalcIoe() const;

  bool Reveal(const Coord& pos);
  Cell GetCell(const Coord& pos) const;
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
  Map* map_;
  int cells_left_;
  int cur_3bv_, tot_3bv_, step_count_;

  void LayMines();
  void Calc3bv() const;

 public:
  Board(const int& n, const int& m, const int& mine);
  ~Board();

  int row() const;
  int col() const;
  int cur_3bv() const;
  int tot_3bv() const;
  int step_count() const;

  bool CheckWin() const;
  double CalcIoe() const;

  bool Reveal(const Coord& pos);
  Cell GetCell(const Coord& pos) const;
};

}  // namespace game

#endif
