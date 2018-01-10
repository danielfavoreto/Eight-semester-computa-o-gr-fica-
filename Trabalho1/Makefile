all: tinyXml.o
	g++ -o trabalhocg trabalhocg.cpp tinyXml.o -lGL -lglut -lGLU

tinyXml.o: tinyXml.cpp tinyXml.hpp
	g++ -c tinyXml.cpp 

clean:
	rm *.o
