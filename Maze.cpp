#include "Maze.h"
#include <curses.h>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <unistd.h>

const char MazeController::TileSymbols::EMPTY = '$';
const char MazeController::TileSymbols::WALL = '#';
const char MazeController::TileSymbols::START = '@';
const char MazeController::TileSymbols::END = '!';
const char MazeController::TileSymbols::CURRENT = '^';
const char MazeController::TileSymbols::PREVIOUS = '.';
const char MazeController::TileSymbols::UNKNOWN = 'E';
const unsigned char MazeController::TileSymbolsColor::EMPTY = 1;
const unsigned char MazeController::TileSymbolsColor::WALL = 2;
const unsigned char MazeController::TileSymbolsColor::START = 3;
const unsigned char MazeController::TileSymbolsColor::END = 4;
const unsigned char MazeController::TileSymbolsColor::CURRENT = 5;
const unsigned char MazeController::TileSymbolsColor::PREVIOUS = 6;
const unsigned char MazeController::TileSymbolsColor::UNKNOWN = 7;

bool MazeController::import(const char *filename) {
  std::fstream file(filename, std::ios::in);
  if (!file.fail()) {
    std::string line;
    maze.clear();
    wmove(stdscr, 20, 0);
    while (getline(file, line)) {
      if (line[0] == '#' || line[0] / sizeof(line) == 0) continue;
      maze.resize(height() + 1);
      for (int i = 0; i < (int)line.size(); ++i) {
        TileData newTile = charToTile(line[i]);
        maze[height() - 1].push_back(newTile);
        if (newTile == START) {
          current_x = i;
          current_y = height() - 1;
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

void MazeController::print(int cols, int rows) {
  if (cols <= 0) cols = lastCols;
  if (rows <= 0) rows = lastRows;
  lastCols = cols;
  lastRows = rows;
  int start_x = 0;
  int start_y = 0;
  if (height() > (unsigned)rows) {
    start_y = current_y - rows / 2;
    if (start_y < 0) start_y = 0;
    if (start_y > (signed)height() - rows) start_y = height() - rows;
  }
  if (width() > (unsigned)cols) {
    start_x = current_x - cols / 2;
    if (start_x < 0) start_x = 0;
    if (start_x > (signed)width() - cols) start_x = width() - cols;
  }

  wmove(stdscr, 0, 0);
  for (unsigned int i = start_y; i - start_y < (unsigned)rows -1&& i < height();
       ++i) {
    for (unsigned int j = start_x; j - start_x < (unsigned)cols-1 && j < width();
         ++j) {
      if (i == current_y && j == current_x) {
        setColor(CURRENT);
        waddch(stdscr, tileToSymbol(CURRENT));
        unsetColor(CURRENT);
      } else {
        setColor(maze[i][j]);
        waddch(stdscr, tileToSymbol(maze[i][j]));
        unsetColor(maze[i][j]);
      }
    }
    waddch(stdscr, '\n');
  }
  if (has_colors()) wattron(stdscr, A_NORMAL);
  wrefresh(stdscr);
}

bool MazeController::isComplete() const {
  if (current_x >= width() || current_y >= height()) return false;
  return maze[current_y][current_x] == END;
}

bool MazeController::move(Direction dir) {
  switch(dir) {
    case DOWN:
      if (current_y >= height() - 1 || maze[current_y + 1][current_x] == WALL) {
        return false;
      } else {
        if (maze[current_y][current_x] == EMPTY)
          maze[current_y][current_x] = PREVIOUS;
        current_y++;
        return true;
      }
    case UP:
      if (current_y <= 0 || maze[current_y - 1][current_x] == WALL) {
        return false;
      } else {
        if (maze[current_y][current_x] == EMPTY)
          maze[current_y][current_x] = PREVIOUS;
        current_y--;
        return true;
      }
    case LEFT:
      if (current_x <= 0 || maze[current_y][current_x - 1] == WALL) {
        return false;
      } else {
        if (maze[current_y][current_x] == EMPTY)
          maze[current_y][current_x] = PREVIOUS;
        current_x--;
        return true;
      }
    case RIGHT:
      if (current_x >= width() - 1 || maze[current_y][current_x + 1] == WALL) {
        return false;
      } else {
        if (maze[current_y][current_x] == EMPTY)
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
    case FRONTIER:
      return FRONTIER;
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
void MazeController::unsetColor(const TileData &input) {
  if (!has_colors()) return;
  switch (input) {
    case EMPTY:
      wattroff(stdscr, COLOR_PAIR(TileSymbolsColor::EMPTY));
      break;
    case WALL:
      wattroff(stdscr, COLOR_PAIR(TileSymbolsColor::WALL));
      break;
    case START:
      wattroff(stdscr, COLOR_PAIR(TileSymbolsColor::START));
      break;
    case END:
      wattroff(stdscr, COLOR_PAIR(TileSymbolsColor::END));
      break;
    case CURRENT:
      wattroff(stdscr, COLOR_PAIR(TileSymbolsColor::CURRENT));
      break;
    case PREVIOUS:
      wattroff(stdscr, COLOR_PAIR(TileSymbolsColor::PREVIOUS));
      break;
    default:
      wattroff(stdscr, COLOR_PAIR(TileSymbolsColor::UNKNOWN));
      break;
  }
}

void MazeController::generate(unsigned int rows, unsigned int cols) {
  maze.clear();
  maze = std::vector<std::vector<TileData> >(
      rows, std::vector<TileData>(cols, UNKNOWN));

  srand(time(NULL));

  const unsigned int start = rand() % cols;
  maze[0][start] = START;
  current_x = start;
  current_y = 0;
  std::vector<std::vector<int> > frontierTiles;
  {
    std::vector<int> coord = std::vector<int>(2);
    coord[0] = 0;
    coord[1] = start;
    addFontierCells(coord, frontierTiles);
  }

  while (frontierTiles.size() > 0) {
    int selectedFrontierCell = rand() % frontierTiles.size();
    std::vector<int> selectedNeighborCell =
        pickRandomNeighbor(frontierTiles[selectedFrontierCell]);
    if (!selectedNeighborCell.empty()) {
      mergeNeighborAndFrontier(frontierTiles[selectedFrontierCell],
                               selectedNeighborCell);
      addFontierCells(frontierTiles[selectedFrontierCell], frontierTiles);
    }
    frontierTiles.erase(frontierTiles.begin() + selectedFrontierCell);
    print();
  }
  for (unsigned int i = 0; i < height(); ++i) {
    for (unsigned int j = 0; j < width(); ++j) {
      if (maze[i][j] == UNKNOWN) {
        maze[i][j] = WALL;
      }
    }
  }
  bool endDetermined = false;
  for (int j = rows - 1; j >= 0 && !endDetermined; --j) {
    for (unsigned int i = 0; i < cols; ++i) {
      int end = rand() % cols;
      if (maze[j][end] == EMPTY) {
        maze[j][end] = END;
        endDetermined = true;
        break;
      }
    }
  }
}

std::vector<int> MazeController::pickRandomNeighbor(
    std::vector<int> coords) const {
  std::vector<int> selection;
  bool left = false, right = false, up = false, down = false;
  TileData mazeCoord;
  while (selection.size() == 0 && (!up || !down || !left || !right)) {
    int direction = rand() % 4;
    switch(direction) {
      case 0: // UP
        if (!up && coords[0] - 2 >= 0) {
          mazeCoord = maze[coords[0] - 2][coords[1]];
          if (mazeCoord == EMPTY || mazeCoord == START || mazeCoord == END) {
            selection.push_back(coords[0] - 2);
            selection.push_back(coords[1]);
            return selection;
          }
        }
        up = true;
        break;
      case 1: // DOWN
        if (!down && (unsigned int)coords[0] + 2 < height()) {
          mazeCoord = maze[coords[0] + 2][coords[1]];
          if (mazeCoord == EMPTY || mazeCoord == START || mazeCoord == END) {
            selection.push_back(coords[0] + 2);
            selection.push_back(coords[1]);
            return selection;
          }
        }
        down = true;
        break;
      case 2: // RIGHT
        if (!right && (unsigned int)coords[1] + 2 < width()) {
          mazeCoord = maze[coords[0]][coords[1] + 2];
          if (mazeCoord == EMPTY || mazeCoord == START || mazeCoord == END) {
            selection.push_back(coords[0]);
            selection.push_back(coords[1] + 2);
            return selection;
          }
        }
        right = true;
        break;
      case 3: // LEFT
        if (!left && coords[1] - 2 >= 0) {
          mazeCoord = maze[coords[0]][coords[1] - 2];
          if (mazeCoord == EMPTY || mazeCoord == START || mazeCoord == END) {
            selection.push_back(coords[0]);
            selection.push_back(coords[1] - 2);
            return selection;
          }
        }
        left = true;
        break;
    }
  }
  return selection;
}
void MazeController::addFontierCells(const std::vector<int> coords,
                                     std::vector<std::vector<int> > &input) {
  std::vector<int> newCoord = std::vector<int>(2);
  if (coords[0] - 2 >= 0 && maze[coords[0] - 2][coords[1]] == UNKNOWN) {
    maze[coords[0] - 2][coords[1]] = FRONTIER;
    newCoord[0] = coords[0] - 2;
    newCoord[1] = coords[1];
    input.push_back(newCoord);
  }
  if ((unsigned int)coords[0] + 2 < width() &&
      maze[coords[0] + 2][coords[1]] == UNKNOWN) {
    maze[coords[0] + 2][coords[1]] = FRONTIER;
    newCoord[0] = coords[0] + 2;
    newCoord[1] = coords[1];
    input.push_back(newCoord);
  }
  if ((unsigned int)coords[1] + 2 < height() &&
      maze[coords[0]][coords[1] + 2] == UNKNOWN) {
    maze[coords[0]][coords[1] + 2] = FRONTIER;
    newCoord[0] = coords[0];
    newCoord[1] = coords[1] + 2;
    input.push_back(newCoord);
  }
  if (coords[1] - 2 >= 0 && maze[coords[0]][coords[1] - 2] == UNKNOWN) {
    maze[coords[0]][coords[1] - 2] = FRONTIER;
    newCoord[0] = coords[0];
    newCoord[1] = coords[1] - 2;
    input.push_back(newCoord);
  }
}

void MazeController::mergeNeighborAndFrontier(std::vector<int> frontier,
                                              std::vector<int> neighbor) {
  if (neighbor[0] == frontier[0]) {
    if (neighbor[1] > frontier[1]) {
      for (int i = 0; i <= neighbor[1] - frontier[1]; ++i) {
        if (!isEmpty(maze[frontier[0]][frontier[1] + i])) {
          maze[frontier[0]][frontier[1] + i] = EMPTY;
        }
      }
    } else {
      for (int i = 0; i >= neighbor[1] - frontier[1]; --i) {
        if (!isEmpty(maze[frontier[0]][frontier[1] + i])) {
          maze[frontier[0]][frontier[1] + i] = EMPTY;
        }
      }
    }
  } else if (neighbor[1] == frontier[1]) {
    if (neighbor[0] > frontier[0]) {
      for (int i = 0; i <= neighbor[0] - frontier[0]; ++i) {
        if (!isEmpty(maze[frontier[0] + i][frontier[1]])) {
          maze[frontier[0] + i][frontier[1]] = EMPTY;
        }
      }
    } else {
      for (int i = 0; i >= neighbor[0] - frontier[0]; --i) {
        if (!isEmpty(maze[frontier[0] + i][frontier[1]])) {
          maze[frontier[0] + i][frontier[1]] = EMPTY;
        }
      }
    }
  }
}
