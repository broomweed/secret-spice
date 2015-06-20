#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#include <windows.h>
#define debug
#endif
#include <iostream>
#include <cstdio>
#include <SFML/Graphics.hpp>
#include "Thing.hpp"
#include "GameObject.hpp"
#include "Character.hpp"
#include "SceneManager.hpp"
#include "Scene.hpp"
#include "Door.hpp"
#include "Dialogue.hpp"
#include "TextBox.hpp"
#include "SpriteSheet.hpp"
#define SCRWIDTH 800
#define SCRHEIGHT 600

using std::string;

int main(int argc, char **argv) {
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#if !defined(debug)
    FreeConsole();
#else
    std::cout << "Hi, you're running in Windows debug mode. Remember to turn this off later!" << std::endl;
#endif
#endif
    sf::RenderWindow window(sf::VideoMode(SCRWIDTH, SCRHEIGHT), "Secret Spice");
    // DANG THIS IS THE BEST THING:
    sf::View camera(sf::FloatRect(0, 0, SCRWIDTH/2, SCRHEIGHT/2));
    window.setView(camera);
    sf::Texture tiles;

    const int level[] = {
        0,0,1,2,0,0,0,0,3,4,5,6,7,0,0,0,0,0,0,0,0,0,0,0,0,8,0,0,0,0,0,0,0,0,8,9,10,11,12,0,0,0,1,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,4,5,6,7,0,0,0,0,0,1,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8,9,10,11,12,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,4,0,0,0,0,0,0,0,0,0,0,0,3,4,5,6,7,0,0,0,0,0,1,2,0,8,9,0,0,0,0,0,0,0,0,0,0,0,8,9,10,11,12,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,13,14,15,16,17,0,0,0,0,1,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8,9,10,11,12,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,4,5,6,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,8,9,10,11,12
    };

    const int level2[] = {
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

    TileMap tilemap2;
    if (!tilemap2.load("images/tests/trees2/tiles.png", sf::Vector2u(16, 16), level2, 26, 20)) {
        fprintf(stderr, "something went wrong with the second tileset!\n");
        return -1;
    }

    Scene scene(tilemap);
    Scene scene2(tilemap2);

    SceneManager sm(&scene);

    SpriteSheet contArrowSheet("images/more-line.png", 4, 1);
    Animation contArrow;
    contArrow.addFrame(contArrowSheet.getSprite(0,0), 290);
    contArrow.addFrame(contArrowSheet.getSprite(1,0), 170);
    contArrow.addFrame(contArrowSheet.getSprite(2,0), 170);
    contArrow.addFrame(contArrowSheet.getSprite(3,0), 290);
    contArrow.addFrame(contArrowSheet.getSprite(2,0), 170);
    contArrow.addFrame(contArrowSheet.getSprite(1,0), 170);

    DialogueTextBox textbox(sf::Rect<int>(5, 5, 200, 35), 18.0f, contArrow);

    int charWidths[68] =
       //A B C D E F G H I J K L M N O P Q R S T U V W X Y Z
        {7,6,6,6,5,5,6,6,2,5,6,5,8,6,6,6,6,6,6,6,6,7,8,6,6,5,
       //a b c d e f g h i j k l m n o p q r s t u v w x y z
         5,5,5,5,5,4,5,5,2,3,5,2,8,5,5,5,5,4,5,4,5,6,8,5,5,5,
       //0 1 2 3 4 5 6 7 8 9 . , ! ? ' space
         5,3,5,5,6,5,5,5,5,5,2,3,2,5,3,2};

    if (!textbox.setFont(string("images/ebfont.png"), 11, charWidths,
            "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789.,!?' ")) {
        fprintf(stderr, "Something went wrong with the font!\n");
        return -2;
    }

    bool arrows[4] = { false, false, false, false };

    SpriteSheet dan("images/dan.png", 3, 8);
    Character guy(dan, sf::Vector2f(0, 16), sf::Rect<float>(5, 24, 14, 8));

    if (!tiles.loadFromFile("images/tilemap.png")) {
        fprintf(stderr, "something went wrong (2)!\n");
    }
    tiles.setSmooth(false);

    Dialogue npc1Dia;
    npc1Dia.addLine("My name is Robert Alexandrius Dominus III, but you can probably just call me Rob. That's what my friends call me, anyway.");
    npc1Dia.addLine("Oh no, was that too familiar? I'm sorry.");
    npc1Dia.addLine("Well, this is awkward. Goodbye then.");

    Dialogue npc2Dia;
    npc2Dia.addLine("My name is Harold.\nIt's nice to meet you, Daniel!\nThis is a new line.");
    npc2Dia.addLine("Whoa, I think that was a haiku!");

    Character npc1(dan, sf::Vector2f(100, 150), sf::Rect<float>(3, 22, 18, 12));
    npc1.setText(npc1Dia);
    Character npc2(dan, sf::Vector2f(200, 24), sf::Rect<float>(3, 22, 18, 12));
    npc2.setText(npc2Dia);

    scene.add(&guy);
    scene.add(&npc1);
    scene.add(&npc2);
    scene.setMC(&guy);
    scene.setName("the verdant forest");

    GameObject tree("images/tests/tree.png", sf::Vector2f(0, 0), sf::Rect<float>(12, 66, 39, 12));

    scene.add_static(&tree, sf::Vector2f(150, 10));
    scene.add_static(&tree, sf::Vector2f(38, 90));
    scene.add_static(&tree, sf::Vector2f(198, 170));
    scene.add_static(&tree, sf::Vector2f(38, 218));
    scene.add_static(&tree, sf::Vector2f(358, 298));

    scene2.add_static(&tree, sf::Vector2f(198, 170));
    scene2.setName("the barren desert");

    Door door(&scene2, sf::Rect<float>(198, 182, 20, 20), sf::Vector2f(16, 16), 0);
    scene.add(&door);
    Door door2(&scene, sf::Rect<float>(0, 0, 32, 32), sf::Vector2f(208, 192), 0);
    scene2.add(&door2);

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
                            if (guy.checked != NULL) {
                                if (guy.checked->getText().numLines() > 0) {
                                    guy.checked->setDirection((guy.getDirection() + 4) % 8);
                                    textbox.setDialogue(guy.checked->getText());
                                    textbox.show();
                                } else {
                                    textbox.setDialogue(Dialogue("No problem here."));
                                    textbox.show();
                                }
                            }
                        } else {
                            if (textbox.lineFinished) {
                                textbox.nextLine();
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

        if (!textbox.hidden) {
            hmove = 0.0;
            vmove = 0.0;
        }

        guy.setSpeed(hmove, vmove);

        textbox.update();
        sm.update();
        float cameraX, cameraY;
        // it's all /4 instead of /2 because the camera is zoomed in 2x on a 400x300 section
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
        textbox.setPosition((int)cameraX - textbox.getSizeRect().width/2, (int)cameraY + SCRHEIGHT/4 - 39);

        window.clear(sf::Color::Black);
        window.draw(scene);
        window.draw(scene2);
        window.draw(textbox);
        window.display();
    }

    return 0;
}
