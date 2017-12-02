#ifndef CALLBACKS_H
#define CALLBACKS_H

struct Callbacks {
  // Chooses easy difficulty maze and begins level.
  int playEasy();
  // Chooses medium difficulty maze and begins level.
  int playMedium();
  // Chooses hard difficulty maze and begins level.
  int playHard();
  // Chooses custom difficulty and opens menu to pick settings.
  int playCustom();
  // Re-opens currently loaded maze.
  int playResume();
  // Called when Confirm is chosen in custom menu, and begins the level with the
  // user inputted settings.
  int customButton();
  // Save option was chosen, starts save menu to allow user to name the file
  // they are about to save.
  int saveButton();
  // Called when Confirm is chosen in the save menu and writes the maze to file.
  int filenameButton();
  // Load option was chosen from main menu, stat the saves dir, and show user
  // all available files to load.
  int loadButton();
  // File was chosen from load menu, load the chosen file and start the level.
  int loadMaze();
  // Close whichever menu is open and return to previous menu.
  int closeSubMenu();
  // Update the custom menu confirm button to only be selectable while values
  // are positive.
  int updateConfirm();
  // Do nothing. For Options that don't need a callback.
  int nothing() { return 0; }
  // Terminate the entire program now.
  int ExitApp_();
  static int ExitApp();

 private:
  void updateMenuOptions();
}; // struct Callbacks
#endif /* ifndef CALLBACKS_H */
