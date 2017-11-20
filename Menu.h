#ifndef MENU_H
#define MENU_H
#include <sstream>
#include <vector>

namespace Menu {
// Available inputs.
enum Input { UP, DOWN, LEFT, RIGHT, SELECT, QUIT, NONE };
// Color pair indexes.
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

  // An option to show in the menu list.
  struct Option {
    Option(const char* text_, int (*selectAction)(), bool isSelectable = true,
           bool isHighlighted = false)
        : text_(text_),
          selectAction(selectAction),
          isSelectable(isSelectable),
          isHighlighted(isHighlighted) {
      isNumber = false;
      isTextInput = false;
    }
    Option(int number, int (*selectAction)(), bool isSelectable = true,
           bool isHighlighted = false)
        : number(number),
          selectAction(selectAction),
          isSelectable(isSelectable),
          isHighlighted(isHighlighted) {
      isNumber = true;
      isTextInput = false;
    }
    Option(bool isSelectable = true, bool isHighlighted = false)
        : isSelectable(isSelectable), isHighlighted(isHighlighted) {
      isNumber = false;
      isTextInput = true;
    }

    const char* text() const {
      if (isNumber) {
        std::ostringstream ss;
        ss << "<" << number << ">";
        return ss.str().c_str();
      } else if (isTextInput) {
        std::ostringstream ss;
        ss << "[" << modifyableText << "]";
        return ss.str().c_str();
      } else {
        return text_;
      }
    }

    // The text of the option to show in the menu.
    const char* text_;
    int number;
    std::string modifyableText;
    // The function to call when the button is selected.
    int (*selectAction)();
    // Whether or not the button can be selected.
    bool isSelectable;
    // If the button is highlighted.
    bool isHighlighted;
    // If the option is a number that can be increased or decreased.
    bool isNumber;
    bool isTextInput;
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

  // Adds an option to the menu list.
  Option* addOption(const Option &newOption) {
    optionList.push_back(newOption);
    if (newOption.isHighlighted) currentIndex = optionList.size() - 1;
    return &optionList[optionList.size() - 1];
  }
  void clearOptions() { optionList.clear(); }

  // Returns whether the menu is open or not.
  bool isWinOpen() const { return isWinOpen_; }
  // Returns list of current options.
  std::vector<Option>& getOptionList() { return optionList; }
  int getCurrentIndex() const { return currentIndex; }

 protected:
  // Gets the user's input.
  Input getInput() const;
  // Get the next Option that isSelectable in the given direction.
  int getNextIndex(Input direction) const;

  // Set index based off Colors
  void setColor(Colors index) const;
  void unsetColor(Colors index) const;

 private:
  // Current selected menu option index in option list.
  int currentIndex;
  // Stores whether the window is open or not.
  bool isWinOpen_;
  // List of all options to show in the menu.
  std::vector<Option> optionList;
  // Title to show at the top before the options.
  const char* title;
  // Get text input via curses screen.
  std::string getString();
};
}  // namespace Menu

#include "Menu.cpp"
#endif  // ifndef MENU_H
