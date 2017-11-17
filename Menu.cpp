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
  addstr(title);
  addstr(
      "\n\nControls:\nUP:      \u2191 or K\nDOWN:    \u2193 or J\nLEFT:    "
      "\u2191 or H\nRIGHT:   \u2192 or L\nHINT:    \"\nGIVE UP: ?\nQUIT:    "
      "Q\n");
  for (int i = 0; i < (int)optionList.size(); ++i) {
    if (optionList[i].isHighlighted) setColor(50);
    else setColor(51);
    addstr(optionList[i].text);
    if (optionList[i].isHighlighted) unsetColor(50);
    else unsetColor(51);
    addch('\n');
  }
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
  init_pair(50, COLOR_CYAN, COLOR_BLACK);
  init_pair(51, COLOR_WHITE, COLOR_BLACK);
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
void MenuController::setColor(int index) const { attron(COLOR_PAIR(index)); }
void MenuController::unsetColor(int index) const { attroff(COLOR_PAIR(index)); }
}  // namespace Menu
