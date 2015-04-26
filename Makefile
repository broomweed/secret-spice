CC=g++
CFLAGS=-lsfml-graphics -lsfml-window -lsfml-system

window_open: window_open.o
	$(CC) window_open.o -g -o window_open $(CFLAGS)

window_open.o: window_open.cpp TextBox.hpp TileMap.hpp GameObject.hpp Scene.hpp Animation.hpp SpriteSheet.hpp Character.hpp Thing.hpp Door.hpp
	$(CC) -g -c window_open.cpp $(CFLAGS)

min: window_open_min.o
	$(CC) window_open_min.o -g -o window_open_min $(CFLAGS)

window_open_min.o: window_open_min.cpp TextBox.hpp TileMap.hpp GameObject.hpp Scene.hpp Animation.hpp SpriteSheetMin.hpp CharacterMin.hpp
	$(CC) -g -c window_open_min.cpp $(CFLAGS)

clean:
	rm *.o
