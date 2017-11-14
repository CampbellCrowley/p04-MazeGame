#include "Maze.h"
#include <fstream>
#include <iostream>
#include "CampbellLib/CampbellLib.h"

using namespace Campbell::Color;
std::string MazeController::TileSymbols::EMPTY = " ";
std::string MazeController::TileSymbols::WALL = "#";
std::string MazeController::TileSymbols::START = "@";
std::string MazeController::TileSymbols::END = "!";
std::string MazeController::TileSymbols::CURRENT = "^";
std::string MazeController::TileSymbols::PREVIOUS = ".";
std::string MazeController::TileSymbols::UNKNOWN = "\0";
const char *MazeController::TileSymbolsColor::EMPTY = black;
const char *MazeController::TileSymbolsColor::WALL = white;
const char *MazeController::TileSymbolsColor::START = blue;
const char *MazeController::TileSymbolsColor::END = red;
const char *MazeController::TileSymbolsColor::CURRENT = green;
const char *MazeController::TileSymbolsColor::PREVIOUS = blue;
const char *MazeController::TileSymbolsColor::UNKNOWN = black;

bool MazeController::import(const char *filename) {
  // TODO: Open the file...
  std::fstream file(filename, std::ios::in);
  if (!file.fail()) {
    std::string line;
    maze.clear();
    bool firstStart = true;
    while(file >> line) {
      maze.resize(maze.size() + 1);
      for (int i = 0; i < (int)line.size(); ++i) {
        TileData newTile = charToTile(line[i]);
        maze[maze.size() - 1].push_back(newTile);
        if (newTile == START) {
          if (!firstStart) {
            std::cerr << red
                      << "There are multiple starting locations. There may "
                         "be only one!"
                      << reset << std::endl;
          } else {
            current_x = maze.size() - 1;
            current_y = maze[0].size() - 1;
            firstStart = false;
          }
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
  if (maze.size() > cols) {
    start_x = current_x + cols / 2;
  }
  if (maze[0].size() > rows) {
    start_y = current_y + rows / 2;
  }

  for (unsigned int i = start_y; i < rows && i < maze.size(); ++i) {
    for (unsigned int j = start_x; j < cols && j < maze[i].size(); ++j) {
      if (i - start_y == current_y && j - start_x == current_x) {
        std::cout << tileToSymbol(CURRENT);
      } else {
        std::cout << tileToSymbol(maze[i][j]);
      }
    }
    std::cout << std::endl;
  }
}

bool MazeController::isComplete() const {
  return maze[current_x][current_y] == END;
}

bool MazeController::move(Direction dir) {
  switch(dir) {
    // TODO: Check if able to move in direction first.
    case UP:
      if (current_y >= maze.size() - 1 ||
          maze[current_y + 1][current_x] == WALL) {
        return false;
      } else {
        maze[current_y][current_x] = PREVIOUS;
        current_y++;
        return true;
      }
    case DOWN:
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
      if (current_x >= maze[current_y].size() - 1 ||
          maze[current_y][current_x - 1] == WALL) {
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

TileData MazeController::charToTile(const char input) const {
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

const std::string MazeController::tileToSymbol(const TileData &input) const {
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
