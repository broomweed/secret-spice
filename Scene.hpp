#ifndef SPICE_SCENE_HPP
#define SPICE_SCENE_HPP
#include <vector>
#include <SFML/Graphics.hpp>
#include "SceneManager.hpp"
#include "Thing.hpp"
#include "GameObject.hpp"
#include "TileMap.hpp"

#ifndef SCRWIDTH
#define SCRWIDTH 800
#define SCRHEIGHT 600
#endif//SCRWIDTH

/* This is a scene that contains a series of Things. This is what handles
 * drawing in order of drawDepth. */
class Scene : public sf::Drawable {
    public:
        SceneManager* parent;

        Scene(Thing* objs_, int size) {
            for (int i = 0; i < size; i++) {
                add(&objs_[i]);
            }
            init();
        }

        Scene(Thing* objs_, int size, TileMap tilemap_) {
            for (int i = 0; i < size; i++) {
                add(&objs_[i]);
            }
            tilemap = tilemap_;
            init();
        }

        Scene(TileMap tilemap_) {
            tilemap = tilemap_;
            init();
        }

        Scene() {
            init();
        }

        void init() {
            num = 0;
            visible = false;
            active = false;
            transferring = NULL;
            parent = NULL;
            initFadeRect();
        }

        void initFadeRect();

        void setName(std::string name_);

        int num;

        void add(Thing *obj);

        void add_static(GameObject *obj, sf::Vector2f position);

        void remove(Thing *obj);

        std::vector<Thing*> getObjs();

        void setMC(Thing *new_mc);

        Thing *getMC();

        void update();

        void transferThingToScene(Thing *toMove, Scene *transferring);

        void move_sprite(Thing& obj, float hmove, float vmove);

        void checkLayerOrdering(Thing *object);

        void setVisible();

        void setActive(bool active_);

        bool isActive();

        void transfer(Scene *scene, sf::Vector2f destCoords_, int targetDir_);

        int getNumObjs();

        Thing *getObj(int index);

        // for easy calling of methods on it, this is a public member
        TileMap tilemap;
        std::string name;
        sf::RectangleShape fadeRect;

        ~Scene() {
            for (int i = 0; i < objs.size(); i++) {
                if (objs[i]->is_copy) {
                    delete objs[i];
                }
            }
        }

    protected:
        std::vector<Thing*> objs;
        sf::Clock loopTime;
        sf::Time lastLoop;
        Scene *transferring;
        int targetDir;
        sf::Vector2f destCoords;
        Thing *mainCharacter;
        bool visible;
        bool active;

        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
#endif // SPICE_SCENE_HPP
