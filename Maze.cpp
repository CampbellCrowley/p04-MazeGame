#include "Maze.h"
#include <iostream>
#include <fstream>

bool MazeController::import(const char *filename) {
  // TODO: Open the file...
  std::fstream file(filename, std::ios::in);
  file.close();
  return false;
}

void MazeController::print(unsigned int cols, unsigned int rows) {
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
      std::cout << maze[i][j];
    }
    std::cout << std::endl;
  }
}

bool MazeController::isComplete() { return maze[current_x][current_y] == END; }

bool MazeController::move(Direction dir) {
  switch(dir) {
    // TODO: Check if able to move in direction first.
    case UP:
      current_y++;
      return true;
      break;
    case DOWN:
      current_y--;
      return true;
      break;
    case LEFT:
      current_x++;
      return true;
      break;
    case RIGHT:
      current_x--;
      return true;
      break;
    case NONE:
    default:
      return false;
      break;
  }
}
