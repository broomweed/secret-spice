#include <iostream>
#include <cstdio>
#include <SFML/Graphics.hpp>
#include "TextBox.hpp"
#include "TileMap.hpp"
#define SCRWIDTH 800
#define SCRHEIGHT 600

int main(int argc, char **argv) {
    sf::RenderWindow window(sf::VideoMode(SCRWIDTH, SCRHEIGHT), "My favorite window");
    // DANG THIS IS THE BEST THING:
    sf::View camera(sf::FloatRect(0, 0, SCRWIDTH/2, SCRHEIGHT/2));
    window.setView(camera);
    sf::Texture gear;
    sf::Texture tiles;
    sf::Sprite sprite;

    const int level[] = {
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1,
        1, 2, 1, 1, 2, 2, 2, 1, 1, 1, 2, 1, 1, 2, 2, 1,
        1, 2, 1, 1, 2, 1, 2, 1, 2, 9, 7, 7, 8, 3, 2, 1,
        1, 2, 2, 2, 1, 2, 2, 1, 2, 6, 4, 4, 5, 3, 2, 1,
        1, 1, 1, 2, 2, 3, 3, 2, 2, 6, 4, 4, 5, 2, 2, 1,
        1, 1, 1, 2, 2, 3, 3, 3, 3, 6, 4, 4, 5, 3, 3, 2,
        1, 1, 2, 2, 2, 3, 3, 3, 3,11,12,12,10, 3, 2, 2,
        1, 1, 2, 2, 2, 3, 3, 3, 3, 2, 1, 1, 2, 3, 3, 2,
        1, 1, 1, 2, 2, 3, 3, 3, 3, 2, 2, 1, 2, 3, 3, 3,
        1, 1, 1, 2, 2, 3, 3, 3, 3, 3, 2, 2, 3, 3, 3, 3,
        1, 1, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    };

    bool walkable[14] = {
        false,  // 0 : empty
        true,   // 1 : grassland
        true,   // 2 : sand
        false,  // 3 : water
        true,   // 4 : bridge
        false,  // 5 : bridge-right-edge
        false,  // 6 : bridge-left-edge
        true,   // 7 : bridge-top
        true,   // 8 : bridge-top-right-edge
        true,   // 9 : bridge-top-left-edge
        true,   // 10: bridge-bottom-right-edge
        true,   // 11: bridge-bottom-left-edge
        true,   // 12: bridge-bottom
        false,  // 13: reserved
    };

    TileMap tilemap;
    if (!tilemap.load("images/tilemap.png", sf::Vector2u(16, 16), level, 16, 12)) {
        fprintf(stderr, "something went wrong with the tileset!\n");
        return -1;
    }

    TypewriterTextBox textbox(sf::Rect<int>(5, 5, 200, 17), 18.0f);
    int charWidths[41] =
       //a b c d e f g h i j k l m n o p q r s t u v w x y z 0 1 2 3 4 5 6 7 8 9 ! ? . , space
        {5,5,5,5,5,5,5,5,4,4,5,5,6,5,5,5,5,5,5,6,5,6,6,6,6,5,5,4,5,5,5,5,5,5,5,5,2,5,2,2,2};

    if (!textbox.setFont(std::string("images/font.png"), 5, charWidths,
            std::string("abcdefghijklmnopqrstuvwxyz0123456789!?., "))) {
        fprintf(stderr, "Something went wrong with the font!\n");
        return -2;
    }

    textbox.setText(std::string("hello, this is text! it is really long but maybe it will go around to the next line! or not. but it does!"));

    bool arrows[4] = { false, false, false, false };

    if (!gear.loadFromFile("images/gear.png")) {
        fprintf(stderr, "something went wrong!\n");
    }
    gear.setSmooth(false);

    if (!tiles.loadFromFile("images/tilemap.png")) {
        fprintf(stderr, "something went wrong (2)!\n");
    }
    tiles.setSmooth(false);

    sprite.setTexture(gear);
    sprite.setPosition(0.0f, 0.0f);

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

        float vmove = 0.0f;
        float hmove = 0.0f;
        if (arrows[0]) {
            if (arrows[1] || arrows[3]) {
                vmove -= 0.71f;
            } else {
                vmove -= 1.0f;
            }
        }
        if (arrows[1]) {
            if (arrows[0] || arrows[2]) {
                hmove -= 0.71f;
            } else {
                hmove -= 1.0f;
            }
        }
        if (arrows[2]) {
            if (arrows[1] || arrows[3]) {
                vmove += 0.71f;
            } else {
                vmove += 1.0f;
            }
        }
        if (arrows[3]) {
            if (arrows[0] || arrows[2]) {
                hmove += 0.71f;
            } else {
                hmove += 1.0f;
            }
        }
        // basically check if they can move to a certain point in the direction. It checks the 2 corners
        // on each side that they are moving.
        if (hmove > 0) {
            // RIGHT
            if (!walkable[tilemap.tile_id_at_point(sf::Vector2f(
                            sprite.getPosition().x + sprite.getLocalBounds().width + hmove,
                            sprite.getPosition().y + 1))] // if we don't put the +1/-1 here, it checks the
                                                          // exact corners and won't let you fit through a
                                                          // 1-tile-wide space
                    || !walkable[tilemap.tile_id_at_point(sf::Vector2f(
                            sprite.getPosition().x + sprite.getLocalBounds().width + hmove,
                            sprite.getPosition().y + sprite.getLocalBounds().height - 1))]) {
                hmove = 0.0f;
            }
        } else if (hmove < 0) {
            // LEFT
            if (!walkable[tilemap.tile_id_at_point(sf::Vector2f(
                            sprite.getPosition().x + hmove,
                            sprite.getPosition().y + 1))]
                    || !walkable[tilemap.tile_id_at_point(sf::Vector2f(
                            sprite.getPosition().x + hmove,
                            sprite.getPosition().y + sprite.getLocalBounds().height - 1))]) {
                hmove = 0.0f;
            }
        }
        if (vmove > 0) {
            // DOWN
            if (!walkable[tilemap.tile_id_at_point(sf::Vector2f(
                            sprite.getPosition().x + 1,
                            sprite.getPosition().y + sprite.getLocalBounds().height + vmove))]
                    || !walkable[tilemap.tile_id_at_point(sf::Vector2f(
                            sprite.getPosition().x + sprite.getLocalBounds().width - 1,
                            sprite.getPosition().y + sprite.getLocalBounds().height + vmove))]) {
                vmove = 0.0f;
            }
        } else if (vmove < 0) {
            // UP
            if (!walkable[tilemap.tile_id_at_point(sf::Vector2f(
                            sprite.getPosition().x + 1,
                            sprite.getPosition().y + vmove))]
                    || !walkable[tilemap.tile_id_at_point(sf::Vector2f(
                            sprite.getPosition().x + sprite.getLocalBounds().width - 1,
                            sprite.getPosition().y + vmove))]) {
                vmove = 0.0f;
            }
        }
        sprite.move(sf::Vector2f(hmove, vmove));
        camera.setCenter(sprite.getPosition().x, sprite.getPosition().y);

        textbox.update();

        window.clear(sf::Color::Black);
        window.draw(tilemap);
        window.draw(sprite);
        window.draw(textbox);
        window.display();

    }

    return 0;
}

