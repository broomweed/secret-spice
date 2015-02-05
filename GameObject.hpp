#ifndef SPICE_GAMEOBJECT_HPP
#define SPICE_GAMEOBJECT_HPP
#include <SFML/Graphics.hpp>

/* This is a drawable object (a character, a piece of scenery, a building, etc).
 * It's basically similar to a sprite except that it has offsets so it is drawn
 * on top of a tile correctly, and it has (or will have) some extra methods for
 * interacting with other objects. */
class GameObject : public sf::Drawable {
    public:
        GameObject(sf::Sprite sprite_,
                sf::Vector2f position_,
                sf::Rect<float> boxLoc_,
                unsigned int drawDepth_) {
            drawDepth = drawDepth_;
            sprite = sprite_;
            position = position_;
            boxLoc = boxLoc_;
            sprite.setPosition(position.x - boxLoc.left, position.y - boxLoc.top);
            absLoc = sf::Rect<float>(position.x, position.y, boxLoc.width, boxLoc.height);
        }

        GameObject(std::string filename,
                sf::Vector2f position_,
                sf::Rect<float> boxLoc_,
                unsigned int drawDepth_) {
            if (!tex.loadFromFile(filename)) {
                std::cerr << "Error loading from file " << filename << "\n";
            }
            tex.setSmooth(false);
            sprite.setTexture(tex);

            drawDepth = drawDepth_;
            position = position_;
            boxLoc = boxLoc_;
            sprite.setPosition(position.x - boxLoc.left, position.y - boxLoc.top);
            absLoc = sf::Rect<float>(position.x, position.y, boxLoc.width, boxLoc.height);
        }

        bool hitTest(sf::Vector2f point) const {
            return absLoc.contains(point);
        }

        bool hitTest(GameObject obj) const {
            return absLoc.intersects(obj.absLoc);
        }

        sf::Sprite getSprite() const {
            return sprite;
        }

        void setSprite(sf::Sprite sprite_) {
            sprite = sprite_;
        }

        void setPosition(sf::Vector2f position_) {
            position = position_;
            sprite.setPosition(sf::Vector2f(position.x - boxLoc.left, position.y - boxLoc.top));
            absLoc = sf::Rect<float>(position.x, position.y, boxLoc.width, boxLoc.height);
        }

        void setPosition(float x, float y) {
            setPosition(sf::Vector2f(x, y));
        }

        sf::Vector2f getPosition() const {
            return position;
        }

        void move(sf::Vector2f dp) {
            setPosition(getPosition().x + dp.x, getPosition().y + dp.y);
        }

        void move(float dx, float dy) {
            setPosition(getPosition().x + dx, getPosition().y + dy);
        }

        sf::Rect<float> absLoc; // the absolute location of the object's bounding
                                // box in relation to global coordinates
        unsigned int drawDepth; // the depth that it will be drawn at -- 0 is the
                                // furthest to the back (next to the background),
                                // higher numbers are drawn in that order

    protected:
        sf::Texture tex;        // the texture of the sprite, in case it's loaded from file
        sf::Vector2f position;  // the offset of the corner of the bounding box's location
        sf::Rect<float> boxLoc; // the size and local-coordinates of the bounding
                                // box in relation to the drawn sprite
        sf::Sprite sprite;      // the sprite that will be drawn

        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
            // draw the sprite, which has had its position set correctly (we hope)
            target.draw(sprite, states);
        }
};
#endif // SPICE_GAMEOBJECT_HPP
