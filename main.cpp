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
Maze::MazeController* instance;
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
int Main(Maze::MazeController& mc, Menu::MenuController menu, bool firstRun);
int playEasy();
int playMedium();
int playHard();
int playCustom();
int saveButton();
int loadButton();
int ExitApp();

// Entry
int main(int /*argc*/, const char** /*argv[]*/) {
  bool firstRun = true;
  Maze::MazeController mc;
  instance = &mc;
  Menu::MenuController menu(title);
  menu.addOption(Menu::MenuController::Option("Easy", &playEasy, true, true));
  menu.addOption(Menu::MenuController::Option("Medium", &playMedium));
  menu.addOption(Menu::MenuController::Option("Hard", &playHard));
  menu.addOption(Menu::MenuController::Option("Custom", &playCustom));
  menu.addOption(Menu::MenuController::Option("Save Maze", &saveButton));
  menu.addOption(Menu::MenuController::Option("Load Maze", &loadButton));
  menu.addOption(Menu::MenuController::Option("Quit", &ExitApp));
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
int Main(Maze::MazeController& mc, Menu::MenuController menu, bool firstRun) {
  // Handle execution interrupt.
  struct sigaction sigIntHandler;
  sigIntHandler.sa_handler = interruptHandler;
  sigemptyset(&sigIntHandler.sa_mask);
  sigIntHandler.sa_flags = 0;
  sigaction(SIGINT, &sigIntHandler, NULL);

  bool loadLastSession = false;
  cout << flush;
  cin.clear();

  // Check if use has a previous unfinished maze.
  if (firstRun) {
    struct stat buf;
    if (stat(Maze::lastSessionFilename, &buf) != -1) {
      cout << yellow << "I found a previous session data. Would you like to "
                        "load it? (Y/n): "
           << reset;
      loadLastSession = Campbell::Strings::getYesNo(true);
    }
  }
  if (!loadLastSession) {
    menu.startMenu();
    return 1;
  } else {
    string option = Maze::lastSessionFilename;
    if (!mc.load(option.c_str())) {
      cout << red << "Failed to open file.\n" << reset;
      return 0;
    } else {
      mc.play();
    }
    loadLastSession = false;
    return 0;
  }
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
  ExitApp();
}
int playEasy() {
  instance->play(11, 11);
  return 1;
}
int playMedium() {
  instance->play(51, 51);
  return 1;
}
int playHard() {
  instance->play(101, 101);
  return 1;
}
int playCustom() {
  // TODO: Get user input for numbers here.
  int rows = 0, cols = 0;
  instance->play(rows, cols);
  return 1;
}
int saveButton() { return 0; }
int loadButton() { return 0; }
int ExitApp() {
  endwin();
  exit(0);
  return 0;
}
