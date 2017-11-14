#include "Maze.h"
#include <curses.h>
#include <fstream>

const char MazeController::TileSymbols::EMPTY = ' ';
const char MazeController::TileSymbols::WALL = '#';
const char MazeController::TileSymbols::START = '@';
const char MazeController::TileSymbols::END = '!';
const char MazeController::TileSymbols::CURRENT = '^';
const char MazeController::TileSymbols::PREVIOUS = '.';
const char MazeController::TileSymbols::UNKNOWN = '\0';
const unsigned char MazeController::TileSymbolsColor::EMPTY = 0;
const unsigned char MazeController::TileSymbolsColor::WALL = 1;
const unsigned char MazeController::TileSymbolsColor::START = 2;
const unsigned char MazeController::TileSymbolsColor::END = 3;
const unsigned char MazeController::TileSymbolsColor::CURRENT = 4;
const unsigned char MazeController::TileSymbolsColor::PREVIOUS = 5;
const unsigned char MazeController::TileSymbolsColor::UNKNOWN = 6;

bool MazeController::import(const char *filename) {
  std::fstream file(filename, std::ios::in);
  if (!file.fail()) {
    std::string line;
    maze.clear();
    while(file >> line) {
      maze.resize(height() + 1);
      for (int i = 0; i < (int)line.size(); ++i) {
        TileData newTile = charToTile(line[i]);
        maze[height() - 1].push_back(newTile);
        if (newTile == START) {
          current_x = i;
          current_y = height() - 1;
          mvwprintw(stdscr, 21, 0, "Start: X: %d, Y: %d\n", current_x, current_y);
        }
      }
    }
    file.close();
    return true;
  } else {
    file.close();
    return false;
  }
}

void MazeController::print(unsigned int cols, unsigned int rows) const {
  unsigned int start_x = 0;
  unsigned int start_y = 0;
  if (height() > cols) {
    start_x = current_x + cols / 2;
  }
  if (width() > rows) {
    start_y = current_y + rows / 2;
  }

  wmove(stdscr, 0, 0);
  for (unsigned int i = start_y; i < rows && i < height(); ++i) {
    for (unsigned int j = start_x; j < cols && j < width(); ++j) {
      if (i == current_y && j == current_x) {
        setColor(CURRENT);
        waddch(stdscr, tileToSymbol(CURRENT));
      } else {
        setColor(maze[i][j]);
        waddch(stdscr, tileToSymbol(maze[i][j]));
      }
    }
    waddch(stdscr, '\n');
  }
  if (has_colors()) wattron(stdscr, A_NORMAL);
  wrefresh(stdscr);
  mvwprintw(stdscr, 22, 0, "Now: X: %d, Y: %d\n", current_x, current_y);
}

bool MazeController::isComplete() const {
  if (current_x >= width() || current_y >= height()) return false;
  return maze[current_y][current_x] == END;
}

bool MazeController::move(Direction dir) {
  switch(dir) {
    // TODO: Check if able to move in direction first.
    case DOWN:
      if (current_y >= height() - 1 || maze[current_y + 1][current_x] == WALL) {
        return false;
      } else {
        maze[current_y][current_x] = PREVIOUS;
        current_y++;
        return true;
      }
    case UP:
      if (current_y <= 0 || maze[current_y - 1][current_x] == WALL) {
        return false;
      } else {
        maze[current_y][current_x] = PREVIOUS;
        current_y--;
        return true;
      }
    case LEFT:
      if (current_x <= 0 || maze[current_y][current_x - 1] == WALL) {
        return false;
      } else {
        maze[current_y][current_x] = PREVIOUS;
        current_x--;
        return true;
      }
    case RIGHT:
      if (current_x >= width() - 1 || maze[current_y][current_x + 1] == WALL) {
        return false;
      } else {
        maze[current_y][current_x] = PREVIOUS;
        current_x++;
        return true;
      }
    case NONE:
    default:
      return false;
      break;
  }
}

TileData MazeController::charToTile(const char input) {
  switch (input) {
    case EMPTY:
      return EMPTY;
    case WALL:
      return WALL;
    case START:
      return START;
    case END:
      return END;
    case CURRENT:
      return CURRENT;
    case PREVIOUS:
      return PREVIOUS;
    default:
      return UNKNOWN;
  }
}
char MazeController::tileToSymbol(const TileData &input) {
  switch (input) {
    case EMPTY:
      return TileSymbols::EMPTY;
    case WALL:
      return TileSymbols::WALL;
    case START:
      return TileSymbols::START;
    case END:
      return TileSymbols::END;
    case CURRENT:
      return TileSymbols::CURRENT;
    case PREVIOUS:
      return TileSymbols::PREVIOUS;
    default:
      return TileSymbols::UNKNOWN;
  }
}
void MazeController::setColor(const TileData &input) {
  if (!has_colors()) return;
  switch (input) {
    case EMPTY:
      wattron(stdscr, COLOR_PAIR(TileSymbolsColor::EMPTY));
      break;
    case WALL:
      wattron(stdscr, COLOR_PAIR(TileSymbolsColor::WALL));
      break;
    case START:
      wattron(stdscr, COLOR_PAIR(TileSymbolsColor::START));
      break;
    case END:
      wattron(stdscr, COLOR_PAIR(TileSymbolsColor::END));
      break;
    case CURRENT:
      wattron(stdscr, COLOR_PAIR(TileSymbolsColor::CURRENT));
      break;
    case PREVIOUS:
      wattron(stdscr, COLOR_PAIR(TileSymbolsColor::PREVIOUS));
      break;
    default:
      wattron(stdscr, COLOR_PAIR(TileSymbolsColor::UNKNOWN));
      break;
  }
}
