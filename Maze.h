#ifndef MAZE_H
#define MAZE_H
#include <string>
#include <vector>
#include "CampbellLib/CampbellLib.h"
using namespace Campbell::Color;

// Data that defines the maze and the characters to read from a file.
enum TileData {
  EMPTY = 'E',
  WALL = 'W',
  START = 'S',
  END = 'F',
  CURRENT = '^',
  PREVIOUS = '.',
  UNKNOWN = '\0'
};
enum Direction { UP, DOWN, LEFT, RIGHT, NONE, EXIT };
typedef std::vector<std::vector<TileData> > Maze;

class MazeController {
 public:
  MazeController() {
    TileSymbols::EMPTY = TileSymbolsColor::EMPTY + TileSymbols::EMPTY + reset;
    TileSymbols::WALL = TileSymbolsColor::WALL + TileSymbols::WALL + reset;
    TileSymbols::START = TileSymbolsColor::START + TileSymbols::START + reset;
    TileSymbols::END = TileSymbolsColor::END + TileSymbols::END + reset;
    TileSymbols::CURRENT =
        TileSymbolsColor::CURRENT + TileSymbols::CURRENT + reset;
    TileSymbols::PREVIOUS =
        TileSymbolsColor::PREVIOUS + TileSymbols::PREVIOUS + reset;
    TileSymbols::UNKNOWN =
        TileSymbolsColor::UNKNOWN + TileSymbols::UNKNOWN + reset;
  }

  // Loads new maze from file. Returns if this succeeded.
  bool import(const char* filename);
  // Prints maze to screen.
  void print(unsigned int cols, unsigned int rows) const;
  // If the player is in the maze exit.
  bool isComplete() const;
  // Move the player in a direction. Returns if the player actually moved.
  bool move(Direction dir);

  // Maze dimensions
  unsigned int width() const { return maze.size() > 0 ? maze[0].size() : 0; }
  unsigned int height() const { return maze.size(); }

  // Symbols that are shown on the screen. Different from TileData to allow for
  // color or different symbols than those used in the files.
  class TileSymbols {
   public:
    static std::string EMPTY;
    static std::string WALL;
    static std::string START;
    static std::string END;
    static std::string CURRENT;
    static std::string PREVIOUS;
    static std::string UNKNOWN;
  };
  class TileSymbolsColor {
   public:
    static const char *EMPTY;
    static const char *WALL;
    static const char *START;
    static const char *END;
    static const char *CURRENT;
    static const char *PREVIOUS;
    static const char *UNKNOWN;
  };

 private:
  // Current coordinates of player from top left of grid.
  unsigned int current_x, current_y;
  // The loaded maze.
  Maze maze;
  // Converts a character to known TileData.
  TileData charToTile(const char input) const;
  // Converts a TileData to symbol to display.
  const std::string tileToSymbol(const TileData &input) const;

};  // class MazeController

#include "Maze.cpp"
#endif  // ifndef MAZE_H
