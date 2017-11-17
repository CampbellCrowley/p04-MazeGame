#ifndef MENU_H
#define MENU_H
#include <vector>

namespace Menu {
enum Input { UP, DOWN, LEFT, RIGHT, SELECT, QUIT, NONE };
enum Colors {
  HIGHLIGHTED = 50,
  NORMAL,
  TITLE,
  INSTRUCTIONS,
  BACKGROUND,
  DISABLED
};
class MenuController {
 public:
  MenuController(const char* title = "") : title(title) {
    isWinOpen_ = false;
    currentIndex = 0;
  }
  ~MenuController() { endWin(); }

  struct Option {
    Option(const char* text, int (*selectAction)(),
           bool isSelectable = true, bool isHighlighted = false)
        : text(text),
          selectAction(selectAction),
          isSelectable(isSelectable),
          isHighlighted(isHighlighted) {}

    const char* text;
    int (*selectAction)();
    bool isSelectable;
    bool isHighlighted;
  };

  // Opens menu and takes over control flow.
  void startMenu();
  // Opens the menu but does not take over control flow.
  void openMenu();
  // Prints the menu to the open screen.
  void printMenu() const;
  // Closes the menu.
  void closeMenu();
  // Starts curses session
  void startWin();
  // Ends curses session
  void endWin();
  // Move the current selection in the requested direction. Returns if actually
  // moved.
  bool move(Input direction);

  // Adds an option to the menu.
  Option* addOption(const Option &newOption) {
    optionList.push_back(newOption);
    return &optionList[optionList.size() - 1];
  }

  // Returns if the menu is open.
  bool isWinOpen() const { return isWinOpen_; }
  // Returns list of current options.
  const std::vector<Option>& getOptionList() const { return optionList; }

 protected:
  // Gets the user's input.
  Input getInput() const;
  // Get the next Option that isSelectable in the given direction.
  int getNextIndex(Input direction) const;
  void setColor(int index) const;
  void unsetColor(int index) const;

 private:
  int currentIndex;
  bool isWinOpen_;
  std::vector<Option> optionList;
  const char* title;
};
}  // namespace Menu

#include "Menu.cpp"
#endif  // ifndef MENU_H
