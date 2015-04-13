#include <iostream>
#include <cstdio>
#include <SFML/Graphics.hpp>
#include "Thing.hpp"
#include "TextBox.hpp"
#include "TileMap.hpp"
#include "GameObject.hpp"
#include "Character.hpp"
#include "Scene.hpp"
#include "SpriteSheet.hpp"
#define SCRWIDTH 800
#define SCRHEIGHT 600

using std::string;

int main(int argc, char **argv) {
    sf::RenderWindow window(sf::VideoMode(SCRWIDTH, SCRHEIGHT), "My favorite window");
    // DANG THIS IS THE BEST THING:
    sf::View camera(sf::FloatRect(0, 0, SCRWIDTH/2, SCRHEIGHT/2));
    window.setView(camera);
    sf::Texture tiles;
    Scene scene;

    const int level[] = {
        0,0,1,2,0,0,0,0,3,4,5,6,7,0,0,0,0,0,0,0,0,0,0,0,0,8,0,0,0,0,0,0,0,0,8,9,10,11,12,0,0,0,1,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,4,5,6,7,0,0,0,0,0,1,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8,9,10,11,12,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,4,0,0,0,0,0,0,0,0,0,0,0,3,4,5,6,7,0,0,0,0,0,1,2,0,8,9,0,0,0,0,0,0,0,0,0,0,0,8,9,10,11,12,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,13,14,15,16,17,0,0,0,0,1,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8,9,10,11,12,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,4,5,6,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,8,9,10,11,12
    };

    bool walkable[18] = {
        true, true, true, true, true, true,
        true, true, true, true, true, true,
        true, true, true, true, true, true,
    };

    TileMap tilemap;
    if (!tilemap.load("images/tests/trees2/tiles.png", sf::Vector2u(16, 16), level, 26, 20)) {
        fprintf(stderr, "something went wrong with the tileset!\n");
        return -1;
    }

    TypewriterTextBox textbox(sf::Rect<int>(5, 5, 200, 17), 18.0f);
    int charWidths[41] =
       //a b c d e f g h i j k l m n o p q r s t u v w x y z 0 1 2 3 4 5 6 7 8 9 ! ? . , space
        {5,5,5,5,5,5,5,5,4,4,5,5,6,5,5,5,5,5,5,6,5,6,6,6,6,5,5,4,5,5,5,5,5,5,5,5,2,5,2,2,2};

    if (!textbox.setFont(string("images/font.png"), 5, charWidths,
            string("abcdefghijklmnopqrstuvwxyz0123456789!?., "))) {
        fprintf(stderr, "Something went wrong with the font!\n");
        return -2;
    }

    textbox.setText(string("lorem ipsum dolor sit amet. my name is miles. i should really get some lowercase letters."));

    bool arrows[4] = { false, false, false, false };

    SpriteSheet dan("images/dan.png", 3, 8);
    dan.getSprite(1,1);

    Character guy(dan, sf::Vector2f(0, 16), sf::Rect<float>(5, 24, 14, 8), 1);

    if (!tiles.loadFromFile("images/tilemap.png")) {
        fprintf(stderr, "something went wrong (2)!\n");
    }
    tiles.setSmooth(false);

    Character npc1(dan, sf::Vector2f(100, 150), sf::Rect<float>(5, 24, 14, 8), 3);
    Character npc2(dan, sf::Vector2f(200, 24), sf::Rect<float>(5, 24, 14, 8), 2);

    scene.add(&guy);
    scene.add(&npc1);
    scene.add(&npc2);

    GameObject tree("images/tests/tree.png", sf::Vector2f(0, 0), sf::Rect<float>(12, 66, 39, 12), 4);

    scene.add_static(&tree, sf::Vector2f(150, 10));
    scene.add_static(&tree, sf::Vector2f(38, 90));
    scene.add_static(&tree, sf::Vector2f(198, 170));
    scene.add_static(&tree, sf::Vector2f(38, 218));
    scene.add_static(&tree, sf::Vector2f(358, 298));

    window.setVerticalSyncEnabled(true);

    while (window.isOpen()) {
        sf::Event event;
        while(window.pollEvent(event)) {
            // if the event was a window close, close window. makes sense.
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed) {
                switch(event.key.code) {
                    case sf::Keyboard::Up:
                        arrows[0] = true;
                        break;
                    case sf::Keyboard::Left:
                        arrows[1] = true;
                        break;
                    case sf::Keyboard::Down:
                        arrows[2] = true;
                        break;
                    case sf::Keyboard::Right:
                        arrows[3] = true;
                        break;
                }
            }
            if (event.type == sf::Event::KeyReleased) {
                switch(event.key.code) {
                    case sf::Keyboard::Up:
                        arrows[0] = false;
                        break;
                    case sf::Keyboard::Left:
                        arrows[1] = false;
                        break;
                    case sf::Keyboard::Down:
                        arrows[2] = false;
                        break;
                    case sf::Keyboard::Right:
                        arrows[3] = false;
                        break;
                    case sf::Keyboard::X:
                        if (textbox.hidden) {
                            textbox.show();
                        } else {
                            if (textbox.finished) {
                                textbox.hide();
                            }
                        }
                        break;
                }
            }

        }

        float vmove = 0.0;
        float hmove = 0.0;
        if (arrows[0]) {
            if (arrows[1] || arrows[3]) {
                vmove -= 0.71; // sin(45)
            } else {
                vmove -= 1.0;
            }
        }
        if (arrows[1]) {
            if (arrows[0] || arrows[2]) {
                hmove -= 0.71;
            } else {
                hmove -= 1.0;
            }
        }
        if (arrows[2]) {
            if (arrows[1] || arrows[3]) {
                vmove += 0.71;
            } else {
                vmove += 1.0;
            }
        }
        if (arrows[3]) {
            if (arrows[0] || arrows[2]) {
                hmove += 0.71;
            } else {
                hmove += 1.0;
            }
        }
        // basically check if they can move to a certain point in the direction. It checks the 2 corners
        // on each side that they are moving.
        if (hmove > 0) {
            // RIGHT
            if (!walkable[tilemap.tile_id_at_point(sf::Vector2f(
                            guy.getPosition().x + guy.absLoc.width + hmove,
                            guy.getPosition().y + 1))] // if we don't put the +1/-1 here, it checks the
                                                          // exact corners and won't let you fit through a
                                                          // 1-tile-wide space
                    || !walkable[tilemap.tile_id_at_point(sf::Vector2f(
                            guy.getPosition().x + guy.absLoc.width + hmove,
                            guy.getPosition().y + guy.absLoc.height - 1))]) {
                hmove = 0.0;
            }
        } else if (hmove < 0) {
            // LEFT
            if (!walkable[tilemap.tile_id_at_point(sf::Vector2f(
                            guy.getPosition().x + hmove,
                            guy.getPosition().y + 1))]
                    || !walkable[tilemap.tile_id_at_point(sf::Vector2f(
                            guy.getPosition().x + hmove,
                            guy.getPosition().y + guy.absLoc.height - 1))]) {
                hmove = 0.0;
            }
        }
        if (vmove > 0) {
            // DOWN
            if (!walkable[tilemap.tile_id_at_point(sf::Vector2f(
                            guy.getPosition().x + 1,
                            guy.getPosition().y + guy.absLoc.height + vmove))]
                    || !walkable[tilemap.tile_id_at_point(sf::Vector2f(
                            guy.getPosition().x + guy.absLoc.width - 1,
                            guy.getPosition().y + guy.absLoc.height + vmove))]) {
                vmove = 0.0;
            }
        } else if (vmove < 0) {
            // UP
            if (!walkable[tilemap.tile_id_at_point(sf::Vector2f(
                            guy.getPosition().x + 1,
                            guy.getPosition().y + vmove))]
                    || !walkable[tilemap.tile_id_at_point(sf::Vector2f(
                            guy.getPosition().x + guy.absLoc.width - 1,
                            guy.getPosition().y + vmove))]) {
                vmove = 0.0;
            }
        }

        guy.setSpeed(hmove, vmove);

        textbox.update();
        scene.update();
        float cameraX, cameraY;
        if (guy.getPosition().x < SCRWIDTH/4) {
            cameraX = SCRWIDTH/4;
        } else if (guy.getPosition().x > tilemap.width * tilemap.tile_size.x - SCRWIDTH/4) {
            cameraX = tilemap.width * tilemap.tile_size.x - SCRWIDTH/4;
        } else {
            cameraX = guy.getPosition().x;
        }
        if (guy.getPosition().y < SCRHEIGHT/4) {
            cameraY = SCRHEIGHT/4;
        } else if (guy.getPosition().y > tilemap.height * tilemap.tile_size.y - SCRHEIGHT/4) {
            cameraY = tilemap.height * tilemap.tile_size.y - SCRHEIGHT/4;
        } else {
            cameraY = guy.getPosition().y;
        }
        camera.setCenter(cameraX, cameraY);
        window.setView(camera);

        window.clear(sf::Color::Black);
        window.draw(tilemap);
        window.draw(scene);
        window.draw(textbox);
        window.display();

    }

    return 0;
}
