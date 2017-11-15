// MUST COMPILE WITH "-lncurses" flag!
// Example: "g++ -lncurses main.cpp"
#include <curses.h>
#include <signal.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cstdlib>
#include <iostream>
#include "CampbellLib/CampbellLib.h"
#include "Maze.h"

using namespace std;
using namespace Campbell::Color;

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
const char* completeTitle = // Maze Complete!
    "                                    :::   :::       :::     ::::::::: "
    "::::::::::                     \n                                  :+:+: "
    ":+:+:    :+: :+:        :+:  :+:                             \n           "
    "                     +:+ +:+:+ +:+  +:+   +:+      +:+   +:+              "
    "                \n                               +#+  +:+  +#+ "
    "+#++:++#++:    +#+    +#++:++#                          \n                "
    "              +#+       +#+ +#+     +#+   +#+     +#+                     "
    "           \n                             #+#       #+# #+#     #+#  #+#  "
    "    #+#                                 \n                            ### "
    "      ### ###     ### ######### ##########                           \n   "
    "   ::::::::   ::::::::    :::   :::   :::::::::  :::        :::::::::: "
    "::::::::::: :::::::::: ::: \n    :+:    :+: :+:    :+:  :+:+: :+:+:  :+:  "
    "  :+: :+:        :+:            :+:     :+:        :+:  \n   +:+        "
    "+:+    +:+ +:+ +:+:+ +:+ +:+    +:+ +:+        +:+            +:+     +:+ "
    "       +:+   \n  +#+        +#+    +:+ +#+  +:+  +#+ +#++:++#+  +#+       "
    " +#++:++#       +#+     +#++:++#   +#+    \n +#+        +#+    +#+ +#+    "
    "   +#+ +#+        +#+        +#+            +#+     +#+        +#+     "
    "\n#+#    #+# #+#    #+# #+#       #+# #+#        #+#        #+#           "
    " #+#     #+#                 \n########   ########  ###       ### ###     "
    "   ########## ##########     ###     ########## ###       \n";

// Prototypes
Direction getMoveDirection();
void ExitApp();
void SaveMaze();
void ExitMaze();
void interruptHandler(int s);
int Main();

// Entry
int main(int /*argc*/, const char** /*argv[]*/) {
  cout << cyan << title;
  while(Main() == 0) {}
  return 0;
}

