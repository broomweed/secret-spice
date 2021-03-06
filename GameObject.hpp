#ifndef SPICE_GAMEOBJECT_HPP
#define SPICE_GAMEOBJECT_HPP
#include <SFML/Graphics.hpp>

/* This is a drawable object (a character, a piece of scenery, a building, etc).
 * It's basically similar to a sprite except that it has offsets so it is drawn
 * on top of a tile correctly, and it has (or will have) some extra methods for
 * interacting with other objects. */
class GameObject : public Thing {
    public:
        GameObject(Animation anim_,
                sf::Vector2f position_,
                sf::Rect<float> boxLoc_)
                    : Thing(boxLoc_) {
            drawDepth = 0;
            anim = anim_;
            position = position_;
            setPosition(position_);
        }

        GameObject(std::string filename,
                sf::Vector2f position_,
                sf::Rect<float> boxLoc_)
                    : Thing(boxLoc_) {
            if(!tex.loadFromFile(filename)) {
                std::cerr << "Error loading filename " << filename << std::endl;
            }
            drawDepth = 0;
            anim = Animation();
            anim.addFrame(sf::Sprite(tex), 1000);
            position = position_;
            setPosition(position_);
        }

        /* Don't use this method if the object is located within a scene
           (ie pretty much always) -- it doesn't switch draw-depths properly. */
        void move(sf::Vector2f dp) {
            setPosition(getPosition().x + dp.x, getPosition().y + dp.y);
        }

        /* Likewise. */
        void move(float dx, float dy) {
            setPosition(getPosition().x + dx, getPosition().y + dy);
        }

    protected:
        sf::Texture tex;        // static-image texture
};
#endif // SPICE_GAMEOBJECT_HPP
