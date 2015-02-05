CC=g++
CFLAGS=-lsfml-graphics -lsfml-window -lsfml-system

window_open: window_open.o
	$(CC) window_open.o -g -o window_open $(CFLAGS)

window_open.o: window_open.cpp TextBox.hpp TileMap.hpp GameObject.hpp Scene.hpp
	$(CC) -g -c window_open.cpp $(CFLAGS)