// Main
int Main() {
  struct stat buf;
  bool loadLastSession = false;
  cout << flush;
  cin.clear();
  if (mc.width() == 0) {
    if (stat("lastsession.dat", &buf) != -1) {
      cout << yellow << "I found a previous session data. Would you like to "
                        "load it? (Y/n): "
           << reset;
      loadLastSession = Campbell::Strings::getYesNo(true);
    }
  }
  string option = "";
  int selection = 0;
  if (!loadLastSession) {
    do {
      cout << yellow << "\n\nControls:\n";
      cout << "UP:      \u2191 or K\n";
      cout << "DOWN:    \u2193 or J\n";
      cout << "LEFT:    \u2191 or H\n";
      cout << "RIGHT:   \u2192 or L\n";
      cout << "HINT:    \"\n";
      cout << "GIVE UP: ?\n";
      cout << "QUIT:    Q\n" << reset;
      cout << "Please select an option:\n";
      cout << "1) Easy/Small\n";
      cout << "2) Medium\n";
      cout << "3) Hard/Large\n";
      cout << "4) Custom size\n";
      if (mc.width() == 0) cout << red;
      cout << "5) Save maze\n";
      if (mc.width() == 0) cout << reset;
      cout << "6) Load maze\n";
      cout << "7) Quit Game\n";
      cout << flush;
      cin.clear();
      getline(cin, option);
      if (option[0] == 'q' || option[0] == 'Q') {
        selection = 7;
        break;
      }
      selection = Campbell::Strings::toNumber(option.c_str());
    } while (!Campbell::Strings::isNumber(option.c_str()) || selection < 1 ||
             (mc.width() == 0 && selection == 5) || selection > 7);
  } else {
    selection = 6;
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
      SaveMaze();
      return 0;
      break;
    case 6:
      generateMaze = false;
      if (!loadLastSession) {
        cout << "Enter a filename to load: ";
        getline(cin, option);
      }
      if (!mc.import(option.c_str())) {
        cout << red << "Failed to open file.\n" << reset;
        return 0;
      } else {
        break;
      }
      loadLastSession = false;
      break;
    case 7:
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
  init_pair(10, COLOR_GREEN, COLOR_BLACK);
  init_pair(MazeController::TileSymbolsColor::EMPTY, COLOR_BLACK, COLOR_BLACK);
  init_pair(MazeController::TileSymbolsColor::WALL, COLOR_RED, COLOR_RED);
  init_pair(MazeController::TileSymbolsColor::START, COLOR_GREEN, COLOR_GREEN);
  init_pair(MazeController::TileSymbolsColor::END, COLOR_YELLOW, COLOR_YELLOW);
  init_pair(MazeController::TileSymbolsColor::CURRENT, COLOR_GREEN, COLOR_GREEN);
  init_pair(MazeController::TileSymbolsColor::PREVIOUS, COLOR_BLUE, COLOR_BLUE);
  init_pair(MazeController::TileSymbolsColor::UNKNOWN, COLOR_RED, COLOR_BLACK);
  init_pair(MazeController::TileSymbolsColor::HINT, COLOR_WHITE, COLOR_WHITE);

  // Get starting screensize
  getmaxyx(stdscr, height, width);
  clear();
  mc.print(width, height);

  clear();
  if (generateMaze) mc.generate(rows, cols);

  // Play game
  Direction nextDirection = NONE;
  getmaxyx(stdscr, height, width);
  clear();
  mc.print(width, height);
  int lastWidth = width;
  int lastHeight = height;
  while (nextDirection != EXIT) {
    nextDirection = getMoveDirection();
    getmaxyx(stdscr, height, width);
    if (mc.move(nextDirection)) {
      mc.print(width, height);
    } else if (lastWidth != width || lastHeight != height) {
      mc.print(width, height);
    }
    if (mc.isComplete()) {
      mc.move(SOLVE);
      mc.print(width, height);
      usleep(100000);
      wattron(stdscr, COLOR_PAIR(10));
      move(height / 2 - 10, 0);
      addstr(completeTitle);
      addstr("Press 'Q' to quit\n");
      wattroff(stdscr, COLOR_PAIR(10));
      refresh();
      while (getMoveDirection() == NONE) {}
      break;
    }
    lastWidth = width;
    lastHeight = height;
  }

  ExitMaze();
  if (mc.isComplete()) {
    cout << green << completeTitle << reset;
  }
  return 0;
}

// Definitions
// Gets user input and converts that to a direction.
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
    case '\'':
    case '\"':
      return HELP;
    case '?':
    case '/':
      return SOLVE;
    default:
      return NONE;
  }
}
// Called before app exits in order to properly release terminal from NCurses.
void ExitApp() {
  endwin();
  exit(0);
}
void SaveMaze() {
  bool tryAgain = true;
  do {
    cout << yellow
         << "Would you like to save the game? (Y/n): "
         << reset;
    if (Campbell::Strings::getYesNo(true)) {
      cout << yellow
           << "Please enter the name of the file you would like to save to: "
           << reset;
      string filename;
      getline(cin, filename);
      cout << green << "Saving game...\r" << reset << flush;
      if (mc.save(filename.c_str())) {
        cout << green << "Game successfull saved.\n" << reset;
        tryAgain = false;
      } else {
        cout << red << "Game failed to saved.\n" << reset;
      }
    } else {
      cout << red << "Not saving.\n" << reset;
      tryAgain = false;
    }
  } while (tryAgain);
}
void ExitMaze() {
  if (!mc.isComplete()) mc.save("lastsession.dat");
  else remove("lastsession.dat");
  erase();
  clear();
  endwin();
}
// Catches Ctrl+C in order to call ExitApp properly.
void interruptHandler(int s) {
  (void)s;
  ExitApp();
}
