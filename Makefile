WINDOWS_MAKE=mingw32-make
UNIX_MAKE=make

win: window_open.cpp TextBox.hpp TileMap.hpp GameObject.hpp Scene.hpp Animation.hpp SpriteSheet.hpp Character.hpp Thing.hpp Door.hpp Dialogue.hpp
	$(WINDOWS_MAKE) -f Makefile.win

nix: window_open.cpp TextBox.hpp TileMap.hpp GameObject.hpp Scene.hpp Animation.hpp SpriteSheet.hpp Character.hpp Thing.hpp Door.hpp Dialogue.hpp
	$(UNIX_MAKE) -f Makefile.nix
