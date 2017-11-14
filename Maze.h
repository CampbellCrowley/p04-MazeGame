#ifndef MAZE_H
#define MAZE_H
#include <vector>

// Data that defines the maze and the characters to read from a file.
enum TileData {
  EMPTY = 'E',
  WALL = 'W',
  START = 'S',
  END = 'F',
  CURRENT = '^',
  PREVIOUS = '.',
  UNKNOWN = '\0',
  FRONTIER = 'T'
};
enum Direction { UP, DOWN, LEFT, RIGHT, NONE, EXIT };
typedef std::vector<std::vector<TileData> > Maze;

class MazeController {
 public:
  MazeController() {}

  // Loads new maze from file. Returns if this succeeded.
  bool import(const char* filename);
  // Generates a maze using a Randomized Prim's Algorithm of the given
  // dimensions.
  void generate(unsigned int rows, unsigned int cols);
  // Prints maze to screen.
  void print(int cols = 0, int rows = 0);
  // If the player is in the maze exit.
  bool isComplete() const;
  // Move the player in a direction. Returns if the player actually moved.
  bool move(Direction dir);

  // Converts a character to known TileData.
  static TileData charToTile(const char input);
  // Converts a TileData to symbol to display.
  static char tileToSymbol(const TileData &input);
  // Converts a TileData to color.
  static void setColor(const TileData &input);
  static void unsetColor(const TileData &input);

  // Maze dimensions
  unsigned int height() const { return maze.size(); }
  unsigned int width() const { return (height() > 0 ? maze[0].size() : 0); }

  // Symbols that are shown on the screen. Different from TileData to allow for
  // color or different symbols than those used in the files.
  class TileSymbols {
   public:
    static const char EMPTY;
    static const char WALL;
    static const char START;
    static const char END;
    static const char CURRENT;
    static const char PREVIOUS;
    static const char UNKNOWN;
  };
  class TileSymbolsColor {
   public:
    static const unsigned char EMPTY;
    static const unsigned char WALL;
    static const unsigned char START;
    static const unsigned char END;
    static const unsigned char CURRENT;
    static const unsigned char PREVIOUS;
    static const unsigned char UNKNOWN;
  };

 protected:
  bool isEmpty(TileData &tile) {
    return (tile == EMPTY || tile == START || tile == END || tile == PREVIOUS);
  }

 private:
  // Current coordinates of player from top left of grid.
  unsigned int current_x, current_y;
  // The loaded maze.
  Maze maze;
  std::vector<int> pickRandomNeighbor(std::vector<int> coords) const;
  void addFontierCells(const std::vector<int> coords,
                       std::vector<std::vector<int> > &input);
  void mergeNeighborAndFrontier(std::vector<int> frontier,
                                std::vector<int> neighbor);
  unsigned int lastCols;
  unsigned int lastRows;

};  // class MazeController

#include "Maze.cpp"
#endif  // ifndef MAZE_H
