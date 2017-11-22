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

typedef Menu::MenuController::Option Option;

// Global vars
Maze::MazeController* instance_;
Menu::MenuController* loadMenu_;
Menu::MenuController* saveMenu_;
Menu::MenuController* customMenu_;
Menu::MenuController* menu_;
int saveOptionIndex = 0;
int saveSettingsOptionIndex = 0;
int rowOptionIndex = 0;
int colOptionIndex = 0;
int confirmOptionIndex = 0;
int filenameOptionIndex = 0;
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
const char *saveTitle = // Save Maze
    "      ::::::::      :::     :::     ::: ::::::::::            :::   :::   "
    "    :::     ::::::::: ::::::::::\n    :+:    :+:   :+: :+:   :+:     :+: "
    ":+:                  :+:+: :+:+:    :+: :+:        :+:  :+:        \n   "
    "+:+         +:+   +:+  +:+     +:+ +:+                 +:+ +:+:+ +:+  +:+ "
    "  +:+      +:+   +:+         \n  +#++:++#++ +#++:++#++: +#+     +:+ "
    "+#++:++#            +#+  +:+  +#+ +#++:++#++:    +#+    +#++:++#     \n   "
    "     +#+ +#+     +#+  +#+   +#+  +#+                 +#+       +#+ +#+    "
    " +#+   +#+     +#+           \n#+#    #+# #+#     #+#   #+#+#+#   #+#     "
    "            #+#       #+# #+#     #+#  #+#      #+#            \n######## "
    " ###     ###     ###     ##########          ###       ### ###     ### "
    "######### ##########      \n";

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
const char* customTitle =  // Custom
    "      ::::::::  :::    :::  :::::::: ::::::::::: ::::::::    :::   ::: \n"
    "    :+:    :+: :+:    :+: :+:    :+:    :+:    :+:    :+:  :+:+: :+:+: \n"
    "   +:+        +:+    +:+ +:+           +:+    +:+    +:+ +:+ +:+:+ +:+ \n"
    "  +#+        +#+    +:+ +#++:++#++    +#+    +#+    +:+ +#+  +:+  +#+  \n"
    " +#+        +#+    +#+        +#+    +#+    +#+    +#+ +#+       +#+   \n"
    "#+#    #+# #+#    #+# #+#    #+#    #+#    #+#    #+# #+#       #+#    \n"
    "########   ########   ########     ###     ########  ###       ###     \n";

// Prototypes
void interruptHandler(int s);
int playEasy();
int playMedium();
int playHard();
int playCustom();
int customButton();
int saveButton();
int filenameButton();
int loadButton();
int loadMaze();
int closeSubMenu();
int ExitApp();
int updateConfirm();
int nothing() { return 0; }

// Entry
int main(int /*argc*/, const char** /*argv[]*/) {
  // Instantiate menus and maze controller.
  Maze::MazeController mc;
  Menu::MenuController menu(title);
  Menu::MenuController loadMenu(loadTitle);
  Menu::MenuController saveMenu(saveTitle);
  Menu::MenuController customMenu(customTitle);
  // Save references for later use.
  instance_ = &mc;
  menu_ = &menu;
  loadMenu_ = &loadMenu;
  saveMenu_ = &saveMenu;
  customMenu_ = &customMenu;

  // Add options to constant/unchanging menus.
  // Main Menu
  menu.addOption(Option("Easy", &playEasy, true, true));
  menu.addOption(Option("Medium", &playMedium));
  menu.addOption(Option("Hard", &playHard));
  menu.addOption(Option("Custom", &playCustom));
  menu.addOption(Option("----------", &nothing, false, false));
  saveOptionIndex = menu.getOptionList().size();
  menu.addOption(Option("Save Maze", &saveButton, false));
  menu.addOption(Option("Load Maze", &loadButton));
  menu.addOption(Option("----------", &nothing, false, false));
  menu.addOption(Option("Quit", &ExitApp));

  // Custom Maze Menu
  customMenu.addOption(Option("Rows:", &nothing, false, false));
  rowOptionIndex = customMenu.getOptionList().size();
  customMenu.addOption(Option(20, &updateConfirm, true, true));
  customMenu.addOption(Option("Columns:", &nothing, false, false));
  colOptionIndex = customMenu.getOptionList().size();
  customMenu.addOption(Option(20, &updateConfirm));
  confirmOptionIndex = customMenu.getOptionList().size();
  customMenu.addOption(Option("Confirm", &customButton));
  customMenu.addOption(Option("----------", &nothing, false, false));
  customMenu.addOption(Option("Back", &closeSubMenu));

  // Save Maze Menu
  vector<string> saveOptions;
  saveOptions.push_back("All session data");
  saveOptions.push_back("Hints (Replace with history)");
  saveOptions.push_back("Hints (Replace with empty)");
  saveOptions.push_back("Nothing");
  saveMenu.addOption(Option("Filename:", &nothing, false, false));
  filenameOptionIndex = saveMenu.getOptionList().size();
  saveMenu.addOption(Option(true, true))->modifyableText = "MyMaze.dat";
  saveSettingsOptionIndex = saveMenu.getOptionList().size();
  saveMenu.addOption(Option("Remove", saveOptions));
  saveMenu.addOption(Option("Confirm", &filenameButton));
  saveMenu.addOption(Option("----------", &nothing, false, false));
  saveMenu.addOption(Option("Back", &closeSubMenu));

  // Handle execution interrupt.
  struct sigaction sigIntHandler;
  sigIntHandler.sa_handler = interruptHandler;
  sigemptyset(&sigIntHandler.sa_mask);
  sigIntHandler.sa_flags = 0;
  sigaction(SIGINT, &sigIntHandler, NULL);

  // Check for previous session data.
  {
    struct stat buf;
    if (stat(Maze::lastSessionFilename, &buf) != -1) {
      cout << yellow << "I found a previous session data. Would you like to "
                        "load it? (Y/n): "
           << reset;
      if (Campbell::Strings::getYesNo(true)) {
        string option = Maze::lastSessionFilename;
        if (!mc.load(option.c_str())) {
          cerr << red << "Failed to open file.\n" << reset;
          return 1;
        } else {
          mc.play();
          menu.getOptionList()[saveOptionIndex].isSelectable =
              instance_->width() > 0;
        }
      }
    }
  }

  // Catch any exceptions to ensure NCurses shuts down properly.
  try {
    // Start main menu.
    menu.startMenu();
  } catch (...) {
    endwin();
    cerr << "Exiting due to unknown failure.\n";
    return 1;
  }
  return 0;
}

