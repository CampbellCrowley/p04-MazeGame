#include <curses.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <iostream>
#include <vector>
#include "CampbellLib/CampbellLib.h"
#include "Maze.h"

using namespace std;

// Global Vars
struct winsize size;

// Prototypes
Direction getMoveDirection();
void OnExit(int s) {
  endwin();
  cout << s << endl;
  exit(0);
}

// Entry
int main() {
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);

  // Handle execution interrupt.
  struct sigaction sigIntHandler;
  sigIntHandler.sa_handler = OnExit;
  sigemptyset(&sigIntHandler.sa_mask);
  sigIntHandler.sa_flags = 0;
  sigaction(SIGINT, &sigIntHandler, NULL);

  // Handle keyboard inputs.
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);

  cout << "Width: " << size.ws_col << " Height: " << size.ws_row << endl;

  MazeController mc;
  mc.import("maze1.dat");
  cout << "Width: " << mc.width() << " Height: " << mc.height() << endl;

  Direction nextDirection = NONE;
  while (!mc.isComplete() && nextDirection != EXIT) {
    mc.print(size.ws_col, size.ws_row);
    nextDirection = getMoveDirection();
    mc.move(nextDirection);
  }

  return 0;
}  // int main()

Direction getMoveDirection() {
  int input;
  while ((input = getch()) == ERR) {
    switch(input) {
      case KEY_DOWN:
      case 'J':
        return DOWN;
      case KEY_UP:
      case 'K':
        return UP;
      case KEY_LEFT:
      case 'H':
        return LEFT;
      case KEY_RIGHT:
      case 'L':
        return RIGHT;
      case 'Q':
        return EXIT;
      default:
        return NONE;
    }
  }
  return NONE;
}
