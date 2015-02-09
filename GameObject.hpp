#ifndef SPICE_GAMEOBJECT_HPP
#define SPICE_GAMEOBJECT_HPP
#include <SFML/Graphics.hpp>
#include "Animation.hpp"

/* This is a drawable object (a character, a piece of scenery, a building, etc).
 * It's basically similar to a sprite except that it has offsets so it is drawn
 * on top of a tile correctly, and it has (or will have) some extra methods for
 * interacting with other objects. */
class GameObject : public sf::Drawable {
    public:
        GameObject(Animation anim_,
                sf::Vector2f position_,
                sf::Rect<float> boxLoc_,
                unsigned int drawDepth_) {
            drawDepth = drawDepth_;
            anim = anim_;
            position = position_;
            boxLoc = boxLoc_;
            setPosition(position_);
        }

        bool hitTest(sf::Vector2f point) const {
            return absLoc.contains(point);
        }

        bool hitTest(GameObject obj) const {
            return absLoc.intersects(obj.absLoc);
        }

        sf::Sprite getSprite() const {
            return anim.getCurrentSprite();
        }

        Animation getAnimation() const {
            return anim;
        }

        void setPosition(sf::Vector2f position_) {
            position = position_;
            anim.setPosition(sf::Vector2f(position.x - boxLoc.left, position.y - boxLoc.top));
            absLoc = sf::Rect<float>(position.x, position.y, boxLoc.width, boxLoc.height);
        }

        void setPosition(float x, float y) {
            setPosition(sf::Vector2f(x, y));
        }

        sf::Vector2f getPosition() const {
            return position;
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

        void update() {
            anim.update();
        }

        sf::Rect<float> absLoc; // the absolute location of the object's bounding
                                // box in relation to global coordinates
        int sceneIndex;         // the index of it in the scene std::vector
                                // so we can look it up in the scene using only
                                // the object itself.
        unsigned int drawDepth; // the depth that it will be drawn at -- 0 is the
                                // furthest to the back (next to the background),
                                // higher numbers are drawn in that order

    protected:
        sf::Vector2f position;  // the offset of the corner of the bounding box's location
        sf::Rect<float> boxLoc; // the size and local-coordinates of the bounding
                                // box in relation to the drawn animation
        Animation anim;         // the animation that it will be drawn with

        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
            // draw the animation, which has had its position set correctly (we hope)
            target.draw(anim, states);
        }
};
#endif // SPICE_GAMEOBJECT_HPP
