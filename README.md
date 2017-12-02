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
**MUST COMPILE USING `-lncurses`!**  
Use make to automatically build and run **(Preferred)**:  
- Build: `make`  
- Run: `make run`  

Manually (without Make):  
- Build: `g++ main.cpp -lncurses -Iinclude -Iinclude/CampbellLib -o data/MazeMaster include/Maze.cpp include/Menu.cpp include/Game.cpp include/Callbacks.cpp include/CampbellLib/CampbellLib.cc`  
- Run: `./data/MazeMaster`  

---

Tested on Linux:  
- Debian 8.9 (jessie amd64) compiled using g++ 4.9.2,  
- Debian 9.2 (stretch amd64) compiled using g++ 6.3.0,  
- Ubuntu 16.04.5 LTS (amd64) compiled using g++ 5.4.0.  

## Input/Output
### [Game.cpp](/include/Game.cpp) lines [72-80](/include/Game.cpp#L72-L80)  
Uses `cout` and `Campbell::Strings::getYesNo()` to promt the user for their preference of what they wish to do with previous session data. `cerr` is also used in the case that something goes wrong.

### [Maze.cpp](/include/Maze.cpp) lines [161-206](/include/Maze.cpp#L161-L206), [601-633](/include/Maze.cpp#L601-L633)  
Prints the maze to the NCurses screen via `waddch()`. User input is received via keypresses to NCurses screen via `getch()` with the `cbreak` and `noecho` options set to prevent characters from being displayed on the screen.

### [Menu.cpp](/include/Menu.cpp) lines [17-87](/include/Menu.cpp#L17-L87), [114-146](/include/Menu.cpp#L114-L146), [221-258](/include/Menu.cpp#L221-L258)  
Prints the menu to the NCurses screen via `addstr()`, and `addch()`. Input is received two ways depending on the situation:
  1) Via `getch()` with `cbreak` and `noecho` set, for directional input and choosing options in a list.
  2) Via `getch()` with `nocbreak` and `echo` set, for inputting user typed strings in this case for typing a filename.

### [CampbellLib.cc](/include/CampbellLib/CampbellLib.cc) lines [79-83](/include/CampbellLib/CampbellLib.cc#L79-L83)  
Input is received using `getline()` in conjunction with `cin` to ensure the whole line is inputted and no trailing characters are left to create unwanted behaviour in the future.

## Control Flow
### [Game.cpp](/include/Game.cpp) lines [71-87](/include/Game.cpp#L71-L87)  
Control flow is modified based on the user inputs and state of the filesystem.

### [Maze.cpp](/include/Maze.cpp) lines [45-89](/include/Maze.cpp#L45-L89)  
Throughout this area is where most of the game logic takes place using switch, if, else if, and else statements; while and for loops; as well as `break` and `return`.

### [Menu.cpp](/include/Menu.cpp) lines [4-219](/include/Menu.cpp#L4-L219)  
This section contains most of the menu control logic and manages user input while in a menu.

### [Callbacks.cpp](/include/Callbacks.cpp) lines [10-153](/include/Callbacks.cpp#L10-L153)  
Here each of the callbacks which are called when menu buttons are pressed control where the program goes next.

## Iteration
### [Maze.cpp](/include/Maze.cpp) lines [45-598](/include/Maze.cpp#L45-L598)  
The maze and solution matrices are iterated through for different purposes. To print to the screen, to generate the maze, solve the maze, saving to a file, reading from a file, and parsing the read data into a useable format.

### [Menu.cpp](/include/Menu.cpp) lines [17-64](/include/Menu.cpp#L17-L64)  
The menu array is iterated through in order to print each option to the screen.

### [Callbacks.cpp](/include/Callbacks.cpp) lines [47-121](/include/Callbacks.cpp#L47-L121)  
Different arrays, matrices, and strings are iterated through to modify or parse data. The maze matrix is iterated through to purge data, a directory's data is iterated through to create an array of available filenames, which are then in turn iterated through to add to a menu.

### [CampbellLib.cc](/include/CampbellLib/CampbellLib.cc) lines [10-61](/include/CampbellLib/CampbellLib.cc#L10-L61)  
Strings are iterated over to check if it could be converted to a number then to actually convert the string to a number.

