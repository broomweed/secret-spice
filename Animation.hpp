#include <SFML/Graphics.hpp>
#include <vector>

using std::vector;
using std::string;

/* AnimationFrame is a single frame of an animation. It consists of
   a sprite and an sf::Time that represents the time that it ends
   in the animation. */

class AnimationFrame {
    public:
        sf::Sprite sprite;
        sf::Time endTime;

        AnimationFrame(sf::Sprite sprite_, sf::Time endTime_) {
            sprite = sprite_;
            endTime = endTime_;
        }
};

/* Animation is a timed sequence of sprites. It tracks the lengths of
   each frame, and goes between them when it changes. You have to call
   update() on it to make it go, though -- don't worry, GameObject will
   call update() on its own animation, and Scene calls update() on
   the GameObjects, so really all you have to do is update the scene
   and they will all update automatically. */
class Animation : public sf::Drawable {
    public:
        Animation() {
            totalTime = sf::Time();
            currTime = sf::Time();
            currFrameIndex = 0;
            setPosition(sf::Vector2f(0, 0));
            timeElapsed.restart();
        }

        Animation(sf::Vector2f pos_) {
            totalTime = sf::Time();
            currTime = sf::Time();
            currFrameIndex = 0;
            setPosition(pos_);
            timeElapsed.restart();
        }

        Animation(sf::Sprite sprite) {
            totalTime = sf::Time();
            currTime = sf::Time();
            currFrameIndex = 0;
            setPosition(sf::Vector2f(0, 0));
            timeElapsed.restart();
            addFrame(sprite, 1000);
        }

        vector<AnimationFrame> frames;

        void addFrame(sf::Sprite sprite, int duration) {
            totalTime += sf::milliseconds(duration);
            frames.push_back(AnimationFrame(sprite, totalTime));
        }

        AnimationFrame getFrame(int i) const {
            return frames[i];
        }

        sf::Sprite getSprite(int i) const {
            return getFrame(i).sprite;
        }

        AnimationFrame getCurrentFrame() const {
            return getFrame(currFrameIndex);
        }

        sf::Sprite getCurrentSprite() const {
            return getSprite(currFrameIndex);
        }

        void setPosition(float x, float y) {
            position = sf::Vector2f(x, y);
            for (int i = 0; i < frames.size(); i++) {
                frames[i].sprite.setPosition(x, y);
            }
        }

        sf::Vector2f getPosition() const {
            return position;
        }

        sf::Rect<float> getLocalBounds() const {
            return getCurrentSprite().getLocalBounds();
        }

        void setPosition(sf::Vector2f pos) {
            setPosition(pos.x, pos.y);
        }

        void move(float dx, float dy) {
            setPosition(getPosition().x + dx, getPosition().y + dy);
        }

        void move(sf::Vector2f dp) {
            move(dp.x, dp.y);
        }

        void update() {
            currTime += timeElapsed.getElapsedTime();
            while (currTime > getCurrentFrame().endTime) {
                if (currFrameIndex < frames.size()-1) {
                    /* size() - 1 because we're about to increment it here */
                    currFrameIndex++;
                } else {
                    currFrameIndex = 0;
                    while (currTime > totalTime) { // no modulo for sf::Time :(
                        currTime = currTime - totalTime;
                    }
                }
            }
            timeElapsed.restart();
        }

    private:
        sf::Clock timeElapsed;  // the time elapsed since last updated
        sf::Time currTime;      // the total current time (so it can find the right frame)
        sf::Time totalTime;     // the total length of all frames combined
        sf::Vector2f position;  // the position of all of the sprite offsets
        int currFrameIndex;     // the index of the current frame in the vector

        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
            target.draw(getCurrentSprite(), states);
        }
};
