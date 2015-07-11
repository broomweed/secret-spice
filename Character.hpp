#include <queue>
#include "Scene.hpp"
#include "SpriteSheet.hpp"
#include "Waypoint.hpp"

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
            follower = NULL;
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
            setAnimationsFromSpriteSheet(ss, 220);
            direction = 0;
            animation_id = 0;
            Thing::setAnimation(anims[animation_id][direction]);
            follower = NULL;
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

            if (old_animation_id != animation_id) {
                Thing::setAnimation(anims[animation_id][direction]);
            }

            if(old_direction != direction) {
                Thing::setAnimation(anims[animation_id][direction]);
                createWaypoint();
            }
        }

        void move(float dx, float dy) {
            setPosition(getPosition().x + dx, getPosition().y + dy);
        }

        void setAnimationsFromSpriteSheet(SpriteSheet& ss, int frameLength) {
            std::vector<Animation> stand;
            std::vector<Animation> walk;
            for (int i = 0; i < 8; i++) {
                stand.push_back(Animation(ss.getSprite(1, i)));
            }
            for (int i = 0; i < 8; i++) {
                Animation ani;
                ani.addFrame(ss.getSprite(0, i), frameLength);
                ani.addFrame(ss.getSprite(1, i), frameLength);
                ani.addFrame(ss.getSprite(2, i), frameLength);
                ani.addFrame(ss.getSprite(1, i), frameLength);
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

        std::queue<Waypoint> waypoints;
        
        virtual void update() {
            Thing::update();
            if (follower) {
                sf::Vector2f target;
                if (waypoints.size() > 0) {
                    target = waypoints.front().position;
                } else {
                    target = getPosition();
                }
                if (getSpeed().x != 0.0f || getSpeed().y != 0.0f) {
                    if (follower->getPosition().x < target.x - 0.5) {
                        if (follower->getPosition().y < target.y - 0.5) {
                            follower->setSpeed(0.71f, 0.71f);
                        } else if (follower->getPosition().y > target.y + 0.5) {
                            follower->setSpeed(0.71f, -0.71f);
                        } else {
                            follower->setSpeed(1, 0);
                        }
                    } else if (follower->getPosition().x > target.x + 0.5) {
                        if (follower->getPosition().y < target.y - 0.5) {
                            follower->setSpeed(-0.71f, 0.71f);
                        } else if (follower->getPosition().y > target.y + 0.5) {
                            follower->setSpeed(-0.71f, -0.71f);
                        } else {
                            follower->setSpeed(-1, 0);
                        }
                    } else {
                        if (follower->getPosition().y < target.y - 0.5) {
                            follower->setSpeed(0, 1);
                        } else if (follower->getPosition().y > target.y + 0.5) {
                            follower->setSpeed(0, -1);
                        } else {
                            if (follower->follower) {
                                follower->waypoints.push(waypoints.front());
                            }
                            waypoints.pop();
                        }
                    }
                    if (!shouldMoveX) {
                        follower->setSpeed(0, follower->getSpeed().y);
                    }
                    if (!shouldMoveY) {
                        follower->setSpeed(follower->getSpeed().x, 0);
                    }
                } else {
                    follower->setSpeed(0, 0);
                }
            }
        }

        void createWaypoint() {
            Waypoint wp = Waypoint(position, getSpeed());
            waypoints.push(wp);
        }

        void follow(Character& charToFollow) {
            charToFollow.follower = this;
        }

    protected:
        int animation_id;
        int direction;
        std::vector<std::vector<Animation> > anims;
        Scene *currentScene;
        Character *follower;
};
