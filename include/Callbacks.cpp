#include "Callbacks.h"
#include <cstdlib>
#include <string>
#include <vector>
#include "Game.h"

using namespace std;

int Callbacks::playEasy() {
  game_.Instance().play(11, 11);
  updateMenuOptions();
  return 1;
}
int Callbacks::playMedium() {
  game_.Instance().play(51, 51);
  updateMenuOptions();
  return 1;
}
int Callbacks::playHard() {
  game_.Instance().play(101, 101);
  updateMenuOptions();
  return 1;
}
int Callbacks::playCustom() {
  game_.CustomMenu().startMenu();
  updateMenuOptions();
  return 1;
}
int Callbacks::playResume() {
  game_.Instance().play();
  updateMenuOptions();
  return 1;
}
int Callbacks::customButton() {
  const int rows =
      game_.CustomMenu().getOptionList()[game_.RowOptionIndex()].number;
  const int cols =
      game_.CustomMenu().getOptionList()[game_.ColOptionIndex()].number;

  game_.Instance().play(rows, cols);
  return 1;
}
int Callbacks::saveButton() {
  game_.SaveMenu().startMenu();
  return 1;
}
int Callbacks::filenameButton() {
  unsigned int option = game_.SaveMenu()
                            .getOptionList()[game_.SaveSettingsOptionIndex()]
                            .currentValue;
  if (option == 0) game_.Instance().resetPosition();
  if (option !=
      game_.SaveMenu()
          .getOptionList()[game_.SaveSettingsOptionIndex()]
          .values.size()) {
    for (int i = 0; i < (int)game_.Instance().height(); ++i) {
      for (int j = 0; j < (int)game_.Instance().width(); ++j) {
        Maze::TileData* data = &(game_.Instance().getMaze()[i][j]);
        switch (option) {
          case 0:  // Clear all data.
            if (*data == Maze::CURRENT || *data == Maze::PREVIOUS ||
                *data == Maze::HINT) {
              *data = Maze::EMPTY;
            }
            break;
          case 1:  // Change HINTs to PREVIOUS.
          case 2:  // Change HINTs to EMPTY.
            if (*data == Maze::HINT) {
              *data = option == 2 ? Maze::EMPTY : Maze::PREVIOUS;
            }
            break;
          default:
            break;
        }
      }
    }
  }
  if (game_.Instance().save((Game::savesDir +
                             game_.SaveMenu()
                                 .getOptionList()[game_.FilenameOptionIndex()]
                                 .modifyableText)
                                .c_str())) {
    return closeSubMenu();
  } else {
    return 0;
  }
}
int Callbacks::loadButton() {
  DIR* dir;
  struct dirent* ent;
  struct stat st;
  dir = opendir(Game::savesDir);
  vector<string> saveFiles;
  while ((ent = readdir(dir)) != NULL) {
    const string file_name = ent->d_name;
    const string full_file_name = (string)Game::savesDir + "/" + file_name;

    // Ignore hidden files.
    if (file_name[0] == '.') continue;

    if (stat(full_file_name.c_str(), &st) == -1) continue;

    // Ignore directories.
    const bool is_directory = (st.st_mode & S_IFDIR) != 0;
    if (is_directory) continue;

    saveFiles.push_back(file_name);
  }
  closedir(dir);

  game_.LoadMenu().clearOptions();
  for (int i = 0; i < (int)saveFiles.size(); ++i) {
    game_.LoadMenu().addOption(Menu::MenuController::Option(
        saveFiles[i].c_str(), &Callbacks::loadMaze, true, i == 0));
  }
  game_.LoadMenu().addOption(
      Menu::MenuController::Option("-Close Menu-", &Callbacks::closeSubMenu));
  game_.LoadMenu().startMenu();

  return 1;
}
int Callbacks::loadMaze() {
  if (game_.Instance().load(
          ((string)Game::savesDir +
           game_.LoadMenu()
               .getOptionList()[game_.LoadMenu().getCurrentIndex()]
               .text())
              .c_str())) {
    game_.Instance().play();
    updateMenuOptions();
    return Callbacks::closeSubMenu();
  } else {
    return 0;
  }
}
int Callbacks::closeSubMenu() { return 100; }
int Callbacks::updateConfirm() {
  const int rows =
      game_.CustomMenu().getOptionList()[game_.RowOptionIndex()].number;
  const int cols =
      game_.CustomMenu().getOptionList()[game_.ColOptionIndex()].number;
  game_.CustomMenu().getOptionList()[game_.ConfirmOptionIndex()].isSelectable =
      rows > 0 && cols > 0;
  return 1;
}

int Callbacks::ExitApp_() { return ExitApp(); }
int Callbacks::ExitApp() {
  endwin();
  exit(0);
  return 0;
}

void Callbacks::updateMenuOptions() {
  bool mazeExists = game_.Instance().width() > 0;
  game_.MainMenu().getOptionList()[game_.SaveOptionIndex()].isSelectable =
      mazeExists;
  game_.MainMenu().getOptionList()[game_.ResumeOptionIndex()].isSelectable =
      mazeExists;
}
