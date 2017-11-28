OBJS = MazeMaster

p1: main.cpp CampbellLib/CampbellLib.cc CampbellLib/CampbellLib.h Maze.h Maze.cpp Menu.h Menu.cpp Game.h Game.cpp Titles.h Callbacks.h Callbacks.cpp
	g++ main.cpp -g -lncurses -o MazeMaster Maze.cpp Menu.cpp Game.cpp Callbacks.cpp

run:
	./MazeMaster

clean:
	\rm -f MazeMaster saves/lastsession.dat
