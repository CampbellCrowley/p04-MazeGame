#ifndef MENU_H
#define MENU_H
#include <sstream>
#include <vector>
#include "Callbacks.h"

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
  ERROR,
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
  // TODO: Move all options to inheritable classes.
  // Shows selectable option with title and callback.
  struct Option {
    typedef int (Callbacks::*callback)();
    Option(const char* text_, callback selectAction,
           bool isSelectable = true, bool isHighlighted = false)
        : text_(text_),
          selectAction(selectAction),
          isSelectable(isSelectable),
          isHighlighted(isHighlighted) {
      isNumber = false;
      isTextInput = false;
      isList = false;
    }
    // Shows number that is changeable with callback that is called if value
    // changes.
    Option(int number, callback selectAction, bool isSelectable = true,
           bool isHighlighted = false)
        : number(number),
          selectAction(selectAction),
          isSelectable(isSelectable),
          isHighlighted(isHighlighted) {
      isTextInput = false;
      isList = false;
      isNumber = true;
    }
    // Shows text input for entering a string. No callback, since everything is
    // managed internally.
    Option(bool isSelectable = true, bool isHighlighted = false)
        : isSelectable(isSelectable), isHighlighted(isHighlighted) {
      isNumber = false;
      isList = false;
      isTextInput = true;
    }
    // Allows a choice between a list of options.
    Option(const char *text_, const std::vector<std::string> &values,
           bool isSelectable = true, bool isHighlighted = false)
        : text_(text_), values(values), isSelectable(isSelectable),
          isHighlighted(isHighlighted) {
      isNumber = false;
      isTextInput = false;
      isList = true;
      currentValue = 0;
    }

    // Returns formatted c-style string for showing the user.
    const char* text() const {
      if (isNumber) {
        std::stringstream ss;
        ss << number;
        return ss.str().c_str();
      } else if (isTextInput) {
        return modifyableText.c_str();
      } else if (isList) {
        if (currentValue < values.size()) {
          return values[currentValue].c_str();
        } else {
          return "Oops, this is broken.";
        }
      } else {
        return text_;
      }
    }

    // The text of the option to show in the menu.
    const char* text_;
    // Current value if this option is choosing a number.
    int number;
    // Current text input if this is a text input option.
    std::string modifyableText;
    // List of values to choose between in list option.
    std::vector<std::string> values;
    unsigned int currentValue;

    // The function to call when the button is selected. Not used in text input.
    callback selectAction;
    // Whether or not the option can be selected.
    bool isSelectable;
    // If the option is highlighted.
    bool isHighlighted;
    // If the option is a number that can be increased or decreased.
    bool isNumber;
    // If the option is a text input option.
    bool isTextInput;
    // If the option is a list of values to choose between.
    bool isList;
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
  // moved or if screen needs to be reprinted.
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

  // Set current color in ncurses.
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
  std::string getString() const;

  Callbacks callbacks_;
}; // class Menu
} // namespace Menu

#endif  // ifndef MENU_H
