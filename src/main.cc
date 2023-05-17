#include <stdlib.h>
#include <string.h>

#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdexcept>

#include "game/game.h"
#include "reaper/reaper.h"

const int kN = 8;
const int kM = 8;
const int kMine = 10;
const int kCases = 1000;

void ShowHelp(int code = 0) {
  // TODO: help page
  std::cout << "HELP PAGE" << std::endl;
  exit(code);
}

void ShowVersion() {
  std::ifstream ver("../VERSION");
  if (ver.fail()) {
    std::cerr << "No VERSION file exsits" << std::endl;
    exit(2);
  }
  std::string line;
  while (std::getline(ver, line)) {
    std::cout << line << std::endl;
  }
  exit(0);
}

void Test(int n, int m, int mine, int cases) {
  game::Game* game = new game::Game();
  reaper::Reaper* reaper;
  int win = 0;
  double ioe = 0;

  for (int i = 1; i <= cases; ++i) {
    std::cout << "CASE " << i << "/" << cases << ": ";
    game->Setup(n, m, mine);
    reaper = new reaper::Reaper(game);
    try {
      while (!game->CheckWin()) {
        reaper->DoNextStep();
      }
      auto x = game->CalcIoe();
      std::cout << "WON with IOE " << x << std::endl;
      ++win;
      ioe += x;
    } catch (std::logic_error) {
      std::cout << "FAILED" << std::endl;
    }
  }

  std::cout << std::setprecision(2);
  std::cout << "AVERAGE WINNING: " << 100.0 * win / cases << "%" << std::endl;
  std::cout << "AVERAGE IOE: " << ioe / win << std::endl;
  delete game;
  delete reaper;
}

int main(int argc, char** argv) {
  int n = kN, m = kM, mine = kMine, cases = kCases;
  int cnt = 0, args[3];

  try {
    for (int i = 1; i < argc; ++i) {
      if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--version") == 0) {
        ShowVersion();
      } else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
        ShowHelp();
      } else if (strcmp(argv[i], "-c") == 0 ||
                 strcmp(argv[i], "--cases") == 0) {
        cases = atoi(argv[++i]);
      } else {
        args[cnt++] = atoi(argv[i]);
      }
    }
    if (cnt == 3) {
      n = args[0];
      m = args[1];
      mine = args[2];
    }
    if (cnt != 0 && cnt != 3) ShowHelp(22);
    if (n <= 0 || m <= 0 || mine <= 0 || cases <= 0) ShowHelp(22);
  } catch (const std::exception& e) {
    ShowHelp(22);
  }

  Test(n, m, mine, cases);
  return 0;
}
