#ifndef MAZE_H
#define MAZE_H
#include <vector>

enum TileData {
  EMPTY = ' ',
  WALL = '#',
  START = '@',
  END = '!',
  CURRENT = '^',
  PREVIOUS = '.'
};
enum Direction { UP, DOWN, LEFT, RIGHT, NONE };
typedef std::vector<std::vector<TileData> > Maze;

class MazeController {
 public:
  MazeController() {}

  // Loads new maze from file. Returns if this succeeded.
  bool import(const char* filename);
  // Prints maze to screen.
  void print(unsigned int cols, unsigned int rows);
  // If the player is in the maze exit.
  bool isComplete();
  // Move the player in a direction. Returns if the player actually moved.
  bool move(Direction dir);

 private:
  // Current coordinates of player.
  unsigned int current_x, current_y;
  // The loaded maze.
  Maze maze;
};  // class MazeController

#endif  // ifndef MAZE_H
