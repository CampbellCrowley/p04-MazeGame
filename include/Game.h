#ifndef GAME_H
#define GAME_H
#include <curses.h>
#include <dirent.h>
#include <signal.h>
#include <sys/stat.h>
#include <cstdlib>
#include <iostream>
#include "Maze.h"
#include "Menu.h"

namespace Game {
class GameController;
}

extern Game::GameController game_;

namespace Game {

static const char* savesDir = "./saves/";

// TODO: Find better way than storing option indexes for each option. Pointers
// didn't work due to poor implementation?
class GameController {
 public:
  void Initialize(int /*argc*/, const char** /*argv[]*/);
  // Check for previous session data.
  // TODO: Better feedback on fail.
  void CheckPrevious();
  // Run the game. Blocks until complete.
  int Run();

  GameController() {
    saveOptionIndex_ = 0;
    saveSettingsOptionIndex_ = 0;
    rowOptionIndex_ = 0;
    colOptionIndex_ = 0;
    confirmOptionIndex_ = 0;
    filenameOptionIndex_ = 0;
    game_ = *this;
  }

  // TODO: Remove getters? Can't return const, must return reference.
  Maze::MazeController& Instance() { return instance_; }
  Menu::MenuController& LoadMenu() { return loadMenu_; }
  Menu::MenuController& SaveMenu() { return saveMenu_; }
  Menu::MenuController& CustomMenu() { return customMenu_; }
  Menu::MenuController& MainMenu() { return menu_; }
  int ResumeOptionIndex() { return resumeOptionIndex_; }
  int SaveOptionIndex() { return saveOptionIndex_; }
  int SaveSettingsOptionIndex() { return saveSettingsOptionIndex_; }
  int RowOptionIndex() { return rowOptionIndex_; }
  int ColOptionIndex() { return colOptionIndex_; }
  int ConfirmOptionIndex() { return confirmOptionIndex_; }
  int FilenameOptionIndex() { return filenameOptionIndex_; }

 private:
  Maze::MazeController instance_;
  Menu::MenuController loadMenu_;
  Menu::MenuController saveMenu_;
  Menu::MenuController customMenu_;
  Menu::MenuController menu_;
  int resumeOptionIndex_;
  int saveOptionIndex_;
  int saveSettingsOptionIndex_;
  int rowOptionIndex_;
  int colOptionIndex_;
  int confirmOptionIndex_;
  int filenameOptionIndex_;

  // Catches Ctrl+C in order to call ExitApp properly.
  static void interruptHandler(int s);
};  // class GameController
}  // namespace Game
#endif /* ifndef GAME_H */
