#ifndef SPICE_THING_HPP
#define SPICE_THING_HPP
#include "Dialogue.hpp"
#include "Animation.hpp"
#include <math.h>
/* This is the last common ancestor of GameObject and Character.
   Actually most things from GameObject go in here. However, they
   move in ways that are incompatible, so one cannot inherit from
   the other. Hence this class. */
class Thing : public sf::Drawable {
    public:
        Thing(sf::Rect<float> boxLoc_)
            :   text(Dialogue()),
                drawDepth(0),
                boxLoc(boxLoc_),
                is_copy(false),
                active(true),
                targetDir(0),
                pastSpeed(sf::Vector2f(0.0f, 0.0f)),
                tangible(true) {
            stopMoving();
        }

        // Stuff to handle party-member following:
        bool tangible;
        sf::Vector2f pastSpeed;

        virtual bool hitTest(sf::Vector2f point) const {
            return absLoc.contains(point);
        }

        virtual bool hitTest(Thing obj) const {
            return absLoc.intersects(obj.absLoc);
        }

        virtual bool hitTest(sf::Rect<float> rect) const {
            return absLoc.intersects(rect);
        }

        virtual void setPosition(sf::Vector2f position_) {
            position = position_;
            anim.setPosition(sf::Vector2f(roundf(position.x - boxLoc.left), roundf(position.y - boxLoc.top)));
            absLoc = sf::Rect<float>(position.x, position.y, boxLoc.width, boxLoc.height);
        }

        void setPosition(float x, float y) {
            setPosition(sf::Vector2f(x, y));
        }

        sf::Vector2f getPosition() const {
            return position;
        }

        float getX() const {
            return position.x;
        }

        float getY() const {
            return position.y;
        }

        sf::Sprite getSprite() const {
            return anim.getCurrentSprite();
        }

        void setAnimation(Animation& anim_) {
            anim = anim_;
        }

        Animation getAnimation() const {
            return anim;
        }

        virtual void update() {
            anim.update();
        }

        int sceneIndex;         // the index of it in the scene std::vector
                                // so we can look it up in the scene using only
                                // the object itself.
        unsigned int drawDepth; // the depth that it will be drawn at -- 0 is the
                                // furthest to the back (next to the background),
                                // higher numbers are drawn in that order
        sf::Rect<float> absLoc; // the absolute location of the object's bounding
                                // box in relation to global coordinates

        void setSpeed(float dx, float dy) {
            speed = sf::Vector2f(dx, dy);
        }

        void setSpeed(sf::Vector2f speed_) {
            speed = speed_;
        }

        sf::Vector2f getSpeed() {
            return speed;
        }

        virtual void stopMoving() {
            speed = sf::Vector2f(0.0f, 0.0f);
        }

        /* things used for some subclasses but not others */
        virtual void turn(float dx, float dy) { /* pass */ }
        virtual int getDirection() { /* pass */ }
        virtual void setDirection(int dir) { /* pass */ }
        virtual void onTouch() { /* pass */ }
        virtual void clearWaypoints() { /* pass */ }
        virtual Thing *getFollower() { return NULL; }

        virtual void turn(sf::Vector2f dp) {
            turn(dp.x, dp.y);
        }

        void face(int dir) {
           /* 5 4 3
               \|/
              6- -2
               /|\
              7 0 1 */
            switch (dir) {
                case 0:
                    turn(0, 1); break;
                case 1:
                    turn(1, 1); break;
                case 2:
                    turn(1, 0); break;
                case 3:
                    turn(1, -1); break;
                case 4:
                    turn(0, -1); break;
                case 5:
                    turn(-1, -1); break;
                case 6:
                    turn(-1, 0); break;
                case 7:
                    turn(-1, 1); break;
                default:
                    std::cerr << "unknown direction " << dir << std::endl;
            }
        }

        virtual void move(float dx, float dy) {
            setPosition(getPosition().x + dx, getPosition().y + dy);
        }

        virtual void move(sf::Vector2f dp) {
            move(dp.x, dp.y);
        }

        void setText(Dialogue text_) {
            text = text_;
        }

        Dialogue getText() {
            return text;
        }

        sf::Rect<float> boxLoc; // the size and local-coordinates of the bounding
                                // box in relation to the drawn animation
        Thing *checked;         // this is actually only used by Character, because
                                // the GameObjects never check each other
        class Scene *parent;    // the scene it lives in

        bool is_copy;           // does the Scene need to take care of its cleanup?

        bool active;            // will it be checked for hittesting? (set to false
                                // if touching it when entering a map)
        int targetDir;

   protected:
        sf::Vector2f position;  // the offset of the corner of the bounding box's location
        Animation anim;         // the animation that it will be drawn with
        sf::Vector2f speed;
        Dialogue text;

        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
            // draw the animation, which has had its position set correctly (we hope)
            target.draw(anim, states);
        }
};
#endif
