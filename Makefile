WINDOWS_MAKE=mingw32-make
UNIX_MAKE=make

win: window_open.cpp TextBox.hpp MenuTextBox.hpp DialogueTextBox.hpp TileMap.hpp GameObject.hpp Scene.hpp Animation.hpp SpriteSheet.hpp Character.hpp Thing.hpp Door.hpp Dialogue.hpp Font.hpp
	$(WINDOWS_MAKE) -f Makefile.win

nix: window_open.cpp TextBox.hpp MenuTextBox.hpp DialogueTextBox.hpp TileMap.hpp GameObject.hpp Scene.hpp Animation.hpp SpriteSheet.hpp Character.hpp Thing.hpp Door.hpp Dialogue.hpp Font.hpp
	$(UNIX_MAKE) -f Makefile.nix
