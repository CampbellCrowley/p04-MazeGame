# Table of Contents
- [Compiling](#compiling)
- [Input/Output](#inputoutput)
- [Control Flow](#control-flow)
- [Iteration](#iteration)
- [Data Structure](#data-structure)
- [Function](#function)
- [File I/O](#file-io)
- [Class](#class)

---

## Compiling
MUST COMPILE USING `-lncurses`!  
Example: `g++ -lncurses main.cpp`  
**or**  
Use make to automatically build and run **(Preferred)**:  
- Build: `make`  
- Run: `make run`

Only tested on Linux using C++ (pre-11) compiled using g++ 4.9.2.

## Input/Output
[main.cpp lines 160-185](/main.cpp)  
Uses `cout` and `Campbell::Strings::getYesNo()` to promt the user for their preference of what they wish to do with previous session data. `Cerr` is also used in the case that something goes wrong.

[Maze.cpp lines 159-204, 599-631](/Maze.cpp)  
Prints the maze to the NCurses screen via `waddch()`. User input is received via keypresses to NCurses screen via `getch()` with the `cbreak` and `noecho` options set to prevent characters from being displayed on the screen.

[Menu.cpp lines 17-72, 98-130, 192-219](/Menu.cpp)  
Prints the menu to the NCurses screen via `addstr()`, and `addch()`. Input is received two ways depending on the situation:
  1) Via `getch()` with `cbreak` and `noecho` set, for directional input and choosing options in a list.
  2) Via `getch()` with `nocbreak` and `echo` set, for inputting user typed strings in this case for typing a filename.

[CampbellLib.cc lines 79-83](/CampbellLib/CampbellLib.cc)  
Input is received using `getline()` in cconjunction with `cin` to ensure the whole line is inputted and no trailing characters are left to create unwanted behaviour in the future.

## Control Flow
[main.cpp lines 161-182](/main.cpp)  
Control flow is modified based on the user inputs and state of the filesystem.

[Maze.cpp lines 43-292](/Maze.cpp)  
Throughout this area is where most of the game logic takes place using switch, if, else if, and else statements; while and for loops; as well as `break` and `return`.

[Menu.cpp lines 4-181](/Menu.cpp)  
This section contains most of the menu control logic and manages user input while in a menu.

## Iteration
[main.cpp lines 236-307](/main.cpp)  
Different arrays, matrices, and strings are iterated through to modify or parse data. The maze matrix is iterated through to purge data, a directory's data is iterated through to create an array of available filenames, which are then in turn iterated through to add to a menu.

[Maze.cpp lines 43-597](/Maze.cpp)  
The maze and solution matrices are iterated through for different purposes. To print to the screen, to generate the maze, solve the maze, saving to a file, reading from a file, and parsing the read data into a useable format.

[Menu.cpp lines 17-72](/Menu.cpp)  
The menu array is iterated through in order to print each option to the screen.

[CampellLib/CampbellLib.cc lines 10-61](/CampbellLib/CampbellLib.cc)  
Strings are iterated over to check if it could be converted to a number then to actually convert the string to a number.

## Data Structure
[main.cpp lines 31-78](/main.cpp)  
Titles are stored in c-style strings. This is because they can be optimized by the compiler, they also will allocate memory before `int main()` is called and will reduce calls to malloc during runtime when menus are loaded.

[Maze.h lines 8-185](/Maze.h)  
A maze is a matrix of TileData which is an enum of different possible tiles in the maze. This is because a maze is a 2D grid of data and can be represented as either `EMPTY` or a `WALL`. Other values are for generating the maze, file I/O, and determining the start and end of the maze. The symbols displayed on the screen are a struct of chars to store each character in an easily accessible unit. Possible input options are also stored in a enum.

[Menu.h lines 8-178](/Menu.h)  
Option is a structure for storing relevant data for any option that may be on the menu. This is then stored in a vector as the menu only supports a list of options, not a grid, and will also be expandable for any number or type of options.

## Function
[main.cpp lines 80-337](/main.cpp)  
Most menu options and callbacks are defined here.

[Maze.cpp lines 19-630](/Maze.cpp)  
[Maze.h lines 59-184](/Maze.h)  
Maze control, generation, solving, fault-checking, helper-functions, file I/O, and user I/O is managed here.

[Menu.cpp lines 4-227](/Menu.cpp)  
[Menu.h lines 18-178](/Menu.h)  
Menu control, helper-functions, user I/O and option generating is managed here.

[CampellLib/CampbellLib.cc lines 5-83](/CampbellLib/CampbellLib.cc)  
[CampellLib/CampbellLib.h lines 9-96](/CampbellLib/CampbellLib.h)  
A collection of helper-functions that I use often and found helpful across multiple projects.

## File IO
[main.cpp lines 160-176, 276-306](/main.cpp)  
`saves/lastsession.dat` is checked to see if it exists. The `saves/` directory is statted to list all files in the load menu.

[Maze.cpp lines 96-157](/Maze.cpp)  
The maze is converted to a file as well as parsed from a file here in order to save session state or load user's choice of maze.

## Class
[Maze.h lines 57-184](/Maze.h)  
The `Maze::MazeController` class is declared here. Stores everything a maze would need to be self-contained.

[Menu.h lines 18-178](/Menu.h)  
Everything pertaining to a self-contained menu is declared here in `Menu::MenuController`.
