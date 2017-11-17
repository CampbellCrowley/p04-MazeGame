OBJS = MazeMaster

p1: main.cpp CampbellLib/CampbellLib.cc CampbellLib/CampbellLib.h Maze.h Maze.cpp Menu.h Menu.cpp
	g++ -g main.cpp -lncurses -o MazeMaster

run:
	./MazeMaster

clean:
	\rm -f MazeMaster saves/lastsession.dat