// Definitions
// Catches Ctrl+C in order to call ExitApp properly.
void interruptHandler(int s) {
  (void)s;
  ExitApp();
}
// Chooses easy difficulty maze and begins level.
int playEasy() {
  instance_->play(11, 11);
  menu_->getOptionList()[saveOptionIndex].isSelectable = instance_->width() > 0;
  return 1;
}
// Chooses medium difficulty maze and begins level.
int playMedium() {
  instance_->play(51, 51);
  menu_->getOptionList()[saveOptionIndex].isSelectable = instance_->width() > 0;
  return 1;
}
// Chooses hard difficulty maze and begins level.
int playHard() {
  instance_->play(101, 101);
  menu_->getOptionList()[saveOptionIndex].isSelectable = instance_->width() > 0;
  return 1;
}
// Chooses custom diffuclty and opens menu to pick settings.
int playCustom() {
  customMenu_->startMenu();
  return 1;
}
// Called when Confirm is chosen in custom menu, and begins the level with the
// user inputted settings.
int customButton() {
  const int rows = customMenu_->getOptionList()[rowOptionIndex].number;
  const int cols = customMenu_->getOptionList()[colOptionIndex].number;

  instance_->play(rows, cols);
  return 1;
}
// Save option was chosen, starts save menu to allow user to name the file they
// are about to save.
int saveButton() {
  saveMenu_->startMenu();
  return 1;
}
// Called when Confirm is chosen in the save menu and writes the maze to file.
int filenameButton() {
  unsigned int option =
      saveMenu_->getOptionList()[saveSettingsOptionIndex].currentValue;
  if (option == 0) instance_->resetPosition();
  if (option !=
      saveMenu_->getOptionList()[saveSettingsOptionIndex].values.size()) {
    for (int i = 0; i < (int)instance_->height(); ++i) {
      for (int j = 0; j < (int)instance_->width(); ++j) {
        Maze::TileData *data = &(instance_->getMaze()[i][j]);
        switch(option) {
          case 0: // Clear all data.
            if (*data == Maze::CURRENT || *data == Maze::PREVIOUS ||
                *data == Maze::HINT) {
              *data = Maze::EMPTY;
            }
            break;
          case 1: // Change HINTs to PREVIOUS.
          case 2: // Change HINTs to EMPTY.
            if (*data == Maze::HINT) {
              *data = option == 2 ? Maze::EMPTY : Maze::PREVIOUS;
            }
            break;
          default:
            break;
        }
      }
    }
    instance_->getMaze();
  }
  if (instance_->save(
          (savesDir +
           saveMenu_->getOptionList()[filenameOptionIndex].modifyableText)
              .c_str())) {
    return closeSubMenu();
  } else {
    return 0;
  }
}
// Load option was chosen from main menu, stat the saves dir, and show user all
// available files to load.
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
    loadMenu_->addOption(Option(saveFiles[i].c_str(), &loadMaze, true, i == 0));
  }
  loadMenu_->addOption(Option("-Close Menu-", &closeSubMenu));
  loadMenu_->startMenu();

  return 1;
}
// File was chosen from load menu, load the chosen file and start the level.
int loadMaze() {
  if (instance_->load(
          ((string)savesDir +
           loadMenu_->getOptionList()[loadMenu_->getCurrentIndex()].text())
              .c_str())) {
    instance_->play();
    menu_->getOptionList()[saveOptionIndex].isSelectable =
        instance_->width() > 0;
    return closeSubMenu();
  } else {
    return 0;
  }
}
// Close whichever menu is open and return to previous menu.
int closeSubMenu() { return 100; }
// Terminate the entire program now.
int ExitApp() {
  endwin();
  exit(0);
  return 0;
}
// Update the custom menu confirm button to only be selectable while values are
// positive.
int updateConfirm() {
  const int rows = customMenu_->getOptionList()[rowOptionIndex].number;
  const int cols = customMenu_->getOptionList()[colOptionIndex].number;
  customMenu_->getOptionList()[confirmOptionIndex].isSelectable =
      rows > 0 && cols > 0;
  return 1;
}
