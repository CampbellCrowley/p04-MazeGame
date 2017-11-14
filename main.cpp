// MUST COMPILE WITH "-lncurses" flag!
// Example: "g++ -lncurses main.cpp"
#include <curses.h>
#include <signal.h>
#include <sys/stat.h>
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
void ExitApp(bool force = false) {
  endwin();
  if (!force) {
    mc.save("lastsession.dat");
    bool tryAgain = true;
    do {
      cout << Campbell::Color::yellow
           << "Would you like to save the game? (Y/n): "
           << Campbell::Color::reset;
      if (Campbell::Strings::getYesNo(true)) {
        cout << Campbell::Color::yellow
             << "Please enter the name of the file you would like to save to: "
             << Campbell::Color::reset;
        string filename;
        getline(cin, filename);
        cout << Campbell::Color::green << "Saving game...\r"
             << Campbell::Color::reset << flush;
        if (mc.save(filename.c_str())) {
          cout << Campbell::Color::green << "Game successfull saved.\n"
               << Campbell::Color::reset;
          tryAgain = false;
        } else {
          cout << Campbell::Color::red << "Game failed to saved.\n"
               << Campbell::Color::reset;
        }
      } else {
        cout << Campbell::Color::red << "Not saving.\n"
             << Campbell::Color::reset;
        tryAgain = false;
      }
    } while (tryAgain);
  }
  exit(0);
}
// Catches Ctrl+C in order to call ExitApp properly.
void interruptHandler(int s) {
  (void)s;
  ExitApp(true);
}
// Entry
int main() {
  cout << Campbell::Color::cyan << title;
  cout << Campbell::Color::yellow << "Controls:\n";
  cout << "UP: \u2191 or K\n";
  cout << "DOWN: \u2193 or J\n";
  cout << "LEFT: \u2191 or H\n";
  cout << "RIGHT: \u2192 or L\n";
  cout << "QUIT: Q\n" << Campbell::Color::reset;
  struct stat buf;
  bool loadLastSession = false;
  if (stat("lastsession.dat", &buf) != -1) {
    cout
        << Campbell::Color::yellow
        << "I found a previous session data. Would you like to load it? (Y/n): "
        << Campbell::Color::reset;
    loadLastSession = Campbell::Strings::getYesNo(true);
  }
  string option = "";
  int selection = 0;
  if (!loadLastSession) {
    do {
      cout << "Please select an option:\n";
      cout << "1) Easy/Small\n";
      cout << "2) Medium\n";
      cout << "3) Hard/Large\n";
      cout << "4) Custom size\n";
      cout << "5) Load File\n";
      cout << "6) Quit Game\n";
      getline(cin, option);
      selection = Campbell::Strings::toNumber(option.c_str());
    } while (!Campbell::Strings::isNumber(option.c_str()) || selection < 1 ||
             selection > 6);
  } else {
    selection = 5;
    option = "lastsession.dat";
  }
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
      while (true) {
        if (!loadLastSession) {
          cout << "Enter a filename to load: ";
          getline(cin, option);
        }
        if (!mc.import (option.c_str())) {
          cout << Campbell::Color::red << "Failed to open file.\n"
               << Campbell::Color::reset;
        } else {
          break;
        }
        loadLastSession = false;
      }
      break;
    case 6:
      ExitApp();
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

  remove("lastsession.dat");
  ExitApp(mc.isComplete());
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
