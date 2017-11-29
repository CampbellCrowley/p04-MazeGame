#include "Maze.h"
#include <curses.h>
#include <unistd.h>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include "Titles.h"

namespace Maze {
// File path to saves directory.
const char MazeController::TileSymbols::EMPTY = '$';
const char MazeController::TileSymbols::WALL = '#';
const char MazeController::TileSymbols::START = '@';
const char MazeController::TileSymbols::END = '!';
const char MazeController::TileSymbols::CURRENT = '^';
const char MazeController::TileSymbols::PREVIOUS = '.';
const char MazeController::TileSymbols::UNKNOWN = 'E';
const char MazeController::TileSymbols::HINT = 'H';

void MazeController::startWin() {
  // Handle NCurses.
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  // Colors
  start_color();
  init_pair(10, COLOR_GREEN, COLOR_BLACK);
  init_pair(EMPTY_COLOR, COLOR_BLACK, COLOR_BLACK);
  init_pair(WALL_COLOR, COLOR_RED, COLOR_RED);
  init_pair(START_COLOR, COLOR_GREEN, COLOR_GREEN);
  init_pair(END_COLOR, COLOR_YELLOW, COLOR_YELLOW);
  init_pair(CURRENT_COLOR, COLOR_GREEN, COLOR_GREEN);
  init_pair(PREVIOUS_COLOR, COLOR_BLUE, COLOR_BLUE);
  init_pair(UNKNOWN_COLOR, COLOR_RED, COLOR_BLACK);
  init_pair(HINT_COLOR, COLOR_WHITE, COLOR_WHITE);
  isWinOpen_ = true;
}
void MazeController::endWin() {
  endwin();
  isWinOpen_ = false;
}

void MazeController::play(int generateRows, int generateCols) {
  unsigned int height, width;
  getmaxyx(stdscr, height, width);
  startWin();
  clear();
  print(width, height);

  clear();
  if (generateRows > 0 && generateCols > 0) {
    generate(generateRows, generateCols);
  } else if (MazeController::width() == 0) {
    Exit(false);
  }

  Direction nextDirection = NONE;
  getmaxyx(stdscr, height, width);
  clear();
  print(width, height);
  bool justFinished = true;
  while (nextDirection != EXIT && isWinOpen()) {
    nextDirection = getMoveDirection();
    getmaxyx(stdscr, height, width);
    if (move(nextDirection, !justFinished)) {
      print(width, height);
    } else if (lastCols != width || lastRows != height) {
      print(width, height);
    }
    if (isComplete() && justFinished) {
      justFinished = false;
      move(SOLVE);
      print(width, height);
      usleep(100000);
      wattron(stdscr, COLOR_PAIR(10));
      ::move(height / 2 - 10, 0);
      addstr(Titles::completeTitle);
      addstr("Press 'Q' to quit\nMovement is unlocked.\n");
      wattroff(stdscr, COLOR_PAIR(10));
      refresh();
    }
    lastCols = width;
    lastRows = height;
  }

  Exit(justFinished);
}
void MazeController::Exit(bool shouldSave) {
  if (shouldSave) save(lastSessionFilename);
  else remove(lastSessionFilename);
  erase();
  clear();
  endWin();
}

bool MazeController::load(const char *filename) {
  std::fstream file(filename, std::ios::in);
  if (!file.fail()) {
    std::string line;
    maze.clear();
    invalidateSolution();
    bool hasCurrent = false;
    while (getline(file, line)) {
      if (line[0] == '#' || line[0] / sizeof(line) == 0) continue;
      maze.resize(height() + 1);
      for (int i = 0; i < (int)line.size(); ++i) {
        TileData newTile = charToTile(line[i]);
        if (!hasCurrent && newTile == START) {
          current_x = i;
          current_y = height() - 1;
        } else if (newTile == CURRENT) {
          current_x = i;
          current_y = height() - 1;
          newTile = EMPTY;
        }
        maze[height() - 1].push_back(newTile);
      }
    }
    file.close();
    return true;
  } else {
    file.close();
    return false;
  }
}
bool MazeController::save(const char *filename) {
  std::fstream file(filename, std::ios::out | std::ios::trunc);
  if (!file.fail()) {
    file << "# This file was created automatically by Maze Master.\n";
    file << "# Empty: " << (char)EMPTY << "\n";
    file << "# Wall: " << (char)WALL << "\n";
    file << "# Start: " << (char)START << "\n";
    file << "# Finish: " << (char)END << "\n";
    file << "# Current: " << (char)CURRENT << "\n";
    file << "# Previous: " << (char)PREVIOUS << "\n";
    file << "# UNKNOWN: " << (char)UNKNOWN << "\n";
    file << "# Frontier: " << (char)FRONTIER << "\n";
    file << "# Hint: " << (char)HINT << "\n";

    for (unsigned int i = 0; i < maze.size(); ++i) {
      for (unsigned int j = 0; j < maze[i].size(); ++j) {
        if ((maze[i][j] == EMPTY || maze[i][j] == PREVIOUS) && i == current_y &&
            j == current_x) {
          file << (char)CURRENT;
        } else {
          file << (char)maze[i][j];
        }
      }
      file << '\n';
    }
    file.close();
    return true;
  } else {
    file.close();
    return false;
  }
}

void MazeController::print(int cols, int rows, const Maze &maze) {
  if (cols == 0 && rows == 0) getmaxyx(stdscr, rows, cols);
  if (cols <= 0) cols = lastCols;
  if (rows <= 0) rows = lastRows;
  lastCols = cols;
  lastRows = rows;
  int start_x = 0;
  int start_y = 0;
  if (height() > (unsigned)rows) {
    start_y = current_y - rows / 2;
    if (start_y < 0) start_y = 0;
    if (start_y > (signed)height() - rows) start_y = height() - rows + 1;
  }
  if (width() > (unsigned)cols) {
    start_x = current_x - cols / 2;
    if (start_x < 0) start_x = 0;
    if (start_x > (signed)width() - cols) start_x = width() - cols + 1;
  }

  wmove(stdscr, 0, 0);
  for (unsigned int i = start_y;
       i - start_y < (unsigned)rows - 1 && i < height(); ++i) {
    setColor(WALL);
    waddch(stdscr, tileToSymbol(WALL));
    unsetColor(WALL);
    for (unsigned int j = start_x;
         j - start_x < (unsigned)cols - 3 && j < width(); ++j) {
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
    setColor(WALL);
    waddch(stdscr, tileToSymbol(WALL));
    unsetColor(WALL);
    waddch(stdscr, '\n');
  }
  if (has_colors()) wattron(stdscr, A_NORMAL);
  ::move(0, 0);
  wrefresh(stdscr);
}

bool MazeController::isComplete() const {
  if (current_x >= width() || current_y >= height()) return false;
  return maze[current_y][current_x] == END;
}

void MazeController::resetPosition() {
  for (unsigned int i = 0; i < height(); ++i) {
    for (unsigned int j = 0; j < width(); ++j) {
      if (maze[i][j] == START) {
        current_x = j;
        current_y = i;
        return;
      }
    }
  }
}

bool MazeController::move(Direction dir, bool godMode) {
  switch(dir) {
    case DOWN:
      if (current_y >= height() - 1 ||
          (!godMode && maze[current_y + 1][current_x] == WALL)) {
        return false;
      } else {
        if (!godMode && maze[current_y][current_x] == EMPTY)
          maze[current_y][current_x] = PREVIOUS;
        current_y++;
        return true;
      }
    case UP:
      if (current_y <= 0 ||
          (!godMode && maze[current_y - 1][current_x] == WALL)) {
        return false;
      } else {
        if (!godMode && maze[current_y][current_x] == EMPTY)
          maze[current_y][current_x] = PREVIOUS;
        current_y--;
        return true;
      }
    case LEFT:
      if (current_x <= 0 ||
          (!godMode && maze[current_y][current_x - 1] == WALL)) {
        return false;
      } else {
        if (!godMode && maze[current_y][current_x] == EMPTY)
          maze[current_y][current_x] = PREVIOUS;
        current_x--;
        return true;
      }
    case RIGHT:
      if (current_x >= width() - 1 ||
          (!godMode && maze[current_y][current_x + 1] == WALL)) {
        return false;
      } else {
        if (!godMode && maze[current_y][current_x] == EMPTY)
          maze[current_y][current_x] = PREVIOUS;
        current_x++;
        return true;
      }
    case HELP:
      solve();
      for (unsigned int i = 0; i < height(); ++i) {
        for (unsigned int j = 0; j < width(); ++j) {
          if ((maze[i][j] == solution[i][j] && solution[i][j] == PREVIOUS) ||
              (maze[i][j] == PREVIOUS && solution[i][j] == EMPTY)) {
            maze[i][j] = HINT;
          }
        }
      }
      return true;
    case SOLVE:
      solve();
      for (unsigned int i = 0; i < height(); ++i) {
        for (unsigned int j = 0; j < width(); ++j) {
          if ((maze[i][j] == solution[i][j] &&
               (solution[i][j] == PREVIOUS || solution[i][j] == EMPTY)) ||
              (maze[i][j] == PREVIOUS && solution[i][j] == EMPTY)) {
            maze[i][j] = HINT;
          }
        }
      }
      return true;
    case NONE:
    default:
      return false;
  }
}

TileData MazeController::charToTile(const char &input) {
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
    case HINT:
      return HINT;
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
    case HINT:
      return TileSymbols::HINT;
    default:
      return TileSymbols::UNKNOWN;
  }
}
void MazeController::setColor(const TileData &input) {
  if (!has_colors()) return;
  switch (input) {
    case EMPTY:
      wattron(stdscr, COLOR_PAIR(EMPTY_COLOR));
      break;
    case WALL:
      wattron(stdscr, COLOR_PAIR(WALL_COLOR));
      break;
    case START:
      wattron(stdscr, COLOR_PAIR(START_COLOR));
      break;
    case END:
      wattron(stdscr, COLOR_PAIR(END_COLOR));
      break;
    case CURRENT:
      wattron(stdscr, COLOR_PAIR(CURRENT_COLOR));
      wattron(stdscr, A_BLINK);
      break;
    case PREVIOUS:
      wattron(stdscr, COLOR_PAIR(PREVIOUS_COLOR));
      break;
    case HINT:
      wattron(stdscr, COLOR_PAIR(HINT_COLOR));
      break;
    default:
      wattron(stdscr, COLOR_PAIR(UNKNOWN_COLOR));
      break;
  }
}
void MazeController::unsetColor(const TileData &input) {
  if (!has_colors()) return;
  switch (input) {
    case EMPTY:
      wattroff(stdscr, COLOR_PAIR(EMPTY_COLOR));
      break;
    case WALL:
      wattroff(stdscr, COLOR_PAIR(WALL_COLOR));
      break;
    case START:
      wattroff(stdscr, COLOR_PAIR(START_COLOR));
      break;
    case END:
      wattroff(stdscr, COLOR_PAIR(END_COLOR));
      break;
    case CURRENT:
      wattroff(stdscr, COLOR_PAIR(CURRENT_COLOR));
      wattroff(stdscr, A_BLINK);
      break;
    case PREVIOUS:
      wattroff(stdscr, COLOR_PAIR(PREVIOUS_COLOR));
      break;
    case HINT:
      wattroff(stdscr, COLOR_PAIR(HINT_COLOR));
      break;
    default:
      wattroff(stdscr, COLOR_PAIR(UNKNOWN_COLOR));
      break;
  }
}

void MazeController::generate(unsigned int rows, unsigned int cols,
                              Maze &maze) {
  load("data/loading.dat");
  print();

  maze.clear();
  invalidateSolution();
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
    const std::vector<int> &coords) const {
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
void MazeController::addFontierCells(
    const std::vector<int> coords,
    std::vector<std::vector<int> > &frontierArray) {
  if (coords.size() != 2 || width() == 0) return;
  std::vector<int> newCoord = std::vector<int>(2);
  if (coords[0] - 2 >= 0 && maze[coords[0] - 2][coords[1]] == UNKNOWN) {
    maze[coords[0] - 2][coords[1]] = FRONTIER;
    newCoord[0] = coords[0] - 2;
    newCoord[1] = coords[1];
    frontierArray.push_back(newCoord);
  }
  if (coords[1] - 2 >= 0 && maze[coords[0]][coords[1] - 2] == UNKNOWN) {
    maze[coords[0]][coords[1] - 2] = FRONTIER;
    newCoord[0] = coords[0];
    newCoord[1] = coords[1] - 2;
    frontierArray.push_back(newCoord);
  }
  if ((unsigned)coords[0] + 2 < height() &&
      maze[coords[0] + 2][coords[1]] == UNKNOWN) {
    maze[coords[0] + 2][coords[1]] = FRONTIER;
    newCoord[0] = coords[0] + 2;
    newCoord[1] = coords[1];
    frontierArray.push_back(newCoord);
  }
  if ((unsigned)coords[1] + 2 < width() &&
      maze[coords[0]][coords[1] + 2] == UNKNOWN) {
    maze[coords[0]][coords[1] + 2] = FRONTIER;
    newCoord[0] = coords[0];
    newCoord[1] = coords[1] + 2;
    frontierArray.push_back(newCoord);
  }
}

void MazeController::mergeNeighborAndFrontier(
    const std::vector<int> &frontier, const std::vector<int> &neighbor) {
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

void MazeController::solve(const Maze &maze, Maze &solution) {
  if (isSolutionValid) return;
  solution = maze;
  bool didSomething = false;
  do {
    didSomething = false;
    for (unsigned int i = 0; i < height(); ++i) {
      for (unsigned int j = 0; j < width(); ++j) {
        if (solution[i][j] == WALL) continue;
        int numSides = 0;
        if (i == 0 || !isEmpty(solution[i - 1][j])) numSides++;
        if (j == 0 || !isEmpty(solution[i][j - 1])) numSides++;
        if (i == height() - 1 || !isEmpty(solution[i + 1][j])) numSides++;
        if (j == width() - 1 || !isEmpty(solution[i][j + 1])) numSides++;
        if (numSides == 3 && solution[i][j] != END && solution[i][j] != START) {
          solution[i][j] = WALL;
          didSomething = true;
        }
      }
    }
  } while (didSomething);
  isSolutionValid = true;
}

Direction MazeController::getMoveDirection() const {
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
        return EXIT;
      case '\'':
      case '\"':
        return HELP;
      case '?':
      case '/':
        return SOLVE;
      default:
        return NONE;
    }
  }
}  // namespace Maze
