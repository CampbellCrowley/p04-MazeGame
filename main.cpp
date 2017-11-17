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
Menu::MenuController::Option* saveOption;
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
int playEasy();
int playMedium();
int playHard();
int playCustom();
int saveButton();
int loadButton();
int ExitApp();

// Entry
int main(int /*argc*/, const char** /*argv[]*/) {
  Maze::MazeController mc;
  instance = &mc;
  Menu::MenuController menu(title);

  menu.addOption(Menu::MenuController::Option("Easy", &playEasy, true, true));
  menu.addOption(Menu::MenuController::Option("Medium", &playMedium));
  menu.addOption(Menu::MenuController::Option("Hard", &playHard));
  menu.addOption(Menu::MenuController::Option("Custom", &playCustom));
  saveOption = menu.addOption(
      Menu::MenuController::Option("Save Maze", &saveButton, false));
  menu.addOption(Menu::MenuController::Option("Load Maze", &loadButton));
  menu.addOption(Menu::MenuController::Option("Quit", &ExitApp));

  // Handle execution interrupt.
  struct sigaction sigIntHandler;
  sigIntHandler.sa_handler = interruptHandler;
  sigemptyset(&sigIntHandler.sa_mask);
  sigIntHandler.sa_flags = 0;
  sigaction(SIGINT, &sigIntHandler, NULL);

  {
    struct stat buf;
    if (stat(Maze::lastSessionFilename, &buf) != -1) {
      cout << yellow << "I found a previous session data. Would you like to "
                        "load it? (Y/n): "
           << reset;
      if (Campbell::Strings::getYesNo(true)) {
        string option = Maze::lastSessionFilename;
        if (!mc.load(option.c_str())) {
          cout << red << "Failed to open file.\n" << reset;
          return 1;
        } else {
          mc.play();
          saveOption->isSelectable = instance->width() > 0;
        }
      }
    }
  }

  try {
    menu.startMenu();
  } catch (...) {
    endwin();
    cerr << "Exiting due to unknown failure.\n";
    return 1;
  }
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
  ExitApp();
}
int playEasy() {
  instance->play(11, 11);
  saveOption->isSelectable = instance->width() > 0;
  return 1;
}
int playMedium() {
  instance->play(51, 51);
  saveOption->isSelectable = instance->width() > 0;
  return 1;
}
int playHard() {
  instance->play(101, 101);
  saveOption->isSelectable = instance->width() > 0;
  return 1;
}
// TODO: Make curses menu for choosing size.
int playCustom() {
  int rows = 0, cols = 0;
  string input;
  cout << "Enter number of rows: ";
  getline (cin, input);
  rows = Campbell::Strings::toNumber(input.c_str());
  cout << "Enter number of columns: ";
  getline (cin, input);
  cols = Campbell::Strings::toNumber(input.c_str());
  instance->play(rows, cols);
  saveOption->isSelectable = instance->width() > 0;
  return 1;
}
// TODO: Get user input for save and load locations.
int saveButton() { return instance->save("myfavoritemaze.dat"); }
int loadButton() {
  if (instance->load("myfavoritemaze.dat")) {
    instance->play();
    saveOption->isSelectable = instance->width() > 0;
    return 1;
  } else {
    return 0;
  }
}
int ExitApp() {
  endwin();
  exit(0);
  return 0;
}
