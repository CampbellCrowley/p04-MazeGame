// MUST COMPILE WITH "-lncurses" flag!
// Example: "g++ -lncurses main.cpp"
#include <curses.h>
#include <signal.h>
#include <cstdlib>
#include <iostream>
#include "CampbellLib/CampbellLib.h"
#include "Maze.h"

using namespace std;

// Global vars
int width = 0;
int height = 0;
MazeController mc;
const char* title =
"        :::   :::       :::     ::::::::: ::::::::::                       \n"
"      :+:+: :+:+:    :+: :+:        :+:  :+:                               \n"
"    +:+ +:+:+ +:+  +:+   +:+      +:+   +:+                                \n"
"   +#+  +:+  +#+ +#++:++#++:    +#+    +#++:++#                            \n"
"  +#+       +#+ +#+     +#+   +#+     +#+                                  \n"
" #+#       #+# #+#     #+#  #+#      #+#                                   \n"
"###       ### ###     ### ######### ##########                             \n"
"        :::   :::       :::      :::::::: ::::::::::: :::::::::: ::::::::: \n"
"      :+:+: :+:+:    :+: :+:   :+:    :+:    :+:     :+:        :+:    :+: \n"
"    +:+ +:+:+ +:+  +:+   +:+  +:+           +:+     +:+        +:+    +:+  \n"
"   +#+  +:+  +#+ +#++:++#++: +#++:++#++    +#+     +#++:++#   +#++:++#:    \n"
"  +#+       +#+ +#+     +#+        +#+    +#+     +#+        +#+    +#+    \n"
" #+#       #+# #+#     #+# #+#    #+#    #+#     #+#        #+#    #+#     \n"
"###       ### ###     ###  ########     ###     ########## ###    ###      \n";

// Prototypes
Direction getMoveDirection();

// Definitions
// Called before app exits in order to properly release terminal from NCurses.
void ExitApp() {
  endwin();
  exit(0);
}
// Catches Ctrl+C in order to call ExitApp properly.
void interruptHandler(int s) {
  (void)s;
  ExitApp();
}
// Entry
int main() {
  cout << title;
  cout << "Controls:\n";
  cout << "UP: \u2191 or K\n";
  cout << "DOWN: \u2193 or J\n";
  cout << "LEFT: \u2191 or H\n";
  cout << "RIGHT: \u2192 or L\n";
  cout << "QUIT: Q\n";
  string option = "";
  int selection = 0;
  do {
    cout << "Please select an option:\n";
    cout << "1) Easy/Small\n";
    cout << "2) Medium\n";
    cout << "3) Hard/Large\n";
    cout << "4) Custom size\n";
    cout << "5) Load File\n";
    getline(cin, option);
    selection = Campbell::Strings::toNumber(option.c_str());
  } while (!Campbell::Strings::isNumber(option.c_str()) || selection < 1 ||
           selection > 5);
  bool generateMaze = true;
  int rows = 0;
  int cols = 0;
  switch (selection) {
    case 1:
      rows = 11;
      cols = 11;
      break;
    case 2:
      rows = 51;
      cols = 51;
      break;
    case 3:
      rows = 101;
      cols = 101;
      break;
    default:
    case 4:
      // TODO: Restrict sizes to larger than a value.
      do {
        cout << "Enter number of rows: ";
        getline(cin, option);
      } while (!Campbell::Strings::isNumber(option.c_str()));
      rows = Campbell::Strings::toNumber(option.c_str());
      do {
        cout << "Enter number of columns: ";
        getline(cin, option);
      } while (!Campbell::Strings::isNumber(option.c_str()));
      cols = Campbell::Strings::toNumber(option.c_str());
      break;
    case 5:
      generateMaze = false;
      string filename = "";
      // TODO: Give better feedback.
      do {
        cout << "Enter a filename to load: ";
        getline(cin, filename);
      } while (!mc.import(filename.c_str()));
      break;
  }

  // Handle execution interrupt.
  struct sigaction sigIntHandler;
  sigIntHandler.sa_handler = interruptHandler;
  sigemptyset(&sigIntHandler.sa_mask);
  sigIntHandler.sa_flags = 0;
  sigaction(SIGINT, &sigIntHandler, NULL);

  // Handle NCurses.
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  // Colors
  start_color();
  init_pair(MazeController::TileSymbolsColor::EMPTY, COLOR_BLACK, COLOR_BLACK);
  init_pair(MazeController::TileSymbolsColor::WALL, COLOR_WHITE, COLOR_WHITE);
  init_pair(MazeController::TileSymbolsColor::START, COLOR_GREEN, COLOR_GREEN);
  init_pair(MazeController::TileSymbolsColor::END, COLOR_RED, COLOR_RED);
  init_pair(MazeController::TileSymbolsColor::CURRENT, COLOR_MAGENTA, COLOR_MAGENTA);
  init_pair(MazeController::TileSymbolsColor::PREVIOUS, COLOR_BLUE, COLOR_BLUE);
  init_pair(MazeController::TileSymbolsColor::UNKNOWN, COLOR_RED, COLOR_BLACK);

  // Get starting screensize
  getmaxyx(stdscr, height, width);
  mc.print(width, height);

  if (generateMaze) mc.generate(rows, cols);

  // Play game
  Direction nextDirection = NONE;
  getmaxyx(stdscr, height, width);
  mc.print(width, height);
  int lastWidth = width;
  int lastHeight = height;
  while (!mc.isComplete() && nextDirection != EXIT) {
    nextDirection = getMoveDirection();
    getmaxyx(stdscr, height, width);
    if (nextDirection != NONE || lastWidth != width || lastHeight != height) {
      mc.move(nextDirection);
      mc.print(width, height);
    }
    lastWidth = width;
    lastHeight = height;
  }

  ExitApp();
  return 0;
}  // int main()

Direction getMoveDirection() {
  wchar_t input = getch();
  switch (input) {
    case KEY_DOWN:
    case 'J':
    case 'j':
      return DOWN;
    case KEY_UP:
    case 'K':
    case 'k':
      return UP;
    case KEY_LEFT:
    case 'H':
    case 'h':
      return LEFT;
    case KEY_RIGHT:
    case 'L':
    case 'l':
      return RIGHT;
    case 'Q':
    case 'q':
      return EXIT;
    default:
      return NONE;
  }
}
