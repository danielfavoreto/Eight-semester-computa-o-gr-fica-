all: utils.o player.o bullet.o
	g++ -o trabalhocg trabalhocg.cpp tinyxml2.cpp utils.o player.o bullet.o -lGL -lglut -lGLU -g -ggdb

utils.o: utils.cpp utils.hpp
	g++ -c utils.cpp

player.o: player.cpp player.h
	g++ -c player.cpp

bullet.o: bullet.cpp bullet.h
	g++ -c bullet.cpp

clean:
	rm *.o
