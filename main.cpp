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
Maze::MazeController* instance_;
Menu::MenuController* loadMenu_;
Menu::MenuController* saveMenu_;
Menu::MenuController* customMenu_;
Menu::MenuController* menu_;
int saveOptionIndex = 0;
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
  menu.addOption(Menu::MenuController::Option("Easy", &playEasy, true, true));
  menu.addOption(Menu::MenuController::Option("Medium", &playMedium));
  menu.addOption(Menu::MenuController::Option("Hard", &playHard));
  menu.addOption(Menu::MenuController::Option("Custom", &playCustom));
  menu.addOption( Menu::MenuController::Option("----------", &nothing, false, false));
  saveOptionIndex = menu.getOptionList().size();
  menu.addOption(Menu::MenuController::Option("Save Maze", &saveButton, false));
  menu.addOption(Menu::MenuController::Option("Load Maze", &loadButton));
  menu.addOption( Menu::MenuController::Option("----------", &nothing, false, false));
  menu.addOption(Menu::MenuController::Option("Quit", &ExitApp));

  // Custom Maze Menu
  customMenu.addOption( Menu::MenuController::Option("Rows:", &nothing, false, false));
  rowOptionIndex = customMenu.getOptionList().size();
  customMenu.addOption(Menu::MenuController::Option(20, &updateConfirm, true, true));
  customMenu.addOption(Menu::MenuController::Option("Columns:", &nothing, false, false));
  colOptionIndex = customMenu.getOptionList().size();
  customMenu.addOption(Menu::MenuController::Option(20, &updateConfirm));
  confirmOptionIndex = customMenu.getOptionList().size();
  customMenu.addOption(Menu::MenuController::Option("Confirm", &customButton));
  customMenu.addOption(Menu::MenuController::Option("----------", &nothing, false, false));
  customMenu.addOption(Menu::MenuController::Option("Back", &closeSubMenu));

  // Save Maze Menu
  saveMenu.addOption(Menu::MenuController::Option("Filename:", &nothing, false, false));
  filenameOptionIndex = saveMenu.getOptionList().size();
  saveMenu.addOption(Menu::MenuController::Option(true, true));
  saveMenu.addOption(Menu::MenuController::Option("Confirm", &filenameButton));
  saveMenu.addOption(Menu::MenuController::Option("----------", &nothing, false, false));
  saveMenu.addOption(Menu::MenuController::Option("Back", &closeSubMenu));

  // Handle execution interrupt.
  struct sigaction sigIntHandler;
  sigIntHandler.sa_handler = interruptHandler;
  sigemptyset(&sigIntHandler.sa_mask);
  sigIntHandler.sa_flags = 0;
  sigaction(SIGINT, &sigIntHandler, NULL);

  // CHeck for previous session data.
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
    loadMenu_->addOption(Menu::MenuController::Option(saveFiles[i].c_str(),
                                                    &loadMaze, true, i == 0));
  }
  loadMenu_->addOption(
      Menu::MenuController::Option("-Close Menu-", &closeSubMenu));
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
