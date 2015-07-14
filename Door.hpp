#include <SFML/Graphics.hpp>
#include "Scene.hpp"

/* This is a door -- a region that leads between
   two separate maps (which are just represented as
   different Scenes, for convenience, and one of them
   is just currently active.) */
class Door : public Thing {
    public:
        Door (Scene *destination_,
                sf::Rect<float> location,
                sf::Vector2f destCoords_,
                int targetDir_)
                    : Thing(location) {
            destination = destination_;
            absLoc = boxLoc;
            drawDepth = 0;
            destCoords = destCoords_;
            targetDir = targetDir_;
            anim = Animation();
            anim.addFrame(sf::Sprite(), 1000);
            tangible = false;
        }

        void update() {
            // no animation to update!
        }

        void move(float dx, float dy) {
            // do nothing
        }

        void onTouch() {
            parent->transfer(destination, destCoords, targetDir);
        }

    protected:
        Scene *destination;
        sf::Vector2f destCoords;

        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
            /* don't draw anything, it is magically intangible */
        }
};
