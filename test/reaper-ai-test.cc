#include <iostream>

#include "../src/game/game.h"
#include "../src/reaper/reaper.h"

using namespace game;
using namespace reaper;

const int kN = 8;
const int kM = 8;
const int kMine = 10;

int main(int argc, char** argv) {
  std::cout << "START TESTING" << std::endl;

  auto game = new Game();
  int n = (argc == 1 ? kN : atoi(argv[1]));
  int m = (argc == 1 ? kM : atoi(argv[2]));
  int mine = (argc == 1 ? kMine : atoi(argv[3]));
  game->Setup(n, m, mine);
  try {
    while (!game->CheckWin()) {
      game->PrintBoard();
      auto r = new Reaper(game);
      r->Print();
      Coord pos = r->GetNextStep();
      std::cout << "REVEAL: " << pos.x << " " << pos.y << std::endl;
      r->DoNextStep();
    }
    std::cout << "SUCCESS " << game->CalcIoe() << std::endl;
  } catch (std::logic_error) {
    std::cout << "BOMB!" << std::endl;
  }

  delete game;
  return 0;
}
