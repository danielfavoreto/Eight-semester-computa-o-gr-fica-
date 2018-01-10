all: circle.o player.o utils.o bullet.o 
	g++ -o trabalhocg trabalhocg.cpp tinyxml2.cpp utils.o player.o bullet.o circle.o -lGL -lglut -lGLU -g -ggdb

utils.o: utils.cpp utils.h
	g++ -c utils.cpp

player.o: player.cpp player.h
	g++ -c player.cpp

bullet.o: bullet.cpp bullet.h
	g++ -c bullet.cpp

circle.o: circle.cpp circle.h
	g++ -c circle.cpp

clean:
	rm *.o
