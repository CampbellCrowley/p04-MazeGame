#include "Menu.h"
#include <curses.h>
namespace Menu {
void MenuController::startMenu() {
  openMenu();
  Input nextInput = NONE;
  while(nextInput != QUIT) {
    nextInput = getInput();
    if (move(nextInput)) printMenu();
  }
}
void MenuController::openMenu() {
  startWin();
  printMenu();
}
void MenuController::printMenu() const {
  if (!isWinOpen_) return;
  ::move(0, 0);
  clear();
  setColor(TITLE);
  addstr(title);
  unsetColor(TITLE);
  int startPos;
  int titleWidth;
  getyx(stdscr, startPos, titleWidth);
  startPos += 2;
  for (int i = 0; i < (int)optionList.size(); ++i) {
    ::move(startPos + i, 0);
    if (optionList[i].isHighlighted) {
      setColor(HIGHLIGHTED);
      addch(' ');
    } else if (!optionList[i].isSelectable) {
      setColor(DISABLED);
    } else {
      setColor(NORMAL);
    }
    addstr(optionList[i].text);
    if (optionList[i].isHighlighted) {
      unsetColor(HIGHLIGHTED);
    } else if (!optionList[i].isSelectable) {
      unsetColor(DISABLED);
    } else {
      unsetColor(NORMAL);
    }
  }
  setColor(INSTRUCTIONS);
  ::move(16, 20);
  addstr("Controls:");
  ::move(17, 20);
  addstr("UP:      K or \u2191 (Up arrow)");
  ::move(18, 20);
  addstr("DOWN:    J or \u2193 (Down arrow)");
  ::move(19, 20);
  addstr("LEFT:    H or \u2191 (Left arrow)");
  ::move(20, 20);
  addstr("RIGHT:   L or \u2192 (Right arrow)");
  ::move(21, 20);
  addstr("HINT:    \"");
  ::move(22, 20);
  addstr("GIVE UP: ?");
  ::move(23, 20);
  addstr("QUIT:    Q");
  unsetColor(INSTRUCTIONS);
  ::move(0, 0);
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
      // return LEFT;
      return SELECT;
    case KEY_RIGHT:
    case 'L':
    case 'l':
      //return RIGHT;
      return SELECT;
    case 'Q':
    case 'q':
      return QUIT;
    case KEY_ENTER:
    case KEY_HOME:
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
      closeMenu();
      optionList[currentIndex].selectAction();
      openMenu();
      return false;
    case LEFT:
    case RIGHT:
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
void MenuController::setColor(Colors index) const { attron(COLOR_PAIR(index)); }
void MenuController::unsetColor(Colors index) const { attroff(COLOR_PAIR(index)); }
}  // namespace Menu
