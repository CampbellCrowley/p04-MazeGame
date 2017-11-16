#ifndef MENU_H
#define MENU_H
#include "Maze.h"
#include <vector>

namespace Menu {
enum Input { UP, DOWN, LEFT, RIGHT, SELECT, QUIT, NONE };
// The game being controlled. This is to get around templates requiring
// everything to be in one file.
typedef Maze::MazeController M;
class Menu {
 public:
  Menu(M &instance, const char* title = "") : instance(instance), title(title) {
    isWinOpen_ = false;
  }
  ~Menu() { endWin(); }

  struct Option {
    Option(const char* text, int (Menu::*selectAction)(),
           bool isSelectable = true, bool isHighlighted = false)
        : text(text),
          selectAction(selectAction),
          isSelectable(isSelectable),
          isHighlighted(isHighlighted) {}

    const char* text;
    int (Menu::*selectAction)();
    bool isSelectable;
    bool isHighlighted;
  };

  // Opens menu and takes over control flow.
  void startMenu();
  // Opens the menu but does not take over control flow.
  void openMenu();
  // Closes the menu.
  void closeMenu();
  // Starts curses session
  void startWin();
  // Ends curses session
  void endWin();

  void addMazeOptions();
  void addOption(Option newOption) { optionList.push_back(newOption); }

  bool isWinOpen() const { return isWinOpen_; }
  const std::vector<Option>& getOptionList() const { return optionList; }

 protected:
  Input getInput();

 private:
  bool isWinOpen_;
  M instance;
  std::vector<Option> optionList;
  int saveButton() {
    //TODO: Get user input for save location.
    return instance.save("filename.dat");
  }
  int loadButton() {
    //TODO: Get user input for save location.
    return instance.load("filename.dat");
  }
  const char* title;
  int playEasy() { instance.play(11, 11); return 1; }
  int playMedium() { instance.play(51, 51); return 1; }
  int playHard() { instance.play(101, 101); return 1; }
  int playCustom() {
    // TODO: Get user input for numbers here.
    int rows = 0, cols = 0;
    instance.play(rows, cols);
    return 1;
  }
};
}  // namespace Menu

#include "Menu.cpp"
#endif  // ifndef MENU_H
