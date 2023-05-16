#include <stdlib.h>
#include <string.h>

#include <iostream>

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
  // TODO: version page
  std::cout << "VERSION" << std::endl;
  exit(0);
}

int main(int argc, char** argv) {
  std::cerr << "START RUNNING" << std::endl;

  int n = kN, m = kM, mine = kMine, cases = kCases;
  int cnt = 0, args[3];

  try {
    for (int i = 1; i < argc; ++i) {
      std::cout << "PARSING: " << argv[i] << std::endl;
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

  std::cout << n << ' ' << m << ' ' << mine << ' ' << cases << std::endl;
  return 0;
}
