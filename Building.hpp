#include "GameObject.hpp"

/* This is essentially the same as a GameObject,
 * except that it has a trapezoid-shaped hitbox
 * area, so can be used for a building. */
class Building : public GameObject {
    public:
        Building(Animation anim_,
                sf::Vector2f position_,
                sf::Rect<float> boxLoc_)
                    : GameObject(anim_, position_, boxLoc_) { }

        Building(std::string filename,
                sf::Vector2f position_,
                sf::Rect<float> boxLoc_)
                    : GameObject(filename, position_, boxLoc_) { }

        virtual bool hitTest(sf::Vector2f point) const {
            return absLoc.contains(point)
                && !pointInTriangle(point.x, point.y);
            /*
             *   ._________________________
             *   :'.                       |
             * x : NO.        YES          |
             *   :....:.___________________|
             *     2x
             */
        }

        virtual bool hitTest(Thing obj) const {
            return hitTest(obj.absLoc);
        }

        virtual bool hitTest(sf::Rect<float> rect) const {
            return absLoc.intersects(rect)
                && !(    /* rects intersect, but doesn't hitTest if both points on the right side
                           are within the magic triangle */
                        pointInTriangle(rect.left + rect.width, rect.top)
                        && pointInTriangle(rect.left + rect.width, rect.top + rect.height)
                    );
        }

        bool pointInTriangle(float x, float y) const {
            return (x - absLoc.left + 2 * (absLoc.top + absLoc.height - y) < absLoc.height * 2);
        }
};
