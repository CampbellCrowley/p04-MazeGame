OBJS = data/MazeMaster

p1: main.cpp include/CampbellLib/CampbellLib.cc include/CampbellLib/CampbellLib.h include/Maze.h include/Maze.cpp include/Menu.h include/Menu.cpp include/Game.h include/Game.cpp include/Titles.h include/Callbacks.h include/Callbacks.cpp
	g++ main.cpp -lncurses -o data/MazeMaster include/Maze.cpp include/Menu.cpp include/Game.cpp include/Callbacks.cpp

run: data/MazeMaster
	./data/MazeMaster

clean:
	\rm -f data/MazeMaster saves/lastsession.dat
