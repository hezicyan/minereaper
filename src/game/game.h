#ifndef BOARD_H
#define BOARD_H

#include <functional>
#include <utility>
#include <vector>

namespace game {

const int kAdjDirCount = 4;
const int kAdjDir[kAdjDirCount][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};

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
  Map map_;
  int cells_left_;
  int cur_3bv_, tot_3bv_, step_count_;

  template <typename T>
  static void Shuffle(std::vector<T>& vec);
  void ExecuteConnected(const Coord& start_pos,
                        const std::function<void(Coord&)>& f);
  void LayMines();
  void Calc3bv();

  bool HasAdjOp(const Coord& pos) const;
  bool IsValidCoord(const Coord& pos) const;
  void CheckCoord(const Coord& pos) const;

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
  std::vector<Cell> GetAdjCells(const Coord& pos) const;
};

}  // namespace game

#endif