## Data Structure
### [Titles.h](/include/Titles.h) lines [3-11](/include/Titles.h#L3-L11)  
Titles are stored in c-style strings. This is because they can be optimized by the compiler, they also will allocate memory before `int main()` is called and will reduce calls to malloc during runtime when menus are loaded.

### [Maze.h](/include/Maze.h) lines [7-164](/include/Maze.h#L7-L164)  
A maze is a matrix of TileData which is an enum of different possible tiles in the maze. This is because a maze is a 2D grid of data and can be represented as either `EMPTY` or a `WALL`. Other values are for generating the maze, file I/O, and determining the start and end of the maze. The symbols displayed on the screen are a struct of chars to store each character in an easily accessible unit. Possible input options are also stored in a enum.

### [Menu.h](/include/Menu.h) lines [8-171](/include/Menu.h#L8-L171)  
Option is a structure for storing relevant data for any option that may be on the menu. This is then stored in a vector as the menu only supports a list of options, not a grid, and will also be expandable for any number or type of options.

## Function
### [Callbacks.cpp](/include/Callbacks.cpp) lines [10-153](/include/Callbacks.cpp#L10-L153); [Callbacks.h](/include/Callbacks.h) lines [4-36](/include/Callbacks.h#L4-L36)  
Most menu options and callbacks are defined here.

### [Maze.cpp](/include/Maze.cpp) lines [21-633](/include/Maze.cpp#L21-L633); [Maze.h](/include/Maze.h) lines [32-164](/include/Maze.h#L32-L164)  
Maze control, generation, solving, fault-checking, helper-functions, file I/O, and user I/O is managed here.

### [Menu.cpp](/include/Menu.cpp) lines [4-266](/include/Menu.cpp#L4-L266); [Menu.h](/include/Menu.h) lines [20-170](/include/Menu.h#L20-L170)  
Menu control, helper-functions, user I/O and option generating is managed here.

### [Game.cpp](/include/Game.cpp) lines [15-106](/include/Game.cpp#L15-L106); [Game.h](/include/Game.h) lines [16-59](/include/Game.h#L16-L59)  
Overall manager of menu and maze controllers and overwatch over execution state and control flow.

### [CampbellLib.cc](/include/CampbellLib/CampbellLib.cc) lines [5-83](/include/CampbellLib/CampbellLib.cc#L5-L83); [CampbellLib.h](/include/CampbellLib/CampbellLib.h) lines [9-96](/include/CampbellLib/CampbellLib.h#L9-L96)  
A collection of helper-functions that I use often and found helpful across multiple projects.

## File IO
### [Game.cpp](/include/Game.cpp) lines [69-89](/include/Game.cpp#L69-L89)  
`saves/lastsession.dat` is checked to see if it exists.

### [Maze.cpp](/include/Maze.cpp) lines [98-160](/include/Maze.cpp#L98-L160)  
The maze is converted to a file as well as parsed from a file here in order to save session state or load user's choice of maze.

### [Callbacks.cpp](/include/Callbacks.cpp) lines [89-121](/include/Callbacks.cpp#L89-L121)  
The `saves/` directory is statted to list all files in the load menu.

## Class
### [Titles.h](/include/Titles.h) lines [3-11](/include/Titles.h#L3-L11)  
Titles are congregated here.

### [Maze.h](/include/Maze.h) lines [32-161](/include/Maze.h#L32-L161)  
The `Maze::MazeController` class is declared here. Stores everything a maze would need to be self-contained.

### [Menu.h](/include/Menu.h) lines [20-170](/include/Menu.h#L20-L170)  
Everything pertaining to a self-contained menu is declared here in `Menu::MenuController`.

### [Game.h](/include/Game.h) lines [16-61](/include/Game.h#L16-L61)  
The overarching class that controls the entirety of the program is declared here in `Game::GameController`.

### [Callbacks.h](/include/Callbacks.h) lines [4-36](/include/Callbacks.h#L4-L36)  
All menu button callbacks are declared here in `Callbacks`.
