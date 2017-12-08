#include "Game.h"
#include <string>
#include <vector>
#include "Callbacks.h"
#include "CampbellLib/CampbellLib.h"
#include "Maze.h"
#include "Menu.h"
#include "Titles.h"

namespace Game {

using namespace std;

void GameController::Initialize(int /*argc*/, const char** /*argv[]*/) {
  // Instantiate menus and maze controller.
  menu_ = Menu::MenuController(Titles::title);
  loadMenu_ = Menu::MenuController(Titles::loadTitle);
  saveMenu_ = Menu::MenuController(Titles::saveTitle);
  customMenu_ = Menu::MenuController(Titles::customTitle);

  // Add options to constant/unchanging menus.
  // Main Menu
  resumeOptionIndex_ = menu_.getOptionList().size();
  menu_.addOption(Menu::MenuController::Option("Resume", &Callbacks::playResume, false));
  menu_.addOption(Menu::MenuController::Option( "----------", &Callbacks::nothing, false, false));
  menu_.addOption(Menu::MenuController::Option("Easy", &Callbacks::playEasy, true, true));
  menu_.addOption(Menu::MenuController::Option("Medium", &Callbacks::playMedium));
  menu_.addOption(Menu::MenuController::Option("Hard", &Callbacks::playHard));
  menu_.addOption(Menu::MenuController::Option("Custom", &Callbacks::playCustom));
  menu_.addOption(Menu::MenuController::Option( "----------", &Callbacks::nothing, false, false));
  saveOptionIndex_ = menu_.getOptionList().size();
  menu_.addOption(Menu::MenuController::Option("Save Maze", &Callbacks::saveButton, false));
  menu_.addOption(Menu::MenuController::Option("Load Maze", &Callbacks::loadButton));
  menu_.addOption(Menu::MenuController::Option( "----------", &Callbacks::nothing, false, false));
  menu_.addOption(Menu::MenuController::Option("Quit", &Callbacks::ExitApp_));

  // Custom Maze Menu
  customMenu_.addOption(Menu::MenuController::Option("Rows:", &Callbacks::nothing, false, false));
  rowOptionIndex_ = customMenu_.getOptionList().size();
  customMenu_.addOption(Menu::MenuController::Option(20, &Callbacks::updateConfirm, true, true));
  customMenu_.addOption(Menu::MenuController::Option( "Columns:", &Callbacks::nothing, false, false));
  colOptionIndex_ = customMenu_.getOptionList().size();
  customMenu_.addOption(Menu::MenuController::Option(20, &Callbacks::updateConfirm));
  confirmOptionIndex_ = customMenu_.getOptionList().size();
  customMenu_.addOption(Menu::MenuController::Option("Confirm", &Callbacks::customButton));
  customMenu_.addOption(Menu::MenuController::Option( "----------", &Callbacks::nothing, false, false));
  customMenu_.addOption(Menu::MenuController::Option("Back", &Callbacks::closeSubMenu));

  // Save Maze Menu
  vector<string> saveOptions;
  saveOptions.push_back("All session data");
  saveOptions.push_back("Hints (Replace with history)");
  saveOptions.push_back("Hints (Replace with empty)");
  saveOptions.push_back("Nothing");
  saveMenu_.addOption(Menu::MenuController::Option( "Filename:", &Callbacks::nothing, false, false));
  filenameOptionIndex_ = saveMenu_.getOptionList().size();
  saveMenu_.addOption(Menu::MenuController::Option(true, true)) ->modifyableText = "MyMaze.dat";
  saveSettingsOptionIndex_ = saveMenu_.getOptionList().size();
  saveMenu_.addOption(Menu::MenuController::Option("Remove", saveOptions));
  saveMenu_.addOption(Menu::MenuController::Option("Confirm", &Callbacks::filenameButton));
  saveMenu_.addOption(Menu::MenuController::Option( "----------", &Callbacks::nothing, false, false));
  saveMenu_.addOption(Menu::MenuController::Option("Back", &Callbacks::closeSubMenu));

  // Handle execution interrupt.
  struct sigaction sigIntHandler;
  sigIntHandler.sa_handler = interruptHandler;
  sigemptyset(&sigIntHandler.sa_mask);
  sigIntHandler.sa_flags = 0;
  sigaction(SIGINT, &sigIntHandler, NULL);
}
void GameController::CheckPrevious() {
  struct stat buf;
  if (stat(Maze::lastSessionFilename, &buf) != -1) {
    cout << Campbell::Color::yellow
         << "I found a previous session data. Would you like to "
            "load it? (Y/n): "
         << Campbell::Color::reset;
    if (Campbell::Strings::getYesNo(true)) {
      string option = Maze::lastSessionFilename;
      if (!instance_.load(option.c_str())) {
        cerr << Campbell::Color::red << "Failed to open file.\n"
             << Campbell::Color::reset;
        return;
      } else {
        instance_.play();
        menu_.getOptionList()[saveOptionIndex_].isSelectable =
            instance_.width() > 0;
        menu_.getOptionList()[resumeOptionIndex_].isSelectable =
            instance_.width() > 0;
      }
    }
  }
}
int GameController::Run() {
  // Catch any exceptions to ensure NCurses shuts down properly.
  try {
    // Start main menu.
    menu_.startMenu();
  } catch (...) {
    endwin();
    cerr << Campbell::Color::red << "Exiting due to unknown failure.\n"
         << Campbell::Color::reset;
    return 1;
  }
  return 0;
}

void GameController::interruptHandler(int s) {
  (void)s;
  Callbacks::ExitApp();
}
}  // namespace Game
