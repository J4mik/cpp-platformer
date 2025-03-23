all: compile link

compile:
	g++ -Iinclude -c main.cpp src/*.cpp

link:
	g++ *.o -o main -Llib -lmingw32 -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lSDL2_test