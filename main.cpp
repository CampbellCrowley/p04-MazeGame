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
  Maze::MazeController mc;
  Menu::MenuController menu(title);
  Menu::MenuController loadMenu(loadTitle);
  Menu::MenuController saveMenu(saveTitle);
  Menu::MenuController customMenu(customTitle);
  instance = &mc;
  menu_ = &menu;
  loadMenu_ = &loadMenu;
  saveMenu_ = &saveMenu;
  customMenu_ = &customMenu;

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
int playCustom() {
  customMenu_->startMenu();
  return 1;
}
int customButton() {
  const int rows = customMenu_->getOptionList()[rowOptionIndex].number;
  const int cols = customMenu_->getOptionList()[colOptionIndex].number;

  instance->play(rows, cols);
  return 1;
}
int saveButton() {
  saveMenu_->startMenu();
  return 1;
}
int filenameButton() {
  if (instance->save(
          (savesDir +
           saveMenu_->getOptionList()[filenameOptionIndex].modifyableText)
              .c_str())) {
    return closeSubMenu();
  } else {
    return 0;
  }
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
           loadMenu_->getOptionList()[loadMenu_->getCurrentIndex()].text())
              .c_str())) {
    instance->play();
    menu_->getOptionList()[saveOptionIndex].isSelectable =
        instance->width() > 0;
    return closeSubMenu();
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
int updateConfirm() {
  const int rows = customMenu_->getOptionList()[rowOptionIndex].number;
  const int cols = customMenu_->getOptionList()[colOptionIndex].number;
  customMenu_->getOptionList()[confirmOptionIndex].isSelectable =
      rows > 0 && cols > 0;
  return 1;
}
