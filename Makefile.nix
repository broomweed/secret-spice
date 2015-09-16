CC=g++
CFLAGS=-lsfml-graphics -lsfml-window -lsfml-system -std=c++11

window_open: window_open.o Scene.o SceneManager.o
	$(CC) window_open.o Scene.o SceneManager.o -g -o window_open $(CFLAGS)

window_open.o: window_open.cpp TextBox.hpp MenuTextBox.hpp DialogueTextBox.hpp TileMap.hpp GameObject.hpp Scene.hpp Animation.hpp SpriteSheet.hpp Character.hpp Thing.hpp Door.hpp Dialogue.hpp SceneManager.o BorderStyle.hpp Font.hpp Building.hpp
	$(CC) -g -c window_open.cpp $(CFLAGS)

SceneManager.o: SceneManager.cpp SceneManager.hpp
	$(CC) -g -c SceneManager.cpp $(CFLAGS)

Scene.o: Scene.cpp Scene.hpp
	$(CC) -g -c Scene.cpp $(CFLAGS)

min: window_open_min.o
	$(CC) window_open_min.o -g -o window_open_min $(CFLAGS)

window_open_min.o: window_open_min.cpp TextBox.hpp TileMap.hpp GameObject.hpp Scene.hpp Animation.hpp SpriteSheetMin.hpp CharacterMin.hpp
	$(CC) -g -c window_open_min.cpp $(CFLAGS)

clean:
	rm *.o
