#include <SFML/Graphics.hpp>

/* This is a drawable object (a character, a piece of scenery, a building, etc).
 * It's basically similar to a sprite except that it has offsets so it is drawn
 * on top of a tile correctly, and it has (or will have) some extra methods for
 * interacting with other objects. */
class GameObject : public sf::Drawable {
    public:
        GameObject(sf::Sprite sprite_,
                sf::Vector2i position_,
                sf::Rect<int> boxLoc_) {
            sprite = sprite_;
            position = position_;
            boxLoc = boxLoc_;
            sprite.setPosition(sf::Vector2f(position.x - boxLoc.left, position.y - boxLoc.top));
            absLoc = sf::Rect<float>(position.x, position.y, boxLoc.width, boxLoc.height);
        }

        bool hitTest(sf::Vector2f point) {
            return absLoc.contains(point);
        }

        bool hitTest(GameObject obj) {
            return absLoc.intersects(obj.absLoc);
        }

        sf::Sprite getSprite() {
            return sprite;
        }

        void setSprite(sf::Sprite sprite_) {
            sprite = sprite_;
        }

        void setPosition(sf::Vector2i position_) {
            position = position_;
            sprite.setPosition(sf::Vector2f(position.x - boxLoc.left, position.y - boxLoc.top));
            absLoc = sf::Rect<float>(position.x, position.y, boxLoc.width, boxLoc.height);
        }

        sf::Vector2i getPosition() {
            return position;
        }

        sf::Rect<float> absLoc; // the absolute location of the object's bounding
                                // box in relation to global coordinates
        int drawDepth;          // the depth that it will be drawn at -- 0 is the
                                // furthest to the back (next to the background),
                                // higher numbers are drawn in that order

    protected:
        sf::Vector2i position;  // the offset of the corner of the bounding box's location
        sf::Rect<int> boxLoc;   // the size and local-coordinates of the bounding
                                // box in relation to the drawn sprite
        sf::Sprite sprite;      // the sprite that will be drawn

        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
            // draw the sprite, which has had its position set correctly (we hope)
            target.draw(sprite, states);
        }
};
