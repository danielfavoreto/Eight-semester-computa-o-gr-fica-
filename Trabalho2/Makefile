all: utils.o
	g++ -o trabalhocg trabalhocg.cpp tinyxml2.cpp utils.o -lGL -lglut -lGLU

utils.o: utils.cpp utils.hpp
	g++ -c utils.cpp

clean:
	rm *.o
