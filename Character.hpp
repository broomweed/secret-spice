#include <list>
#include <math.h> // sqrt
#include "Scene.hpp"
#include "SpriteSheet.hpp"
#include "Waypoint.hpp"

class Character : public Thing {
    public:
        Character(std::vector<std::vector<Animation> > anims_,
                sf::Vector2f position_,
                sf::Rect<float> boxLoc_)
                :   Thing(boxLoc_),
                    anims(anims_),
                    animation_id(0),
                    direction(0),
                    follower(NULL),
                    partyLeader(false) {
            setPosition(position_);
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
                :   Thing(boxLoc_),
                    animation_id(0),
                    direction(0),
                    follower(NULL),
                    partyLeader(false) {
            setPosition(position_);
            setAnimationsFromSpriteSheet(ss, 220);
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
                if (animation_id != 0 && follower) {
                    if (partyLeader) {
                        createWaypoint(); // no waypoints for stopping moving
                    }
                    position = sf::Vector2f(roundf(position.x), roundf(position.y)); // for smooth diagonals
                }
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
            Thing::stopMoving();
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

        Thing *getFollower() {
            return follower;
        }

        bool partyLeader;
        std::list<Waypoint> waypoints;

        virtual void update() {
            Thing::update();
            if (follower) {
                if (!waypoints.empty()) { // don't do something random if no waypoints
                    //sf::Vector2f dist = waypoints.front().position - follower->getPosition();
                    sf::Vector2f dist = waypoints.front().position - sf::Vector2f(roundf(follower->getPosition().x), roundf(follower->getPosition().y));
                    float dp = follower->pastSpeed.x * dist.x + follower->pastSpeed.y * dist.y;
                    while (dp <= 0) {
                        /* this is dot product: if it's 0 or negative, the follower is
                         * on top of the waypoint or went past the waypoint, so put it
                         * back and change its speed, and hand the waypoint along in
                         * case there's someone else following the follower */
                        if (dp <= -sqrt(dist.x * dist.x + dist.y * dist.y)) {
                            /* if error very large, put it back in position, but otherwise
                             * leave it on its own because this can introduce lag */
                            follower->setPosition(waypoints.front().position);
                        }
                        follower->pastSpeed = waypoints.front().speed;
                        follower->setSpeed(follower->pastSpeed);
                        if (follower->follower) {
                            follower->waypoints.push_back(waypoints.front());
                        }
                        waypoints.pop_front();
                        if (waypoints.empty()) break;
                        dist = waypoints.front().position - follower->getPosition();
                        dp = follower->pastSpeed.x * dist.x + follower->pastSpeed.y * dist.y;
                    }
                }
                if (getFollowDistance() > 18.0f) {
                    /* always catch up if too far behind/correct distance behind */
                    follower->setSpeed(follower->pastSpeed);
                } else {
                    /* otherwise, stop, but not if it's already stopped */
                    if (follower->getSpeed() != sf::Vector2f(0.0f, 0.0f)) {
                        follower->pastSpeed = follower->getSpeed();
                        follower->setSpeed(0.0f, 0.0f);
                    }
                }
            }
        }

        void createWaypoint() {
            Waypoint wp = Waypoint(roundf(position.x), roundf(position.y), getSpeed());
            waypoints.push_back(wp);
        }

        void clearWaypoints() {
            waypoints.clear();
        }

        void follow(Character& charToFollow) {
            charToFollow.follower = this;
        }

        float getFollowDistance() {
            float xdist = 0.0f, ydist = 0.0f;
            float total = 0.0f;
            if (follower) {
                if (waypoints.empty()) {
                    // just the distance between you and the follower
                    xdist = follower->getPosition().x - position.x;
                    ydist = follower->getPosition().y - position.y;
                    total += sqrt(xdist * xdist + ydist * ydist);
                } else {
                    // add up the total distance between all your waypoints, plus the distance
                    // from you to the most recent one and from the least recent one to the follower
                    xdist = waypoints.front().position.x - follower->getPosition().x;
                    ydist = waypoints.front().position.y - follower->getPosition().y;
                    total += sqrt(xdist * xdist + ydist * ydist);
                    for (std::list<Waypoint>::iterator wp = waypoints.begin(); wp != waypoints.end(); wp++) { // jeez, c++
                        if (std::next(wp) == waypoints.end()) break;
                        xdist = wp->position.x - std::next(wp)->position.x; // this is why we have to use c++11
                        ydist = wp->position.y - std::next(wp)->position.y;
                        total += sqrt(xdist * xdist + ydist * ydist);
                    }
                    xdist = position.x - waypoints.back().position.x;
                    ydist = position.y - waypoints.back().position.y;
                    total += sqrt(xdist * xdist + ydist * ydist);
                }
            }
            return total;
        }

    protected:
        int animation_id;
        int direction;
        std::vector<std::vector<Animation> > anims;
        Scene *currentScene;
        Character *follower;
};
