/* This is a point at which an entity changed direction.
 * It will be used to make the other party members
 * follow the main character around. */
class Waypoint {
    public:
        Waypoint(float x, float y, sf::Vector2f speed_)
            :   position(sf::Vector2f(x, y)),
                speed(speed_) { }

        Waypoint(sf::Vector2f position_, sf::Vector2f speed_)
            :   position(position_),
                speed(speed_) { }

        sf::Vector2f position;
        sf::Vector2f speed;
};
