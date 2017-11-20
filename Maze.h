#ifndef MAZE_H
#define MAZE_H
#include <curses.h>
#include <vector>

namespace Maze {
// Data that defines the maze and the characters to read from a file.
enum TileData {
  EMPTY = 'E',
  WALL = 'W',
  START = 'S',
  END = 'F',
  CURRENT = '^',
  PREVIOUS = '_',
  UNKNOWN = '\0',
  FRONTIER = 'T',
  HINT = 'H'
};
// Available movement directions and general controls.
enum Direction { UP, DOWN, LEFT, RIGHT, NONE, EXIT, HELP, SOLVE };
// Make typing a little easier.
typedef std::vector<std::vector<TileData> > Maze;
// File path to saves directory.
const char *lastSessionFilename = "saves/lastsession.dat";
const char *completeTitle =  // Maze Complete!
    "                                    :::   :::       :::     ::::::::: "
    "::::::::::                     \n                                  :+:+: "
    ":+:+:    :+: :+:        :+:  :+:                             \n           "
    "                     +:+ +:+:+ +:+  +:+   +:+      +:+   +:+              "
    "                \n                               +#+  +:+  +#+ "
    "+#++:++#++:    +#+    +#++:++#                          \n                "
    "              +#+       +#+ +#+     +#+   +#+     +#+                     "
    "           \n                             #+#       #+# #+#     #+#  #+#  "
    "    #+#                                 \n                            ### "
    "      ### ###     ### ######### ##########                           \n   "
    "   ::::::::   ::::::::    :::   :::   :::::::::  :::        :::::::::: "
    "::::::::::: :::::::::: ::: \n    :+:    :+: :+:    :+:  :+:+: :+:+:  :+:  "
    "  :+: :+:        :+:            :+:     :+:        :+:  \n   +:+        "
    "+:+    +:+ +:+ +:+:+ +:+ +:+    +:+ +:+        +:+            +:+     +:+ "
    "       +:+   \n  +#+        +#+    +:+ +#+  +:+  +#+ +#++:++#+  +#+       "
    " +#++:++#       +#+     +#++:++#   +#+    \n +#+        +#+    +#+ +#+    "
    "   +#+ +#+        +#+        +#+            +#+     +#+        +#+     "
    "\n#+#    #+# #+#    #+# #+#       #+# #+#        #+#        #+#           "
    " #+#     #+#                 \n########   ########  ###       ### ###     "
    "   ########## ##########     ###     ########## ###       \n";

class MazeController {
 public:
  MazeController() {
    isSolutionValid = false;
    isWinOpen_ = false;
    lastCols = 0;
    lastRows = 0;
  }
  ~MazeController() { endWin(); }

  // Possibly save, then exit the game.
  void Exit(bool shouldSave = false);
  // Takes over control flow and only returns once the user has requested exit.
  void play(int generateRows = -1, int generateCols = -1);

  // Loads new maze from file. Returns if this succeeded.
  bool load(const char *filename);
  // Import given maze.
  void load(const Maze &newMaze) { maze = newMaze; }

  // Saves current maze to file. Returns if this succeeded.
  bool save(const char *filename);
  // Return the current maze instance.
  Maze &getMaze() { return maze; }

  // Computes all solutions for the maze and fills solution with the map. Uses
  // Dead-end Filling.
  void solve() { solve(maze, solution); }
  void solve(const Maze &maze, Maze &solution);
  // Generates a maze using a Randomized Prim's Algorithm of the given
  // dimensions.
  void generate(unsigned int rows, unsigned int cols) {
    generate(rows, cols, maze);
  }
  void generate(unsigned int rows, unsigned int cols, Maze &maze);
  // Prints maze to screen.
  void print(int cols = 0, int rows = 0) { print(cols, rows, maze); }
  void print(int cols, int rows, const Maze &maze);
  // If the player is in the maze exit.
  bool isComplete() const;
  // If the NCurses window is open.
  bool isWinOpen() const { return isWinOpen_; }
  // Move the player in a direction. Returns if the player actually moved.
  bool move(Direction dir, bool godMode = false);

  // Invalidate the solution so we don't use the current solution if the maze
  // changes.
  void invalidateSolution() {
    isSolutionValid = false;
    solution.clear();
  }

  // Maze dimensions
  unsigned int height() const { return height(maze); }
  unsigned int width() const { return width(maze); }
  template <typename T>
  unsigned int height(const std::vector<T> &maze) const {
    return maze.size();
  }
  template <typename T>
  unsigned int width(const std::vector<std::vector<T> > &maze) const {
    return (height(maze) > 0 ? maze[0].size() : 0);
  }

  // Symbols that are shown on the screen. Different from TileData to allow for
  // color or different symbols than those used in the files.
  struct TileSymbols {
    static const char EMPTY;
    static const char WALL;
    static const char START;
    static const char END;
    static const char CURRENT;
    static const char PREVIOUS;
    static const char UNKNOWN;
    static const char HINT;
  };
  struct TileSymbolsColor {
    enum Color {
      EMPTY = 1,
      WALL,
      START,
      END,
      CURRENT,
      PREVIOUS,
      UNKNOWN,
      HINT
    };
  };

protected:
  // Starts curses window.
  void startWin();
  // Ends curses window.
  void endWin();

  // Checks if the TileData is walkable.
  bool isEmpty(const TileData tile) const {
    return (tile == EMPTY || tile == START || tile == END || tile == PREVIOUS ||
            tile == HINT);
  }
  // Converts a character to known TileData.
  static TileData charToTile(const char &input);
  // Converts a TileData to symbol to display.
  static char tileToSymbol(const TileData &input);

  // Converts a TileData to color and sets color to screen.
  static void setColor(const TileData &input);
  static void unsetColor(const TileData &input);

  // Gets user input and converts that to a direction.
  Direction getMoveDirection() const;

 private:
  // Picks a random empty tile around a coordinate (x,y).
  std::vector<int> pickRandomNeighbor(const std::vector<int> &coords) const;
  // Sets surrounding tiles around coord to FRONTIER and adds them to the
  // frontier array.
  void addFontierCells(const std::vector<int> coords,
                       std::vector<std::vector<int> > &frontierArray);
  // Sets tiles inclusively between the two coords to EMPTY.
  void mergeNeighborAndFrontier(const std::vector<int> &frontier,
                                const std::vector<int> &neighbor);

  // Current coordinates of player from top left of grid.
  unsigned int current_x, current_y;
  // The loaded maze.
  Maze maze;
  // The solution to the loaded maze.
  Maze solution;
  // Whether the solution is still correct for this maze.
  bool isSolutionValid;
  // The previously inputted screen dimensions.
  unsigned int lastCols;
  unsigned int lastRows;
  // If the NCurses window is open.
  bool isWinOpen_;

};  // class MazeController
}  // namespace Maze

#include "Maze.cpp"
#endif  // ifndef MAZE_H
