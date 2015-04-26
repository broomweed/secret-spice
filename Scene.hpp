#ifndef SPICE_SCENE_HPP
#define SPICE_SCENE_HPP
#include <vector>
#include <SFML/Graphics.hpp>
#include "Thing.hpp"
#include "TileMap.hpp"

/* This is a scene that contains a series of Things. This is what handles
 * drawing in order of drawDepth. */
class Scene : public sf::Drawable {
    public:
        Scene(Thing* objs_, int size) {
            for (int i = 0; i < size; i++) {
                add(&objs_[i]);
            }
            num = 0;
            visible = false;
        }

        Scene(Thing* objs_, int size, TileMap tilemap_) {
            for (int i = 0; i < size; i++) {
                add(&objs_[i]);
            }
            num = 0;
            tilemap = tilemap_;
            visible = false;
        }

        Scene(TileMap tilemap_) {
            num = 0;
            tilemap = tilemap_;
            visible = false;
        }

        Scene() {
            num = 0;
            visible = false;
        }

        void setName(std::string name_) {
            name = name_;
        }

        int num;

        void add(Thing *obj) {
            objs.push_back(obj);
            obj->parent = this;
            obj->sceneIndex = objs.size()-1;
            for (int i = 0; i < objs.size(); i++) {
                // Swap objects if they passed behind/in front of each other
                if ((obj->getPosition().y < objs[i]->getPosition().y
                            && obj->drawDepth > objs[i]->drawDepth)
                        || (obj->getPosition().y > objs[i]->getPosition().y
                            && obj->drawDepth < objs[i]->drawDepth)) {
                    unsigned int tmp = obj->drawDepth;
                    obj->drawDepth = objs[i]->drawDepth;
                    objs[i]->drawDepth = tmp;
                }
            }
        }

        /* This adds a copy of a GameObject to the scene.
           Useful for things that appear a lot over and over
           and are exactly the same except for position. */
        void add_static(GameObject *obj, sf::Vector2f position) {
            add(new GameObject(obj->getAnimation(), position, obj->boxLoc, 5+num));
            objs[objs.size()-1]->is_copy = true;
            num++;
        }

        void remove(Thing *obj) {
            objs.erase(objs.begin()+obj->sceneIndex);
        }

        std::vector<Thing*> getObjs() {
            return objs;
        }

        void set_mc(Thing *new_mc) {
            main_character = objs[new_mc->sceneIndex];
        }

        Thing *get_mc() {
            return main_character;
        }

        void update() {
            if (visible) {
                last_loop = loop_time.getElapsedTime();
                loop_time.restart();
                for (int i = 0; i < objs.size(); i++) {
                    move_sprite((*objs[i]), objs[i]->getSpeed().x * last_loop.asSeconds() * 30, objs[i]->getSpeed().y * last_loop.asSeconds() * 30);
                    objs[i]->update();
                }
            }
        }

        void move_sprite(Thing& obj, float hmove, float vmove) {
            int index = obj.sceneIndex;
            objs[index]->turn(hmove, vmove);
            if (hmove != 0.0f || vmove != 0.0f) {
                objs[index]->checked = NULL;
                /* Check if object is touching other objects; if it is, don't move it */
                for (int i = 0; i < objs.size(); i++) {
                    if (i == index) continue; // obviously it's always touching itself
                    if (objs[i]->hitTest(sf::Rect<float>(objs[index]->absLoc.left + objs[index]->getSpeed().x,
                            objs[index]->absLoc.top, objs[index]->absLoc.width, objs[index]->absLoc.height))) {
                        hmove = 0.0f;
                        objs[index]->checked = objs[i];
                        objs[i]->onTouch();
                    }
                    if (objs[i]->hitTest(sf::Rect<float>(objs[index]->absLoc.left, objs[index]->absLoc.top + objs[index]->getSpeed().y,
                            objs[index]->absLoc.width, objs[index]->absLoc.height))) {
                        vmove = 0.0f;
                        objs[index]->checked = objs[i];
                        objs[i]->onTouch();
                    }
                }
                objs[index]->move(hmove, vmove);
                for (int i = 0; i < objs.size(); i++) {
                    // Swap objects if they passed behind/in front of each other
                    if ((objs[index]->getPosition().y < objs[i]->getPosition().y
                                && objs[index]->drawDepth > objs[i]->drawDepth)
                            || (objs[index]->getPosition().y > objs[i]->getPosition().y
                                && objs[index]->drawDepth < objs[i]->drawDepth)) {
                        unsigned int tmp = objs[index]->drawDepth;
                        objs[index]->drawDepth = objs[i]->drawDepth;
                        objs[i]->drawDepth = tmp;
                    }
                }
            } else {
                objs[obj.sceneIndex]->stopMoving();
            }
        }

        void set_active() {
            visible = true;
            loop_time.restart();
        }

        void transfer(Scene *scene, sf::Vector2f destCoords) {
            std::cout << "Transferring from " << name << " to " << scene->name << "." << std::endl;
            Thing *mc = get_mc();
            visible = false;
            remove(mc);
            mc->setPosition(destCoords.x, destCoords.y);
            scene->set_active();
            scene->add(mc);
            scene->set_mc(mc);
        }

        // for easy calling of methods on it, this is a public member
        TileMap tilemap;
        std::string name;

        ~Scene() {
            for (int i = 0; i < objs.size(); i++) {
                if (objs[i]->is_copy) {
                    delete objs[i];
                }
            }
        }

    protected:
        std::vector<Thing*> objs;
        sf::Clock loop_time;
        sf::Time last_loop;
        Thing *main_character;
        bool visible;

        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
            if (visible) {
                // draw the tilemap in the background
                target.draw(tilemap);
                /* there is DEFINITELY a more efficient way to do this */
                /* TODO: figure this out. Maybe sort the list of thingies when
                   they are added, and then exchange objects only when they swap depths?
                   (i.e. be the gatekeeper for swapping depths in a scene) But then the
                   drawing depths should probably be contained in this class, not in
                   the objects themselves. Which could be done, with like:
                        struct depthObj { uint depth; Thing* obj; }
                   or something. Then objs would be a std::vector<struct depthObj>. */
                /* (see, I'm learning this c++ stuff!!) */
                /* OR, Or an alternative way to do this would be to have an 'update'
                   function that sorts the list, but is only called when two objects
                   switch depths. This would probably be slower, but it would allow
                   the drawing-depths to stay inside of the objects themselves. */
                /* IDK, I'm more partial to the first one atm. I'll figure it out */
                unsigned int maxDepth = 0;
                for (int i = 0; i < objs.size(); i++) {
                    maxDepth = std::max(maxDepth, objs[i]->drawDepth);
                }
                for (int i = 0; i <= maxDepth; i++) {
                    for (int j = 0; j < objs.size(); j++) {
                        if (objs[j]->drawDepth == i) {
                            target.draw(objs[j]->getSprite(), states);
                        }
                    }
                }
            }
        }
};
#endif // SPICE_SCENE_HPP
