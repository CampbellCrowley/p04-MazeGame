// MUST COMPILE WITH "-lncurses" flag!
// Example: "g++ -lncurses main.cpp"
#include <curses.h>
#include <dirent.h>
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
int saveOptionIndex = 0;
Menu::MenuController* loadMenu_;
Menu::MenuController* menu_;
const char* savesDir = "./saves/";
const char* title = // Maze Master
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
const char* loadTitle =  // Load Maze
    "      :::        ::::::::      :::     :::::::::            :::   :::     "
    "  :::     ::::::::: :::::::::: \n     :+:       :+:    :+:   :+: :+:   "
    ":+:    :+:          :+:+: :+:+:    :+: :+:        :+:  :+:         \n    "
    "+:+       +:+    +:+  +:+   +:+  +:+    +:+         +:+ +:+:+ +:+  +:+   "
    "+:+      +:+   +:+          \n   +#+       +#+    +:+ +#++:++#++: +#+    "
    "+:+         +#+  +:+  +#+ +#++:++#++:    +#+    +#++:++#      \n  +#+     "
    "  +#+    +#+ +#+     +#+ +#+    +#+         +#+       +#+ +#+     +#+   "
    "+#+     +#+            \n #+#       #+#    #+# #+#     #+# #+#    #+#     "
    "    #+#       #+# #+#     #+#  #+#      #+#             \n########## "
    "########  ###     ### #########          ###       ### ###     ### "
    "######### ##########       \n";

// Prototypes
void SaveMaze(Maze::MazeController& mc);
void interruptHandler(int s);
int playEasy();
int playMedium();
int playHard();
int playCustom();
int saveButton();
int loadButton();
int loadMaze();
int closeSubMenu();
int ExitApp();
int nothing() { return 0; }

// Entry
int main(int /*argc*/, const char** /*argv[]*/) {
  Maze::MazeController mc;
  Menu::MenuController menu(title);
  Menu::MenuController loadMenu(loadTitle);
  instance = &mc;
  menu_ = &menu;
  loadMenu_ = &loadMenu;

  menu.addOption(Menu::MenuController::Option("Easy", &playEasy, true, true));
  menu.addOption(Menu::MenuController::Option("Medium", &playMedium));
  menu.addOption(Menu::MenuController::Option("Hard", &playHard));
  menu.addOption(Menu::MenuController::Option("Custom", &playCustom));
  menu.addOption(
      Menu::MenuController::Option("----------", &nothing, false, false));
  saveOptionIndex = menu.getOptionList().size();
  menu.addOption(Menu::MenuController::Option("Save Maze", &saveButton, false));
  menu.addOption(Menu::MenuController::Option("Load Maze", &loadButton));
  menu.addOption(
      Menu::MenuController::Option("----------", &nothing, false, false));
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
          menu.getOptionList()[saveOptionIndex].isSelectable =
              instance->width() > 0;
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
      if (mc.save((savesDir + filename).c_str())) {
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
  menu_->getOptionList()[saveOptionIndex].isSelectable = instance->width() > 0;
  return 1;
}
int playMedium() {
  instance->play(51, 51);
  menu_->getOptionList()[saveOptionIndex].isSelectable = instance->width() > 0;
  return 1;
}
int playHard() {
  instance->play(101, 101);
  menu_->getOptionList()[saveOptionIndex].isSelectable = instance->width() > 0;
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
  menu_->getOptionList()[saveOptionIndex].isSelectable = instance->width() > 0;
  return 1;
}
// TODO: Get user input for save location.
int saveButton() {
  SaveMaze(*instance);
  return 1;
}
int loadButton() {
  DIR* dir;
  struct dirent* ent;
  struct stat st;
  dir = opendir(savesDir);
  vector<string> saveFiles;
  while ((ent = readdir(dir)) != NULL) {
    const string file_name = ent->d_name;
    const string full_file_name = (string)savesDir + "/" + file_name;

    if (file_name[0] == '.') continue;

    if (stat(full_file_name.c_str(), &st) == -1) continue;

    const bool is_directory = (st.st_mode & S_IFDIR) != 0;

    if (is_directory) continue;

    saveFiles.push_back(file_name);
  }
  closedir(dir);

  loadMenu_->clearOptions();
  for (int i = 0; i < (int)saveFiles.size(); ++i) {
    loadMenu_->addOption(Menu::MenuController::Option(saveFiles[i].c_str(),
                                                    &loadMaze, true, i == 0));
  }
  loadMenu_->addOption(
      Menu::MenuController::Option("-Close Menu-", &closeSubMenu));
  loadMenu_->startMenu();

  return 1;
}
int loadMaze() {
  if (instance->load(
          ((string)savesDir +
           loadMenu_->getOptionList()[loadMenu_->getCurrentIndex()].text)
              .c_str())) {
    instance->play();
    menu_->getOptionList()[saveOptionIndex].isSelectable =
        instance->width() > 0;
    return 1;
  } else {
    return 0;
  }
}
int closeSubMenu() { return 100; }
int ExitApp() {
  endwin();
  exit(0);
  return 0;
}
