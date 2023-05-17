#include "../src/game/game.h"
#include "../src/reaper/reaper.h"

#include <istream>

using namespace game;
using namespace reaper;

const int kN = 8;
const int kM = 8;
const int kMine = 10;

int ToInt(char* s) {
  int res = 0;
  while (*s != '\0') {
    res = res * 10 + *(s++) - '0';
  }
  return res;
}

int main(int argc, char** argv) {
  std::cout << "START TESTING" << std::endl;
  auto game = new Game();
  int n = (argc == 1 ? kN : ToInt(argv[1]));
  int m = (argc == 1 ? kM : ToInt(argv[2]));
  int mine = (argc == 1 ? kMine : ToInt(argv[3]));
  game->Setup(n, m, mine);
  while (!game->CheckWin()) {
    game->PrintBoard();
    auto r = new Reaper(game);
    int x, y;
    std::cin >> x >> y;
    bool ret = game->Reveal(Coord(x, y));
    if (!ret) {
      std::cerr << "BOMB!" << std::endl;
      break;
    }
    r->Print();
  }

  delete game;
  return 0;
}
