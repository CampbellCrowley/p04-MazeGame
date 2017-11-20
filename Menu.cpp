#include "Menu.h"
#include <curses.h>
namespace Menu {
void MenuController::startMenu() {
  openMenu();
  Input nextInput = NONE;
  while (nextInput != QUIT && isWinOpen_) {
    nextInput = getInput();
    if (move(nextInput)) printMenu();
    if (!isWinOpen_) break;
  }
}
void MenuController::openMenu() {
  startWin();
  printMenu();
}
void MenuController::printMenu() const {
  if (!isWinOpen_) return;
  ::move(0, 0);
  erase();
  clear();
  setColor(TITLE);
  addstr(title);
  unsetColor(TITLE);
  int startPos;
  int cursorPos = 0;
  int titleWidth;
  getyx(stdscr, startPos, titleWidth);
  startPos += 2;
  for (int i = 0; i < (int)optionList.size(); ++i) {
    ::move(startPos + i, 0);
    if (optionList[i].isHighlighted) {
      setColor(HIGHLIGHTED);
      addch(' ');
      cursorPos = startPos + i;
    } else if (!optionList[i].isSelectable) {
      setColor(DISABLED);
    } else {
      setColor(NORMAL);
    }
    addstr(optionList[i].text());
    if (optionList[i].isHighlighted) {
      unsetColor(HIGHLIGHTED);
    } else if (!optionList[i].isSelectable) {
      unsetColor(DISABLED);
    } else {
      unsetColor(NORMAL);
    }
  }
  setColor(INSTRUCTIONS);
  ::move(startPos, 20);
  addstr("Controls:");
  ::move(startPos+1, 20);
  addstr("UP:      K or \u2191 (Up arrow)");
  ::move(startPos+2, 20);
  addstr("DOWN:    J or \u2193 (Down arrow)");
  ::move(startPos+3, 20);
  addstr("LEFT:    H or \u2191 (Left arrow)");
  ::move(startPos+4, 20);
  addstr("RIGHT:   L or \u2192 (Right arrow)");
  ::move(startPos+5, 20);
  addstr("SELECT:  Enter or HINT or GIVE UP");
  ::move(startPos+6, 20);
  addstr("HINT:    \"");
  ::move(startPos+7, 20);
  addstr("GIVE UP: ?");
  ::move(startPos+8, 20);
  addstr("QUIT:    Q");
  unsetColor(INSTRUCTIONS);
  ::move(cursorPos, 0);
  refresh();
}
void MenuController::closeMenu() { endWin(); }
void MenuController::startWin() {
  // Handle NCurses.
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  // Colors
  start_color();
  init_pair(HIGHLIGHTED, COLOR_CYAN, COLOR_BLACK);
  init_pair(NORMAL, COLOR_WHITE, COLOR_BLACK);
  init_pair(TITLE, COLOR_CYAN, COLOR_BLACK);
  init_pair(INSTRUCTIONS, COLOR_YELLOW, COLOR_BLACK);
  init_pair(BACKGROUND, COLOR_BLACK, COLOR_BLACK);
  init_pair(DISABLED, COLOR_RED, COLOR_BLACK);

  // TODO: Doesn't seem to work?
  bkgd(COLOR_PAIR(BACKGROUND));

  isWinOpen_ = true;
}
void MenuController::endWin() {
  endwin();
  isWinOpen_ = false;
}
Input MenuController::getInput() const {
  wchar_t input = getch();
  switch (input) {
    case KEY_DOWN:
    case 'J':
    case 'j':
      return DOWN;
    case KEY_UP:
    case 'K':
    case 'k':
      return UP;
    case KEY_LEFT:
    case 'H':
    case 'h':
      return LEFT;
    case KEY_RIGHT:
    case 'L':
    case 'l':
      return RIGHT;
    case 'Q':
    case 'q':
      return QUIT;
    case 10: // Enter
    case KEY_ENTER:
    case '\'':
    case '\"':
    case '/':
    case '?':
      return SELECT;
    default:
      return NONE;
  }
}
bool MenuController::move(Input direction) {
  switch(direction) {
    case UP:
    case DOWN: {
      int nextIndex = getNextIndex(direction);
      if (nextIndex != currentIndex) {
        optionList[currentIndex].isHighlighted = false;
        currentIndex = nextIndex;
        optionList[currentIndex].isHighlighted = true;
        return true;
      } else {
        return false;
      }
    }
    case SELECT:
      if (optionList[currentIndex].isTextInput) {
        optionList[currentIndex].modifyableText.clear();
        printMenu();
        optionList[currentIndex].modifyableText = getString();
        return true;
      } else {
        closeMenu();
        if (optionList[currentIndex].selectAction() != 100) openMenu();
        return false;
      }
    case LEFT:
    case RIGHT:
      if (optionList[currentIndex].isNumber) {
        optionList[currentIndex].number += direction == LEFT ? -1 : 1;
        if (optionList[currentIndex].selectAction() == 100) closeMenu();
        return true;
      } else {
        return false;
      }
    case NONE:
    default:
      return false;
  }
}
int MenuController::getNextIndex(Input direction) const {
  int multiplier = 1;
  if (direction == UP) multiplier = -1;
  for (int i = currentIndex + multiplier;
       (multiplier > 0) ? i < (int)optionList.size() : i >= 0;
       i += multiplier) {
    if (optionList[i].isSelectable) return i;
  }
  return currentIndex;
}
std::string MenuController::getString() const {
  std::string input;
  char ch;

  nocbreak();
  echo();

  int y, x;
  getyx(stdscr, y, x);
  ::move(y, x + 2);

  setColor(HIGHLIGHTED);
  while ((ch = getch())) {
    if (ch == '\n' || ch <= 31) break;
    unsetColor(HIGHLIGHTED);
    input.push_back(ch);
    printMenu();
    getyx(stdscr, y, x);
    ::move(y, x + 2);
    setColor(HIGHLIGHTED);
  }
  unsetColor(HIGHLIGHTED);

  cbreak();
  noecho();

  return input;
}
void MenuController::setColor(Colors index) const { attron(COLOR_PAIR(index)); }
void MenuController::unsetColor(Colors index) const { attroff(COLOR_PAIR(index)); }
}  // namespace Menu
