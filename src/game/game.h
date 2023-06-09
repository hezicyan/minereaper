#ifndef BOARD_H
#define BOARD_H

#include <functional>
#include <iostream>
#include <utility>
#include <vector>

namespace game {

using Dirs = std::vector<std::pair<int, int>>;
const Dirs kAdjDirs = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1},
                       {0, 1},   {1, -1}, {1, 0},  {1, 1}};

struct Coord {
 public:
  const int x, y;
  Coord(const int& x, const int& y);
  ~Coord();
};

class Game {
 private:
  class Board;
  Board* board_;

 public:
  struct Cell;

  Game();
  ~Game();

  int row() const;
  int col() const;
  int mine_count() const;

  void Setup(const int& n, const int& m, const int& mine);
  bool CheckWin() const;
  double CalcIoe() const;

  bool Reveal(const Coord& pos);
  Cell GetCell(const Coord& pos) const;

  void PrintBoard(std::ostream& out = std::cerr) const;
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
  int cur_3bv_, tot_3bv_, step_count_;

  template <typename T>
  static void Shuffle(std::vector<T>& vec);
  void ExecuteConnected(const Coord& start_pos, const Dirs& dirs,
                        const std::function<bool(const Coord&)>& f);
  void LayMines();
  void Calc3bv();

  bool HasOpAround(const Coord& pos) const;
  bool IsValidCoord(const Coord& pos) const;
  void CheckCoord(const Coord& pos) const;

 public:
  Board(const int& n, const int& m, const int& mine);
  ~Board();

  int row() const;
  int col() const;
  int mine_count() const;
  int cur_3bv() const;
  int tot_3bv() const;
  int step_count() const;

  bool CheckWin() const;
  double CalcIoe() const;

  bool Reveal(const Coord& pos);
  Cell GetCell(const Coord& pos) const;
  std::vector<Cell> GetCellsAround(const Coord& pos) const;

  void PrintBoard(std::ostream& out = std::cerr) const;
};

}  // namespace game

#endif
