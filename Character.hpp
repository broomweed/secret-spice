#include "SpriteSheet.hpp"

class Character : public Thing {
    public:
        Character(std::vector<std::vector<Animation> > anims_,
                sf::Vector2f position_,
                sf::Rect<float> boxLoc_)
                    : Thing() {
            drawDepth = 0;
            boxLoc = boxLoc_;
            setPosition(position_);
            anims = anims_;
            direction = 0;
            animation_id = 0;
            Thing::setAnimation(anims[animation_id][direction]);
           /* 5 4 3
               \|/
              6- -2
               /|\
              7 0 1 */
        }

        Character(SpriteSheet& ss,
                sf::Vector2f position_,
                sf::Rect<float> boxLoc_)
                    : Thing() {
            drawDepth = 0;
            boxLoc = boxLoc_;
            setPosition(position_);
            setAnimationsFromSpriteSheet(ss);
            direction = 0;
            animation_id = 0;
            Thing::setAnimation(anims[animation_id][direction]);
        }

        void move(sf::Vector2f dp) {
            move(dp.x, dp.y);
        }

        void turn(float dx, float dy) {
            int old_animation_id = animation_id;
            int old_direction = direction;
            animation_id = 1;
            if (dx == 0) {
                if (dy < 0) {
                    direction = 4;
                } else if (dy > 0) {
                    direction = 0;
                } else {
                    // We have stopped walking.
                    animation_id = 0;
                }
            } else if (dx < 0) {
                if (dy < 0) {
                    direction = 5;
                } else if (dy > 0) {
                    direction = 7;
                } else {
                    direction = 6;
                }
            } else {
                if (dy < 0) {
                    direction = 3;
                } else if (dy > 0) {
                    direction = 1;
                } else {
                    direction = 2;
                }
            }
            if (old_animation_id != animation_id || old_direction != direction) {
                Thing::setAnimation(anims[animation_id][direction]);
            }
        }

        void move(float dx, float dy) {
            setPosition(getPosition().x + dx, getPosition().y + dy);
        }

        void setAnimationsFromSpriteSheet(SpriteSheet& ss) {
            std::vector<Animation> stand;
            std::vector<Animation> walk;
            for (int i = 0; i < 8; i++) {
                stand.push_back(Animation(ss.getSprite(1, i)));
            }
            for (int i = 0; i < 8; i++) {
                Animation ani;
                ani.addFrame(ss.getSprite(0, i), 250);
                ani.addFrame(ss.getSprite(2, i), 250);
                walk.push_back(ani);
            }
            std::vector<std::vector<Animation> > anims_;
            anims_.push_back(stand);
            anims_.push_back(walk);
            anims = anims_;
        }

        void stopMoving() {
            xspeed = 0;
            yspeed = 0;
            turn(0.0f, 0.0f);
            move(0.0f, 0.0f);
        }

        int getDirection() {
            return direction;
        }

        void setDirection(int dir) {
            direction = dir;
            Thing::setAnimation(anims[animation_id][direction]);
        }

    protected:
        int animation_id;
        int direction;
        std::vector<std::vector<Animation> > anims;
};
