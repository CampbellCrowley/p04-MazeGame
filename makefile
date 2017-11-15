OBJS = MazeMaster

p1: main.cpp CampbellLib/CampbellLib.cc CampbellLib/CampbellLib.h Maze.h Maze.cpp
	g++ -g main.cpp -lncurses -o MazeMaster

run:
	./MazeMaster

clean:
	\rm -f MazeMaster lastsession.dat
