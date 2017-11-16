// MUST COMPILE WITH "-lncurses" flag!
// Example: "g++ -lncurses main.cpp"
#include <curses.h>
#include <signal.h>
#include <sys/stat.h>
#include <cstdlib>
#include <iostream>
#include "CampbellLib/CampbellLib.h"
#include "Maze.h"
#include "Menu.h"

using namespace std;
using namespace Campbell::Color;

// Global vars
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
void SaveMaze(Maze::MazeController& mc);
void interruptHandler(int s);
int Main(Maze::MazeController& mc, Menu::Menu menu, bool firstRun);

// Entry
int main(int /*argc*/, const char** /*argv[]*/) {
  cout << cyan << title;
  bool firstRun = true;
  Maze::MazeController mc;
  Menu::Menu menu(mc, title);
  try {
    while (Main(mc, menu, firstRun) == 0) firstRun = false;
  } catch (...) {
    endwin();
    cerr << "Exiting due to unknown failure.\n";
    return 1;
  }
  return 0;
}

// Main
int Main(Maze::MazeController& mc, Menu::Menu menu, bool firstRun) {
  bool loadLastSession = false;
  cout << flush;
  cin.clear();
  if (firstRun) {
    struct stat buf;
    if (stat(Maze::lastSessionFilename, &buf) != -1) {
      cout << yellow << "I found a previous session data. Would you like to "
                        "load it? (Y/n): "
           << reset;
      loadLastSession = Campbell::Strings::getYesNo(true);
    }
  }
  string option = "";
  int selection = 0;
  if (!loadLastSession) {
    menu.startMenu();
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
    option = Maze::lastSessionFilename;
  }
  int rows = -1;
  int cols = -1;
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
        cout << "Enter number of rows: ";
        getline(cin, option);
        if (!Campbell::Strings::isNumber(option.c_str())) return 0;
        rows = Campbell::Strings::toNumber(option.c_str());
        cout << "Enter number of columns: ";
        getline(cin, option);
        if (!Campbell::Strings::isNumber(option.c_str())) return 0;
        cols = Campbell::Strings::toNumber(option.c_str());
        break;
    case 5:
      SaveMaze(mc);
      return 0;
      break;
    case 6:
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
      return 1;
  }

  // Handle execution interrupt.
  struct sigaction sigIntHandler;
  sigIntHandler.sa_handler = interruptHandler;
  sigemptyset(&sigIntHandler.sa_mask);
  sigIntHandler.sa_flags = 0;
  sigaction(SIGINT, &sigIntHandler, NULL);

  // Play Game
  mc.playGame(rows, cols);

  return 0;
}

// Definitions
void SaveMaze(Maze::MazeController& mc) {
  bool tryAgain = true;
  do {
    cout << yellow << "Would you like to save the game? (Y/n): " << reset;
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
        cout << red << "Game failed to save.\n" << reset;
      }
    } else {
      cout << red << "Not saving.\n" << reset;
      tryAgain = false;
    }
  } while (tryAgain);
}
// Catches Ctrl+C in order to call ExitApp properly.
void interruptHandler(int s) {
  (void)s;
  endwin();
  exit(0);
}
