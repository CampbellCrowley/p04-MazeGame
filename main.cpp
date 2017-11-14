// MUST COMPILE WITH "-lncurses" flag!
// Example: "g++ -lncurses main.cpp"
#include <curses.h>
#include <signal.h>
#include "CampbellLib/CampbellLib.h"
#include "Maze.h"

using namespace std;

// Global vars
unsigned int width = 0;
unsigned int height = 0;
MazeController mc;

// Prototypes
Direction getMoveDirection();

// Definitions
// Called before app exits in order to properly release terminal from NCurses.
void ExitApp() {
  endwin();
  printf("ENDWIN\n");
  exit(0);
}
// Catches Ctrl+C in order to call ExitApp properly.
void interruptHandler(int s) {
  (void)s;
  ExitApp();
}
// Reprints maze if it doesn't fit in screen anymore.
void resizeHandler(int sig) {
  (void)sig;
  getmaxyx(stdscr, height, width);
  mc.print(height, width);
}
// Entry
int main() {
  // Handle execution interrupt.
  struct sigaction sigIntHandler;
  sigIntHandler.sa_handler = interruptHandler;
  sigemptyset(&sigIntHandler.sa_mask);
  sigIntHandler.sa_flags = 0;
  sigaction(SIGINT, &sigIntHandler, NULL);

  // Handle window resize
  signal(SIGWINCH, &resizeHandler);

  // Handle NCurses.
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  // Colors
  start_color();
  init_pair(MazeController::TileSymbolsColor::EMPTY, COLOR_BLACK, COLOR_BLACK);
  init_pair(MazeController::TileSymbolsColor::WALL, COLOR_WHITE, COLOR_BLACK);
  init_pair(MazeController::TileSymbolsColor::START, COLOR_GREEN, COLOR_BLACK);
  init_pair(MazeController::TileSymbolsColor::END, COLOR_RED, COLOR_BLACK);
  init_pair(MazeController::TileSymbolsColor::CURRENT, COLOR_MAGENTA, COLOR_BLACK);
  init_pair(MazeController::TileSymbolsColor::PREVIOUS, COLOR_BLUE, COLOR_BLACK);
  init_pair(MazeController::TileSymbolsColor::UNKNOWN, COLOR_BLACK, COLOR_BLACK);

  // Get starting screensize
  getmaxyx(stdscr, height, width);

  mc.import("maze1.dat");

  // Play game
  Direction nextDirection = NONE;
  while (!mc.isComplete() && nextDirection != EXIT) {
    mc.print(height, width);
    nextDirection = getMoveDirection();
    mc.move(nextDirection);
  }

  ExitApp();
  return 0;
}  // int main()

Direction getMoveDirection() {
  wchar_t input;
  while ((input = getch()) == ERR) {}
  mvwprintw(stdscr, 20, 0, "Input: %c ", input);
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
      return EXIT;
    default:
      return NONE;
  }
  return NONE;
}
