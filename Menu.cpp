#include "Menu.h"
#include <curses.h>
namespace Menu {
void Menu::startMenu() { openMenu(); }
void Menu::openMenu() { startWin(); }
void Menu::closeMenu() { endWin(); }
void Menu::startWin() {
  // Handle NCurses.
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  // Colors
  start_color();
  init_pair(10, COLOR_GREEN, COLOR_BLACK);
  isWinOpen_ = true;
}
void Menu::endWin() {
  endwin();
  isWinOpen_ = false;
}
void Menu::addMazeOptions() {
  addOption(Option("Easy", &Menu::playEasy, true, true));
  addOption(Option("Medium", &Menu::playMedium));
  addOption(Option("Hard", &Menu::playHard));
  addOption(Option("Custom", &Menu::playCustom));
  addOption(Option("Save Maze", &Menu::saveButton));
  addOption(Option("Load Maze", &Menu::loadButton));
}
Input Menu::getInput() {
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
    default:
      return NONE;
  }
}
}  // namespace Menu
